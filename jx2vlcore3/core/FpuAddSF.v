/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Low Precision FPU Adder
 */

`ifndef HAS_FPUADDSF
`define HAS_FPUADDSF

`include "CoreDefs.v"

module FpuAddSF(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRm,
	regValRn,
	regValRo,
	regExOp,
	regRMode
	);

input	clock;
input	reset;
input	exHold;

input[31:0]		regValRs;
input[31:0]		regValRt;
output[31:0]	regValRo;
input[3:0]		regExOp;
input[7:0]		regRMode;

reg[31:0]		tRegValRo2;
reg[1:0]		tRegExOK2;

assign	regValRo	= tRegValRo2;

reg[31:0]		tRegValRo;

reg			tFmtHalf;
reg			tSgS;
reg			tSgT;
reg[8:0]	tExpS;
reg[8:0]	tExpT;
reg[16:0]	tFraS;
reg[16:0]	tFraT;

reg			tFmtHalf1;
reg			tSgnS1;
reg			tSgnT1;
reg[8:0]	tExpS1;
reg[8:0]	tExpT1;
reg[8:0]	tExpD1;
reg[17:0]	tFraS1;
reg[17:0]	tFraT1;
reg[17:0]	tFraT1B;
reg[17:0]	tFraT1C;

reg			tSgnU1;
reg[8:0]	tExpU1;
reg[17:0]	tFraU1;

reg			tFmtHalf2;
reg			tSgnU2;
reg[8:0]	tExpU2;
reg[17:0]	tFraU2;

reg			tSgnU2A;
reg[8:0]	tExpU2A;
reg[17:0]	tFraU2A;

reg[31:0]	tValU2S;
reg[15:0]	tValU2H;

reg			tFmtHalf3;
reg[31:0]	tValU3S;
reg[15:0]	tValU3H;

always @*
begin
	tFmtHalf = 0;

	if(tFmtHalf)
	begin
		tSgnS	= regValRs[15];
		tSgnT	= regValRt[15];
		tExpS	= { 1'b0, regValRs[14] ? 4'h8 : 4'h7, regValRs[13:10] };
		tExpT	= { 1'b0, regValRt[14] ? 4'h8 : 4'h7, regValRt[13:10] };
		tFraS	= { 3'b001, regValRs[9:0], 5'h0 };
		tFraT	= { 3'b001, regValRt[9:0], 5'h0 };
	end
	else
	begin
		tSgnS	= regValRs[31];
		tSgnT	= regValRt[31];
		tExpS	= { 1'b0, regValRs[30:23] };
		tExpT	= { 1'b0, regValRt[30:23] };
		tFraS	= { 3'b001, regValRs[22:8] };
		tFraT	= { 3'b001, regValRt[22:8] };
	end
	
	if(regExOp[0])
		tSgnT = !tSgnT;
	

	tFmtHalf1 = tFmtHalf;
	if(tExpS>=tExpT)
	begin
		tSgnS1 = tSgnS;		tExpS1 = tExpS;		tFraS1 = tFraS;
		tSgnT1 = tSgnT;		tExpT1 = tExpT;		tFraT1 = tFraT;
		tExpD1 = tExpS - tExpT;
	end
	else
	begin
		tSgnS1 = tSgnT;		tExpS1 = tExpT;		tFraS1 = tFraT;
		tSgnT1 = tSgnS;		tExpT1 = tExpS;		tFraT1 = tFraS;
		tExpD1 = tExpT - tExpS;
	end
	
	tFraT1B = tExpT1;
	if(tExpD1[3])
		tFraT1B = {  8'b0, tFraT1B[17: 8] };
	if(tExpD1[2])
		tFraT1B = {  4'b0, tFraT1B[17: 4] };
	if(tExpD1[1])
		tFraT1B = {  2'b0, tFraT1B[17: 2] };
	if(tExpD1[0])
		tFraT1B = {  1'b0, tFraT1B[17: 1] };
	if(tExpD1[8:4]!=0)
		tFraT1B = 0;

	if(tSgnS1!=tSgnT1)
		tFraT1C = ~tFraT1B;
	else
		tFraT1C =  tFraT1B;
		
	tFraU1 = tFraS1 + tFraT1C;
	
	tSgnU1 = tSgnS1;
	tExpU1 = tExpS1;
	if(tFraU1[17])
	begin
		tSgnU1	= !tSgnS1;
		tFraU1	= ~tFraU1;
	end
	
	/* Clock Edge */

	tSgnU2A		= tSgnU2;
	tExpU2A		= tExpU2;
	tFraU2A		= tFraU2;
	
	if(tFraU2[16])
	begin
		tExpU2A		= tExpU2 + 1;
		tFraU2A		= { 1'b0, tFraU2[17:1] };
	end
	else
	begin
		if(tFraU2A[15:8]==8'h0)
		begin
			tExpU2A		= tExpU2A - 8;
			tFraU2A		= { tFraU2A[9:0], 8'b0 };
		end
		if(tFraU2A[15:12]==4'h0)
		begin
			tExpU2A		= tExpU2A - 4;
			tFraU2A		= { tFraU2A[13:0], 4'b0 };
		end
		if(tFraU2A[15:14]==2'h0)
		begin
			tExpU2A		= tExpU2A - 2;
			tFraU2A		= { tFraU2A[15:0], 2'b0 };
		end
		if(tFraU2A[15]==1'b0)
		begin
			tExpU2A		= tExpU2A - 1;
			tFraU2A		= { tFraU2A[16:0], 1'b0 };
		end
	end
	
	tValU2S = { tSgnU2A, tExpU2A[7:0], tFraU2A[15:0], 8'b0 };
	tValU2H = { tSgnU2A, tExpU2A[7], tExpU2A[3:0], tFraU2A[15:6] };
	
	tRegValRo = tFmtHalf2 ? { 16'b0, tValU2H } : tValU2S;
end

always @(posedge clock)
begin
	tSgnU2		<= tSgnU1;
	tExpU2		<= tExpU1;
	tFraU2		<= tFraU1;
	tFmtHalf2	<= tFmtHalf1;
	
	tRegValRo2	<= tRegValRo;
end

endmodule

`endif
