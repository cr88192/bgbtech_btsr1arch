/*
Extract a pixel from a UTX3(B) block.

 */

`include "ExScAddSc511_12F.v"
`include "ExConv_Fp8Exp12.v"

module ExBtcUtx3B(
	/* verilator lint_off UNUSED */
	regValRs,
	regValXs,
	regValRt,
	idUIxt,
	regOutVal
	);

input[63:0]		regValRs;
input[63:0]		regValXs;
input[ 3:0]		regValRt;
input[7:0]		idUIxt;

output[63:0]	regOutVal;

reg[63:0]	tRegOutVal;
assign	regOutVal = tRegOutVal;

reg[11:0]	tValSr;
reg[11:0]	tValSg;
reg[11:0]	tValSb;
reg[11:0]	tValSa;

reg[11:0]	tValTr;
reg[11:0]	tValTg;
reg[11:0]	tValTb;
reg[11:0]	tValTa;

reg[ 3:0]	tValSelIx;
reg			tValSelB;
reg			tValSelA;

reg[1:0]	tSelIxRB;
reg[1:0]	tSelIxGA;

reg[1:0]	tSelIxR;
reg[1:0]	tSelIxG;
reg[1:0]	tSelIxB;
reg[1:0]	tSelIxA;

reg[11:0]		tColorA_R;
reg[11:0]		tColorA_G;
reg[11:0]		tColorA_B;
reg[11:0]		tColorA_A;
reg[11:0]		tColorB_R;
reg[11:0]		tColorB_G;
reg[11:0]		tColorB_B;
reg[11:0]		tColorB_A;

wire[11:0]	tInterpA_R;
wire[11:0]	tInterpA_G;
wire[11:0]	tInterpA_B;
wire[11:0]	tInterpB_R;
wire[11:0]	tInterpB_G;
wire[11:0]	tInterpB_B;

ExScAddSc511_12F	interp_Ar(tColorB_R, tColorA_R, tInterpA_R);
ExScAddSc511_12F	interp_Ag(tColorB_G, tColorA_G, tInterpA_G);
ExScAddSc511_12F	interp_Ab(tColorB_B, tColorA_B, tInterpA_B);
ExScAddSc511_12F	interp_Br(tColorA_R, tColorB_R, tInterpB_R);
ExScAddSc511_12F	interp_Bg(tColorA_G, tColorB_G, tInterpB_G);
ExScAddSc511_12F	interp_Bb(tColorA_B, tColorB_B, tInterpB_B);

wire[11:0]	tInterpA_A;
wire[11:0]	tInterpB_A;
ExScAddSc511_12F	interp_Aa(tColorB_A, tColorA_A, tInterpA_A);
ExScAddSc511_12F	interp_Ba(tColorA_A, tColorB_A, tInterpB_A);

wire[7:0]	tFp8A_R;
wire[7:0]	tFp8A_G;
wire[7:0]	tFp8A_B;
wire[7:0]	tFp8A_A;
wire[7:0]	tFp8B_R;
wire[7:0]	tFp8B_G;
wire[7:0]	tFp8B_B;
wire[7:0]	tFp8B_A;

// assign	tFp8A_A	= { regValRs[29:22]       };
// assign	tFp8A_R	= { regValRs[21:15], 1'b0 };
// assign	tFp8A_G	= { regValRs[14: 7]       };
// assign	tFp8A_B	= { regValRs[ 6: 0], 1'b0 };
// assign	tFp8B_A	= { regValRs[61:54]       };
// assign	tFp8B_R	= { regValRs[53:47], 1'b0 };
// assign	tFp8B_G	= { regValRs[46:39]       };
// assign	tFp8B_B	= { regValRs[38:32], 1'b0 };

assign	tFp8B_A	= regValRs[63:56];
assign	tFp8B_R	= regValRs[55:48];
assign	tFp8B_G	= regValRs[47:40];
assign	tFp8B_B	= regValRs[39:32];
assign	tFp8A_A	= regValRs[31:24];
assign	tFp8A_R	= regValRs[23:16];
assign	tFp8A_G	= regValRs[15: 8];
assign	tFp8A_B	= regValRs[ 7: 0];


wire[11:0]	tExpA_R;
wire[11:0]	tExpA_G;
wire[11:0]	tExpA_B;
wire[11:0]	tExpA_A;
wire[11:0]	tExpB_R;
wire[11:0]	tExpB_G;
wire[11:0]	tExpB_B;
wire[11:0]	tExpB_A;

wire[1:0]		tExpIsSign;
// assign		tExpIsSign = !regValRs[30];
assign		tExpIsSign = 2'b10;

ExConv_Fp8Exp12		exp8_Ar(tFp8A_R, tExpA_R, tExpIsSign);
ExConv_Fp8Exp12		exp8_Ag(tFp8A_G, tExpA_G, tExpIsSign);
ExConv_Fp8Exp12		exp8_Ab(tFp8A_B, tExpA_B, tExpIsSign);
ExConv_Fp8Exp12		exp8_Aa(tFp8A_A, tExpA_A, tExpIsSign);

ExConv_Fp8Exp12		exp8_Br(tFp8B_R, tExpB_R, tExpIsSign);
ExConv_Fp8Exp12		exp8_Bg(tFp8B_G, tExpB_G, tExpIsSign);
ExConv_Fp8Exp12		exp8_Bb(tFp8B_B, tExpB_B, tExpIsSign);
ExConv_Fp8Exp12		exp8_Ba(tFp8B_A, tExpB_A, tExpIsSign);

