`include "FpSvModShrAddSp.v"

module FpSvAdd(
	/* verilator lint_off UNUSED */
	clock,		reset,		exHold,
	regValRs,
	regValRt,
	regValRn,
	regExOp,
	regRMode
	);

input	clock;
input	reset;

input[31:0]		regValRs;
input[31:0]		regValRt;
output[31:0]	regValRn;
input[3:0]		regExOp;
input[7:0]		regRMode;

reg				tSgnA1;
reg				tSgnB1;
reg[8:0]		tExpA1;
reg[8:0]		tExpB1;

reg[8:0]		tExpDA1;
reg[8:0]		tExpDB1;

reg[27:0]		tFraA1;
reg[27:0]		tFraB1;

reg[27:0]		tFraAn1;
reg[27:0]		tFraBn1;
reg				tCaA1;


reg[8:0]		tExpA2;
reg[8:0]		tExpDA2;
reg				tCaA2;

reg[27:0]		tFraA2;
reg[27:0]		tFraB2;
// reg[27:0]		tFraP2;
wire[27:0]		tFraP2;
reg[27:0]		tFraPa2;

FpSvModShrAddSp		shadd1(tFraA2, tFraB2, tExpDA2, tCaA2, tFraP2);

always @*
begin

	/* Stage 1 */

	tSgnA1	= regValRs[31];
	tSgnB1	= regValRt[31];

	tExpA1	= { 1'b0, regValRs[30:23] };
	tExpB1	= { 1'b0, regValRt[30:23] };
	tFraA1	= { 3'b001, regValRs[22: 0], 2'h0 };
	tFraB1	= { 3'b001, regValRt[22: 0], 2'h0 };
	tExpDA1	= tExpA1 - tExpB1;
	tExpDB1	= tExpB1 - tExpA1;

	tFraAn1 = tSgnA1 ? (~tFraA1) : tFraA1;
	tFraBn1 = tSgnB1 ? (~tFraB1) : tFraB1;
	tCaA1	= tSgnA1 ^ tSgnB1;

	/* Stage 2 */

//	tFraP2	= tFraA2 + (tFraB2 >>> tExpDA2[4:0]) + { 27'h0, tCaA2 };
	tFraPa2	= tFraP2[27] ? ~tFraP2 : tFraP2;
end

always @(posedge clock)
begin
	tCaA2	<= tCaA1;

	if(!tExpDA1[8])
	begin
		tExpA2		<= tExpA1;
		tFraA2		<= tFraAn1;
		tExpB2		<= tExpB1;
		tFraB2		<= tFraBn1;
		tExpDA2		<= tExpDA1;
	end
	else
	begin
		tExpA2		<= tExpB1;
		tFraA2		<= tFraBn1;
		tExpB2		<= tExpA1;
		tFraB2		<= tFraAn1;
		tExpDA2		<= tExpDB1;
	end

end


endmodule
