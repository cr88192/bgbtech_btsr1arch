/*
Multiply two 3 bit numbers using a 3-bit radix.
 */

`ifndef HAS_ExLuMul3Ra8
`define HAS_ExLuMul3Ra8

module ExLuMul3Ra8(valA, valB, valC);

input[2:0]		valA;
input[2:0]		valB;
output[5:0]		valC;

reg[5:0]		tValC;
assign		valC = tValC;

always @*
begin
	case({valA, valB})
		6'b000_000: tValC=6'b000000;
		6'b000_001: tValC=6'b000000;
		6'b000_010: tValC=6'b000000;
		6'b000_011: tValC=6'b000000;
		6'b000_100: tValC=6'b000000;
		6'b000_101: tValC=6'b000000;
		6'b000_110: tValC=6'b000000;
		6'b000_111: tValC=6'b000000;

		6'b001_000: tValC=6'b000000;
		6'b001_001: tValC=6'b000001;
		6'b001_010: tValC=6'b000010;
		6'b001_011: tValC=6'b000011;
		6'b001_100: tValC=6'b000100;
		6'b001_101: tValC=6'b000101;
		6'b001_110: tValC=6'b000110;
		6'b001_111: tValC=6'b000111;

		6'b010_000: tValC=6'b000000;
		6'b010_001: tValC=6'b000010;
		6'b010_010: tValC=6'b000100;
		6'b010_011: tValC=6'b000110;
		6'b010_100: tValC=6'b001000;
		6'b010_101: tValC=6'b001010;
		6'b010_110: tValC=6'b001100;
		6'b010_111: tValC=6'b001110;

		6'b011_000: tValC=6'b000000;
		6'b011_001: tValC=6'b000011;
		6'b011_010: tValC=6'b000110;
		6'b011_011: tValC=6'b001001;
		6'b011_100: tValC=6'b001100;
		6'b011_101: tValC=6'b001111;
		6'b011_110: tValC=6'b010010;
		6'b011_111: tValC=6'b010101;

		6'b100_000: tValC=6'b000000;
		6'b100_001: tValC=6'b000100;
		6'b100_010: tValC=6'b001000;
		6'b100_011: tValC=6'b001100;
		6'b100_100: tValC=6'b010000;
		6'b100_101: tValC=6'b010100;
		6'b100_110: tValC=6'b011000;
		6'b100_111: tValC=6'b011100;

		6'b101_000: tValC=6'b000000;
		6'b101_001: tValC=6'b000101;
		6'b101_010: tValC=6'b001010;
		6'b101_011: tValC=6'b001111;
		6'b101_100: tValC=6'b010100;
		6'b101_101: tValC=6'b011001;
		6'b101_110: tValC=6'b011110;
		6'b101_111: tValC=6'b100011;

		6'b110_000: tValC=6'b000000;
		6'b110_001: tValC=6'b000110;
		6'b110_010: tValC=6'b001100;
		6'b110_011: tValC=6'b010010;
		6'b110_100: tValC=6'b011000;
		6'b110_101: tValC=6'b011110;
		6'b110_110: tValC=6'b100100;
		6'b110_111: tValC=6'b101010;

		6'b111_000: tValC=6'b000000;
		6'b111_001: tValC=6'b000111;
		6'b111_010: tValC=6'b001110;
		6'b111_011: tValC=6'b010101;
		6'b111_100: tValC=6'b011100;
		6'b111_101: tValC=6'b100011;
		6'b111_110: tValC=6'b101010;
		6'b111_111: tValC=6'b110001;
	endcase
end

endmodule

`endif
