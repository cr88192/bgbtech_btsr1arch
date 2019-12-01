/*
Carry Select Adder

Allows Carry-In, Carry-Out, and the ability to operate on packed values.

vMode:
  000: ADD/ADC 64 bit
  001: SUB/SBB 64 bit
  010: ADD/ADC 32 bit
  011: SUB/SBB 32 bit
  100: PADDL
  101: PSUBL
  110: PADDW
  111: PSUBW

 */

module Jx2CsAdd64(
	valInA, valInB, valOut,
	valInC, valOutC, vMode);

input[63:0]		valInA;
input[63:0]		valInB;
input			valInC;

output[63:0]	valOut;
output			valOutC;

input[2:0]		vMode;


wire[63:0]		valInB1;
wirq			valInC1;
assign			valInB1 = vMode[0] ? (~valInB) : valInB;
assign			valInC1 = vMode[0] ? (!valInC) : valInC;

reg[63:0]		tValOut;
reg				tValOutC;
assign			valOut		= tValOut;
assign			valOutC		= tValOutC;


reg[16:0]		tValSA0;
reg[16:0]		tValSA1;
reg[16:0]		tValSA2;
reg[16:0]		tValSA3;
reg[16:0]		tValSA4;

reg[16:0]		tValSB0;
reg[16:0]		tValSB1;
reg[16:0]		tValSB2;
reg[16:0]		tValSB3;
reg[16:0]		tValSB4;

reg[16:0]		tValSC0;
reg[16:0]		tValSC1;
reg[16:0]		tValSC2;
reg[16:0]		tValSC3;
reg[16:0]		tValSC4;

reg[16:0]		tValSD0;
reg[16:0]		tValSD1;
reg[16:0]		tValSD2;
reg[16:0]		tValSD3;
reg[16:0]		tValSD4;

always @*
begin
	tValSA0 = { 1'b0, valInA[15: 0] } + { 1'b0, valInB1[15: 0] } + 0;
	tValSA1 = { 1'b0, valInA[15: 0] } + { 1'b0, valInB1[15: 0] } + 1;
	tValSB0 = { 1'b0, valInA[31:16] } + { 1'b0, valInB1[31:16] } + 0;
	tValSB1 = { 1'b0, valInA[31:16] } + { 1'b0, valInB1[31:16] } + 1;
	tValSC0 = { 1'b0, valInA[47:32] } + { 1'b0, valInB1[47:32] } + 0;
	tValSC1 = { 1'b0, valInA[47:32] } + { 1'b0, valInB1[47:32] } + 1;
	tValSD0 = { 1'b0, valInA[63:48] } + { 1'b0, valInB1[63:48] } + 0;
	tValSD1 = { 1'b0, valInA[63:48] } + { 1'b0, valInB1[63:48] } + 1;

//	tValSA2 = valInC1		? tValSA1 : tValSA0;
//	tValSB2 = tValSA2[16]	? tValSB1 : tValSB0;
//	tValSC2 = tValSB2[16]	? tValSC1 : tValSC0;
//	tValSD2 = tValSC2[16]	? tValSD1 : tValSD0;

	tValSA2 = tValSA0;
	tValSB2 = tValSA0[16] ? tValSB1 : tValSB0;
	tValSC2 = tValSC0;
	tValSD2 = tValSC0[16] ? tValSD1 : tValSD0;

	tValSA3 = tValSA1;
	tValSB3 = tValSA1[16] ? tValSB1 : tValSB0;
	tValSC3 = tValSC1;
	tValSD3 = tValSC1[16] ? tValSD1 : tValSD0;

	tValSA4 = valInC1 ? tValSB3 : tValSB2;
	tValSB4 = valInC1 ? tValSB3 : tValSB2;
	tValSC4 = tValSB4[16] ? tValSD3 : tValSD2;
	tValSD4 = tValSB4[16] ? tValSD3 : tValSD2;


	case(vMode[2:1])

	2'b00: begin
		tValOut = {
			tValSD4[15:0], tValSC4[15:0],
			tValSB4[15:0], tValSA4[15:0] };
		tValOutC = tValSD2[16];
	end

	2'b01: begin
		tValOut = {
			tValSD4[15:0], tValSC4[15:0],
			tValSB4[15:0], tValSA4[15:0] };
		tValOutC = tValSB2[16];
	end

	2'b10: begin
		tValOut = {
			tValSD2[15:0], tValSC2[15:0],
			tValSB2[15:0], tValSA2[15:0] };
		tValOutC = 0;
	end

	2'b11: begin
		tValOut = {
			tValSD0[15:0], tValSC0[15:0],
			tValSB0[15:0], tValSA0[15:0] };
		tValOutC = 0;
	end

	endcase
end

endmodule
