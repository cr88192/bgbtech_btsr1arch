`include "Jx2CoreDefs.v"
`include "Jx2ExCsAdd64F.v"

module Jx2ExMul(
	clock, reset,

	regMulA,		regMulB,
//	regMulC,		regMulD,
	regMulOp,	
//	regMulIsPMul,

	oregMulDlr,		oregMulDhr,
//	oregMulDlrHi,	oregMulDhrHi,
	oregMulOpDr
);

input			clock;
input			reset;

// input[31:0]		regMulA;
// input[31:0]		regMulB;
// input[31:0]		regMulC;
// input[31:0]		regMulD;
input[63:0]		regMulA;
input[63:0]		regMulB;
input[2:0]		regMulOp;
// input			regMulIsPMul;

// output[31:0]	oregMulDlr;
// output[31:0]	oregMulDhr;
// output[31:0]	oregMulDlrHi;
// output[31:0]	oregMulDhrHi;
output[63:0]	oregMulDlr;
output[63:0]	oregMulDhr;

output[2:0]		oregMulOpDr;

reg[31:0]		regMulDlr;
reg[31:0]		regMulDhr;
reg[31:0]		regMulDlrHi;
reg[31:0]		regMulDhrHi;
reg[2:0]		regMulOpDr;

// assign		oregMulDlr		= regMulDlr;
// assign		oregMulDhr		= regMulDhr;
// assign		oregMulDlrHi	= regMulDlrHi;
// assign		oregMulDhrHi	= regMulDhrHi;

assign		oregMulDlr		= {regMulDlrHi, regMulDlr};
assign		oregMulDhr		= {regMulDhrHi, regMulDhr};

assign		oregMulOpDr		= regMulOpDr;


reg[31:0]		regMulA1;	//Low 32 of A
reg[31:0]		regMulB1;	//Low 32 of B
reg[31:0]		regMulC1;	//High 32 of A
reg[31:0]		regMulD1;	//High 32 of B
reg[2:0]		regMulOp1;
reg				regMulIsPMul1;

// reg[31:0]	regMulPiAA;
// reg[31:0]	regMulPiAB;
// reg[31:0]	regMulPiBA;
// reg[31:0]	regMulPiBB;

reg[15:0]	regMulPiAA;
reg[15:0]	regMulPiAB;
reg[15:0]	regMulPiAC;
reg[15:0]	regMulPiAD;
reg[15:0]	regMulPiBA;
reg[15:0]	regMulPiBB;
reg[15:0]	regMulPiBC;
reg[15:0]	regMulPiBD;

reg[31:0]	regMulPiAE;
reg[31:0]	regMulPiBE;

// reg[31:0]	regMulPiD;

reg[2:0]	regMulOpPi;
reg[2:0]	regMulOpPj;

reg[31:0]	regMulPpA;
reg[31:0]	regMulPpB;
reg[31:0]	regMulPpC;
reg[31:0]	regMulPpD;
reg[31:0]	regMulPpE;

reg[2:0]	regMulOpPp;

reg[31:0]	regMulPpA1;
reg[31:0]	regMulPpB1;
reg[31:0]	regMulPpC1;
reg[31:0]	regMulPpD1;


// reg[31:0]	regMulPqA;
// reg[31:0]	regMulPqB;
reg[63:0]	regMulPqA;
reg[63:0]	regMulPqB;

reg[63:0]	regMulPqA1;
reg[63:0]	regMulPqB1;
wire[63:0]	regMulPqC1;

Jx2ExCsAdd64F	mulAdd(regMulPqA1, regMulPqB1, regMulPqC1);

reg[2:0]	regMulOpPq;

reg[31:0]	regMulPrA;
reg[31:0]	regMulPrB;
reg[2:0]	regMulOpPr;

always @*
begin
// / *
//	regMulPiAA	= { 16'h0, regMulA[15: 0] };
//	regMulPiAB	= { 16'h0, regMulA[31:16] };
//	regMulPiBA	= { 16'h0, regMulB[15: 0] };
//	regMulPiBB	= { 16'h0, regMulB[31:16] };

