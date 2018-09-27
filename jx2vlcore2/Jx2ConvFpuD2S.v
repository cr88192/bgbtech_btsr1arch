module Jx2ConvFpuD2S(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[31:0]	regValFRn;

reg[31:0]		tRegValFRn;

assign	regValFRn = tRegValFRn;

always @*
begin
	casez(regValFRm[62:59])
	4'b0111:
		tRegValFRn	= {
			regValFRm[63:62],
			regValFRm[58:52],
			regValFRm[51:29]+(regValFRm[28]?23'h1:23'h0) };
	4'b1000:
		tRegValFRn	= { 
			regValFRm[63:62],
			regValFRm[58:52],
			regValFRm[51:29]+(regValFRm[28]?23'h1:23'h0) };
	default:
		if(regValFRm[62])
			tRegValFRn	= { regValFRm[63], 31'h7F800000 };
		else
			tRegValFRn = 0;
	endcase
end

endmodule