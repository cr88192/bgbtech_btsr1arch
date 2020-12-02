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
`include "DecOpFC.v"

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

reg[32:0]		opImmA;
reg[7:0]		opUCmdA;
reg[7:0]		opUIxtA;

reg[32:0]		opImmB;
reg[7:0]		opUCmdB;
reg[7:0]		opUIxtB;

reg[32:0]		opImmC;
reg[7:0]		opUCmdC;
reg[7:0]		opUIxtC;

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
wire[23:0]		tOpJBitsA;
wire[23:0]		tOpJBitsB;
wire[23:0]		tOpJBitsC;

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

DecOpFz	decOpFzC(
	clock,		reset,
	{ UV32_XX, istrWord[95:64] },	1'b1,	{ opIsWexJumboB, tOpJBitsC },
	decOpFzC_idRegN,		decOpFzC_idRegM,
	decOpFzC_idRegO,		decOpFzC_idImm,
	decOpFzC_idUCmd,		decOpFzC_idUIxt
	);

wire[5:0]		decOpFzB_idRegN;
wire[5:0]		decOpFzB_idRegM;
wire[5:0]		decOpFzB_idRegO;
wire[32:0]		decOpFzB_idImm;
wire[7:0]		decOpFzB_idUCmd;
wire[7:0]		decOpFzB_idUIxt;

DecOpFz	decOpFzB(
	clock,		reset,
	{ UV32_XX, istrWord[63:32] },	1'b1,	{ opIsWexJumboA, tOpJBitsB },
	decOpFzB_idRegN,		decOpFzB_idRegM,
	decOpFzB_idRegO,		decOpFzB_idImm,
	decOpFzB_idUCmd,		decOpFzB_idUIxt
	);

wire[5:0]		decOpFzA_idRegN;
wire[5:0]		decOpFzA_idRegM;
wire[5:0]		decOpFzA_idRegO;
wire[32:0]		decOpFzA_idImm;
wire[7:0]		decOpFzA_idUCmd;
wire[7:0]		decOpFzA_idUIxt;

DecOpFz	decOpFzA(
	clock,		reset,
	{ UV32_XX, istrWord[31: 0] },	1'b0,	UV25_00,
	decOpFzA_idRegN,		decOpFzA_idRegM,
	decOpFzA_idRegO,		decOpFzA_idImm,
	decOpFzA_idUCmd,		decOpFzA_idUIxt
	);

`ifndef jx2_enable_ops48
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

reg opIsFxA;
reg opIsFzA;
reg opIsFCA;
reg opIsDzA;	//Predicated Ops
reg opIsDwA;		//PrWEX Ops
reg opIsDfA;	//Pred-False or WEX
reg opIsWfA;	//WEX

reg opIsScalar;		//Scalar Operation
reg opIsDualLane;	//Op uses both lanes
reg opIsDualLane3R;	//Op uses both lanes (with all 3 regs as 128b)

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
assign	tOpJBitsB		= { istrWord [7: 0], istrWord[31:16] };
assign	tOpJBitsC		= { istrWord[39:32], istrWord[63:48] };

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

	opIsDwA = 0;
	opIsDwB = 0;
	opIsDwC = 0;
	opIsWexJumboLdi	= 0;
	opIsScalar		= 0;
	opIsDualLane	= 0;
	opIsDualLane3R	= 0;

	casez(istrWord[15:10])
//		6'b11100z: begin	//E0..E7
		6'b1110zz: begin	//E0..EF
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 1;
			opIsDfA = istrWord[10];
		end

`ifndef def_true
		6'b111010: begin	//E8..EB
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 1;
			opIsDfA = istrWord[8];
			opIsDwA = istrWord[9];
		end
		6'b111011: begin	//EC..EF
			opIsFxA = 1;		opIsFzA = 0;
			opIsFCA = 1;		opIsDzA = 1;
			opIsDfA = istrWord[9];
		end
`endif

//		6'b11110z: begin	//F0..F7
		6'b1111zz: begin	//F0..FF
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
			opIsDfA = istrWord[10];
		end

