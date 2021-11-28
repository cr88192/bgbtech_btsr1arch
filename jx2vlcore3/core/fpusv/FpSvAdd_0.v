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

reg				tOpIsHalf1;

reg				tSgnA1;
reg				tSgnB1;
reg				tSgnC1;
reg				tSgnD1;
reg[8:0]		tExpA1;
reg[8:0]		tExpB1;
reg[5:0]		tExpC1;
reg[5:0]		tExpD1;

reg[8:0]		tExpDA1;
reg[8:0]		tExpDB1;
reg[5:0]		tExpDC1;
reg[5:0]		tExpDD1;

reg[27:0]		tFraA1;
reg[27:0]		tFraB1;
reg[15:0]		tFraC1;
reg[15:0]		tFraD1;

reg[27:0]		tFraAn1;
reg[27:0]		tFraBn1;
reg[15:0]		tFraCn1;
reg[15:0]		tFraDn1;
reg				tCaA1;
reg				tCaC1;


reg[8:0]		tExpA2;
reg[5:0]		tExpC2;
reg[8:0]		tExpDA2;
reg[5:0]		tExpDC2;
reg				tCaA2;
reg				tCaC2;

reg[27:0]		tFraA2;
reg[27:0]		tFraB2;
reg[27:0]		tFraP2;
reg[27:0]		tFraPa2;

reg[15:0]		tFraC2;
reg[15:0]		tFraD2;
reg[15:0]		tFraQ2;
reg[15:0]		tFraQa2;

always @*
begin

	/* Stage 1 */

	tSgnA1	= regValRs[31];
	tSgnB1	= regValRt[31];
	tSgnC1	= regValRs[15];
	tSgnD1	= regValRt[15];

	tOpIsHalf1	= 0;

	tExpC1	= { 1'b0, regValRs[14:10] };
	tExpD1	= { 1'b0, regValRt[14:10] };
	tFraC1	= { 3'b001, regValRs[9: 0], 3'h0 };
	tFraD1	= { 3'b001, regValRt[9: 0], 3'h0 };
	tExpDC1	= tExpC1 - tExpD1;
	tExpDD1	= tExpD1 - tExpC1;
	
	if(tOpIsHalf1)
	begin
		tExpA1	= {
			1'b0,
			regValRs[30] ? 3'b000 : 3'b111,
			regValRs[30:26] };
		tExpB1	= {
			1'b0,
			regValRt[30] ? 3'b000 : 3'b111,
			regValRt[30:26] };
		tFraA1	= { 3'b001, regValRs[25:16], 13'h0, 2'h0 };
		tFraB1	= { 3'b001, regValRt[25:16], 13'h0, 2'h0 };
		tExpDA1	= tExpA1 - tExpB1;
		tExpDB1	= tExpB1 - tExpA1;
	end
	else
	begin
		tExpA1	= { 1'b0, regValRs[30:23] };
		tExpB1	= { 1'b0, regValRt[30:23] };
		tFraA1	= { 3'b001, regValRs[22: 0], 2'h0 };
		tFraB1	= { 3'b001, regValRt[22: 0], 2'h0 };
		tExpDA1	= tExpA1 - tExpB1;
		tExpDB1	= tExpB1 - tExpA1;
	end

	tFraAn1 = tSgnA1 ? (~tFraA1) : tFraA1;
	tFraBn1 = tSgnB1 ? (~tFraB1) : tFraB1;
	tFraCn1 = tSgnC1 ? (~tFraC1) : tFraC1;
	tFraDn1 = tSgnD1 ? (~tFraD1) : tFraD1;
	tCaA1	= tSgnA1 ^ tSgnB1;
	tCaC1	= tSgnC1 ^ tSgnD1;

	/* Stage 2 */

	tFraP2	= tFraA2 + (tFraB2 >>> tExpDA2[4:0]) + { 27'h0, tCaA2 };
	tFraQ2	= tFraC2 + (tFraD2 >>> tExpDC2[3:0]) + { 15'h0, tCaC2 };

	tFraPa2	= tFraP2[27] ? ~tFraP2 : tFraP2;
	tFraQa2	= tFraQ2[15] ? ~tFraQ2 : tFraQ2;
end

always @(posedge clock)
begin
	tCaA2	<= tCaA1;
	tCaC2	<= tCaC1;

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

	if(!tExpDC1[5])
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
