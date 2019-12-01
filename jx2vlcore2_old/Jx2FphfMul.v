/*
Half-Float Multiply.
*/

module Jx2FphfMul(
	clock,	
	reset,
	regValRs,
	regValRt,
	regValRo,
	regExOp,
	regExOK
	);

/* verilator lint_off UNUSED */

input	clock;
input	reset;

input[15:0]		regValRs;
input[15:0]		regValRt;
output[15:0]	regValRo;
input[1:0]		regExOp;
output[1:0]		regExOK;

reg[15:0]		tRegValRo2;
reg[1:0]		tRegExOK2;

assign	regValRo	= tRegValRo2;
assign	regExOK		= tRegExOK2;

reg[15:0]		tRegValRo;
reg[1:0]		tRegExOK;


reg[1:0]	tRegExOp0;
reg[1:0]	tRegExOp1;

reg[5:0]	tExa;
reg[5:0]	tExb;
reg[5:0]	tExc0;
reg[5:0]	tExc1;
reg			tSga;
reg			tSgb;
reg			tSgc;

reg[10:0]	tFra;
reg[10:0]	tFrb;
reg[21:0]	tFrc;

reg[5:0]	tExd;
reg[9:0]	tFrd;

/* verilator lint_on UNUSED */

always @*
begin
	tRegExOp0	= regExOp;
	tExa		= { 1'b0, regValRs[14:10] };
	tExb		= { 1'b0, regValRt[14:10] };
	tFra		= { 1'b1, regValRs[9:0] };
	tFrb		= { 1'b1, regValRt[9:0] };
	tSga		= regValRs[15];
	tSgb		= regValRt[15];
	
/*
	if(tExc0[5])
	begin
		tRegValRo = tExc0[4] ? 16'h0000 : {tSgc, 15'h7C00};
	end
	else
		if(tFrc[19])
	begin
		tRegValRo = { tSgc, tExc1[4:0], tFrc[18:9] };
	end else begin
		tRegValRo = { tSgc, tExc0[4:0], tFrc[17:8] };
	end
*/

	if(tFrc[21])
	begin
		tExd = tExc1;
		tFrd = tFrc[20:11];
	end else begin
		tExd = tExc0;
		tFrd = tFrc[19:10];
	end

	if(tExd[5])
		tRegValRo = tExd[4] ? 16'h0000 : {tSgc, 15'h7C00};
	else
		tRegValRo = { tSgc, tExd[4:0], tFrd };

	tRegExOK = (tRegExOp1 != 0) ? UMEM_OK_OK : UMEM_OK_READY;
end

always @(posedge clock)
begin
	tRegValRo2	<= tRegValRo;
	tRegExOK2	<= tRegExOK;

	tRegExOp1	<= tRegExOp0;
	tExc0		<= tExa + tExb - 6'h0F;
	tExc1		<= tExa + tExb - 6'h0E;
	tFrc		<= { 11'h0, tFra } * { 11'h0, tFrb };
	tSgc		<= tSga ^ tSgb;
end

endmodule
