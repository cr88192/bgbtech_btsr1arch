/*
 * Low Accuracy FP8 Adder
 * Approximate FP8 as a Log-Scale Value
 *
 * ADD/SUB is the harder case with Log-Scale.
 *
 * General idea is to find the difference of the larger and smaller value.
 * This is used to lookup an offset to add to the larger value to get the result.
 */

`ifndef HAS_FP8LAADD
`define HAS_FP8LAADD

module Fp8LaAdd(valA, valB, valC);
input[7:0]		valA;
input[7:0]		valB;
output[7:0]		valC;

reg[7:0]		tValX;
reg[7:0]		tValY;
reg[7:0]		tValD;
reg[7:0]		tValU;
reg				tIsSub;

reg				tSgC;
reg[7:0]		tValC;
assign		valC = tValC;

reg[8:0]		tValC0;

always @*
begin
	tIsSub = valA[7]^valB[7];

	if(valA[6:0]>valB[6:0])
	begin
		tValX	= {1'b0, valA[6:0] };
		tValY	= {1'b0, valB[6:0] };
		tSgC	= valA[7];
	end
	else
	begin
		tValX	= {1'b0, valB[6:0] };
		tValY	= {1'b0, valA[6:0] };
		tSgC	= valB[7];
	end
	
	tValD = tValX - tValY;
	case( { tIsSub, tValD[4:0] } )
		6'h00: tValU = 8'h08;		6'h01: tValU = 8'h08;
		6'h02: tValU = 8'h07;		6'h03: tValU = 8'h07;
		6'h04: tValU = 8'h06;		6'h05: tValU = 8'h06;
		6'h06: tValU = 8'h05;		6'h07: tValU = 8'h05;
		6'h08: tValU = 8'h04;		6'h09: tValU = 8'h04;
		6'h0A: tValU = 8'h04;		6'h0B: tValU = 8'h04;
		6'h0C: tValU = 8'h03;		6'h0D: tValU = 8'h03;
		6'h0E: tValU = 8'h03;		6'h0F: tValU = 8'h03;
		6'h10: tValU = 8'h02;		6'h11: tValU = 8'h02;
		6'h12: tValU = 8'h02;		6'h13: tValU = 8'h02;
		6'h14: tValU = 8'h02;		6'h15: tValU = 8'h02;
		6'h16: tValU = 8'h02;		6'h17: tValU = 8'h02;
		6'h18: tValU = 8'h01;		6'h19: tValU = 8'h01;
		6'h1A: tValU = 8'h01;		6'h1B: tValU = 8'h01;
		6'h1C: tValU = 8'h01;		6'h1D: tValU = 8'h01;
		6'h1E: tValU = 8'h01;		6'h1F: tValU = 8'h01;
		6'h20: tValU = 8'h80;		6'h21: tValU = 8'hE3;
		6'h22: tValU = 8'hEA;		6'h23: tValU = 8'hEE;
		6'h24: tValU = 8'hF1;		6'h25: tValU = 8'hF3;
		6'h26: tValU = 8'hF5;		6'h27: tValU = 8'hF7;
		6'h28: tValU = 8'hF8;		6'h29: tValU = 8'hF9;
		6'h2A: tValU = 8'hFA;		6'h2B: tValU = 8'hFA;
		6'h2C: tValU = 8'hFB;		6'h2D: tValU = 8'hFB;
		6'h2E: tValU = 8'hFC;		6'h2F: tValU = 8'hFC;
		6'h30: tValU = 8'hFC;		6'h31: tValU = 8'hFD;
		6'h32: tValU = 8'hFD;		6'h33: tValU = 8'hFD;
		6'h34: tValU = 8'hFD;		6'h35: tValU = 8'hFE;
		6'h36: tValU = 8'hFE;		6'h37: tValU = 8'hFE;
		6'h38: tValU = 8'hFE;		6'h39: tValU = 8'hFE;
		6'h3A: tValU = 8'hFE;		6'h3B: tValU = 8'hFF;
		6'h3C: tValU = 8'hFF;		6'h3D: tValU = 8'hFF;
		6'h3E: tValU = 8'hFF;		6'h3F: tValU = 8'hFF;
	endcase

	if(tValD[7:5])
		tValU = 8'h00;

	tValC0 =
		{ 2'b0, valA[6:0] } +
		{ tValU[7], tValU[7:0] };

	case(tValC0[8:7])
		/* Normal Range */
		2'b00: tValC = { tSgC, tValC0[6:0] };
		/* Overflow */
		2'b01: tValC = { tSgC, 7'h7F };
		/* Underflow to 0 */
		2'b10, 2'b11: tValC = 8'h00;
	endcase
end

endmodule

`endif
