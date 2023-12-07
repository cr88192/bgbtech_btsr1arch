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
	exInHold,	exOutHold,
	ex1MulFaz
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

input	ex1MulFaz;		//Multiplier has handled divide.

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

reg[16:0]		tValRsubD_Cs0p0;
reg[16:0]		tValRsubD_Cs0p1;
reg[16:0]		tValRsubD_Cs1p0;
reg[16:0]		tValRsubD_Cs1p1;
reg[16:0]		tValRsubD_Cs2p0;
reg[16:0]		tValRsubD_Cs2p1;
reg[16:0]		tValRsubD_Cs3p0;
reg[16:0]		tValRsubD_Cs3p1;
reg				tValRsubD_Sel0;
reg				tValRsubD_Sel1;
reg				tValRsubD_Sel2;
reg				tValRsubD_Sel3;
reg				tValRsubD_Sel4;

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

reg				tNxtValSgFdiv;
reg				tValSgFdiv;
reg[11:0]		tNxtValExpFdiv;
reg[11:0]		tValExpFdiv;
reg				tValFdivRndb;
reg[8:0]		tValFdivRnd;

reg[6:0]		tOpCnt;
reg[6:0]		tNxtOpCnt;
reg[6:0]		tLstOpCnt;

always @*
begin
	tNxtValAddD		= tValAddD;
	tNxtValAddDc	= tValAddDc;
	tNxtValOp		= tValOp;
	tNxtValSg		= tValSg;
	tNxtValSgFdiv	= tValSgFdiv;
	tNxtValExpFdiv	= tValExpFdiv;
	tDoHold			= 0;
	tNxtOpCnt		= tOpCnt;
	
	tValC1		=	tValC;
	if(!tNxtValOp[2])
		tValC1		=	tValR[63];
	
//	tNxtValR	= { tValR[62:0], tValQ[63] };
	tNxtValR	= { tValR[62:0], tValQ[63] && tNxtValOp[2] };

//	tValRsubD	= tNxtValR[63:0] + tValAddD;
//	tValRsubD	= tNxtValR[63:0] + tValAddD + { 63'h0, tValAddDc };

//	{ tValRsubDc, tValRsubD }	=
//		{  1'b0, tNxtValR[63:0] } +
//		{  1'b0, tValAddD } +
//		{ 64'h0, tValAddDc };

	tValRsubD_Cs0p0 = { 1'b0, tNxtValR[15: 0] } + { 1'b0, tValAddD[15: 0] } + 0;
	tValRsubD_Cs0p1 = { 1'b0, tNxtValR[15: 0] } + { 1'b0, tValAddD[15: 0] } + 1;
	tValRsubD_Cs1p0 = { 1'b0, tNxtValR[31:16] } + { 1'b0, tValAddD[31:16] } + 0;
	tValRsubD_Cs1p1 = { 1'b0, tNxtValR[31:16] } + { 1'b0, tValAddD[31:16] } + 1;
	tValRsubD_Cs2p0 = { 1'b0, tNxtValR[47:32] } + { 1'b0, tValAddD[47:32] } + 0;
	tValRsubD_Cs2p1 = { 1'b0, tNxtValR[47:32] } + { 1'b0, tValAddD[47:32] } + 1;
	tValRsubD_Cs3p0 = { 1'b0, tNxtValR[63:48] } + { 1'b0, tValAddD[63:48] } + 0;
	tValRsubD_Cs3p1 = { 1'b0, tNxtValR[63:48] } + { 1'b0, tValAddD[63:48] } + 1;

	tValRsubD_Sel0 = tValAddDc;
	tValRsubD_Sel1 = tValRsubD_Sel0 ? tValRsubD_Cs0p1[16] : tValRsubD_Cs0p0[16];
	tValRsubD_Sel2 = tValRsubD_Sel1 ? tValRsubD_Cs1p1[16] : tValRsubD_Cs1p0[16];
	tValRsubD_Sel3 = tValRsubD_Sel2 ? tValRsubD_Cs2p1[16] : tValRsubD_Cs2p0[16];
	tValRsubD_Sel4 = tValRsubD_Sel3 ? tValRsubD_Cs3p1[16] : tValRsubD_Cs3p0[16];

	tValRsubD	= {
		tValRsubD_Sel3 ? tValRsubD_Cs3p1[15:0] : tValRsubD_Cs3p0[15:0],
		tValRsubD_Sel2 ? tValRsubD_Cs2p1[15:0] : tValRsubD_Cs2p0[15:0],
		tValRsubD_Sel1 ? tValRsubD_Cs1p1[15:0] : tValRsubD_Cs1p0[15:0],
		tValRsubD_Sel0 ? tValRsubD_Cs0p1[15:0] : tValRsubD_Cs0p0[15:0]
		};
	tValRsubDc	= tValRsubD_Sel4;

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
	
	if(tValOp[3])
	begin
		tNxtValAR[63:32] = tValSg ? UV32_FF : UV32_00;
		tNxtValAQ[63:32] = tValSg ? UV32_FF : UV32_00;
	end

	tNxtValRnHi = tValAQ[63:0];
	if(tValOp[1] ^ !tValOp[2])
		tNxtValRn	= tValAR[63:0];
	else
		tNxtValRn	= tValAQ[63:0];

