`ifndef HAS_FPUMULSF
`define HAS_FPUMULSF

`include "CoreDefs.v"

`include "ExLuMul9Ra8H.v"
// `include "ExLuMul9Ra8.v"

module FpuMulSF(
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

assign	regValRo	= tRegValRo2;

reg[31:0]		tRegValRo;

reg			tFmtHalf;
reg			tSgnS;
reg			tSgnT;
reg[8:0]	tExpS;
reg[8:0]	tExpT;

`ifdef jx2_fpu_v4sf_fullsp
reg[24:0]	tFraS;
reg[24:0]	tFraT;
`else
reg[17:0]	tFraS;
reg[17:0]	tFraT;
`endif

reg			tSgN;
reg[8:0]	tExpN;
reg[8:0]	tExpNp1;

`ifdef jx2_fpu_v4sf_fullsp
reg[35:0]	tFraN0A;
reg[49:0]	tFraN0;
reg[25:0]	tFraN0B;
reg[24:0]	tFraN;
`else
reg[35:0]	tFraN0;
reg[17:0]	tFraN;
`endif

reg			tSgN2;
reg[8:0]	tExpN2;
reg[8:0]	tExpN2p1;
reg[8:0]	tExpN2b;

`ifdef jx2_fpu_v4sf_fullsp
reg[24:0]	tFraN2;
reg[24:0]	tFraN2b;
`else
reg[17:0]	tFraN2;
reg[17:0]	tFraN2b;
`endif

reg			tExpIsZero0;
reg			tExpIsZero1;

reg			tExpIsZero;

wire[9:0]	tMulLoC0;

// `ifndef def_true
`ifdef def_true
wire[8:0]	tMulLoA0;
wire[8:0]	tMulLoB0;

ExLuMul9Ra8H	exMulLoA(
	{regValRs[6:0], 2'b0 },
	{1'b1, regValRt[22:15]},
	tMulLoA0);
ExLuMul9Ra8H	exMulLoB(
	{regValRt[6:0], 2'b0 },
	{1'b1, regValRs[22:15]},
	tMulLoB0);
assign		tMulLoC0 = {1'b0, tMulLoA0} + {1'b0, tMulLoB0};
`endif


// `ifdef def_true
`ifndef def_true
wire[17:0]	tMulLoA0;
wire[17:0]	tMulLoB0;

ExLuMul9Ra8		exMulLoA(
	{regValRs[6:0], 2'b0 },
	{1'b1, regValRt[22:15]},
	tMulLoA0);
ExLuMul9Ra8		exMulLoB(
	{regValRt[6:0], 2'b0 },
	{1'b1, regValRs[22:15]},
	tMulLoB0);
assign		tMulLoC0 = {1'b0, tMulLoA0[17:9]} + {1'b0, tMulLoB0[17:9]};
`endif

// `ifdef def_true
`ifndef def_true
wire[17:0]	tMulLoA0;
wire[17:0]	tMulLoB0;

assign	tMulLoA0 =
	{ 9'b0, regValRs[6:0], 2'b0 } *
	{ 9'b0, 1'b1, regValRt[22:15] };
assign	tMulLoB0 =
	{ 9'b0, regValRt[6:0], 2'b0 } *
	{ 9'b0, 1'b1, regValRs[22:15] };
assign		tMulLoC0 = {1'b0, tMulLoA0[17:9]} + {1'b0, tMulLoB0[17:9]};
`endif


always @*
begin
`ifndef def_true
	tFmtHalf = 0;

	if(tFmtHalf)
	begin
		tSgnS	= regValRs[15];
		tSgnT	= regValRt[15];
		tExpS	= { 1'b0, regValRs[14] ? 4'h8 : 4'h7, regValRs[13:10] };
		tExpT	= { 1'b0, regValRt[14] ? 4'h8 : 4'h7, regValRt[13:10] };
		tFraS	= { 2'b01, regValRs[9:0], 6'h0 };
		tFraT	= { 2'b01, regValRt[9:0], 6'h0 };
	end
	else
	begin
		tSgnS	= regValRs[31];
		tSgnT	= regValRt[31];
		tExpS	= { 1'b0, regValRs[30:23] };
		tExpT	= { 1'b0, regValRt[30:23] };
		tFraS	= { 2'b01, regValRs[22:7] };
		tFraT	= { 2'b01, regValRt[22:7] };
	end
`endif

`ifdef def_true
	tSgnS	= regValRs[31];
	tSgnT	= regValRt[31];
	tExpS	= { 1'b0, regValRs[30:23] };
	tExpT	= { 1'b0, regValRt[30:23] };

`ifdef jx2_fpu_v4sf_fullsp
	tFraS	= { 2'b01, regValRs[22:0] };
	tFraT	= { 2'b01, regValRt[22:0] };
`else
	tFraS	= { 2'b01, regValRs[22:7] };
	tFraT	= { 2'b01, regValRt[22:7] };
`endif

`endif

	tExpIsZero0	= 0;
	if(tExpS == 0)
		tExpIsZero0	= 1;
	if(tExpT == 0)
		tExpIsZero0	= 1;
	
	tSgN	= tSgnS ^ tSgnT;
	tExpN	= tExpS + tExpT - 127;
	tExpNp1	= tExpN + 1;
	
`ifdef jx2_fpu_v4sf_fullsp
	tFraN0A	= { 18'h0, tFraS[24:7] } * { 18'h0, tFraT[24:7] };
//	tFraN	= tFraN0A[33: 9];
//	tFraN	= tFraN0A[33: 9] +
//		{ 17'h0, {1'b0, tFraS[6:0]} + {1'b0, tFraT[6:0]}};

	tFraN0B = tFraN0A[33: 8] + { 16'h0, tMulLoC0[9:0] } + 1;
	tFraN	= tFraN0B[25:1];

//	tFraN	= tFraN0A[33: 9] + { 16'h0, tMulLoC0[9:1] };

//	tFraN0	= { 25'h0, tFraS } * { 25'h0, tFraT };
//	tFraN	= tFraN0[47:23];
`else
	tFraN0	= { 18'h0, tFraS } * { 18'h0, tFraT };
	tFraN	= tFraN0[33:16];
`endif
	
	/* Clock Edge */

//	tExpIsZero = 0;
	tExpIsZero = tExpIsZero1;

	if((tExpN2[8:7]==2'b11) || (tExpN2[7:0]==0))
//	if((tExpN2[8]==1'b1) || (tExpN2[7:0]==0))
		tExpIsZero = 1;

`ifdef jx2_fpu_v4sf_fullsp
	if(tFraN2[24])
	begin
		tExpN2b = tExpN2p1;
		tFraN2b = { 1'b0, tFraN2[24:1] };
	end
	else
	begin
		tExpN2b = tExpN2;
		tFraN2b = tFraN2[24:0];
	end
	
	tRegValRo = { tSgN2, tExpN2b[7:0], tFraN2b[22:0] };
`else
	if(tFraN2[17])
	begin
		tExpN2b = tExpN2p1;
		tFraN2b = { 1'b0, tFraN2[17:1] };
	end
	else
	begin
		tExpN2b = tExpN2;
		tFraN2b = tFraN2[17:0];
	end
	
	tRegValRo = { tSgN2, tExpN2b[7:0], tFraN2b[15:0], 7'b0 };
`endif

	if(tExpIsZero)
	begin
		tRegValRo = 0;
	end

end

always @(posedge clock)
begin
	if(exHoldN)
	begin
		tSgN2		<= tSgN;
		tExpN2		<= tExpN;
		tFraN2		<= tFraN;
		tExpN2p1	<= tExpNp1;
		tExpIsZero1	<= tExpIsZero0;
		
		tRegValRo2	<= tRegValRo;
	end
end

endmodule

`endif
