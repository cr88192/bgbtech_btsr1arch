`ifndef HAS_CSSHR64F
`define HAS_CSSHR64F

module ExCsShr64F(valIn, valOut, valShr);

input [63:0]	valIn;
output[63:0]	valOut;
input [ 7:0]	valShr;

reg[ 6:0]		tValShr;

reg[63:0]		tVal0;
reg[63:0]		tVal1;
reg[63:0]		tVal2;
reg[63:0]		tVal3;
reg[63:0]		tVal4;
reg[63:0]		tVal5;
reg[63:0]		tVal6;
reg[63:0]		tVal7;
reg[63:0]		tValO;

// assign			valOut = tVal7;
assign			valOut = tValO;

always @*
begin

	tValShr	= valShr[6:0];
	if(valShr[7:6]!=0)
		tValShr	= 7'h7F;

//	tVal0 = (tValShr[7:6]!=0) ? 0 : valIn;
//	tVal1 = tVal0;
	tVal1 = valIn;
	tVal2 = tValShr[5] ? { 32'h0, tVal1[63:32] } : tVal1;
	tVal3 = tValShr[4] ? { 16'h0, tVal2[63:16] } : tVal2;
	tVal4 = tValShr[3] ? {  8'h0, tVal3[63: 8] } : tVal3;
	tVal5 = tValShr[2] ? {  4'h0, tVal4[63: 4] } : tVal4;
	tVal6 = tValShr[1] ? {  2'h0, tVal5[63: 2] } : tVal5;
	tVal7 = tValShr[0] ? {  1'h0, tVal6[63: 1] } : tVal6;

`ifdef def_true
	case(tValShr[5:3])
		3'b000: tVal2 = valIn;
		3'b001: tVal2 = {  8'h0, valIn[63: 8] };
		3'b010: tVal2 = { 16'h0, valIn[63:16] };
		3'b011: tVal2 = { 24'h0, valIn[63:24] };
		3'b100: tVal2 = { 32'h0, valIn[63:32] };
		3'b101: tVal2 = { 40'h0, valIn[63:40] };
		3'b110: tVal2 = { 48'h0, valIn[63:48] };
		3'b111: tVal2 = { 56'h0, valIn[63:56] };
	endcase
	case(tValShr[2:0])
		3'b000: tVal7 = tVal2;
		3'b001: tVal7 = {  1'h0, tVal2[63: 1] };
		3'b010: tVal7 = {  2'h0, tVal2[63: 2] };
		3'b011: tVal7 = {  3'h0, tVal2[63: 3] };
		3'b100: tVal7 = {  4'h0, tVal2[63: 4] };
		3'b101: tVal7 = {  5'h0, tVal2[63: 5] };
		3'b110: tVal7 = {  6'h0, tVal2[63: 6] };
		3'b111: tVal7 = {  7'h0, tVal2[63: 7] };
	endcase
`endif

	tValO = tVal7;
	if(tValShr[6])
		tValO[0] = 0;

//	tValO = (valShr[7:6]!=0) ? 0 : tVal7;

end

endmodule

`endif
