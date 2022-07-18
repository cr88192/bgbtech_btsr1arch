`ifndef HAS_FPUMULSF
`define HAS_FPUMULSF

`include "CoreDefs.v"

module FpuMulSF(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRs,
	regValRt,
	regValRo,
	regExOp,
	regRMode
	);

input	clock;
input	reset;
input	exHold;

input[31:0]		regValRs;
input[31:0]		regValRt;
output[31:0]	regValRo;
input[3:0]		regExOp;
input[7:0]		regRMode;

reg[31:0]		tRegValRo2;
reg[1:0]		tRegExOK2;

assign	regValRo	= tRegValRo2;

reg[31:0]		tRegValRo;

reg			tFmtHalf;
reg			tSgnS;
reg			tSgnT;
reg[8:0]	tExpS;
reg[8:0]	tExpT;
reg[17:0]	tFraS;
reg[17:0]	tFraT;

reg			tSgN;
reg[8:0]	tExpN;
reg[8:0]	tExpNp1;
reg[35:0]	tFraN0;
reg[17:0]	tFraN;

reg			tSgN2;
reg[8:0]	tExpN2;
reg[8:0]	tExpN2p1;
reg[8:0]	tExpN2b;
reg[17:0]	tFraN2;
reg[17:0]	tFraN2b;

reg			tExpIsZero0;
reg			tExpIsZero1;

reg			tExpIsZero;

always @*
begin
	tFmtHalf = 0;

	if(tFmtHalf)
	begin
		tSgnS	= regValRs[15];
		tSgnT	= regValRt[15];
		tExpS	= { 1'b0, regValRs[14] ? 4'h8 : 4'h7, regValRs[13:10] };
		tExpT	= { 1'b0, regValRt[14] ? 4'h8 : 4'h7, regValRt[13:10] };
		tFraS	= { 2'b01, regValRs[9:0], 6'h0 };
		tFraT	= { 2'b01, regValRt[9:0], 6'h0 };
	end
	else
	begin
		tSgnS	= regValRs[31];
		tSgnT	= regValRt[31];
		tExpS	= { 1'b0, regValRs[30:23] };
		tExpT	= { 1'b0, regValRt[30:23] };
		tFraS	= { 2'b01, regValRs[22:7] };
		tFraT	= { 2'b01, regValRt[22:7] };
	end
	
	tExpIsZero0	= 0;
	if(tExpS == 0)
		tExpIsZero0	= 1;
	if(tExpT == 0)
		tExpIsZero0	= 1;
	
	tSgN	= tSgnS ^ tSgnT;
	tExpN	= tExpS + tExpT - 127;
	tExpNp1	= tExpN + 1;
	
	tFraN0	= { 18'h0, tFraS} * { 18'h0, tFraT};
	tFraN	= tFraN0[33:16];
	
	/* Clock Edge */

//	tExpIsZero = 0;
	tExpIsZero = tExpIsZero1;

	if((tExpN2[8:7]==2'b11) || (tExpN2[7:0]==0))
//	if((tExpN2[8]==1'b1) || (tExpN2[7:0]==0))
		tExpIsZero = 1;

	if(tFraN2[17])
	begin
		tExpN2b = tExpN2p1;
		tFraN2b = { 1'b0, tFraN2[17:1] };
	end
	else
	begin
		tExpN2b = tExpN2;
		tFraN2b = tFraN2[17:0];
	end
	
	tRegValRo = { tSgN2, tExpN2b[7:0], tFraN2b[15:0], 7'b0 };

	if(tExpIsZero)
	begin
		tRegValRo = 0;
	end

end

always @(posedge clock)
begin
	if(!exHold)
	begin
		tSgN2		<= tSgN;
		tExpN2		<= tExpN;
		tFraN2		<= tFraN;
		tExpN2p1	<= tExpNp1;
		tExpIsZero1	<= tExpIsZero0;
		
		tRegValRo2	<= tRegValRo;
	end
end

endmodule

`endif
