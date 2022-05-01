/*
  DAA/DAS on Packed BCD Digits
  
  mode=0: DAA (Adjust After ADD)
  mode=1: DAS (Adjust After SUB)
 */

module ExMiscDaa4(inDigit, outDigit, inCarry, outCarry, inMode);
input[3:0]		inDigit;
input			inCarry;
input			inMode;

output[3:0]		outDigit;
output			outCarry;

reg[4:0]		tOut;

assign		outDigit	= tOut[3:0];
assign		outCarry	= tOut[4];

always @*
begin
	case( { inMode, inCarry, inDigit } )
		/* DAA */
		6'b00_0000:		tOut = 5'b0_0000;
		6'b00_0001:		tOut = 5'b0_0001;
		6'b00_0010:		tOut = 5'b0_0010;
		6'b00_0011:		tOut = 5'b0_0011;
		6'b00_0100:		tOut = 5'b0_0100;
		6'b00_0101:		tOut = 5'b0_0101;
		6'b00_0110:		tOut = 5'b0_0110;
		6'b00_0111:		tOut = 5'b0_0111;
		6'b00_1000:		tOut = 5'b0_1000;
		6'b00_1001:		tOut = 5'b0_1001;
		6'b00_1010:		tOut = 5'b1_0000;
		6'b00_1011:		tOut = 5'b1_0001;
		6'b00_1100:		tOut = 5'b1_0010;
		6'b00_1101:		tOut = 5'b1_0011;
		6'b00_1110:		tOut = 5'b1_0100;
		6'b00_1111:		tOut = 5'b1_0101;
		6'b01_0000:		tOut = 5'b0_0001;
		6'b01_0001:		tOut = 5'b0_0010;
		6'b01_0010:		tOut = 5'b0_0011;
		6'b01_0011:		tOut = 5'b0_0100;
		6'b01_0100:		tOut = 5'b0_0101;
		6'b01_0101:		tOut = 5'b0_0110;
		6'b01_0110:		tOut = 5'b0_0111;
		6'b01_0111:		tOut = 5'b0_1000;
		6'b01_1000:		tOut = 5'b0_1001;
		6'b01_1001:		tOut = 5'b1_0000;
		6'b01_1010:		tOut = 5'b1_0001;
		6'b01_1011:		tOut = 5'b1_0010;
		6'b01_1100:		tOut = 5'b1_0011;
		6'b01_1101:		tOut = 5'b1_0100;
		6'b01_1110:		tOut = 5'b1_0101;
		6'b01_1111:		tOut = 5'b1_0110;

		/* DAS */
		6'b10_0000:		tOut = 5'b0_0000;
		6'b10_0001:		tOut = 5'b0_0001;
		6'b10_0010:		tOut = 5'b0_0010;
		6'b10_0011:		tOut = 5'b0_0011;
		6'b10_0100:		tOut = 5'b0_0100;
		6'b10_0101:		tOut = 5'b0_0101;
		6'b10_0110:		tOut = 5'b0_0110;
		6'b10_0111:		tOut = 5'b0_0111;
		6'b10_1000:		tOut = 5'b0_1000;
		6'b10_1001:		tOut = 5'b0_1001;
		6'b10_1010:		tOut = 5'b1_0100;
		6'b10_1011:		tOut = 5'b1_0101;
		6'b10_1100:		tOut = 5'b1_0110;
		6'b10_1101:		tOut = 5'b1_0111;
		6'b10_1110:		tOut = 5'b1_1000;
		6'b10_1111:		tOut = 5'b1_1001;

		6'b11_0000:		tOut = 5'b1_1001;
		6'b11_0001:		tOut = 5'b0_0000;
		6'b11_0010:		tOut = 5'b0_0001;
		6'b11_0011:		tOut = 5'b0_0010;
		6'b11_0100:		tOut = 5'b0_0011;
		6'b11_0101:		tOut = 5'b0_0100;
		6'b11_0110:		tOut = 5'b0_0101;
		6'b11_0111:		tOut = 5'b0_0110;
		6'b11_1000:		tOut = 5'b0_0111;
		6'b11_1001:		tOut = 5'b0_1000;
		6'b11_1010:		tOut = 5'b0_1001;
		6'b11_1011:		tOut = 5'b1_0100;
		6'b11_1100:		tOut = 5'b1_0101;
		6'b11_1101:		tOut = 5'b1_0110;
		6'b11_1110:		tOut = 5'b1_0111;
		6'b11_1111:		tOut = 5'b1_1000;
	endcase
end

endmodule