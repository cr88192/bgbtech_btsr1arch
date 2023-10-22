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


(* max_fanout = 200 *)
	wire			exHoldN;

assign	exHoldN = !exHold;

reg[31:0]		tRegValRo2;
reg[1:0]		tRegExOK2;

reg[31:0]		tRegValRo;

assign	regValRo	= tRegValRo2;

// assign	regValRo	= tRegValRo;

reg			tFmtHalf;
reg			tSgnS;
reg			tSgnT;
reg[8:0]	tExpS;
reg[8:0]	tExpT;

`ifdef jx2_fpu_v4sf_fullsp
reg[25:0]	tFraS;
reg[25:0]	tFraT;
`else
reg[18:0]	tFraS;
reg[18:0]	tFraT;
`endif

reg			tFmtHalf1;
reg			tSgnS1;
reg			tSgnT1;
reg[8:0]	tExpS1;
reg[8:0]	tExpT1;
reg[8:0]	tExpD1;
reg[8:0]	tExpD1S;
reg[8:0]	tExpD1T;

reg			tSgnS2;
reg			tSgnT2;
reg[8:0]	tExpS2;
reg[8:0]	tExpT2;
reg[8:0]	tExpD2;
reg			tSgnCin2;

reg			tSgnCin1;
reg			tSgnCin1S;
reg			tSgnCin1T;
reg			tSgnU1;
reg[8:0]	tExpU1;

reg			tFmtHalf2;
reg			tSgnU2;
reg[8:0]	tExpU2;

reg			tSgnU2A;
reg[8:0]	tExpU2A;
reg[8:0]	tExpU2Adj;

`ifdef jx2_fpu_v4sf_fullsp
reg[25:0]	tFraS1;
reg[25:0]	tFraT1;

reg[25:0]	tFraS1B;
reg[25:0]	tFraS1C;

reg[25:0]	tFraT1B;
reg[25:0]	tFraT1C;

reg[25:0]	tFraS2;
reg[25:0]	tFraT2;
reg[25:0]	tFraT2B;
reg[25:0]	tFraT2C;

reg[12:0]	tFraU1_Ad0;
reg[14:0]	tFraU1_Ad1p0;
reg[14:0]	tFraU1_Ad1p1;

reg[25:0]	tFraU1;
reg[25:0]	tFraU2;
reg[25:0]	tFraU2A;
`else
reg[18:0]	tFraS1;
reg[18:0]	tFraT1;

reg[18:0]	tFraS1B;
reg[18:0]	tFraT1B;
reg[18:0]	tFraT1C;

reg[18:0]	tFraS2;
reg[18:0]	tFraT2;
reg[18:0]	tFraT2B;
reg[18:0]	tFraT2C;

reg[18:0]	tFraU1;
reg[18:0]	tFraU2;
reg[18:0]	tFraU2A;
`endif


reg[31:0]	tValU2S;
reg[15:0]	tValU2H;

reg			tFmtHalf3;
reg[31:0]	tValU3S;
reg[15:0]	tValU3H;

reg			tExpIsZero;

