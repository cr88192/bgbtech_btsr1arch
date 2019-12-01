module Jx2ConvFpuH2D(
	regValFRm,
	regValFRn);

input[15:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;
reg				tExpIsZero;

assign	regValFRn = tRegValFRn;

always @*
begin
	tExpIsZero = (regValFRm[14:10]==0);

	tRegValFRn = {
		regValFRm[15:14],
//		regValFRm[13] ? 7'b1111111 : 7'b0000000,
		(!regValFRm[14] && !tExpIsZero) ? 7'b1111111 : 7'b0000000,
		regValFRm[12:10],
		regValFRm[9:0],
		42'h0 };
end

endmodule
