module FpuConvS2E(
	regValFRm,
	regValFRn);

input[31:0]		regValFRm;
output[95:0]	regValFRn;

reg[95:0]		tRegValFRn;
reg				tExpIsZero;
reg				tExpIsNaN;

assign	regValFRn = tRegValFRn;

always @*
begin
	tExpIsZero = (regValFRm[30:23]==8'h00);
	tExpIsNaN  = (regValFRm[30:23]==8'hFF);

	tRegValFRn = {
		regValFRm[31:30],
		((!regValFRm[30] && !tExpIsZero) ||
			tExpIsNaN) ? 7'b1111111 : 7'b0000000,
		regValFRm[29:23],
		regValFRm[22: 0],
		57'h0 };
end

endmodule
