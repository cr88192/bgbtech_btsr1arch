/*
Carry Save for 3x 2-bit inputs, producing 2x 2-bit outputs.
 */

module ExCsa3x2(valA, valB, valC, valP, valQ);
input[1:0]		valA;
input[1:0]		valB;
input[1:0]		valC;

output[1:0]		valP;
output[1:0]		valQ;

reg[3:0]		tValPQ;
assign		valP = tValPQ[1:0];
assign		valQ = tValPQ[3:2];

always @*
begin
	case({valA, valB, valC})
		6'b00_00_00:	tValPQ = 4'b0000;
		6'b00_00_01:	tValPQ = 4'b0001;
		6'b00_00_10:	tValPQ = 4'b0010;
		6'b00_00_11:	tValPQ = 4'b0011;

		6'b00_01_00:	tValPQ = 4'b0001;
		6'b00_01_01:	tValPQ = 4'b0100;
		6'b00_01_10:	tValPQ = 4'b0011;
		6'b00_01_11:	tValPQ = 4'b0110;

		6'b00_10_00:	tValPQ = 4'b0010;
		6'b00_10_01:	tValPQ = 4'b0011;
		6'b00_10_10:	tValPQ = 4'b1000;
		6'b00_10_11:	tValPQ = 4'b1001;

		6'b00_11_00:	tValPQ = 4'b0011;
		6'b00_11_01:	tValPQ = 4'b0110;
		6'b00_11_10:	tValPQ = 4'b1001;
		6'b00_11_11:	tValPQ = 4'b1100;


		6'b01_00_00:	tValPQ = 4'b0001;
		6'b01_00_01:	tValPQ = 4'b0100;
		6'b01_00_10:	tValPQ = 4'b0011;
		6'b01_00_11:	tValPQ = 4'b0110;

		6'b01_01_00:	tValPQ = 4'b0100;
		6'b01_01_01:	tValPQ = 4'b0101;
		6'b01_01_10:	tValPQ = 4'b0110;
		6'b01_01_11:	tValPQ = 4'b0111;

		6'b01_10_00:	tValPQ = 4'b0011;
		6'b01_10_01:	tValPQ = 4'b0110;
		6'b01_10_10:	tValPQ = 4'b1001;
		6'b01_10_11:	tValPQ = 4'b1100;

		6'b01_11_00:	tValPQ = 4'b0110;
		6'b01_11_01:	tValPQ = 4'b0111;
		6'b01_11_10:	tValPQ = 4'b1100;
		6'b01_11_11:	tValPQ = 4'b1101;


		6'b10_00_00:	tValPQ = 4'b0010;
		6'b10_00_01:	tValPQ = 4'b0011;
		6'b10_00_10:	tValPQ = 4'b1000;
		6'b10_00_11:	tValPQ = 4'b1001;

		6'b10_01_00:	tValPQ = 4'b0011;
		6'b10_01_01:	tValPQ = 4'b0110;
		6'b10_01_10:	tValPQ = 4'b1001;
		6'b10_01_11:	tValPQ = 4'b1100;

		6'b10_10_00:	tValPQ = 4'b1000;
		6'b10_10_01:	tValPQ = 4'b1001;
		6'b10_10_10:	tValPQ = 4'b1010;
		6'b10_10_11:	tValPQ = 4'b1011;

		6'b10_11_00:	tValPQ = 4'b1001;
		6'b10_11_01:	tValPQ = 4'b1100;
		6'b10_11_10:	tValPQ = 4'b1011;
		6'b10_11_11:	tValPQ = 4'b1110;


		6'b11_00_00:	tValPQ = 4'b0011;
		6'b11_00_01:	tValPQ = 4'b0110;
		6'b11_00_10:	tValPQ = 4'b1001;
		6'b11_00_11:	tValPQ = 4'b1100;

		6'b11_01_00:	tValPQ = 4'b0110;
		6'b11_01_01:	tValPQ = 4'b0111;
		6'b11_01_10:	tValPQ = 4'b1100;
		6'b11_01_11:	tValPQ = 4'b1101;

		6'b11_10_00:	tValPQ = 4'b1001;
		6'b11_10_01:	tValPQ = 4'b1100;
		6'b11_10_10:	tValPQ = 4'b1011;
		6'b11_10_11:	tValPQ = 4'b1110;

		6'b11_11_00:	tValPQ = 4'b1100;
		6'b11_11_01:	tValPQ = 4'b1101;
		6'b11_11_10:	tValPQ = 4'b1110;
		6'b11_11_11:	tValPQ = 4'b1111;
	endcase
end

endmodule
