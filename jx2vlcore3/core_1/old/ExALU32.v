/*
ALU32

Perform some 32-bit ALU Operations
Ignores high 32 bits of inputs.
Sign or Zero Extends output.

idUIxt:
  [7:6]=CC (AL/NV/CT/CF)
  [5:4]=Op2
  [  3]=ZExt (0=SX, 1=ZX)
  [2:0]=Op1

{Op2,Op1}:
  00,000: ADD
  00,001: SUB
  00,010: ADC
  00,011: SBB
  00,100: TST
  00,101: AND
  00,110: OR
  00,111: XOR

  01,010: CMPEQ
  01,011: CMPNE
  01,100: CMPHI
  01,101: CMPHS
  01,110: CMPGT
  01,111: CMPGE

Op2:
//  00: T=T
//  01: T=CF
//  10: T=ZF
//  11: T=!Z & !(S^V)

CMPEQ: SUB, ZF
GMPGT: SUB, CF

V=!(S1^S2) & (S2^S3)

S1, S2, S3 -> V
 0,  0,  0 -> 0
 0,  0,  1 -> 1
 0,  1,  0 -> 0
 0,  1,  1 -> 0
 1,  0,  0 -> 0
 1,  0,  1 -> 0
 1,  1,  0 -> 1
 1,  1,  1 -> 0

 */

module ExALU32(
	regValRs,
	regValRt,
	idUIxt,
	regOutVal,
	regInSrT,
	regOutSrT
	);

input[63:0]		regValRs;
input[63:0]		regValRt;
input[7:0]		idUIxt;

output[63:0]	regOutVal;

input	regInSrT;
output	regOutSrT;

reg[63:0]	tRegOutVal;
assign	regOutVal = tRegOutVal;

reg		tRegOutSrT;
assign	regOutSrT = tRegOutSrT;


reg[16:0]	tAddA0;
reg[16:0]	tAddA1;
reg[16:0]	tAddB0;
reg[16:0]	tAddB1;

reg[16:0]	tSubA0;
reg[16:0]	tSubA1;
reg[16:0]	tSubB0;
reg[16:0]	tSubB1;

reg[16:0]	tAdd0;
reg[16:0]	tAdd1;

reg[32:0]	tSub0;
reg[32:0]	tSub1;

reg[32:0]	tResultA;
reg			tResultT;

reg			tSub1ZF;
reg			tSub1CF;
reg			tSub1SF;
reg			tSub1VF;

reg			tTstZF;


always @*
begin
	tAddA0 = { 1'b0, regValRs[15: 0] } + { 1'b0,  regValRt[15: 0] } + 0;
	tAddA1 = { 1'b0, regValRs[15: 0] } + { 1'b0,  regValRt[15: 0] } + 1;
	tAddB0 = { 1'b0, regValRs[31:16] } + { 1'b0,  regValRt[31:16] } + 0;
	tAddB1 = { 1'b0, regValRs[31:16] } + { 1'b0,  regValRt[31:16] } + 1;

	tSubA0 = { 1'b0, regValRs[15: 0] } + { 1'b0, ~regValRt[15: 0] } + 0;
	tSubA1 = { 1'b0, regValRs[15: 0] } + { 1'b0, ~regValRt[15: 0] } + 1;
	tSubB0 = { 1'b0, regValRs[31:16] } + { 1'b0, ~regValRt[31:16] } + 0;
	tSubB1 = { 1'b0, regValRs[31:16] } + { 1'b0, ~regValRt[31:16] } + 1;
	
	tAdd0 = { tAddA0[16]?tAddB1:tAddB0, tSubA0[15:0] };
	tAdd1 = { tAddA1[16]?tAddB1:tAddB0, tSubA1[15:0] };
	tSub0 = { tSubA0[16]?tSubB1:tSubB0, tSubA0[15:0] };
	tSub1 = { tSubA1[16]?tSubB1:tSubB0, tSubA1[15:0] };


	tSub1ZF = (tSub1[15:0]==0) && (tSub1[31:16]==0);
	tSub1CF = tSub1[32];
	tSub1SF = tSub1[31];
	
	tTstZF =
		((regValRs[15:0]&regValRt[15:0])==0) &&
		((regValRs[31:16]&regValRt[31:16])==0) ;

	case({regValRs[31], regValRs[31], tResSF})
		3'b000: tSub1VF=0;
		3'b001: tSub1VF=1;
		3'b010: tSub1VF=0;
		3'b011: tSub1VF=0;
		3'b100: tSub1VF=0;
		3'b101: tSub1VF=0;
		3'b110: tSub1VF=1;
		3'b111: tSub1VF=0;
	endcase


	tResCFn=0;
	case({ idUIxt[5:4], idUIxt[2:0] })
		5'b00000: begin
			tResultA=tAdd0;
			tResultT=regInSrT;
		end
		5'b00001: begin
			tResultA=tSub1;
			tResultT=regInSrT;
		end
		5'b00010: begin
			tResultA=regInSrT ? tAdd1 : tAdd0;
			tResultT=tResultA[32];
		end

		5'b00011: begin
			tResultA=regInSrT ? tSub0 : tSub1;
			tResultT=!tResultA[32];
		end
		
		5'00100: begin
			tResultA=UV33_XX;
			tResultT=tTstZF;
		end
		5'00101: begin
			tResultA={1'b0, regValRs[31:0] & regValRt[31:0]};
			tResultT=regInSrT;
		end
		5'00110: begin
			tResultA={1'b0, regValRs[31:0] | regValRt[31:0]};
			tResultT=regInSrT;
		end
		5'00111: begin
			tResultA={1'b0, regValRs[31:0] ^ regValRt[31:0]};
			tResultT=regInSrT;
		end

		5'01010: begin
			tResultA=UV33_XX;
			tResultT=tSub1ZF;
		end
		5'01011: begin
			tResultA=UV33_XX;
			tResultT=!tSub1ZF;
		end
		5'01100: begin
			tResultA=UV33_XX;
			tResultT=!tSub1CF && !tSub1ZF;
		end
		5'01101: begin
			tResultA=UV33_XX;
			tResultT=!tSub1CF;
		end
		5'01110: begin
			tResultA=UV33_XX;
//			tResultT=!tResZF && !(tResSF^tResVF);
			tResultT=(tSub1SF^tSub1VF);
		end
		5'01111: begin
			tResultA=UV33_XX;
			tResultT=tSub1ZF || (tSub1SF^tSub1VF);
		end
		default: begin
			tResultA=UV33_XX;
			tResultT=regInSrT;
		end
	endcase

	if(idUIxt[3])
		tRegOutVal = { UV32_00, tResultA[31:0] };
	else
		tRegOutVal = { tResultA[31]?UV32_FF:UV32_00, tResultA[31:0] };
	tRegOutSrT = tResultT;
end

endmodule
