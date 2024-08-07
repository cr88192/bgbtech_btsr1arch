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
MULx.W
 */

`ifndef HAS_EXMULW
`define HAS_EXMULW

`ifndef def_true
`include "ModDsp48.v"
`endif

module ExMulW(
	clock,		reset,
	valRs,		valRt,
	idUCmd,		idUIxt,
	exHold,		valRn,
	idLane
	);

input			clock;
input			reset;

input[63:0]		valRs;
input[63:0]		valRt;
output[63:0]	valRn;
input[8:0]		idUCmd;
input[8:0]		idUIxt;
input			exHold;
input[1:0]		idLane;

(* max_fanout = 200 *)
	wire		exHoldN = !exHold;



reg[63:0]	tValRn;
assign 	valRn = tValRn;

reg[8:0]		tIdUCmd;
reg[8:0]		tIdUIxt;
reg[8:0]		tIdUCmdB;

(* max_fanout = 200 *)
	reg[8:0]		tIdUIxtB;

reg[63:0]		tValRs;
reg[63:0]		tValRt;

reg[31:0]		tValRn1A;
reg[31:0]		tValRn1B;
reg[31:0]		tValRn1C;
reg[31:0]		tValRn1D;

reg[35:0]		tValRn1E;

reg[35:0]		tValRsSxA;
reg[35:0]		tValRtSxA;
reg[35:0]		tValRsSxB;
reg[35:0]		tValRtSxB;
reg[35:0]		tValRsSxC;
reg[35:0]		tValRtSxC;
reg[35:0]		tValRsSxD;
reg[35:0]		tValRtSxD;

`ifndef def_true
wire[47:0]	tResMacA;
wire[47:0]	tResMacB;
wire[47:0]	tResMacC;
wire[47:0]	tResMacD;
ModDsp48	macA(
	{ tValRs[15], tValRs[15], tValRs[15:0] },
	{ tValRt[15], tValRt[15], tValRt[15:0] },
	UV48_00, tResMacA);
ModDsp48	macB(
	{ tValRs[31], tValRs[31], tValRs[31:16] },
	{ tValRt[31], tValRt[31], tValRt[31:16] },
	tResMacA, tResMacB);
ModDsp48	macC(
	{ tValRs[47], tValRs[47], tValRs[47:32] },
	{ tValRt[47], tValRt[47], tValRt[47:32] },
	tResMacB, tResMacC);
ModDsp48	macD(
	{ tValRs[63], tValRs[63], tValRs[63:48] },
	{ tValRt[63], tValRt[63], tValRt[63:48] },
	tResMacC, tResMacD);
`endif

(* max_fanout = 200 *)
	reg	tValRsSxAp;
(* max_fanout = 200 *)
	reg	tValRsSxBp;
(* max_fanout = 200 *)
	reg	tValRsSxCp;
(* max_fanout = 200 *)
	reg	tValRsSxDp;

(* max_fanout = 200 *)
	reg	tValRtSxAp;
(* max_fanout = 200 *)
	reg	tValRtSxBp;
(* max_fanout = 200 *)
	reg	tValRtSxCp;
(* max_fanout = 200 *)
	reg	tValRtSxDp;

always @*
begin
	tIdUCmd	= idUCmd;
	tIdUIxt	= idUIxt;
	tValRs	= valRs;
	tValRt	= valRt;

`ifndef def_true
	if(tIdUIxt[0])
	begin
		tValRsSxA= { UV16_00, tValRs[15:0] };
		tValRtSxA= { UV16_00, tValRt[15:0] };
	end
	else
	begin
		tValRsSxA= { tValRs[15] ? UV16_FF : UV16_00, tValRs[15:0] };
		tValRtSxA= { tValRt[15] ? UV16_FF : UV16_00, tValRt[15:0] };
	end
