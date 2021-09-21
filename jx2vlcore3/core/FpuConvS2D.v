`ifndef HAS_FPUCONVS2D
`define HAS_FPUCONVS2D

module FpuConvS2D(
	regValFRm,
	regValFRn);

input[31:0]		regValFRm;
output[63:0]	regValFRn;

reg[63:0]		tRegValFRn;
reg				tExpIsZero;
reg				tExpIsNaN;

assign	regValFRn = tRegValFRn;

always @*
begin
	tExpIsZero = (regValFRm[30:23]==8'h00);
	tExpIsNaN  = (regValFRm[30:23]==8'hFF);

	tRegValFRn = {
		regValFRm[31:30],
//		regValFRm[29] ? 4'b1111 : 4'b0000,
//		(!regValFRm[30] && !tExpIsZero) ? 4'b1111 : 4'b0000,
		((!regValFRm[30] && !tExpIsZero) ||
			tExpIsNaN) ? 3'b111 : 3'b000,
//		regValFRm[28:23],
		regValFRm[29:23],
		regValFRm[22: 0],
		29'h0 };
end

endmodule

`endif
