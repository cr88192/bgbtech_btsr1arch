/*
BJX2 Instruction Decoder (WEX, 3-Wide)

Takes a 16/32/48 bit instruction word.

Ex block is predicated.
Fx block is unconditional.

This decoder will support WEX.
  Rs/Rt/Rm: Lane 1
  Ru/Rv/Rn: Lane 2
  Rx/Ry/Ro: Lane 3

For scalar Ops, Lane 2/3 will hold:
  ZZR, ZZR, ZZR
  ZZR, Rm, ZZR

 */

`include "CoreDefs.v"

`include "DecOpBz.v"
`include "DecOpFz.v"
// `include "DecOpFC.v"

`ifdef jx2_enable_ops24
`include "DecOpHz.v"
`endif

`ifdef jx2_enable_riscv
`include "DecOpRvI.v"
`endif

module DecOpWx3(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	regSr,		istrSxo,
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
// input			srWxe;
input[63:0]		regSr;
input[3:0]		istrSxo;	//source instruction word

wire			srWxe;
wire			srRiscv;
wire			srUser;
wire			srSxo;
assign		srWxe	= regSr[27];
assign		srUser	= !regSr[30];
assign		srSxo	= istrSxo[0];
assign		srRiscv	= (regSr[27:26] == 2'b01);

wire[2:0]		srMod;
assign		srMod = { regSr[29], srSxo, srUser };

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


`reg_gpr		opRegAM;
`reg_gpr		opRegAO;
`reg_gpr		opRegAN;

`reg_gpr		opRegBM;
`reg_gpr		opRegBO;
`reg_gpr		opRegBN;

`reg_gpr		opRegCM;
`reg_gpr		opRegCO;
`reg_gpr		opRegCN;

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

reg[32:0]		opImmB;
reg[8:0]		opUCmdB;
reg[8:0]		opUIxtB;

reg[32:0]		opImmC;
reg[8:0]		opUCmdC;
reg[8:0]		opUIxtC;

reg[32:0]		opImmA0;
reg[8:0]		opUCmdA0;
reg[8:0]		opUIxtA0;

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

// wire[21:0]		tOpJBitsA;
// wire[21:0]		tOpJBitsB;
// wire[21:0]		tOpJBitsC;
wire[24:0]		tOpJBitsA;
wire[24:0]		tOpJBitsB;
wire[24:0]		tOpJBitsC;

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
wire[3:0]		decOpFzC_idUFl;

DecOpFz	decOpFzC(
	clock,		reset,	srMod,
	{ UV32_XX, istrWord[95:64] },	4'h1,
		{ tOpJBitsB[24], tOpJBitsC[24],
			opIsWexJumbo96,
			opIsWexJumboB | opIsWex2x40B,
			tOpJBitsC[23:0] },
	decOpFzC_idRegN,		decOpFzC_idRegM,
	decOpFzC_idRegO,		decOpFzC_idRegP,
	decOpFzC_idImm,
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
wire[3:0]		decOpFzB_idUFl;

DecOpFz	decOpFzB(
	clock,		reset,	srMod,
	{ UV32_XX, istrWord[63:32] },	4'h1,
		{ 1'b0, tOpJBitsB[24],
			1'b0,
			opIsWexJumboA | opIsWex2x40B,
			tOpJBitsB[23:0] },
	decOpFzB_idRegN,		decOpFzB_idRegM,
	decOpFzB_idRegO,		decOpFzB_idRegP,
	decOpFzB_idImm,
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
wire[3:0]		decOpFzA_idUFl;

DecOpFz	decOpFzA(
	clock,		reset,	srMod,
	{ UV32_XX, istrWord[31: 0] },	4'h0,	UV28_00,
	decOpFzA_idRegN,		decOpFzA_idRegM,
	decOpFzA_idRegO,		decOpFzA_idRegP,
	decOpFzA_idImm,
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
wire[3:0]		decOpRvA_idUFl;

DecOpRvI	decOpRvA(
	clock,		reset,	srMod,
	{ UV32_XX, istrWord[31: 0] },
	{srRiscv, srWxe, 2'b00},		UV28_00,
	decOpRvA_idRegN,		decOpRvA_idRegM,
	decOpRvA_idRegO,		decOpRvA_idRegP,
	decOpRvA_idImm,
	decOpRvA_idUCmd,		decOpRvA_idUIxt,
	decOpRvA_idUFl
	);

`endif

`ifdef jx2_enable_ops48
`wire_gpr		decOpFC_idRegN;
`wire_gpr		decOpFC_idRegM;
`wire_gpr		decOpFC_idRegO;
wire[32:0]		decOpFC_idImm;
wire[8:0]		decOpFC_idUCmd;
wire[8:0]		decOpFC_idUIxt;

DecOpFC	decOpFC(
	clock,		reset,
	istrWord[63:0],
	decOpFC_idRegN,		decOpFC_idRegM,
	decOpFC_idRegO,		decOpFC_idImm,
	decOpFC_idUCmd,		decOpFC_idUIxt
	);
`endif

`ifdef jx2_enable_ops24
`wire_gpr		decOpHz_idRegN;
`wire_gpr		decOpHz_idRegM;
`wire_gpr		decOpHz_idRegO;
wire[32:0]		decOpHz_idImm;
wire[8:0]		decOpHz_idUCmd;
wire[8:0]		decOpHz_idUIxt;
wire[3:0]		decOpHz_idUFl;

DecOpHz	decOpHz(
	clock,		reset,
	{ UV32_XX, istrWord[31: 0] },	4'h0,	UV26_00,
	decOpHz_idRegN,		decOpHz_idRegM,
	decOpHz_idRegO,		decOpHz_idImm,
	decOpHz_idUCmd,		decOpHz_idUIxt,
	decOpHz_idUFl
	);
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
// reg opIsDualLane3R;	//Op uses both lanes (with all 3 regs as 128b)
// reg opIsDualLane2R;	//Op uses both lanes (with Rm and Rn regs as 128b)
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
		(istrWord[15: 9] == 7'b1111_111) ;
assign	opIsWexJumboB =
		(istrWord[47:41] == 7'b1111_111) ;

assign	opIsWexJumboXA =
		((istrWord[15:12] == 4'b0111) && (istrWord[10:8] == 3'b000)) ||
		((istrWord[15:12] == 4'b1001) && (istrWord[10:9] == 2'b00 )) ;
assign	opIsWexJumboXB =
		((istrWord[47:44] == 4'b0111) && (istrWord[42:40] == 3'b000)) ||
		((istrWord[47:44] == 4'b1001) && (istrWord[42:41] == 2'b00 )) ;

assign	opIsWexJumbo96 =
//	opIsWexJumboA && istrWord[42];
	opIsWexJumboA && opIsWexJumboB;

assign	opIsWexB =
	((istrWord[47:44] == 4'b1111) && istrWord[42]) ||
	((istrWord[47:44] == 4'b1110) && (istrWord[43:41]==3'b101));

assign	opIsWexaB =
	((istrWord[47:44] == 4'b1111) && (istrWord[43:42]==2'b01 )) ||
	((istrWord[47:44] == 4'b1110) && (istrWord[43:41]==3'b101));

`ifdef jx2_enable_2x40b
assign	opIsWex2x40B =
//	opIsWexJumboA && istrWord[8] && opIsWexaB;
	opIsWexJumboXA && opIsWexaB;
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
	(istrWord[53:52] == 2'b00) &&
	(istrWord[61:60] == 2'b00) &&
	istrWord[50] && istrWord[27] ;

`ifdef jx2_enable_2x40b
assign	tOpJBitsB		=
	opIsWex2x40B ? {
		1'b1, istrWord [7:4], 8'h00,
//		istrWord[31] ?
//			{ 1'b1, istrWord[30:29],
//				istrWord[4] ? 4'hF : 4'h0, istrWord[28] } :
//			{ 1'b0, istrWord[4] ? 4'hF : 4'h0, istrWord[30:28] },

//		{ istrWord[31], istrWord[4] ? 4'hF : 4'h0, istrWord[30:28] },
		{ istrWord[31], istrWord[30] ? 4'hF : 4'h0, istrWord[30:28] },
		istrWord[23:20] } :
	{ istrWord [8: 0], istrWord[31:16] };
assign	tOpJBitsC		=
	opIsWex2x40B ? {
		1'b1, istrWord [3:0], 8'h00,
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
`else
assign	tOpJBitsB		= { istrWord [8: 0], istrWord[31:16] };
assign	tOpJBitsC		= { istrWord[40:32], istrWord[63:48] };
`endif

`else

assign	opIsWexJumboA	= 0;
assign	opIsWexJumboB	= 0;
assign	opIsWexJumbo96	= 0;
assign	tOpJBitsA		= 0;
assign	tOpJBitsB		= 0;
assign	tOpJBitsC		= 0;

`endif

`reg_gpr	opCmRemapRx[31:0];

initial
begin
	opCmRemapRx[ 0]=JX2_GR_PC_HI;
//	opCmRemapRx[ 1]=JX2_GR_LR_HI;
	opCmRemapRx[ 1]=JX2_GR_ZZR;
	opCmRemapRx[ 2]=JX2_GR_ZZR;
	opCmRemapRx[ 3]=JX2_GR_VBR_HI;
	opCmRemapRx[ 4]=JX2_GR_SPC_HI;
//	opCmRemapRx[ 5]=JX2_GR_SSP_HI;
	opCmRemapRx[ 5]=JX2_GR_ZZR;
	opCmRemapRx[ 6]=JX2_GR_GBR_HI;
	opCmRemapRx[ 7]=JX2_GR_ZZR;
	opCmRemapRx[ 8]=JX2_GR_ZZR;
	opCmRemapRx[ 9]=JX2_GR_ZZR;
	opCmRemapRx[10]=JX2_GR_ZZR;
	opCmRemapRx[11]=JX2_GR_ZZR;
	opCmRemapRx[12]=JX2_GR_ZZR;
	opCmRemapRx[13]=JX2_GR_ZZR;
	opCmRemapRx[14]=JX2_GR_ZZR;
	opCmRemapRx[15]=JX2_GR_ZZR;
//	opCmRemapRx[15]=JX2_GR_SP_HI;

	opCmRemapRx[16]=JX2_GR_PC2_HI;
//	opCmRemapRx[17]=JX2_GR_LR2_HI;
	opCmRemapRx[17]=JX2_GR_ZZR;
	opCmRemapRx[18]=JX2_GR_ZZR;
//	opCmRemapRx[19]=JX2_GR_VBR2_HI;
	opCmRemapRx[19]=JX2_GR_ZZR;
	opCmRemapRx[20]=JX2_GR_SPC2_HI;
//	opCmRemapRx[21]=JX2_GR_SSP2_HI;
	opCmRemapRx[21]=JX2_GR_ZZR;
	opCmRemapRx[22]=JX2_GR_GBR2_HI;
	opCmRemapRx[23]=JX2_GR_ZZR;
	opCmRemapRx[24]=JX2_GR_ZZR;
	opCmRemapRx[25]=JX2_GR_ZZR;
	opCmRemapRx[26]=JX2_GR_ZZR;
	opCmRemapRx[27]=JX2_GR_ZZR;
	opCmRemapRx[28]=JX2_GR_ZZR;
	opCmRemapRx[29]=JX2_GR_ZZR;
	opCmRemapRx[30]=JX2_GR_ZZR;
	opCmRemapRx[31]=JX2_GR_ZZR;
//	opCmRemapRx[31]=JX2_GR_SP2_HI;
end

always @*
begin

	opRegAM	= JX2_GR_ZZR;
	opRegAO	= JX2_GR_ZZR;
	opRegAN	= JX2_GR_ZZR;

	opRegBM	= JX2_GR_ZZR;
	opRegBO	= JX2_GR_ZZR;
	opRegBN	= JX2_GR_ZZR;

	opRegCM	= JX2_GR_ZZR;
	opRegCO	= JX2_GR_ZZR;
	opRegCN	= JX2_GR_ZZR;

	opRegXM	= JX2_GR_ZZR;
	opRegXO	= JX2_GR_ZZR;
	opRegXN	= JX2_GR_ZZR;

	opIsDwA = 0;
	opIsDwB = 0;
	opIsDwC = 0;
	opIsWexJumboLdi	= 0;
	opIsScalar		= 0;
	opIsDualLane	= 0;
//	opIsDualLane3R	= 0;
//	opIsDualLane2R	= 0;
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
	if(srRiscv)
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

//	opIsWfA = opIsDfA && !opIsDzA && srWxe;
//	opIsWfB = opIsDfB && !opIsDzB && srWxe;

//	opIsWfA = opIsDfA && (!opIsDzA || opIsDwA) && srWxe;
//	opIsWfB = opIsDfB && (!opIsDzB || opIsDwB) && srWxe;
	opIsWfA = ((opIsDfA && !opIsDzA) || opIsDwA) && srWxe;
	opIsWfB = ((opIsDfB && !opIsDzB) || opIsDwB) && srWxe;

`ifdef jx2_enable_wexjumbo
	opIsWexJumboLdi = opIsWexJumbo96 &&
//		(istrWord[47:40] == 8'b1111_0100) &&
		(istrWord[79:73] == 7'b1111_101 );

	opWexJumboRn[3:2] = istrWord[29:28];
	opWexJumboRn[1:0] = istrWord[61:60];
	opWexJumboRn[  4] = istrWord[72];
	opWexJumboRn[  5] = (opWexJumboRn[4:1] == 0);
`endif

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

		opRegBM	= JX2_GR_ZZR;
		opRegBO	= JX2_GR_ZZR;
		opRegBN	= JX2_GR_ZZR;
//		opImmB	= { 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };
		
		opImmB	= decOpFzC_idUFl[0] ?
//			{ 9'b0, tOpJBitsB[23:0] } :
			{ opImmA[32] ? 9'h1FF : 9'h000, tOpJBitsB[23:0] } :
			{ 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };

		opUCmdB	= UV9_00;
		opUIxtB	= UV9_00;

		opRegCM	= JX2_GR_ZZR;
		opRegCO	= JX2_GR_ZZR;
		opRegCN	= JX2_GR_ZZR;
		opImmC	= UV33_XX;
		opUCmdC	= UV9_00;
		opUIxtC	= UV9_00;

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
//		opRegXM = { istrWord[ 0], istrWord[25], istrWord[ 3: 1], 1'b0 };
//		opRegXO = { istrWord[20], istrWord[24], istrWord[23:21], 1'b0 };
//		opRegXN = { istrWord[ 4], istrWord[26], istrWord[ 7: 5], 1'b0 };

`ifdef jx2_enable_ops48
		if(opIsFCA)
`else
		if(1'b0)
`endif
		begin
`ifdef jx2_enable_ops48
			opRegAN	= decOpFC_idRegN;
			opRegAM	= decOpFC_idRegM;
			opRegAO	= decOpFC_idRegO;
			opImmA	= decOpFC_idImm;
			opUCmdA	= decOpFC_idUCmd;
			opUIxtA	= decOpFC_idUIxt;

			opRegAN0	= decOpFC_idRegN;
			opRegAP0	= decOpFC_idRegN;
			opRegAM0	= decOpFC_idRegM;
			opRegAO0	= decOpFC_idRegO;
			opUCmdA0	= decOpFC_idUCmd;
			opUIxtA0	= decOpFC_idUIxt;

			opRegBN	= JX2_GR_ZZR;
			opRegBM	= JX2_GR_ZZR;
			opRegBO	= decOpFC_idRegN;
			opImmB	= UV33_XX;
			opUCmdB	= UV9_00;
			opUIxtB	= UV9_00;
				
			opRegCM	= JX2_GR_ZZR;
			opRegCO	= decOpFC_idRegN;
			opRegCN	= JX2_GR_ZZR;
			opImmC	= UV33_XX;
			opUCmdC	= UV9_00;
			opUIxtC	= UV9_00;

//			if(opIsDzA)
//			begin
//				opUCmdA[8:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
//			end
`endif
		end
		else
		begin
			if(opIsWfA && opIsWfB)
			begin
//				$display("DecOpWz3: WEX3");
			
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

				opRegBM	= decOpFzB_idRegM;
				opRegBO	= decOpFzB_idRegO;
				opRegBN	= decOpFzB_idRegN;
				opImmB	= decOpFzB_idImm;
				opUCmdB	= decOpFzB_idUCmd;
				opUIxtB	= decOpFzB_idUIxt;

				opRegCM	= decOpFzA_idRegM;
				opRegCO	= decOpFzA_idRegO;
				opRegCN	= decOpFzA_idRegN;
				opImmC	= decOpFzA_idImm;
				opUCmdC	= decOpFzA_idUCmd;
				opUIxtC	= decOpFzA_idUIxt;

`ifndef def_true
				if(opIsDzA)
				begin
					opUCmdC[8:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
				end
				if(opIsDzB)
				begin
					opUCmdB[8:6]=opIsDfB?JX2_IXC_CF:JX2_IXC_CT;
				end
				if(opIsDzC)
				begin
					opUCmdA[8:6]=opIsDfC?JX2_IXC_CF:JX2_IXC_CT;
				end
`endif
			end
			else
			if(opIsWfA || opIsWexJumboA)
			begin
			
				if(!opIsWexJumboA)
				begin
//					$display("DecOpWz3: WEX2 %X-%X %X-%X",
//						istrWord[15:0], istrWord[31:16],
//						istrWord[47:32], istrWord[63:48]);
				end

				opRegAM	= decOpFzB_idRegM;
				opRegAO	= decOpFzB_idRegO;
				opRegAN	= decOpFzB_idRegN;
				opImmA	= decOpFzB_idImm;
				opUCmdA	= decOpFzB_idUCmd;
				opUIxtA	= decOpFzB_idUIxt;

				opRegAM0	= decOpFzB_idRegM;
				opRegAO0	= decOpFzB_idRegO;
				opRegAN0	= decOpFzB_idRegN;
				opRegAP0	= decOpFzB_idRegP;
				opUCmdA0	= decOpFzB_idUCmd;
				opUIxtA0	= decOpFzB_idUIxt;

				opRegBM	= decOpFzA_idRegM;
				opRegBO	= decOpFzA_idRegO;
				opRegBN	= decOpFzA_idRegN;
				opImmB	= decOpFzA_idImm;
				opUCmdB	= decOpFzA_idUCmd;
				opUIxtB	= decOpFzA_idUIxt;

//				opRegCM	= decOpFzA_idRegN;
//				opRegCO	= decOpFzB_idRegN;
				opRegCM	= decOpFzA_idRegP;
				opRegCO	= decOpFzB_idRegP;
				opRegCN	= JX2_GR_ZZR;
				opImmC	= UV33_XX;
				opUCmdC	= UV9_00;
				opUIxtC	= UV9_00;

				opIsScalar	= opIsWexJumboA;

				if(opIsWexJumboA)
				begin
					if(decOpFzC_idUFl[0])
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

`ifndef def_true
				if(opIsDzA)
				begin
					opUCmdB[8:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
				end
				if(opIsDzB)
				begin
					opUCmdA[8:6]=opIsDfB?JX2_IXC_CF:JX2_IXC_CT;
				end
`endif
			end
			else
			begin
`ifdef jx2_enable_riscv

				if(srRiscv)
				begin
					opRegAM0	= decOpRvA_idRegM;
					opRegAO0	= decOpRvA_idRegO;
					opRegAN0	= decOpRvA_idRegN;
					opRegAP0	= decOpRvA_idRegP;
					opImmA0		= decOpRvA_idImm;
					opUCmdA0	= decOpRvA_idUCmd;
					opUIxtA0	= decOpRvA_idUIxt;
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
				end

`else

				opRegAM0	= decOpFzA_idRegM;
				opRegAO0	= decOpFzA_idRegO;
				opRegAN0	= decOpFzA_idRegN;
				opRegAP0	= decOpFzA_idRegP;
				opImmA0		= decOpFzA_idImm;
				opUCmdA0	= decOpFzA_idUCmd;
				opUIxtA0	= decOpFzA_idUIxt;

`endif

`ifdef def_true
				opRegAM	= opRegAM0;
				opRegAO	= opRegAO0;
				opRegAN	= opRegAN0;
				opImmA	= opImmA0;
				opUCmdA	= opUCmdA0;
				opUIxtA	= opUIxtA0;
`endif

`ifndef def_true
				opRegAM	= decOpFzA_idRegM;
				opRegAO	= decOpFzA_idRegO;
				opRegAN	= decOpFzA_idRegN;
				opImmA	= decOpFzA_idImm;
				opUCmdA	= decOpFzA_idUCmd;
				opUIxtA	= decOpFzA_idUIxt;

				opRegAM0	= decOpFzA_idRegM;
				opRegAO0	= decOpFzA_idRegO;
				opRegAN0	= decOpFzA_idRegN;
				opRegAP0	= decOpFzA_idRegP;
				opUCmdA0	= decOpFzA_idUCmd;
				opUIxtA0	= decOpFzA_idUIxt;
`endif

				opRegBM	= JX2_GR_ZZR;
//				opRegBO	= decOpFzA_idRegN;
				opRegBO	= opRegAN0;
				opRegBN	= JX2_GR_ZZR;
				opImmB	= UV33_XX;
				opUCmdB	= UV9_00;
				opUIxtB	= UV9_00;
				
				opRegCM	= JX2_GR_ZZR;
//				opRegCO	= decOpFzA_idRegN;
//				opRegCO	= decOpFzA_idRegP;
				opRegCO	= opRegAP0;
				opRegCN	= JX2_GR_ZZR;
				opImmC	= UV33_XX;
				opUCmdC	= UV9_00;
				opUIxtC	= UV9_00;

				opIsScalar	= 1;

//				if(opIsDzA)
//				begin
//					opUCmdA[8:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
//				end
				
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

`ifdef jx2_enable_ops24
		if(	(istrWord[15:12] == 4'h9) ||
			(istrWord[15:12] == 4'h7)	)
		begin
			opRegAM	= decOpHz_idRegM;
			opRegAO	= decOpHz_idRegO;
			opRegAN	= decOpHz_idRegN;
			opImmA	= decOpHz_idImm;
			opUCmdA	= decOpHz_idUCmd;
			opUIxtA	= decOpHz_idUIxt;
		end
`endif

//		opRegXM = decOpBz_idRegM;
//		opRegXO = decOpBz_idRegO;
//		opRegXN = decOpBz_idRegN;

//		opRegAM0	= decOpBz_idRegM;
//		opRegAO0	= decOpBz_idRegO;
//		opRegAN0	= decOpBz_idRegN;
//		opUCmdA0	= decOpBz_idUCmd;
//		opUIxtA0	= decOpBz_idUIxt;

		opRegAM0	= opRegAM;
		opRegAO0	= opRegAO;
		opRegAN0	= opRegAN;
		opRegAP0	= opRegAN;
		opUCmdA0	= opUCmdA;
		opUIxtA0	= opUIxtA;

		opRegBN	= JX2_GR_ZZR;
		opRegBM	= JX2_GR_ZZR;
//		opRegBO	= decOpBz_idRegN;
		opRegBO	= opRegAN;
		opImmB	= UV33_XX;
		opUCmdB	= UV9_00;
		opUIxtB	= UV9_00;

		opRegCM	= JX2_GR_ZZR;
//		opRegCO	= decOpBz_idRegN;
		opRegCO	= opRegAN;
		opRegCN	= JX2_GR_ZZR;
		opImmC	= UV33_XX;
		opUCmdC	= UV9_00;
		opUIxtC	= UV9_00;
		
		opIsScalar	= 1;
	end

`ifndef def_true
	if(opIsFxA)
	begin
//		opRegXM = { istrWord[ 0], istrWord[25], istrWord[ 3: 1], 1'b0 };
//		opRegXO = { istrWord[20], istrWord[24], istrWord[23:21], 1'b0 };
//		opRegXN = { istrWord[ 4], istrWord[26], istrWord[ 7: 5], 1'b0 };

//		opRegXM	= { decOpFzA_idRegM[0], decOpFzA_idRegM[4:1], 1'b0 };
//		opRegXO	= { decOpFzA_idRegO[0], decOpFzA_idRegO[4:1], 1'b0 };
//		opRegXN	= { decOpFzA_idRegN[0], decOpFzA_idRegN[4:1], 1'b0 };

		opRegXM	= { opRegAM0[0], opRegAM0[4:1], 1'b0 };
		opRegXO	= { opRegAO0[0], opRegAO0[4:1], 1'b0 };
		opRegXN	= { opRegAN0[0], opRegAN0[4:1], 1'b0 };

	end
	else
	begin
		opRegXM = decOpBz_idRegM;
		opRegXO = decOpBz_idRegO;
		opRegXN = decOpBz_idRegN;
	end
`endif

//	opRegXM	= { opRegAM0[0], opRegAM0[4:1], 1'b0 };
//	opRegXO	= { opRegAO0[0], opRegAO0[4:1], 1'b0 };
//	opRegXN	= { opRegAN0[0], opRegAN0[4:1], 1'b0 };

//	if(opIsScalar)
	if(1'b1)
	begin

`ifndef def_true
// `ifdef def_true

`ifndef def_true
//		if(opUCmdA[5:0]==JX2_UCMD_POPX)
		if(opUCmdA0[5:1]==5'h03)
		begin
//			opIsDualLane = decOpBz_idUIxt[2];
			opIsDualLane = opUIxtA0[2];
		end
`endif

`ifndef def_true
//		if(opUCmdA0[5:0]==JX2_UCMD_MOV_MR)
		if(opUCmdA0[5:1]==5'h02)
		begin
//			opIsDualLane = (decOpBz_idUIxt[2:0]==3'b111);
//			opIsDualLane = (opUIxtA[2:0]==3'b111);
			opIsDualLane = ( {opUIxtA0[2], opUIxtA0[5:4]} == 3'b111);
			opIsDualLaneRn	= 1;
		end
`endif

`ifndef def_true
		if(opUCmdA0[5:0]==JX2_UCMD_FPU3)
		begin
			opIsDualLane = opUIxtA0[5];
			opIsDualLane3R = 1;
		end
`endif

`endif

		if(opUCmdA0[5:1] == 5'h02)
		begin
			opIsBaseRm	= 1;
		end

`ifdef def_true
// `ifndef def_true
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
//				opIsDualLane3R	= 0;
//				opIsDualLane2R	= 1;

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
		opRegXM		= opRegAM0;
		opRegXMv	= opCmRemapRx[opRegAM0[4:0]];
	end
	if(opRegAN0[6])
	begin
		opRegXN		= opRegAN0;
		opRegXNv	= opCmRemapRx[opRegAN0[4:0]];
	end
	if(opRegAP0[6])
	begin
		opRegXP		= opRegAP0;
		opRegXPv	= opCmRemapRx[opRegAP0[4:0]];
	end
	
`else

	opRegXM	= { opRegAM0[0], opRegAM0[4:1], opDualLaneSw };
	opRegXO	= { opRegAO0[0], opRegAO0[4:1], opDualLaneSw };
	opRegXN	= { opRegAN0[0], opRegAN0[4:1], opDualLaneSw };
	opRegXP	= { opRegAP0[0], opRegAP0[4:1], opDualLaneSw };

`endif

//	opRegXM	= { opRegAM0[0]^opRegAM0[5], opRegAM0[4:1], opDualLaneSw };
//	opRegXO	= { opRegAO0[0]^opRegAO0[5], opRegAO0[4:1], opDualLaneSw };
//	opRegXN	= { opRegAN0[0]^opRegAN0[5], opRegAN0[4:1], opDualLaneSw };

//	opRegXM	= { opRegAM0[0]^(opRegAM0[4:1]==4'b0), opRegAM0[4:1], opDualLaneSw };
//	opRegXO	= { opRegAO0[0]^(opRegAO0[4:1]==4'b0), opRegAO0[4:1], opDualLaneSw };
//	opRegXN	= { opRegAN0[0]^(opRegAN0[4:1]==4'b0), opRegAN0[4:1], opDualLaneSw };

`ifdef jx2_enable_vaddr96
// `ifndef def_true
	if(opIsBaseRm)
	begin
		if(	(opUIxtA0[8:6]==JX2_IUC_WA) ||
			(opUIxtA0[8:6]==JX2_IUC_WXA))
//		if(1'b0)
		begin
			opRegAM = opRegXM;
			opRegBM = opRegXMv;
		end
		else
		begin
//			opRegBM = JX2_GR_GBR_HI;
//			if(opRegAM0 == JX2_GR_PC)
//				opRegBM = JX2_GR_PC_HI;
		end

	end
`endif

	if(opIsDualLane)
	begin
		if(!opIsScalar)
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
		
		opRegBN	= opRegAN;
		opRegBM	= opRegAM;
		opRegBO	= opRegAO;
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
//			opRegBN	= opRegAN;
//			opRegBN[0] = !opRegAN[0];

			if(opIsScalar)
			begin
				opRegCO		= opRegXP;
				opRegCM		= opRegXPv;
//				opRegCM		= opRegCO;
//				opRegCM[0]	= !opRegCO[0];
			end
		end

`ifdef def_true
		if(opIsDualLaneRm)
		begin
			opRegAM = opRegXM;
			opRegBM = opRegXMv;
//			opRegBM[0] = !opRegAM[0];
		end

		if(opIsDualLaneRo)
		begin
			opRegAO = opRegXO;
			opRegBO = opRegXOv;
//			opRegBO[0] = !opRegAO[0];
		end
`endif

		if(opIsScalar)
		begin
//			opRegCM	= opRegBN;
//			opRegCO	= opRegAN;
		end
	end
end

endmodule