`endif

	tValRsSxAp = (tValRs[15] && !tIdUIxt[0]);
	tValRsSxBp = (tValRs[31] && !tIdUIxt[0]);
	tValRsSxCp = (tValRs[47] && !tIdUIxt[0]);
	tValRsSxDp = (tValRs[63] && !tIdUIxt[0]);

	tValRtSxAp = (tValRt[15] && !tIdUIxt[0]);
	tValRtSxBp = (tValRt[31] && !tIdUIxt[0]);
	tValRtSxCp = (tValRt[47] && !tIdUIxt[0]);
	tValRtSxDp = (tValRt[63] && !tIdUIxt[0]);

	tValRsSxA = { tValRsSxAp ? UV20_FF : UV20_00, tValRs[15: 0] };
	tValRtSxA = { tValRtSxAp ? UV20_FF : UV20_00, tValRt[15: 0] };
	tValRsSxB = { tValRsSxBp ? UV20_FF : UV20_00, tValRs[31:16] };
	tValRtSxB = { tValRtSxBp ? UV20_FF : UV20_00, tValRt[31:16] };
	tValRsSxC = { tValRsSxCp ? UV20_FF : UV20_00, tValRs[47:32] };
	tValRtSxC = { tValRtSxCp ? UV20_FF : UV20_00, tValRt[47:32] };
	tValRsSxD = { tValRsSxDp ? UV20_FF : UV20_00, tValRs[63:48] };
	tValRtSxD = { tValRtSxDp ? UV20_FF : UV20_00, tValRt[63:48] };

//	tValRn1E = UV36_XX;
//	if(idLane == 0)
//	begin
//		tValRn1E = 
//			{ (tValRn1A[31] && !tIdUIxtB[0]) ? UV4_FF : UV4_00, tValRn1A } +
//			{ (tValRn1B[31] && !tIdUIxtB[0]) ? UV4_FF : UV4_00, tValRn1B } +
//			{ (tValRn1C[31] && !tIdUIxtB[0]) ? UV4_FF : UV4_00, tValRn1C } +
//			{ (tValRn1D[31] && !tIdUIxtB[0]) ? UV4_FF : UV4_00, tValRn1D } ;
//	end

	case(tIdUIxtB[4:1])
		4'h1:
			tValRn = {
				(tValRn1A[31] && !tIdUIxtB[0]) ? UV32_FF : UV32_00,
				tValRn1A };
		4'h3:
			tValRn = {
				tValRn1B,
				tValRn1A };
		4'h4:
			tValRn = {
				tValRn1D[15: 0],
				tValRn1C[15: 0],
				tValRn1B[15: 0],
				tValRn1A[15: 0] };
		4'h5:
			tValRn = {
				tValRn1D[31:16],
				tValRn1C[31:16],
				tValRn1B[31:16],
				tValRn1A[31:16] };
`ifndef def_true
		4'h6:
			tValRn = {
				(tValRn1E[35] && !tIdUIxtB[0]) ? UV28_FF : UV28_00,
				tValRn1E };
`endif
		default:
			tValRn = UV64_00;
	endcase
end

always @(posedge clock)
begin
//	if(!exHold)
//	begin
//		tIdUCmd	<= idUCmd;
//		tIdUIxt	<= idUIxt;
//		tValRs	<= valRs;
//		tValRt	<= valRt;
//	end
	
	if(exHoldN)
	begin
		tIdUCmdB	<= tIdUCmd;
		tIdUIxtB	<= tIdUIxt;
		tValRn1A	<= tValRsSxA[31:0] * tValRtSxA[31:0];
		tValRn1B	<= tValRsSxB[31:0] * tValRtSxB[31:0];
		tValRn1C	<= tValRsSxC[31:0] * tValRtSxC[31:0];
		tValRn1D	<= tValRsSxD[31:0] * tValRtSxD[31:0];

`ifndef def_true
//		if(idLane == 0)
		if(1'b1)
		begin
			tValRn1E	<= tResMacD[35:0];
//			tValRn1E	<=
//				{ tValRsSxA[35:0] * tValRtSxA[35:0] } +
//				{ tValRsSxB[35:0] * tValRtSxB[35:0] } +
//				{ tValRsSxC[35:0] * tValRtSxC[35:0] } +
//				{ tValRsSxD[35:0] * tValRtSxD[35:0] } ;
		end
		else
		begin
			tValRn1E <= UV36_XX;
		end
`endif

	end
end

endmodule

`endif
