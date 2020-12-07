`include "CoreDefs.v"
`include "ExCsAdd80F.v"

module Fp2Mul(
	/* verilator lint_off UNUSED */
	clock,			reset,				exHold,
	regSgnS,		regExpS,			regFraS,
	regSgnT,		regExpT,			regFraT,
	regSgnN,		regExpN,			regFraN
	);

input			clock;
input			reset;
input			exHold;

input			regSgnS;
input[15:0]		regExpS;
input[71:0]		regFraS;

input			regSgnT;
input[15:0]		regExpT;
input[71:0]		regFraT;

output			regSgnN;
output[15:0]	regExpN;
output[71:0]	regFraN;

/*
   AD
   BC
   CB
   DA
  BD
  CC
  DB
 CD
 DC
DD
 */

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

reg[89:0]		tFraC3_P;
reg[89:0]		tFraC3_Q;
reg[89:0]		tFraC3_R;

wire[79:0]		tFraC3_S;
ExCsAdd80F		fpmulAdd(tFraC3_Q[89:10], tFraC3_R[89:10], tFraC3_S);

reg				tSgnS1;
reg[15:0]		tExpS1;
reg[71:0]		tFraS1;

reg				tSgnT1;
reg[15:0]		tExpT1;
reg[71:0]		tFraT1;

reg				tSgnC1;
reg				tSgnC2;
reg				tSgnC3;
reg				tSgnC4;

reg[16:0]		tExpC1;
reg[16:0]		tExpC2;
reg[16:0]		tExpC3;
reg[16:0]		tExpC4;

reg[79:0]		tFraC4_S;

reg				tSgnC4B;
reg[15:0]		tExpC4B;
reg[71:0]		tFraC4B;

assign		regSgnN = tSgnC4B;
assign		regExpN = tExpC4B;
assign		regFraN = tFraC4B;


always @*
begin
	/* Stage 1 */

	tSgnS1	= regSgnS;
	tExpS1	= regExpS;
	tFraS1	= regFraS;

	tSgnT1	= regSgnT;
	tExpT1	= regExpT;
	tFraT1	= regFraT;
	
	tSgnC1	= tSgnS1 ^ tSgnT1;
	tExpC1	=
		{1'b0, tExpS1} +
		{1'b0, tExpT1} - 16383;
	
	if((tExpS1==0) || (tExpT1==0))
		tExpC1	= 0;

//	$display("FpuMul: Exp %X %X %X", tExpA1, tExpB1, tExpC1);

	tFraC1_CB	=	{ 18'h0, tFraS1[53:36]} * { 18'h0, tFraT1[35:18]};
	tFraC1_BC	=	{ 18'h0, tFraS1[35:18]} * { 18'h0, tFraT1[53:36]};
	tFraC1_CC	=	{ 18'h0, tFraS1[53:36]} * { 18'h0, tFraT1[53:36]};
	tFraC1_AD	=	{ 18'h0, tFraS1[17: 0]} * { 18'h0, tFraT1[71:54]};
	tFraC1_BD	=	{ 18'h0, tFraS1[35:18]} * { 18'h0, tFraT1[71:54]};
	tFraC1_CD	=	{ 18'h0, tFraS1[53:36]} * { 18'h0, tFraT1[71:54]};
	tFraC1_DA	=	{ 18'h0, tFraS1[71:54]} * { 18'h0, tFraT1[17: 0]};
	tFraC1_DB	=	{ 18'h0, tFraS1[71:54]} * { 18'h0, tFraT1[35:18]};
	tFraC1_DC	=	{ 18'h0, tFraS1[71:54]} * { 18'h0, tFraT1[53:36]};
	tFraC1_DD	=	{ 18'h0, tFraS1[71:54]} * { 18'h0, tFraT1[71:54]};

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

	tSgnC4B		= tSgnC4;
	tExpC4B		= tExpC4  [15:0];
	tFraC4B		= tFraC4_S[79:8];
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		/* Mul 1->2 */
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

		/* Mul 2->3 */

		tSgnC3		<= tSgnC2;
		tExpC3		<= tExpC2;
		tFraC3_P	<= tFraC2_P;
		tFraC3_Q	<= tFraC2_Q;
		tFraC3_R	<= tFraC2_R;

		/* Mul 3->4 */

		tSgnC4		<= tSgnC3;
		tExpC4		<= tExpC3;
		tFraC4_S	<= tFraC3_S;
	end

end

endmodule

