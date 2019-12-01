module Jx2ExOpCmp(
	/* verilator lint_off UNUSED */
	clock,		reset,

	regValRm,		regValRn,

	cmpRmRnEqL,		cmpRmRnEqQ,
	cmpRmRnHiL,		cmpRmRnHiQ,
	cmpRmRnHsL,		cmpRmRnHsQ,
	tstRmRnL,		tstRmRnQ, 

	cmpRmRnEqPW,	cmpRmRnEqPL,
	cmpRmRnHiPW,	cmpRmRnHiPL,
	cmpRmRnHsPW,	cmpRmRnHsPL
	);

input			clock;
input			reset;

input[63:0]		regValRm;		//Rm input value
input[63:0]		regValRn;		//Rn input value

output			cmpRmRnEqL;
output			cmpRmRnEqQ;
output			cmpRmRnHiL;
output			cmpRmRnHiQ;
output			cmpRmRnHsL;
output			cmpRmRnHsQ;

output			tstRmRnL;
output			tstRmRnQ;

output[3:0]		cmpRmRnEqPW;
output[3:0]		cmpRmRnHiPW;
output[3:0]		cmpRmRnHsPW;
output[1:0]		cmpRmRnEqPL;
output[1:0]		cmpRmRnHiPL;
output[1:0]		cmpRmRnHsPL;

assign		cmpRmRnEqL = tCmpRmRnEqL;
assign		cmpRmRnEqQ = tCmpRmRnEqQ;
assign		cmpRmRnHiL = tCmpRmRnHiL;
assign		cmpRmRnHiQ = tCmpRmRnHiQ;
assign		cmpRmRnHsL = tCmpRmRnHsL;
assign		cmpRmRnHsQ = tCmpRmRnHsQ;

assign		tstRmRnL = tTstRmRnL;
assign		tstRmRnQ = tTstRmRnQ;

assign		cmpRmRnEqPW = tCmpRmRnEqPW;
assign		cmpRmRnHiPW = tCmpRmRnHiPW;
assign		cmpRmRnHsPW = tCmpRmRnHsPW;
assign		cmpRmRnEqPL = tCmpRmRnEqPL;
assign		cmpRmRnHiPL = tCmpRmRnHiPL;
assign		cmpRmRnHsPL = tCmpRmRnHsPL;

reg				tCmpRmRnEqL;
reg				tCmpRmRnEqQ;
reg				tCmpRmRnHiL;
reg				tCmpRmRnHiQ;
reg				tCmpRmRnHsL;
reg				tCmpRmRnHsQ;

reg				tCmpRmRnEq0;
reg				tCmpRmRnEq1;
reg				tCmpRmRnEq2;
reg				tCmpRmRnEq3;

reg				tCmpRmRnHi0;
reg				tCmpRmRnHi1;
reg				tCmpRmRnHi2;
reg				tCmpRmRnHi3;

reg				tCmpRmRnHs0;
reg				tCmpRmRnHs1;
reg				tCmpRmRnHs2;
reg				tCmpRmRnHs3;

reg				tCmpRmRnEq01;
reg				tCmpRmRnEq23;
// reg				tCmpRmRnEqL;
// reg				tCmpRmRnEqQ;

reg				tCmpRmRnHi01;
reg				tCmpRmRnHi23;
// reg				tCmpRmRnHiL;
// reg				tCmpRmRnHiQ;

reg				tCmpRmRnHs01;
reg				tCmpRmRnHs23;
// reg				tCmpRmRnHsL;
// reg				tCmpRmRnHsQ;

reg				tTstRmRn0;
reg				tTstRmRn1;
reg				tTstRmRn2;
reg				tTstRmRn3;
reg				tTstRmRnL;
reg				tTstRmRnQ;

reg[3:0]		tCmpRmRnEqPW;
reg[3:0]		tCmpRmRnHiPW;
reg[3:0]		tCmpRmRnHsPW;
reg[1:0]		tCmpRmRnEqPL;
reg[1:0]		tCmpRmRnHiPL;
reg[1:0]		tCmpRmRnHsPL;

