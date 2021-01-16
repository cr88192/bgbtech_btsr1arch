`ifndef HAS_CSSHR64F
`define HAS_CSSHR64F

module ExCsShr64F(valIn, valOut, valShr);

input [63:0]	valIn;
output[63:0]	valOut;
input [ 7:0]	valShr;

reg[63:0]		tVal0;
reg[63:0]		tVal1;
reg[63:0]		tVal2;
reg[63:0]		tVal3;
reg[63:0]		tVal4;
reg[63:0]		tVal5;
reg[63:0]		tVal6;
reg[63:0]		tVal7;

assign			valOut = tVal7;

always @*
begin

	tVal0 = (valShr[7:6]!=0) ? 0 : valIn;
	tVal1 = tVal0;
	tVal2 = valShr[5] ? { 32'h0, tVal1[63:32] } : tVal1;
	tVal3 = valShr[4] ? { 16'h0, tVal2[63:16] } : tVal2;
	tVal4 = valShr[3] ? {  8'h0, tVal3[63: 8] } : tVal3;
	tVal5 = valShr[2] ? {  4'h0, tVal4[63: 4] } : tVal4;
	tVal6 = valShr[1] ? {  2'h0, tVal5[63: 2] } : tVal5;
	tVal7 = valShr[0] ? {  1'h0, tVal6[63: 1] } : tVal6;

end

endmodule

`endif
