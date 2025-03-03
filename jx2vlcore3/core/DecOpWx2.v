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
BJX2 Instruction Decoder (WEX, 2-Wide)

Takes a 16/32/48 bit instruction word.

Ex block is predicated.
Fx block is unconditional.

This decoder will support WEX.
  Rs/Rt/Rm: Lane 1
  Ru/Rv/Rn: Lane 2

 */

`include "CoreDefs.v"

`include "DecOpBz.v"
`include "DecOpFz.v"

`ifdef jx2_enable_riscv
`include "DecOpRvI.v"
`endif

module DecOpWx2(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	istrMTag,
	istrBPc,	regSr,
	istrSxo,	idPcStep,

	idRegS,		idRegT,		idRegM,
	idImmA,		idUCmdA,	idUIxtA,
	idRegU,		idRegV,		idRegN,
	idImmB,		idUCmdB,	idUIxtB
	);

input			clock;		//clock
input			reset;		//clock

input[95:0]		istrWord;	//source instruction word
input[47:0]		istrBPc;	//Instruction PC Address
// input			srWxe;
input[63:0]		regSr;
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

assign		istrWordA = istrWord[31: 0];
assign		istrWordB = istrWord[63:32];
assign		istrWordC = istrWord[95:64];

assign		istrMTagA = istrMTag[1:0];
assign		istrMTagB = istrMTag[3:2];
assign		istrMTagC = istrMTag[5:4];

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
wire[7:0]		srMod;
wire[7:0]		srModA;
wire[7:0]		srModB;
wire[7:0]		srModC;

// assign		srMod = { regSr[29], srSxo, srUser };
assign		srMod = {
	1'b0, 1'b0, 1'b0, srRiscv,
	srXG2, regSr[29], srSxo, srUser };

assign		srModA = { istrMTagA[1:0], srMod[5:0] };
assign		srModB = { istrMTagB[1:0], srMod[5:0] };
assign		srModC = { istrMTagC[1:0], srMod[5:0] };

`output_gpr		idRegS;
`output_gpr		idRegT;
`output_gpr		idRegM;

`output_gpr		idRegU;
`output_gpr		idRegV;
`output_gpr		idRegN;

output[32:0]	idImmA;
output[8:0]		idUCmdA;
output[8:0]		idUIxtA;

output[32:0]	idImmB;
output[8:0]		idUCmdB;
output[8:0]		idUIxtB;

parameter		noLane3 = 0;
parameter		noNoRiscV = 0;
parameter		fpuLowPrec = 0;
defparam		decOpFzA.fpuLowPrec		= fpuLowPrec;
defparam		decOpFzB.fpuLowPrec		= fpuLowPrec;
defparam		decOpFzC.fpuLowPrec		= fpuLowPrec;
defparam		decOpBz.fpuLowPrec		= fpuLowPrec;


`reg_gpr		opRegAM;
`reg_gpr		opRegAO;
`reg_gpr		opRegAN;
`reg_gpr		opRegAP;

`reg_gpr		opRegBM;
`reg_gpr		opRegBO;
`reg_gpr		opRegBN;
`reg_gpr		opRegBP;

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

assign	idImmA = opImmA;
assign	idUCmdA = opUCmdA;
assign	idUIxtA = opUIxtA;

assign	idImmB = opImmB;
assign	idUCmdB = opUCmdB;
assign	idUIxtB = opUIxtB;

wire[24:0]		tOpJBitsA;
wire[24:0]		tOpJBitsB;
wire[24:0]		tOpJBitsC;

wire[2:0]		opJumboWxBitsB;
wire[2:0]		opJumboWxBitsC;

wire		opIsWexJumboA;
wire		opIsWexJumboB;
wire		opIsWexJumbo96;
reg			opIsWexJumboLdi;
`reg_gpr	opWexJumboRn;

wire		opIsWexJumboXA;
wire		opIsWexJumboXB;

wire	opIsWexB;
wire	opIsWexaB;
wire	opIsWex2x40B;

wire[24:0]	tOpRvJBitsB;
wire		opIsRvJumboA;

assign	opIsRvJumboA	= 0;
assign	tOpRvJBitsB		= 0;

`wire_gpr		decOpBz_idRegN;
`wire_gpr		decOpBz_idRegM;
`wire_gpr		decOpBz_idRegO;
wire[32:0]		decOpBz_idImm;
wire[8:0]		decOpBz_idUCmd;
wire[8:0]		decOpBz_idUIxt;

