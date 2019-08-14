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

module DecOpWx2(
	/* verilator lint_off UNUSED */
	clock,		reset,
	istrWord,	srWxe,
	idRegS,		idRegT,		idRegM,
	idImmA,		idUCmdA,	idUIxtA,
	idRegU,		idRegV,		idRegN,
	idImmB,		idUCmdB,	idUIxtB
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

wire[5:0]		decOpBz_idRegN;
wire[5:0]		decOpBz_idRegM;
wire[5:0]		decOpBz_idRegO;
wire[32:0]		decOpBz_idImm;
wire[7:0]		decOpBz_idUCmd;
wire[7:0]		decOpBz_idUIxt;

DecOpBz	decOpBz(
	clock,		reset,
	istrWord,
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
	{ UV32_XX, istrWord[95:64] },	1'b1,
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
	{ UV32_XX, istrWord[63:32] },	1'b1,
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
	{ UV32_XX, istrWord[31: 0] },	1'b0,
	decOpFzA_idRegN,		decOpFzA_idRegM,
	decOpFzA_idRegO,		decOpFzA_idImm,
	decOpFzA_idUCmd,		decOpFzA_idUIxt
	);

wire[5:0]		decOpFC_idRegN;
wire[5:0]		decOpFC_idRegM;
wire[5:0]		decOpFC_idRegO;
wire[32:0]		decOpFC_idImm;
wire[7:0]		decOpFC_idUCmd;
wire[7:0]		decOpFC_idUIxt;

DecOpFC	decOpFC(
	clock,		reset,
	istrWord,
	decOpFC_idRegN,		decOpFC_idRegM,
	decOpFC_idRegO,		decOpFC_idImm,
	decOpFC_idUCmd,		decOpFC_idUIxt
	);

reg opIsFxA;
reg opIsFzA;
reg opIsFCA;
reg opIsDzA;	//Predicated Ops
reg opIsDfA;	//Pred-False or WEX
reg opIsWfA;	//WEX

reg opIsFxB;
reg opIsFzB;
reg opIsFCB;
reg opIsDzB;		//Predicated Ops
reg opIsDfB;		//Pred-False or WEX
reg opIsWfB;		//WEX

always @*
begin

	casez(istrWord[15:10])
		6'b11100z: begin	//E0..E7
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 1;
			opIsDfA = istrWord[10];
		end
		6'b111010: begin	//E8..EB
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 1;
			opIsDfA = istrWord[8];
		end
		6'b111011: begin	//EC..EF
			opIsFxA = 1;		opIsFzA = 0;
			opIsFCA = 1;		opIsDzA = 1;
			opIsDfA = istrWord[9];
		end

		6'b11110z: begin	//F0..F7
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
			opIsDfA = istrWord[10];
		end
		6'b111110: begin	//F8..FB
			opIsFxA = 1;		opIsFzA = 1;
			opIsFCA = 0;		opIsDzA = 0;
			opIsDfA = istrWord[8];
		end
		6'b111111: begin	//FC..FF
			opIsFxA = 1;		opIsFzA = 0;
			opIsFCA = 1;		opIsDzA = 0;
			opIsDfA = istrWord[9];
		end

		default: begin
			opIsFxA = 0;	opIsFzA = 0;
			opIsFCA = 0;	opIsDzA = 0;
			opIsDfA = 0;
		end
	endcase

	casez(istrWord[47:42])
		6'b11100z: begin	//E0..E7
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 1;
			opIsDfB = istrWord[42];
		end
		6'b111010: begin	//E8..EB
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 1;
			opIsDfB = istrWord[40];
		end
		6'b111011: begin	//EC..EF
			opIsFxB = 1;		opIsFzB = 0;
			opIsFCB = 1;		opIsDzB = 1;
			opIsDfB = istrWord[41];
		end

		6'b11110z: begin	//F0..F7
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
			opIsDfB = istrWord[42];
		end
		6'b111110: begin	//F8..FB
			opIsFxB = 1;		opIsFzB = 1;
			opIsFCB = 0;		opIsDzB = 0;
			opIsDfB = istrWord[40];
		end
		6'b111111: begin	//FC..FF
			opIsFxB = 1;		opIsFzB = 0;
			opIsFCB = 1;		opIsDzB = 0;
			opIsDfB = istrWord[41];
		end

		default: begin
			opIsFxB = 0;	opIsFzB = 0;
			opIsFCB = 0;	opIsDzB = 0;
			opIsDfB = 0;
		end
	endcase

	opIsWfA = opIsDfA && !opIsDzA && srWxe;
	opIsWfB = opIsDfB && !opIsDzB && srWxe;

	if(opIsFxA)
	begin
		if(opIsFCA)
		begin
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
		end
		else
		begin
			if(opIsWfA && opIsWfB)
			begin
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
			end
			else
			if(opIsWfA)
			begin
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
	end
	
	if(opIsDzA)
	begin
		opUCmdA[7:6]=opIsDfA?JX2_IXC_CF:JX2_IXC_CT;
	end
end

endmodule
