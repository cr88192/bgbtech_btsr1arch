/*
 * Low Accuracy FP8 Multiply
 * Approximate FP8 as a Log-Scale Value
 *
 * Multiply is easier, A+B-Bias
 * Main complexity is value going out of range.
 */

`ifndef HAS_FP8LAMUL
`define HAS_FP8LAMUL

module Fp8LaMul(valA, valB, valC);
input[7:0]		valA;
input[7:0]		valB;
output[7:0]		valC;

reg[7:0]		tValC;
assign		valC = tValC;

reg[8:0]		tValC0;

always @*
begin

	tValC0 =
		{ 2'b0, valA[6:0] } +
		{ 2'b0, valB[6:0] } +
		9'h1C8;
	case(tValC0[8:7])
		/* Normal Range */
		2'b00: tValC = { valA[7]^valB[7], tValC0[6:0] };
		/* Overflow */
		2'b01: tValC = { valA[7]^valB[7], 7'h7F };
		/* Underflow to 0 */
		2'b10, 2'b11: tValC = 8'h00;
	endcase
end

endmodule

`endif
