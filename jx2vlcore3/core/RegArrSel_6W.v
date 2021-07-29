module RegArrSel_6W(
	clock,		reset,
	regIdRs,	regValRsA,
	gprArrA,	gprArrB,
	gprArrC,	gprArrD,
	gprArrE,	gprArrF,
	gprArrMA,
	gprArrMB,
	gprArrMC
	);

input			clock;
input			reset;

input[6:0]		regIdRs;
output[63:0]	regValRsA;

input[63:0]		gprArrA[63:0];
input[63:0]		gprArrB[63:0];
input[63:0]		gprArrC[63:0];
input[63:0]		gprArrD[63:0];
input[63:0]		gprArrE[63:0];
input[63:0]		gprArrF[63:0];
input[63:0]		gprArrMA;
input[63:0]		gprArrMB;
input[63:0]		gprArrMC;

reg[63:0]		tRegValRsA;
assign		regValRsA = tRegValRsA;

reg[63:0]		tRegValA;
reg[63:0]		tRegValB;
reg[63:0]		tRegValC;
reg[63:0]		tRegValD;
reg[63:0]		tRegValE;
reg[63:0]		tRegValF;

reg[2:0]		sel;

always @*
begin

	sel			= {
		gprArrMC[regIdRs[5:0]],
		gprArrMB[regIdRs[5:0]],
		gprArrMA[regIdRs[5:0]]
		};

	tRegValA	= gprArrA[regIdRs[5:0]];
	tRegValB	= gprArrB[regIdRs[5:0]];
	tRegValC	= gprArrC[regIdRs[5:0]];
	tRegValD	= gprArrD[regIdRs[5:0]];
	tRegValE	= gprArrE[regIdRs[5:0]];
	tRegValF	= gprArrF[regIdRs[5:0]];

	case(sel)
		3'b000: tRegValRsA = UV64_00;
		3'b001: tRegValRsA = tRegValA;
		3'b010: tRegValRsA = tRegValB;
		3'b011: tRegValRsA = tRegValC;
		3'b100: tRegValRsA = UV64_00;
		3'b101: tRegValRsA = tRegValD;
		3'b110: tRegValRsA = tRegValE;
		3'b111: tRegValRsA = tRegValF;
	endcase
end

endmodule