always @*
begin
	tFmtHalf = 0;
	
	tSgnS	= regValRs[31];
	tSgnT	= regValRt[31];
	tExpS	= { 1'b0, regValRs[30:23] };
	tExpT	= { 1'b0, regValRt[30:23] };

`ifdef jx2_fpu_v4sf_fullsp
	tFraS	= { 2'b00, tExpS!=0, regValRs[22:0] };
	tFraT	= { 2'b00, tExpT!=0, regValRt[22:0] };
`else
	tFraS	= { 2'b00, tExpS!=0, regValRs[22:7] };
	tFraT	= { 2'b00, tExpT!=0, regValRt[22:7] };
`endif

	if((regExOp[3:0]==4'h2) && (tExpT!=0))
		tSgnT = !tSgnT;

	tExpD1S = tExpT - tExpS;
	tExpD1T = tExpS - tExpT;

`ifdef jx2_fpu_v4sf_fullsp
	tFraS1B = tFraS;
	tFraT1B = tFraT;
	tSgnCin1S = (tSgnS!=tSgnT) && tFraS[23];
	tSgnCin1T = (tSgnS!=tSgnT) && tFraT[23];

	if(tExpD1S[4])
		tFraS1B = { 16'b0, tFraS1B[25:16] };
	if(tExpD1S[3])
		tFraS1B = {  8'b0, tFraS1B[25: 8] };
	if(tExpD1S[2])
		tFraS1B = {  4'b0, tFraS1B[25: 4] };
	if(tExpD1S[1])
		tFraS1B = {  2'b0, tFraS1B[25: 2] };
	if(tExpD1S[0])
		tFraS1B = {  1'b0, tFraS1B[25: 1] };
	if(tExpD1S[8:5]!=0)
		tFraS1B = 0;

	if(tExpD1T[4])
		tFraT1B = { 16'b0, tFraT1B[25:16] };
	if(tExpD1T[3])
		tFraT1B = {  8'b0, tFraT1B[25: 8] };
	if(tExpD1T[2])
		tFraT1B = {  4'b0, tFraT1B[25: 4] };
	if(tExpD1T[1])
		tFraT1B = {  2'b0, tFraT1B[25: 2] };
	if(tExpD1T[0])
		tFraT1B = {  1'b0, tFraT1B[25: 1] };
	if(tExpD1T[8:5]!=0)
		tFraT1B = 0;

`else
	tFraS1B = tFraS;
	tFraT1B = tFraT;
	tSgnCin1S = (tSgnS!=tSgnT) && tFraS[16];
	tSgnCin1T = (tSgnS!=tSgnT) && tFraT[16];

	if(tExpD1S[3])
		tFraS1B = {  8'b0, tFraS1B[18: 8] };
	if(tExpD1S[2])
		tFraS1B = {  4'b0, tFraS1B[18: 4] };
	if(tExpD1S[1])
		tFraS1B = {  2'b0, tFraS1B[18: 2] };
	if(tExpD1S[0])
		tFraS1B = {  1'b0, tFraS1B[18: 1] };
	if(tExpD1S[8:4]!=0)
		tFraS1B = 0;

	if(tExpD1T[3])
		tFraT1B = {  8'b0, tFraT1B[18: 8] };
	if(tExpD1T[2])
		tFraT1B = {  4'b0, tFraT1B[18: 4] };
	if(tExpD1T[1])
		tFraT1B = {  2'b0, tFraT1B[18: 2] };
	if(tExpD1T[0])
		tFraT1B = {  1'b0, tFraT1B[18: 1] };
	if(tExpD1T[8:4]!=0)
		tFraT1B = 0;
`endif


	tFmtHalf1 = tFmtHalf;
//	if(tExpS>=tExpT)
	if(tExpD1S[8])
	begin
		tSgnS1 = tSgnS;		tExpS1 = tExpS;		tFraS1 = tFraS;
		tSgnT1 = tSgnT;		tExpT1 = tExpT;		tFraT1 = tFraT1B;
		tExpD1		= tExpD1T;
		tSgnCin1	= tSgnCin1T;
	end
	else
	begin
		tSgnS1 = tSgnT;		tExpS1 = tExpT;		tFraS1 = tFraT;
		tSgnT1 = tSgnS;		tExpT1 = tExpS;		tFraT1 = tFraS1B;
		tExpD1		= tExpD1S;
		tSgnCin1	= tSgnCin1S;
	end

//	tSgnS2		= tSgnS1;
//	tSgnT2		= tSgnT1;
//	tExpS2		= tExpS1;
//	tExpT2		= tExpT1;
//	tFraS2		= tFraS1;
//	tFraT2		= tFraT1;

//	tExpD2		= tExpD1;
//	tSgnCin2	= tSgnCin1;

`ifndef def_true

	tFraS2		= tFraS1;
	tFraT2		= tFraT1;
	tExpD2		= tExpD1;

`ifdef jx2_fpu_v4sf_fullsp
	tFraT2B = tFraT2;
	if(tExpD2[4])
		tFraT2B = { 16'b0, tFraT2B[25:16] };
	if(tExpD2[3])
		tFraT2B = {  8'b0, tFraT2B[25: 8] };
	if(tExpD2[2])
		tFraT2B = {  4'b0, tFraT2B[25: 4] };
	if(tExpD2[1])
		tFraT2B = {  2'b0, tFraT2B[25: 2] };
	if(tExpD2[0])
		tFraT2B = {  1'b0, tFraT2B[25: 1] };
	if(tExpD2[8:5]!=0)
		tFraT2B = 0;

//	tSgnCin1 = (tSgnS2!=tSgnT2);
	tSgnCin1 = (tSgnS2!=tSgnT2) && tFraT2[23];

`else

	tFraT2B = tFraT2;
	if(tExpD2[3])
		tFraT2B = {  8'b0, tFraT2B[18: 8] };
	if(tExpD2[2])
		tFraT2B = {  4'b0, tFraT2B[18: 4] };
	if(tExpD2[1])
		tFraT2B = {  2'b0, tFraT2B[18: 2] };
	if(tExpD2[0])
		tFraT2B = {  1'b0, tFraT2B[18: 1] };
	if(tExpD2[8:4]!=0)
		tFraT2B = 0;

	tSgnCin1 = (tSgnS2!=tSgnT2);
//	tSgnCin1 = (tSgnS2!=tSgnT2) && (tExpD2[8:4]==0);
	tSgnCin1 = (tSgnS2!=tSgnT2) && tFraT2[16];
`endif

`endif

	if(tSgnCin2)
		tFraT2C = ~tFraT2;
	else
		tFraT2C =  tFraT2;


//	tFraU1 = tFraS1 + tFraT1C;
`ifdef jx2_fpu_v4sf_fullsp
	tFraU1_Ad0 =
		{ 1'b0, tFraS2 [11:0] } +
		{ 1'b0, tFraT2C[11:0] } +
		{ 12'h0, tSgnCin2 };

	tFraU1_Ad1p0 =
		{ 1'b0, tFraS2 [25:12] } +
		{ 1'b0, tFraT2C[25:12] } +
		0;
	tFraU1_Ad1p1 =
		{ 1'b0, tFraS2 [25:12] } +
		{ 1'b0, tFraT2C[25:12] } +
		1;
	tFraU1 = {
		tFraU1_Ad0[12] ?
			tFraU1_Ad1p1[13:0] :
			tFraU1_Ad1p0[13:0],
		tFraU1_Ad0[11:0]
	};

//	tFraU1 = tFraS2 + tFraT2C + { 25'h0, tSgnCin2 };
`else
	tFraU1 = tFraS2 + tFraT2C;
//	tFraU1 = tFraS2 + tFraT2C + { 18'h0, tSgnCin2 };
`endif
	
	tSgnU1 = tSgnS2;
	tExpU1 = tExpS2;
`ifdef jx2_fpu_v4sf_fullsp
	if(tFraU1[25])
`else
	if(tFraU1[18])
`endif
	begin
		tSgnU1	= !tSgnS2;
		tFraU1	= ~tFraU1;
//		tFraU1	= -tFraU1;
	end
	
	/* Clock Edge */

	tSgnU2		= tSgnU1;
	tExpU2		= tExpU1;
	tFraU2		= tFraU1;
	tFmtHalf2	= tFmtHalf1;


	tSgnU2A		= tSgnU2;
	tExpU2A		= tExpU2;
	tFraU2A		= tFraU2;

	tExpIsZero = 0;

`ifdef jx2_fpu_v4sf_fullsp
	if(tFraU2[24])
	begin
		tExpU2A		= tExpU2 + 1;
		tFraU2A		= { 1'b0, tFraU2[25:1] };
	end
	else
	begin
		tExpU2Adj = 0;

		if(tFraU2A[23:1]==23'h0)
		begin
			tExpIsZero = 1;
		end

		if(tFraU2A[23:8]==16'h0)
		begin
//			tExpU2A		= tExpU2A - 16;
			tExpU2Adj[4] = 1;
			tFraU2A		= { tFraU2A[9:0], 16'b0 };
		end

		if(tFraU2A[23:16]==8'h0)
		begin
//			tExpU2A		= tExpU2A - 8;
			tExpU2Adj[3] = 1;
			tFraU2A		= { tFraU2A[17:0], 8'b0 };
		end
		if(tFraU2A[23:20]==4'h0)
		begin
//			tExpU2A		= tExpU2A - 4;
			tExpU2Adj[2] = 1;
			tFraU2A		= { tFraU2A[21:0], 4'b0 };
		end
		if(tFraU2A[23:22]==2'h0)
		begin
//			tExpU2A		= tExpU2A - 2;
			tExpU2Adj[1] = 1;
			tFraU2A		= { tFraU2A[23:0], 2'b0 };
		end
		if(tFraU2A[23]==1'b0)
		begin
//			tExpU2A		= tExpU2A - 1;
			tExpU2Adj[0] = 1;
			tFraU2A		= { tFraU2A[24:0], 1'b0 };
		end

		tExpU2A		= tExpU2 - tExpU2Adj;
	end
`else
	if(tFraU2[17])
	begin
		tExpU2A		= tExpU2 + 1;
		tFraU2A		= { 1'b0, tFraU2[18:1] };
	end
	else
	begin
		tExpU2Adj = 0;

		if(tFraU2A[16:1]==16'h0)
		begin
			tExpIsZero = 1;
//			tExpU2A		= tExpU2A - 8;
//			tFraU2A		= { tFraU2A[10:0], 8'b0 };
		end

		if(tFraU2A[16:9]==8'h0)
		begin
//			tExpU2A		= tExpU2A - 8;
			tExpU2Adj[3] = 1;
			tFraU2A		= { tFraU2A[10:0], 8'b0 };
		end
		if(tFraU2A[16:13]==4'h0)
		begin
//			tExpU2A		= tExpU2A - 4;
			tExpU2Adj[2] = 1;
			tFraU2A		= { tFraU2A[14:0], 4'b0 };
		end
		if(tFraU2A[16:15]==2'h0)
		begin
//			tExpU2A		= tExpU2A - 2;
			tExpU2Adj[1] = 1;
			tFraU2A		= { tFraU2A[16:0], 2'b0 };
		end
		if(tFraU2A[16]==1'b0)
		begin
//			tExpU2A		= tExpU2A - 1;
			tExpU2Adj[0] = 1;
			tFraU2A		= { tFraU2A[17:0], 1'b0 };
		end

		tExpU2A		= tExpU2 - tExpU2Adj;
	end
`endif

//	tExpIsZero = 0;
	if((tExpU2A[8:7]==2'b11) || (tExpU2A[7:0]==0))
		tExpIsZero = 1;
	
//	if(tExpIsZero)
//	begin
//		tSgnU2A = 0;
//		tExpU2A = 0;
//		tFraU2A = 0;
//	end
	
`ifdef jx2_fpu_v4sf_fullsp
	tValU2S = { tSgnU2A, tExpU2A[7:0], tFraU2A[22:0] };
	tValU2H = { tSgnU2A, tExpU2A[7], tExpU2A[3:0], tFraU2A[22:13] };
`else
	tValU2S = { tSgnU2A, tExpU2A[7:0], tFraU2A[15:0], 7'b0 };
	tValU2H = { tSgnU2A, tExpU2A[7], tExpU2A[3:0], tFraU2A[15:6] };
`endif
	
//	tRegValRo = tFmtHalf2 ? { 16'b0, tValU2H } : tValU2S;
//	tRegValRo = tValU2S;
	tRegValRo = tExpIsZero ? UV32_00 : tValU2S;
end

always @(posedge clock)
begin
	if(exHoldN)
	begin
		tSgnS2		<= tSgnS1;
		tSgnT2		<= tSgnT1;
		tExpS2		<= tExpS1;
		tExpT2		<= tExpT1;
		tFraS2		<= tFraS1;
		tFraT2		<= tFraT1;

		tExpD2		<= tExpD1;
		tSgnCin2	<= tSgnCin1;


//		tSgnU2		<= tSgnU1;
//		tExpU2		<= tExpU1;
//		tFraU2		<= tFraU1;
//		tFmtHalf2	<= tFmtHalf1;
		
		tRegValRo2	<= tRegValRo;
	end
end

endmodule

`endif
