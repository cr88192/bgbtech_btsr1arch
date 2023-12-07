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
	istrWord,	isAltOpB,
	istrJBits,
	istrJ2Bits,
	idRegN,		idRegM,
	idRegO,		idRegP,
	idImm,		idUCmd,
	idUIxt,		idUFl
	);

input			clock;		//clock
input			reset;		//reset
// input			srUser;		//usermode
// input[2:0]		srMod;		//mode
input[7:0]		srMod;		//mode

input[63:0]		istrWord;	//source instruction word
input[3:0]		isAltOpB;
input[27:0]		istrJBits;
input[27:0]		istrJ2Bits;

`output_gpr		idRegN;
`output_gpr		idRegM;
`output_gpr		idRegO;
`output_gpr		idRegP;
output[32:0]	idImm;
output[8:0]		idUCmd;
output[8:0]		idUIxt;
// output[3:0]		idUFl;
// output[7:0]		idUFl;
// output[12:0]		idUFl;
output[18:0]		idUFl;

parameter		fpuLowPrec = 0;

wire			isAltOp;
wire			isAltOpC;
wire			isOp24;
wire			isAltOp96Only;
assign		isAltOp		= isAltOpB[0];
// assign		isOp24		= isAltOpB[1];
assign		isOp24		= 0;
assign		isAltOpC	= isAltOpB[0] && isAltOpB[2];
assign		isAltOp96Only	= isAltOpB[1] && isAltOpC;

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
// reg[3:0]		opULdOp;		//Operation for Load/Store, etc
reg[8:0]		opULdOp;		//Operation for Load/Store, etc
reg[8:0]		opUShufOp;		//Inline Shuffle
reg[5:0]		opULdOp2;		//Operation for Load/Store, etc

assign	idRegN = opRegN;
assign	idRegM = opRegM;
assign	idRegO = opRegO;
assign	idRegP = opRegP;
assign	idImm = opImm;
assign	idUCmd = opUCmd;
assign	idUIxt = opUIxt;
// assign	idUFl = opUFl;
assign	idUFl = { opULdOp2, opULdOp, opUFl };

`reg_gpr	opRegM_Dfl;
`reg_gpr	opRegO_Dfl;
`reg_gpr	opRegN_Dfl;
`reg_gpr	opRegP_Dfl;

`reg_gpr	opRegM_RvoDfl;
`reg_gpr	opRegN_RvoDfl;
`reg_gpr	opRegO_RvoDfl;

`reg_gpr	opRegM_OrgDfl;
`reg_gpr	opRegN_OrgDfl;
`reg_gpr	opRegO_OrgDfl;

`reg_gpr	opRegM_Fix;
`reg_gpr	opRegO_Fix;
`reg_gpr	opRegN_Fix;

`reg_gpr	opRegO_Df2;
reg			opRegO_Df2_IsSP;

`reg_gpr	opRegM_Cr;
`reg_gpr	opRegN_Cr;
// `reg_gpr	opRegO_Cr;

`reg_gpr	opRegM_Sr;
`reg_gpr	opRegN_Sr;

`reg_gpr	opRegM_Rv;
`reg_gpr	opRegN_Rv;
`reg_gpr	opRegO_Rv;

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

reg				opImm_dispAltLr;

reg[32:0]		opImm_disp5u;

reg[32:0]		opImm_imm5u;
reg[32:0]		opImm_imm5n;

reg[15:0]		opImm_imm5f0;
reg[32:0]		opImm_imm5f;

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

reg[2:0]	opCcty_Dfl;


reg[5:0]	opNmid_F0;
reg[4:0]	opFmid_F0;
reg[2:0]	opBty_F0;
reg[3:0]	opIty_F0;
reg[2:0]	opCcty_F0;
reg[5:0]	opUCmdIx_F0;
reg[2:0]	opUCty_F0;

reg[5:0]	opNmid_F1;
reg[4:0]	opFmid_F1;
reg[2:0]	opBty_F1;
reg[3:0]	opIty_F1;
reg[2:0]	opCcty_F1;
reg[5:0]	opUCmdIx_F1;
reg[2:0]	opUCty_F1;

reg[5:0]	opNmid_F2;
reg[4:0]	opFmid_F2;
reg[2:0]	opBty_F2;
reg[3:0]	opIty_F2;
reg[2:0]	opCcty_F2;
reg[5:0]	opUCmdIx_F2;
reg[2:0]	opUCty_F2;

reg[5:0]	opNmid_F3;
reg[4:0]	opFmid_F3;
reg[2:0]	opBty_F3;
reg[3:0]	opIty_F3;
reg[2:0]	opCcty_F3;
reg[5:0]	opUCmdIx_F3;
reg[2:0]	opUCty_F3;

reg[5:0]	opNmid_F8;
reg[4:0]	opFmid_F8;
reg[2:0]	opBty_F8;
reg[3:0]	opIty_F8;
reg[2:0]	opCcty_F8;
reg[5:0]	opUCmdIx_F8;
reg[2:0]	opUCty_F8;

reg[5:0]	opNmid_F9;
reg[4:0]	opFmid_F9;
reg[2:0]	opBty_F9;
reg[3:0]	opIty_F9;
reg[2:0]	opCcty_F9;
reg[5:0]	opUCmdIx_F9;
reg[2:0]	opUCty_F9;

`reg_gpr	opRegM_Fix_F0;
`reg_gpr	opRegO_Fix_F0;
`reg_gpr	opRegN_Fix_F0;

`reg_gpr	opRegM_Fix_F1;
`reg_gpr	opRegO_Fix_F1;
`reg_gpr	opRegN_Fix_F1;

`reg_gpr	opRegM_Fix_F2;
`reg_gpr	opRegO_Fix_F2;
`reg_gpr	opRegN_Fix_F2;

`reg_gpr	opRegM_Fix_F8;
`reg_gpr	opRegO_Fix_F8;
`reg_gpr	opRegN_Fix_F8;

reg[5:0]	opNmid_FA;
reg[4:0]	opFmid_FA;
reg[2:0]	opBty_FA;
reg[3:0]	opIty_FA;
reg[2:0]	opCcty_FA;
reg[5:0]	opUCmdIx_FA;
reg[2:0]	opUCty_FA;


reg[3:0]	opDoImm;

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

// reg[3:0]	opIsImmLdOp;
reg[8:0]	opIsImmLdOp;

reg[8:0]	opIsImmShufOp;

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

reg tRegRmIsRv;
reg tRegRnIsRv;
reg tRegRoIsRv;

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
// reg tBlockIsWex;
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

reg[2:0]	tSelBlock;

`ifdef jx2_enable_xgpr
wire[31:0]	usrRejectCmMask;
wire[31:0]	usrRejectCnMask;

assign	usrRejectCmMask = 32'b1111_1111_1111_1111_1111_1111_0011_1000;
assign	usrRejectCnMask = 32'b1111_1111_1111_1111_1111_1111_1011_1100;

// assign	usrRejectCmMask = 32'b0001_1100_1111_1111_1111_1111_0011_1000;
// assign	usrRejectCnMask = 32'b0011_1101_1111_1111_1111_1111_1011_1100;

`else
wire[15:0]	usrRejectCmMask;
wire[15:0]	usrRejectCnMask;
assign	usrRejectCmMask = 16'b1111_1111_0011_1000;
assign	usrRejectCnMask = 16'b1111_1111_1011_1100;
`endif

reg			usrRejectCmR;
reg			usrRejectCmW;
// reg			usrRejectCoR;
// reg			usrRejectCoW;
reg			usrRejectCnR;
reg			usrRejectCnW;
reg			usrReject;
reg[1:0]	usrSuAllow;
reg			usrSuAllowEn;

reg			usrReject_F0;
reg[1:0]	usrSuAllow_F0;


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
	opIsImmShufOp	= 0;
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