DecOpBz	decOpBz(
	clock,		reset,
	istrWord[63:0],		srMod,
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

wire[31:0]		decOpFzC_maskInstr;
//assign		decOpFzC_maskInstr =
//	(	(istrWord[79:77] == 3'b111) &&
//		(istrWord[75] == 1'b1) &&
//		(istrWord[73:72] == 2'b00) ) ?
//	istrWord[95:64] :
//	UV32_00;

assign		decOpFzC_maskInstr = istrWord[95:64] ;

DecOpFz	decOpFzC(
	clock,		reset,	srModC,
	{ UV32_00, decOpFzC_maskInstr },	4'h7,
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
	{ UV32_00, istrWord[63:32] },	4'h1,
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
	{ UV32_00, istrWord[31: 0] },	4'h0,
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

DecOpRvI	decOpRvA(
	clock,		reset,	srModA,
	{ UV32_00, istrWord[31: 0] },
	{srRiscv && !srXG2RV, srWxe, 2'b00},		UV28_00,
	decOpRvA_idRegN,		decOpRvA_idRegM,
	decOpRvA_idRegO,		decOpRvA_idRegP,
	decOpRvA_idImm,
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
	{srRiscv && !srXG2RV, srWxe, 2'b00},
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

`endif

reg opIsFxA;
reg opIsFzA;
reg opIsFCA;
reg opIsDzA;	//Predicated Ops
reg opIsDwA;		//PrWEX Ops
reg opIsDfA;	//Pred-False or WEX
reg opIsWfA;	//WEX

reg opIsScalar;		//Scalar Operation
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


`ifdef jx2_enable_wexjumbo

`ifdef jx2_enable_riscv
wire	srNoJumbo =
	(srRiscv && !srXG2RV);
`else
wire	srNoJumbo = 0;
`endif

// assign	opIsWexJumboA =
//		(istrWord[15: 9] == 7'b1111_111) ;
// assign	opIsWexJumboB =
//		(istrWord[47:41] == 7'b1111_111) ;

assign	opIsWexJumboA =
		 (istrWordA[12: 9] == 4'b1111) &&
		((istrWordA[15:13] == 3'b111 ) || srXG2X) &&
			(!srNoJumbo || istrMTagA[0]) ;

assign	opIsWexJumboB =
		 (istrWordB[12: 9] == 4'b1111) &&
		((istrWordB[15:13] == 3'b111 ) || srXG2X) &&
			(!srNoJumbo || istrMTagB[0]) ;

assign	opIsWexJumboXA =
		((istrWord[15:12] == 4'b0111) && (istrWord[10:8] == 3'b000)) ||
		((istrWord[15:12] == 4'b1001) && (istrWord[10:9] == 2'b00 )) ;
assign	opIsWexJumboXB =
		((istrWord[47:44] == 4'b0111) && (istrWord[42:40] == 3'b000)) ||
		((istrWord[47:44] == 4'b1001) && (istrWord[42:41] == 2'b00 )) ;

assign	opIsWexJumbo96 =
	opIsWexJumboA && opIsWexJumboB;

assign	opIsWexB =
	((istrWord[47:44] == 4'b1111) && istrWord[42]) ||
	((istrWord[47:44] == 4'b1110) && (istrWord[43:41]==3'b101));

assign	opIsWexaB =
	((istrWord[47:44] == 4'b1111) && (istrWord[43:42]==2'b01 )) ||
	((istrWord[47:44] == 4'b1110) && (istrWord[43:41]==3'b101));

assign	opIsWex2x40B = 0;

assign	tOpJBitsA		= 0;

wire		opIsWex2x40B_RiAltC;
assign		opIsWex2x40B_RiAltC =
	(istrWord[53:52] == 2'b00) &&
	(istrWord[61:60] == 2'b00) &&
	istrWord[50] && istrWord[27] ;

assign	tOpJBitsB		= { istrWord [8: 0], istrWord[31:16] };
assign	tOpJBitsC		= { istrWord[40:32], istrWord[63:48] };

assign	opJumboWxBitsB = ~istrWord[15:13];
assign	opJumboWxBitsC = ~istrWord[47:45];

`else

assign	opIsWexJumboA	= 0;
assign	opIsWexJumboB	= 0;
assign	opIsWexJumbo96	= 0;
assign	tOpJBitsA		= 0;
assign	tOpJBitsB		= 0;
assign	tOpJBitsC		= 0;

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
	opUFlA		= 0;

	opUCmdB		= UV9_00;
	opUIxtB		= UV9_00;
	opImmB		= UV33_00;
	opRegBM		= JX2_GR_ZZR;
	opRegBO		= JX2_GR_ZZR;
	opRegBN		= JX2_GR_ZZR;
	opRegBP		= JX2_GR_ZZR;

	opRegXM		= JX2_GR_ZZR;
	opRegXO		= JX2_GR_ZZR;
	opRegXN		= JX2_GR_ZZR;
	opRegXP		= JX2_GR_ZZR;
	
	opRegXMv = JX2_GR_ZZR;
	opRegXOv = JX2_GR_ZZR;
	opRegXNv = JX2_GR_ZZR;
	opRegXPv = JX2_GR_ZZR;

	opUCmdA0	= UV9_00;
	opUIxtA0	= UV9_00;
	opRegAM0	= JX2_GR_ZZR;
	opRegAO0	= JX2_GR_ZZR;
	opRegAN0	= JX2_GR_ZZR;
	opRegAP0	= JX2_GR_ZZR;
	opImmA0		= 0;
	opUFlA0		= 0;

	opIsDwA = 0;
	opIsDwB = 0;
	opIsDwC = 0;
	opIsWexJumboLdi	= 0;
	opIsScalar		= 0;
	opIsDualLane	= 0;
	opIsDualLaneRm	= 0;
	opIsDualLaneRn	= 0;
	opIsDualLaneRo	= 0;
	opDualLaneSw	= 0;
	opIsBaseRm		= 0;

	casez(istrWord[15:10])
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

	casez(istrWord[47:42])
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

	casez(istrWord[79:74])
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

`ifdef jx2_enable_riscv
//	if(srRiscv)
	if(srRiscv && !srXG2RV)
	begin
		if(istrWord[1:0]==2'b11)
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
	end
`endif

`ifdef jx2_enable_xg2mode
	if(srXG2)
	begin
		opIsFxA = 1;		opIsFzA = 1;
		opIsFxB = 1;		opIsFzB = 1;
		opIsFxC = 1;		opIsFzC = 1;
		opIsFCA = 0;		opIsFCB = 0;
		opIsFCC = 0;
		opIsDfA = istrWord[10];
		opIsDfB = istrWord[42];
		opIsDfC = istrWord[74];
		opIsDzA = 0;
		opIsDzB = 0;
		opIsDzC = 0;

		if(!istrWord[12])
		begin
			opIsDzA = 1;
			opIsDwA = istrWord[11] && istrWord[9];
		end

		if(!istrWord[44])
		begin
			opIsDzB = 1;
			opIsDwB = istrWord[43] && istrWord[41];
		end

		if(!istrWord[76])
		begin
			opIsDzC = 1;
			opIsDwC = istrWord[75] && istrWord[73];
		end
	end
`endif


//	opIsWfA = opIsDfA && !opIsDzA && srWxe;
//	opIsWfB = opIsDfB && !opIsDzB && srWxe;

//	opIsWfA = opIsDfA && (!opIsDzA || opIsDwA) && srWxe;
//	opIsWfB = opIsDfB && (!opIsDzB || opIsDwB) && srWxe;
	opIsWfA = ((opIsDfA && !opIsDzA) || opIsDwA) && srWxe;
	opIsWfB = ((opIsDfB && !opIsDzB) || opIsDwB) && srWxe;

`ifdef jx2_enable_wexjumbo
	if(opIsWexJumbo96)
	begin
		opImmA	= decOpFzC_idImm;

		opRegAM	= decOpFzC_idRegM;
		opRegAO	= decOpFzC_idRegO;
		opRegAN	= decOpFzC_idRegN;
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
//		opImmB	= { 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };
		
		opImmB	= decOpFzC_idUFl[0] ?
//			{ 9'b0, tOpJBitsB[23:0] } :
			{ opImmA[32] ? 9'h1FF : 9'h000, tOpJBitsB[23:0] } :
			{ 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };

		opUCmdB	= UV9_00;
		opUIxtB	= decOpFzC_idUFl[12:4];

		opIsScalar	= 1;
	end
	else
`endif
	if(opIsFxA)
	begin
		if(1'b1)
		begin
//			if(opIsWfA && opIsWfB)
			if(srSsc3)
			begin
				$display("DecOpWx2: WEX-3 Invalid if 2-wide");
				opUCmdA	= { 3'b000, JX2_UCMD_INVOP };
				opUIxtA	= UV9_00;

`ifndef def_true
			
				opRegAM	= decOpFzC_idRegM;
				opRegAO	= decOpFzC_idRegO;
				opRegAN	= decOpFzC_idRegN;
//				opRegAN	= decOpFzC_idRegN;
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

				opRegBM	= decOpFzB_idRegM;
				opRegBO	= decOpFzB_idRegO;
				opRegBN	= decOpFzB_idRegN;
				opImmB	= decOpFzB_idImm;
				opUCmdB	= decOpFzB_idUCmd;
				opUIxtB	= decOpFzB_idUIxt;
`endif
			end
			else
//			if(opIsWfA || opIsWexJumboA)
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

`ifdef jx2_dec_ssc_riscv
				if(srRiscv && !noNoRiscV && !srXG2RV && !srMTagB[0])
				begin
					opRegAM		= decOpRvB_idRegM;
					opRegAO		= decOpRvB_idRegO;
					opRegAN		= decOpRvB_idRegN;
					opRegAP		= decOpRvB_idRegP;
					opImmA		= decOpRvB_idImm;
					opUCmdA		= decOpRvB_idUCmd;
					opUIxtA		= decOpRvB_idUIxt;
					opUFlA		= decOpRvB_idUFl;
					
					opIsScalar	= opIsRvJumboA
				end

				if(srRiscv && !noNoRiscV && !srXG2RV && !srMTagA[0])
				begin
					opRegBM		= decOpRvA_idRegM;
					opRegBO		= decOpRvA_idRegO;
					opRegBN		= decOpRvA_idRegN;
					opRegBP		= decOpRvA_idRegP;
					opImmB		= decOpRvA_idImm;
					opUCmdB		= decOpRvA_idUCmd;
					opUIxtB		= decOpRvA_idUIxt;
					opUFlB		= decOpRvA_idUFl;
				end
`endif

				opRegAM0	= opRegAM;
				opRegAO0	= opRegAO;
				opRegAN0	= opRegAN;
				opRegAP0	= opRegAP;
				opUCmdA0	= opUCmdA;
				opUIxtA0	= opUIxtA;
				opUFlA0		= opUFlA;

				if(opIsWexJumboA)
				begin
					if(decOpFzB_idUFl[0])
					begin
						/* Jumbo24 + Imm16 */
						opImmB	= {
							opImmA[32] ? UV25_FF : UV25_00,
							tOpJBitsB[23:16] };
					end else begin
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
				if(srRiscv && !noNoRiscV && !srXG2RV && !srMTagA[0])
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
				opImmA	= opImmA0;
				opUCmdA	= opUCmdA0;
				opUIxtA	= opUIxtA0;
				opUFlA	= opUFlA0;
`endif

//				opRegBM	= JX2_GR_ZZR;
				opRegBM	= opRegAP0;
				opRegBO	= opRegAP0;
				opRegBN	= JX2_GR_ZZR;
				opImmB	= UV33_00;
				opUCmdB	= UV9_00;
				opUIxtB	= decOpFzA_idUFl[12:4];

				opIsScalar	= 1;
				
			end
		end
	end
	else
	begin
		opRegAM	= decOpBz_idRegM;
		opRegAO	= decOpBz_idRegO;
		opRegAN	= decOpBz_idRegN;
		opImmA	= decOpBz_idImm;
		opUCmdA	= decOpBz_idUCmd;
		opUIxtA	= decOpBz_idUIxt;

		opRegAM0	= opRegAM;
		opRegAO0	= opRegAO;
		opRegAN0	= opRegAN;
		opRegAP0	= opRegAN;
		opUCmdA0	= opUCmdA;
		opUIxtA0	= opUIxtA;
		opUFlA0		= opUFlA;

		opRegBN	= JX2_GR_ZZR;
//		opRegBM	= JX2_GR_ZZR;
		opRegBM	= opRegAN;
		opRegBO	= opRegAN;
		opImmB	= UV33_00;
		opUCmdB	= UV9_00;
		opUIxtB	= UV9_00;
		
		opIsScalar	= 1;
	end

	if(1'b1)
	begin
		if(opUCmdA0[5:1] == 5'h02)
		begin
			opIsBaseRm	= 1;
		end

`ifdef def_true
		if((opUIxtA0[8:6]==JX2_IUC_WX) || (opUIxtA0[8:6]==JX2_IUC_WXA))
		begin
			opIsDualLane = 1;
			opIsDualLaneRn	= 1;

			if(opUCmdA0[5:1] != 5'h02)
			begin
				opIsDualLaneRm	= 1;
				opIsDualLaneRo	= 1;
				opIsDualLaneRn	= 1;
			end

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

			if(	(opUCmdA0[5:0] == JX2_UCMD_SHADQ3) ||
				(opUCmdA0[5:0] == JX2_UCMD_SHLDQ3))
			begin
				opDualLaneSw	= 1;

				opIsDualLaneRm	= 1;
				opIsDualLaneRo	= 0;
				opIsDualLaneRn	= 1;
			end
`endif

		end
`endif

	end

`ifdef jx2_enable_xgpr

	opRegXM	= { 1'b0, opRegAM0[0], opRegAM0[4:1], opDualLaneSw };
	opRegXO	= { 1'b0, opRegAO0[0], opRegAO0[4:1], opDualLaneSw };
	opRegXN	= { 1'b0, opRegAN0[0], opRegAN0[4:1], opDualLaneSw };
	opRegXP	= { 1'b0, opRegAP0[0], opRegAP0[4:1], opDualLaneSw };
	
	opRegXMv = { opRegXM[6:1], !opRegXM[0] };
	opRegXOv = { opRegXO[6:1], !opRegXO[0] };
	opRegXNv = { opRegXN[6:1], !opRegXN[0] };
	opRegXPv = { opRegXP[6:1], !opRegXP[0] };
	
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
// `ifndef def_true
	if(opIsBaseRm)
	begin
		if(	(opUIxtA0[8:6]==JX2_IUC_WA) ||
			(opUIxtA0[8:6]==JX2_IUC_WXA))
		begin
			opRegAM = opRegXM;
			opRegBM = opRegXMv;
		end
	end
`endif

	if(opIsDualLane)
	begin		
		opRegBN	= opRegAN;
		opRegBM	= opRegAM;
		opRegBO	= opRegAO;
		if(!opIsWexJumbo96)
			opImmB	= opImmA;
		opUCmdB	= opUCmdA;
		opUIxtB	= opUIxtA;
		
		if(!opIsScalar)
		begin
			if(decOpFzA_idUFl[1])
			begin
				opImmB	= { UV17_00, opImmA[31:16] };
			end
		end

		if(!opIsDualLaneRn)
		begin
			$display("DL, No Rn opUCmdA=%X", opUCmdA);
		end

		if(opIsDualLaneRn)
		begin
			opRegAN = opRegXN;
			opRegBN = opRegXNv;
			opRegBO	= opRegXP;
			opRegBM	= opRegXPv;
//			opRegBO	= opRegXN;
//			opRegBM	= opRegXNv;
		end

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
	end
end

endmodule
