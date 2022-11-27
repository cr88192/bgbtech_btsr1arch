/*
Multiply two 9 bit numbers using a 3-bit radix.
 */

`ifndef HAS_ExLuMul9Ra8
`define HAS_ExLuMul9Ra8

`include "ExLuMul3Ra8.v"

module ExLuMul9Ra8(valA, valB, valC);

input[8:0]		valA;
input[8:0]		valB;
output[17:0]	valC;

wire[5:0]	tResAA;
wire[5:0]	tResAB;
wire[5:0]	tResAC;
wire[5:0]	tResBA;
wire[5:0]	tResBB;
wire[5:0]	tResBC;
wire[5:0]	tResCA;
wire[5:0]	tResCB;
wire[5:0]	tResCC;

reg[17:0]	tValC;
assign	valC = tValC;

ExLuMul3Ra8		luMulAA(valA[2:0], valB[2:0], tResAA);		//1
ExLuMul3Ra8		luMulAB(valA[2:0], valB[5:3], tResAB);		//1
ExLuMul3Ra8		luMulAC(valA[2:0], valB[8:6], tResAC);		//1

ExLuMul3Ra8		luMulBA(valA[5:3], valB[2:0], tResBA);		//1
ExLuMul3Ra8		luMulBB(valA[5:3], valB[5:3], tResBB);		//1
ExLuMul3Ra8		luMulBC(valA[5:3], valB[8:6], tResBC);		//1

ExLuMul3Ra8		luMulCA(valA[8:6], valB[2:0], tResCA);		//1
ExLuMul3Ra8		luMulCB(valA[8:6], valB[5:3], tResCB);		//1
ExLuMul3Ra8		luMulCC(valA[8:6], valB[8:6], tResCC);		//1

always @*
begin
	tValC =
	{tResCC[5:3],tResCC[2:0],tResAC[5:3],tResAC[2:0],tResAA[5:3],tResAA[2:0]} +
	{     3'b000,tResBC[5:3],tResBC[2:0],tResBA[5:3],tResBA[2:0],     3'b000} +
	{     3'b000,tResCB[5:3],tResCB[2:0],tResAB[5:3],tResAB[2:0],     3'b000} +
	{     3'b000,     3'b000,tResCA[5:3],tResCA[2:0],     3'b000,     3'b000} +
	{     3'b000,     3'b000,tResBB[5:3],tResBB[2:0],     3'b000,     3'b000} ;
end

endmodule

`endif
