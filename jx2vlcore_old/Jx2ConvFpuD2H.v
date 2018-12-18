module Jx2ConvFpuD2H(
	regValFRm,
	regValFRn);

input[63:0]		regValFRm;
output[15:0]	regValFRn;

reg[15:0]		tRegValFRn;

assign	regValFRn = tRegValFRn;

always @*
begin
	casez(regValFRm[62:56])
	7'b0111111:
		tRegValFRn	= {
			regValFRm[63:62],
			regValFRm[55:52],
			regValFRm[51:42] };
	7'b1000000:
		tRegValFRn	= { 
			regValFRm[63:62],
			regValFRm[55:52],
			regValFRm[51:42] };
	default:
		if(regValFRm[62])
			tRegValFRn	= { regValFRm[63], 15'h7C00 };
		else
			tRegValFRn = 0;
	endcase
end

endmodule