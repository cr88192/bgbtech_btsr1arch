/*

Multiply a pair of RGB555 values.

0rrrr-rggg-ggbb-bbbb	//Opaque Pixel
1rrrr-aggg-gabb-bbba	//Alpha Pixel

Alpha blended pixels will use the LSB of each component as an alpha bit.

 */

`include "ExModRgba_Mul4.v"

module ExModRgba_Mul555(
	valRs,	valRt,	valRn
	);

input[15:0]		valRs;
input[15:0]		valRt;
output[15:0]	valRn;

reg[15:0]		tValRn;

assign	valRn = tValRn;

wire[3:0]	tMulR0;
wire[3:0]	tMulG0;
wire[3:0]	tMulB0;
ExModRgba_Mul4	mul_r(valRs[14:11], valRt[14:11], tMulR0);
ExModRgba_Mul4	mul_g(valRs[ 9: 6], valRt[ 9: 6], tMulG0);
ExModRgba_Mul4	mul_g(valRs[ 4: 1], valRt[ 4: 1], tMulB0);

reg[2:0]	tValAs;
reg[2:0]	tValAt;
reg[2:0]	tMulA0;

always @*
begin

	tValAs = { valRs[10], valRs[5], valRs[0] };
	tValAt = { valRt[10], valRt[5], valRt[0] };

	case( {	tValAs, tValAt } )
		6'h00: tMulA0=3'h0;
		6'h01: tMulA0=3'h0;
		6'h02: tMulA0=3'h0;
		6'h03: tMulA0=3'h0;
		6'h04: tMulA0=3'h0;
		6'h05: tMulA0=3'h0;
		6'h06: tMulA0=3'h0;
		6'h07: tMulA0=3'h0;
		6'h08: tMulA0=3'h0;
		6'h09: tMulA0=3'h0;
		6'h0A: tMulA0=3'h0;
		6'h0B: tMulA0=3'h0;
		6'h0C: tMulA0=3'h1;
		6'h0D: tMulA0=3'h1;
		6'h0E: tMulA0=3'h1;
		6'h0F: tMulA0=3'h1;
		6'h10: tMulA0=3'h0;
		6'h11: tMulA0=3'h0;
		6'h12: tMulA0=3'h1;
		6'h13: tMulA0=3'h1;
		6'h14: tMulA0=3'h1;
		6'h15: tMulA0=3'h1;
		6'h16: tMulA0=3'h2;
		6'h17: tMulA0=3'h2;
		6'h18: tMulA0=3'h0;
		6'h19: tMulA0=3'h0;
		6'h1A: tMulA0=3'h1;
		6'h1B: tMulA0=3'h1;
		6'h1C: tMulA0=3'h2;
		6'h1D: tMulA0=3'h2;
		6'h1E: tMulA0=3'h2;
		6'h1F: tMulA0=3'h3;
		6'h20: tMulA0=3'h0;
		6'h21: tMulA0=3'h1;
		6'h22: tMulA0=3'h1;
		6'h23: tMulA0=3'h2;
		6'h24: tMulA0=3'h2;
		6'h25: tMulA0=3'h3;
		6'h26: tMulA0=3'h3;
		6'h27: tMulA0=3'h4;
		6'h28: tMulA0=3'h0;
		6'h29: tMulA0=3'h1;
		6'h2A: tMulA0=3'h1;
		6'h2B: tMulA0=3'h2;
		6'h2C: tMulA0=3'h3;
		6'h2D: tMulA0=3'h3;
		6'h2E: tMulA0=3'h4;
		6'h2F: tMulA0=3'h5;
		6'h30: tMulA0=3'h0;
		6'h31: tMulA0=3'h1;
		6'h32: tMulA0=3'h2;
		6'h33: tMulA0=3'h2;
		6'h34: tMulA0=3'h3;
		6'h35: tMulA0=3'h4;
		6'h36: tMulA0=3'h5;
		6'h37: tMulA0=3'h6;
		6'h38: tMulA0=3'h0;
		6'h39: tMulA0=3'h1;
		6'h3A: tMulA0=3'h2;
		6'h3B: tMulA0=3'h3;
		6'h3C: tMulA0=3'h4;
		6'h3D: tMulA0=3'h5;
		6'h3E: tMulA0=3'h6;
		6'h3F: tMulA0=3'h7;
	endcase

	case( {valRs[15], valRt[15]} )
		2'b00:
			tValRn = { 1'b0,
				tMulR0, tMulR0[3],
				tMulG0, tMulG0[3],
				tMulB0, tMulB0[3] };
		2'b01:
			tValRn = { 1'b1,
				tMulR0, tValAt[2],
				tMulG0, tValAt[1],
				tMulB0, tValAt[0] };
		2'b10:
			tValRn = { 1'b1,
				tMulR0, tValAs[2],
				tMulG0, tValAs[1],
				tMulB0, tValAs[0] };
		2'b11:
			tValRn = { 1'b1,
				tMulR0, tMulA0[2],
				tMulG0, tMulA0[1],
				tMulB0, tMulA0[0] };
	endcase

end

endmodule
