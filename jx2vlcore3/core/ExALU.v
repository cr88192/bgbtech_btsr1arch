/*
ALU

Perform some ALU Operations
May Sign or Zero Extend output.

idUIxt:
  [7:6]=CC (AL/NV/CT/CF)
  [  5]=QWord
  [  4]=ZExt (0=SX, 1=ZX)
  [3:0]=Op1

If GSV is enabled, Q+ZX = Packed Integer Ops

Op1:
  0000: ADD	/ PADD.L
  0001: SUB	/ PSUB.L
  0010: ADC	/ PADC.L
  0011: SBB	/ PSBB.L
  0100: TST
  0101: AND
  0110: OR
  0111: XOR
  1000: CMPNE
  1001: CMPHS
  1010: CMPGE
  1011: NOR
  1100: CMPEQ
  1101: CMPHI
  1110: CMPGT
  1111: CSELT / PCSELT.L

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

`include "CoreDefs.v"

module ExALU(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	regValRs,
	regValRt,
	idUIxt,
	regInSrST,
	regOutVal,
	regOutSrST
	);

input			clock;
input			reset;

input[63:0]		regValRs;
input[63:0]		regValRt;
input[7:0]		idUIxt;
input[1:0]		regInSrST;

output[63:0]	regOutVal;
output[1:0]		regOutSrST;

wire			regInSrT;
wire			regInSrS;
assign		regInSrT = regInSrST[0];
assign		regInSrS = regInSrST[1];

reg[7:0]		idUIxt2;


reg[63:0]	tRegOutVal2;
reg			tRegOutSrT2;
reg			tRegOutSrS2;
assign	regOutVal = tRegOutVal2;
assign	regOutSrST = { tRegOutSrS2, tRegOutSrT2 };

reg[63:0]	tRegOutVal;
reg			tRegOutSrT;
reg			tRegOutSrS;


reg[16:0]	tAdd1A0;
reg[16:0]	tAdd1A1;
reg[16:0]	tAdd1B0;
reg[16:0]	tAdd1B1;

reg[16:0]	tAdd1C0;
reg[16:0]	tAdd1C1;
reg[16:0]	tAdd1D0;
reg[16:0]	tAdd1D1;

reg[16:0]	tSub1A0;
reg[16:0]	tSub1A1;
reg[16:0]	tSub1B0;
reg[16:0]	tSub1B1;

reg[16:0]	tSub1C0;
reg[16:0]	tSub1C1;
reg[16:0]	tSub1D0;
reg[16:0]	tSub1D1;

reg[32:0]	tAdd2A0;
reg[32:0]	tAdd2A1;
reg[32:0]	tAdd2B0;
reg[32:0]	tAdd2B1;

reg[32:0]	tSub2A0;
reg[32:0]	tSub2A1;
reg[32:0]	tSub2B0;
reg[32:0]	tSub2B1;

reg[64:0]	tAdd3A0;
reg[64:0]	tAdd3A1;
reg[64:0]	tSub3A0;
reg[64:0]	tSub3A1;

reg[32:0]	tResult1A;
reg			tResult1T;

reg[32:0]	tResult1B;
reg			tResult1S;

reg[64:0]	tResult2A;
reg			tResult2T;

reg			tSub1ZF;
reg			tSub1CF;
reg			tSub1SF;
reg			tSub1VF;
reg			tTst1ZF;

reg			tSub2ZF;
reg			tSub2CF;
reg			tSub2SF;
reg			tSub2VF;
reg			tTst2ZF;

reg			tSub1BZF;
reg			tSub1BCF;
reg			tSub1BSF;
reg			tSub1BVF;
reg			tTst1BZF;


always @*
begin
	tAdd1A0 = { 1'b0, regValRs[15: 0] } + { 1'b0,  regValRt[15: 0] } + 0;
	tAdd1A1 = { 1'b0, regValRs[15: 0] } + { 1'b0,  regValRt[15: 0] } + 1;
	tAdd1B0 = { 1'b0, regValRs[31:16] } + { 1'b0,  regValRt[31:16] } + 0;
	tAdd1B1 = { 1'b0, regValRs[31:16] } + { 1'b0,  regValRt[31:16] } + 1;
	tAdd1C0 = { 1'b0, regValRs[47:32] } + { 1'b0,  regValRt[47:32] } + 0;
	tAdd1C1 = { 1'b0, regValRs[47:32] } + { 1'b0,  regValRt[47:32] } + 1;
	tAdd1D0 = { 1'b0, regValRs[63:48] } + { 1'b0,  regValRt[63:48] } + 0;
	tAdd1D1 = { 1'b0, regValRs[63:48] } + { 1'b0,  regValRt[63:48] } + 1;

	tSub1A0 = { 1'b0, regValRs[15: 0] } + { 1'b0, ~regValRt[15: 0] } + 0;
	tSub1A1 = { 1'b0, regValRs[15: 0] } + { 1'b0, ~regValRt[15: 0] } + 1;
	tSub1B0 = { 1'b0, regValRs[31:16] } + { 1'b0, ~regValRt[31:16] } + 0;
	tSub1B1 = { 1'b0, regValRs[31:16] } + { 1'b0, ~regValRt[31:16] } + 1;
	tSub1C0 = { 1'b0, regValRs[47:32] } + { 1'b0, ~regValRt[47:32] } + 0;
	tSub1C1 = { 1'b0, regValRs[47:32] } + { 1'b0, ~regValRt[47:32] } + 1;
	tSub1D0 = { 1'b0, regValRs[63:48] } + { 1'b0, ~regValRt[63:48] } + 0;
	tSub1D1 = { 1'b0, regValRs[63:48] } + { 1'b0, ~regValRt[63:48] } + 1;
	
	tAdd2A0 = { tAdd1A0[16]?tAdd1B1:tAdd1B0, tAdd1A0[15:0] };
	tAdd2A1 = { tAdd1A1[16]?tAdd1B1:tAdd1B0, tAdd1A1[15:0] };
	tAdd2B0 = { tAdd1C0[16]?tAdd1D1:tAdd1D0, tAdd1C0[15:0] };
	tAdd2B1 = { tAdd1C1[16]?tAdd1D1:tAdd1D0, tAdd1C1[15:0] };

	tSub2A0 = { tSub1A0[16]?tSub1B1:tSub1B0, tSub1A0[15:0] };
	tSub2A1 = { tSub1A1[16]?tSub1B1:tSub1B0, tSub1A1[15:0] };
	tSub2B0 = { tSub1C0[16]?tSub1D1:tSub1D0, tSub1C0[15:0] };
	tSub2B1 = { tSub1C1[16]?tSub1D1:tSub1D0, tSub1C1[15:0] };

	tAdd3A0 = { tAdd2A0[32]?tAdd2B1:tAdd2B0, tAdd2A0[31:0] };
	tAdd3A1 = { tAdd2A1[32]?tAdd2B1:tAdd2B0, tAdd2A1[31:0] };
	tSub3A0 = { tSub2A0[32]?tSub2B1:tSub2B0, tSub2A0[31:0] };
	tSub3A1 = { tSub2A1[32]?tSub2B1:tSub2B0, tSub2A1[31:0] };


	tSub1ZF		= (tSub2A1[15:0]==0) && (tSub2A1[31:16]==0);
	tSub1BZF	= (tSub3A1[47:32]==0) && (tSub3A1[63:48]==0);
	tSub2ZF		= tSub1ZF && tSub1BZF;
	tSub1CF = tSub2A1[32];
	tSub2CF = tSub3A1[64];
	tSub1SF = tSub2A1[31];
	tSub2SF = tSub3A1[63];

	tSub1BCF = tSub2B1[32];
	tSub1BSF = tSub2B1[31];
	
	tTst1ZF =
		((regValRs[15: 0]&regValRt[15: 0])==0) &&
		((regValRs[31:16]&regValRt[31:16])==0) ;
	tTst1BZF =
		((regValRs[47:32]&regValRt[47:32])==0) &&
		((regValRs[63:48]&regValRt[63:48])==0) ;
	tTst2ZF =
		tTst1ZF && tTst1BZF;

	case({regValRs[31], regValRt[31], tSub1SF})
		3'b000: tSub1VF=0;
		3'b001: tSub1VF=1;
		3'b010: tSub1VF=0;
		3'b011: tSub1VF=0;
		3'b100: tSub1VF=0;
		3'b101: tSub1VF=0;
		3'b110: tSub1VF=1;
		3'b111: tSub1VF=0;
	endcase

`ifdef jx2_enable_gsv
	case({regValRs[63], regValRt[63], tSub1BSF})
		3'b000: tSub1BVF=0;
		3'b001: tSub1BVF=1;
		3'b010: tSub1BVF=0;
		3'b011: tSub1BVF=0;
		3'b100: tSub1BVF=0;
		3'b101: tSub1BVF=0;
		3'b110: tSub1BVF=1;
		3'b111: tSub1BVF=0;
	endcase
`else
	tSub1BVF=1'bX;
`endif

	case({regValRs[63], regValRt[63], tSub2SF})
		3'b000: tSub2VF=0;
		3'b001: tSub2VF=1;
		3'b010: tSub2VF=0;
		3'b011: tSub2VF=0;
		3'b100: tSub2VF=0;
		3'b101: tSub2VF=0;
		3'b110: tSub2VF=1;
		3'b111: tSub2VF=0;
	endcase

	tResult1A=UV33_XX;
	tResult2A=UV65_XX;
	tResult1T=regInSrT;
	tResult2T=regInSrT;

	tResult1B=UV33_XX;
	tResult1S=regInSrS;

	case({ idUIxt[3:0] })
		4'h0: begin		/* ADD */
			tResult1A=tAdd2A0;
			tResult2A=tAdd3A0;
			tResult1T=regInSrT;
			tResult2T=regInSrT;

			tResult1B=tAdd2B0;
			tResult1S=regInSrS;
		end
		4'h1: begin		/* SUB */
			tResult1A=tSub2A1;
			tResult2A=tSub3A1;
			tResult1T=regInSrT;
			tResult2T=regInSrT;

			tResult1B=tSub2B1;
			tResult1S=regInSrS;
		end
		4'h2: begin		/* ADC */
			tResult1A=regInSrT ? tAdd2A1 : tAdd2A0;
			tResult2A=regInSrT ? tAdd3A1 : tAdd3A0;
			tResult1T=tResult1A[32];
			tResult2T=tResult2A[64];

			tResult1B=regInSrS ? tAdd2B1 : tAdd2B0;
			tResult1S=tResult1B[32];
		end
		4'h3: begin		/* SBB */
			tResult1A=regInSrT ? tSub2A0 : tSub2A1;
			tResult2A=regInSrT ? tSub3A0 : tSub3A1;
			tResult1T=!tResult1A[32];
			tResult2T=!tResult2A[64];

			tResult1B=regInSrS ? tSub2B0 : tSub2B1;
			tResult1S=!tResult1B[32];
		end
		
		4'h4: begin		/* TST */
			tResult1A=UV33_XX;
			tResult2A=UV65_XX;
			tResult1T=tTst1ZF;
			tResult2T=tTst2ZF;
			tResult1S=tTst1BZF;
		end
		4'h5: begin		/* AND */
			tResult1A={1'b0, regValRs[31:0] & regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] & regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
		end
		4'h6: begin		/* OR */
			tResult1A={1'b0, regValRs[31:0] | regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] | regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
		end
		4'h7: begin		/* XOR */
			tResult1A={1'b0, regValRs[31:0] ^ regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] ^ regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
		end

		4'h8: begin		/* CMPNE */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
			tResult1T=!tSub1ZF;
			tResult2T=!tSub2ZF;
			tResult1S=!tSub1BZF;
		end
		4'h9: begin		/* CMPHS */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
			tResult1T=!tSub1CF;
			tResult2T=!tSub2CF;
			tResult1S=!tSub1BCF;
		end
		4'hA: begin		/* CMPGE */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
			tResult1T=tSub1ZF || (tSub1SF^tSub1VF);
			tResult2T=tSub2ZF || (tSub2SF^tSub2VF);
			tResult1S=tSub1BZF || (tSub1BSF^tSub1BVF);
		end
		4'hB: begin		/* NOR */
//			tResult1A={1'b0, ~(regValRs[31: 0] | regValRt[31: 0])};
//			tResult2A={1'b0, ~(regValRs[63:32] | regValRt[63:32]),
//				tResult1A[31:0]};
//			tResult1T=regInSrT;
//			tResult2T=regInSrT;

//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
//			tResult1T=regInSrT;
//			tResult2T=regInSrT;
		end

		4'hC: begin		/* CMPEQ */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
			tResult1T=tSub1ZF;
			tResult2T=tSub2ZF;
			tResult1S=tSub1BZF;
		end
		4'hD: begin		/* CMPHI */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
			tResult1T=!tSub1CF && !tSub1ZF;
			tResult2T=!tSub2CF && !tSub2ZF;
			tResult1S=!tSub1BCF && !tSub1BZF;
		end
		4'hE: begin		/* CMPGT */
//			tResult1A=UV33_XX;
//			tResult2A=UV65_XX;
			tResult1T=(tSub1SF^tSub1VF);
			tResult2T=(tSub2SF^tSub2VF);
			tResult1S=(tSub1BSF^tSub1BVF);
		end
		4'hF: begin		/* CSELT */
			tResult1A={1'b0, regInSrT ? regValRs[31: 0] : regValRt[31: 0] };
			tResult2A={1'b0, regInSrT ? regValRs[63: 0] : regValRt[63: 0] };
			tResult1B={1'b0, regInSrS ? regValRs[31: 0] : regValRt[31: 0] };
		end
	endcase

	if(idUIxt[5])
	begin
`ifdef jx2_enable_gsv
		if(idUIxt[4])
		begin
			tRegOutVal = { tResult1B[31:0], tResult1A[31:0] };
			tRegOutSrT = tResult1T;
			tRegOutSrS = tResult1S;
		end
		else
		begin
			tRegOutVal = tResult2A[63:0];
			tRegOutSrT = tResult2T;
			tRegOutSrS = regInSrS;
		end
`else
		tRegOutVal = tResult2A[63:0];
		tRegOutSrT = tResult2T;
		tRegOutSrS = regInSrS;
`endif
	end
	else
	begin
		if(idUIxt[4])
			tRegOutVal = { UV32_00, tResult1A[31:0] };
		else
			tRegOutVal = { tResult1A[31]?UV32_FF:UV32_00, tResult1A[31:0] };
		tRegOutSrT = tResult1T;
		tRegOutSrS = regInSrS;
	end
end

always @(posedge clock)
begin
	idUIxt2			<= idUIxt;
	tRegOutVal2		<= tRegOutVal;
	tRegOutSrT2		<= tRegOutSrT;
	tRegOutSrS2		<= tRegOutSrS;
end

endmodule
