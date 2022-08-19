`ifndef HAS_CSSHL64F
`define HAS_CSSHL64F

module ExCsShl64F(valIn, valOut, valShl);

input [63:0]	valIn;
output[63:0]	valOut;
input [ 7:0]	valShl;

reg[63:0]		tVal0;
reg[63:0]		tVal1;
reg[63:0]		tVal2;
reg[63:0]		tVal3;
reg[63:0]		tVal4;
reg[63:0]		tVal5;
reg[63:0]		tVal6;
reg[63:0]		tVal7;

// assign			valOut = tVal7;
assign			valOut = tVal0;

always @*
begin
//	tVal0 = (valShl[7:6]!=0) ? 0 : valIn;
//	tVal1 = tVal0;
//	tVal2 = valShl[5] ? { tVal1[31:0], 32'h0 } : tVal1;
//	tVal3 = valShl[4] ? { tVal2[47:0], 16'h0 } : tVal2;
//	tVal4 = valShl[3] ? { tVal3[55:0],  8'h0 } : tVal3;
//	tVal5 = valShl[2] ? { tVal4[59:0],  4'h0 } : tVal4;
//	tVal6 = valShl[1] ? { tVal5[61:0],  2'h0 } : tVal5;
//	tVal7 = valShl[0] ? { tVal6[62:0],  1'h0 } : tVal6;

	tVal1 = valIn;
	tVal2 = valShl[0] ? { tVal1[62:0],  1'h0 } : tVal1;
	tVal3 = valShl[1] ? { tVal2[61:0],  2'h0 } : tVal2;
	tVal4 = valShl[2] ? { tVal3[59:0],  4'h0 } : tVal3;
	tVal5 = valShl[3] ? { tVal4[55:0],  8'h0 } : tVal4;
	tVal6 = valShl[4] ? { tVal5[47:0], 16'h0 } : tVal5;
	tVal7 = valShl[5] ? { tVal6[31:0], 32'h0 } : tVal6;
	tVal0 = (valShl[7:6]!=0) ? 0 : tVal7;
end

endmodule

`endif