always @*
begin

	tCmpRmRnEq0 = (regValRn[15: 0]==regValRm[15: 0]);
	tCmpRmRnEq1 = (regValRn[31:16]==regValRm[31:16]);
	tCmpRmRnEq2 = (regValRn[47:32]==regValRm[47:32]);
	tCmpRmRnEq3 = (regValRn[63:48]==regValRm[63:48]);

	tCmpRmRnHi0 = (regValRn[15: 0]> regValRm[15: 0]);
	tCmpRmRnHi1 = (regValRn[31:16]> regValRm[31:16]);
	tCmpRmRnHi2 = (regValRn[47:32]> regValRm[47:32]);
	tCmpRmRnHi3 = (regValRn[63:48]> regValRm[63:48]);
	
	tCmpRmRnHs0 = tCmpRmRnHi0 || tCmpRmRnEq0;
	tCmpRmRnHs1 = tCmpRmRnHi1 || tCmpRmRnEq1;
	tCmpRmRnHs2 = tCmpRmRnHi2 || tCmpRmRnEq2;
	tCmpRmRnHs3 = tCmpRmRnHi3 || tCmpRmRnEq3;

	tCmpRmRnEq01 = tCmpRmRnEq0 && tCmpRmRnEq1;
	tCmpRmRnEq23 = tCmpRmRnEq2 && tCmpRmRnEq3;
	tCmpRmRnEqL = tCmpRmRnEq01;
	tCmpRmRnEqQ = tCmpRmRnEq01 && tCmpRmRnEq23;
	
	tCmpRmRnHi01 = tCmpRmRnHi1 || (tCmpRmRnEq1 && tCmpRmRnHi0);
	tCmpRmRnHs01 = tCmpRmRnHi1 || (tCmpRmRnEq1 && tCmpRmRnHs0);

	tCmpRmRnHi23 = tCmpRmRnHi3 || (tCmpRmRnEq3 && tCmpRmRnHi2);
	tCmpRmRnHs23 = tCmpRmRnHi3 || (tCmpRmRnEq3 && tCmpRmRnHs2);

	tCmpRmRnHiL = tCmpRmRnHi01;
	tCmpRmRnHsL = tCmpRmRnHs01;

	tCmpRmRnHiQ = tCmpRmRnHi3 || (tCmpRmRnEq3 && tCmpRmRnHi2) ||
		(tCmpRmRnEq3 && tCmpRmRnEq2 && tCmpRmRnHiL);
	tCmpRmRnHsQ = tCmpRmRnHi3 || (tCmpRmRnEq3 && tCmpRmRnHs2) ||
		(tCmpRmRnEq3 && tCmpRmRnEq2 && tCmpRmRnHsL);


	tTstRmRn0	= (regValRm[15: 0]&regValRn[15: 0])==0;
	tTstRmRn1	= (regValRm[31:16]&regValRn[31:16])==0;
	tTstRmRn2	= (regValRm[47:32]&regValRn[47:32])==0;
	tTstRmRn3	= (regValRm[63:48]&regValRn[63:48])==0;
	tTstRmRnL	= tTstRmRn0 && tTstRmRn1;
	tTstRmRnQ	= tTstRmRn2 && tTstRmRn3 && tTstRmRnL;

	tCmpRmRnEqPW	= { tCmpRmRnEq3, tCmpRmRnEq2, tCmpRmRnEq1, tCmpRmRnEq0};
	tCmpRmRnHiPW	= { tCmpRmRnHi3, tCmpRmRnHi2, tCmpRmRnHi1, tCmpRmRnHi0};
	tCmpRmRnHsPW	= { tCmpRmRnHs3, tCmpRmRnHs2, tCmpRmRnHs1, tCmpRmRnHs0};
	tCmpRmRnEqPL	= { tCmpRmRnEq23, tCmpRmRnEq01 };
	tCmpRmRnHiPL	= { tCmpRmRnHi23, tCmpRmRnHi01 };
	tCmpRmRnHsPL	= { tCmpRmRnHs23, tCmpRmRnHs01 };

end

endmodule