`ifndef def_true
		6'b111110: begin	//F8..FB
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
//			opIsDfA = istrWord[8];
			opIsDfA = istrWord[8] && !istrWord[9];
		end
		6'b111111: begin	//FC..FF
			opIsFxA = 1;		opIsFzA = 0;
			opIsFCA = 1;		opIsDzA = 0;
			opIsDfA = istrWord[9];
		end
`endif

		default: begin
			opIsFxA = 0;	opIsFzA = 0;
			opIsFCA = 0;	opIsDzA = 0;
			opIsDfA = 0;
		end
	endcase

	casez(istrWord[47:42])
//		6'b11100z: begin	//E0..E7
		6'b1110zz: begin	//E0..EF
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 1;
			opIsDfB = istrWord[42];
		end

`ifndef def_true
		6'b111010: begin	//E8..EB
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 1;
			opIsDfB = istrWord[40];
			opIsDwB = istrWord[41];
		end
		6'b111011: begin	//EC..EF
			opIsFxB = 1;		opIsFzB = 0;
			opIsFCB = 1;		opIsDzB = 1;
			opIsDfB = istrWord[41];
		end
`endif

//		6'b11110z: begin	//F0..F7
		6'b1111zz: begin	//F0..FF
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
			opIsDfB = istrWord[42];
		end

`ifndef def_true
		6'b111110: begin	//F8..FB
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
//			opIsDfB = istrWord[40];
			opIsDfB = istrWord[40] && !istrWord[41];
		end
		6'b111111: begin	//FC..FF
			opIsFxB = 1;		opIsFzB = 0;
			opIsFCB = 1;		opIsDzB = 0;
			opIsDfB = istrWord[41];
		end
`endif

		default: begin
			opIsFxB = 0;	opIsFzB = 0;
			opIsFCB = 0;	opIsDzB = 0;
			opIsDfB = 0;
		end
	endcase

	casez(istrWord[79:74])
//		6'b11100z: begin	//E0..E7
		6'b1110zz: begin	//E0..EF
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 1;
			opIsDfC = istrWord[74];
		end


`ifndef def_true
		6'b111010: begin	//E8..EB
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 1;
			opIsDfC = istrWord[72];
			opIsDwC = istrWord[73];
		end
		6'b111011: begin	//EC..EF
			opIsFxC = 1;		opIsFzC = 0;
			opIsFCC = 1;		opIsDzC = 1;
			opIsDfC = istrWord[73];
		end
`endif

//		6'b11110z: begin	//F0..F7
		6'b1111zz: begin	//F0..FF
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 0;
			opIsDfC = istrWord[74];
		end

`ifndef def_true
		6'b111110: begin	//F8..FB
			opIsFxC = 1;		opIsFzC = 1;
			opIsFCC = 0;		opIsDzC = 0;
//			opIsDfC = istrWord[72];
			opIsDfC = istrWord[72] && !istrWord[73];
		end

		6'b111111: begin	//FC..FF
			opIsFxC = 1;		opIsFzC = 0;
			opIsFCC = 1;		opIsDzC = 0;
			opIsDfC = istrWord[73];
		end
`endif

		default: begin
			opIsFxC = 0;	opIsFzC = 0;
			opIsFCC = 0;	opIsDzC = 0;
			opIsDfC = 0;
		end
	endcase

//	opIsWfA = opIsDfA && !opIsDzA && srWxe;
//	opIsWfB = opIsDfB && !opIsDzB && srWxe;

	opIsWfA = opIsDfA && (!opIsDzA || opIsDwA) && srWxe;
	opIsWfB = opIsDfB && (!opIsDzB || opIsDwB) && srWxe;

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
		
