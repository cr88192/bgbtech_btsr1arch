
`include "CoreDefs.v"
`include "ExCsAdd64F.v"

module FpuMul(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regValRm,
	regValRn,
	regValRo
	);

input	clock;
input	reset;

input[63:0]		regValRm;
input[63:0]		regValRn;
output[63:0]	regValRo;

reg[63:0]		tRegValRo;
assign	regValRo	= tRegValRo;

reg				tSgnA1;
reg				tSgnB1;
reg				tSgnC1;
reg[10:0]		tExpA1;
reg[10:0]		tExpB1;
reg[11:0]		tExpC1;
reg[53:0]		tFraA1;
reg[53:0]		tFraB1;

reg[35:0]		tFraC1_AC;
reg[35:0]		tFraC1_BB;
reg[35:0]		tFraC1_BC;
reg[35:0]		tFraC1_CA;
reg[35:0]		tFraC1_CB;
reg[35:0]		tFraC1_CC;

reg				tFraC1_AB_Co;
reg				tFraC1_BA_Co;
reg[9:0]		tFraC1_AB_C5;
reg[9:0]		tFraC1_BA_C5;


reg				tSgnC2;
reg[11:0]		tExpC2;

reg[35:0]		tFraC2_AC;
reg[35:0]		tFraC2_BB;
reg[35:0]		tFraC2_BC;
reg[35:0]		tFraC2_CA;
reg[35:0]		tFraC2_CB;
reg[35:0]		tFraC2_CC;

reg				tFraC2_AB_Co;
reg				tFraC2_BA_Co;
reg[9:0]		tFraC2_AB_C5;
reg[9:0]		tFraC2_BA_C5;

reg[63:0]		tFraC2_P;
reg[63:0]		tFraC2_Q;
reg[63:0]		tFraC2_R;


reg				tSgnC3;
reg[11:0]		tExpC3;

reg[63:0]		tFraC3_P;
reg[63:0]		tFraC3_Q;
reg[63:0]		tFraC3_R;

wire[63:0]		tFraC3_S;
ExCsAdd64F		fpmulAdd(tFraC3_Q, tFraC3_R, tFraC3_S);

reg				tSgnC4;
reg[11:0]		tExpC4;
reg[63:0]		tFraC4_S;

reg				tSgnC4B;
reg[11:0]		tExpC4B;
reg[53:0]		tFraC4B;

reg				tFraRbit4B;
reg[8:0]		tFraRnd4B;

reg[63:0]		tValC4;

//reg[1:0]		tExEn1;
//reg[1:0]		tExEn2;
//reg[1:0]		tExEn3;
//reg[1:0]		tExEn4;
//reg[1:0]		tExEn5;
//reg[1:0]		tExEn6;

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
	tSgnA1	= regValRn[63];
	tSgnB1	= regValRm[63];
	tExpA1	= regValRn[62:52];
	tExpB1	= regValRm[62:52];
//	tFraA1	= {1'b0, (tExpA1!=0), regValRn[51:0]};
//	tFraB1	= {1'b0, (tExpB1!=0), regValRm[51:0]};
	tFraA1	= {2'b01, regValRn[51:0]};
	tFraB1	= {2'b01, regValRm[51:0]};
	
	tSgnC1	= tSgnA1 ^ tSgnB1;
	tExpC1	=
		{1'b0, tExpA1} +
		{1'b0, tExpB1} - 1023;
	
	if((tExpA1==0) || (tExpB1==0))
		tExpC1	= 0;


//	$display("FpuMul: Exp %X %X %X", tExpA1, tExpB1, tExpC1);

	tFraC1_AC	=
		{ 18'h0, tFraA1[17: 0]} *
		{ 18'h0, tFraB1[53:36]};

	tFraC1_BB	=
		{ 18'h0, tFraA1[35:18]} *
		{ 18'h0, tFraB1[35:18]};
	tFraC1_BC	=
		{ 18'h0, tFraA1[35:18]} *
		{ 18'h0, tFraB1[53:36]};

	tFraC1_CA	=
		{ 18'h0, tFraA1[53:36]} *
		{ 18'h0, tFraB1[17: 0]};
	tFraC1_CB	=
		{ 18'h0, tFraA1[53:36]} *
		{ 18'h0, tFraB1[35:18]};
	tFraC1_CC	=
		{ 18'h0, tFraA1[53:36]} *
		{ 18'h0, tFraB1[53:36]};

	tFraC1_AB_Co = tFraA1[17] & tFraB1[35];
	tFraC1_BA_Co = tFraA1[35] & tFraB1[17];
	tFraC1_AB_C5 = { 5'h0, tFraA1[17:13] } * { 5'h0, tFraB1[35:31] };
	tFraC1_BA_C5 = { 5'h0, tFraA1[35:31] } * { 5'h0, tFraB1[17:13] };


	/* Stage 2 */

	tFraC2_P =
		{36'h0, tFraC2_AC[35:8]} +
		{36'h0, tFraC2_CA[35:8]};
	tFraC2_Q =
		{18'h0, tFraC2_BC, tFraC1_AB_C5} +
		{18'h0, tFraC2_CB, tFraC1_BA_C5} ;
	tFraC2_R =
		{tFraC2_CC, tFraC2_BB[35:8]} +
		{36'h0, tFraC2_AC[35:8]} +
		{36'h0, tFraC2_CA[35:8]};


	/* Stage 3 */


	/* Stage 4 */

//	if(tExpC4[11])
	if(tExpC4[11] || (tExpC4==0))
	begin
		tSgnC4B = 0;
		tExpC4B = 0;
		tFraC4B = 0;
		tFraRbit4B	= 0;
	end
	else
	if(tFraC4_S[61])
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4+1;
//		tFraC4B = tFraC4_S[62:9] +
//			(tFraC4_S[8]?54'h1:54'h0);
		tFraC4B = tFraC4_S[62:9];
		tFraRbit4B	= tFraC4_S[8];
	end
	else
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4;
//		tFraC4B = tFraC4_S[61:8] +
//			(tFraC4_S[7]?54'h1:54'h0);
		tFraC4B = tFraC4_S[61:8];
		tFraRbit4B	= tFraC4_S[7];
	end
	
	tFraRnd4B = { 1'b0, tFraC4B[7:0] } + { 8'b0, tFraRbit4B };
	if(!tFraRnd4B[8])
		tFraC4B[7:0] = tFraRnd4B[7:0];

//	$display("FpuMul: ExpB %X %X", tExpC4, tExpC4B);

	tValC4 = { tSgnC4B, tExpC4B[10:0], tFraC4B[51:0] };
	
//	$display("FpuMul: Val=%X", tValC4);
	
end

always @(posedge clock)
begin
	tSgnC2		<= tSgnC1;
	tExpC2		<= tExpC1;

	tFraC2_AC	<= tFraC1_AC;
	tFraC2_BB	<= tFraC1_BB;
	tFraC2_BC	<= tFraC1_BC;
	tFraC2_CA	<= tFraC1_CA;
	tFraC2_CB	<= tFraC1_CB;
	tFraC2_CC	<= tFraC1_CC;

	tFraC2_AB_Co	<= tFraC1_AB_Co;
	tFraC2_BA_Co	<= tFraC1_BA_Co;
	tFraC2_AB_C5	<= tFraC1_AB_C5;
	tFraC2_BA_C5	<= tFraC1_BA_C5;

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
