
`include "CoreDefs.v"
// `include "ExCsAdd64F.v"

`include "Fp2Mul.v"

module Fp2Mac(
	/* verilator lint_off UNUSED */
	clock,		reset,		exHold,
	regValRs,	regValRt,	regValRu,
	regValRm,	regValRn,	regValOp,
	regValPos,	regValPosDst
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
input[5:0]			regValOp;		//Operation

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

// reg[16:0]		tExpC1;

// reg				tSgnC2;
// reg[16:0]		tExpC2;

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
reg[79:0]		tAddFraU;
reg[79:0]		tAddFraV;
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

// reg				tSgnC3;
// reg[16:0]		tExpC3;

reg[5:0]		tPosDstM1;	//Vector Position Destination (Mul 1)
reg[5:0]		tPosDstM2;	//Vector Position Destination (Mul 2)
reg[5:0]		tPosDstM3;	//Vector Position Destination (Mul 3)
reg[5:0]		tPosDstM4;	//Vector Position Destination (Mul 4)

reg[5:0]		tPosDstA1;	//Vector Position Destination (Add 1)
reg[5:0]		tPosDstA2;	//Vector Position Destination (Add 2)
reg[5:0]		tPosDstA3;	//Vector Position Destination (Add 3)
reg[5:0]		tPosDstA4;	//Vector Position Destination (Add 4)



// reg				tFraRbit4B;
// reg[8:0]		tFraRnd4B;
// reg[63:0]		tValC4;

reg[79:0]		tAddFraU1;
reg[79:0]		tAddFraV1;

reg[79:0]		tAddFraU1B;
reg[79:0]		tAddFraV1B;

reg[79:0]		tAddFraU2;
reg[79:0]		tAddFraV2;
wire[79:0]		tAddFraC2B;
ExCsAdd80F		fpAdd(tAddFraU2, tAddFraV2, tAddFraC2B);

reg[15:0]		tAddExpC3;
reg[79:0]		tAddFraC3;

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

wire			tSgnMul;
wire[15:0]		tExpMul;
wire[71:0]		tFraMul;

Fp2Mul	fpMul(
	clock,		reset,		exHold,
	tSgnS1,		tExpS1,		tFraS1,
	tSgnT1,		tExpT1,		tFraT1,
	tSgnMul,	tExpMul,	tFraMul);

reg		tOpIsAddSub;
reg		tOpIsMul;
reg		tOpIsSub;
reg		tOpIsRSub;

always @*
begin
	tOpIsAddSub	= 0;
	tOpIsMul	= 0;
	tOpIsSub	= 0;
	tOpIsRSub	= 0;

	case(regValOp[3:0])
		4'b0000: begin
			tOpIsAddSub	= 1;
		end
		4'b0001: begin
			tOpIsAddSub	= 1;
			tOpIsSub	= 1;
		end
		4'b0010: begin
			tOpIsMul	= 1;
		end
		4'b1001: begin
		end
		4'b1011: begin
			tOpIsSub	= 1;
		end
		4'b1101: begin
			tOpIsRSub	= 1;
		end
		
		default: begin
		end
	endcase

	tPosDstM1 = regValPosDst;

	if(tOpIsAddSub)
	begin
		tPosDstA1	= regValPosDst;
		tAddExpU	= tExpS1;
		tAddExpV	= tExpT1;
		tAddFraU	= (tSgnS1 ^ tOpIsSub ) ?
			{1'b1, (~tFraS1), 7'h7F } :
			{1'b0,   tFraS1 , 7'h00 } ;
		tAddFraV	= (tSgnT1 ^ tOpIsRSub) ?
			{1'b1, (~tFraT1), 7'h7F } :
			{1'b0,   tFraT1 , 7'h00 } ;
		tAddExpC	= (tAddExpU > tAddExpV) ? tAddExpU : tAddExpV;
	end
	else
	begin
		tPosDstA1	= tPosDstM4;
		tAddExpU	= tExpU4;
		tAddExpV	= tExpMul;
		tAddFraU	= (tSgnU4  ^ tOpIsSub ) ?
			{1'b1, (~tFraU4), 7'h7F } :
			{1'b0,   tFraU4 , 7'h00 } ;
		tAddFraV	= (tSgnMul ^ tOpIsRSub) ?
			{1'b1, (~tFraMul), 7'h7F } :
			{1'b0,   tFraMul , 7'h00 } ;
		tAddExpC	= (tAddExpU > tAddExpV) ? tAddExpU : tAddExpV;
	end

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

	if(tOpIsMul)
	begin
		tPackPosDst = tPosDstM4;
		tPackSign	= tSgnMul;
		tPackExp	= tExpMul;
		tPackFrac	= tFraMul;
	end
	else
	begin
		tPackPosDst = tPosDstA4;
		tPackSign	= tAddFraC3[79];
		tPackExp	= tAddExpC3;
//		tPackFrac	= tAddFraC3;
		tPackFrac	= tPackSign ?
			(~tAddFraC3[78:7]) : tAddFraC3[78:7];
	end

end

always @(posedge clock)
begin
	if(!exHold)
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

		tPosDstM2	<= tPosDstM1;
		tPosDstM3	<= tPosDstM2;
		tPosDstM4	<= tPosDstM3;

		tPosDstA2	<= tPosDstA1;
		tPosDstA3	<= tPosDstA2;
		tPosDstA4	<= tPosDstA3;

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
end

endmodule
