/*
BJX2 Instruction Decoder (WEX, 2-Wide)

Takes a 16/32/48 bit instruction word.

Ex block is predicated.
Fx block is unconditional.

This decoder will support WEX.
  Rs/Rt/Rm: Lane 1
  Ru/Rv/Rn: Lane 2

For scalar Ops, Lane 2 will hold:
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
	);

input			clock;		//clock
input			reset;		//clock

input[63:0]		istrWord;	//source instruction word
input			srWxe;

output[5:0]		idRegS;
output[5:0]		idRegT;
output[5:0]		idRegM;

output[5:0]		idRegU;
output[5:0]		idRegV;
output[5:0]		idRegN;

output[32:0]	idImmA;
output[7:0]		idUCmdA;
output[7:0]		idUIxtA;

output[32:0]	idImmB;
output[7:0]		idUCmdB;
output[7:0]		idUIxtB;


reg[5:0]		opRegAN;
reg[5:0]		opRegAM;
reg[5:0]		opRegAO;

reg[5:0]		opRegBN;
reg[5:0]		opRegBM;
reg[5:0]		opRegBO;

reg[32:0]		opImmA;
reg[7:0]		opUCmdA;
reg[7:0]		opUIxtA;

reg[32:0]		opImmB;
reg[7:0]		opUCmdB;
reg[7:0]		opUIxtB;

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

reg opIsFx;
reg opIsFz;
reg opIsFC;
reg opIsDz;		//Predicated Ops
reg opIsDw;		//PrWEX Ops
reg opIsDf;		//Pred-False or WEX
reg opIsWf;		//WEX

always @*
begin

	opIsDw = 0;
	casez(istrWord[15:10])
		6'b11100z: begin	//E0..E7
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 1;
			opIsDf = istrWord[10];
		end
		6'b111010: begin	//E8..EB
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 1;
			opIsDw = istrWord[9];
			opIsDf = istrWord[8];
		end
		6'b111011: begin	//EC..EF
			opIsFx = 1;		opIsFz = 0;
			opIsFC = 1;		opIsDz = 1;
			opIsDf = istrWord[9];
		end

		6'b11110z: begin	//F0..F7
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = istrWord[10];
		end
		6'b111110: begin	//F8..FB
			opIsFx = 1;		opIsFz = 1;
			opIsFC = 0;		opIsDz = 0;
//			opIsDf = istrWord[8];
			opIsDf = istrWord[8] && !istrWord[9];
		end
		6'b111111: begin	//FC..FF
			opIsFx = 1;		opIsFz = 0;
			opIsFC = 1;		opIsDz = 0;
			opIsDf = istrWord[9];
		end

		default: begin
			opIsFx = 0;		opIsFz = 0;
			opIsFC = 0;		opIsDz = 0;
			opIsDf = 0;
		end
	endcase

	opIsWf = opIsDf && (!opIsDz || opIsDw) && srWxe;

	if(opIsFx)
	begin
		if(opIsFC)
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

			if(opIsDz)
			begin
				opUCmdA[7:6]=opIsDf?JX2_IXC_CF:JX2_IXC_CT;
			end
		end
		else
		begin
			if(opIsWf)
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
				
				if(opIsDz)
				begin
					opUCmdA[7:6]=opIsDf?JX2_IXC_CF:JX2_IXC_CT;
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
	end
end

endmodule