//	regMulPiAA	= regMulA1[15: 0];
//	regMulPiAB	= regMulA1[31:16];
//	regMulPiBA	= regMulB1[15: 0];
//	regMulPiBB	= regMulB1[31:16];

`ifdef JX2_EX_GPRSIMD
	if(regMulIsPMul1)
	begin
		regMulPiAA	= regMulA1[15: 0];
		regMulPiAB	= regMulC1[15: 0];
		regMulPiAC	= regMulA1[31:16];
		regMulPiAD	= regMulC1[31:16];

		regMulPiBA	= regMulB1[15: 0];
		regMulPiBB	= regMulB1[31:16];
		regMulPiBC	= regMulD1[15: 0];
		regMulPiBD	= regMulD1[31:16];
	end
	else
	begin
		regMulPiAA	= regMulA1[15: 0];
		regMulPiAB	= regMulA1[31:16];
		regMulPiBA	= regMulB1[15: 0];
		regMulPiBB	= regMulB1[31:16];

		regMulPiAC	= regMulPiAA;
		regMulPiAD	= regMulPiAB;
		regMulPiBC	= regMulPiBA;
		regMulPiBD	= regMulPiBB;
	end
`else
		regMulPiAA	= regMulA1[15: 0];
		regMulPiAB	= regMulA1[31:16];
		regMulPiBA	= regMulB1[15: 0];
		regMulPiBB	= regMulB1[31:16];

		regMulPiAC	= regMulPiAA;
		regMulPiAD	= regMulPiAB;
		regMulPiBC	= regMulPiBA;
		regMulPiBD	= regMulPiBB;
`endif

//	regMulPiAE	= { regMulA[31]?16'hFFFF:16'h0000, regMulA[31:16] };
//	regMulPiBE	= { regMulB[31]?16'hFFFF:16'h0000, regMulB[31:16] };
// * /

	regMulPiAE	= regMulA1[31]?(-regMulB1):UV32_00;
	regMulPiBE	= regMulB1[31]?(-regMulA1):UV32_00;

//	regMulPpA1	= regMulPiAA * regMulPiBA;
//	regMulPpB1	= regMulPiAA * regMulPiBB;
//	regMulPpC1	= regMulPiAB * regMulPiBA;
//	regMulPpD1	= regMulPiAB * regMulPiBB;

	regMulOpPi	= regMulOp1;

//	{ regMulPqB, regMulPqA } =
//		{UV32_00, regMulPpA} +
//		{UV16_00, regMulPpB, UV16_00} +
//		{UV16_00, regMulPpC, UV16_00} +
//		{regMulPpD, UV32_00} +
//		{regMulPpE, UV32_00};

//	{ regMulPqB, regMulPqA } =
//		{UV16_00, regMulPpB, UV16_00} +
//		{UV16_00, regMulPpC, UV16_00} +
//		{regMulPpD, regMulPpA} +
//		{regMulPpE, UV32_00};

//	regMulPqA =
//		{UV16_00, regMulPpB, UV16_00} +
//		{UV16_00, regMulPpC, UV16_00};
//	regMulPqB =
//		{regMulPpD, regMulPpA} +
//		{regMulPpE, UV32_00};

	regMulPqA = {UV15_00,
		{1'b0, regMulPpB} + {1'b0, regMulPpC},
		UV16_00};
	regMulPqB = {regMulPpD + regMulPpE, regMulPpA};

//	{ regMulPrB, regMulPrA } = regMulPqA1 + regMulPqB1;
	{ regMulPrB, regMulPrA } = regMulPqC1;

//	regMulOpPq = regMulOpPp;
end

always @ (posedge clock)
begin


//	regMulA1		<= regMulA;
//	regMulB1		<= regMulB;
//	regMulC1		<= regMulC;
//	regMulD1		<= regMulD;

	regMulA1		<= regMulA[31: 0];
	regMulB1		<= regMulB[31: 0];
	regMulC1		<= regMulA[63:32];
	regMulD1		<= regMulB[63:32];

	regMulOp1		<= regMulOp;
`ifdef JX2_EX_GPRSIMD
	regMulIsPMul1	<= regMulOp[2];