always @*
begin
//	if(idUIxt[0])
	if(1'b0)
	begin
//		tColorA_A	= { tFp8A_A, tFp8A_A[7:4] };
//		tColorA_R	= { tFp8A_R, tFp8A_R[7:4] };
//		tColorA_G	= { tFp8A_G, tFp8A_G[7:4] };
//		tColorA_B	= { tFp8A_B, tFp8A_B[7:4] };
//		tColorB_A	= { tFp8B_A, tFp8B_A[7:4] };
//		tColorB_R	= { tFp8B_R, tFp8B_R[7:4] };
//		tColorB_G	= { tFp8B_G, tFp8B_G[7:4] };
//		tColorB_B	= { tFp8B_B, tFp8B_B[7:4] };

		tColorA_A	= { tFp8A_A, 4'h0 };
		tColorA_R	= { tFp8A_R, 4'h0 };
		tColorA_G	= { tFp8A_G, 4'h0 };
		tColorA_B	= { tFp8A_B, 4'h0 };
		tColorB_A	= { tFp8B_A, 4'h0 };
		tColorB_R	= { tFp8B_R, 4'h0 };
		tColorB_G	= { tFp8B_G, 4'h0 };
		tColorB_B	= { tFp8B_B, 4'h0 };
	end
	else
	begin
		tColorA_R	= tExpA_R;
		tColorA_G	= tExpA_G;
		tColorA_B	= tExpA_B;
		tColorA_A	= tExpA_A;

		tColorB_R	= tExpB_R;
		tColorB_G	= tExpB_G;
		tColorB_B	= tExpB_B;
		tColorB_A	= tExpB_A;
	end
end

always @*
begin
	tValSelIx = regValRt[3:0];
	case(tValSelIx)
		4'h0: begin
			tSelIxGA=regValXs[33:32];
			tSelIxRB=regValXs[ 1: 0];
		end
		4'h1: begin
			tSelIxGA=regValXs[35:34];
			tSelIxRB=regValXs[ 3: 2];
		end
		4'h2: begin
			tSelIxGA=regValXs[37:36];
			tSelIxRB=regValXs[ 5: 4];
		end
		4'h3: begin
			tSelIxGA=regValXs[39:38];
			tSelIxRB=regValXs[ 7: 6];
		end
		4'h4: begin
			tSelIxGA=regValXs[41:40];
			tSelIxRB=regValXs[ 9: 8];
		end
		4'h5: begin
			tSelIxGA=regValXs[43:42];
			tSelIxRB=regValXs[11:10];
		end
		4'h6: begin
			tSelIxGA=regValXs[45:44];
			tSelIxRB=regValXs[13:12];
		end
		4'h7: begin
			tSelIxGA=regValXs[47:46];
			tSelIxRB=regValXs[15:14];
		end
		4'h8: begin
			tSelIxGA=regValXs[49:48];
			tSelIxRB=regValXs[17:16];
		end
		4'h9: begin
			tSelIxGA=regValXs[51:50];
			tSelIxRB=regValXs[19:18];
		end
		4'hA: begin
			tSelIxGA=regValXs[53:52];
			tSelIxRB=regValXs[21:20];
		end
		4'hB: begin
			tSelIxGA=regValXs[55:54];
			tSelIxRB=regValXs[23:22];
		end
		4'hC: begin
			tSelIxGA=regValXs[57:56];
			tSelIxRB=regValXs[25:24];
		end
		4'hD: begin
			tSelIxGA=regValXs[59:58];
			tSelIxRB=regValXs[27:26];
		end
		4'hE: begin
			tSelIxGA=regValXs[61:60];
			tSelIxRB=regValXs[29:28];
		end
		4'hF: begin
			tSelIxGA=regValXs[63:62];
			tSelIxRB=regValXs[31:30];
		end
	endcase

	tSelIxR = tSelIxRB;
	tSelIxG = tSelIxRB;
	tSelIxB = tSelIxRB;
	tSelIxA = tSelIxGA;

	case( tSelIxR )
		2'b00:	tValSr = tColorB_R;
		2'b01:	tValSr = tInterpB_R;
		2'b10:	tValSr = tInterpA_R;
		2'b11:	tValSr = tColorA_R;
	endcase
	case( tSelIxG )
		2'b00:	tValSg = tColorB_G;
		2'b01:	tValSg = tInterpB_G;
		2'b10:	tValSg = tInterpA_G;
		2'b11:	tValSg = tColorA_G;
	endcase
	case( tSelIxB )
		2'b00:	tValSb = tColorB_B;
		2'b01:	tValSb = tInterpB_B;
		2'b10:	tValSb = tInterpA_B;
		2'b11:	tValSb = tColorA_B;
	endcase
	case( tSelIxA )
		2'b00:	tValSa = tColorB_A;
		2'b01:	tValSa = tInterpB_A;
		2'b10:	tValSa = tInterpA_A;
		2'b11:	tValSa = tColorA_A;
	endcase

//	tRegOutVal = {
//		tValSa, tValSa[11:8],
//		tValSr, tValSr[11:8],
//		tValSg, tValSg[11:8],
//		tValSb, tValSb[11:8] };

	tRegOutVal = {
		tValSa, 4'h0,	tValSr, 4'h0,
		tValSg, 4'h0,	tValSb, 4'h0 };
end

endmodule
