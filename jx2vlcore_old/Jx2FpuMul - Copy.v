
`include "Jx2CoreDefs.v"

module Jx2FpuMul(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regValRm,
	regValRn,
	regValRo,
	regExOp,
	regExOK
	);

input	clock;
input	reset;

input[63:0]		regValRm;
input[63:0]		regValRn;
output[63:0]	regValRo;
input[1:0]		regExOp;
output[1:0]		regExOK;

reg[63:0]		tRegValRo;
reg[1:0]		tRegExOK;

assign	regValRo	= tRegValRo;
assign	regExOK		= tRegExOK;


wire[63:0]		regValRmRcp;
Jx2FpuRcpA	mrcp(
	regValRm, regValRmRcp);

reg[63:0]		regValRmRcp1;


reg				tSgnA1;
reg				tSgnB1;
reg				tSgnC1;
reg[10:0]		tExpA1;
reg[10:0]		tExpB1;
reg[11:0]		tExpC1;
reg[53:0]		tFraA1;
reg[53:0]		tFraB1;

reg[35:0]		tFraC1_AA;
reg[35:0]		tFraC1_AB;
reg[35:0]		tFraC1_AC;
reg[35:0]		tFraC1_BA;
reg[35:0]		tFraC1_BB;
reg[35:0]		tFraC1_BC;
reg[35:0]		tFraC1_CA;
reg[35:0]		tFraC1_CB;
reg[35:0]		tFraC1_CC;


reg				tSgnC2;
reg[11:0]		tExpC2;

reg[35:0]		tFraC2_AA;
reg[35:0]		tFraC2_AB;
reg[35:0]		tFraC2_AC;
reg[35:0]		tFraC2_BA;
reg[35:0]		tFraC2_BB;
reg[35:0]		tFraC2_BC;
reg[35:0]		tFraC2_CA;
reg[35:0]		tFraC2_CB;
reg[35:0]		tFraC2_CC;

reg[57:0]		tFraC2_P;
reg[57:0]		tFraC2_Q;
reg[57:0]		tFraC2_R;


reg				tSgnC3;
reg[11:0]		tExpC3;

reg[57:0]		tFraC3_P;
reg[57:0]		tFraC3_Q;
reg[57:0]		tFraC3_R;

reg[57:0]		tFraC3_S;

reg				tSgnC4;
reg[11:0]		tExpC4;
reg[57:0]		tFraC4_S;

reg				tSgnC4B;
reg[11:0]		tExpC4B;
reg[53:0]		tFraC4B;

reg[63:0]		tValC4;

reg[1:0]		tExEn1;
reg[1:0]		tExEn2;
reg[1:0]		tExEn3;
reg[1:0]		tExEn4;
reg[1:0]		tExEn5;
reg[1:0]		tExEn6;

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
//	tExEn1	= regExOp != 0;
	tExEn1	= regExOp;

	tRegExOK=UMEM_OK_READY;
//	if(tExEn1)
//	if(tExEn1 && (regExOp != 3))
	if(tExEn1!=0)
	begin
//		if(tExEn5)
//		if(tExEn6 || (tExEn5 && !regExOp[1]))
//		if(tExEn6== || (tExEn5 && !regExOp[1]))
		if(tExEn5==regExOp)
//		if(tExEn6==regExOp)
			tRegExOK=UMEM_OK_OK;
		else
			tRegExOK=UMEM_OK_HOLD;
	end
	
	
	/* Stage 1 */
	tSgnA1	= regValRn[63];
	tSgnB1	= regValRm[63];
	tExpA1	= regValRn[62:52];
//	tExpB1	= regValRm[62:52];
	tFraA1	= {2'b01, regValRn[51:0]};
//	tFraB1	= {2'b01, regValRm[51:0]};
	
//	if(regExOp[1])
	if(regExOp==2)
	begin
//		tExpB1	= 2045-regValRm[62:52];
//		tFraB1	= {2'b01, ~regValRm[51:0]};
		tExpB1	= regValRmRcp1[62:52];
		tFraB1	= {2'b01, regValRmRcp1[51:0]};
	end
	else
	begin
		tExpB1	= regValRm[62:52];
		tFraB1	= {2'b01, regValRm[51:0]};
	end
	
	tSgnC1	= tSgnA1 ^ tSgnB1;
	tExpC1	=
		{1'b0, tExpA1} +
		{1'b0, tExpB1} - 1023;

	tFraC1_AA	=
		{ 18'h0, tFraA1[17: 0]} *
		{ 18'h0, tFraB1[17: 0]};
	tFraC1_AB	=
		{ 18'h0, tFraA1[17: 0]} *
		{ 18'h0, tFraB1[35:18]};
	tFraC1_AC	=
		{ 18'h0, tFraA1[17: 0]} *
		{ 18'h0, tFraB1[53:36]};

	tFraC1_BA	=
		{ 18'h0, tFraA1[35:18]} *
		{ 18'h0, tFraB1[17: 0]};
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


	/* Stage 2 */

	tFraC2_P =
		{36'h0, tFraC2_AC[35:14]} +
		{36'h0, tFraC2_CA[35:14]};
	tFraC2_Q =
		{18'h0, tFraC2_BC, 4'h0} +
		{18'h0, tFraC2_CB, 4'h0} ;
	tFraC2_R =
		{36'h0, tFraC2_BB[35:14]} +
		{tFraC2_CC, 22'h0} ;


	/* Stage 3 */

//	tFraC3_S =
//		tFraC3_P +
//		tFraC3_Q +
//		tFraC3_R ;
	tFraC3_S =
		tFraC3_P +
		tFraC3_Q +
		tFraC3_R +
		(tFraC3_R[55]?3:2);


	/* Stage 4 */

	if(tExpC4[11])
	begin
		tSgnC4B = 0;
		tExpC4B = 0;
		tFraC4B = 0;
	end
	else
	if(tFraC4_S[55])
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4+1;
		tFraC4B = tFraC4_S[56:3];
//		tFraC4B = (tFraC4_S+3)[56:3];
	end
	else
	begin
		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4;
		tFraC4B = tFraC4_S[55:2];
	end
	
	tValC4 = { tSgnC4B, tExpC4B[10:0], tFraC4B[51:0] };
	
end

always @(posedge clock)
begin

//	if(tExEn1)
	if(tExEn1!=0)
	begin
		regValRmRcp1	<= regValRmRcp;
	
		tSgnC2		<= tSgnC1;
		tExpC2		<= tExpC1;

		tFraC2_AA	<= tFraC1_AA;
		tFraC2_AB	<= tFraC1_AB;
		tFraC2_AC	<= tFraC1_AC;
		tFraC2_BA	<= tFraC1_BA;
		tFraC2_BB	<= tFraC1_BB;
		tFraC2_BC	<= tFraC1_BC;
		tFraC2_CA	<= tFraC1_CA;
		tFraC2_CB	<= tFraC1_CB;
		tFraC2_CC	<= tFraC1_CC;

		tSgnC3		<= tSgnC2;
		tExpC3		<= tExpC2;
		tFraC3_P	<= tFraC2_P;
		tFraC3_Q	<= tFraC2_Q;
		tFraC3_R	<= tFraC2_R;

		tSgnC4		<= tSgnC3;
		tExpC4		<= tExpC3;
		tFraC4_S	<= tFraC3_S;

		tExEn2		<= tExEn1;
		tExEn3		<= tExEn2;
		tExEn4		<= tExEn3;
		tExEn5		<= tExEn4;
		tExEn6		<= tExEn5;

		tRegValRo	<= tValC4;
	end
	else
	begin
		tExEn2		<= 0;
		tExEn3		<= 0;
		tExEn4		<= 0;
		tExEn5		<= 0;
		tExEn6		<= 0;
		tRegValRo	<= UV64_XX;
	end
end

endmodule
