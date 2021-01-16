`ifndef HAS_CSSHL90F
`define HAS_CSSHL90F

module ExCsShl90F(valIn, valOut, valShl);

input [89:0]	valIn;
output[89:0]	valOut;
input [ 7:0]	valShl;

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
	tVal0 = valShl[7] ? 0 : valIn;
	tVal1 = valShl[6] ? { tVal0[25:0], 64'h0 } : tVal0;
	tVal2 = valShl[5] ? { tVal1[57:0], 32'h0 } : tVal1;
	tVal3 = valShl[4] ? { tVal2[73:0], 16'h0 } : tVal2;
	tVal4 = valShl[3] ? { tVal3[81:0],  8'h0 } : tVal3;
	tVal5 = valShl[2] ? { tVal4[85:0],  4'h0 } : tVal4;
	tVal6 = valShl[1] ? { tVal5[87:0],  2'h0 } : tVal5;
	tVal7 = valShl[0] ? { tVal6[88:0],  1'h0 } : tVal6;
end

endmodule

`endif
