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

module DecOpWx3(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	srWxe,
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
input			srWxe;

output[5:0]		idRegS;
output[5:0]		idRegT;
output[5:0]		idRegM;

output[5:0]		idRegU;
output[5:0]		idRegV;
output[5:0]		idRegN;

output[5:0]		idRegX;
output[5:0]		idRegY;
output[5:0]		idRegO;

output[32:0]	idImmA;
output[7:0]		idUCmdA;
output[7:0]		idUIxtA;

output[32:0]	idImmB;
output[7:0]		idUCmdB;
output[7:0]		idUIxtB;

output[32:0]	idImmC;
output[7:0]		idUCmdC;
output[7:0]		idUIxtC;


reg[5:0]		opRegAM;
reg[5:0]		opRegAO;
reg[5:0]		opRegAN;

reg[5:0]		opRegBM;
reg[5:0]		opRegBO;
reg[5:0]		opRegBN;

reg[5:0]		opRegCM;
reg[5:0]		opRegCO;
reg[5:0]		opRegCN;

reg[5:0]		opRegAM0;
reg[5:0]		opRegAO0;
reg[5:0]		opRegAN0;


reg[5:0]		opRegXM;
reg[5:0]		opRegXO;
reg[5:0]		opRegXN;

reg[32:0]		opImmA;
reg[7:0]		opUCmdA;
reg[7:0]		opUIxtA;

reg[32:0]		opImmB;
reg[7:0]		opUCmdB;
reg[7:0]		opUIxtB;

reg[32:0]		opImmC;
reg[7:0]		opUCmdC;
reg[7:0]		opUIxtC;

reg[32:0]		opImmA0;
reg[7:0]		opUCmdA0;
reg[7:0]		opUIxtA0;

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
reg[5:0]	opWexJumboRn;


wire[5:0]		decOpBz_idRegN;
wire[5:0]		decOpBz_idRegM;
wire[5:0]		decOpBz_idRegO;
wire[32:0]		decOpBz_idImm;
wire[7:0]		decOpBz_idUCmd;
wire[7:0]		decOpBz_idUIxt;

DecOpBz	decOpBz(
	clock,		reset,
	istrWord[63:0],
	decOpBz_idRegN,		decOpBz_idRegM,
	decOpBz_idRegO,		decOpBz_idImm,
	decOpBz_idUCmd,		decOpBz_idUIxt
	);

wire[5:0]		decOpFzC_idRegN;
wire[5:0]		decOpFzC_idRegM;
wire[5:0]		decOpFzC_idRegO;
wire[32:0]		decOpFzC_idImm;
wire[7:0]		decOpFzC_idUCmd;
wire[7:0]		decOpFzC_idUIxt;
wire[3:0]		decOpFzC_idUFl;

DecOpFz	decOpFzC(
	clock,		reset,
	{ UV32_XX, istrWord[95:64] },	4'h1,
		{ tOpJBitsB[24], tOpJBitsC[24],
		opIsWexJumbo96, opIsWexJumboB, tOpJBitsC[23:0] },
	decOpFzC_idRegN,		decOpFzC_idRegM,
	decOpFzC_idRegO,		decOpFzC_idImm,
	decOpFzC_idUCmd,		decOpFzC_idUIxt,
	decOpFzC_idUFl
	);

wire[5:0]		decOpFzB_idRegN;
wire[5:0]		decOpFzB_idRegM;
wire[5:0]		decOpFzB_idRegO;
wire[32:0]		decOpFzB_idImm;
wire[7:0]		decOpFzB_idUCmd;
wire[7:0]		decOpFzB_idUIxt;
wire[3:0]		decOpFzB_idUFl;

DecOpFz	decOpFzB(
	clock,		reset,
	{ UV32_XX, istrWord[63:32] },	4'h1,
		{ 1'b0, tOpJBitsB[24], 1'b0, opIsWexJumboA, tOpJBitsB[23:0] },
	decOpFzB_idRegN,		decOpFzB_idRegM,
	decOpFzB_idRegO,		decOpFzB_idImm,
	decOpFzB_idUCmd,		decOpFzB_idUIxt,
	decOpFzB_idUFl
	);

wire[5:0]		decOpFzA_idRegN;
wire[5:0]		decOpFzA_idRegM;
wire[5:0]		decOpFzA_idRegO;
wire[32:0]		decOpFzA_idImm;
wire[7:0]		decOpFzA_idUCmd;
wire[7:0]		decOpFzA_idUIxt;
wire[3:0]		decOpFzA_idUFl;

DecOpFz	decOpFzA(
	clock,		reset,
	{ UV32_XX, istrWord[31: 0] },	4'h0,	UV28_00,
	decOpFzA_idRegN,		decOpFzA_idRegM,
	decOpFzA_idRegO,		decOpFzA_idImm,
	decOpFzA_idUCmd,		decOpFzA_idUIxt,
	decOpFzA_idUFl
	);

`ifdef jx2_enable_ops48
wire[5:0]		decOpFC_idRegN;
wire[5:0]		decOpFC_idRegM;
wire[5:0]		decOpFC_idRegO;
wire[32:0]		decOpFC_idImm;
wire[7:0]		decOpFC_idUCmd;
wire[7:0]		decOpFC_idUIxt;

DecOpFC	decOpFC(
	clock,		reset,
	istrWord[63:0],
	decOpFC_idRegN,		decOpFC_idRegM,
	decOpFC_idRegO,		decOpFC_idImm,
	decOpFC_idUCmd,		decOpFC_idUIxt
	);
`endif

`ifdef jx2_enable_ops24
wire[5:0]		decOpHz_idRegN;
wire[5:0]		decOpHz_idRegM;
wire[5:0]		decOpHz_idRegO;
wire[32:0]		decOpHz_idImm;
wire[7:0]		decOpHz_idUCmd;
wire[7:0]		decOpHz_idUIxt;
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

assign	opIsWexJumboA =
		(istrWord[15: 8] == 8'b1111_1110) ;
assign	opIsWexJumboB =
		(istrWord[47:40] == 8'b1111_1110) ;

assign	opIsWexJumbo96 =
//	opIsWexJumboA && istrWord[42];
	opIsWexJumboA && opIsWexJumboB;

assign	tOpJBitsA		= 0;
// assign	tOpJBitsB		= { istrWord [7: 0], istrWord[29:16] };
// assign	tOpJBitsC		= { istrWord[39:32], istrWord[61:48] };
assign	tOpJBitsB		= { istrWord [8: 0], istrWord[31:16] };
assign	tOpJBitsC		= { istrWord[40:32], istrWord[63:48] };

`else

assign	opIsWexJumboA	= 0;
assign	opIsWexJumboB	= 0;
assign	opIsWexJumbo96	= 0;
assign	tOpJBitsA		= 0;
assign	tOpJBitsB		= 0;
assign	tOpJBitsC		= 0;

`endif

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

	casez(istrWord[15:10])
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

		opUCmdB	= UV8_00;
		opUIxtB	= UV8_00;

		opRegCM	= JX2_GR_ZZR;
		opRegCO	= JX2_GR_ZZR;
		opRegCN	= JX2_GR_ZZR;
		opImmC	= UV33_XX;
		opUCmdC	= UV8_00;
		opUIxtC	= UV8_00;

		opIsScalar	= 1;

		if(opIsDzC)
		begin
			opUCmdA[7:6]=opIsDfC?JX2_IXC_CF:JX2_IXC_CT;
		end
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
			opRegAM0	= decOpFC_idRegM;
			opRegAO0	= decOpFC_idRegO;
			opUCmdA0	= decOpFC_idUCmd;
			opUIxtA0	= decOpFC_idUIxt;

			opRegBN	= JX2_GR_ZZR;
			opRegBM	= JX2_GR_ZZR;
			opRegBO	= decOpFC_idRegN;
			opImmB	= UV33_XX;
			opUCmdB	= UV8_00;
			opUIxtB	= UV8_00;
				
			opRegCM	= JX2_GR_ZZR;
			opRegCO	= decOpFC_idRegN;
			opRegCN	= JX2_GR_ZZR;
			opImmC	= UV33_XX;
			opUCmdC	= UV8_00;
			opUIxtC	= UV8_00;

			if(opIsDzA)
			begin
				opUCmdA[7:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
			end
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
				opImmA	= decOpFzC_idImm;
				opUCmdA	= decOpFzC_idUCmd;
				opUIxtA	= decOpFzC_idUIxt;

				opRegAM0	= decOpFzC_idRegM;
				opRegAO0	= decOpFzC_idRegO;
				opRegAN0	= decOpFzC_idRegN;
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

				if(opIsDzA)
				begin
					opUCmdC[7:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
				end
				if(opIsDzB)
				begin
					opUCmdB[7:6]=opIsDfB?JX2_IXC_CF:JX2_IXC_CT;
				end
				if(opIsDzC)
				begin
					opUCmdA[7:6]=opIsDfC?JX2_IXC_CF:JX2_IXC_CT;
				end
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
				opUCmdA0	= decOpFzB_idUCmd;
				opUIxtA0	= decOpFzB_idUIxt;

				opRegBM	= decOpFzA_idRegM;
				opRegBO	= decOpFzA_idRegO;
				opRegBN	= decOpFzA_idRegN;
				opImmB	= decOpFzA_idImm;
				opUCmdB	= decOpFzA_idUCmd;
				opUIxtB	= decOpFzA_idUIxt;

				opRegCM	= decOpFzA_idRegN;
				opRegCO	= decOpFzB_idRegN;
				opRegCN	= JX2_GR_ZZR;
				opImmC	= UV33_XX;
				opUCmdC	= UV8_00;
				opUIxtC	= UV8_00;

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

				if(opIsDzA)
				begin
					opUCmdB[7:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
				end
				if(opIsDzB)
				begin
					opUCmdA[7:6]=opIsDfB?JX2_IXC_CF:JX2_IXC_CT;
				end
			end
			else
			begin
				opRegAM	= decOpFzA_idRegM;
				opRegAO	= decOpFzA_idRegO;
				opRegAN	= decOpFzA_idRegN;
				opImmA	= decOpFzA_idImm;
				opUCmdA	= decOpFzA_idUCmd;
				opUIxtA	= decOpFzA_idUIxt;

				opRegAM0	= decOpFzA_idRegM;
				opRegAO0	= decOpFzA_idRegO;
				opRegAN0	= decOpFzA_idRegN;
				opUCmdA0	= decOpFzA_idUCmd;
				opUIxtA0	= decOpFzA_idUIxt;
				
				opRegBM	= JX2_GR_ZZR;
				opRegBO	= decOpFzA_idRegN;
				opRegBN	= JX2_GR_ZZR;
				opImmB	= UV33_XX;
				opUCmdB	= UV8_00;
				opUIxtB	= UV8_00;
				
				opRegCM	= JX2_GR_ZZR;
				opRegCO	= decOpFzA_idRegN;
				opRegCN	= JX2_GR_ZZR;
				opImmC	= UV33_XX;
				opUCmdC	= UV8_00;
				opUIxtC	= UV8_00;

				opIsScalar	= 1;

				if(opIsDzA)
				begin
					opUCmdA[7:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
				end
				
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
		opUCmdA0	= opUCmdA;
		opUIxtA0	= opUIxtA;

		opRegBN	= JX2_GR_ZZR;
		opRegBM	= JX2_GR_ZZR;
//		opRegBO	= decOpBz_idRegN;
		opRegBO	= opRegAN;
		opImmB	= UV33_XX;
		opUCmdB	= UV8_00;
		opUIxtB	= UV8_00;

		opRegCM	= JX2_GR_ZZR;
//		opRegCO	= decOpBz_idRegN;
		opRegCO	= opRegAN;
		opRegCN	= JX2_GR_ZZR;
		opImmC	= UV33_XX;
		opUCmdC	= UV8_00;
		opUIxtC	= UV8_00;
		
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

`ifdef def_true
// `ifndef def_true
		if(opUIxtA0[7:6]==JX2_IUC_WX)
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

	opRegXM	= { opRegAM0[0], opRegAM0[4:1], opDualLaneSw };
	opRegXO	= { opRegAO0[0], opRegAO0[4:1], opDualLaneSw };
	opRegXN	= { opRegAN0[0], opRegAN0[4:1], opDualLaneSw };

//	opRegXM	= { opRegAM0[0]^opRegAM0[5], opRegAM0[4:1], opDualLaneSw };
//	opRegXO	= { opRegAO0[0]^opRegAO0[5], opRegAO0[4:1], opDualLaneSw };
//	opRegXN	= { opRegAN0[0]^opRegAN0[5], opRegAN0[4:1], opDualLaneSw };

//	opRegXM	= { opRegAM0[0]^(opRegAM0[4:1]==4'b0), opRegAM0[4:1], opDualLaneSw };
//	opRegXO	= { opRegAO0[0]^(opRegAO0[4:1]==4'b0), opRegAO0[4:1], opDualLaneSw };
//	opRegXN	= { opRegAN0[0]^(opRegAN0[4:1]==4'b0), opRegAN0[4:1], opDualLaneSw };
	
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

		if(!opIsDualLaneRn)
		begin
			$display("DL, No Rn opUCmdA=%X", opUCmdA);
		end

		if(opIsDualLaneRn)
//		if(1'b1)
		begin
			opRegAN = opRegXN;
//			if(opDualLaneSw)
//				opRegAN[0] = !opRegAN[0];
//				opRegAN[0] = !opRegXN[0];
			opRegBN	= opRegAN;
			opRegBN[0] = !opRegAN[0];
		end

`ifndef def_true
		if(opIsDualLane3R)
		begin
			if(opDualLaneSw)
			begin
				opRegAM[0] = !opRegAM0[0];
				opRegAO[0] = !opRegAO0[0];
			end

			opRegBM[0] = !opRegAM[0];
			opRegBO[0] = !opRegAO[0];
		end
`endif

`ifdef def_true
		if(opIsDualLaneRm)
		begin
			opRegAM = opRegXM;
//			if(opDualLaneSw)
//				opRegAM[0] = !opRegAM[0];
//				opRegAM[0] = !opRegXM[0];
			opRegBM[0] = !opRegAM[0];
		end

		if(opIsDualLaneRo)
		begin
			opRegAO = opRegXO;
//			if(opDualLaneSw)
//				opRegAO[0] = !opRegAO[0];
//				opRegAO[0] = !opRegXO[0];
			opRegBO[0] = !opRegAO[0];
		end
`endif

		if(opIsScalar)
		begin
			opRegCM	= opRegBN;
			opRegCO	= opRegAN;
		end
	end
end

endmodule
