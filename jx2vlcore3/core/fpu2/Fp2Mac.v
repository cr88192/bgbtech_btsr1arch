
`include "CoreDefs.v"
`include "ExCsAdd64F.v"

`include "Fp2Mul.v"

module Fp2Mac(
	/* verilator lint_off UNUSED */
	clock,		reset,
	exHold,
	regValRs,
	regValRt,
	regValRu,
	regValRm,
	regValRn,
	regValPos,
	regValPosDst
	);

input	clock;
input	reset;
input	exHold;

input[127:0]		regValRs;
input[127:0]		regValRt;
input[127:0]		regValRu;
input[127:0]		regValRm;		//input dest vector
output[127:0]		regValRn;		//output dest vector
input[5:0]			regValPos;		//Vector Position (Source)
input[5:0]			regValPosDst;	//Vector Position (Destination)

reg[127:0]		tRegValRn;
assign	regValRn	= tRegValRn;

wire			tSgnS1;
wire			tSgnT1;
wire			tSgnU1;
wire[15:0]		tExpS1;
wire[15:0]		tExpT1;
wire[15:0]		tExpU1;
wire[71:0]		tFraS1;
wire[71:0]		tFraT1;
wire[71:0]		tFraU1;

reg[16:0]		tExpC1;

reg				tSgnC2;
reg[16:0]		tExpC2;

reg				tSgnU2;
reg				tSgnU3;
reg				tSgnU4;
reg[15:0]		tExpU2;
reg[15:0]		tExpU3;
reg[15:0]		tExpU4;
reg[71:0]		tFraU2;
reg[71:0]		tFraU3;
reg[71:0]		tFraU4;

reg				tSgnV4;
reg[15:0]		tExpV4;
reg[71:0]		tFraV4;

reg[15:0]		tAddExpU;
reg[15:0]		tAddExpV;
reg[71:0]		tAddFraU;
reg[71:0]		tAddFraV;
reg[15:0]		tAddExpC;

reg[15:0]		tAddExpU1;
reg[15:0]		tAddExpV1;
reg[15:0]		tAddExpC1;

reg[15:0]		tAddExpU1B;
reg[15:0]		tAddExpV1B;
reg[15:0]		tAddExpC1B;

reg[15:0]		tAddExpU2;
reg[15:0]		tAddExpV2;
reg[15:0]		tAddExpC2;

reg[15:0]		tAddExpU2B;
reg[15:0]		tAddExpV2B;
reg[15:0]		tAddExpC2B;

reg				tSgnC3;
reg[16:0]		tExpC3;

reg[5:0]		tPosDst1;	//Vector Position (Destination)
reg[5:0]		tPosDst2;	//Vector Position (Destination)
reg[5:0]		tPosDst3;	//Vector Position (Destination)
reg[5:0]		tPosDst4;	//Vector Position (Destination)



// reg				tFraRbit4B;
// reg[8:0]		tFraRnd4B;
// reg[63:0]		tValC4;

reg[71:0]		tAddFraU1;
reg[71:0]		tAddFraV1;

reg[71:0]		tAddFraU1B;
reg[71:0]		tAddFraV1B;

reg[71:0]		tAddFraU2;
reg[71:0]		tAddFraV2;
wire[71:0]		tAddFraC2B;
ExCsAdd72F		fpAdd(tAddFraU2, tAddFraV2, tAddFraC2);

reg[15:0]		tAddExpC3;
wire[71:0]		tAddFraC3;

Fp2Unpack fpUpS(
	clock,		reset, 
	regValRs,	regValPos,
	tSgnS1,		tExpS1,		tFraS1);
Fp2Unpack fpUpT(
	clock,		reset, 
	regValRt,	regValPos,
	tSgnT1,		tExpT1,		tFraT1);
Fp2Unpack fpUpU(
	clock,		reset, 
	regValRu,	regValPos,
	tSgnU1,		tExpU1,		tFraU1);

reg[5:0]	tPackPosDst;
reg			tPackSign;
reg[15:0]	tPackExp;
reg[71:0]	tPackFrac;

Fp2Pack fpPackN(
	clock,		reset,
	regValRm,	regValRn,	tPackPosDst,
	tPackSign,	tPackExp,	tPackFrac);
	

always @*
begin

	tAddExpU	= tExpU4;
	tAddFraU	= (tSgnU4 ^ tOpIsSub) ? (~tFraU4) : tFraU4;

	tAddExpV	= tExpC4;
	tAddFraV	= (tSgnC4 ^ tOpIsRSub) ? (~tFraC4_S) : tFraC4_S;
	tAddExpC	= (tAddExpU>tAddExpV) ? tAddExpU : tAddExpV;

	/* FADD, Stage 1 */

	tAddExpU1B	= tAddExpU1;
	tAddExpV1B	= tAddExpV1;
	tAddExpC1B	= tAddExpC1;
	tAddFraU1B	= tAddFraU1 >>> (tAddExpC1 - tAddExpU1);
	tAddFraV1B	= tAddFraU1 >>> (tAddExpC1 - tAddExpV1);

	/* FADD, Stage 2 */

	tAddExpU2B	= tAddExpU2;
	tAddExpV2B	= tAddExpV2;
	tAddExpC2B	= tAddExpC2;
	/* Frac computed by dedicated adder */

	/* FADD, Stage 3 */

	if(!tOpIsMul)
	begin
		tPackPosDst = tAddFraC3[71];
		tPackSign	= tSgnC4;
		tPackExp	= tAddExpC3;
		tPackFrac	= tAddFraC3;
	end

end

always @(posedge clock)
begin
		tSgnU2		<= tSgnU1;
		tExpU2		<= tExpU1;
		tFraU2		<= tFraU1;

		tSgnU3		<= tSgnU2;
		tExpU3		<= tExpU2;
		tFraU3		<= tFraU2;

		tSgnU4		<= tSgnU3;
		tExpU4		<= tExpU3;
		tFraU4		<= tFraU3;

		tPosDst2	<= tPosDst1;
		tPosDst3	<= tPosDst2;
		tPosDst4	<= tPosDst3;

	tAddExpU1	<= tAddExpU;
	tAddExpV1	<= tAddExpV;
	tAddExpC1	<= tAddExpC;
	tAddFraU1	<= tAddFraU;
	tAddFraV1	<= tAddFraV;

	tAddExpU2	<= tAddExpU1B;
	tAddExpV2	<= tAddExpV1B;
	tAddExpC2	<= tAddExpC1B;
	tAddFraU2	<= tAddFraU1B;
	tAddFraV2	<= tAddFraV1B;
//	tAddFraC2	<= tAddFraC1B;

	tAddExpC3	<= tAddExpC2B;
	tAddFraC3	<= tAddFraC2B;

//	tRegValRo	<= tValC4;
end

endmodule
