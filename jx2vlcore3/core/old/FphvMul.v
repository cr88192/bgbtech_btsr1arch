/*
Half Precision Multiply.

Multiply two 16-bit Half-Float Numbers.
Intended for pipelined operation.
*/

module FphvMul(
	clock,	
	reset,
	regValRs,
	regValRt,
	regValRn
	);

/* verilator lint_off UNUSED */

input	clock;
input	reset;

input[15:0]		regValRs;
input[15:0]		regValRt;
output[15:0]	regValRn;

reg[31:0]		tRegValRn2;
assign	regValRn	= tRegValRn2;

reg[31:0]		tRegValRn;

reg[5:0]	tExa;
reg[5:0]	tExb;
reg[5:0]	tExc0;
reg[5:0]	tExc1;
reg			tSga;
reg			tSgb;
reg			tSgc;

reg[11:0]	tFra;
reg[11:0]	tFrb;
reg[23:0]	tFrc;

reg[5:0]	tExd;
reg[11:0]	tFrd;

/* verilator lint_on UNUSED */

always @*
begin
	/* A */
	tRegExOp0	= regExOp;
	tExa		= { 1'b0, regValRs[14:10] };
	tExb		= { 1'b0, regValRt[14:10] };
	tFra		= { 2'b01, regValRs[9:0] };
	tFrb		= { 2'b01, regValRt[9:0] };
	tSga		= regValRs[31];
	tSgb		= regValRt[31];
	
	/* B */
	if(tFrc[21])
	begin
		tExd = tExc1;
		tFrd = tFrc[20:11];
	end else begin
		tExd = tExc0;
		tFrd = tFrc[19:10];
	end

	if(tExd[5])
		tRegValRn = tExd[6] ? 16'h0000 : {tSgc, 15'h7C00};
	else
		tRegValRn = { tSgc, tExd[4:0], tFrd};
end

always @(posedge clock)
begin
	tRegValRn2	<= tRegValRn;

	tExc0		<= tExa + tExb - 6'hF;
	tExc1		<= tExa + tExb - 6'hE;
	tFrc		<= { 12'h000, tFra } * { 12'h000, tFrb };
	tSgc		<= tSga ^ tSgb;
end

endmodule