`else
	regMulIsPMul1	<= 0;
`endif

//	regMulPpA	<= { 16'h0, regMulPiAA } * { 16'h0, regMulPiBA };
//	regMulPpB	<= { 16'h0, regMulPiAA } * { 16'h0, regMulPiBB };
//	regMulPpC	<= { 16'h0, regMulPiAB } * { 16'h0, regMulPiBA };
//	regMulPpD	<= { 16'h0, regMulPiAB } * { 16'h0, regMulPiBB };

	regMulPpA	<= { 16'h0, regMulPiAA } * { 16'h0, regMulPiBA };
	regMulPpB	<= { 16'h0, regMulPiAC } * { 16'h0, regMulPiBB };
	regMulPpC	<= { 16'h0, regMulPiAB } * { 16'h0, regMulPiBC };
	regMulPpD	<= { 16'h0, regMulPiAD } * { 16'h0, regMulPiBD };

	regMulPqA1	<= regMulPqA;
	regMulPqB1	<= regMulPqB;

	case(regMulOp1[1:0])
		2'h0: begin
			regMulOpPp	<= 0;
			regMulOpPq	<= 0;
			regMulOpPr	<= 0;
			regMulOpDr	<= 0;
		end

		2'h1: begin
			regMulPpE	<= 0;
			regMulOpPp	<= regMulOpPi;
			regMulOpPq	<= regMulOpPp;
			regMulOpPr	<= regMulOpPq;

//			regMulDlr	<= regMulPqA;
//			regMulDhr	<= regMulPqB;
//			regMulOpDr	<= regMulOpPq;

			regMulDlr	<= regMulPrA;
			regMulDhr	<= regMulPrB;
			regMulOpDr	<= regMulOpPr;

			regMulDlrHi	<= UV32_XX;
			regMulDhrHi	<= UV32_XX;
			
`ifdef JX2_EX_GPRSIMD
			if(regMulIsPMul1)
			begin
				regMulDlr	<= {
					regMulPpB[15: 0], regMulPpA[15: 0] };
				regMulDhr	<= {
					regMulPpB[31:16], regMulPpA[31:16] };
				regMulDlrHi	<= {
					regMulPpD[15: 0], regMulPpC[15: 0] };
				regMulDhrHi	<= {
					regMulPpD[31:16], regMulPpC[31:16] };
			end
`endif
		end

		2'h2: begin
			regMulPpE	<= regMulPiAE + regMulPiBE;
			regMulOpPp	<= regMulOpPi;
			regMulOpPq	<= regMulOpPp;
			regMulOpPr	<= regMulOpPq;

//			regMulDlr	<= regMulPqA;
//			regMulDhr	<= regMulPqB;
//			regMulOpDr	<= regMulOpPq;

			regMulDlr	<= regMulPrA;
			regMulDhr	<= regMulPrB;
			regMulOpDr	<= regMulOpPr;

			regMulDlrHi	<= UV32_XX;
			regMulDhrHi	<= UV32_XX;
			
`ifndef JX2_QUIET
			$display("Jx2ExOp: Mul A: %X %X", regMulA, regMulB);
			$display("Jx2ExOp: Mul B: %X %X %X %X",
				regMulPpA, regMulPpB,
				regMulPpC, regMulPpD);
			$display("Jx2ExOp: Mul C: %X %X", regMulDlr, regMulDhr);
`endif
			
		end

		2'h3: begin
			regMulOpPp	<= 0;
			regMulOpPq	<= 0;
			regMulOpPr	<= 0;
			regMulOpDr	<= 0;
		end
	endcase
end

endmodule

