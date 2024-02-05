/*
 * Fast-Case ALU
 * Try to perform a subset of operations within a single clock-cycle.
 * Does not need to handle all operations.
 */

module ExFastALU(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	idUCmd,
	idUIxt,
	regValRs,
	regValRt,
	regOutVal,
	regOutOK
	);

input			clock;
input			reset;

input[63:0]		regValRs;
input[63:0]		regValRt;
input[8:0]		idUCmd;
input[8:0]		idUIxt;

output[63:0]	regOutVal;
output			regOutOK;

reg[63:0]	tRegOutVal;
reg			tRegOutOK;
assign	regOutVal = tRegOutVal;
assign	regOutOK = tRegOutOK;

wire	tRsHiIsZero;
wire	tRtHiIsZero;
wire	tRsHiIsOnes;
wire	tRtHiIsOnes;
assign		tRsHiIsZero = (regValRs[63:30] == UV34_00);
assign		tRtHiIsZero = (regValRt[63:30] == UV34_00);
assign		tRsHiIsOnes = (regValRs[63:30] == UV34_FF);
assign		tRtHiIsOnes = (regValRt[63:30] == UV34_FF);

wire[63:0]	tRegValRtInv;
assign			tRegValRtInv = ~regValRt;

wire	tOpIsAdd = (idUIxt[3:0] == 0);
wire	tOpIsSub = (idUIxt[3:0] == 1);
wire	tOpIsZx = (idUIxt[5:4] == 2'b01);
wire	tOpIs64 = (idUIxt[5:4] == 2'b10);
wire	tOpIsPx = (idUIxt[5:4] == 2'b11);

wire	tRsHiIsValid = tOpIs64 ? (tRsHiIsZero || tRsHiIsOnes) : 1;
wire	tRtHiIsValid = tOpIs64 ? (tRtHiIsZero || tRtHiIsOnes) : 1;

reg[63:0]	tRegValRt;

reg[16:0]	tValAddA0;
reg[16:0]	tValAddA1;
reg[16:0]	tValAddB0;
reg[16:0]	tValAddB1;
reg			tValAddSelA;
reg			tValAddSelB;
reg			tValAddSx;
reg			tValAddSxP;

reg[32:0]	tValAdd;
reg			tValAddValid;

always @*
begin

	tRegOutVal	= UV64_XX;
	tRegOutOK	= 0;

	tRegValRt = tOpIsSub ? tRegValRtInv : regValRt;
	
	tValAddA0 = { 1'b0, regValRs[15: 0] } + { 1'b0, tRegValRt[15: 0] } + 0;
	tValAddA1 = { 1'b0, regValRs[15: 0] } + { 1'b0, tRegValRt[15: 0] } + 1;
	tValAddB0 = { 1'b0, regValRs[31:16] } + { 1'b0, tRegValRt[31:16] } + 0;
	tValAddB1 = { 1'b0, regValRs[31:16] } + { 1'b0, tRegValRt[31:16] } + 1;
	tValAddSelA	= tOpIsSub;
	tValAddSelB = tValAddSelA ? tValAddA1[16] : tValAddA0[16];
	tValAddSx = tValAddSelB ? tValAddB1[15] : tValAddB0[15];
	
	tValAdd = {
		(tValAddSx && !tOpIsZx),
		tValAddSelB ? tValAddB1[15:0] : tValAddB0[15:0],
		tValAddSelA ? tValAddA1[15:0] : tValAddA0[15:0]
		};
	tValAddValid = (!tOpIs64 || (tRsHiIsValid && tRtHiIsValid)) && !tOpIsPx;
	
	if(idUCmd[5:0] == JX2_UCMD_ALU3)
	begin
		case(idUIxt[3:0])
			4'h0, 4'h1:
			begin
				tRegOutVal = {
					tValAdd[32] ? UV32_FF : UV32_00,
					tValAdd[31:0]
					};
				tRegOutOK = tValAddValid;
			end
			4'h5:
			begin
				tRegOutVal = regValRs & regValRt;
				tRegOutOK = 1;
			end
			4'h6:
			begin
				tRegOutVal = regValRs | regValRt;
				tRegOutOK = 1;
			end
			4'h7:
			begin
				tRegOutVal = regValRs ^ regValRt;
				tRegOutOK = 1;
			end
			default: begin
				tRegOutOK = 0;
			end
		endcase
	end
end

endmodule
