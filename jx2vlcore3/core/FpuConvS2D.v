module FpuConvS2D(
	regValFRm,
	regValFRn);

input[31:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;
reg				tExpIsZero;

assign	regValFRn = tRegValFRn;

always @*
begin
	tExpIsZero = (regValFRm[30:23]==0);
	tRegValFRn = {
		regValFRm[31:30],
//		regValFRm[29] ? 4'b1111 : 4'b0000,
		(!regValFRm[30] && !tExpIsZero) ? 4'b1111 : 4'b0000,
		regValFRm[28:23],
		regValFRm[22: 0],
		29'h0 };
end

endmodule
