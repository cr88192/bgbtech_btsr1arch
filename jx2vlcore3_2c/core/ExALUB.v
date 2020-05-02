/*
ALU (Lane 2+)

Perform some ALU Operations
May Sign or Zero Extend output.

Leaves out operations which update SR.

idUIxt:
  [7:6]=CC (AL/NV/CT/CF)
  [  5]=QWord
  [  4]=ZExt (0=SX, 1=ZX)
  [3:0]=Op1

If GSV is enabled, Q+ZX = Packed DWord Ops

Op1:
  0000: ADD	/ PADD.L
  0001: SUB	/ PSUB.L
  0010: ADC	/ PADC.L
  0011: SBB	/ PSBB.L
  0100: - TST
  0101: AND
  0110: OR
  0111: XOR
  1000: - CMPNE
  1001: - CMPHS
  1010: - CMPGE
  1011: NOR
  1100: - CMPEQ
  1101: - CMPHI
  1110: - CMPGT
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

`ifdef jx2_enable_clz
`include "ExOpClz.v"
`endif

module ExALUB(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	regValRs,
	regValRt,
	idUCmd,
	idUIxt,
	exHold,
	regInSrST,
	regOutVal,
	regOutSrST
	);

input			clock;
input			reset;

input[63:0]		regValRs;
input[63:0]		regValRt;
input[7:0]		idUCmd;
input[7:0]		idUIxt;
input			exHold;
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

// `ifdef def_true
`ifndef def_true
// `ifdef jx2_enable_clz
wire[7:0]		tClzVal;
ExOpClz	clz(
	clock,		reset,
	idUCmd,		idUIxt,
	regValRs,	tClzVal);
`endif


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


reg[32:0]	tResultu1A;
reg[32:0]	tResultu1B;
reg[64:0]	tResultu2A;

reg[32:0]	tResult1W;
reg[64:0]	tResult2W;
reg[32:0]	tResultShufB;
reg[64:0]	tResultShufW;


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

	tResult1A=UV33_XX;
	tResult2A=UV65_XX;
	tResult1T=regInSrT;
	tResult2T=regInSrT;

	tResult1B=UV33_XX;
	tResult1S=regInSrS;

	tResultShufB = UV33_00;
	tResultShufW = UV65_00;

`ifdef def_true
	case(regValRt[1:0])
		2'b00: tResultShufB[ 7: 0]=regValRs[ 7: 0];
		2'b01: tResultShufB[ 7: 0]=regValRs[15: 8];
		2'b10: tResultShufB[ 7: 0]=regValRs[23:16];
		2'b11: tResultShufB[ 7: 0]=regValRs[31:24];
	endcase
	case(regValRt[3:2])
		2'b00: tResultShufB[15: 8]=regValRs[ 7: 0];
		2'b01: tResultShufB[15: 8]=regValRs[15: 8];
		2'b10: tResultShufB[15: 8]=regValRs[23:16];
		2'b11: tResultShufB[15: 8]=regValRs[31:24];
	endcase
	case(regValRt[5:4])
		2'b00: tResultShufB[23:16]=regValRs[ 7: 0];
		2'b01: tResultShufB[23:16]=regValRs[15: 8];
		2'b10: tResultShufB[23:16]=regValRs[23:16];
		2'b11: tResultShufB[23:16]=regValRs[31:24];
	endcase
	case(regValRt[7:6])
		2'b00: tResultShufB[31:24]=regValRs[ 7: 0];
		2'b01: tResultShufB[31:24]=regValRs[15: 8];
		2'b10: tResultShufB[31:24]=regValRs[23:16];
		2'b11: tResultShufB[31:24]=regValRs[31:24];
	endcase
`endif

`ifdef def_true
	case(regValRt[1:0])
		2'b00: tResultShufW[15:0]=regValRs[15: 0];
		2'b01: tResultShufW[15:0]=regValRs[31:16];
		2'b10: tResultShufW[15:0]=regValRs[47:32];
		2'b11: tResultShufW[15:0]=regValRs[63:48];
	endcase
	case(regValRt[3:2])
		2'b00: tResultShufW[31:16]=regValRs[15: 0];
		2'b01: tResultShufW[31:16]=regValRs[31:16];
		2'b10: tResultShufW[31:16]=regValRs[47:32];
		2'b11: tResultShufW[31:16]=regValRs[63:48];
	endcase
	case(regValRt[5:4])
		2'b00: tResultShufW[47:32]=regValRs[15: 0];
		2'b01: tResultShufW[47:32]=regValRs[31:16];
		2'b10: tResultShufW[47:32]=regValRs[47:32];
		2'b11: tResultShufW[47:32]=regValRs[63:48];
	endcase
	case(regValRt[7:6])
		2'b00: tResultShufW[63:48]=regValRs[15: 0];
		2'b01: tResultShufW[63:48]=regValRs[31:16];
		2'b10: tResultShufW[63:48]=regValRs[47:32];
		2'b11: tResultShufW[63:48]=regValRs[63:48];
	endcase
`endif

	case(idUIxt[3:0])
		4'h0: begin		/* ADD */
			tResult1A=tAdd2A0;
			tResult2A=tAdd3A0;
			tResult1T=regInSrT;
			tResult2T=regInSrT;

			tResult1B=tAdd2B0;
			tResult1S=regInSrS;

			tResult2W = { 1'b0,
				tAdd1D0[15:0], tAdd1C0[15:0],
				tAdd1B0[15:0], tAdd1A0[15:0] };
		end
		4'h1: begin		/* SUB */
			tResult1A=tSub2A1;
			tResult2A=tSub3A1;
			tResult1T=regInSrT;
			tResult2T=regInSrT;

			tResult1B=tSub2B1;
			tResult1S=regInSrS;

			tResult2W = { 1'b0,
                tSub1D1[15:0], tSub1C1[15:0],
				tSub1B1[15:0], tSub1A1[15:0] };
		end
		4'h2: begin		/* ADC */
			tResult1A=regInSrT ? tAdd2A1 : tAdd2A0;
			tResult2A=regInSrT ? tAdd3A1 : tAdd3A0;
			tResult1T=tResult1A[32];
			tResult2T=tResult2A[64];

			tResult1B=regInSrS ? tAdd2B1 : tAdd2B0;
			tResult1S=tResult1B[32];

			tResult2W = { 1'b0, regValRs[63:32], regValRt[31:0] };
		end
		4'h3: begin		/* SBB */
			tResult1A=regInSrT ? tSub2A0 : tSub2A1;
			tResult2A=regInSrT ? tSub3A0 : tSub3A1;
			tResult1T=!tResult1A[32];
			tResult2T=!tResult2A[64];

			tResult1B=regInSrS ? tSub2B0 : tSub2B1;
			tResult1S=!tResult1B[32];

			tResult2W = { 1'b0, regValRs[31:0], regValRt[63:32] };
		end
		
		4'h4: begin		/* TST */
		end
		4'h5: begin		/* AND */
			tResult1A={1'b0, regValRs[31:0] & regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] & regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
			tResult1W = tResultShufB;
			tResult2W = tResultShufW;
		end
		4'h6: begin		/* OR */
			tResult1A={1'b0, regValRs[31:0] | regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] | regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
			tResult2W = { 1'b0, regValRs[31:0], regValRt[31:0] };
		end
		4'h7: begin		/* XOR */
			tResult1A={1'b0, regValRs[31:0] ^ regValRt[31:0]};
			tResult2A={1'b0, regValRs[63:32] ^ regValRt[63:32], tResult1A[31:0]};
			tResult1T=regInSrT;
			tResult2T=regInSrT;
			tResult2W = { 1'b0, regValRs[63:32], regValRt[63:32] };
		end

		4'h8: begin		/* CMPNE */
		end
		4'h9: begin		/* CMPHS */
		end
		4'hA: begin		/* CMPGE */
		end
		4'hB: begin		/* NOR */
		end

		4'hC: begin		/* CMPEQ */
		end
		4'hD: begin		/* CMPHI */
		end
		4'hE: begin		/* CMPGT */
		end
		4'hF: begin		/* CSELT */
			tResult1A={1'b0, regInSrT ? regValRs[31: 0] : regValRt[31: 0] };
			tResult2A={1'b0, regInSrT ? regValRs[63: 0] : regValRt[63: 0] };
			tResult1B={1'b0, regInSrS ? regValRs[31: 0] : regValRt[31: 0] };
		end
	endcase

`ifdef jx2_enable_aluunary
// `ifndef def_true
	tResultu1A=0;
	tResultu1B=0;
	tResultu2A=0;

	case(idUIxt[3:0])
// `ifdef jx2_enable_clz
`ifndef def_true
		4'h0: begin
			tResultu1A = { UV25_00, tClzVal };
			tResultu2A = { UV57_00, tClzVal };
		end
`endif
		
		default: begin
		end
	endcase

	if(idUCmd[5:0]==JX2_UCMD_UNARY)
	begin
		tResult1A = tResultu1A;
		tResult1B = tResultu1B;
		tResult2A = tResultu2A;
	end
`endif

`ifdef jx2_enable_gsv
	if(idUCmd[5:0]==JX2_UCMD_ALUW3)
	begin
//		tResult1A = tResultu1A;
//		tResult1B = tResultu1B;
		tResult1A = tResult1W;
		tResult2A = tResult2W;
	end
`endif

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

`ifdef jx2_debug_alu
	if((idUCmd[5:0]==JX2_UCMD_ALU3) && !exHold)
	begin
		$display("ALU: Op=%X Rs=%X Rt=%X Rn=%X",
			idUIxt,
			regValRs, regValRt, tRegOutVal);
	end

	if((idUCmd[5:0]==JX2_UCMD_ALUCMP) && !exHold)
	begin
		$display("ALUCMP: Op=%X Rs=%X Rt=%X SR.T=%X, S=%d V=%d Z=%d C=%d",
			idUIxt,
			regValRs, regValRt, tRegOutSrT,
			tSub1SF, tSub1VF, tSub1ZF, tSub1CF);
	end
`endif
end

always @(posedge clock)
begin
	if(!exHold)
	begin
		idUIxt2			<= idUIxt;
		tRegOutVal2		<= tRegOutVal;
		tRegOutSrT2		<= tRegOutSrT;
		tRegOutSrS2		<= tRegOutSrS;
	end
end

endmodule
