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
	regValRs,
	regValRt,
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
reg			tSgnS;
reg			tSgnT;
reg[8:0]	tExpS;
reg[8:0]	tExpT;
reg[18:0]	tFraS;
reg[18:0]	tFraT;

reg			tFmtHalf1;
reg			tSgnS1;
reg			tSgnT1;
reg[8:0]	tExpS1;
reg[8:0]	tExpT1;
reg[8:0]	tExpD1;
reg[18:0]	tFraS1;
reg[18:0]	tFraT1;
reg[18:0]	tFraT1B;
reg[18:0]	tFraT1C;
reg			tSgnCin1;

reg			tSgnU1;
reg[8:0]	tExpU1;
reg[18:0]	tFraU1;

reg			tFmtHalf2;
reg			tSgnU2;
reg[8:0]	tExpU2;
reg[18:0]	tFraU2;

reg			tSgnU2A;
reg[8:0]	tExpU2A;
reg[18:0]	tFraU2A;

reg[31:0]	tValU2S;
reg[15:0]	tValU2H;

reg			tFmtHalf3;
reg[31:0]	tValU3S;
reg[15:0]	tValU3H;

reg			tExpIsZero;

always @*
begin
	tFmtHalf = 0;

//	if(tFmtHalf)
	if(1'b0)
	begin
		tSgnS	= regValRs[15];
		tSgnT	= regValRt[15];
		tExpS	= { 1'b0, regValRs[14] ? 4'h8 : 4'h7, regValRs[13:10] };
		tExpT	= { 1'b0, regValRt[14] ? 4'h8 : 4'h7, regValRt[13:10] };
		tFraS	= { 3'b001, regValRs[9:0], 6'h0 };
		tFraT	= { 3'b001, regValRt[9:0], 6'h0 };
	end
	else
	begin
		tSgnS	= regValRs[31];
		tSgnT	= regValRt[31];
		tExpS	= { 1'b0, regValRs[30:23] };
		tExpT	= { 1'b0, regValRt[30:23] };
//		tFraS	= { 3'b001, regValRs[22:7] };
//		tFraT	= { 3'b001, regValRt[22:7] };

		tFraS	= { 2'b00, tExpS!=0, regValRs[22:7] };
		tFraT	= { 2'b00, tExpT!=0, regValRt[22:7] };
	end
	
//	if(!regExOp[0])
//	if(regExOp[3:0]==4'h2)
	if((regExOp[3:0]==4'h2) && (tExpT!=0))
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
	
	tFraT1B = tFraT1;
	if(tExpD1[3])
		tFraT1B = {  8'b0, tFraT1B[18: 8] };
	if(tExpD1[2])
		tFraT1B = {  4'b0, tFraT1B[18: 4] };
	if(tExpD1[1])
		tFraT1B = {  2'b0, tFraT1B[18: 2] };
	if(tExpD1[0])
		tFraT1B = {  1'b0, tFraT1B[18: 1] };
	if(tExpD1[8:4]!=0)
		tFraT1B = 0;

	tSgnCin1 = (tSgnS1!=tSgnT1);
//	tSgnCin1 = (tSgnS1!=tSgnT1) && (tExpD1[8:4]==0);
	tSgnCin1 = (tSgnS1!=tSgnT1) && tFraT1[16];
	if(tSgnCin1)
		tFraT1C = ~tFraT1B;
//		tFraT1C = -tFraT1B;
	else
		tFraT1C =  tFraT1B;
		
	tFraU1 = tFraS1 + tFraT1C;
//	tFraU1 = tFraS1 + tFraT1C + { 18'h0, tSgnCin1 };
	
	tSgnU1 = tSgnS1;
	tExpU1 = tExpS1;
	if(tFraU1[18])
	begin
		tSgnU1	= !tSgnS1;
		tFraU1	= ~tFraU1;
//		tFraU1	= -tFraU1;
	end
	
	/* Clock Edge */

	tSgnU2A		= tSgnU2;
	tExpU2A		= tExpU2;
	tFraU2A		= tFraU2;

	tExpIsZero = 0;
	
	if(tFraU2[17])
	begin
		tExpU2A		= tExpU2 + 1;
		tFraU2A		= { 1'b0, tFraU2[18:1] };
	end
	else
	begin
		if(tFraU2A[16:1]==16'h0)
		begin
			tExpIsZero = 1;
//			tExpU2A		= tExpU2A - 8;
//			tFraU2A		= { tFraU2A[10:0], 8'b0 };
		end

		if(tFraU2A[16:9]==8'h0)
		begin
			tExpU2A		= tExpU2A - 8;
			tFraU2A		= { tFraU2A[10:0], 8'b0 };
		end
		if(tFraU2A[16:13]==4'h0)
		begin
			tExpU2A		= tExpU2A - 4;
			tFraU2A		= { tFraU2A[14:0], 4'b0 };
		end
		if(tFraU2A[16:15]==2'h0)
		begin
			tExpU2A		= tExpU2A - 2;
			tFraU2A		= { tFraU2A[16:0], 2'b0 };
		end
		if(tFraU2A[16]==1'b0)
		begin
			tExpU2A		= tExpU2A - 1;
			tFraU2A		= { tFraU2A[17:0], 1'b0 };
		end
	end
	
//	tExpIsZero = 0;
	if((tExpU2A[8:7]==2'b11) || (tExpU2A[7:0]==0))
		tExpIsZero = 1;
	
	if(tExpIsZero)
	begin
		tSgnU2A = 0;
		tExpU2A = 0;
		tFraU2A = 0;
	end
	
	tValU2S = { tSgnU2A, tExpU2A[7:0], tFraU2A[15:0], 7'b0 };
	tValU2H = { tSgnU2A, tExpU2A[7], tExpU2A[3:0], tFraU2A[15:6] };
	
//	tRegValRo = tFmtHalf2 ? { 16'b0, tValU2H } : tValU2S;
	tRegValRo = tValU2S;
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		tSgnU2		<= tSgnU1;
		tExpU2		<= tExpU1;
		tFraU2		<= tFraU1;
		tFmtHalf2	<= tFmtHalf1;
		
		tRegValRo2	<= tRegValRo;
	end
end

endmodule

`endif
