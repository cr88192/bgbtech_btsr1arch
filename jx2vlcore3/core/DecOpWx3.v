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
Instruction Decoder (WEX, 3-Wide)

Takes a 16/32 bit instruction word, or 32/64/96 bit bundle.

Ex block is predicated.
Fx block is unconditional.

This decoder will support WEX.
  Rs/Rt/Rm: Lane 1
  Ru/Rv/Rn: Lane 2
  Rx/Ry/Ro: Lane 3

For scalar Ops, Lane 2/3 will hold:
  ZZR, ZZR, ZZR
  ZZR, Rm, ZZR

Store gets its 3rd input from Lane 3 (Ry), with Store-Pair using Rx:Ry.

 */

`include "CoreDefs.v"

`include "DecOpBz.v"
`include "DecOpFz.v"

`ifdef jx2_enable_riscv
`include "DecOpRvI.v"

`ifndef jx2_dec_ssc3_riscv
`ifdef jx2_enable_rvjumbo96
`include "DecOpRvJO.v"
`endif
`endif

`endif

`ifdef jx2_enable_riscv_rvc
`include "DecOpRvC.v"
`endif

`ifdef jx2_enable_riscv_xg3
// `include "DecOpRepXG3.v"
`endif

/* verilator lint_off DEFPARAM */
// /* verilator lint_off UNUSEDSIGNAL */

module DecOpWx3(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	istrMTag,
	istrBPc,	regSr,		regFpsr,
	istrSxo,	idPcStep,

	idRegS,		idRegT,		idRegM,
	idImmA,		idUCmdA,	idUIxtA,
	idRegU,		idRegV,		idRegN,
	idImmB,		idUCmdB,	idUIxtB,
	idRegX,		idRegY,		idRegO,
	idImmC,		idUCmdC,	idUIxtC
	);

input			clock;		//clock
input			reset;		//clock

input[95:0]		istrWord;	//source instruction word
input[47:0]		istrBPc;	//Instruction PC Address
// input			srWxe;
input[63:0]		regSr;
input[15:0]		regFpsr;	//FPU control register.
input[3:0]		istrSxo;	//source instruction word
input[3:0]		idPcStep;	//PC Step
input[5:0]		istrMTag;	//PC Step


wire[31:0]		istrWordA;
wire[31:0]		istrWordB;
wire[31:0]		istrWordC;

wire[1:0]		istrMTagA;
wire[1:0]		istrMTagB;
wire[1:0]		istrMTagC;

wire	srXG3;

// `ifdef jx2_enable_riscv_xg3
//`ifndef def_true
// DecOpRepXG3		repXg3a(istrWord[31: 0], istrWordA, srXG3);
// DecOpRepXG3		repXg3b(istrWord[63:32], istrWordB, srXG3);
// DecOpRepXG3		repXg3c(istrWord[95:64], istrWordC, srXG3);
// `else

assign		istrWordA = istrWord[31: 0];
assign		istrWordB = istrWord[63:32];
assign		istrWordC = istrWord[95:64];

assign		istrMTagA = istrMTag[1:0];
assign		istrMTagB = istrMTag[3:2];
assign		istrMTagC = istrMTag[5:4];

// `endif

wire			srWxe;
wire			srXG2;
wire			srRiscv;
wire			srXG2RV;
wire			srUser;
wire			srSxo;
assign		srWxe	= regSr[27];
assign		srUser	= !regSr[30];
assign		srSxo	= istrSxo[0];
// assign		srRiscv	= (regSr[27:26] == 2'b01);
assign		srRiscv	= regSr[26];

`ifdef jx2_enable_riscv_xg3
assign	srXG3 = srRiscv && srWxe;
`else
assign	srXG3 = 0;
`endif

`ifdef jx2_enable_xg2mode
assign		srXG2	= regSr[23];
`else
assign		srXG2	= 0;
`endif

wire			srXG2X;
assign		srXG2X	= srXG2 || srXG3;

assign		srXG2RV	= srRiscv && srXG2;

wire			srSsc2;
wire			srSsc3;
wire			srRiscvSsc;
assign		srSsc2 = idPcStep[3:2]==2'b10;
assign		srSsc3 = idPcStep[3:2]==2'b11;
assign		srRiscvSsc = srRiscv && idPcStep[3];

// wire[2:0]		srMod;
wire[11:0]		srMod;
wire[11:0]		srModA;
wire[11:0]		srModB;
wire[11:0]		srModC;

// assign		srMod = { regSr[29], srSxo, srUser };
assign		srMod = {
	1'b0, 1'b0, 1'b0, regFpsr[4],
	1'b0, 1'b0, srWxe, srRiscv,
	srXG2, regSr[29], srSxo, srUser };

assign		srModA = { srMod[11:8], istrMTagA[1:0], srMod[5:0] };
assign		srModB = { srMod[11:8], istrMTagB[1:0], srMod[5:0] };
assign		srModC = { srMod[11:8], istrMTagC[1:0], srMod[5:0] };

`output_gpr		idRegS;
`output_gpr		idRegT;
`output_gpr		idRegM;

`output_gpr		idRegU;
`output_gpr		idRegV;
`output_gpr		idRegN;

`output_gpr		idRegX;
`output_gpr		idRegY;
`output_gpr		idRegO;

output[32:0]	idImmA;
output[8:0]		idUCmdA;
output[8:0]		idUIxtA;

output[32:0]	idImmB;
output[8:0]		idUCmdB;
output[8:0]		idUIxtB;

output[32:0]	idImmC;
output[8:0]		idUCmdC;
output[8:0]		idUIxtC;

parameter		noLane3 = 0;
parameter		noNoRiscV = 0;
parameter		fpuLowPrec = 0;
defparam		decOpFzA.fpuLowPrec		= fpuLowPrec;
defparam		decOpFzB.fpuLowPrec		= fpuLowPrec;
defparam		decOpFzC.fpuLowPrec		= fpuLowPrec;
defparam		decOpBz.fpuLowPrec		= fpuLowPrec;

wire[2:0]	opJumboWxBitsB;
wire[2:0]	opJumboWxBitsC;

assign	opJumboWxBitsB = ~istrWordA[15:13];
assign	opJumboWxBitsC = ~istrWordB[15:13];


`reg_gpr		opRegAM;
`reg_gpr		opRegAO;
`reg_gpr		opRegAN;
`reg_gpr		opRegAP;

`reg_gpr		opRegBM;
`reg_gpr		opRegBO;
`reg_gpr		opRegBN;
`reg_gpr		opRegBP;

`reg_gpr		opRegCM;
`reg_gpr		opRegCO;
`reg_gpr		opRegCN;
`reg_gpr		opRegCP;

`reg_gpr		opRegAM0;
`reg_gpr		opRegAO0;
`reg_gpr		opRegAN0;
`reg_gpr		opRegAP0;


`reg_gpr		opRegXM;
`reg_gpr		opRegXO;
`reg_gpr		opRegXN;
`reg_gpr		opRegXP;

`reg_gpr		opRegXMv;
`reg_gpr		opRegXOv;
`reg_gpr		opRegXNv;
`reg_gpr		opRegXPv;

reg[32:0]		opImmA;
reg[8:0]		opUCmdA;
reg[8:0]		opUIxtA;
reg[18:0]		opUFlA;

reg[15:0]		opFpImmA;

reg[32:0]		opImmB;
reg[8:0]		opUCmdB;
reg[8:0]		opUIxtB;

reg[32:0]		opImmC;
reg[8:0]		opUCmdC;
reg[8:0]		opUIxtC;

reg[32:0]		opImmA0;
reg[8:0]		opUCmdA0;
reg[8:0]		opUIxtA0;
reg[18:0]		opUFlA0;

assign	idRegS = opRegAM;
assign	idRegT = opRegAO;
assign	idRegM = opRegAN;

assign	idRegU = opRegBM;
assign	idRegV = opRegBO;
assign	idRegN = opRegBN;

assign	idRegX = opRegCM;
assign	idRegY = opRegCO;
assign	idRegO = opRegCN;

assign	idImmA = opImmA;
assign	idUCmdA = opUCmdA;
assign	idUIxtA = opUIxtA;

assign	idImmB = opImmB;
assign	idUCmdB = opUCmdB;
assign	idUIxtB = opUIxtB;

assign	idImmC = opImmC;
assign	idUCmdC = opUCmdC;
assign	idUIxtC = opUIxtC;

wire[24:0]		tOpJBitsA;
wire[24:0]		tOpJBitsB;
wire[24:0]		tOpJBitsC;

wire		opIsWexJumboA;
wire		opIsWexJumboB;
wire		opIsWexJumbo96;
reg			opIsWexJumboLdi;

wire		opIsWexJumboXA;
wire		opIsWexJumboXB;

wire		opIsRvJumboA;
wire		opIsRvJumboB;
wire		opIsRvJumbo96;

wire[24:0]		tOpRvJBitsA;
wire[24:0]		tOpRvJBitsB;
wire[24:0]		tOpRvJBitsC;

wire	opIsWexB;
wire	opIsWexaB;
wire	opIsWex2x40B;

reg	tMsgLatch;
reg	tNextMsgLatch;

reg[15:0]	tMsgLatchBPc;
reg[15:0]	tNextMsgLatchBPc;


`wire_gpr		decOpBz_idRegN;
`wire_gpr		decOpBz_idRegM;
`wire_gpr		decOpBz_idRegO;
wire[32:0]		decOpBz_idImm;
wire[8:0]		decOpBz_idUCmd;
wire[8:0]		decOpBz_idUIxt;

DecOpBz	decOpBz(
	clock,		reset,
//	istrWord[63:0],		srMod,
	{ UV32_00, istrWordA },		srMod,
	decOpBz_idRegN,		decOpBz_idRegM,
	decOpBz_idRegO,		decOpBz_idImm,
	decOpBz_idUCmd,		decOpBz_idUIxt
	);

`wire_gpr		decOpFzC_idRegN;
`wire_gpr		decOpFzC_idRegM;
`wire_gpr		decOpFzC_idRegO;
`wire_gpr		decOpFzC_idRegP;
wire[32:0]		decOpFzC_idImm;
wire[8:0]		decOpFzC_idUCmd;
wire[8:0]		decOpFzC_idUIxt;
wire[18:0]		decOpFzC_idUFl;
wire[16:0]		decOpFzC_idImmB;

