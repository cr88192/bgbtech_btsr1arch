/*
Multiply two 9 bit numbers using a 3-bit radix.
 */

`ifndef HAS_ExLuMul9Ra8H
`define HAS_ExLuMul9Ra8H

`include "ExLuMul3Ra8.v"

module ExLuMul9Ra8H(valA, valB, valC);

input[8:0]		valA;
input[8:0]		valB;
output[8:0]		valC;

wire[5:0]	tResAA;
wire[5:0]	tResAB;
wire[5:0]	tResAC;
wire[5:0]	tResBA;
wire[5:0]	tResBB;
wire[5:0]	tResBC;
wire[5:0]	tResCA;
wire[5:0]	tResCB;
wire[5:0]	tResCC;

reg[8:0]	tValC;
assign	valC = tValC;

ExLuMul3Ra8		luMulAA(valA[2:0], valB[2:0], tResAA);
ExLuMul3Ra8		luMulAB(valA[2:0], valB[5:3], tResAB);
ExLuMul3Ra8		luMulAC(valA[2:0], valB[8:6], tResAC);

ExLuMul3Ra8		luMulBA(valA[5:3], valB[2:0], tResBA);
ExLuMul3Ra8		luMulBB(valA[5:3], valB[5:3], tResBB);
ExLuMul3Ra8		luMulBC(valA[5:3], valB[8:6], tResBC);

ExLuMul3Ra8		luMulCA(valA[8:6], valB[2:0], tResCA);
ExLuMul3Ra8		luMulCB(valA[8:6], valB[5:3], tResCB);
ExLuMul3Ra8		luMulCC(valA[8:6], valB[8:6], tResCC);

wire[6:0]	tResACpCA;
wire[6:0]	tResBCpCB;
assign	tResACpCA = {1'b0, tResAC} + {1'b0, tResCA};
assign	tResBCpCB = {1'b0, tResBC} + {1'b0, tResCB};

always @*
begin
//	tValC =
//		{ tResCC[5:3], tResCC[2:0], tResAC[5:3] } +
//		{      3'b000, tResBC[5:3], tResBC[2:0] } +
//		{      3'b000, tResCB[5:3], tResCB[2:0] } +
//		{      3'b000,      3'b000, tResCA[5:3] } +
//		{      3'b000,      3'b000, tResBB[5:3] } ;

	tValC =
		{ tResCC[5:0], tResBB[5:3] } +
		{       2'b00, tResBCpCB[5:0] } +
		{       5'b00, tResACpCA[5:3] } + 1;
end

endmodule

`endif