`ifdef jx2_alu_slomuldiv_fdiv
	if(tValOp[5:4]==2'b10)
	begin
		tNxtValRn[63] = tValSgFdiv;
		if(tValAQ[54])
		begin
			tNxtValRn[62:52]	= tValExpFdiv[10:0]+1;
			tNxtValRn[51:0]		= tValAQ[53:2];
			tValFdivRndb		= tValAQ[1];
		end
		else
		begin
			tNxtValRn[62:52]	= tValExpFdiv[10:0];
			tNxtValRn[51:0]		= tValAQ[52:1];
			tValFdivRndb		= tValAQ[0];
		end
		
		tValFdivRnd = { 1'b0, tNxtValRn[7:0] } + { 8'b0, tValFdivRndb };
		if(!tValFdivRnd[8])
			tNxtValRn[7:0] = tValFdivRnd[7:0];

		if(tValExpFdiv[11])
		begin
			tNxtValRn[51:48]	= 0;
			if(tValExpFdiv[10])
			begin
				tNxtValRn[62:52]	= 11'h000;
			end
			else
			begin
				tNxtValRn[62:52]	= 11'h7FF;
			end		
		end
	end
`endif

`ifdef jx2_alu_slomuldiv_fdivs
	if(tValOp[5:4]==2'b11)
//	if(tValOp[5:0]==JX2_UCIX_QMUL_FDIVS)
	begin
		tNxtValRn[31] = tValSgFdiv;
		if(tValAQ[54])
		begin
			tNxtValRn[30:23]	= { tValExpFdiv[10], tValExpFdiv[6:0] } +1;
			tNxtValRn[22:0]		= tValAQ[53:31];
			tValFdivRndb		= tValAQ[30];
		end
		else
		begin
			tNxtValRn[30:23]	= { tValExpFdiv[10], tValExpFdiv[6:0] };
			tNxtValRn[22:0]		= tValAQ[52:30];
			tValFdivRndb		= tValAQ[29];
		end
		
		tValFdivRnd = { 1'b0, tNxtValRn[7:0] } + { 8'b0, tValFdivRndb };
		if(!tValFdivRnd[8])
			tNxtValRn[7:0] = tValFdivRnd[7:0];

		if(tValExpFdiv[11])
		begin
			tNxtValRn[23:19]	= 0;
			if(tValExpFdiv[10])
			begin
				tNxtValRn[30:23]	= 8'h00;
			end
			else
			begin
				tNxtValRn[30:23]	= 8'hFF;
			end		
		end
		
		tNxtValRn[63:32]=0;
	end
`endif

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
//		$display("SloMulDiv: Rn=%X Cnt=%d Ixt=%X", tNxtValRn, tOpCnt, tValOp);

	end
	else
		if(tLstOpCnt==0)
//		if((tLstOpCnt==0) && !ex1MulFaz)
	begin
		tNxtValRn		= tValRn;
		tNxtValRnHi		= tValRnHi;

		if(idUCmd[5:0]==JX2_UCMD_QMULDIV)
//		if((idUCmd[5:0]==JX2_UCMD_QMULDIV) && !ex1MulFaz)
		begin