DecOpFz	decOpFzC(
	clock,		reset,	srModC,
//	{ UV32_00, istrWord[95:64] },	4'h5,
	{ UV32_00, istrWordC },	4'h5,
		{	opJumboWxBitsC,
			tOpJBitsB[24], tOpJBitsC[24],
			opIsWexJumbo96,
			opIsWexJumboB | opIsWex2x40B,
			tOpJBitsC[23:0] },
		{	opJumboWxBitsB,
			1'b0, tOpJBitsB[24],
			1'b0,
			opIsWexJumboA | opIsWex2x40B,
			tOpJBitsB[23:0] },
	decOpFzC_idRegN,		decOpFzC_idRegM,
	decOpFzC_idRegO,		decOpFzC_idRegP,
	decOpFzC_idImm,			decOpFzC_idImmB,
	decOpFzC_idUCmd,		decOpFzC_idUIxt,
	decOpFzC_idUFl
	);

`wire_gpr		decOpFzB_idRegN;
`wire_gpr		decOpFzB_idRegM;
`wire_gpr		decOpFzB_idRegO;
`wire_gpr		decOpFzB_idRegP;
wire[32:0]		decOpFzB_idImm;
wire[8:0]		decOpFzB_idUCmd;
wire[8:0]		decOpFzB_idUIxt;
wire[18:0]		decOpFzB_idUFl;
wire[16:0]		decOpFzB_idImmB;

DecOpFz	decOpFzB(
	clock,		reset,	srModB,
//	{ UV32_00, istrWord[63:32] },	4'h1,
	{ UV32_00, istrWordB },	4'h1,
		{	opJumboWxBitsB,
			1'b0, tOpJBitsB[24],
			1'b0,
			opIsWexJumboA | opIsWex2x40B,
			tOpJBitsB[23:0] },
		UV31_00,
	decOpFzB_idRegN,		decOpFzB_idRegM,
	decOpFzB_idRegO,		decOpFzB_idRegP,
	decOpFzB_idImm,			decOpFzB_idImmB,
	decOpFzB_idUCmd,		decOpFzB_idUIxt,
	decOpFzB_idUFl
	);

`wire_gpr		decOpFzA_idRegN;
`wire_gpr		decOpFzA_idRegM;
`wire_gpr		decOpFzA_idRegO;
`wire_gpr		decOpFzA_idRegP;
wire[32:0]		decOpFzA_idImm;
wire[8:0]		decOpFzA_idUCmd;
wire[8:0]		decOpFzA_idUIxt;
wire[18:0]		decOpFzA_idUFl;
wire[16:0]		decOpFzA_idImmB;

DecOpFz	decOpFzA(
	clock,		reset,	srModA,
//	{ UV32_00, istrWord[31: 0] },	4'h0,
	{ UV32_00, istrWordA },	4'h0,
		UV31_00,
		UV31_00,
	decOpFzA_idRegN,		decOpFzA_idRegM,
	decOpFzA_idRegO,		decOpFzA_idRegP,
	decOpFzA_idImm,			decOpFzA_idImmB,
	decOpFzA_idUCmd,		decOpFzA_idUIxt,
	decOpFzA_idUFl
	);

`ifdef jx2_enable_riscv

`wire_gpr		decOpRvA_idRegN;
`wire_gpr		decOpRvA_idRegM;
`wire_gpr		decOpRvA_idRegO;
`wire_gpr		decOpRvA_idRegP;
wire[32:0]		decOpRvA_idImm;
wire[8:0]		decOpRvA_idUCmd;
wire[8:0]		decOpRvA_idUIxt;
wire[18:0]		decOpRvA_idUFl;
wire[16:0]		decOpRvA_idImmB;

DecOpRvI	decOpRvA(
	clock,		reset,	srModA,
//	{ UV32_00, istrWord[31: 0] },
//	{ UV32_00, istrWordA },
	{ istrWordB, istrWordA },
	{srRiscv && !srXG2RV, srWxe, 2'b00},
	UV28_00,
	decOpRvA_idRegN,		decOpRvA_idRegM,
	decOpRvA_idRegO,		decOpRvA_idRegP,
	decOpRvA_idImm,			decOpRvA_idImmB,
	decOpRvA_idUCmd,		decOpRvA_idUIxt,
	decOpRvA_idUFl
	);

`ifdef jx2_dec_ssc_riscv

`wire_gpr		decOpRvB_idRegN;
`wire_gpr		decOpRvB_idRegM;
`wire_gpr		decOpRvB_idRegO;
`wire_gpr		decOpRvB_idRegP;
wire[32:0]		decOpRvB_idImm;
wire[8:0]		decOpRvB_idUCmd;
wire[8:0]		decOpRvB_idUIxt;
wire[18:0]		decOpRvB_idUFl;
wire[16:0]		decOpRvB_idImmB;

