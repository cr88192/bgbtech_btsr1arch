/*
Multiplier, Extended Precision
Uses S.15.80 as its format.
This format is a truncated version of the Binary128 format.

The output from this unit is not rounded.

 */

`include "CoreDefs.v"
`include "ExCsAdd90F.v"

module FpuMulE(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRm,
	regValRn,
	regValRo
	);

input	clock;
input	reset;
input	exHold;

input[95:0]		regValRm;
input[95:0]		regValRn;
output[95:0]	regValRo;

reg[95:0]		tRegValRo;
assign	regValRo	= tRegValRo;

reg				tSgnA1;
reg				tSgnB1;
reg				tSgnC1;
reg[14:0]		tExpA1;
reg[14:0]		tExpB1;
reg[16:0]		tExpC1;
reg[79:0]		tFraA1;
reg[79:0]		tFraB1;

reg[35:0]		tFraC1_BC;
reg[35:0]		tFraC1_CB;
reg[35:0]		tFraC1_CC;
reg[35:0]		tFraC1_AD;
reg[35:0]		tFraC1_BD;
reg[35:0]		tFraC1_CD;
reg[35:0]		tFraC1_DA;
reg[35:0]		tFraC1_DB;
reg[35:0]		tFraC1_DC;
reg[35:0]		tFraC1_DD;


reg				tSgnC2;
reg[16:0]		tExpC2;

reg[35:0]		tFraC2_BC;
reg[35:0]		tFraC2_CB;
reg[35:0]		tFraC2_CC;
reg[35:0]		tFraC2_AD;
reg[35:0]		tFraC2_BD;
reg[35:0]		tFraC2_CD;
reg[35:0]		tFraC2_DA;
reg[35:0]		tFraC2_DB;
reg[35:0]		tFraC2_DC;
reg[35:0]		tFraC2_DD;

reg[89:0]		tFraC2_P;
reg[89:0]		tFraC2_Q;
reg[89:0]		tFraC2_R;


reg				tSgnC3;
reg[16:0]		tExpC3;

reg[89:0]		tFraC3_P;
reg[89:0]		tFraC3_Q;
reg[89:0]		tFraC3_R;

wire[89:0]		tFraC3_S;
ExCsAdd90F		fpmulAdd(tFraC3_Q, tFraC3_R, tFraC3_S);

reg				tSgnC4;
reg[16:0]		tExpC4;
reg[89:0]		tFraC4_S;

reg				tSgnC4B;
reg[16:0]		tExpC4B;
reg[79:0]		tFraC4B;

reg[95:0]		tValC4;

/*
    AA
   AB
   BA
  AC
  CA
  BB
 BC
 CB
CC
RQP
 */

always @*
begin
	/* Stage 1 */
	tSgnA1	= regValRn[95];
	tSgnB1	= regValRm[95];
	tExpA1	= regValRn[94:80];
	tExpB1	= regValRm[94:80];
	tFraA1	= {1'b1, regValRn[79:1]};
	tFraB1	= {1'b1, regValRm[79:1]};
	
	tSgnC1	= tSgnA1 ^ tSgnB1;
	tExpC1	=
		{2'b00, tExpA1} +
		{2'b00, tExpB1} - 16383;
	
	if((tExpA1==0) || (tExpB1==0))
		tExpC1	= 0;

//	$display("FpuMul: Exp %X %X %X", tExpA1, tExpB1, tExpC1);

	tFraC1_CB	=	{ 18'h0, tFraA1[61:44]} * { 18'h0, tFraB1[43:26]};
	tFraC1_BC	=	{ 18'h0, tFraA1[43:26]} * { 18'h0, tFraB1[61:44]};
	tFraC1_CC	=	{ 18'h0, tFraA1[61:44]} * { 18'h0, tFraB1[61:44]};
	tFraC1_AD	=	{ 18'h0, tFraA1[25: 8]} * { 18'h0, tFraB1[79:62]};
	tFraC1_BD	=	{ 18'h0, tFraA1[43:26]} * { 18'h0, tFraB1[79:62]};
	tFraC1_CD	=	{ 18'h0, tFraA1[61:44]} * { 18'h0, tFraB1[79:62]};
	tFraC1_DA	=	{ 18'h0, tFraA1[79:62]} * { 18'h0, tFraB1[25: 8]};
	tFraC1_DB	=	{ 18'h0, tFraA1[79:62]} * { 18'h0, tFraB1[43:26]};
	tFraC1_DC	=	{ 18'h0, tFraA1[79:62]} * { 18'h0, tFraB1[61:44]};
	tFraC1_DD	=	{ 18'h0, tFraA1[79:62]} * { 18'h0, tFraB1[79:62]};

	/* Stage 2 */

	tFraC2_P =
		{18'h0, 36'h0    , tFraC2_AD } +
		{18'h0, 36'h0    , tFraC2_DA };
	tFraC2_Q =
		{18'h0, tFraC2_CD, tFraC2_BC } +
		{18'h0, tFraC2_DC, tFraC2_CB } +
		tFraC2_P;
	tFraC2_R =
		{tFraC2_DD, tFraC2_CC, 18'h0} +
		{36'h0    , tFraC2_BD, 18'h0} +
		{36'h0    , tFraC2_DB, 18'h0};


	/* Stage 3 */


	/* Stage 4 */

	if(tExpC4[16] || (tExpC4==0))
	begin
		tSgnC4B = 0;
		tExpC4B = 0;
		tFraC4B = 0;
	end
	else
	if(tExpC4[15] || (tExpC4==32767))
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = 2047;
		tFraC4B = 0;
	end
	else
	if(tFraC4_S[89])
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4+1;
		tFraC4B = tFraC4_S[88:9];
	end
	else
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4;
		tFraC4B = tFraC4_S[87:8];
	end
	
//	$display("FpuMul: ExpB %X %X", tExpC4, tExpC4B);

	tValC4 = { tSgnC4B, tExpC4B[14:0], tFraC4B[79:0] };
	
//	$display("FpuMul: Val=%X", tValC4);
	
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		tSgnC2		<= tSgnC1;
		tExpC2		<= tExpC1;

		tFraC2_BC	<= tFraC1_BC;
		tFraC2_CB	<= tFraC1_CB;
		tFraC2_CC	<= tFraC1_CC;
		tFraC2_AD	<= tFraC1_AD;
		tFraC2_BD	<= tFraC1_BD;
		tFraC2_CD	<= tFraC1_CD;
		tFraC2_DA	<= tFraC1_DA;
		tFraC2_DB	<= tFraC1_DB;
		tFraC2_DC	<= tFraC1_DC;
		tFraC2_DD	<= tFraC1_DD;
	end

	tSgnC3		<= tSgnC2;
	tExpC3		<= tExpC2;
	tFraC3_P	<= tFraC2_P;
	tFraC3_Q	<= tFraC2_Q;
	tFraC3_R	<= tFraC2_R;

	tSgnC4		<= tSgnC3;
	tExpC4		<= tExpC3;
	tFraC4_S	<= tFraC3_S;

	tRegValRo	<= tValC4;
end

endmodule
