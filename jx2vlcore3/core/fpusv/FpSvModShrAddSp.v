module FpSvModShrAddSp(
	fraS,	fraT,
	shr,	carry,
	fraN
	);

input[27:0]		fraS;
input[27:0]		fraT;
input[8 :0]		shr;
input			carry;
output[27:0]	fraN;

reg[27:0]		tFraN;
assign		fraN = tFraN;

reg[27:0]		tFraSx;
reg[27:0]		tFraShr16;
reg[27:0]		tFraShr8;
reg[27:0]		tFraShr4;
reg[27:0]		tFraShr2;
reg[27:0]		tFraShr1;
reg[27:0]		tFraShr;

always @*
begin
	tFraSx		= fraT[27] ? 28'hFFF_FFFF : 28'h000_0000;
	tFraShr16	= shr[4] ? { tFraSx[15:0], fraT     [27:16] } : fraT;
	tFraShr8	= shr[3] ? { tFraSx[ 7:0], tFraShr16[27: 8] } : tFraShr16;
	tFraShr4	= shr[2] ? { tFraSx[ 3:0], tFraShr8 [27: 4] } : tFraShr8;
	tFraShr2	= shr[1] ? { tFraSx[ 1:0], tFraShr4 [27: 2] } : tFraShr4;
	tFraShr1	= shr[0] ? { tFraSx[   0], tFraShr2 [27: 1] } : tFraShr2;
	tFraShr		= (shr[8:5] != 0) ? 28'h000_0000 : tFraShr1;
	
	tFraN		= fraS + tFraShr + { 27'h000_0000, carry };
end

endmodule
