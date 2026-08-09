`ifndef HAS_CSSHL64WF
`define HAS_CSSHL64WF

module ExCsShl64WF(valIn, valInLo, valOut, valShl);

input [63:0]	valIn;
input [63:0]	valInLo;
output[63:0]	valOut;
input [ 7:0]	valShl;

reg[127:0]		tVal0;
reg[127:0]		tVal1;
reg[127:0]		tVal2;
reg[127:0]		tVal3;
reg[127:0]		tVal4;
reg[127:0]		tVal5;
reg[127:0]		tVal6;
reg[127:0]		tVal7;

// assign			valOut = tVal7;
// assign			valOut = tVal0;
assign			valOut = tVal0[127:64];

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

//	tVal1 = { valIn, valInLo };
//	tVal2 = valShl[0] ? { tVal1[62:0],  1'h0 } : tVal1;
//	tVal3 = valShl[1] ? { tVal2[61:0],  2'h0 } : tVal2;
//	tVal4 = valShl[2] ? { tVal3[59:0],  4'h0 } : tVal3;
//	tVal5 = valShl[3] ? { tVal4[55:0],  8'h0 } : tVal4;
//	tVal6 = valShl[4] ? { tVal5[47:0], 16'h0 } : tVal5;
//	tVal7 = valShl[5] ? { tVal6[31:0], 32'h0 } : tVal6;
//	tVal0 = (valShl[7:6]!=0) ? 0 : tVal7;

	tVal1 = { valIn, valInLo };
	tVal2 = valShl[0] ? { tVal1[126:0],  1'h0 } : tVal1;
	tVal3 = valShl[1] ? { tVal2[125:0],  2'h0 } : tVal2;
	tVal4 = valShl[2] ? { tVal3[123:0],  4'h0 } : tVal3;
	tVal5 = valShl[3] ? { tVal4[119:0],  8'h0 } : tVal4;
	tVal6 = valShl[4] ? { tVal5[111:0], 16'h0 } : tVal5;
	tVal7 = valShl[5] ? { tVal6[ 95:0], 32'h0 } : tVal6;
	tVal0 = (valShl[7:6]!=0) ? 0 : tVal7;
end

endmodule

`endif
