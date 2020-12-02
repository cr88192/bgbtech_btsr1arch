/*

Operations on pairs of RGB555 values.

* 0: Average: Average RGB555 pixels.
* 1: Modulate: Pairwise multiply of RGBA555 pixels.

*/

`include "ExModRgba_Mul555.v"

module ExModRgba(
	clock,		reset,
	opCmd,
	regValRs,	regValRt,
	regValRn
	);

input			clock;
input			reset;
input[7:0]		opCmd;

input[63:0]		regValRs;
input[63:0]		regValRt;
output[63:0]	regValRn;

reg[63:0]		tRegValRn;
assign		regValRn = tRegValRn;

wire[63:0]		tRegMulRn;

ExModRgba_Mul555	mul_rgb0(
	regValRs[63:48], regValRt[63:48], tRegMulRn[63:48]);
ExModRgba_Mul555	mul_rgb1(
	regValRs[47:32], regValRt[47:32], tRegMulRn[47:32]);
ExModRgba_Mul555	mul_rgb2(
	regValRs[31:16], regValRt[31:16], tRegMulRn[31:16]);
ExModRgba_Mul555	mul_rgb3(
	regValRs[15: 0], regValRt[15: 0], tRegMulRn[15: 0]);

always @*
begin

	tRegValRn = UV64_00;
	
	case(opCmd[3:0])
	
		4'h1: tRegValRn = tRegMulRn;
	
		default: begin
		end
	
	endcase

end

endmodule