`ifndef def_true
		if(opIsWexJumboLdi)
		begin
			opRegAM	= opWexJumboRn;
			opRegAO	= JX2_GR_JIMM;
			opRegAN	= opWexJumboRn;

			opUCmdA	= { JX2_IXC_AL, JX2_UCMD_MOV_IR };
			opUIxtA	= { JX2_IXC_AL, JX2_UCIX_LDI_JLDIX };
		end
		else
		begin
`ifndef jx2_enable_ops48
			opRegAN	= decOpFC_idRegN;
			opRegAM	= decOpFC_idRegM;
			opRegAO	= decOpFC_idRegO;
			opImmA	= decOpFC_idImm;
			opUCmdA	= decOpFC_idUCmd;
			opUIxtA	= decOpFC_idUIxt;
`endif
		end
`endif

		opRegAM	= decOpFzC_idRegM;
		opRegAO	= decOpFzC_idRegO;
		opRegAN	= decOpFzC_idRegN;
		opImmA	= decOpFzC_idImm;
		opUCmdA	= decOpFzC_idUCmd;
		opUIxtA	= decOpFzC_idUIxt;

		opRegBM	= JX2_GR_ZZR;
		opRegBO	= JX2_GR_ZZR;
		opRegBN	= JX2_GR_ZZR;
//		opImmB	= decOpFzB_idImm;
		opImmB	= { 1'b0, tOpJBitsB[23:0], tOpJBitsC[23:16] };
		opUCmdB	= UV8_00;
		opUIxtB	= UV8_00;

		opRegCM	= JX2_GR_ZZR;
		opRegCO	= JX2_GR_ZZR;
		opRegCN	= JX2_GR_ZZR;
//		opImmC	= decOpFzA_idImm;
		opImmC	= UV33_XX;
		opUCmdC	= UV8_00;
		opUIxtC	= UV8_00;
	end
	else
`endif
	if(opIsFxA)
	begin
		if(opIsFCA)
		begin
`ifndef jx2_enable_ops48
			opRegAN	= decOpFC_idRegN;
			opRegAM	= decOpFC_idRegM;
			opRegAO	= decOpFC_idRegO;
			opImmA	= decOpFC_idImm;
			opUCmdA	= decOpFC_idUCmd;
			opUIxtA	= decOpFC_idUIxt;

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

		opRegBN	= JX2_GR_ZZR;
		opRegBM	= JX2_GR_ZZR;
		opRegBO	= decOpBz_idRegN;
		opImmB	= UV33_XX;
		opUCmdB	= UV8_00;
		opUIxtB	= UV8_00;

		opRegCM	= JX2_GR_ZZR;
		opRegCO	= decOpBz_idRegN;
		opRegCN	= JX2_GR_ZZR;
		opImmC	= UV33_XX;
		opUCmdC	= UV8_00;
		opUIxtC	= UV8_00;
		
		opIsScalar	= 1;
	end

//	if(opIsScalar)
	if(1'b1)
	begin
//		if(opUCmdA[5:0]==JX2_UCMD_POPX)
		if(opUCmdA[5:1]==5'h03)
		begin
//			opIsDualLane = decOpBz_idUIxt[2];
			opIsDualLane = opUIxtA[2];
		end

//		if(opUCmdA[5:0]==JX2_UCMD_MOV_MR)
		if(opUCmdA[5:1]==5'h02)
		begin
//			opIsDualLane = (decOpBz_idUIxt[2:0]==3'b111);
//			opIsDualLane = (opUIxtA[2:0]==3'b111);
			opIsDualLane = ( {opUIxtA[2], opUIxtA[5:4]} == 3'b111);
		end

		if(opUCmdA[5:0]==JX2_UCMD_FPU3)
		begin
			opIsDualLane = opUIxtA[5];
			opIsDualLane3R = 1;
		end
	end
	
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
		opRegBN[0] = !opRegAN[0];

//		opRegBN	= opRegAN;
		opRegBM	= opRegAM;
		opRegBO	= opRegAO;
		opImmB	= opImmA;
		opUCmdB	= opUCmdA;
		opUIxtB	= opUIxtA;

		if(opIsDualLane3R)
		begin
			opRegBM[0] = !opRegAM[0];
			opRegBO[0] = !opRegAO[0];
		end

		if(opIsScalar)
		begin
			opRegCM	= opRegBN;
			opRegCO	= opRegAN;
		end
	end
end

endmodule
