`ifndef HAS_CSSHR90F
`define HAS_CSSHR90F

module ExCsShr90F(valIn, valOut, valShr);

input [89:0]	valIn;
output[89:0]	valOut;
input [ 7:0]	valShr;

reg[89:0]		tVal0;
reg[89:0]		tVal1;
reg[89:0]		tVal2;
reg[89:0]		tVal3;
reg[89:0]		tVal4;
reg[89:0]		tVal5;
reg[89:0]		tVal6;
reg[89:0]		tVal7;

assign			valOut = tVal7;

always @*
begin

	tVal0 = valShr[7] ? 0 : valIn;
	tVal1 = valShr[6] ? { 64'h0, tVal0[89:64] } : tVal0;
	tVal2 = valShr[5] ? { 32'h0, tVal1[89:32] } : tVal1;
	tVal3 = valShr[4] ? { 16'h0, tVal2[89:16] } : tVal2;
	tVal4 = valShr[3] ? {  8'h0, tVal3[89: 8] } : tVal3;
	tVal5 = valShr[2] ? {  4'h0, tVal4[89: 4] } : tVal4;
	tVal6 = valShr[1] ? {  2'h0, tVal5[89: 2] } : tVal5;
	tVal7 = valShr[0] ? {  1'h0, tVal6[89: 1] } : tVal6;

end

endmodule

`endif
