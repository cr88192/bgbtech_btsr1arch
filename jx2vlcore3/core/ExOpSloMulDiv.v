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
Slow Integer MUL/DIV

Perform an Integer MUL/DIV via the Shift-Add approach.
This is slow, but it can do 64-bit MUL/DIV in a moderately cost-effective way.

 */

`include "CoreDefs.v"

module ExOpSloMulDiv(
	clock,		reset,
	idUCmd,		idUIxt,
	valRs,		valRt,
	valRn,		valRnHi,
	exInHold,	exOutHold
	);

input	clock;
input	reset;

input		exInHold;
output		exOutHold;

input[8:0]		idUCmd;
input[8:0]		idUIxt;

input[63:0]		valRs;
input[63:0]		valRt;
output[63:0]	valRn;
output[63:0]	valRnHi;

reg				tDoHold;
assign		exOutHold = tDoHold;

reg[63:0]		tValRs;
reg[63:0]		tValRt;
reg[63:0]		tValRn;
reg[63:0]		tNxtValRn;
reg[63:0]		tValRnHi;
reg[63:0]		tNxtValRnHi;

assign		valRn = tValRn;
assign		valRnHi = tValRnHi;

reg[63:0]		tValQ;
reg[63:0]		tNxtValQ;
reg[63:0]		tValR;
reg[63:0]		tNxtValR;

reg[63:0]		tValAQ;
reg[63:0]		tNxtValAQ;
reg[63:0]		tValAR;
reg[63:0]		tNxtValAR;

reg[63:0]		tValRsubD;
reg				tValRsubDc;

reg[63:0]		tValAddD;
reg[63:0]		tNxtValAddD;
reg				tValAddDc;
reg				tNxtValAddDc;

reg[5:0]		tValOp;
reg[5:0]		tNxtValOp;
reg				tValSg;
reg				tNxtValSg;
reg				tValSg1;
reg				tValSg1s;
reg				tValSg1t;

reg				tValC1;
reg				tValC;
reg				tNxtValC;

reg[6:0]		tOpCnt;
reg[6:0]		tNxtOpCnt;

always @*
begin
	tNxtValAddD		= tValAddD;
	tNxtValAddDc	= tValAddDc;
	tNxtValOp		= tValOp;
	tNxtValSg		= tValSg;
	tDoHold			= 0;
	tNxtOpCnt		= tOpCnt;
	
	tValC1		=	tValC;
	if(!tNxtValOp[2])
		tValC1		=	tValR[63];
	
//	tNxtValR	= { tValR[62:0], tValQ[63] };
	tNxtValR	= { tValR[62:0], tValQ[63] && tNxtValOp[2] };

//	tValRsubD	= tNxtValR[63:0] + tValAddD;
//	tValRsubD	= tNxtValR[63:0] + tValAddD + { 63'h0, tValAddDc };
	{ tValRsubDc, tValRsubD }	=
		{  1'b0, tNxtValR[63:0] } +
		{  1'b0, tValAddD } +
		{ 64'h0, tValAddDc };

//	if(tValRsubDc && !tNxtValOp[2])
//		tValC1 = 1;
	tNxtValQ	= { tValQ[62:0], tValC1 };

	if(tValRsubDc && !tNxtValOp[2])
//		tNxtValQ = tNxtValQ+1;
		tNxtValQ[3:0] = tNxtValQ[3:0] + 1;

	tNxtValC	= !tValRsubD[63];
	if(tNxtValC && tNxtValOp[2])
		tNxtValR = tValRsubD;

	if(tValQ[63] && !tNxtValOp[2])
	begin
		tNxtValR	= tValRsubD;
	end

	tNxtValAR	= tValSg ? -tValR[63:0] : tValR[63:0];
	tNxtValAQ	= tValSg ? -tValQ[63:0] : tValQ[63:0];

	tNxtValRnHi = tValAQ[63:0];
	if(tValOp[1] ^ !tValOp[2])
		tNxtValRn	= tValAR[63:0];
	else
		tNxtValRn	= tValAQ[63:0];

//	tNxtValRnHi = tValQ[63:0];
//	if(tValOp[1] ^ !tValOp[2])
//		tNxtValRn	= tValSg ? -tValR[63:0] : tValR[63:0];
//	else
//		tNxtValRn	= tValSg ? -tValQ[63:0] : tValQ[63:0];

	tDoHold		= (tOpCnt!=0);

	if(tOpCnt!=0)
	begin
		tNxtOpCnt	= tOpCnt - 1;
//		tDoHold		= 1;
	end
	else
	begin
		tNxtValRn		= tValRn;
		tNxtValRnHi		= tValRnHi;

		if(idUCmd[5:0]==JX2_UCMD_QMULDIV)
		begin

`ifdef def_true
			tValSg1s		= valRs[63];
			tValSg1t		= valRt[63];

			if(idUIxt[3])
			begin
				tValSg1s		= valRs[31];
				tValSg1t		= valRt[31];
			end

			if(idUIxt[0])
			begin
				tValSg1s		= 0;
				tValSg1t		= 0;
			end

			if(idUIxt[4])
				tValSg1t		= 0;

//			tNxtValSg		= (valRs[63] ^ valRt[63]) && !idUIxt[0];
			tNxtValSg		= tValSg1s ^ tValSg1t;

			tNxtValR		= UV64_00;
			tNxtValAddDc	= 0;

			tNxtValOp		= idUIxt[5:0];

//			if(valRs[63] && !idUIxt[0])
			if(tValSg1s)
				tNxtValQ		= -valRs;
			else
				tNxtValQ		=  valRs;

//			if((valRt[63] && !idUIxt[0]) ^ idUIxt[2])
			if(tValSg1t ^ idUIxt[2])
			begin
				tNxtValAddD		= ~valRt;
				tNxtValAddDc	= 1;
			end
			else
			begin
				tNxtValAddD		=  valRt;
			end

			if(idUIxt[3])
			begin
				tNxtValQ[63:32]		= UV32_00;
				tNxtValAddD[63:32]	= UV32_00;
			end

			if(idUIxt[2])
			begin
				tNxtOpCnt		= 67;
				if(idUIxt[1])
					tNxtOpCnt		= 66;
			end
			else
			begin
				tNxtOpCnt		= 66;
			end
`endif


		end
	end
end

always @(posedge clock)
begin
	tValQ		<= tNxtValQ;
	tValR		<= tNxtValR;
	tValC		<= tNxtValC;
	tOpCnt		<= tNxtOpCnt;
	tValAddD	<= tNxtValAddD;
	tValAddDc	<= tNxtValAddDc;
	tValOp		<= tNxtValOp;
	tValSg		<= tNxtValSg;

	tValAQ		<= tNxtValAQ;
	tValAR		<= tNxtValAR;

	tValRn		<= tNxtValRn;
	tValRnHi	<= tNxtValRnHi;
	
	if(!tDoHold)
	begin
		tValRs	<= valRs;
		tValRt	<= valRt;
	end
end

endmodule
