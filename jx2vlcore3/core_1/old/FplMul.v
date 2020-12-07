/*
Low Precision Multiply.
*/

module FplMul(
	clock,	
	reset,
	regValRs,
	regValRt,
	regValRn,
	regExOp,
	regExOK
	);

/* verilator lint_off UNUSED */

input	clock;
input	reset;

input[31:0]		regValRs;
input[31:0]		regValRt;
output[31:0]	regValRn;
input[1:0]		regExOp;
output[1:0]		regExOK;

reg[31:0]		tRegValRn2;
reg[1:0]		tRegExOK2;

assign	regValRn	= tRegValRn2;
assign	regExOK		= tRegExOK2;

reg[31:0]		tRegValRn;
reg[1:0]		tRegExOK;


reg[1:0]	tRegExOp0;
reg[1:0]	tRegExOp1;

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

reg[8:0]	tExd;
reg[17:0]	tFrd;

/* verilator lint_on UNUSED */

always @*
begin
	tRegExOp0	= regExOp;
	tExa		= { 1'b0, regValRs[30:23] };
	tExb		= { 1'b0, regValRt[30:23] };
	tFra		= { 1'b1, regValRs[22:6] };
	tFrb		= { 1'b1, regValRt[22:6] };
	tSga		= regValRs[31];
	tSgb		= regValRt[31];
	
	if(tFrc[35])
	begin
		tExd = tExc1;
		tFrd = tFrc[34:17];
	end else begin
		tExd = tExc0;
		tFrd = tFrc[33:16];
	end

	if(tExd[8])
		tRegValRn = tExd[7] ? 32'h00000000 : {tSgc, 31'h7F800000};
	else
		tRegValRn = { tSgc, tExd[7:0], tFrd, 5'h0 };

	tRegExOK = (tRegExOp1 != 0) ? UMEM_OK_OK : UMEM_OK_READY;
end

always @(posedge clock)
begin
	tRegValRn2	<= tRegValRn;
	tRegExOK2	<= tRegExOK;

	tRegExOp1	<= tRegExOp0;
	tExc0		<= tExa + tExb - 9'h7F;
	tExc1		<= tExa + tExb - 9'h7E;
	tFrc		<= { 18'h0, tFra } * { 18'h0, tFrb };
	tSgc		<= tSga ^ tSgb;
end

endmodule
