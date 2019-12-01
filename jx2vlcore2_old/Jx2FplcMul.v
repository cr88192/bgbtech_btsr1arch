/*
'Low Cost' Multiply.

Will nominally use the single-precision format, but will operate at reduced precision.
*/

module Jx2FplcMul(
	clock,		reset,
	regValRs,
	regValRt,
	regValRo,
	regExOp,
	regExOK
	);

input	clock;
input	reset;

input[31:0]		regValRs;
input[31:0]		regValRt;
output[31:0]	regValRo;
input[1:0]		regExOp;
output[1:0]		regExOK;

reg[31:0]		tRegValRo;
reg[1:0]		tRegExOK;

assign	regValRo	= tRegValRo;
assign	regExOK		= tRegExOK;

reg[8:0]	tExa;
reg[8:0]	tExb;
reg[8:0]	tExc0;
reg[8:0]	tExc1;
reg			tSga;
reg			tSgb;
reg			tSgc;

reg[17:0]	tFra;
reg[17:0]	tFrb;
reg[35:0]	tFrc;

always @*
begin
	tExa	= { 1'b0, regValRs[30:23] };
	tExb	= { 1'b0, regValRt[30:23] };
	tFra	= { 1'b1, regValRs[22:5] };
	tFrb	= { 1'b1, regValRt[22:5] };
	tSga	= regValRs[31];
	tSgb	= regValRs[31];
	
	if(tExc0[8])
	begin
		tRegValRo = 0;
	end
	else
		if(tFrc[35])
	begin
		tRegValRo = { tSgc, tExc1[7:0], tFrc[34:11] };
	end
	else
	begin
		tRegValRo = { tSgc, tExc0[7:0], tFrc[33:10] };
	end
end

always @(posedge clock)
begin
	tExc0	<= tExa0 + tExb0 - 9'h7F;
	tExc1	<= tExa0 + tExb0 - 9'h7E;
	tFrc	<= { 18'h0, tFra } * { 18'h0, tFrb };
	tSgc	<= tSga ^ tSgb;
end

endmodule