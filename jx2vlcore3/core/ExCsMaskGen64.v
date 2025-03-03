/*
Generate a Bit-Mask given a count.
 */

`ifndef HAS_CSMASKGEN64
`define HAS_CSMASKGEN64

module ExCsMaskGen64(valIn, valOut);

input [6:0]		valIn;
output[63:0]	valOut;

reg[7:0]	tMaskLo8;
reg[15:0]	tMaskSelB;

reg[7:0]	tSelA0;
reg[7:0]	tSelA1;
reg[7:0]	tSelA2;
reg[7:0]	tSelA3;
reg[7:0]	tSelA4;
reg[7:0]	tSelA5;
reg[7:0]	tSelA6;
reg[7:0]	tSelA7;

reg[63:0]	tValRn;

assign	valOut = tValRn;

always @*
begin
	case(valIn[2:0])
		3'b000: tMaskLo8 = 8'b00000000;
		3'b001: tMaskLo8 = 8'b00000001;
		3'b010: tMaskLo8 = 8'b00000011;
		3'b011: tMaskLo8 = 8'b00000111;
		3'b100: tMaskLo8 = 8'b00001111;
		3'b101: tMaskLo8 = 8'b00011111;
		3'b110: tMaskLo8 = 8'b00111111;
		3'b111: tMaskLo8 = 8'b01111111;
	endcase
	case(valIn[6:3])
		4'b0000: tMaskSelB = 16'b0000_0000_0000_0010;
		4'b0001: tMaskSelB = 16'b0000_0000_0000_1001;
		4'b0010: tMaskSelB = 16'b0000_0000_0010_0101;
		4'b0011: tMaskSelB = 16'b0000_0000_1001_0101;
		4'b0100: tMaskSelB = 16'b0000_0010_0101_0101;
		4'b0101: tMaskSelB = 16'b0000_1001_0101_0101;
		4'b0110: tMaskSelB = 16'b0010_0101_0101_0101;
		4'b0111: tMaskSelB = 16'b1001_0101_0101_0101;

		4'b1000: tMaskSelB = 16'b0101_0101_0101_0111;
		4'b1001: tMaskSelB = 16'b0101_0101_0101_1100;
		4'b1010: tMaskSelB = 16'b0101_0101_0111_0000;
		4'b1011: tMaskSelB = 16'b0101_0101_1100_0000;
		4'b1100: tMaskSelB = 16'b0101_0111_0000_0000;
		4'b1101: tMaskSelB = 16'b0101_1100_0000_0000;
		4'b1110: tMaskSelB = 16'b0111_0000_0000_0000;
		4'b1111: tMaskSelB = 16'b1100_0000_0000_0000;
	endcase

	case(tMaskSelB[1:0])
		2'b00: tSelA0 = 8'h00;
		2'b01: tSelA0 = 8'hFF;
		2'b10: tSelA0 = tMaskLo8;
		2'b11: tSelA0 = ~tMaskLo8;
	endcase
	case(tMaskSelB[3:2])
		2'b00: tSelA1 = 8'h00;
		2'b01: tSelA1 = 8'hFF;
		2'b10: tSelA1 = tMaskLo8;
		2'b11: tSelA1 = ~tMaskLo8;
	endcase

	case(tMaskSelB[5:4])
		2'b00: tSelA2 = 8'h00;
		2'b01: tSelA2 = 8'hFF;
		2'b10: tSelA2 = tMaskLo8;
		2'b11: tSelA2 = ~tMaskLo8;
	endcase
	case(tMaskSelB[7:6])
		2'b00: tSelA3 = 8'h00;
		2'b01: tSelA3 = 8'hFF;
		2'b10: tSelA3 = tMaskLo8;
		2'b11: tSelA3 = ~tMaskLo8;
	endcase

	case(tMaskSelB[9:8])
		2'b00: tSelA4 = 8'h00;
		2'b01: tSelA4 = 8'hFF;
		2'b10: tSelA4 = tMaskLo8;
		2'b11: tSelA4 = ~tMaskLo8;
	endcase
	case(tMaskSelB[11:10])
		2'b00: tSelA5 = 8'h00;
		2'b01: tSelA5 = 8'hFF;
		2'b10: tSelA5 = tMaskLo8;
		2'b11: tSelA5 = ~tMaskLo8;
	endcase

	case(tMaskSelB[13:12])
		2'b00: tSelA6 = 8'h00;
		2'b01: tSelA6 = 8'hFF;
		2'b10: tSelA6 = tMaskLo8;
		2'b11: tSelA6 = ~tMaskLo8;
	endcase
	case(tMaskSelB[15:14])
		2'b00: tSelA7 = 8'h00;
		2'b01: tSelA7 = 8'hFF;
		2'b10: tSelA7 = tMaskLo8;
		2'b11: tSelA7 = ~tMaskLo8;
	endcase
	
	tValRn = {
		tSelA7, tSelA6, tSelA5, tSelA4,
		tSelA3, tSelA2, tSelA1, tSelA0
	};
end

endmodule

`endif