//	opIsXGpr	=
//		(istrWord[15:12]==4'b0111) ||
//		(istrWord[15:12]==4'b1001);

	opIsXGpr	=
		((istrWord[15:12]==4'b0111) ||
		(istrWord[15:12]==4'b1001)) && !srXG2;

//	tOpIsXGprX0	= (istrWord[15:12]==4'b0111);
//	tOpIsXGprX1	= (istrWord[15:12]==4'b1001) &&  istrWord[ 8];
//	tOpIsXGprX2	= (istrWord[15:12]==4'b1001) && !istrWord[ 8];

	tOpIsXGprX0	= (istrWord[15:12]==4'b0111) && !srXG2;
	tOpIsXGprX1	= (istrWord[15:12]==4'b1001) &&  istrWord[ 8] && !srXG2;
	tOpIsXGprX2	= (istrWord[15:12]==4'b1001) && !istrWord[ 8] && !srXG2;

//	opExWN		= istrWord[10] && opIsXGpr;
//	opExWM		= istrWord[ 9] && opIsXGpr;
//	opExWI		= istrWord[ 8] && tOpIsXGprX0;

	opExWN		=
		(istrWord[10] && opIsXGpr) ||
		(!istrWord[15] && srXG2) ||
		(istrJBits[22] && opIsJumboAu) ;
	opExWM		=
		(istrWord[ 9] && opIsXGpr) ||
		(!istrWord[14] && srXG2) ||
		(istrJBits[21] && opIsJumboAu) ;
	opExWI		=
		(istrWord[ 8] && tOpIsXGprX0) ||
		(!istrWord[13] && srXG2) ||
		(istrJBits[20] && opIsJumboAu) ;

`ifndef def_true
	if(srXG2)
	begin
		opIsXGpr		= 0;
		tOpIsXGprX0		= 0;
		tOpIsXGprX1		= 0;
		tOpIsXGprX2		= 0;

		opExWN		= !istrWord[15];
		opExWM		= !istrWord[14];
		opExWI		= !istrWord[13];
	end
`endif

//	opExWIS		= opExWI;
//	if(istrWord[15:12]==4'b1001)
//		opExWI		= 0;

`ifdef def_true
	if(opIsJumboAu)
	begin
		opExWQ		= istrJBits[23];
//		opExWN		= istrJBits[22];
//		opExWM		= istrJBits[21];
//		opExWI		= istrJBits[20];
		opIsExWB2	= istrJBits[19:16];
	end
`endif

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
		opExWN, istrWord[4], istrWord[3:0]};

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
//	opRegO_Cr	= {1'b1, !opExWI, opExI, istrWord[23:20]};

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

	opRegP_Dfl	= opRegN_Dfl;

	opRegN_Sr	= {1'b1, opExN, istrWord[ 7: 4]};
	opRegM_Sr	= {1'b1, opExM, istrWord[ 3: 0]};

	opRegN_Cr	= {1'b1, !opExN, istrWord[ 7: 4]};
	opRegM_Cr	= {1'b1, !opExM, istrWord[ 3: 0]};
//	opRegO_Cr	= {1'b1, !opExI, istrWord[23:20]};

`endif

`ifdef jx2_enable_riscv
	if(opRegM_Cr[4:0]==5'h0F)
		opRegM_Cr = JX2_GR_R15;
	if(opRegN_Cr[4:0]==5'h0F)
		opRegN_Cr = JX2_GR_R15;
`endif

`ifdef def_true
	if(opRegM_Cr[4:0]==5'h1E)
		opRegM_Cr = JX2_GR_R0;
	if(opRegN_Cr[4:0]==5'h1E)
		opRegN_Cr = JX2_GR_R0;

	if(opRegM_Cr[4:0]==5'h1F)
		opRegM_Cr = JX2_GR_R1;
	if(opRegN_Cr[4:0]==5'h1F)
		opRegN_Cr = JX2_GR_R1;
`endif


	opRegO_Df2_IsSP = (opRegO_Df2 == JX2_GR_SP);

	opRegM_RvoDfl = opRegM_Dfl;
	opRegN_RvoDfl = opRegN_Dfl;
	opRegO_RvoDfl = opRegO_Dfl;

`ifdef jx2_enable_xg2rvmode
	tRegRoIsRv	= (opRegO_Dfl[5:4]==2'b00);
	tRegRmIsRv	= (opRegM_Dfl[5:4]==2'b00);
	tRegRnIsRv	= (opRegN_Dfl[5:4]==2'b00);

	case(istrWord[23:20])
		4'b0000: opRegO_Rv = JX2_GR_ZZR;
		4'b0001: opRegO_Rv = JX2_GR_LR;
		4'b0010: opRegO_Rv = JX2_GR_SP;
		4'b0011: opRegO_Rv = JX2_GR_GBR;
		4'b0100: opRegO_Rv = JX2_GR_TBR;
		4'b0101: opRegO_Rv = JX2_GR_DHR;
//		4'b0110: opRegO_Rv = JX2_GR_DLR;
		4'b0110: opRegO_Rv = JX2_GR_R6;
		4'b0111: opRegO_Rv = JX2_GR_R7;
		4'b1000: opRegO_Rv = JX2_GR_R8;
		4'b1001: opRegO_Rv = JX2_GR_R9;
		4'b1010: opRegO_Rv = JX2_GR_R10;
		4'b1011: opRegO_Rv = JX2_GR_R11;
		4'b1100: opRegO_Rv = JX2_GR_R12;
		4'b1101: opRegO_Rv = JX2_GR_R13;
		4'b1110: opRegO_Rv = JX2_GR_R2;
		4'b1111: opRegO_Rv = JX2_GR_R3;
	endcase

	case(istrWord[ 3: 0])
		4'b0000: opRegM_Rv = JX2_GR_ZZR;
		4'b0001: opRegM_Rv = JX2_GR_LR;
		4'b0010: opRegM_Rv = JX2_GR_SP;
		4'b0011: opRegM_Rv = JX2_GR_GBR;
		4'b0100: opRegM_Rv = JX2_GR_TBR;
		4'b0101: opRegM_Rv = JX2_GR_DHR;
//		4'b0110: opRegM_Rv = JX2_GR_DLR;
		4'b0110: opRegM_Rv = JX2_GR_R6;
		4'b0111: opRegM_Rv = JX2_GR_R7;
		4'b1000: opRegM_Rv = JX2_GR_R8;
		4'b1001: opRegM_Rv = JX2_GR_R9;
		4'b1010: opRegM_Rv = JX2_GR_R10;
		4'b1011: opRegM_Rv = JX2_GR_R11;
		4'b1100: opRegM_Rv = JX2_GR_R12;
		4'b1101: opRegM_Rv = JX2_GR_R13;
		4'b1110: opRegM_Rv = JX2_GR_R2;
		4'b1111: opRegM_Rv = JX2_GR_R3;
	endcase

	case(istrWord[ 7: 4])
		4'b0000: opRegN_Rv = JX2_GR_ZZR;
		4'b0001: opRegN_Rv = JX2_GR_LR;
		4'b0010: opRegN_Rv = JX2_GR_SP;
		4'b0011: opRegN_Rv = JX2_GR_GBR;
		4'b0100: opRegN_Rv = JX2_GR_TBR;
		4'b0101: opRegN_Rv = JX2_GR_DHR;
//		4'b0110: opRegN_Rv = JX2_GR_DLR;
		4'b0110: opRegN_Rv = JX2_GR_R6;
		4'b0111: opRegN_Rv = JX2_GR_R7;
		4'b1000: opRegN_Rv = JX2_GR_R8;
		4'b1001: opRegN_Rv = JX2_GR_R9;
		4'b1010: opRegN_Rv = JX2_GR_R10;
		4'b1011: opRegN_Rv = JX2_GR_R11;
		4'b1100: opRegN_Rv = JX2_GR_R12;
		4'b1101: opRegN_Rv = JX2_GR_R13;
		4'b1110: opRegN_Rv = JX2_GR_R2;
		4'b1111: opRegN_Rv = JX2_GR_R3;
	endcase

	if(srXGRV)
	begin
		if(tRegRoIsRv)
			opRegO_Dfl = opRegO_Rv;
		if(tRegRmIsRv)
			opRegM_Dfl = opRegM_Rv;
		if(tRegRnIsRv)
			opRegN_Dfl = opRegN_Rv;

		opRegP_Dfl	= opRegN_Dfl;

		opRegO_Df2_IsSP = 0;
	end
`endif


`ifdef jx2_enable_xgpr
//	opImm_imm5u	= {UV27_00, opRegO_Dfl[5:0]};
//	opImm_imm5n	= {UV27_FF, opRegO_Dfl[5:0]};

	opImm_imm5u	= {UV27_00, opRegO_RvoDfl[5:0]};
	opImm_imm5n	= {UV27_FF, opRegO_RvoDfl[5:0]};

//	opImmRm_imm6u	= {UV27_00, opRegM_Dfl[5:0]};
//	opImmRm_imm6n	= {UV27_FF, opRegM_Dfl[5:0]};

//	opImm_disp5u	= opImm_imm5u;
	opImm_disp5u	= {opExWI ? UV27_FF : UV27_00, opRegO_Dfl[5:0]};
	
	opImm_imm5u		= opImm_disp5u;
`else
//	opImm_imm5u	= {UV28_00, opRegO_Dfl[4:0]};
//	opImm_imm5n	= {UV28_FF, opRegO_Dfl[4:0]};
	opImm_imm5u	= {UV28_00, opRegO_RvoDfl[4:0]};
	opImm_imm5n	= {UV28_FF, opRegO_RvoDfl[4:0]};
	opImm_disp5u	= opImm_imm5u;

//	opImmRm_imm6u	= {UV28_00, opRegM_Dfl[4:0]};
//	opImmRm_imm6n	= {UV28_FF, opRegM_Dfl[4:0]};
`endif

	opImmRm_imm6u	= {UV27_00, istrWord[25:24], istrWord[ 3: 0]};
	opImmRm_imm6n	= {UV27_FF, istrWord[25:24], istrWord[ 3: 0]};

	//0.011-01.00-zzzz-zzzz
	//0.011-10.00-zzzz-zzzz
	//0.011-11.00-zzzz-zzzz

`ifndef def_true
// `ifdef def_true
	case(opRegO_Dfl[4:0])
		5'h00: opImm_imm5f0 = 16'h3000;		5'h01: opImm_imm5f0 = 16'h3100;
		5'h02: opImm_imm5f0 = 16'h3200;		5'h03: opImm_imm5f0 = 16'h3300;
		5'h04: opImm_imm5f0 = 16'h3400;		5'h05: opImm_imm5f0 = 16'h3500;
		5'h06: opImm_imm5f0 = 16'h3600;		5'h07: opImm_imm5f0 = 16'h3700;
		5'h08: opImm_imm5f0 = 16'h3800;		5'h09: opImm_imm5f0 = 16'h3900;
		5'h0A: opImm_imm5f0 = 16'h3A00;		5'h0B: opImm_imm5f0 = 16'h3B00;
		5'h0C: opImm_imm5f0 = 16'h3C00;		5'h0D: opImm_imm5f0 = 16'h3D00;
		5'h0E: opImm_imm5f0 = 16'h3E00;		5'h0F: opImm_imm5f0 = 16'h3F00;
		5'h10: opImm_imm5f0 = 16'h4000;		5'h11: opImm_imm5f0 = 16'h4100;
		5'h12: opImm_imm5f0 = 16'h4200;		5'h13: opImm_imm5f0 = 16'h4300;
		5'h14: opImm_imm5f0 = 16'h4400;		5'h15: opImm_imm5f0 = 16'h4500;
		5'h16: opImm_imm5f0 = 16'h4600;		5'h17: opImm_imm5f0 = 16'h4700;
		5'h18: opImm_imm5f0 = 16'h4800;		5'h19: opImm_imm5f0 = 16'h4900;
		5'h1A: opImm_imm5f0 = 16'h4A00;		5'h1B: opImm_imm5f0 = 16'h4B00;
		5'h1C: opImm_imm5f0 = 16'h4C00;		5'h1D: opImm_imm5f0 = 16'h4D00;
		5'h1E: opImm_imm5f0 = 16'h4E00;		5'h1F: opImm_imm5f0 = 16'h4F00;
	endcase
	opImm_imm5f = { UV17_00, opImm_imm5f0 };
`endif

`ifdef def_true
	opImm_imm5f = { UV17_00,
		opRegO_Dfl[4] ? 4'h4 : 4'h3,
		opRegO_Dfl[3:0],
		8'h00 };
`endif

//	opIsNotFx	= (istrWord[15:13]!=3'b111);
	opIsNotFx	= ((istrWord[15:13]!=3'b111) && !srXG2) || srRV;
	
	tNextMsgLatch	= 0;
	opImm_imm8au	= 0;
	opImm_imm8as	= opImm_imm5u;

	opImm_disp11as	= 0;
//	opImm_disp9as	= 0;
	opImm_dispasc	= 0;

`ifdef jx2_use_imm_shuffle
	if(opIsJumbo96)
	begin
		opIsImmShufOp = { 1'b1,
			istrJ2Bits[11:10],
			istrJBits [21:20],
			istrJBits [ 7: 6],
			istrWord  [17:16]
		};
	end
`endif

//	opImm_disp20s = { istrWord[7] ? UV13_FF : UV13_00,
//		istrWord[7:0], istrWord[27:16] };

	opImm_disp20s = { istrWord[7] ? UV10_FF : UV10_00,
		istrWord[7] ^ opExWN,
		istrWord[7] ^ opExWM,
		istrWord[7] ^ opExWI,
		istrWord[7:0], istrWord[27:16] };
	opImm_dispAltLr	= 0;

	if(opIsJumboAu)
	begin
//		opRegO_Df2[5]	= opExWN;

`ifdef jx2_enable_xgpr
		if(opExWN)
		begin
			opRegO_Df2[6:5]	= 2'b01;
		end
`endif

		opRegImm16 = JX2_GR_IMM;
		opRegImm10 = opIsJumbo96 ? JX2_GR_JIMM : JX2_GR_IMM;
	
		opImm_disp20s	= { istrJBits[11], istrJBits[11:0],
			istrWord[7:0], istrWord[27:16] };

//		opImm_disp20s	= { opExWI, istrJBits[11:0],
//			istrWord[7:0], istrWord[27:16] };

//		opImm_imm9s		= { opExI ? UV17_FF : UV17_00,
//			istrJBits[7:0], istrWord [23:16] };
//		opImm_imm9u		= opImm_imm9s;
//		opImm_imm9n		= opImm_imm9s;
//		opImm_imm9u		= { UV16_00, opExI, istrJBits[7:0], istrWord [23:16] };

		opImm_imm9s		= { opExWI ? UV16_FF : UV16_00,
			opExI, istrJBits[7:0], istrWord [23:16] };
		opImm_imm9u		= opImm_imm9s;
		opImm_imm9n		= opImm_imm9s;
//		opImm_imm9n		= { UV16_FF, opExI, istrJBits[7:0], istrWord [23:16] };

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
//		opIsImmLdOp	= istrJBits[15:12];
		opIsImmLdOp	= { 5'h0, istrJBits[15:12] };

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

`ifdef jx2_use_imm_shuffle
//			opIsImmShufOp = { 1'b1, istrJBits [ 7: 0] };
			opIsImmShufOp = {
				istrJBits[11] && (istrJBits[10:9] != 2'b00),
				istrJBits [ 7: 0] };
`endif

//			opImm_disp5u	= {
//				istrJBits[8] ? UV24_FF : UV24_FF, 
//				istrJBits[8:0] };
		end

		opImm_disp13s   = { opExI, istrJBits[19:0],
			istrWord[7:0], istrWord[23:20] };

		opImm_imm8au	= { UV25_00, istrJBits[7:0] };
//		opImm_imm8as	= { istrJBits[7] ? UV25_FF : UV25_FF, istrJBits[7:0] };
`ifdef jx2_enable_xgpr
		opRegP_Dfl		= { 1'b0, istrJBits[5:0] };
`else
		opRegP_Dfl		= { 1'b0, istrJBits[4:0] };
`endif

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
	
//		opImm_disp20s	= { istrJBits[11], istrJBits[11:0],
//			istrWord[7:0], istrWord[27:16] };

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

//		opImm_disp20s = { istrWord[7] ? UV13_FF : UV13_00,
//			istrWord[7:0], istrWord[27:16] };

//		opImm_imm9u	= {UV24_00, istrWord[24:16]};
//		opImm_imm9n	= {UV24_FF, istrWord[24:16]};
//		opImm_imm9s	= opImm_imm9u;
//		opImm_disp9s	= opImm_imm9u;
		opImm_disp9s	= opExWI ? opImm_imm9n : opImm_imm9u;

		opImm_imm9u	= {UV23_00,  opExWI, istrWord[24:16]};
		opImm_imm9n	= {UV23_FF, !opExWI, istrWord[24:16]};
		opImm_imm9s	= opImm_imm9u;
		opImm_disp9s	= { opExWI ? UV24_FF : UV24_00, istrWord[24:16] };

//		opImm_imm10u	= {UV23_00, istrWord[25:16]};
//		opImm_imm10n	= {UV23_FF, istrWord[25:16]};

		opImm_imm10u	= {UV22_00,  opExWI, istrWord[25:16]};
		opImm_imm10n	= {UV22_FF, !opExWI, istrWord[25:16]};

//		opImm_imm10s	= {istrWord[25]?UV23_FF:UV23_00, istrWord[25:16]};

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

	if(srXG2)
	begin
		/* If XG2, Disable LSB of branch ops.
		   The LSB will instead encode an Alt-LR flag.
		 */
		opImm_dispAltLr		= opImm_disp20s[0];
		opImm_disp20s[0]	= 0;
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
//	usrRejectCoR	= usrRejectCmMask[opRegO_Cr[4:0]];
//	usrRejectCoW	= usrRejectCnMask[opRegO_Cr[4:0]];
	usrRejectCnR	= usrRejectCmMask[opRegN_Cr[4:0]];
	usrRejectCnW	= usrRejectCnMask[opRegN_Cr[4:0]];
`else
	usrRejectCmR	= usrRejectCmMask[opRegM_Cr[3:0]];
	usrRejectCmW	= usrRejectCnMask[opRegM_Cr[3:0]];
//	usrRejectCoR	= usrRejectCmMask[opRegO_Cr[3:0]];
//	usrRejectCoW	= usrRejectCnMask[opRegO_Cr[3:0]];
	usrRejectCnR	= usrRejectCmMask[opRegN_Cr[3:0]];
	usrRejectCnW	= usrRejectCnMask[opRegN_Cr[3:0]];
`endif


	opRegM_OrgDfl	= opRegM_Dfl;
	opRegN_OrgDfl	= opRegN_Dfl;
	opRegO_OrgDfl	= opRegO_Dfl;

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

	opRegN		= JX2_GR_ZZR;
	opRegM		= JX2_GR_ZZR;
	opRegO		= JX2_GR_ZZR;
	opRegP		= JX2_GR_ZZR;
	opDoImm		= JX2_FMIMM_NONE;
	opImm		= 0;
	opUIxt		= 0;
	opCcty		= JX2_IXC_AL;


	opNmid		= JX2_UCMD_INVOP;
	opFmid		= JX2_FMID_INV;
	opUCmdIx	= 0;
	opBty		= 0;
	opIty		= 0;
	opUCty		= JX2_IUC_SC;
	opRegM_Fix	= JX2_GR_ZZR;
	opRegO_Fix	= JX2_GR_ZZR;
	opRegN_Fix	= JX2_GR_ZZR;

	opUShufOp	= 0;

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
//	tBlockIsWex		= istrWord[10];

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

		tBlockIsF8 = 0;
		tBlockIsF9 = 0;
		tBlockIsFA = 0;
		tBlockIsFB = 0;

		tBlockIsFE = 0;
		tBlockIsFF = 0;
	end

`ifndef def_true
	if(opIsExWB2!=0)
	begin
		tBlockIsF0 = 0;
		tBlockIsF1 = 0;
		tBlockIsF2 = 0;
		tBlockIsF3 = 0;

		tBlockIsF8 = 0;
		tBlockIsF9 = 0;
	end
`endif

`ifndef jx2_enable_2x40b
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
`endif

	if(tOpIsXGprX0)
		tBlockIsF0 = 1;
	if(tOpIsXGprX1)
		tBlockIsF1 = 1;
	if(tOpIsXGprX2)
		tBlockIsF2 = 1;

//	if(opIsXGpr)
//		tBlockIsWex		= istrWord[11];

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

//	tBlockIsEz		= (istrWord[15:12] == 4'b1110);
//	if(tOpIsXGprX2)
//		tBlockIsEz	= !istrWord[12];

	tBlockIsEz	= !istrWord[12] && !opIsNotFx;

//	tBlockIsEA_09	= !istrWord[31] || (istrWord[30:29]==0);
	tBlockIsEA_09	= !istrWord[31];
//	tBlockIsEA_F0	= tBlockIsEz && tBlockIsFA && tBlockIsEA_09;
//	tBlockIsEA_F2	= tBlockIsEz && tBlockIsFA && !tBlockIsEA_09;
//	tBlockIsEA_F0	= tBlockIsEz && tBlockIsPrWx && tBlockIsEA_09;
//	tBlockIsEA_F2	= tBlockIsEz && tBlockIsPrWx && !tBlockIsEA_09;

	tBlockIsEA_F0	= tBlockIsEz && tBlockIsPrWxA;
	tBlockIsEA_F2	= tBlockIsEz && tBlockIsPrWxB;

	if(isAltOp96Only)
	begin
		tBlockIsF0 = 0;
		tBlockIsF1 = 0;
		tBlockIsF2 = 0;

		tBlockIsEA_F0	= 0;
		tBlockIsEA_F2	= 0;
	end

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

	opCcty_Dfl = opCcty;

//	if(tBlockIsF0 || tBlockIsEA_F0)

	if(1'b1)
	begin
		opNmid		= JX2_UCMD_INVOP;
		opFmid		= JX2_FMID_INV;
		opUCmdIx	= 0;
		opBty		= 0;
		opIty		= 0;
		opUCty		= JX2_IUC_SC;
		opCcty		= JX2_IXC_AL;
		opRegM_Fix	= JX2_GR_ZZR;
		opRegO_Fix	= JX2_GR_ZZR;
		opRegN_Fix	= JX2_GR_ZZR;

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

`ifndef def_true
//				if(	(opRegM_Dfl[4:0] == 5'h00) &&
//					(opRegN_Dfl[4:0] == 5'h00) )
				if(opRegN_Dfl[4:0] == 5'h00)
				begin
					opNmid		= JX2_UCMD_OP_IXT;
					opFmid		= JX2_FMID_Z;
					opUCmdIx	= JX2_UCIX_IXT_BREAK;
				end
`endif
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
			end

			16'h1zz4: begin		/* F0nm_1eo4 */
`ifndef def_true
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
`endif

				opNmid		= JX2_UCMD_UNARY;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALUN_MINQ;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_ALUN_MAXQ;
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
			end

			16'h1zz8: begin	/* F0nm_1ez8 */
				case(istrWord[23:20])
					4'h0: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_BNDCHKB;

						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
//							opNmid		= JX2_UCMD_INVOP;
//							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
					end

					4'h1: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_BNDCHKW;

						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
//							opNmid		= JX2_UCMD_INVOP;
//							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
					end

					4'h2: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_BNDCHKL;

						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
//							opNmid		= JX2_UCMD_INVOP;
//							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
					end

					4'h3: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_BNDCHKQ;

						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
//							opNmid		= JX2_UCMD_INVOP;
//							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
					end


					4'h4: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
//						opIty		= JX2_ITY_NB;
						opIty		= JX2_ITY_XL;
						opUCmdIx	= JX2_UCIX_IXT_BNDCHKB;

						if(opExQ)
						begin
							opNmid		= JX2_UCMD_INVOP;
							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
					end

					4'h5: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
//						opIty		= JX2_ITY_NB;
						opIty		= JX2_ITY_XL;
						opUCmdIx	= JX2_UCIX_IXT_BNDCMPB;

						if(opExQ)
						begin
							opNmid		= JX2_UCMD_INVOP;
							opUCmdIx	= JX2_UCIX_IXT_BREAK;
						end
					end

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
//						opNmid		= JX2_UCMD_ALUW3;
//						opUCmdIx	= JX2_UCIX_ALUW_MOVTA16;
//						opFmid		= JX2_FMID_REGIMMREG;
//						opIty		= JX2_ITY_SB;

						opNmid		= JX2_UCMD_CONV_RR;
						opUCmdIx	= JX2_UCIX_CONV_MOVZT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;

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
`ifdef jx2_gprs_mergecm
						opNmid		= JX2_UCMD_MOV_RR;
`endif
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
//						usrReject	= 1;
						if(opExQ)
						begin
							opNmid	= JX2_UCMD_NOP;
						end
					end
					4'hB: begin
						opNmid	= JX2_UCMD_MOV_CR;
						opFmid	= JX2_FMID_REGREG;
						opIty	= JX2_ITY_UQ;
//						usrReject	= 1;
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
//						opNmid		= JX2_UCMD_ALUW3;
						opNmid		= JX2_UCMD_ALUCMPW;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALUW_PCMPEQ;
					end
					4'hD: begin
//						opNmid		= JX2_UCMD_ALUW3;
						opNmid		= JX2_UCMD_ALUCMPW;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALUW_PCMPHI;
					end
					4'hE: begin
//						opNmid		= JX2_UCMD_ALUW3;
						opNmid		= JX2_UCMD_ALUCMPW;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALUW_PCMPGT;
					end

					4'hF: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV_BSWAPL;
						if(opExQ)
						begin
							opUCmdIx	= JX2_UCIX_CONV_BSWAPQ;
						end
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

`ifdef jx2_enable_rgb5minmax_alu
						if(opExQ)
						begin
							opNmid		= JX2_UCMD_CONV2_RR;
							opUCmdIx	= JX2_UCIX_CONV2_RGB5MINMAX;
						end
`endif

`ifdef jx2_enable_rgb5minmax_mul
						if(opExQ)
						begin
							opNmid		= JX2_UCMD_MUL3;
							opUCmdIx	= JX2_UCIX_MUL3_RGB5MINMAX;
						end
`endif
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
							opUCmdIx	= JX2_UCIX_CONV2_RGB32UPCK64FU;
						else
							opUCmdIx	= JX2_UCIX_CONV2_RGB32UPCK64FS;
					end
					4'h9: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV2_RGB30AUPCK64F;
						if(opExQ)
						begin
							opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32L;
							opUCty		= JX2_IUC_WX;
						end
					end
					4'hA: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_RGB32PCK64FU;
						else
							opUCmdIx	= JX2_UCIX_CONV2_RGB32PCK64FS;
					end
					4'hB: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV2_RGB30APCK64F;
						if(opExQ)
						begin
//							opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32L;
							opUCmdIx	= JX2_UCIX_CONV2_FP16PCK32;
							opUCty		= JX2_IUC_WX;
						end
					end

`ifdef jx2_enable_convfp16
					4'hC: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32H;
						else
							opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32L;
					end

					4'hD: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_FP16EUPCK32H;
						else
							opUCmdIx	= JX2_UCIX_CONV2_FP16EUPCK32L;
					end

					4'hE: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV2_FP16PCK32;
						if(opExQ)
						begin
//							opNmid		= JX2_UCMD_CONV_RR;
//							opUCmdIx	= JX2_UCIX_CONV_BSWAPQ;
						end
					end
`endif

					4'hF: begin		/* F0nm_1eFE */
						opNmid		= JX2_UCMD_CONV_RR;
						opUCmdIx	= JX2_UCIX_CONV_MOVST;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;

						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end

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

`ifdef jx2_enable_convfp16al
					4'hC: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_FP16PCKAL;
						else
							opUCmdIx	= JX2_UCIX_CONV2_FP16PCKAL;
					end
					4'hD: begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						if(opExQ)
							opUCmdIx	= JX2_UCIX_CONV2_FP16UPCKAL;
						else
							opUCmdIx	= JX2_UCIX_CONV2_FP16UPCKAL;
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
				opUShufOp	= opIsImmShufOp;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_PADDX;
					opUCty		= JX2_IUC_WX;
				end

`ifdef jx2_use_fpu_v4sf

`ifdef jx2_use_fpu_w
				if(opIsImmShufOp[8])
					opNmid		= JX2_UCMD_FPUV4SF;
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`else
				opNmid		= JX2_UCMD_FPUV4SF;
`endif

`ifdef jx2_fpu_v4sf_fullsp
				opNmid		= JX2_UCMD_FPUV4SF;
`endif
`endif
			end
			16'h2zz6: begin		/* F0nm_2eo6 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_PSUB;
				opUShufOp	= opIsImmShufOp;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_PSUBX;
					opUCty		= JX2_IUC_WX;
				end

`ifdef jx2_use_fpu_v4sf

`ifdef jx2_use_fpu_w
				if(opIsImmShufOp[8])
					opNmid		= JX2_UCMD_FPUV4SF;
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`else
				opNmid		= JX2_UCMD_FPUV4SF;
`endif

`ifdef jx2_fpu_v4sf_fullsp
				opNmid		= JX2_UCMD_FPUV4SF;
`endif
`endif
			end
			16'h2zz7: begin		/* F0nm_2eo7 */
				opNmid		= JX2_UCMD_FPU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_PMUL;
				opUShufOp	= opIsImmShufOp;
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_FPU_PMULX;
					opUCty		= JX2_IUC_WX;
				end

`ifdef jx2_use_fpu_v4sf

`ifdef jx2_use_fpu_w
				if(opIsImmShufOp[8])
					opNmid		= JX2_UCMD_FPUV4SF;
				if(opImm_disp11as[7:2]==6'h02)
					opNmid		= JX2_UCMD_FPUV4SF;
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`else
				opNmid		= JX2_UCMD_FPUV4SF;
`endif

`ifdef jx2_fpu_v4sf_fullsp
				opNmid		= JX2_UCMD_FPUV4SF;
`endif
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

`ifdef jx2_use_fpu_w
`ifdef jx2_use_fpu_v2sd
					if(opImm_disp11as[7:2]==6'h02)
						opNmid		= JX2_UCMD_FPUV4SF;
					if(fpuLowPrec)
						opNmid		= JX2_UCMD_FPUV4SF;
`endif
`else
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

`ifdef jx2_use_fpu_w
`ifdef jx2_use_fpu_v2sd
					if(opImm_disp11as[7:2]==6'h02)
						opNmid		= JX2_UCMD_FPUV4SF;
					if(fpuLowPrec)
						opNmid		= JX2_UCMD_FPUV4SF;
`endif
`else
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

`ifdef jx2_use_fpu_w
`ifdef jx2_use_fpu_v2sd
					if(opImm_disp11as[7:2]==6'h02)
						opNmid		= JX2_UCMD_FPUV4SF;
					if(fpuLowPrec)
						opNmid		= JX2_UCMD_FPUV4SF;
`endif
`else
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
//								opNmid		= JX2_UCMD_NOP;
//								opFmid		= JX2_FMID_Z;
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_BREAK;
							end
							4'h1: begin
								opNmid		= JX2_UCMD_JMP;
								opFmid		= JX2_FMID_Z;
								opRegM_Fix	= JX2_GR_LR;
							end

							4'h2: begin
								opNmid		= JX2_UCMD_OP_IXT;
								opFmid		= JX2_FMID_Z;
								opUCmdIx	= JX2_UCIX_IXT_SYSE;
								opRegM_Fix	= JX2_GR_DLR;
							end

							4'h3: begin
								opNmid		= JX2_UCMD_NOP;
								opFmid		= JX2_FMID_Z;
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
						opIty		= JX2_ITY_SW;
						opBty		= JX2_BTY_SW;
					end

`ifndef def_true
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
`endif

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

`ifndef def_true
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
						opIty		= JX2_ITY_SW;
						opBty		= JX2_BTY_SL;
					end
`endif

`ifndef def_true
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

						if(opExQ)
						begin
							opNmid		= JX2_UCMD_NOP;
							opFmid		= JX2_FMID_Z;
						end
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
//						opIty		= JX2_ITY_SB;
						opIty		= JX2_ITY_XB;
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
//						usrReject	= 1;
//						usrReject	= !opExI;
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
						if(opExQ)
						begin
							opNmid		= JX2_UCMD_NOP;
							opFmid		= JX2_FMID_Z;
						end
					end

					default: begin
					end

				endcase
			end

			16'h3zz1: begin		/* F0nm_3eo1 */
				opNmid		= JX2_UCMD_ALUW3;
				opUCmdIx	= JX2_UCIX_ALUW_MOVTA16;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SQ;
//				if(opExQ && !opIsJumbo)
				if(opExQ && !opIsJumbo && !opIsJumboAu)
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

`ifndef def_true
			16'h3zz7: begin
				opNmid		= JX2_UCMD_SWAPN;
				opFmid		= JX2_FMID_REGREG;
//				opUCmdIx	= { 2'h0, istrWord[3:0] };
				opUCmdIx	= { 2'h0, istrWord[23:20] };
			end
`endif

			16'h3zz8: begin	/* F0nm_3ez8 */
				case(istrWord[23:20])
					4'h0: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_BNDCMPB;

						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
						end
					end

					4'h1: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_BNDCMPW;

						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
						end
					end

					4'h2: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_BNDCMPL;

						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
						end
					end

					4'h3: begin
						opNmid		= JX2_UCMD_OP_IXT;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_IXT_BNDCMPQ;

						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
						end
					end

					4'h4: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV_SWAPE2B;
						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
						end
					end

					4'h5: begin
						opNmid		= JX2_UCMD_CONV_RR;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_UB;
						opUCmdIx	= JX2_UCIX_CONV_SWAPE4B;
						if(opExQ)
						begin
//							opIty		= JX2_ITY_XL;
						end
					end

`ifdef jx2_enable_aluptr
					4'h6: begin
						opNmid		= JX2_UCMD_ALUCMPW;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALUW_CMPEQP;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end
					4'h7: begin
						opNmid		= JX2_UCMD_ALUCMPW;
						opFmid		= JX2_FMID_REGREG;
						opIty		= JX2_ITY_NB;
						opUCmdIx	= JX2_UCIX_ALUW_CMPGTP;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end
`endif

					default: begin end
				endcase
			end

			16'h4zz0: begin		/* F0nm_4eo0 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opBty	= JX2_BTY_UQ;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
//					opBty	= JX2_BTY_SQ;
//					opIty	= JX2_ITY_UL;

					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opBty	= JX2_BTY_UB;
					opIty	= JX2_ITY_NB;
				end
			end
			16'h4zz1: begin		/* F0nm_4eo1 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
//				opBty	= JX2_BTY_UQ;
//				opIty	= JX2_ITY_UB;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UL;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opBty	= JX2_BTY_UW;
					opIty	= JX2_ITY_NB;
				end
			end

			16'h4zz2: begin		/* F0nm_4eo2 */
//				opNmid	= JX2_UCMD_MOV_RM;
//				opFmid	= JX2_FMID_REGSTREGDISP;
//				opBty	= JX2_BTY_SQ;
//				opIty	= JX2_ITY_UL;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opBty	= JX2_BTY_UL;
					opIty	= JX2_ITY_NB;
				end
			end

			16'h4zz3: begin		/* F0nm_4eo3 */
//				opNmid	= JX2_UCMD_MOV_RM;
//				opFmid	= JX2_FMID_REGSTREGDISP;
//				opBty	= JX2_BTY_SQ;
//				opIty	= JX2_ITY_UL;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opBty	= JX2_BTY_UQ;
					opIty	= JX2_ITY_NB;
				end
			end

`ifndef def_true
			16'h4zz3: begin		/* F0nm_4eo3 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UL;

				if(opExQ)
				begin
				end
			end
`endif
			16'h4zz4: begin		/* F0nm_4eo4 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_UQ;
				opIty	= JX2_ITY_UB;
				if(opExQ)
				begin
//					opBty	= JX2_BTY_SQ;
//					opIty	= JX2_ITY_UL;

					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opBty	= JX2_BTY_UB;
					opIty	= JX2_ITY_NB;
				end
			end

			16'h4zz5: begin		/* F0nm_4eo4 */
				opNmid	= JX2_UCMD_MOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
//				opBty	= JX2_BTY_UQ;
//				opIty	= JX2_ITY_UB;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UL;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opBty	= JX2_BTY_UW;
					opIty	= JX2_ITY_NB;
				end
			end

			16'h4zz6: begin		/* F0nm_4eo6 */
`ifdef jx2_enable_fmov
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
`endif
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opBty	= JX2_BTY_UL;
					opIty	= JX2_ITY_NB;
				end
			end

			16'h4zz7: begin		/* F0nm_4eo7 */
`ifdef jx2_enable_fmov
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
`endif
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opBty	= JX2_BTY_UQ;
					opIty	= JX2_ITY_NB;
				end
			end

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

			16'h4zzA: begin		/* F0nm_4eoA */
`ifdef jx2_enable_fmov
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
`endif

				if(opExQ)
				begin
`ifdef jx2_enable_pmov
					opNmid	= JX2_UCMD_PMOV_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opBty	= JX2_BTY_SL;
					opIty	= JX2_ITY_NL;
`else
					opNmid		= JX2_UCMD_INVOP;
`endif
				end
			end

			16'h4zzB: begin		/* F0nm_4eoB */
`ifdef jx2_enable_fmov
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
`endif

				if(opExQ)
				begin
`ifdef jx2_enable_pmov
					opNmid	= JX2_UCMD_PMOV_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opBty	= JX2_BTY_SB;
					opIty	= JX2_ITY_NL;
`else
					opNmid		= JX2_UCMD_INVOP;
`endif
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
					opNmid		= JX2_UCMD_INVOP;
				end

`ifndef def_true
				if(opExQ)
				begin
//					opBty	= JX2_BTY_SQ;
//					opIty	= JX2_ITY_UL;

`ifdef jx2_ena_fpu_v2sd
`ifdef jx2_use_fpu_v4sf
					opNmid		= JX2_UCMD_FPUV4SF;
`else
					opNmid		= JX2_UCMD_FPU3;
`endif
`else
					opNmid		= JX2_UCMD_FPU3;
`endif

					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_FPU_FADD;
				end
`endif
			end

			16'h4zzD: begin		/* F0nm_4eoD */
				//...
//				opNmid		= JX2_UCMD_INVOP;

				opNmid	= JX2_UCMD_MOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SQ;
				opIty	= JX2_ITY_UL;

`ifndef def_true
				if(opExQ)
				begin
`ifdef jx2_ena_fpu_v2sd
`ifdef jx2_use_fpu_v4sf
					opNmid		= JX2_UCMD_FPUV4SF;
`else
					opNmid		= JX2_UCMD_FPU3;
`endif
`else
					opNmid		= JX2_UCMD_FPU3;
`endif

					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_FPU_FSUB;
				end
`endif
			end

			16'h4zzE: begin		/* F0nm_4eoE */
`ifdef jx2_enable_fmov
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SL;
				opIty	= JX2_ITY_UB;
`endif

				if(opExQ)
				begin
`ifdef jx2_enable_pmov
					opNmid	= JX2_UCMD_PMOV_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opBty	= JX2_BTY_SL;
					opIty	= JX2_ITY_NL;
`else
					opNmid		= JX2_UCMD_INVOP;
`endif
				end

`ifndef def_true
				if(opExQ)
				begin
`ifdef jx2_ena_fpu_v2sd
`ifdef jx2_use_fpu_v4sf
					opNmid		= JX2_UCMD_FPUV4SF;
`else
					opNmid		= JX2_UCMD_FPU3;
`endif
`else
					opNmid		= JX2_UCMD_FPU3;
`endif

					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_FPU_FMUL;
				end
`endif
			end

			16'h4zzF: begin		/* F0nm_4eoF */
`ifdef jx2_enable_fmov
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_SW;
				opIty	= JX2_ITY_UB;
`endif

				if(opExQ)
				begin
`ifdef jx2_enable_pmov
					opNmid	= JX2_UCMD_PMOV_MR;
					opFmid	= JX2_FMID_LDDRREGREG;
					opBty	= JX2_BTY_SB;
					opIty	= JX2_ITY_NL;
`else
					opNmid		= JX2_UCMD_INVOP;
`endif
				end
			end

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
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
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
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
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
//				if(fpuLowPrec)
//					opNmid		= JX2_UCMD_FPUV4SF;
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
`ifdef jx2_use_fpu_fpimm
				if(opExQ)
				begin
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_NB;
				end
`endif

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
`ifdef jx2_use_fpu_fpimm
				if(opExQ)
				begin
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_NB;
				end
`endif

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

`ifdef jx2_use_fpu_fpimm
				if(opExQ)
				begin
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_NB;
				end
`endif

`ifdef jx2_use_fpu_v2sd
				if(fpuLowPrec)
					opNmid		= JX2_UCMD_FPUV4SF;
`endif
			end


			16'h7zz0: begin		/* F0nm_7eo0 */
				opNmid		= JX2_UCMD_FPU3;

`ifdef jx2_use_fpu_v4sf
`ifdef jx2_ena_fpu_v2sd
// `ifdef jx2_use_fpu_v4sf
`ifdef jx2_fpu_v4sf_fullsp
				opNmid		= JX2_UCMD_FPUV4SF;
`else
				opNmid		= JX2_UCMD_FPU3;
`endif
`else
				opNmid		= JX2_UCMD_FPU3;
`endif
`endif

				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FADD;

				if(opExQ)
				begin
					opNmid		= JX2_UCMD_UNARY;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_ALUN_FMIN;
				end
			end

			16'h7zz1: begin		/* F0nm_7eo1 */
				opNmid		= JX2_UCMD_FPU3;

`ifdef jx2_use_fpu_v4sf
`ifdef jx2_ena_fpu_v2sd
// `ifdef jx2_use_fpu_v4sf
`ifdef jx2_fpu_v4sf_fullsp
				opNmid		= JX2_UCMD_FPUV4SF;
`else
				opNmid		= JX2_UCMD_FPU3;
`endif
`else
				opNmid		= JX2_UCMD_FPU3;
`endif
`endif

				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FSUB;

				if(opExQ)
				begin
					opNmid		= JX2_UCMD_UNARY;
					opFmid		= JX2_FMID_REGREG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_ALUN_FMAX;
				end
			end

			16'h7zz2: begin		/* F0nm_7eo2 */
				opNmid		= JX2_UCMD_FPU3;

`ifdef jx2_use_fpu_v4sf
`ifdef jx2_ena_fpu_v2sd
// `ifdef jx2_use_fpu_v4sf
`ifdef jx2_fpu_v4sf_fullsp
				opNmid		= JX2_UCMD_FPUV4SF;
`else
				opNmid		= JX2_UCMD_FPU3;
`endif
`else
				opNmid		= JX2_UCMD_FPU3;
`endif
`endif

				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_FPU_FMUL;
			end

`ifdef jx2_alu_slomuldiv
			16'h7zz4: begin		/* F0nm_7eo4 */
				opNmid		= JX2_UCMD_QMULDIV;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_QMUL_DIVSL;
//				opUCmdIx	= JX2_UCIX_QMUL_DIVS;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_QMUL_DIVUL;
//					opUCmdIx	= JX2_UCIX_QMUL_DIVU;
			end
			16'h7zz5: begin		/* F0nm_7eo5 */
				opNmid		= JX2_UCMD_QMULDIV;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_QMUL_MODSL;
//				opUCmdIx	= JX2_UCIX_QMUL_MODS;
				if(opExQ)
					opUCmdIx	= JX2_UCIX_QMUL_MODUL;
//					opUCmdIx	= JX2_UCIX_QMUL_MODU;
			end
`endif

`ifdef jx2_enable_rgb5btcenccc
			16'h7zz6: begin		/* F0nm_7eo6 */
				opNmid		= JX2_UCMD_MUL3;
				opFmid		= JX2_FMID_REGREG;
//				opIty		= JX2_ITY_SW;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_MUL3_ENCCC1;
				
				if(opExQ)
				begin
					opUCmdIx	= JX2_UCIX_MUL3_ENCCC2;
				end
			end
`endif

`ifdef jx2_enable_aluptr
			16'h7zz7: begin		/* F0nm_7eo7 */
				opNmid		= JX2_UCMD_ALUW3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALUW_PSUBP;
				if(opExQ)
				begin
					opUCty	= JX2_IUC_WX;
				end
			end
`else
			16'h7zz7: begin		/* F0nm_7eo7 */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGREG;
				opIty		= JX2_ITY_SB;
				opUCmdIx	= JX2_UCIX_ALU_SUB;
				if(opExQ)
				begin
					opUCty		= JX2_IUC_WX;
				end
			end
`endif

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

`ifdef jx2_enable_ldst48a
			16'h9zz0: begin		/* F0nm_9eo0 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opBty	= JX2_BTY_UB;
				opIty	= JX2_ITY_NW;
				if(opExQ)
				begin
				end
			end
			16'h9zz1: begin		/* F0nm_9eo1 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTREGDISP;
				opBty	= JX2_BTY_UL;
				opIty	= JX2_ITY_NW;
			end

			16'h9zz2: begin		/* F0nm_9eo2 */
			end

			16'h9zz3: begin		/* F0nm_9eo3 */
			end

			16'h9zz4: begin		/* F0nm_9eo4 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_UB;
				opIty	= JX2_ITY_NW;
			end

			16'h9zz5: begin		/* F0nm_9eo5 */
				opNmid	= JX2_UCMD_FMOV_RM;
				opFmid	= JX2_FMID_REGSTDRREG;
				opBty	= JX2_BTY_UL;
				opIty	= JX2_ITY_NW;
				if(opExQ)
				begin
				end
			end

			16'h9zz6: begin		/* F0nm_9eo6 */
			end

			16'h9zz7: begin		/* F0nm_9eo7 */
			end

			16'h9zz8: begin		/* F0nm_9eo8 */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= JX2_BTY_UB;
				opIty	= JX2_ITY_NW;
				if(opExQ)
				begin
					opBty	= JX2_BTY_UW;
				end
			end

			16'h9zz9: begin		/* F0nm_9eo9 */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDREGDISPREG;
				opBty	= JX2_BTY_UL;
				opIty	= JX2_ITY_NW;
				if(opExQ)
				begin
				end
			end

			16'h9zzA: begin		/* F0nm_9eoA */
			end

			16'h9zzB: begin		/* F0nm_9eoB */
			end

			16'h9zzC: begin		/* F0nm_9eoC */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_UB;
				opIty	= JX2_ITY_NW;
				if(opExQ)
					opBty	= JX2_BTY_UW;
			end

			16'h9zzD: begin		/* F0nm_9eoD */
				opNmid	= JX2_UCMD_FMOV_MR;
				opFmid	= JX2_FMID_LDDRREGREG;
				opBty	= JX2_BTY_UL;
				opIty	= JX2_ITY_NW;

				if(opExQ)
				begin
					opNmid		= JX2_UCMD_INVOP;
				end
			end

			16'h9zzE: begin		/* F0nm_9eoE */
			end

			16'h9zzF: begin		/* F0nm_9eoF */
			end
`endif

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

`ifndef def_true
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
`endif

			default: begin
			end
		endcase

		if(opCcty == JX2_IXC_AL)
			opCcty = opCcty_Dfl;

		opNmid_F0		= opNmid;
		opFmid_F0		= opFmid;
		opUCmdIx_F0		= opUCmdIx;
		opBty_F0		= opBty;
		opIty_F0		= opIty;
		opUCty_F0		= opUCty;
		opCcty_F0		= opCcty;
		opRegM_Fix_F0	= opRegM_Fix;
		opRegO_Fix_F0	= opRegO_Fix;
		opRegN_Fix_F0	= opRegN_Fix;

		usrReject_F0	= usrReject;
		usrSuAllow_F0	= usrSuAllow;
	end

//	else
//		if(tBlockIsF1)		/* F1nm_Xedd */

	if(1'b1)
	begin
		opNmid		= JX2_UCMD_INVOP;
		opFmid		= JX2_FMID_INV;
		opUCmdIx	= 0;
		opBty		= 0;
		opIty		= 0;
		opUCty		= JX2_IUC_SC;
		opCcty		= JX2_IXC_AL;
		opRegM_Fix	= JX2_GR_ZZR;
		opRegO_Fix	= JX2_GR_ZZR;
		opRegN_Fix	= JX2_GR_ZZR;

		case(istrWord[31:28])
			4'h0: begin		/* F1nm_0edd */
				opIty	= JX2_ITY_SW;
				opBty	= JX2_BTY_SB;
				if(opExQ)
				begin
					opNmid	= JX2_UCMD_LEA_MR;
					opFmid	= JX2_FMID_LDREGDISPREG;
					opIty	= JX2_ITY_SL;

//`ifdef jx2_cpu_addp
`ifdef def_true
					if(opIsJumbo96)
					begin
`ifdef jx2_cpu_addp
						opNmid		= JX2_UCMD_ALUW3;
						opUCmdIx	= JX2_UCIX_ALUW_PADDP;
`else
						opNmid		= JX2_UCMD_ALU3;
						opUCmdIx	= JX2_UCIX_ALU_ADD;
`endif
						opFmid		= JX2_FMID_REGIMMREG;
						opIty		= JX2_ITY_XW;
					end
`endif
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

		if(opCcty == JX2_IXC_AL)
			opCcty = opCcty_Dfl;

		opNmid_F1		= opNmid;
		opFmid_F1		= opFmid;
		opUCmdIx_F1		= opUCmdIx;
		opBty_F1		= opBty;
		opIty_F1		= opIty;
		opUCty_F1		= opUCty;
		opCcty_F1		= opCcty;
		opRegM_Fix_F1	= opRegM_Fix;
		opRegO_Fix_F1	= opRegO_Fix;
		opRegN_Fix_F1	= opRegN_Fix;
	end

//	else
//		if(tBlockIsF2 || tBlockIsEA_F2)		/* F2nm_Xedd */

	if(1'b1)
	begin
		opNmid		= JX2_UCMD_INVOP;
		opFmid		= JX2_FMID_INV;
		opUCmdIx	= 0;
		opBty		= 0;
		opIty		= 0;
		opUCty		= JX2_IUC_SC;
		opCcty		= JX2_IXC_AL;
		opRegM_Fix	= JX2_GR_ZZR;
		opRegO_Fix	= JX2_GR_ZZR;
		opRegN_Fix	= JX2_GR_ZZR;

		case(istrWord[31:28])
`ifdef def_true
			4'h0: begin		/* F2nm_0ejj */
				opNmid		= JX2_UCMD_ALU3;
				opFmid		= JX2_FMID_REGIMMREG;
				opIty		= JX2_ITY_UW;
				opUCmdIx	= JX2_UCIX_ALU_ADD;
`ifdef jx2_use_fpu_fpimm
//				case(opIsImmLdOp)
				case(opIsImmLdOp[3:0])
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

`ifdef jx2_use_fpu_fpimm
				if(opIsJumbo96)
				begin
					opNmid		= JX2_UCMD_FPUV4SF;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UB;

					if(opExQ)
					begin
						if(opExI)
							opUCmdIx	= JX2_UCIX_FPU_PMULX;
						else
							opUCmdIx	= JX2_UCIX_FPU_PADDX;
						opUCty		= JX2_IUC_WX;
					end else begin
						if(opExI)
							opUCmdIx	= JX2_UCIX_FPU_PMULH;
						else
							opUCmdIx	= JX2_UCIX_FPU_PADDH;
					end
				end
`endif
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

`ifdef jx2_use_imm_shuffle
				if(opIsJumbo96)
				begin
					opNmid		= JX2_UCMD_FPUV4SF;
					opFmid		= JX2_FMID_REGIMMREG;
					opIty		= JX2_ITY_UB;
					opUShufOp	= opIsImmShufOp;

					if(opExQ)
					begin
						if(opExI)
							opUCmdIx	= JX2_UCIX_FPU_PMULX;
						else
							opUCmdIx	= JX2_UCIX_FPU_PADDX;
						opUCty		= JX2_IUC_WX;
					end else begin
						if(opExI)
							opUCmdIx	= JX2_UCIX_FPU_PMULH;
						else
							opUCmdIx	= JX2_UCIX_FPU_PADDH;
					end
				end
`endif
			end
`endif

`ifdef def_true
			4'hC: begin		/* F2nz_Cejj */
				opIty	= istrWord[0] ?
					JX2_ITY_NQ :
					JX2_ITY_UQ;
				opFmid = JX2_FMID_IMM10REG;

				casez(istrWord[3:0])
					4'b000z: begin
						opNmid		= JX2_UCMD_MOV_IR;
						opUCmdIx	= 0;
`ifdef jx2_gprs_mergecm
						opNmid		= JX2_UCMD_MOV_RR;
						opIty	= istrWord[0] ?
							JX2_ITY_NL :
							JX2_ITY_UL ;
`endif
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
				opFmid = JX2_FMID_IMM10REG;

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

`ifdef jx2_alu_jcmpz
					4'b0110: begin		/* F2n6_Dejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27],
							1'b0, istrWord[25:24]};
						opIty		= JX2_ITY_UW;
					end
					4'b0111: begin		/* F2n7_Dejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27],
							1'b1, istrWord[25:24]};
						opIty		= JX2_ITY_UW;
					end
`else
`ifdef jx2_alu_jcmp
					4'b0110: begin		/* F2n6_Dejj */
						opNmid		= JX2_UCMD_JCMP;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 1'b0, istrWord[27],
							2'b00, istrWord[25:24]};
						opIty		= JX2_ITY_UW;
					end
					4'b0111: begin		/* F2n7_Dejj */
						opNmid		= JX2_UCMD_JCMP;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 1'b0, istrWord[27],
							2'b01, istrWord[25:24]};
						opIty		= JX2_ITY_UW;
					end
`endif
`endif

`ifdef def_true
					4'b1000: begin
						opNmid		= JX2_UCMD_MOV_RM;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_XL;
						opBty		= JX2_BTY_SL;
//						if(opExQ)
//							opNmid		= JX2_UCMD_LEA_MR;
					end
					4'b1001: begin
						opNmid		= JX2_UCMD_MOV_RM;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_XL;
						opBty		= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
					end
					4'b1010: begin
						opNmid		= JX2_UCMD_MOV_MR;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_XL;
						opBty		= opExQ ? JX2_BTY_UL : JX2_BTY_SL;
					end
					4'b1011: begin
						opNmid		= JX2_UCMD_MOV_MR;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_XL;
						opBty		= opExQ ? JX2_BTY_UQ : JX2_BTY_SQ;
					end
`endif

`ifdef jx2_use_fpu_fpimm
					4'b1100: begin
						opNmid		= JX2_UCMD_FCMP;
						opUCmdIx	= JX2_UCIX_FPU_CMPEQ;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_XQ;
						if(opExQ)
						begin
							opNmid		= JX2_UCMD_CONV_RR;
							opUCmdIx	= JX2_UCIX_CONV_MOV;
							opIty		= JX2_ITY_XB;
						end
					end
					4'b1101: begin
//						opNmid		= JX2_UCMD_FCMP;
//						opUCmdIx	= JX2_UCIX_FPU_CMPEQ;
					end
					4'b1110: begin
						opNmid		= JX2_UCMD_FCMP;
						opUCmdIx	= JX2_UCIX_FPU_CMPGT;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_XQ;
						if(opExQ)
						begin
							opNmid		= JX2_UCMD_FPU3;
							opUCmdIx	= JX2_UCIX_FPU_FADD;
						end
					end
					4'b1111: begin
						opNmid		= JX2_UCMD_FCMP;
						opUCmdIx	= JX2_UCIX_FPU_CMPGE;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_XQ;
						if(opExQ)
						begin
							opNmid		= JX2_UCMD_FPU3;
							opUCmdIx	= JX2_UCIX_FPU_FMUL;
						end
					end
`endif

					default: begin
					end
				endcase
			end
`endif

`ifdef def_true
			4'hE: begin		/* F2nz_Eejj */
				opIty	= istrWord[0] ?
					JX2_ITY_NQ :
					JX2_ITY_UQ;
				opFmid = JX2_FMID_IMM10REG;

				casez(istrWord[3:0])
					4'b0000: begin
						opNmid		= JX2_UCMD_LEA_MR;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_UQ;
						opUCmdIx	= 6'b000100;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end
					4'b0001: begin
						opNmid		= JX2_UCMD_LEA_MR;
						opFmid		= JX2_FMID_IMM10REG;
						opIty		= JX2_ITY_NQ;
						opUCmdIx	= 6'b000100;
						if(opExQ)
						begin
							opUCty		= JX2_IUC_WX;
						end
					end


`ifdef jx2_alu_jcmpz
					4'b0100: begin		/* F2n4_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b000};
						opIty		= JX2_ITY_UL;
					end
					4'b0101: begin		/* F2n5_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b000};
						opIty		= JX2_ITY_NL;
					end
					4'b0110: begin		/* F2n6_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b001};
						opIty		= JX2_ITY_UL;
					end
					4'b0111: begin		/* F2n7_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b001};
						opIty		= JX2_ITY_NL;
					end

					4'b1000: begin		/* F2n8_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b100};
						opIty		= JX2_ITY_UL;
					end
					4'b1001: begin		/* F2n9_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b100};
						opIty		= JX2_ITY_NL;
					end
					4'b1010: begin		/* F2nA_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b101};
						opIty		= JX2_ITY_UL;
					end
					4'b1011: begin		/* F2nB_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b101};
						opIty		= JX2_ITY_NL;
					end

					4'b1100: begin		/* F2nC_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b010};
						opIty		= JX2_ITY_UL;
					end
					4'b1101: begin		/* F2nD_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b010};
						opIty		= JX2_ITY_NL;
					end
					4'b1110: begin		/* F2nE_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b011};
						opIty		= JX2_ITY_UL;
					end
					4'b1111: begin		/* F2nF_Eejj */
						opNmid		= JX2_UCMD_JCMPZ;
						opFmid		= JX2_FMID_REGPC;
						opUCmdIx	= { 2'h0, istrWord[27], 3'b011};
						opIty		= JX2_ITY_NL;
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

		if(opCcty == JX2_IXC_AL)
			opCcty = opCcty_Dfl;

		opNmid_F2		= opNmid;
		opFmid_F2		= opFmid;
		opUCmdIx_F2		= opUCmdIx;
		opBty_F2		= opBty;
		opIty_F2		= opIty;
		opUCty_F2		= opUCty;
		opCcty_F2		= opCcty;
		opRegM_Fix_F2	= opRegM_Fix;
		opRegO_Fix_F2	= opRegO_Fix;
		opRegN_Fix_F2	= opRegN_Fix;
	end

//	else
//		if(tBlockIsF8)		/* F8zz_zzzz */

	if(1'b1)
	begin
		opNmid		= JX2_UCMD_INVOP;
		opFmid		= JX2_FMID_INV;
		opUCmdIx	= 0;
		opBty		= 0;
		opIty		= 0;
		opUCty		= JX2_IUC_SC;
		opCcty		= JX2_IXC_AL;
		opRegM_Fix	= JX2_GR_ZZR;
		opRegO_Fix	= JX2_GR_ZZR;
		opRegN_Fix	= JX2_GR_ZZR;

		casez(istrWord[7:5])
			3'b000: begin
//				if(opIsJumbo)
				if(opIsJumbo96)
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_LDI_JLDIX;
`ifdef jx2_gprs_mergecm
					opNmid		= JX2_UCMD_MOV_RR;
					opIty		= JX2_ITY_SW;
`endif
				end
				else
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
//					opIty		= JX2_ITY_UW;
					opIty		= JX2_ITY_UB;
					opUCmdIx	= JX2_UCIX_LDI_LDIX;

`ifdef jx2_gprs_mergecm
					opNmid		= JX2_UCMD_MOV_RR;
					opIty		= JX2_ITY_UW;
`endif
				end
			end
			3'b001: begin
				if(opIsJumbo96)
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SB;
					opUCmdIx	= JX2_UCIX_LDI_JLDIX;
`ifdef jx2_gprs_mergecm
					opNmid		= JX2_UCMD_MOV_RR;
					opIty		= JX2_ITY_SW;
`endif
				end
				else
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
	//				opIty		= JX2_ITY_NW;
					opIty		= JX2_ITY_NB;
					opUCmdIx	= JX2_UCIX_LDI_LDIX;
`ifdef jx2_gprs_mergecm
					opNmid		= JX2_UCMD_MOV_RR;
					opIty		= JX2_ITY_NW;
`endif
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
				if(opIsJumbo96)
				begin
					opNmid		= JX2_UCMD_CONV2_RR;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32L;
					opUCty		= JX2_IUC_WX;
				end
				else if(opIsJumboAu)
				begin
//					opNmid		= JX2_UCMD_MOV_IR;
//					opFmid		= JX2_FMID_IMM8REG;
//					opIty		= JX2_ITY_NB;
//					opUCmdIx	= JX2_UCIX_LDI_FLDCH;

					opNmid		= JX2_UCMD_FLDCX;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_UW;
					opUCmdIx	= JX2_UCIX_FPCX_SG;

`ifdef jx2_enable_convfp16
					if(opExWM || opExWI)
					begin
						opNmid		= JX2_UCMD_CONV2_RR;
						opUCmdIx	= JX2_UCIX_CONV2_FP16UPCK32L;
					end
					
					if(opExWM)
					begin
						opUCmdIx	= opExWI ?
							JX2_UCIX_CONV2_RGB32UPCK64FU :
							JX2_UCIX_CONV2_RGB32UPCK64FS;
					end
`endif
				end
				else
				begin
					opNmid		= JX2_UCMD_MOV_IR;
					opFmid		= JX2_FMID_IMM8REG;
//					opIty		= JX2_ITY_NB;
					opIty		= JX2_ITY_UB;
					opUCmdIx	= JX2_UCIX_LDI_FLDCH;

//					opNmid		= JX2_UCMD_FLDCX;
//					opFmid		= JX2_FMID_IMM8REG;
//					opIty		= JX2_ITY_UW;
//					opUCmdIx	= JX2_UCIX_FPCX_HG;

	//				if(opRegO_Df2_IsSP)
					if(istrWord[4:0]==5'h0F)
//					if((istrWord[4:0]==5'h0F) && !opIsJumboAu)
					begin
						/* "FLDCH Imm16, SP" is a BREAK */
						opNmid		= JX2_UCMD_OP_IXT;
						opUCmdIx	= JX2_UCIX_IXT_BREAK;
					end
				end
			end
`endif


`ifndef def_true
			3'b101: begin
				if(opIsJumbo96)
				begin
				end
				else if(opIsJumboAu)
				begin
//					opNmid		= JX2_UCMD_FLDCX;
//					opFmid		= JX2_FMID_IMM8REG;
//					opIty		= JX2_ITY_UW;
//					opUCmdIx	= JX2_UCIX_FPCX_SG;

					opNmid		= JX2_UCMD_JCMPZ;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SQ;
					opUCmdIx	= { 2'b00, istrWord[31:28] };
				end
				else
				begin
`ifdef jx2_alu_jcmpz
					opNmid		= JX2_UCMD_JCMPZ;
					opFmid		= JX2_FMID_IMM8REG;
					opIty		= JX2_ITY_SQ;
					opUCmdIx	= { 2'b00, istrWord[31:28] };
`endif
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

		if(opCcty == JX2_IXC_AL)
			opCcty = opCcty_Dfl;

		opNmid_F8		= opNmid;
		opFmid_F8		= opFmid;
		opUCmdIx_F8		= opUCmdIx;
		opBty_F8		= opBty;
		opIty_F8		= opIty;
		opUCty_F8		= opUCty;
		opCcty_F8		= opCcty;
		opRegM_Fix_F8	= opRegM_Fix;
		opRegO_Fix_F8	= opRegO_Fix;
		opRegN_Fix_F8	= opRegN_Fix;
	end

	if(1'b1)
	begin
		opNmid		= JX2_UCMD_INVOP;
		opFmid		= JX2_FMID_INV;
		opUCmdIx	= 0;
		opBty		= 0;
		opIty		= 0;
		opUCty		= JX2_IUC_SC;
		opCcty		= JX2_IXC_AL;
		opRegM_Fix	= JX2_GR_ZZR;
		opRegO_Fix	= JX2_GR_ZZR;
		opRegN_Fix	= JX2_GR_ZZR;

		opNmid_F3		= opNmid;
		opFmid_F3		= opFmid;
		opUCmdIx_F3		= opUCmdIx;
		opBty_F3		= opBty;
		opIty_F3		= opIty;
		opUCty_F3		= opUCty;
		opCcty_F3		= opCcty;
//		opRegM_Fix_F3	= opRegM_Fix;
//		opRegO_Fix_F3	= opRegO_Fix;
//		opRegN_Fix_F3	= opRegN_Fix;
	end

	if(1'b1)
	begin
		opNmid		= JX2_UCMD_INVOP;
		opFmid		= JX2_FMID_INV;
		opUCmdIx	= 0;
		opBty		= 0;
		opIty		= 0;
		opUCty		= JX2_IUC_SC;
		opCcty		= JX2_IXC_AL;
		opRegM_Fix	= JX2_GR_ZZR;
		opRegO_Fix	= JX2_GR_ZZR;
		opRegN_Fix	= JX2_GR_ZZR;

		opNmid_F9		= opNmid;
		opFmid_F9		= opFmid;
		opUCmdIx_F9		= opUCmdIx;
		opBty_F9		= opBty;
		opIty_F9		= opIty;
		opUCty_F9		= opUCty;
		opCcty_F9		= opCcty;
//		opRegM_Fix_F9	= opRegM_Fix;
//		opRegO_Fix_F9	= opRegO_Fix;
//		opRegN_Fix_F9	= opRegN_Fix;
	end


	if(1'b1)
	begin
		opNmid		= JX2_UCMD_INVOP;

		if(tBlockIsFA || tBlockIsFB)
		begin
			opNmid		= JX2_UCMD_MOV_IR;
			opFmid		= JX2_FMID_IMM12Z;
			opUCmdIx	= JX2_UCIX_LDI_LDIX;
			opIty		= JX2_ITY_SB;
			
//			if(opIsJumbo && opIsJumboAu)
			if(opIsJumboAu)
			begin
				opNmid		= JX2_UCMD_JMP;
	//			opFmid		= JX2_FMID_IMM12Z;
				opIty		= JX2_ITY_UW;
				if(tBlockIsFB)
				begin
					opNmid		= JX2_UCMD_JSR;
//					opFmid		= JX2_FMID_IMM12Z;
				end
			end
			else
			begin
				if(tBlockIsFB)
					opFmid		= JX2_FMID_IMM12N;
			end
		end

		if(tBlockIsFE || tBlockIsFF)
		begin
			opNmid		= JX2_UCMD_NOP;
			opFmid		= JX2_FMID_IMM12Z;
		end

		opNmid_FA		= opNmid;
		opFmid_FA		= opFmid;
		opUCmdIx_FA		= opUCmdIx;
		opIty_FA		= opIty;
		opBty_FA		= opBty;
		opUCty_FA		= opUCty;
		opCcty_FA		= opCcty;
	end


	opNmid		= JX2_UCMD_INVOP;
	opFmid		= JX2_FMID_INV;
	opUCmdIx	= 0;
	opBty		= 0;
	opIty		= 0;
	opUCty		= JX2_IUC_SC;
	opCcty		= JX2_IXC_AL;
	opRegM_Fix	= JX2_GR_ZZR;
	opRegO_Fix	= JX2_GR_ZZR;
	opRegN_Fix	= JX2_GR_ZZR;
	
	usrReject	= 0;
	usrSuAllow	= 0;

	opCcty = opCcty_Dfl;

`ifndef def_true
	casez( {
		tBlockIsF0 || tBlockIsEA_F0,
		tBlockIsF1,
		tBlockIsF2 || tBlockIsEA_F2,
		tBlockIsF3,
		tBlockIsF8,
		tBlockIsF9,
		tBlockIsFA || tBlockIsFB ||
			tBlockIsFE || tBlockIsFF
		} )
			7'b1zzzzzz: tSelBlock = 0;
			7'b01zzzzz: tSelBlock = 1;
			7'b001zzzz: tSelBlock = 2;
			7'b0001zzz: tSelBlock = 3;
			7'b00001zz: tSelBlock = 4;
			7'b000001z: tSelBlock = 5;
			7'b0000001: tSelBlock = 6;
			7'b0000000: tSelBlock = 7;
	endcase
`endif

	if(tBlockIsF0 || tBlockIsEA_F0)
	begin
		opNmid		= opNmid_F0;
		opFmid		= opFmid_F0;
		opUCmdIx	= opUCmdIx_F0;
		opBty		= opBty_F0;
		opIty		= opIty_F0;
		opUCty		= opUCty_F0;
		opCcty		= opCcty_F0;
		opRegM_Fix	= opRegM_Fix_F0;
		opRegO_Fix	= opRegO_Fix_F0;
		opRegN_Fix	= opRegN_Fix_F0;

		usrReject	= usrReject_F0;
		usrSuAllow	= usrSuAllow_F0;
	end
	else
		if(tBlockIsF1)		/* F1nm_Xedd */
	begin
		opNmid		= opNmid_F1;
		opFmid		= opFmid_F1;
		opUCmdIx	= opUCmdIx_F1;
		opBty		= opBty_F1;
		opIty		= opIty_F1;
		opUCty		= opUCty_F1;
//		opCcty		= opCcty_F1;
//		opRegM_Fix	= opRegM_Fix_F1;
//		opRegO_Fix	= opRegO_Fix_F1;
//		opRegN_Fix	= opRegN_Fix_F1;
	end
	else
		if(tBlockIsF2 || tBlockIsEA_F2)		/* F2nm_Xedd */
	begin
		opNmid		= opNmid_F2;
		opFmid		= opFmid_F2;
		opUCmdIx	= opUCmdIx_F2;
		opBty		= opBty_F2;
		opIty		= opIty_F2;
//		opCcty		= opCcty_F2;
//		opRegM_Fix	= opRegM_Fix_F2;
//		opRegO_Fix	= opRegO_Fix_F2;
//		opRegN_Fix	= opRegN_Fix_F2;
	end
	else
		if(tBlockIsF8)		/* F8zz_zzzz */
	begin
		opNmid		= opNmid_F8;
		opFmid		= opFmid_F8;
		opUCmdIx	= opUCmdIx_F8;
		opBty		= opBty_F8;
		opIty		= opIty_F8;
		opUCty		= opUCty_F8;
//		opCcty		= opCcty_F8;
//		opRegM_Fix	= opRegM_Fix_F8;
//		opRegO_Fix	= opRegO_Fix_F8;
//		opRegN_Fix	= opRegN_Fix_F8;
	end
	else
//		if(tBlockIsFA || tBlockIsFB)	/* FAjj_jjjj / FBjj_jjjj */
	begin
		opNmid		= opNmid_FA;
		opFmid		= opFmid_FA;
		opUCmdIx	= opUCmdIx_FA;
		opIty		= opIty_FA;
	end
//	else
//		if(tBlockIsFE || tBlockIsFF)
//	begin
//		opNmid		= JX2_UCMD_NOP;
//		opFmid		= JX2_FMID_IMM12Z;
//	end

`ifndef def_true
	case(tSelBlock)
		3'h0: begin
			opNmid		= opNmid_F0;
			opFmid		= opFmid_F0;
			opUCmdIx	= opUCmdIx_F0;
			opBty		= opBty_F0;
			opIty		= opIty_F0;
			opUCty		= opUCty_F0;
			opCcty		= opCcty_F0;
			opRegM_Fix	= opRegM_Fix_F0;
			opRegO_Fix	= opRegO_Fix_F0;
			opRegN_Fix	= opRegN_Fix_F0;

			usrReject	= usrReject_F0;
			usrSuAllow	= usrSuAllow_F0;
		end
		3'h1: begin
			opNmid		= opNmid_F1;
			opFmid		= opFmid_F1;
			opUCmdIx	= opUCmdIx_F1;
			opBty		= opBty_F1;
			opIty		= opIty_F1;
			opUCty		= opUCty_F1;
			opCcty		= opCcty_F1;
		end
		3'h2: begin
			opNmid		= opNmid_F2;
			opFmid		= opFmid_F2;
			opUCmdIx	= opUCmdIx_F2;
			opBty		= opBty_F2;
			opIty		= opIty_F2;
			opUCty		= opUCty_F2;
			opCcty		= opCcty_F2;
		end
		3'h3: begin
			opNmid		= opNmid_F3;
			opFmid		= opFmid_F3;
			opUCmdIx	= opUCmdIx_F3;
			opBty		= opBty_F3;
			opIty		= opIty_F3;
			opUCty		= opUCty_F3;
			opCcty		= opCcty_F3;
		end
		3'h4: begin
			opNmid		= opNmid_F8;
			opFmid		= opFmid_F8;
			opUCmdIx	= opUCmdIx_F8;
			opBty		= opBty_F8;
			opIty		= opIty_F8;
			opUCty		= opUCty_F8;
			opCcty		= opCcty_F8;
		end
		3'h5: begin
			opNmid		= opNmid_F9;
			opFmid		= opFmid_F9;
			opUCmdIx	= opUCmdIx_F9;
			opBty		= opBty_F9;
			opIty		= opIty_F9;
			opUCty		= opUCty_F9;
			opCcty		= opCcty_F9;
		end
		3'h6: begin
			opNmid		= opNmid_FA;
			opFmid		= opFmid_FA;
			opUCmdIx	= opUCmdIx_FA;
			opBty		= opBty_FA;
			opIty		= opIty_FA;
			opUCty		= opUCty_FA;
			opCcty		= opCcty_FA;
		end
		3'h7: begin
		end
	endcase
`endif

`ifndef def_true
//	if(opIsNotFx)
	if(opIsNotFx && !opIsXGpr)
	begin
		opFmid	= JX2_FMID_Z;
		opIty	= JX2_ITY_SB;
	end
`endif

	usrSuAllowEn = 0;
	case(usrSuAllow)
		2'b00: usrSuAllowEn = 0;					//Supervisor Only
		2'b01: usrSuAllowEn = srSuperuser;			//Superuser
		2'b10: usrSuAllowEn = srMod[1];				//Secure Execute
		2'b11: usrSuAllowEn = srMod[1] && srMod[2];	//Superuser+Secure Exe
	endcase

`ifndef def_true
//	if(usrReject && srUser && !(usrSuAllow && srSuperuser))
	if(usrReject && srUser && !usrSuAllowEn)
//	if(usrReject && srUser && !usrSuAllowEn && (!opIsNotFx || opIsXGpr))
	begin
		if(!opIsNotFx || opIsXGpr)
		begin
			$display("DecOpFz: Usermode Reject %X-%X %X-%X",
				istrWord[15:0], istrWord[31:16], opFmid, opIty);
		end
		opNmid		= JX2_UCMD_INVOP;
//		opFmid		= JX2_FMID_INV;
		opUCmdIx	= JX2_UCIX_INVOP_PRIVFAULT;
	end
`endif

	opUCmd = { opCcty, opNmid };

`ifdef def_true
	if((opNmid==JX2_UCMD_INVOP) && !opIsNotFx && !isAltOp)
	begin
		$display("Jx2DecOpFx: InvOp, Istr %X-%X",
			istrWord[15:0], istrWord[31:16]);
	end
`endif

	opUFl		= 0;
	opULdOp		= 0;
	opULdOp2	= 0;
//	opUShufOp	= 0;
	
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
			opRegM	= opRegO_Dfl;
			opRegO	= opRegO_Dfl;
			opRegN	= opRegO_Dfl;
			opRegP	= opRegO_Dfl;
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

`ifndef def_true
				JX2_ITY_NB: begin
					opRegM	= opRegO_Cr;
					opRegO	= JX2_GR_ZZR;
					opRegN	= opRegO_Cr;
					opRegP	= opRegO_Cr;
					if(usrRejectCoW)
						usrReject = 1;
				end
`endif

				JX2_ITY_XB: begin
					opRegM	= JX2_GR_ZZR;
//					opRegO	= opRegO_Dfl;
					opRegO	= opRegO_OrgDfl;
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
			SQ: Rm, Q?Disp5u:Ro, Rn

			UB: Rm, Rn, Rn
			UW: Rm, Rn, Rn
			UL: Rm, Cn, Cn
			UQ: Cm, Rn, Rn

			NB: Rn, Rm, Rn
			NW: /
			NL: /
			NQ: /

			SW: Rm, Eq?Imm5:Ro, Rn

			XB: Rm, Ro, Rn
			XW: Rm, Ro, Rn

			XL: Rn, RmImm6u, Rn
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
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
			opRegO	= opRegO_Dfl;

`ifdef jx2_reg_rp
			if(opIsImm4R)
				opRegP	= opRegP_Dfl;
`endif

//			opImm	= opImm_imm5u;
			opDoImm	= JX2_FMIMM_IMM5U;


			opUIxt	= { opUCty, opUCmdIx };

`ifdef jx2_use_imm_shuffle
			opULdOp		= opUShufOp;
			opULdOp2	= { 4'h0, opImm_dispasc[1:0] };
`endif

			case(opIty)
				JX2_ITY_SB: begin
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_FMIMM_IMM8AU;
				end

				JX2_ITY_SW: begin
//					opImm	= opImm_imm5u;
					opDoImm	= JX2_FMIMM_IMM5U;
					if(opExQ)
					begin
						opRegO	= JX2_GR_IMM;
						opRegP	= opRegN_Dfl;
`ifdef jx2_reg_rp
						if(opIsImm4R)
							opRegP	= opRegO_Dfl;
`endif
					end
				end

				JX2_ITY_UB: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Dfl;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_FMIMM_IMM8AU;
				end

				JX2_ITY_UW: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegM_Dfl;
					opRegO	= opRegN_Dfl;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_FMIMM_IMM8AU;
				end

				JX2_ITY_NB: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= opRegM_Dfl;
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_FMIMM_IMM8AU;
				end

				JX2_ITY_SQ: begin
//					opImm	= opImm_disp5u;
					opDoImm	= JX2_FMIMM_DISP5U;
					if(opExQ)
					begin
						opRegO	= JX2_GR_IMM;
						opRegP	= opRegN_Dfl;
					end
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
					opRegO	= opRegN_Dfl;
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					if(usrRejectCmR)
						usrReject = 1;
				end

				JX2_ITY_XB: begin
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_FMIMM_IMM8AU;
//					opRegP	= opRegN_Dfl;
`ifdef jx2_reg_rp
					opRegP	= opRegP_Dfl;
`endif
				end
				JX2_ITY_XW: begin
//					opImm	= opImm_imm8au;
					opDoImm	= JX2_FMIMM_IMM8AU;
					opRegP	= JX2_GR_IMM;
				end

				JX2_ITY_XL: begin
					opRegN	= opRegN_Dfl;
					opRegP	= opRegN_Dfl;
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_IMM;
//					opImm	= opImmRm_imm6u;
//					opDoImm	= JX2_FMIMM_NONE;
					opDoImm	= JX2_FMIMM_IMM6U;
				end
				
				default: begin
					$display("Jx2DecOpFx: RegReg, Bad Ity=%X", opIty);
				end

			endcase
		end

		/*
		REGIMMREG, Fz:
			SB: Rm, 0, Rn
			SW:	/ Rm, Imm9s, Rn
			/ SL: Rm, ImmVf, Rn

			UB:	Rm, Imm9f/Imm16f, Rn
			UW:	Rm, Imm9u, Rn
			UL: Imm9u, Rm, Rn

			NB:	Rm, Imm5f, Rn
			NW:	Rm, Imm9n, Rn
		 */
		JX2_FMID_REGIMMREG: begin
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			opRegM	= opRegM_Dfl;
//			opRegO	= JX2_GR_IMM;
			opRegO	= opRegImm10;
			opUIxt	= { opUCty, opUCmdIx };
//			opImm	= opImm_imm9u;
			opDoImm		= JX2_FMIMM_IMM9U;
			opIsImm9	= 0;

			case(opIty)
				JX2_ITY_SB: begin
					opImm	= 0;
					opDoImm	= JX2_FMIMM_NONE;
				end

`ifndef def_true
				JX2_ITY_SW: begin
//					opImm	= opImm_imm9s;
					opDoImm		= JX2_FMIMM_IMM9S;
					opIsImm9	= 1;
				end
`endif

`ifdef jx2_use_fpu_fpimm
				JX2_ITY_UB: begin
//					opImm		= opImm_imm9u;
					opDoImm		= JX2_FMIMM_IMM9U;
					opIsImm9	= 1;
					
//					opRegO	= JX2_GR_FPIMM10;
					if(opIsJumbo96)
					begin
//						opImm	= opImm_imm9u;
						opDoImm	= JX2_FMIMM_IMM9U;
						opRegO	= JX2_GR_FPIMM56VF;
					end
					else
					if(opIsJumboAu)
					begin
//						opImm	= opImm_imm9u;
						opDoImm	= JX2_FMIMM_IMM9U;
						opRegO	= JX2_GR_FPIMM16;
					end
					else
					begin
						opImm	= { opImm_imm9u[26:0], 6'h00 };
						opDoImm	= JX2_FMIMM_NONE;
						opRegO	= JX2_GR_FPIMM16;
					end

//					opRegO		= JX2_GR_JIMM;
//					opUFl[2]	= 1;
//					opUFl[0]	= 0;
				end
`endif

`ifdef jx2_use_fpu_fpimm
				JX2_ITY_NB: begin
					opRegO	= JX2_GR_FPIMM16;
					opImm	= opImm_imm5f;
					opDoImm	= JX2_FMIMM_NONE;
				end
`endif

				JX2_ITY_UW: begin
//					opImm	= opImm_imm9u;
					opDoImm		= JX2_FMIMM_IMM9U;
					opIsImm9	= 1;
				end
				JX2_ITY_NW: begin
//					opImm	= opImm_imm9n;
					opDoImm		= JX2_FMIMM_IMM9N;
					opIsImm9	= 1;
				end

				JX2_ITY_UL: begin
//					opImm	= opImm_imm9u;
					opDoImm		= JX2_FMIMM_IMM9U;
					opIsImm9	= 1;

					opRegM	= JX2_GR_IMM;
					opRegO	= opRegM_Dfl;
				end

				JX2_ITY_XW: begin
//					opImm	= opImm_imm9n;
					opDoImm		= JX2_FMIMM_IMM9N;
					opIsImm9	= 1;

					if(tRegRmIsRz)
					begin
						opRegM	= tRegRmIsR1 ? JX2_GR_GBR : JX2_GR_PC;
					end
				end
				
				default: begin
					$display("Jx2DecOpFx: RegReg, Bad Ity=%X", opIty);
				end

			endcase

			opUFl[0]	= opIsImm9;

`ifdef jx2_use_imm_shuffle
			opULdOp	= opUShufOp;
`endif
		end

`ifdef def_true
		/*
			SW: (Rm, Disp9u/17s), Rn
			SL: (Rm, Disp9u/17s), Rn  (Keeps Scale)

			UB: (Rm, Disp5u), Rn
			UW: -
			UL: (Rm, Disp9u/17s), Cn

			NB: (Rm, Disp5u), Rn  (Keeps Scale)
			NW: (Rm, Disp5u*2), Rn
			NL: (Rm, Disp5u*4), Rn
			NQ: (Rm, Disp5u*8), Rn
		 */
//		JX2_FMID_LDREGDISPREG: begin
		JX2_FMID_LDREGDISPREG, JX2_FMID_REGSTREGDISP: begin
//			if(opBty == JX2_BTY_UQ)
//			if((opBty == JX2_BTY_UQ) && (opUCty == JX2_IUC_SC))
			if((opBty == JX2_BTY_UQ) && (opUCty == JX2_IUC_SC) &&
				(opIty != JX2_ITY_NB))
			begin
				opUCty			= JX2_IUC_WX;
//				opRegN_Dfl		= opRegN_OrgDfl;
//				opRegN_Dfl[6]	= 0;
			end

			opRegO	= JX2_GR_IMM;
//			opUIxt	= {opUCty, opBty[1:0], 1'b1, opBty};
			opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
//			opImm	= opImm_disp9s;
			opDoImm	= JX2_FMIMM_DISP9U;

`ifdef jx2_use_mem_ldop
			opULdOp	= opIsImmLdOp;
`endif
			
//			if(opIty==JX2_ITY_UB)
			if((opIty==JX2_ITY_UB) || (opIty==JX2_ITY_NB))
			begin
//				opImm	= opImm_disp5u;
				opDoImm		= JX2_FMIMM_DISP5U;
				if(opIsImm4R)
					opUIxt[1:0] = 0;
			end

`ifdef jx2_enable_ldst48a
			if((opIty==JX2_ITY_NW) && !tRegRmIsRz)
			begin
//				opImm	= opImm_disp5u;
				opDoImm		= JX2_FMIMM_DISP5U;
				opUIxt[2:0] = 3'b001;
			end
`endif

`ifdef jx2_enable_pmov
			if(opIty==JX2_ITY_NL)
			begin
//				opImm	= opImm_disp5u;
				opDoImm		= JX2_FMIMM_DISP5U;
				opUIxt[2:0] = 3'b010;
			end
			if(opIty==JX2_ITY_NQ)
			begin
//				opImm	= opImm_disp5u;
				opDoImm		= JX2_FMIMM_DISP5U;
				opUIxt[2:0] = 3'b011;
			end
`endif

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
//				(opNmid != JX2_UCMD_FMOV_MR))
				(opNmid != JX2_UCMD_FMOV_MR) &&
				(opIty != JX2_ITY_NB))
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
//				opDoImm	= JX2_FMIMM_DISP11AS;
				opDoImm	= JX2_FMIMM_NONE;

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
//					if(opIty!=JX2_ITY_SL)
					if((opIty!=JX2_ITY_SL) && (opIty!=JX2_ITY_NB))
//					if(1'b1)
					begin
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

`ifdef jx2_enable_ldst48a
				if((opIty==JX2_ITY_NW) && !tRegRmIsRz)
				begin
					opUIxt[2:0] = 3'b001;
				end
`endif
`ifdef jx2_enable_pmov
				if(opIty==JX2_ITY_NL)
				begin
					opUIxt[2:0] = 3'b010;
				end
				if(opIty==JX2_ITY_NQ)
				begin
					opUIxt[2:0] = 3'b011;
				end
`endif
			end
			else
			begin
				opRegN	= opRegN_Dfl;
				opRegP	= opRegN_Dfl;
				opRegM	= opRegM_Dfl;
				opRegO	= JX2_GR_DLR;
				opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
			end
		end

`ifdef def_true
		/*
			SB: Fzeo_iiii		Ro, Imm16s, Ro
			SW: Fzeo_iiii		Imm16s, Ro, Ro
			SL: Fzeo_iiii		Imm16s, DLR, DLR
			SQ: Fzeo_Ziii		Ro, Imm12s, Ro

			UB: Fzeo_jjjj		Ro, Imm16u, Ro
			UW: Fzeo_jjjj		Imm16u, Ro, Ro
			UL: /
			UQ: /

			NB: Fzeo_jjjj		Ro, Imm16n, Ro
			NW: Fzeo_jjjj		Imm16n, Ro, Ro
			NL: /
			NQ: /

			XB: /
			XW: Fzeo_jjjj		ImmFp16, Ro, Ro
			XL: /
			XQ: /

		 */
	
		JX2_FMID_IMM8REG: begin
//			$display("IMM8REG: IsJumbo=%d", opIsJumbo);
		
			opUIxt	= { opUCty, opUCmdIx };
			opUFl		= 0;
			opIsImm9	= 0;
			opIsImmSplit	= 0;
			
			if(opUCty == JX2_IUC_WX)
				opIsImmSplit = (opExWM && opExWI);

			opRegM	= opRegImm16;
			opRegO	= opRegO_Df2;
			opRegN	= opRegO_Df2;
			opRegP	= opRegO_Df2;
			
			case(opIty)
				JX2_ITY_SB: begin
//					opImm	= opImm_imm16s;
					opDoImm	= JX2_FMIMM_IMM16S;
					opRegM	= opRegO_Df2;
					opRegO	= opRegImm16;
//					opRegN	= opRegO_Df2;
//					opRegP	= opRegO_Df2;
				end

				JX2_ITY_SW: begin
//					opImm	= opImm_imm16s;
					opDoImm	= JX2_FMIMM_IMM16S;
				end

				JX2_ITY_SL: begin
//					opImm	= opImm_imm16s;
					opDoImm	= JX2_FMIMM_IMM16S;
					opRegM	= JX2_GR_DLR;
//					opRegO	= JX2_GR_IMM;
					opRegO	= opRegImm16;
					opRegN	= JX2_GR_DLR;
					opRegP	= JX2_GR_DLR;
				end

				JX2_ITY_UB: begin
//					opImm	= opImm_imm16u;
					opDoImm	= JX2_FMIMM_IMM16U;
					opRegM	= opRegO_Df2;
					opRegO	= opRegImm16;
//					opRegN	= opRegO_Df2;
//					opRegP	= opRegO_Df2;
				end

				JX2_ITY_UW: begin
//					opImm	= opImm_imm16u;
					opDoImm	= JX2_FMIMM_IMM16U;
				end

				JX2_ITY_NB: begin
//					opImm	= opImm_imm16n;
					opDoImm	= JX2_FMIMM_IMM16N;
					opRegM	= opRegO_Df2;
//					opRegO	= JX2_GR_IMM;
					opRegO	= opRegImm16;
//					opRegN	= opRegO_Df2;
//					opRegP	= opRegO_Df2;
				end

				JX2_ITY_NW: begin
//					opImm	= opImm_imm16n;
					opDoImm	= JX2_FMIMM_IMM16N;
				end

				default: begin
					$display("Jx2DecOpFx: Imm8Reg, Bad Ity=%X", opIty);
				end
			endcase
			
			opUFl[0]	= opIsImm9;
			opUFl[1]	= opIsImmSplit;
		end

		/*
			SB:
			SW:
			SL:
			SQ:
			SQ:

			UB:
			UW:
			UL: Fznz_zejj		Imm10u, Rn, Rn
			UQ: Fznz_zejj		Rn, Imm10u, Rn

			NB:
			NW:
			NL: Fznz_zejj		Imm10n, Rn, Rn
			NQ: Fznz_zejj		Rn, Imm10n, Rn

			XB: Fznz_zejj		ImmFp10, Rn, Rn
			XW:
			XL: Fznz_zzjj		(GBR, Disp10u), Rn
			XQ: Fznz_zejj		Rn, ImmFp10, Rn

		 */
		JX2_FMID_IMM10REG: begin
//			$display("IMM10REG: IsJumbo=%d", opIsJumbo);
		
			opUIxt	= { opUCty, opUCmdIx };
			opUFl		= 0;
			opIsImm9	= 0;
			opIsImmSplit	= 0;
			
//			if(opUCty == JX2_IUC_WX)
//				opIsImmSplit = (opExWM && opExWI);

			opRegM	= opRegN_Dfl;
			opRegO	= opRegN_Dfl;
			opRegN	= opRegN_Dfl;
			opRegP	= opRegN_Dfl;
			
			case(opIty)
				JX2_ITY_SB: begin
				end

				JX2_ITY_SW: begin
				end

`ifdef def_true
				JX2_ITY_UL: begin
//					opImm		= opImm_imm10u;
					opDoImm		= JX2_FMIMM_IMM10U;
//					opRegM		= JX2_GR_IMM;
					opRegM		= opRegImm10;
					opRegO		= opRegN_Dfl;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end
`endif

				JX2_ITY_UQ: begin
//					opImm		= opImm_imm10u;
					opDoImm		= JX2_FMIMM_IMM10U;
					opRegM		= opRegN_Dfl;
//					opRegO		= JX2_GR_IMM;
					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

`ifdef def_true
				JX2_ITY_NL: begin
//					opImm		= opImm_imm10n;
					opDoImm		= JX2_FMIMM_IMM10N;
//					opRegM		= JX2_GR_IMM;
					opRegM		= opRegImm10;
					opRegO		= opRegN_Dfl;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end
`endif

				JX2_ITY_NQ: begin
//					opImm		= opImm_imm10n;
					opDoImm		= JX2_FMIMM_IMM10N;
					opRegM		= opRegN_Dfl;
//					opRegO		= JX2_GR_IMM;
					opRegO		= opRegImm10;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

`ifdef jx2_use_fpu_fpimm
				JX2_ITY_XB: begin
//					opImm		= opImm_imm10u;
//					opRegM		= JX2_GR_FPIMM10;
					opImm		= { opImm_imm10u[26:0], 6'h0 };
					opDoImm		= JX2_FMIMM_NONE;
					opRegM		= JX2_GR_FPIMM16;
					opRegO		= opRegN_Dfl;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

				JX2_ITY_XW: begin
//					opImm		= opImm_imm16u;
					opDoImm		= JX2_FMIMM_IMM16U;
					opRegM		= JX2_GR_FPIMM16;
					opRegO		= opRegN_Dfl;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end

				JX2_ITY_XQ: begin
					opImm		= { opImm_imm10u[26:0], 6'h0 };
					opDoImm		= JX2_FMIMM_NONE;
					opRegM		= opRegN_Dfl;
					opRegO		= JX2_GR_FPIMM16;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;
					opIsImm9	= 1;
				end
`endif

				JX2_ITY_XL: begin
					if((opBty == JX2_BTY_UQ) && (opUCty == JX2_IUC_SC))
						opUCty		= JX2_IUC_WX;

//					opImm		= opImm_imm10u;
					opDoImm		= JX2_FMIMM_IMM10U;
					opRegM		= JX2_GR_GBR;
					opRegO		= JX2_GR_IMM;
					opRegN		= opRegN_Dfl;
					opRegP		= opRegN_Dfl;

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
`endif


`ifdef jx2_enable_ldirot
		JX2_FMID_IMM4ZREG: begin
//			opImm	= opImm_imm10u;
			opImm	= { opImm_imm10u[28:4], 8'h00 };
			opDoImm	= JX2_FMIMM_NONE;
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
			begin
/				opImm	= opImm_imm10u;
				opDoImm		= JX2_FMIMM_IMM10U;
			end
		end

		JX2_FMID_IMM4NREG: begin
//			opImm	= opImm_imm10n;
			opImm	= { opImm_imm10n[28:4], 8'h00 };
			opDoImm	= JX2_FMIMM_NONE;
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
			begin
//				opImm	= opImm_imm10u;
				opDoImm	= JX2_FMIMM_IMM10U;
			end
		end
`endif
		
		/*
			SB: (PC, Ro), ZZR
			SW: (PC, Ro), LR

			UB: Rn, Rm, disp8s
			UW: Rn, ZZR, Disp8s

			UL: Rn, ZZR, Disp10u
			NL: Rn, ZZR, Disp10n
		 */
		JX2_FMID_REGPC: begin
			opRegM	= JX2_GR_PC;
			opRegO	= opRegO_Dfl;
			opRegN	= JX2_GR_ZZR;
			opRegP	= JX2_GR_ZZR;

			case(opIty)
				JX2_ITY_SB: begin
					opRegM	= JX2_GR_PC;
					opRegO	= opRegO_Dfl;
					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;
					opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
				end

				JX2_ITY_SW: begin
					opRegM	= JX2_GR_PC;
					opRegO	= opRegO_Dfl;
					opRegN	= JX2_GR_LR;
					opRegP	= JX2_GR_LR;
					opUIxt	= {opUCty, opBty[1:0], 1'b0, opBty};
				end

`ifdef jx2_alu_jcmp
				JX2_ITY_UB: begin
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

//					opImm	= opImm_disp8s;
					opDoImm		= JX2_FMIMM_DISP8S;
					opUIxt	= { opUCty, opUCmdIx };
				end

`ifndef jx2_alu_jcmpz
				JX2_ITY_UW: begin
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_ZZR;
					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;

//					opImm	= opImm_disp8s;
					opDoImm	= JX2_FMIMM_DISP8S;
					opUIxt	= { opUCty, opUCmdIx };
				end
`endif
`endif

`ifdef jx2_alu_jcmpz
				JX2_ITY_UW: begin
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_ZZR;
					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;

//					opImm	= opImm_disp8s;
					opDoImm	= JX2_FMIMM_DISP8S;
					opUIxt	= { opUCty, opUCmdIx };
				end
`endif

				JX2_ITY_UL: begin
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_ZZR;
					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;

//					opImm	= opImm_imm10u;
					opDoImm	= JX2_FMIMM_IMM10U;
					opUIxt	= { opUCty, opUCmdIx };
				end

				JX2_ITY_NL: begin
					opRegM	= opRegN_Dfl;
					opRegO	= JX2_GR_ZZR;
					opRegN	= JX2_GR_ZZR;
					opRegP	= JX2_GR_ZZR;

//					opImm	= opImm_imm10n;
					opDoImm	= JX2_FMIMM_IMM10N;
					opUIxt	= { opUCty, opUCmdIx };
				end
				
				default: begin
					$display("Jx2DecOpFx: RegPc Bad Ity=%X", opIty);
				end
			endcase
		end

		/*
			SB: Fzdd_xxdd		(PC, disp16s), DLR
			SW: Fzdd_xddd		(PC, disp20s), DLR

			UB: / Fzdd_xxdx		(PC, disp13s), DLR
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
				opDoImm		= JX2_FMIMM_IMM16S;
			end
			JX2_ITY_SW: begin
//				opImm	= opImm_disp20s;
//				opDoImm	= JX2_FMIMM_NONE;
				opDoImm	= JX2_FMIMM_DISP20S;

				if(opImm_dispAltLr)
				begin
					opRegN	= JX2_GR_DHR;
					opRegP	= JX2_GR_DHR;
				end
			end

			JX2_ITY_UW: begin
//				opImm	= opImm_disp9s;
				opDoImm		= JX2_FMIMM_DISP9U;
			end

			default: begin
				$display("Jx2DecOpFx: PcDisp, Bad Ity=%X", opIty);
			end

			endcase
		end

		JX2_FMID_IMM12Z: begin
//			opImm	= {UV9_00, istrWord[7:0], istrWord[31:16]};
//			opDoImm	= JX2_FMIMM_NONE;
			opDoImm	= JX2_FMIMM_IMM24Z;
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
				opDoImm	= JX2_FMIMM_NONE;
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
//			opImm	= {UV9_FF, istrWord[7:0], istrWord[31:16]};
//			opDoImm	= JX2_FMIMM_NONE;
			opDoImm	= JX2_FMIMM_IMM24N;

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
				opDoImm	= JX2_FMIMM_NONE;
			end
		end
		
		JX2_FMID_INV: begin
			opUCmd = { opCcty, JX2_UCMD_INVOP };
//			if(istrWord[15:12]==4'b1111)
//			if(!opIsNotFx)
			if(!opIsNotFx || opIsXGpr)
			begin
//				opUCmd = { opCcty, JX2_UCMD_INVOP };
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
	
`ifndef jx2_reg_rp
//	opRegP = opRegN;
`endif

	case(opDoImm)
		JX2_FMIMM_NONE:		begin end
		JX2_FMIMM_DISP9U:	opImm	= opImm_disp9s;
		JX2_FMIMM_DISP5U:	opImm	= opImm_disp5u;
		JX2_FMIMM_IMM8AU:	opImm	= opImm_imm8au;

//		JX2_FMIMM_IMM5U:	opImm	= opImm_imm5u;
		JX2_FMIMM_IMM6U:	opImm	= opImmRm_imm6u;
		JX2_FMIMM_IMM9U:	opImm	= opImm_imm9u;
		JX2_FMIMM_IMM10U:	opImm	= opImm_imm10u;
		JX2_FMIMM_IMM16U:	opImm	= opImm_imm16u;

		JX2_FMIMM_DISP20S:	opImm	= opImm_disp20s;
//		JX2_FMIMM_IMM5N:	opImm	= opImm_imm5n;
		JX2_FMIMM_IMM9N:	opImm	= opImm_imm9n;
		JX2_FMIMM_IMM10N:	opImm	= opImm_imm10n;
		JX2_FMIMM_IMM16N:	opImm	= opImm_imm16n;
		
		JX2_FMIMM_IMM24Z:	opImm	= {UV9_00, istrWord[7:0], istrWord[31:16]};
		JX2_FMIMM_IMM24N:	opImm	= {UV9_FF, istrWord[7:0], istrWord[31:16]};

//		JX2_FMIMM_IMM9S:	opImm	= opImm_imm9s;
//		JX2_FMIMM_IMM10S:	opImm	= opImm_imm10s;
		JX2_FMIMM_IMM16S:	opImm	= opImm_imm16s;
		JX2_FMIMM_DISP8S:	opImm	= opImm_disp8s;

		default:			opImm	= UV33_XX;
//		default:		begin end
	endcase

`ifdef def_true
	if(usrReject && srUser && !usrSuAllowEn)
	begin
		opUCmd = { opCcty, JX2_UCMD_INVOP };
		opUIxt	= { opUCty, JX2_UCIX_INVOP_PRIVFAULT };
	end
`endif

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
