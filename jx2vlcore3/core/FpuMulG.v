/*
Multiplier, Extended Precision (64/128)

Uses S.15.80 as its nominal format.

Accepts and produces either a Binary64 or Binary128.
Binary128 is partial-precision.

ExOp(2:0): 
	0: No Op		(Ignore)
	1: Ro=Rn+Rm		(Ignore)
	2: Ro=Rn-Rm		(Ignore)
	3: Ro=I2F(Rn)	(Ignore)
ExOp(3):
	0: Double
	1: Long Double

 */

`include "CoreDefs.v"
// `include "ExCsAdd90F.v"
`include "ExCsAdd90C.v"

module FpuMulG(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRmLo,	regValRmHi,
	regValRnLo,	regValRnHi,
	regValRoLo,	regValRoHi,
	regExOp,	regRMode
	);

input	clock;
input	reset;
input	exHold;

input[63:0]		regValRmLo;
input[63:0]		regValRmHi;
input[63:0]		regValRnLo;
input[63:0]		regValRnHi;
output[63:0]	regValRoLo;
output[63:0]	regValRoHi;
input[3:0]		regExOp;
input[7:0]		regRMode;

reg[95:0]		tRegValRmE;
reg[95:0]		tRegValRnE;
reg[95:0]		tRegValRoE;

reg[63:0]		tRegValRoLo;
reg[63:0]		tRegValRoHi;
assign	regValRoLo	= tRegValRoLo;
assign	regValRoHi	= tRegValRoHi;

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
// wire[90:0]		tFraC3_S;
wire			tFraC3_Co;
// ExCsAdd90F		fpmulAdd(tFraC3_Q, tFraC3_R, tFraC3_S);
ExCsAdd90C		fpmulAdd(tFraC3_Q, tFraC3_R, {tFraC3_Co, tFraC3_S}, 1'b1);

reg				tSgnC4;
reg[16:0]		tExpC4;
reg[89:0]		tFraC4_S;

reg				tSgnC4B;
reg[16:0]		tExpC4B;
reg[79:0]		tFraC4B;

reg				tSgnC4D;
reg[10:0]		tExpC4D;
reg[51:0]		tFraC4D;

reg[95:0]		tValC4;
reg[63:0]		tValC4D;

reg[3:0]		tRegExOp1;
reg[3:0]		tRegExOp2;
reg[3:0]		tRegExOp3;
reg[3:0]		tRegExOp4;

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

reg				tExpRmIsZero;
reg				tExpRnIsZero;
reg				tExpRmIsNaN;
reg				tExpRnIsNaN;
reg[14:0]		tExpRmExt;
reg[14:0]		tExpRnExt;
reg				tAdderRoundBit;

reg				tInfC4B;
reg				tZeroC4B;
reg				tInfC4D;
reg				tZeroC4D;
reg				tInfC4;
reg				tZeroC4;

always @*
begin
	tRegValRmE		= { regValRmHi, regValRmLo[63:32] };
	tRegValRnE		= { regValRnHi, regValRnLo[63:32] };
//	tRegValRoE		= 0;
	tRegExOp1		= regExOp;

	tExpRmIsZero	= (regValRmLo[62:52]==11'h000);
	tExpRnIsZero	= (regValRnLo[62:52]==11'h000);
	tExpRmIsNaN		= (regValRmLo[62:52]==11'h7FF);
	tExpRnIsNaN		= (regValRnLo[62:52]==11'h7FF);

	tExpRmExt		= {
		regValRmLo[62],
		((!regValRmLo[62] && !tExpRmIsZero) ||
			tExpRmIsNaN) ? 4'b1111 : 4'b0000,
		regValRmLo[61:52]
		};
	tExpRnExt		= {
		regValRnLo[62],
		((!regValRnLo[62] && !tExpRnIsZero) ||
			tExpRnIsNaN) ? 4'b1111 : 4'b0000,
		regValRnLo[61:52]
		};

	/* Stage 1 */
	if(tRegExOp1[3])
	begin
		tSgnA1	= tRegValRnE[95];
		tSgnB1	= tRegValRmE[95];
		tExpA1	= tRegValRnE[94:80];
		tExpB1	= tRegValRmE[94:80];
		tFraA1	= {1'b1, tRegValRnE[79:1]};
		tFraB1	= {1'b1, tRegValRmE[79:1]};
	end
	else
	begin
		tSgnA1	= regValRnLo[63];
		tSgnB1	= regValRmLo[63];
		tExpA1	= tExpRnExt;
		tExpB1	= tExpRmExt;
		tFraA1	= { 1'b1, regValRnLo[51:0], 27'h0 };
		tFraB1	= { 1'b1, regValRmLo[51:0], 27'h0 };
	end
	
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

	tAdderRoundBit = !tRegExOp4[3];

//	tFraC2_P =
//		{18'h0, 36'h0    , tFraC2_AD } +
//		{18'h0, 36'h0    , tFraC2_DA } +
//		{18'h0, 36'h0    , tAdderRoundBit, 35'h0 };

	tFraC2_P =
		{ 52'h0,
			{2'h0    , tFraC2_AD } +
			{2'h0    , tFraC2_DA } +
			{2'h0    , tAdderRoundBit, 35'h0 } };

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

	tSgnC4B = tSgnC4;
	tFraC4B = 0;

	tZeroC4B	= tExpC4[16] || (tExpC4==0);
	tInfC4B		= tExpC4[15] || (tExpC4==32767);
	tZeroC4D	= tZeroC4B	|| (!tExpC4[14] && tExpC4[13:10]!=4'b1111);
	tInfC4D		= tInfC4B	|| ( tExpC4[14] && tExpC4[13:10]!=4'b0000);
	tZeroC4		= tRegExOp4[3] ? tZeroC4B : tZeroC4D;
	tInfC4		= tRegExOp4[3] ? tInfC4B  : tInfC4D;

	if(tZeroC4)
	begin
		tSgnC4B = 0;
		tExpC4B = 0;
//		tFraC4B = 0;
	end
	else
	if(tInfC4)
	begin
//		tSgnC4B = tSgnC4;
//		tExpC4B = 2047;
		tExpC4B = 32767;
//		tFraC4B = 0;
	end
	else
	if(tFraC4_S[89])
	begin
//		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4+1;
		tFraC4B = tFraC4_S[88:9];
	end
	else
	begin
//		tSgnC4B = tSgnC4;
		tExpC4B = tExpC4;
		tFraC4B = tFraC4_S[87:8];
	end
	
	tSgnC4D = tSgnC4;
	tExpC4D = { tExpC4B[14], tExpC4B[9:0] };
	tFraC4D = tFraC4B[79:28];
	
//	$display("FpuMul: ExpB %X %X", tExpC4, tExpC4B);

	tValC4	= { tSgnC4B, tExpC4B[14:0], tFraC4B[79:0] };
	tValC4D	= { tSgnC4D, tExpC4D[10:0], tFraC4D[51:0] };
	
//	$display("FpuMul: Val=%X", tValC4);
	
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		tSgnC2		<= tSgnC1;
		tExpC2		<= tExpC1;
		tRegExOp2	<= tRegExOp1;

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
	tRegExOp3	<= tRegExOp2;

	tSgnC4		<= tSgnC3;
	tExpC4		<= tExpC3;
	tFraC4_S	<= tFraC3_S;
	tRegExOp4	<= tRegExOp3;

	tRegValRoHi	<= tValC4[95:32];
	tRegValRoLo	<= tRegExOp4[3] ? { tValC4[31:0], 32'h0 } : tValC4D;
end

endmodule
