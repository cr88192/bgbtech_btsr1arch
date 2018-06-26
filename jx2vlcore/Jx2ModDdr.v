/*
LPDDR Interface Module

*/

module Jx2ModDdr(
	clock,	reset,
	memDataIn,	memDataOut,
	memAddr,	memOpm,
	memOK,
	
	ddrData,
	ddrCmd,
	ddrClk);

input			clock;
input			reset;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
input[31:0]		memAddr;
input[5:0]		memOpm;
output[1:0]		memOK;
	
inout[15:0]		ddrData;		//DDR data pins
output[9:0]		ddrCmd;			//Command/Address pins
output[1:0]		ddrClk;			//clock pins
	
endmodule