DecOpRvI	decOpRvB(
	clock,		reset,	srModB,
//	{ UV32_00, istrWord[63:32] },
	{ UV32_00, istrWordB },
	{srRiscv && !srXG2RV, srWxe, 2'b01},
//	UV28_00,
	{ 1'b0, tOpRvJBitsB[24],
		1'b0,
		opIsRvJumboA,
		tOpRvJBitsB[23:0] },
	decOpRvB_idRegN,		decOpRvB_idRegM,
	decOpRvB_idRegO,		decOpRvB_idRegP,
	decOpRvB_idImm,			decOpRvB_idImmB,
	decOpRvB_idUCmd,		decOpRvB_idUIxt,
	decOpRvB_idUFl
	);

`endif

`ifdef jx2_enable_riscv_rvc
`wire_gpr		decOpRc_idRegN;
`wire_gpr		decOpRc_idRegM;
`wire_gpr		decOpRc_idRegO;
wire[32:0]		decOpRc_idImm;
wire[8:0]		decOpRc_idUCmd;
wire[8:0]		decOpRc_idUIxt;

DecOpRvC	decOpRc(
	clock,		reset,
//	istrWord[63:0],		srMod,
	{ UV32_00, istrWordA },		srMod,
	decOpRc_idRegN,		decOpRc_idRegM,
	decOpRc_idRegO,		decOpRc_idImm,
	decOpRc_idUCmd,		decOpRc_idUIxt
	);
`endif

`ifdef jx2_enable_rvjumbo96
`wire_gpr		decOpRvC_idRegN;
`wire_gpr		decOpRvC_idRegM;
`wire_gpr		decOpRvC_idRegO;
`wire_gpr		decOpRvC_idRegP;
wire[32:0]		decOpRvC_idImm;
wire[8:0]		decOpRvC_idUCmd;
wire[8:0]		decOpRvC_idUIxt;
wire[18:0]		decOpRvC_idUFl;
wire[16:0]		decOpRvC_idImmB;

`ifdef jx2_dec_ssc3_riscv
DecOpRvI	decOpRvC(
`else
DecOpRvJO	decOpRvC(
`endif
	clock,		reset,	srModC,
//	{ UV32_00, istrWord[95:64] },
	{ UV32_00, istrWordC },
	{srRiscv && !srXG2RV, srWxe, 2'b10},
//	UV28_00,
	{ tOpRvJBitsB[24], tOpRvJBitsC[24],
		opIsRvJumbo96,
		opIsRvJumboB,
		tOpRvJBitsC[23:0] },
	decOpRvC_idRegN,		decOpRvC_idRegM,
	decOpRvC_idRegO,		decOpRvC_idRegP,
	decOpRvC_idImm,			decOpRvC_idImmB,
	decOpRvC_idUCmd,		decOpRvC_idUIxt,
	decOpRvC_idUFl
	);
`endif

`endif

reg opIsFxA;
reg opIsFzA;
reg opIsFCA;
reg opIsDzA;	//Predicated Ops
reg opIsDwA;		//PrWEX Ops
reg opIsDfA;	//Pred-False or WEX
reg opIsWfA;	//WEX

reg opIsScalar;		//Scalar Operation
reg opIsScalarBase;	//Scalar Operation (Single Op Word)
reg opIsDualLane;	//Op uses both lanes
reg opIsDualLaneRm;	//Op uses both lanes (with Rm as 128b)
reg opIsDualLaneRn;	//Op uses both lanes (with Rn as 128b)
reg opIsDualLaneRo;	//Op uses both lanes (with Ro as 128b)
reg opDualLaneSw;	//Dual lane op but swap A / B regs.

reg opIsBaseRm;		//Rm is a base register

reg opIsFxB;
reg opIsFzB;
reg opIsFCB;
reg opIsDzB;		//Predicated Ops
reg opIsDwB;		//PrWEX Ops
reg opIsDfB;		//Pred-False or WEX
reg opIsWfB;		//WEX

reg opIsFxC;
reg opIsFzC;
reg opIsFCC;
reg opIsDzC;		//Predicated Ops
reg opIsDwC;		//PrWEX Ops
reg opIsDfC;		//Pred-False or WEX
reg opIsWfC;		//WEX

wire[6:0]	opJumbo96WxBits;

`ifdef jx2_enable_wexjumbo

`ifdef jx2_enable_riscv
wire	srNoJumbo =
	(srRiscv && !srXG2RV);
`else
wire	srNoJumbo = 0;
`endif

//assign	opIsWexJumboA =
//		(istrWord[15: 8] == 8'b1111_0100) &&
//		(istrWord[31:30] == 2'b11       ) ;
//assign	opIsWexJumboB =
//		(istrWord[47:40] == 8'b1111_0100) &&
//		(istrWord[63:62] == 2'b11       ) ;

// assign	opIsWexJumboA =
//		(istrWord[15: 8] == 8'b1111_1110) ;
// assign	opIsWexJumboB =
//		(istrWord[47:40] == 8'b1111_1110) ;

assign	opIsWexJumboA =
//		(istrWord[15: 9] == 7'b1111_111) && !srNoJumbo;
//		(istrWord[12: 9] == 4'b1111) ;
//		(istrWord[12: 9] == 4'b1111) &&
//		((istrWord[15:13] == 3'b111) || srXG2X) && !srNoJumbo ;
		 (istrWordA[12: 9] == 4'b1111) &&
		((istrWordA[15:13] == 3'b111 ) || srXG2X) &&
			(!srNoJumbo || istrMTagA[0]) ;

assign	opIsWexJumboB =
//		(istrWord[47:41] == 7'b1111_111) && !srNoJumbo;
//		(istrWord[44:41] == 4'b1111) ;
//		(istrWord[44:41] == 4'b1111) &&
//		((istrWord[47:45] == 3'b111) || srXG2X) && !srNoJumbo ;
		 (istrWordB[12: 9] == 4'b1111) &&
		((istrWordB[15:13] == 3'b111 ) || srXG2X) &&
			(!srNoJumbo || istrMTagB[0]) ;

assign	opIsWexJumboXA =
//		((istrWord[15:12] == 4'b0111) && (istrWord[10:8] == 3'b000)) ||
//		((istrWord[15:12] == 4'b1001) && (istrWord[10:9] == 2'b00 )) ;
		((istrWordA[15:12] == 4'b0111) && (istrWordA[10:8] == 3'b000)) ||
		((istrWordA[15:12] == 4'b1001) && (istrWordA[10:9] == 2'b00 )) ;
assign	opIsWexJumboXB =
//		((istrWord[47:44] == 4'b0111) && (istrWord[42:40] == 3'b000)) ||
//		((istrWord[47:44] == 4'b1001) && (istrWord[42:41] == 2'b00 )) ;
		((istrWordB[15:12] == 4'b0111) && (istrWordB[10:8] == 3'b000)) ||
		((istrWordB[15:12] == 4'b1001) && (istrWordB[10:9] == 2'b00 )) ;

assign	opIsWexJumbo96 =
//	opIsWexJumboA && istrWord[42];
	opIsWexJumboA && opIsWexJumboB;

assign	opJumbo96WxBits =
//	srXG2 ? { ~istrWord[15:13], ~istrWord[47:45], ~istrWord[61] } : 7'h00;
	srXG2X ? { ~istrWordA[15:13], ~istrWordB[15:13], ~istrWordB[29] } : 7'h00;

assign	opIsWexB =
//	((istrWord[47:44] == 4'b1111) && istrWord[42]) ||
//	((istrWord[47:44] == 4'b1110) && (istrWord[43:41]==3'b101));
	((istrWordB[15:12] == 4'b1111) && istrWordB[10]) ||
	((istrWordB[15:12] == 4'b1110) && (istrWordB[11:9]==3'b101));

assign	opIsWexaB =
//	((istrWord[47:44] == 4'b1111) && (istrWord[43:42]==2'b01 )) ||
//	((istrWord[47:44] == 4'b1110) && (istrWord[43:41]==3'b101));
	((istrWordB[15:12] == 4'b1111) && (istrWordB[11:10]==2'b01 )) ||
	((istrWordB[15:12] == 4'b1110) && (istrWordB[11: 9]==3'b101));

`ifdef jx2_enable_2x40b
assign	opIsWex2x40B =
//	opIsWexJumboA && istrWord[8] && opIsWexaB;
//	opIsWexJumboXA && opIsWexaB;
	opIsWexJumboXA && opIsWexaB && !srXG2X;
`else
assign	opIsWex2x40B = 0;
`endif

assign	tOpJBitsA		= 0;
// assign	tOpJBitsB		= { istrWord [7: 0], istrWord[29:16] };
// assign	tOpJBitsC		= { istrWord[39:32], istrWord[61:48] };

// assign	tOpJBitsB		= { istrWord [8: 0], istrWord[31:16] };
// assign	tOpJBitsC		= { istrWord[40:32], istrWord[63:48] };

wire		opIsWex2x40B_RiAltC;
assign		opIsWex2x40B_RiAltC =
//	(istrWord[53:52] == 2'b00) &&
//	(istrWord[61:60] == 2'b00) &&
//	istrWord[50] && istrWord[27] ;

	(istrWordB[21:20] == 2'b00) &&
	(istrWordB[29:28] == 2'b00) &&
	istrWordB[18] && istrWordA[27] ;

`ifdef jx2_enable_2x40b

`ifndef def_true
assign	tOpJBitsB		=
	opIsWex2x40B ? {
//		1'b1, istrWord [7:4], 8'h00,
		1'b1, istrWordA[7:4], 8'h00,
//		istrWord[31] ?
//			{ 1'b1, istrWord[30:29],
//				istrWord[4] ? 4'hF : 4'h0, istrWord[28] } :
//			{ 1'b0, istrWord[4] ? 4'hF : 4'h0, istrWord[30:28] },

//		{ istrWord[31], istrWord[4] ? 4'hF : 4'h0, istrWord[30:28] },
		
//		{ istrWord[31], istrWord[30] ? 4'hF : 4'h0, istrWord[30:28] },
//		istrWord[23:20] } :
//	{ istrWord [8: 0], istrWord[31:16] };

		{ istrWordA[31], istrWordA[30] ? 4'hF : 4'h0, istrWordA[30:28] },
		istrWordA[23:20] } :
	{ istrWordA[8: 0], istrWordA[31:16] };

assign	tOpJBitsC		=
	opIsWex2x40B ? {
//		1'b1, istrWord [3:0], 8'h00,
		1'b1, istrWordA[3:0], 8'h00,
//		{ istrWord[27], istrWord[0] ? 4'hF : 4'h0, istrWord[26:24] },
//		{ istrWord[27], istrWord[26] ? 4'hF : 4'h0, istrWord[26:24] },
		{ istrWord[27],
			opIsWex2x40B_RiAltC ?
				istrWord[25:24] :
				(istrWord[26] ? 2'b11 : 2'b00),
			istrWord[26] ? 3'b111 : 3'b000,
			opIsWex2x40B_RiAltC ?
				( istrWord[26] ? 2'b11 : 2'b00 ) :
				istrWord[25:24]
			},
		istrWord[19:16] } :
	{ istrWord[40:32], istrWord[63:48] };
`endif

`ifdef def_true
assign	tOpJBitsB		=
	opIsWex2x40B ? {
		1'b1, istrWordA[7:4], 8'h00,
		{ istrWordA[31], istrWordA[30] ? 4'hF : 4'h0, istrWordA[30:28] },
		istrWordA[23:20] } :
	{ istrWordA[8:0], istrWordA[31:16] };

assign	tOpJBitsC		=
	opIsWex2x40B ? {
		1'b1, istrWordA[3:0], 8'h00,
		{ istrWordA[27],
			opIsWex2x40B_RiAltC ?
				istrWordA[25:24] :
				(istrWordA[26] ? 2'b11 : 2'b00),
			istrWordA[26] ? 3'b111 : 3'b000,
			opIsWex2x40B_RiAltC ?
				( istrWordA[26] ? 2'b11 : 2'b00 ) :
				istrWordA[25:24]
			},
		istrWordA[19:16] } :
	{ istrWordB[8:0], istrWordB[31:16] };
`endif

`else
// assign	tOpJBitsB		= { istrWord [8: 0], istrWord[31:16] };
// assign	tOpJBitsC		= { istrWord[40:32], istrWord[63:48] };
assign	tOpJBitsB		= { istrWordA[8:0], istrWordA[31:16] };
 assign	tOpJBitsC		= { istrWordB[8:0], istrWordB[31:16] };
`endif

`else

assign	opIsWexJumboA	= 0;
assign	opIsWexJumboB	= 0;
assign	opIsWexJumbo96	= 0;
assign	tOpJBitsA		= 0;
assign	tOpJBitsB		= 0;
assign	tOpJBitsC		= 0;
assign	opJumbo96WxBits = 0;


`endif


`ifdef jx2_enable_riscv
`ifdef jx2_enable_rvjumbo

wire		opIsRvJumboA_En;
wire		opIsRvJumboB_En;
wire		opIsRvJumboA_0;
wire		opIsRvJumboA_1;
wire		opIsRvJumboB_0;
wire		opIsRvJumboB_1;

// assign		opIsRvJumboA_En = (srRiscv && !srXG2RV && !istrMTagA[0]);
// assign		opIsRvJumboB_En = (srRiscv && !srXG2RV && !istrMTagB[0]);

assign		opIsRvJumboA_En = (srRiscv && !istrMTagA[0]);
assign		opIsRvJumboB_En = (srRiscv && !istrMTagB[0]);

assign		opIsRvJumboA_0 =
	(istrWordA[ 6: 0] == 7'h1B) && (istrWordA[14:12] == 3'h4);
assign		opIsRvJumboB_0 =
	(istrWordB[ 6: 0] == 7'h1B) && (istrWordB[14:12] == 3'h4);

assign		opIsRvJumboA_1 =
	(istrWordA[ 6: 0] == 7'h3F) && (istrWordA[14:12] == 3'h4);
assign		opIsRvJumboB_1 =
	(istrWordB[ 6: 0] == 7'h3F) && (istrWordB[14:12] == 3'h4);

assign		opIsRvJumboA = (opIsRvJumboA_0 || opIsRvJumboA_1) && 
				opIsRvJumboA_En;
assign		opIsRvJumboB = (opIsRvJumboB_0 || opIsRvJumboB_1) && 
				opIsRvJumboB_En;

`ifdef jx2_enable_rvjumbo96
assign		opIsRvJumbo96 = opIsRvJumboA && opIsRvJumboB;
`else
assign		opIsRvJumbo96 = 0;
`endif

assign		tOpRvJBitsA = 0;

assign		tOpRvJBitsB = {
	istrWordA[31],
	istrWordA[31], 2'b00,
	istrWordA[11: 7], istrWordA[19:15], istrWordA[30:20] };

assign		tOpRvJBitsC = {
	istrWordB[31],
	istrWordB[31], 2'b00,
	istrWordB[11: 7], istrWordB[19:15], istrWordB[30:20] };

`else

assign		opIsRvJumboA = 0;
assign		opIsRvJumboB = 0;
assign		opIsRvJumbo96 = opIsRvJumboA && opIsRvJumboB;

assign		tOpRvJBitsA = 0;
assign		tOpRvJBitsB = 0;
assign		tOpRvJBitsC = 0;

`endif
`endif


// `reg_gpr	opCmRemapRxA[31:0];

`reg_gpr	opCmRemapRxA[63:0];
`reg_gpr	opCmRemapRxB[63:0];

`ifdef jx2_enable_xgpr

initial
begin
`ifdef def_true
	opCmRemapRxA[6'h00]=JX2_GR_DLR;
	opCmRemapRxA[6'h01]=JX2_GR_R32;
	opCmRemapRxA[6'h02]=JX2_GR_R2 ;
	opCmRemapRxA[6'h03]=JX2_GR_R34;
	opCmRemapRxA[6'h04]=JX2_GR_R4 ;
	opCmRemapRxA[6'h05]=JX2_GR_R36;
	opCmRemapRxA[6'h06]=JX2_GR_R6 ;
	opCmRemapRxA[6'h07]=JX2_GR_R38;
	opCmRemapRxA[6'h08]=JX2_GR_R8 ;
	opCmRemapRxA[6'h09]=JX2_GR_R40;
	opCmRemapRxA[6'h0A]=JX2_GR_R10;
	opCmRemapRxA[6'h0B]=JX2_GR_R42;
	opCmRemapRxA[6'h0C]=JX2_GR_R12;
	opCmRemapRxA[6'h0D]=JX2_GR_R44;
	opCmRemapRxA[6'h0E]=JX2_GR_R14;
	opCmRemapRxA[6'h0F]=JX2_GR_R46;
	opCmRemapRxA[6'h10]=JX2_GR_R16;
	opCmRemapRxA[6'h11]=JX2_GR_R48;
	opCmRemapRxA[6'h12]=JX2_GR_R18;
	opCmRemapRxA[6'h13]=JX2_GR_R50;
	opCmRemapRxA[6'h14]=JX2_GR_R20;
	opCmRemapRxA[6'h15]=JX2_GR_R52;
	opCmRemapRxA[6'h16]=JX2_GR_R22;
	opCmRemapRxA[6'h17]=JX2_GR_R54;
	opCmRemapRxA[6'h18]=JX2_GR_R24;
	opCmRemapRxA[6'h19]=JX2_GR_R56;
	opCmRemapRxA[6'h1A]=JX2_GR_R26;
	opCmRemapRxA[6'h1B]=JX2_GR_R58;
	opCmRemapRxA[6'h1C]=JX2_GR_R28;
	opCmRemapRxA[6'h1D]=JX2_GR_R60;
	opCmRemapRxA[6'h1E]=JX2_GR_R30;
	opCmRemapRxA[6'h1F]=JX2_GR_R62;

	opCmRemapRxB[6'h00]=JX2_GR_DHR;
	opCmRemapRxB[6'h01]=JX2_GR_R33;
	opCmRemapRxB[6'h02]=JX2_GR_R3 ;
	opCmRemapRxB[6'h03]=JX2_GR_R35;
	opCmRemapRxB[6'h04]=JX2_GR_R5 ;
	opCmRemapRxB[6'h05]=JX2_GR_R37;
	opCmRemapRxB[6'h06]=JX2_GR_R7 ;
	opCmRemapRxB[6'h07]=JX2_GR_R39;
	opCmRemapRxB[6'h08]=JX2_GR_R9 ;
	opCmRemapRxB[6'h09]=JX2_GR_R41;
	opCmRemapRxB[6'h0A]=JX2_GR_R11;
	opCmRemapRxB[6'h0B]=JX2_GR_R43;
	opCmRemapRxB[6'h0C]=JX2_GR_R13;
	opCmRemapRxB[6'h0D]=JX2_GR_R45;
	opCmRemapRxB[6'h0E]=JX2_GR_R15;
	opCmRemapRxB[6'h0F]=JX2_GR_R47;
	opCmRemapRxB[6'h10]=JX2_GR_R17;
	opCmRemapRxB[6'h11]=JX2_GR_R49;
	opCmRemapRxB[6'h12]=JX2_GR_R19;
	opCmRemapRxB[6'h13]=JX2_GR_R51;
	opCmRemapRxB[6'h14]=JX2_GR_R21;
	opCmRemapRxB[6'h15]=JX2_GR_R53;
	opCmRemapRxB[6'h16]=JX2_GR_R23;
	opCmRemapRxB[6'h17]=JX2_GR_R55;
	opCmRemapRxB[6'h18]=JX2_GR_R25;
	opCmRemapRxB[6'h19]=JX2_GR_R57;
	opCmRemapRxB[6'h1A]=JX2_GR_R27;
	opCmRemapRxB[6'h1B]=JX2_GR_R59;
	opCmRemapRxB[6'h1C]=JX2_GR_R29;
	opCmRemapRxB[6'h1D]=JX2_GR_R61;
	opCmRemapRxB[6'h1E]=JX2_GR_R31;
	opCmRemapRxB[6'h1F]=JX2_GR_R63;

	opCmRemapRxA[6'h20]=JX2_GR_PC;
	opCmRemapRxA[6'h21]=JX2_GR_LR;
	opCmRemapRxA[6'h22]=JX2_GR_SR;
	opCmRemapRxA[6'h23]=JX2_GR_VBR;
	opCmRemapRxA[6'h24]=JX2_GR_SPC;
	opCmRemapRxA[6'h25]=JX2_GR_R46;
	opCmRemapRxA[6'h26]=JX2_GR_GBR;
	opCmRemapRxA[6'h27]=JX2_GR_ZZR;
	opCmRemapRxA[6'h28]=JX2_GR_ZZR;
	opCmRemapRxA[6'h29]=JX2_GR_ZZR;
	opCmRemapRxA[6'h2A]=JX2_GR_ZZR;
	opCmRemapRxA[6'h2B]=JX2_GR_ZZR;
	opCmRemapRxA[6'h2C]=JX2_GR_ZZR;
	opCmRemapRxA[6'h2D]=JX2_GR_ZZR;
	opCmRemapRxA[6'h2E]=JX2_GR_ZZR;
	opCmRemapRxA[6'h2F]=JX2_GR_ZZR;
	opCmRemapRxA[6'h30]=JX2_GR_PC2;
	opCmRemapRxA[6'h31]=JX2_GR_ZZR;
	opCmRemapRxA[6'h32]=JX2_GR_ZZR;
	opCmRemapRxA[6'h33]=JX2_GR_ZZR;
	opCmRemapRxA[6'h34]=JX2_GR_SPC2;
	opCmRemapRxA[6'h35]=JX2_GR_ZZR;
	opCmRemapRxA[6'h36]=JX2_GR_GBR2;
	opCmRemapRxA[6'h37]=JX2_GR_ZZR;
	opCmRemapRxA[6'h38]=JX2_GR_ZZR;
	opCmRemapRxA[6'h39]=JX2_GR_ZZR;
	opCmRemapRxA[6'h3A]=JX2_GR_ZZR;
	opCmRemapRxA[6'h3B]=JX2_GR_ZZR;
	opCmRemapRxA[6'h3C]=JX2_GR_ZZR;
	opCmRemapRxA[6'h3D]=JX2_GR_ZZR;
	opCmRemapRxA[6'h3E]=JX2_GR_IMM;
	opCmRemapRxA[6'h3F]=JX2_GR_ZZR;

	opCmRemapRxB[6'h20]=JX2_GR_PC_HI;
	opCmRemapRxB[6'h21]=JX2_GR_ZZR;
	opCmRemapRxB[6'h22]=JX2_GR_ZZR;
	opCmRemapRxB[6'h23]=JX2_GR_VBR_HI;
	opCmRemapRxB[6'h24]=JX2_GR_SPC_HI;
	opCmRemapRxB[6'h25]=JX2_GR_R47;
	opCmRemapRxB[6'h26]=JX2_GR_GBR_HI;
	opCmRemapRxB[6'h27]=JX2_GR_ZZR;
	opCmRemapRxB[6'h28]=JX2_GR_ZZR;
	opCmRemapRxB[6'h29]=JX2_GR_ZZR;
	opCmRemapRxB[6'h2A]=JX2_GR_ZZR;
	opCmRemapRxB[6'h2B]=JX2_GR_ZZR;
	opCmRemapRxB[6'h2C]=JX2_GR_ZZR;
	opCmRemapRxB[6'h2D]=JX2_GR_ZZR;
	opCmRemapRxB[6'h2E]=JX2_GR_ZZR;
	opCmRemapRxB[6'h2F]=JX2_GR_ZZR;
	opCmRemapRxB[6'h30]=JX2_GR_PC2_HI;
	opCmRemapRxB[6'h31]=JX2_GR_ZZR;
	opCmRemapRxB[6'h32]=JX2_GR_ZZR;
	opCmRemapRxB[6'h33]=JX2_GR_ZZR;
	opCmRemapRxB[6'h34]=JX2_GR_SPC2_HI;
	opCmRemapRxB[6'h35]=JX2_GR_ZZR;
	opCmRemapRxB[6'h36]=JX2_GR_GBR2_HI;
	opCmRemapRxB[6'h37]=JX2_GR_ZZR;
	opCmRemapRxB[6'h38]=JX2_GR_ZZR;
	opCmRemapRxB[6'h39]=JX2_GR_ZZR;
	opCmRemapRxB[6'h3A]=JX2_GR_ZZR;
	opCmRemapRxB[6'h3B]=JX2_GR_ZZR;
	opCmRemapRxB[6'h3C]=JX2_GR_ZZR;
	opCmRemapRxB[6'h3D]=JX2_GR_ZZR;
	opCmRemapRxB[6'h3E]=JX2_GR_IMM;
	opCmRemapRxB[6'h3F]=JX2_GR_ZZR;
`endif

end

`endif

always @*
begin
	opUCmdA		= UV9_00;
	opUIxtA		= UV9_00;
	opImmA		= UV33_00;
	opRegAM		= JX2_GR_ZZR;
	opRegAO		= JX2_GR_ZZR;
	opRegAN		= JX2_GR_ZZR;
	opRegAP		= JX2_GR_ZZR;

	opUCmdB		= UV9_00;
	opUIxtB		= UV9_00;
	opImmB		= UV33_00;
	opRegBM		= JX2_GR_ZZR;
	opRegBO		= JX2_GR_ZZR;
	opRegBN		= JX2_GR_ZZR;
	opRegBP		= JX2_GR_ZZR;

	opUCmdC		= UV9_00;
	opUIxtC		= UV9_00;
	opImmC		= UV33_00;
	opRegCM		= JX2_GR_ZZR;
	opRegCO		= JX2_GR_ZZR;
	opRegCN		= JX2_GR_ZZR;
	opRegCP		= JX2_GR_ZZR;

	opRegXM		= JX2_GR_ZZR;
	opRegXO		= JX2_GR_ZZR;
	opRegXN		= JX2_GR_ZZR;
	opRegXP		= JX2_GR_ZZR;

	opRegXPv	= JX2_GR_ZZR;
	opRegXNv	= JX2_GR_ZZR;
	opRegXMv	= JX2_GR_ZZR;
	opRegXOv	= JX2_GR_ZZR;

	opUFlA		= 0;
	opImmA0		= 0;

	tNextMsgLatch	= 0;

//	opRegXP			= 0;
//	opRegXN			= 0;
//	opRegXM			= 0;
//	opRegXO			= 0;


	opIsDwA			= 0;
	opIsDwB			= 0;
	opIsDwC			= 0;
	opIsWexJumboLdi	= 0;
	opIsScalar		= 0;
	opIsScalarBase	= 0;
	opIsDualLane	= 0;
//	opIsDualLane3R	= 0;
//	opIsDualLane2R	= 0;
	opIsDualLaneRm	= 0;
	opIsDualLaneRn	= 0;
	opIsDualLaneRo	= 0;
	opDualLaneSw	= 0;
	opIsBaseRm		= 0;

`ifndef def_true

//	casez(istrWord[15:10])
	casez(istrWordA[15:10])
`ifdef jx2_enable_xgpr
		6'b0111zz: begin	//70..7F
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
			opIsDfA = istrWord[11];
		end

		6'b1001zz: begin	//70..7F
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
			opIsDfA = istrWord[11];
		end
`endif

		6'b1110zz: begin	//E0..EF
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 1;
			opIsDfA = istrWord[10];
			opIsDwA = istrWord[11] && istrWord[9];
		end

		6'b1111zz: begin	//F0..FF
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
			opIsDfA = istrWord[10];
		end

		default: begin
			opIsFxA = 0;	opIsFzA = 0;
			opIsFCA = 0;	opIsDzA = 0;
			opIsDfA = 0;
		end
	endcase

//	casez(istrWord[47:42])
	casez(istrWordB[15:10])
`ifdef jx2_enable_xgpr
		6'b0111zz: begin	//70..7F
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
			opIsDfB = istrWord[43];
		end

		6'b1001zz: begin	//70..7F
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
			opIsDfB = istrWord[43];
		end
`endif

		6'b1110zz: begin	//E0..EF
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 1;
			opIsDfB = istrWord[42];
			opIsDwB = istrWord[43] && istrWord[41];
		end

		6'b1111zz: begin	//F0..FF
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
			opIsDfB = istrWord[42];
		end

		default: begin
			opIsFxB = 0;	opIsFzB = 0;
			opIsFCB = 0;	opIsDzB = 0;
			opIsDfB = 0;
		end
	endcase

//	casez(istrWord[79:74])
	casez(istrWordC[15:10])
`ifdef jx2_enable_xgpr
		6'b0111zz: begin	//70..7F
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 0;
			opIsDfC = istrWord[75];
		end

		6'b1001zz: begin	//70..7F
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 0;
			opIsDfC = istrWord[75];
		end
`endif

		6'b1110zz: begin	//E0..EF
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 1;
			opIsDfC = istrWord[74];
			opIsDwC = istrWord[75] && istrWord[73];
		end

		6'b1111zz: begin	//F0..FF
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 0;
			opIsDfC = istrWord[74];
		end

		default: begin
			opIsFxC = 0;	opIsFzC = 0;
			opIsFCC = 0;	opIsDzC = 0;
			opIsDfC = 0;
		end
	endcase

`endif


`ifdef def_true

//	casez({srXG2X, istrWord[15:10]})
	casez({srXG2X, istrWordA[15:10]})
`ifdef jx2_enable_xgpr
		7'b0_0111zz: begin	//70..7F
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
//			opIsDfA = istrWord[11];
			opIsDfA = istrWordA[11];
		end

		7'b0_1001zz: begin	//70..7F
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
//			opIsDfA = istrWord[11];
			opIsDfA = istrWordA[11];
		end
`endif

		7'b0_1110zz, 7'b1_zzz0zz: begin	//E0..EF
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 1;
//			opIsDfA = istrWord[10];
//			opIsDwA = istrWord[11] && istrWord[9];
			opIsDfA = istrWordA[10];
			opIsDwA = istrWordA[11] && istrWordA[9];
		end

		7'b0_1111zz, 7'b1_zzz1zz: begin	//F0..FF
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
//			opIsDfA = istrWord[10];
			opIsDfA = istrWordA[10];
		end

		default: begin
			opIsFxA = 0;	opIsFzA = 0;
			opIsFCA = 0;	opIsDzA = 0;
			opIsDfA = 0;
		end
	endcase

//	casez({srXG2X, istrWord[47:42]})
	casez({srXG2X, istrWordB[15:10]})
`ifdef jx2_enable_xgpr
		7'b0_0111zz: begin	//70..7F
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
//			opIsDfB = istrWord[43];
			opIsDfB = istrWordB[11];
		end

		7'b0_1001zz: begin	//70..7F
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
//			opIsDfB = istrWord[43];
			opIsDfB = istrWordB[11];
		end
`endif

		7'b0_1110zz, 7'b1_zzz0zz: begin	//E0..EF
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 1;
//			opIsDfB = istrWord[42];
//			opIsDwB = istrWord[43] && istrWord[41];
			opIsDfB = istrWordB[10];
			opIsDwB = istrWordB[11] && istrWordB[9];
		end

		7'b0_1111zz, 7'b1_zzz1zz: begin	//F0..FF
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
//			opIsDfB = istrWord[42];
			opIsDfB = istrWordB[10];
		end

		default: begin
			opIsFxB = 0;	opIsFzB = 0;
			opIsFCB = 0;	opIsDzB = 0;
			opIsDfB = 0;
		end
	endcase

//	casez({srXG2X, istrWord[79:74]})
	casez({srXG2X, istrWordC[15:10]})
`ifdef jx2_enable_xgpr
		7'b0_0111zz: begin	//70..7F
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 0;
//			opIsDfC = istrWord[75];
			opIsDfC = istrWordC[11];
		end

		7'b0_1001zz: begin	//70..7F
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 0;
//			opIsDfC = istrWord[75];
			opIsDfC = istrWordC[11];
		end
`endif

		7'b0_1110zz, 7'b1_zzz0zz: begin	//E0..EF
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 1;
//			opIsDfC = istrWord[74];
//			opIsDwC = istrWord[75] && istrWord[73];
			opIsDfC = istrWordC[10];
			opIsDwC = istrWordC[11] && istrWordC[9];
		end

		7'b0_1111zz, 7'b1_zzz1zz: begin	//F0..FF
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 0;
//			opIsDfC = istrWord[74];
			opIsDfC = istrWordC[10];
		end

		default: begin
			opIsFxC = 0;	opIsFzC = 0;
			opIsFCC = 0;	opIsDzC = 0;
			opIsDfC = 0;
		end
	endcase

`endif

	tNextMsgLatchBPc = istrBPc[15:0];

	if(srXG3)
	begin
		if((istrBPc[1:0]!=2'b00) || (idPcStep[1:0]!=2'b00))
		begin
			if(!tMsgLatch && (tMsgLatchBPc!=tNextMsgLatchBPc))
			begin
				$display("DecOpWx3: XG3 Bad PC/Step PC=%X St=%X",
					istrBPc, idPcStep);
			end
			tNextMsgLatch = 1;
		end
	end

`ifdef jx2_enable_riscv
//	if(srRiscv)
	if(srRiscv && !srXG2RV)
	begin
//		if(istrWord[1:0]==2'b11)
//		if(istrWordA[1:0]==2'b11)
		if((istrWordA[1:0]==2'b11) || srXG3 || istrMTagA[0])
		begin
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
//			opIsDfA = istrWord[10];
			opIsDfA = 0;
		end
		else
		begin
			opIsFxA = 0;		opIsFzA = 0;
			opIsFCA = 0;		opIsDzA = 0;
			opIsDfA = 0;
		end
		
`ifndef jx2_dec_ssc3_riscv
		if(srSsc3 &&
			(!istrMTagA[0] || !istrMTagB[0] || !istrMTagC[0]) && 
			!opIsRvJumbo96)
		begin
			$display("DecOpWx3: 3-Wide RV");
		end
`endif
		
		if(opIsRvJumboA && !srSsc2 && !srSsc3)
		begin
			$display("DecOpWx3: RV Jumbo and not Ssc2");
		end
		
		if(opIsRvJumboA && istrMTagB[0])
		begin
			$display("DecOpWx3: RV Jumbo and XG3 Op");
		end

		if(opIsWexJumboA && !istrMTagB[0])
		begin
			$display("DecOpWx3: WEX Jumbo and RV Op");
		end
		
		if(srSsc2 && (istrMTagA[0] != istrMTagB[0]))
		begin
			$display("DecOpWx3: RV Mixed Bundle");
		end
	end
`endif

// `ifdef jx2_enable_xg2mode
`ifndef def_true
	if(srXG2X)
	begin
		opIsFxA = 1;		opIsFzA = 1;
		opIsFxB = 1;		opIsFzB = 1;
		opIsFxC = 1;		opIsFzC = 1;
		opIsFCA = 0;		opIsFCB = 0;
		opIsFCC = 0;
//		opIsDfA = istrWord[10];
//		opIsDfB = istrWord[42];
//		opIsDfC = istrWord[74];
		opIsDfA = istrWordA[10];
		opIsDfB = istrWordB[10];
		opIsDfC = istrWordC[10];
		opIsDzA = 0;
		opIsDzB = 0;
		opIsDzC = 0;

//		if(!istrWord[12])
		if(!istrWordA[12])
		begin
			opIsDzA = 1;
//			opIsDwA = istrWord[11] && istrWord[9];
			opIsDwA = istrWordA[11] && istrWordA[9];
		end

//		if(!istrWord[44])
		if(!istrWordB[12])
		begin
			opIsDzB = 1;
//			opIsDwB = istrWord[43] && istrWord[41];
			opIsDwB = istrWordB[11] && istrWordB[9];
		end

//		if(!istrWord[76])
		if(!istrWordC[12])
		begin
			opIsDzC = 1;
//			opIsDwC = istrWord[75] && istrWord[73];
			opIsDwC = istrWordC[11] && istrWordC[9];
		end
	end
`endif

//	opIsWfA = opIsDfA && !opIsDzA && srWxe;
//	opIsWfB = opIsDfB && !opIsDzB && srWxe;

//	opIsWfA = opIsDfA && (!opIsDzA || opIsDwA) && srWxe;
//	opIsWfB = opIsDfB && (!opIsDzB || opIsDwB) && srWxe;
	opIsWfA = ((opIsDfA && !opIsDzA) || opIsDwA) && srWxe;
	opIsWfB = ((opIsDfB && !opIsDzB) || opIsDwB) && srWxe;

// `ifdef jx2_enable_rvjumbo96
`ifndef def_true
	if(opIsRvJumbo96)
	begin
		opImmA	= decOpFzC_idImm;

		opRegAM	= decOpRvC_idRegM;
		opRegAO	= decOpRvC_idRegO;
		opRegAN	= decOpRvC_idRegN;
		opImmA	= decOpRvC_idImm;
		opUCmdA	= decOpRvC_idUCmd;
		opUIxtA	= decOpRvC_idUIxt;

		opRegAM0	= decOpRvC_idRegM;
		opRegAO0	= decOpRvC_idRegO;
		opRegAN0	= decOpRvC_idRegN;
		opRegAP0	= decOpRvC_idRegP;
		opUCmdA0	= decOpRvC_idUCmd;
		opUIxtA0	= decOpRvC_idUIxt;
		opUFlA0		= decOpRvC_idUFl;

		opRegBM	= JX2_GR_ZZR;
		opRegBO	= JX2_GR_ZZR;
		opRegBN	= JX2_GR_ZZR;

//		opImmB	= { 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };
		opImmB	= { 1'b0,
			tOpRvJBitsB[20: 0],
			tOpRvJBitsB[   23],
			tOpRvJBitsC[20:11] };

		opUCmdB	= UV9_00;
		opUIxtB	= UV9_00;

		opRegCM	= JX2_GR_ZZR;
		opRegCO	= JX2_GR_ZZR;
		opRegCN	= JX2_GR_ZZR;
		opImmC	= UV33_00;

`ifdef jx2_enable_riscv_immb
		opImmC	= {
			decOpRvC_idImmB[16] ? UV17_FF : UV17_00,
			decOpRvC_idImmB[15:0] };
`endif

		opUCmdC	= UV9_00;
		opUIxtC	= UV9_00;

		opUIxtC		= decOpRvC_idUFl[12:4];

		opIsScalar	= 1;
	end
	else
`endif

`ifdef jx2_enable_wexjumbo
	if(opIsWexJumbo96)
	begin
		opImmA	= decOpFzC_idImm;

		opRegAM	= decOpFzC_idRegM;
		opRegAO	= decOpFzC_idRegO;
		opRegAN	= decOpFzC_idRegN;
		opRegAP	= decOpFzC_idRegP;
		opImmA	= decOpFzC_idImm;
		opUCmdA	= decOpFzC_idUCmd;
		opUIxtA	= decOpFzC_idUIxt;

		opRegAM0	= decOpFzC_idRegM;
		opRegAO0	= decOpFzC_idRegO;
		opRegAN0	= decOpFzC_idRegN;
		opRegAP0	= decOpFzC_idRegP;
		opUCmdA0	= decOpFzC_idUCmd;
		opUIxtA0	= decOpFzC_idUIxt;
		opUFlA0		= decOpFzC_idUFl;

		opRegBM	= JX2_GR_ZZR;
		opRegBO	= JX2_GR_ZZR;
		opRegBN	= JX2_GR_ZZR;

`ifdef jx2_use_fpu_fpimm
		opImmB	= decOpFzC_idUFl[0] ?
//			{ 9'b0, tOpJBitsB[23:0] } :
//			{ opImmA[32] ? 9'h1FF : 9'h000, tOpJBitsB[23:0] } :
//			{ 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };

			{ (opImmA[32] ? 9'h1FF : 9'h000) ^
				{ 2'b0, opJumbo96WxBits }, tOpJBitsB[23:0] } :
			{ 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };
`else
		opImmB	= { 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };
`endif

		opUCmdB	= UV9_00;
		opUIxtB	= UV9_00;

		opRegCM	= JX2_GR_ZZR;
		opRegCO	= JX2_GR_ZZR;
		opRegCN	= JX2_GR_ZZR;
		opImmC	= UV33_00;
		opUCmdC	= UV9_00;
		opUIxtC	= UV9_00;

`ifdef jx2_enable_immb
		opImmC	= {
			decOpFzC_idImmB[16] ? UV17_FF : UV17_00,
			decOpFzC_idImmB[15:0] };
`endif

//		opUCmdC		= { 3'b001, decOpFzC_idUFl[18:13] };
		opUIxtC		= decOpFzC_idUFl[12:4];

		opIsScalar	= 1;

//		if(opIsDzC)
//		begin
//			opUCmdA[8:6]=opIsDfC?JX2_IXC_CF:JX2_IXC_CT;
//		end
	end
	else
`endif
	if(opIsFxA)
	begin
		if(srSsc3)
		begin
//			$display("DecOpWz3: WEX3");
		
			opRegAM	= decOpFzC_idRegM;
			opRegAO	= decOpFzC_idRegO;
			opRegAN	= decOpFzC_idRegN;
			opRegAP	= decOpFzC_idRegP;
			opImmA	= decOpFzC_idImm;
			opUCmdA	= decOpFzC_idUCmd;
			opUIxtA	= decOpFzC_idUIxt;

//			opRegAM0	= decOpFzC_idRegM;
//			opRegAO0	= decOpFzC_idRegO;
//			opRegAN0	= decOpFzC_idRegN;
//			opRegAP0	= decOpFzC_idRegP;
//			opUCmdA0	= decOpFzC_idUCmd;
//			opUIxtA0	= decOpFzC_idUIxt;
//			opUFlA0		= decOpFzC_idUFl;

			opRegBM	= decOpFzB_idRegM;
			opRegBO	= decOpFzB_idRegO;
			opRegBN	= decOpFzB_idRegN;
			opRegBP	= decOpFzB_idRegP;
			opImmB	= decOpFzB_idImm;
			opUCmdB	= decOpFzB_idUCmd;
			opUIxtB	= decOpFzB_idUIxt;

			opRegCM	= decOpFzA_idRegM;
			opRegCO	= decOpFzA_idRegO;
			opRegCN	= decOpFzA_idRegN;
			opRegCP	= decOpFzA_idRegP;
			opImmC	= decOpFzA_idImm;
			opUCmdC	= decOpFzA_idUCmd;
			opUIxtC	= decOpFzA_idUIxt;

`ifdef jx2_enable_riscv
`ifdef jx2_dec_ssc3_riscv
			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagC[0])
			begin
				opRegAM	= decOpRvC_idRegM;
				opRegAO	= decOpRvC_idRegO;
				opRegAN	= decOpRvC_idRegN;
				opRegAP	= decOpRvC_idRegP;
				opImmA	= decOpRvC_idImm;
				opUCmdA	= decOpRvC_idUCmd;
				opUIxtA	= decOpRvC_idUIxt;
				opUFlA	= decOpRvC_idUFl;

		//		opRegAM0	= opRegAM;
		//		opRegAO0	= opRegAO;
		//		opRegAN0	= opRegAN;
		//		opRegAP0	= opRegAP;
		//		opUCmdA0	= opUCmdA;
		//		opUIxtA0	= opUIxtA;
		//		opUFlA0		= opUFlA;

				opIsScalar	= opIsRvJumboA && opIsRvJumboB;
			end

			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagB[0])
			begin
				opRegBM	= decOpRvB_idRegM;
				opRegBO	= decOpRvB_idRegO;
				opRegBN	= decOpRvB_idRegN;
				opRegBP	= decOpRvB_idRegP;
				opImmB	= decOpRvB_idImm;
				opUCmdB	= decOpRvB_idUCmd;
				opUIxtB	= decOpRvB_idUIxt;
			end

			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagA[0])
			begin
				opRegCM	= decOpRvA_idRegM;
				opRegCO	= decOpRvA_idRegO;
				opRegCN	= decOpRvA_idRegN;
				opRegCP	= decOpRvA_idRegP;
				opImmC	= decOpRvA_idImm;
				opUCmdC	= decOpRvA_idUCmd;
				opUIxtC	= decOpRvA_idUIxt;
			end
`endif
`endif

`ifdef jx2_enable_rvjumbo96
			if(opIsRvJumbo96 && !istrMTagC[0])
			begin
				opImmB	= { 1'b0,
					tOpRvJBitsB[20: 0],
					tOpRvJBitsB[   23],
					tOpRvJBitsC[20:11] };
			end
`endif

			if(noLane3)
			begin
				opRegCM	= JX2_GR_ZZR;
				opRegCO	= JX2_GR_ZZR;
				opRegCN	= JX2_GR_ZZR;
				opImmC	= 0;
				opUCmdC	= 0;
				opUIxtC	= 0;
			end

			opRegAM0	= opRegAM;
			opRegAO0	= opRegAO;
			opRegAN0	= opRegAN;
			opRegAP0	= opRegAP;
			opUCmdA0	= opUCmdA;
			opUIxtA0	= opUIxtA;
			opUFlA0		= opUFlA;

		end
		else
		if(srSsc2)
		begin
			opRegAM	= decOpFzB_idRegM;
			opRegAO	= decOpFzB_idRegO;
			opRegAN	= decOpFzB_idRegN;
			opRegAP	= decOpFzB_idRegP;
			opImmA	= decOpFzB_idImm;
			opUCmdA	= decOpFzB_idUCmd;
			opUIxtA	= decOpFzB_idUIxt;
			opUFlA	= decOpFzB_idUFl;

			opRegBM	= decOpFzA_idRegM;
			opRegBO	= decOpFzA_idRegO;
			opRegBN	= decOpFzA_idRegN;
			opRegBP	= decOpFzA_idRegP;
			opImmB	= decOpFzA_idImm;
			opUCmdB	= decOpFzA_idUCmd;
			opUIxtB	= decOpFzA_idUIxt;

			opIsScalar	= opIsWexJumboA;

`ifdef jx2_enable_riscv
`ifdef jx2_dec_ssc_riscv
			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagB[0])
			begin
				opRegAM	= decOpRvB_idRegM;
				opRegAO	= decOpRvB_idRegO;
				opRegAN	= decOpRvB_idRegN;
				opRegAP	= decOpRvB_idRegP;
				opImmA	= decOpRvB_idImm;
				opUCmdA	= decOpRvB_idUCmd;
				opUIxtA	= decOpRvB_idUIxt;
				opUFlA	= decOpRvB_idUFl;

				opIsScalar	= opIsRvJumboA;
			end

			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagA[0])
			begin
				opRegBM	= decOpRvA_idRegM;
				opRegBO	= decOpRvA_idRegO;
				opRegBN	= decOpRvA_idRegN;
				opRegBP	= decOpRvA_idRegP;
				opImmB	= decOpRvA_idImm;
				opUCmdB	= decOpRvA_idUCmd;
				opUIxtB	= decOpRvA_idUIxt;
			end
`endif
`endif

			opRegAM0	= opRegAM;
			opRegAO0	= opRegAO;
			opRegAN0	= opRegAN;
			opRegAP0	= opRegAP;
			opUCmdA0	= opUCmdA;
			opUIxtA0	= opUIxtA;
			opUFlA0		= opUFlA;

			opRegCM	= opRegBP;
			opRegCO	= opRegAP;

			opRegCN	= JX2_GR_ZZR;
			opImmC	= UV33_00;

`ifdef jx2_enable_immb
			opImmC	= {
				decOpFzB_idImmB[16] ? UV17_FF : UV17_00,
				decOpFzB_idImmB[15:0] };
`endif

`ifdef jx2_enable_riscv_immb
`ifdef jx2_dec_ssc_riscv
			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagB[0])
			begin
				opImmC	= {
					decOpRvB_idImmB[16] ? UV17_FF : UV17_00,
					decOpRvB_idImmB[15:0] };
			end
`endif
`endif

			opUCmdC	= UV9_00;
//				opUIxtC	= UV9_00;
//				opUIxtC	= { 5'h0, decOpFzB_idUFl[7:4] };
//			opUIxtC	= decOpFzB_idUFl[12:4];
			opUIxtC	= opUFlA[12:4];

//			opUCmdC		= { 3'b001, decOpFzB_idUFl[18:13] };
			opUCmdC		= { 3'b001, opUFlA[18:13] };

			if(opIsRvJumboA || opIsWexJumboA)
			begin
				opImmB	= opImmA;

	//			if(opIsWexJumboA)
				if(opRegAO==JX2_GR_JIMM)
				begin
					opImmB	= opImmA;

					/* Jumbo24 + Imm24 */
					opImmB	= {
						opImmA[32] ? UV17_FF : UV17_00,
						tOpJBitsB[23:8] };
				end
			end
		end
		else
		begin
			opRegAM0	= decOpFzA_idRegM;
			opRegAO0	= decOpFzA_idRegO;
			opRegAN0	= decOpFzA_idRegN;
			opRegAP0	= decOpFzA_idRegP;
			opImmA0		= decOpFzA_idImm;
			opUCmdA0	= decOpFzA_idUCmd;
			opUIxtA0	= decOpFzA_idUIxt;
			opUFlA0		= decOpFzA_idUFl;

`ifdef jx2_enable_riscv
//			if(srRiscv && !noNoRiscV)
//			if(srRiscv && !noNoRiscV && !srXG2RV)
			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagA[0])
			begin
				opRegAM0	= decOpRvA_idRegM;
				opRegAO0	= decOpRvA_idRegO;
				opRegAN0	= decOpRvA_idRegN;
				opRegAP0	= decOpRvA_idRegP;
				opImmA0		= decOpRvA_idImm;
				opUCmdA0	= decOpRvA_idUCmd;
				opUIxtA0	= decOpRvA_idUIxt;
				opUFlA0		= decOpRvA_idUFl;
			end
`endif

`ifdef def_true
			opRegAM	= opRegAM0;
			opRegAO	= opRegAO0;
			opRegAN	= opRegAN0;
			opRegAP	= opRegAP0;
			opImmA	= opImmA0;
			opUCmdA	= opUCmdA0;
			opUIxtA	= opUIxtA0;
			opUFlA	= opUFlA0;
`endif

			opRegBM	= JX2_GR_ZZR;
			opRegBO	= opRegAN0;
			opRegBN	= JX2_GR_ZZR;
			opImmB	= UV33_00;
			opUCmdB	= UV9_00;
			opUIxtB	= UV9_00;
			
			opRegCM	= JX2_GR_ZZR;
			opRegCO	= opRegAP0;
			opRegCN	= JX2_GR_ZZR;
			opImmC	= UV33_00;
			opUCmdC	= UV9_00;
//			opUIxtC	= UV9_00;
//			opUIxtC	= { 5'h0, decOpFzA_idUFl[7:4] };
			opUIxtC	= decOpFzA_idUFl[12:4];

`ifdef jx2_enable_immb
			opImmC	= {
				decOpFzA_idImmB[16] ? UV17_FF : UV17_00,
				decOpFzA_idImmB[15:0] };
`endif

`ifdef jx2_enable_riscv_immb
			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagA[0])
			begin
				opImmC	= {
					decOpRvA_idImmB[16] ? UV17_FF : UV17_00,
					decOpRvA_idImmB[15:0] };
			end
`endif

`ifdef jx2_enable_riscv_zilsx
			if(srRiscv && !noNoRiscV && !srXG2RV && !istrMTagA[0])
			begin
				/* pre/post increment */
				if(decOpRvA_idUFl[3])
				begin
					opRegBM	= opRegAM;
					opRegBO	= JX2_GR_IMM;
					opRegBN	= opRegAM;
					opImmB	= opImmA;
//					opUCmdB	= { opUCmdA[8:6], JX2_UCMD_ALU3 };
//					opUIxtB	= { JX2_IUC_SC, JX2_UCIX_ALU_ADD };

					opUCmdB	= { opUCmdA[8:6], JX2_UCMD_LEA_MR };
					opUIxtB	= opUIxtA;
					opUIxtB[2] = 1;

`ifndef def_true
					case(decOpRvA_idUIxt[1:0])
						2'b00: opImmB[9:0] = opImmA[9:0];
						2'b01: opImmB[9:0] = { opImmA[8:0], 1'b0 };
						2'b10: opImmB[9:0] = { opImmA[7:0], 2'b0 };
						2'b11: opImmB[9:0] = { opImmA[6:0], 3'b0 };
					endcase
`endif
				end
			end
`endif


			if(decOpFzA_idUFl[12])
				opUCmdC		= { 3'b001, decOpFzA_idUFl[18:13] };

			opIsScalar		= 1;				
			opIsScalarBase	= 1;				
		end
	end
	else
	begin
		opRegAM	= decOpBz_idRegM;
		opRegAO	= decOpBz_idRegO;
		opRegAN	= decOpBz_idRegN;
		opRegAP	= decOpBz_idRegN;

		opImmA	= {
			decOpBz_idImm[12] ? UV20_FF : UV20_00,
			decOpBz_idImm[12:0] };

		opUCmdA	= decOpBz_idUCmd;
		opUIxtA	= decOpBz_idUIxt;

`ifdef jx2_enable_riscv
`ifdef jx2_enable_riscv_rvc
		if(srRiscv && !noNoRiscV)
		begin
			opRegAM	= decOpRc_idRegM;
			opRegAO	= decOpRc_idRegO;
			opRegAN	= decOpRc_idRegN;
			opRegAP	= decOpRc_idRegN;
			opImmA	= decOpRc_idImm;
			opUCmdA	= decOpRc_idUCmd;
			opUIxtA	= decOpRc_idUIxt;
		end
`endif
`endif

`ifdef jx2_reg_rp
		if(opUCmdA[5:0] == JX2_UCMD_MOV_RM)
			opRegAN	= JX2_GR_ZZR;
		if(opUCmdA[5:0] == JX2_UCMD_MOV_MR)
			opRegAP	= JX2_GR_ZZR;
`endif

		opRegAM0	= opRegAM;
		opRegAO0	= opRegAO;
		opRegAN0	= opRegAN;
		opRegAP0	= opRegAP;
		opUCmdA0	= opUCmdA;
		opUIxtA0	= opUIxtA;
		opUFlA0		= opUFlA;

		opRegBN	= JX2_GR_ZZR;
		opRegBM	= JX2_GR_ZZR;
		opRegBO	= opRegAP;
		opImmB	= UV33_00;
		opUCmdB	= UV9_00;
		opUIxtB	= UV9_00;

		opRegCM	= JX2_GR_ZZR;
		opRegCO	= opRegAP;
		opRegCN	= JX2_GR_ZZR;
		opImmC	= UV33_00;
		opUCmdC	= UV9_00;
		opUIxtC	= UV9_00;
		
		opIsScalar		= 1;
		opIsScalarBase	= 1;
	end

//	if(opIsScalar)
	if(1'b1)
	begin
		if(opUCmdA0[5:1] == 5'h02)
		begin
			opIsBaseRm	= 1;
		end

		if((opUIxtA0[8:6]==JX2_IUC_WX) || (opUIxtA0[8:6]==JX2_IUC_WXA))
		begin
			opIsDualLane = 1;
			opIsDualLaneRn	= 1;

			if(opUCmdA0[5:1] != 5'h02)
			begin
//				opIsDualLane3R	= 1;
//				opIsDualLane2R	= 1;
				opIsDualLaneRm	= 1;
				opIsDualLaneRo	= 1;
				opIsDualLaneRn	= 1;
			end
		end

		if(	(opUCmdA0[5:0] == JX2_UCMD_ALUCMP3R) &&
			(opRegAN0 == JX2_GR_ZZR))
		begin
			/* Remap CMP3R Compares targeting ZZR to CMP */
			opUCmdA[5:0] = JX2_UCMD_ALUCMP;
		end

		if(opIsScalarBase &&
			((opUIxtA0[8:6]==JX2_IUC_WX) || (opUIxtA0[8:6]==JX2_IUC_WXA)))
		begin
			if(opUCmdA0[5:0] == JX2_UCMD_CONV2_RR)
			begin
				if(opUIxtA0[5:0] == JX2_UCIX_CONV2_FP16PCK32)
				begin
					opIsDualLaneRm	= 1;
					opIsDualLaneRo	= 0;
					opIsDualLaneRn	= 0;
				end
				else
				begin
					opIsDualLaneRm	= 0;
					opIsDualLaneRo	= 0;
					opIsDualLaneRn	= 1;
				end
			end

`ifdef jx2_fpu_longdbl
`ifdef jx2_fcmp_alu
			if(opUCmdA0[5:0] == JX2_UCMD_FCMP)
				opDualLaneSw	= 1;
`endif

			if(opUCmdA0[5:0] == JX2_UCMD_FLDCX)
			begin
				opIsDualLaneRm	= 0;
				opIsDualLaneRo	= 0;
				opIsDualLaneRn	= 1;
			end
			if(opUCmdA0[5:0] == JX2_UCMD_FSTCX)
			begin
				opIsDualLaneRm	= 1;
				opIsDualLaneRo	= 0;
				opIsDualLaneRn	= 0;
			end

`endif

`ifdef jx2_alu_wx
			if(opUCmdA0[5:0] == JX2_UCMD_ALU3)
				opDualLaneSw	= 1;
			if(opUCmdA0[5:0] == JX2_UCMD_ALUCMP)
				opDualLaneSw	= 1;
			if(opUCmdA0[5:0] == JX2_UCMD_ALUCMP3R)
				opDualLaneSw	= 1;

			if(	(opUCmdA0[5:0] == JX2_UCMD_SHADQ3) ||
				(opUCmdA0[5:0] == JX2_UCMD_SHLDQ3))
			begin
				opDualLaneSw	= 1;
//				opIsDualLane3R	= 0;
//				opIsDualLane2R	= 1;

				opIsDualLaneRm	= 1;
				opIsDualLaneRo	= 0;
				opIsDualLaneRn	= 1;
			end
`endif

		end

	end

`ifdef jx2_enable_xgpr

//	opRegXM	= { 1'b0, opRegAM0[0], opRegAM0[4:1], opDualLaneSw };
//	opRegXO	= { 1'b0, opRegAO0[0], opRegAO0[4:1], opDualLaneSw };
//	opRegXN	= { 1'b0, opRegAN0[0], opRegAN0[4:1], opDualLaneSw };
//	opRegXP	= { 1'b0, opRegAP0[0], opRegAP0[4:1], opDualLaneSw };
	
	opRegXM	= { 1'b0, opRegAM0[5] || opRegAM0[0], opRegAM0[4:1], opDualLaneSw };
	opRegXO	= { 1'b0, opRegAO0[5] || opRegAO0[0], opRegAO0[4:1], opDualLaneSw };
	opRegXN	= { 1'b0, opRegAN0[5] || opRegAN0[0], opRegAN0[4:1], opDualLaneSw };
	opRegXP	= { 1'b0, opRegAP0[5] || opRegAP0[0], opRegAP0[4:1], opDualLaneSw };
	
	opRegXMv = { opRegXM[6:1], !opRegXM[0] };
	opRegXOv = { opRegXO[6:1], !opRegXO[0] };
	opRegXNv = { opRegXN[6:1], !opRegXN[0] };
	opRegXPv = { opRegXP[6:1], !opRegXP[0] };

`ifdef jx2_enable_vaddr96
// `ifdef def_true

	if(opRegAM0[6])
	begin
		if(opDualLaneSw)
		begin
			opRegXM		= opCmRemapRxB[opRegAM0[5:0]];
			opRegXMv	= opCmRemapRxA[opRegAM0[5:0]];
		end else begin
			opRegXM		= opCmRemapRxA[opRegAM0[5:0]];
			opRegXMv	= opCmRemapRxB[opRegAM0[5:0]];
		end
	end

	if(opRegAN0[6])
	begin
		if(opDualLaneSw)
		begin
			opRegXN		= opCmRemapRxB[opRegAN0[5:0]];
			opRegXNv	= opCmRemapRxA[opRegAN0[5:0]];
		end else begin
			opRegXN		= opCmRemapRxA[opRegAN0[5:0]];
			opRegXNv	= opCmRemapRxB[opRegAN0[5:0]];
		end
	end

`ifndef def_true
	if(opRegAP0[6])
	begin
		if(opDualLaneSw)
		begin
			opRegXP		= opCmRemapRxB[opRegAP0[5:0]];
			opRegXPv	= opCmRemapRxA[opRegAP0[5:0]];
		end else begin
			opRegXP		= opCmRemapRxA[opRegAP0[5:0]];
			opRegXPv	= opCmRemapRxB[opRegAP0[5:0]];
		end
	end

	if(opRegAO0[6])
	begin
		if(opDualLaneSw)
		begin
			opRegXO		= opCmRemapRxB[opRegAO0[5:0]];
			opRegXOv	= opCmRemapRxA[opRegAO0[5:0]];
		end else begin
			opRegXO		= opCmRemapRxA[opRegAO0[5:0]];
			opRegXOv	= opCmRemapRxB[opRegAO0[5:0]];
		end
	end
`endif

`else

	if(opRegAM0[6])
	begin
		if(	(opRegAM0==JX2_GR_SP) ||
			(opRegAM0==JX2_GR_SSP) || 
			(opRegAM0==JX2_GR_ZSP))
		begin
			opRegXM	= { 1'b0, 5'b10111, opDualLaneSw };
			opRegXMv = { opRegXM[6:1], !opRegXM[0] };
		end
	end

	if(opRegAN0[6])
	begin
//		if((opRegAN0==JX2_GR_ZZR) || (opRegAN0==JX2_GR_IMM))
		if(opRegAN0[6])
		begin
			opRegXN	= JX2_GR_ZZR;
			opRegXNv = JX2_GR_ZZR;
		end

		if(	(opRegAN0==JX2_GR_SP) ||
			(opRegAN0==JX2_GR_SSP) ||
			(opRegAN0==JX2_GR_ZSP))
		begin
			opRegXN	= { 1'b0, 5'b10111, opDualLaneSw };
			opRegXNv = { opRegXN[6:1], !opRegXN[0] };
		end
	end

	if(opRegAO0[6])
	begin
		if(	(opRegAO0==JX2_GR_SP) ||
			(opRegAO0==JX2_GR_SSP) ||
			(opRegAO0==JX2_GR_ZSP))
		begin
			opRegXO	= { 1'b0, 5'b10111, opDualLaneSw };
			opRegXOv = { opRegXO[6:1], !opRegXO[0] };
		end
	end

	if(opRegAP0[6])
	begin
//		if(opRegAP0==JX2_GR_ZZR)
		if(opRegAP0[6])
		begin
			opRegXP	= JX2_GR_ZZR;
			opRegXPv = JX2_GR_ZZR;
		end
	end

`endif

	
`else

	opRegXM	= { opRegAM0[0], opRegAM0[4:1], opDualLaneSw };
	opRegXO	= { opRegAO0[0], opRegAO0[4:1], opDualLaneSw };
	opRegXN	= { opRegAN0[0], opRegAN0[4:1], opDualLaneSw };
	opRegXP	= { opRegAP0[0], opRegAP0[4:1], opDualLaneSw };

	opRegXMv = { opRegXM[5:1], !opRegXM[0] };
	opRegXOv = { opRegXO[5:1], !opRegXO[0] };
	opRegXNv = { opRegXN[5:1], !opRegXN[0] };
	opRegXPv = { opRegXP[5:1], !opRegXP[0] };

`endif

`ifdef jx2_enable_vaddr96
	if(	(opUIxtA0[8:6]==JX2_IUC_WA) ||
		(opUIxtA0[8:6]==JX2_IUC_WXA))
	begin
		opRegAM = opRegXM;
		opRegBM = opRegXMv;
	end
`endif

	if(opIsDualLane)
	begin
`ifndef def_true
		if(!opIsScalar && !noLane3)
		begin
			/* WEX+DualLane: Shove Lane2 into Lane3
			 * Assume no 3-wide; this is invalid with dual-lane ops.
			 */
			opRegCN	= opRegBN;
			opRegCM	= opRegBM;
			opRegCO	= opRegBO;
			opImmC	= opImmB;
			opUCmdC	= opUCmdB;
			opUIxtC	= opUIxtB;
		end
`endif

		opRegBN	= opRegAN;
		opRegBM	= opRegAM;
		opRegBO	= opRegAO;
		if(!opIsWexJumbo96)
			opImmB	= opImmA;
		opUCmdB	= opUCmdA;
		opUIxtB	= opUIxtA;

`ifndef def_true
		/* Split Immediate, but can't figure out logic path. */
		if(!opIsScalar)
		begin
			if(decOpFzA_idUFl[1])
			begin
				opImmB	= { UV17_00, opImmA[31:16] };
			end
		end
`endif

		if(!opIsDualLaneRn)
		begin
			$display("DL, No Rn opUCmdA=%X", opUCmdA);
		end

		if(opIsDualLaneRn)
		begin
			opRegAN = opRegXN;
			opRegBN = opRegXNv;

//			if(opIsScalar)
//			begin
				opRegCO		= opRegXP;
				opRegCM		= opRegXPv;
//			end
		end

`ifdef def_true
		if(opIsDualLaneRm)
		begin
			opRegAM = opRegXM;
			opRegBM = opRegXMv;
		end

		if(opIsDualLaneRo)
		begin
			opRegAO = opRegXO;
			opRegBO = opRegXOv;
		end
`endif
	end

//	tNextMsgLatch = 0;

`ifndef def_true
// `ifdef def_true
//	if(srXG3)
	if((opUCmdA0[5:0] == JX2_UCMD_ALU3) && srRiscv && srSsc2)
	begin
		if(tMsgLatchBPc!=tNextMsgLatchBPc)
		begin
			$display("DecOpWx3 Istr=%X-%X,%X-%X,%X-%X Mod=%X PC=%X Step=%X",
				istrWordA[15: 0], istrWordA[31:16],
				istrWordB[15: 0], istrWordB[31:16],
				istrWordC[15: 0], istrWordC[31:16],
				srMod, istrBPc, idPcStep);
			$display("DecOpWx3 Tags=%X,%X,%X Mod=%X,%X,%X",
				istrMTagA, istrMTagB, istrMTagC,
				srModA, srModB, srModC);

			$display("DecOpWx3 Ucmd/UIxt, FzA=%X-%X, FzB=%X-%X, FzC=%X-%X",
				decOpFzA_idUCmd, decOpFzA_idUIxt,
				decOpFzB_idUCmd, decOpFzB_idUIxt,
				decOpFzC_idUCmd, decOpFzC_idUIxt);

			$display("DecOpWx3 Ucmd/UIxt, RvA=%X-%X, RvB=%X-%X",
				decOpRvA_idUCmd, decOpRvA_idUIxt,
				decOpRvB_idUCmd, decOpRvB_idUIxt);
			$display("DecOpWx3 %X-%X %X,%X %X,%X %X,%X ->%X,%X,%X %X-%X-%X",
				opUCmdA0, opUIxtA0,
				opRegAM, opRegAO,
				opRegBM, opRegBO,
				opRegCM, opRegCO,
				opRegAN, opRegBN, opRegCN,
				opIsDualLaneRm, opIsDualLaneRo, opIsDualLaneRn);
			$display("  %X-%X", opImmB, opImmA);
		end
	end
`endif

// `ifndef def_true
`ifdef def_true
//	if(opUCmdA0[5:0] == JX2_UCMD_CONV2_RR)
	if(opUCmdA0[5:0] == JX2_UCMD_INVOP)
	begin

		if(!tMsgLatch)
		begin
	//		$display("DecOpWx3 Istr=%X Mod=%X", istrWord, srMod);
			$display("DecOpWx3 Istr=%X-%X,%X-%X,%X-%X Mod=%X PC=%X Step=%X",
//				istrWord[15: 0], istrWord[31:16],
//				istrWord[47:32], istrWord[63:48],
//				istrWord[79:64], istrWord[95:80],
				istrWordA[15: 0], istrWordA[31:16],
				istrWordB[15: 0], istrWordB[31:16],
				istrWordC[15: 0], istrWordC[31:16],
				srMod, istrBPc, idPcStep);
			$display("DecOpWx3 Tags=%X,%X,%X Mod=%X,%X,%X",
				istrMTagA, istrMTagB, istrMTagC,
				srModA, srModB, srModC);

			$display("DecOpWx3 Ucmd/UIxt, FzA=%X-%X, FzB=%X-%X, FzC=%X-%X",
				decOpFzA_idUCmd, decOpFzA_idUIxt,
				decOpFzB_idUCmd, decOpFzB_idUIxt,
				decOpFzC_idUCmd, decOpFzC_idUIxt);

`ifdef jx2_enable_rvjumbo96
			$display("DecOpWx3 Ucmd/UIxt, RvA=%X-%X, RvB=%X-%X, RvC=%X-%X",
				decOpRvA_idUCmd, decOpRvA_idUIxt,
				decOpRvB_idUCmd, decOpRvB_idUIxt,
				decOpRvC_idUCmd, decOpRvC_idUIxt);
`else
			$display("DecOpWx3 Ucmd/UIxt, RvA=%X-%X, RvB=%X-%X",
				decOpRvA_idUCmd, decOpRvA_idUIxt,
				decOpRvB_idUCmd, decOpRvB_idUIxt);
`endif

			$display("DecOpWx3 %X-%X %X,%X %X,%X %X,%X ->%X,%X,%X %X-%X-%X",
				opUCmdA0, opUIxtA0,
				opRegAM, opRegAO,
				opRegBM, opRegBO,
				opRegCM, opRegCO,
				opRegAN, opRegBN, opRegCN,
				opIsDualLaneRm, opIsDualLaneRo, opIsDualLaneRn);
			$display("  %X-%X", opImmB, opImmA);
		end
		tNextMsgLatch = 1;
	end
`endif
end


always @(posedge clock)
begin
	tMsgLatch		<= tNextMsgLatch;
	tMsgLatchBPc	<= tNextMsgLatchBPc;
end

endmodule
