module Jx2FpuSqrtA(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;

assign	regValFRn = tRegValFRn;

reg			tSgnB1;
reg[10:0]	tExpB1;
reg[51:0]	tFraB1;
reg[51:0]	tFraB1B;
reg[51:0]	tFraB1C;

reg[31:0]	tFraB1_Adj1P;
reg[31:0]	tFraB1_Adj2P;

reg[51:0]	tFraB1_Adj1;
reg[51:0]	tFraB1_Adj2;

always @*
begin
//	tSgnB1	= regValFRm[63];
//	tExpB1	= 2045-regValFRm[62:52];
//	tFraB1	= ~regValFRm[51:0];

	tRegValFRn = { regValFRm[63],
		(regValFRm[62:0]>>1)+63'h1FF80000_00000000 };
end

endmodule