//			$display("SloMulDiv: %x %x %x-%d", valRs, valRt, idUCmd, idUIxt);

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
			tNxtValSgFdiv	= valRs[63] ^ valRt[63];

			if(idUIxt[2:0]==3'b110)
			begin
				tNxtValSg		= tValSg1s;
			end

			tNxtValExpFdiv	=
				1022 +
				{ 1'b0, valRs[62:52] } -
				{ 1'b0, valRt[62:52] };

`ifdef jx2_alu_slomuldiv_fdivs
			if(idUIxt[5:4]==2'b11)
//			if(idUIxt[5:0]==JX2_UCIX_QMUL_FDIVS)
			begin
				tNxtValExpFdiv	=
					1022 +
					{ 4'b0, valRs[30:23] } -
					{ 4'b0, valRt[30:23] };
			end
`endif

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
//				tNxtValQ[63:32]		= UV32_00;
//				tNxtValAddD[63:32]	= UV32_00;

				tNxtValQ		= { tNxtValQ[31:0], 32'h0 };
//				tNxtValAddD		= UV32_00;
			end

`ifdef jx2_alu_slomuldiv_fdiv
			if(idUIxt[5:4]==2'b10)
			begin
				tNxtValQ[63:52]		= 12'h001;
				tNxtValAddD[63:52]	= 12'hFFE;
			end
`endif

`ifdef jx2_alu_slomuldiv_fdivs
			if(idUIxt[5:4]==2'b11)
//			if(idUIxt[5:0]==JX2_UCIX_QMUL_FDIVS)
			begin
				tNxtValQ[63:52]		= 12'h001;
				tNxtValAddD[63:52]	= 12'hFFE;
				tNxtValQ   [51:29]=tNxtValQ   [22:0];
				tNxtValAddD[51:29]=tNxtValAddD[22:0];
			end
`endif

			if(idUIxt[2])
			begin
				tNxtOpCnt		= 67;
				if(idUIxt[1])
					tNxtOpCnt		= 66;

				if(idUIxt[3])
				begin
					tNxtOpCnt		= 35;
					if(idUIxt[1])
						tNxtOpCnt		= 34;
				end

`ifdef jx2_alu_slomuldiv_fdiv
				if(idUIxt[5])
				begin
					tNxtOpCnt		= 121;
//					tNxtOpCnt		= 106;
				end
`endif
			end
			else
			begin
				if(idUIxt[3])
					tNxtOpCnt		= 34;
				else
					tNxtOpCnt		= 66;
			end
`endif

			if(ex1MulFaz)
				tNxtOpCnt		= 0;

		end
	end
	
//	if(ex1MulFaz)
//		tNxtOpCnt		= 0;
	
//	if(exInHold && !tDoHold)
//	begin
//		tNxtValRn	= tValRn;
//		tNxtValRnHi	= tValRnHi;
//	end

	if((tOpCnt!=0) && (tNxtOpCnt==0))
	begin
//		$display("SloMulDiv: Out %X %X", tNxtValRn, tNxtValRnHi);
	end
	
	if(reset)
	begin
		tNxtOpCnt		= 0;
		tDoHold			= 0;
		tNxtValOp		= 0;
	end
end

always @(posedge clock)
begin

//	if(tDoHold)
//	if((tDoHold || (tNxtOpCnt!=0)) && !exInHold)
//	if(tDoHold || (tNxtOpCnt!=0))
	if(tDoHold || (tNxtOpCnt!=0) || reset)
//	if(tNxtOpCnt!=0)
	begin
		tOpCnt		<= tNxtOpCnt;
	end

	tLstOpCnt	<= tOpCnt;

//	if(tNxtOpCnt!=0)
	if(tDoHold || (tNxtOpCnt!=0))
	begin
		tValQ		<= tNxtValQ;
		tValR		<= tNxtValR;
		tValC		<= tNxtValC;
		tValAddD	<= tNxtValAddD;
		tValAddDc	<= tNxtValAddDc;
//		tValOp		<= tNxtValOp;
		tValSg		<= tNxtValSg;

		tValSgFdiv	<= tNxtValSgFdiv;
		tValExpFdiv	<= tNxtValExpFdiv;

		tValAQ		<= tNxtValAQ;
		tValAR		<= tNxtValAR;
	end

	if(tDoHold)
	begin
		tValRn		<= tNxtValRn;
		tValRnHi	<= tNxtValRnHi;
	end
	
	if(!tDoHold)
//	if(!tDoHold && !exInHold)
	begin
		tValRs		<= valRs;
		tValRt		<= valRt;
		tValOp		<= tNxtValOp;
	end
end

endmodule
