/*
Extract a pixel from a UTX3 block.

 */

`include "ExScAddSc511_12F.v"
`include "ExConv_Fp8Exp12.v"

module ExBtcUtx3(
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
input[8:0]		idUIxt;

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

reg			tDoInterp;
reg			tDoAlpha;
reg			tDoShr;
reg			tDoShrA;
reg[1:0]	tRgbPerm;

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

`ifdef def_true
ExScAddSc511_12F	interp_Ar(tColorB_R, tColorA_R, tInterpA_R);
ExScAddSc511_12F	interp_Ag(tColorB_G, tColorA_G, tInterpA_G);
ExScAddSc511_12F	interp_Ab(tColorB_B, tColorA_B, tInterpA_B);
ExScAddSc511_12F	interp_Br(tColorA_R, tColorB_R, tInterpB_R);
ExScAddSc511_12F	interp_Bg(tColorA_G, tColorB_G, tInterpB_G);
ExScAddSc511_12F	interp_Bb(tColorA_B, tColorB_B, tInterpB_B);
`endif

`ifdef def_true
wire[11:0]	tInterpA_A;
wire[11:0]	tInterpB_A;
ExScAddSc511_12F	interp_Aa(tColorB_A, tColorA_A, tInterpA_A);
ExScAddSc511_12F	interp_Ba(tColorA_A, tColorB_A, tInterpB_A);
`endif


// `ifdef def_true
`ifndef def_true

reg[11:0]	tInterpA_A;
reg[11:0]	tInterpB_A;

wire[7:0]		tInterpA_A8;
wire[7:0]		tInterpB_A8;
wire[4:0]		tAlphaA;
wire[4:0]		tAlphaB;
ExScAddSc511_8F	interp_Aa(tAlphaB[ 4: 0], tAlphaA[ 4: 0], tInterpA_A8);
ExScAddSc511_8F	interp_Ba(tAlphaA[ 4: 0], tAlphaB[ 4: 0], tInterpB_A8);

wire[11:0]			tExpInterpA_A;
wire[11:0]			tExpInterpB_A;
ExConv_Fp8Exp12	exp8i_Aa(tInterpA_A8, tExpInterpA_A, 1'b0);
ExConv_Fp8Exp12	exp8i_Ba(tInterpB_A8, tExpInterpB_A, 1'b0);

`endif

wire[7:0]	tFp8A_R;
wire[7:0]	tFp8A_G;
wire[7:0]	tFp8A_B;
wire[7:0]	tFp8A_A;
wire[7:0]	tFp8B_R;
wire[7:0]	tFp8B_G;
wire[7:0]	tFp8B_B;
wire[7:0]	tFp8B_A;

assign	tFp8A_A	= { regValRs[29:22]       };
assign	tFp8A_R	= { regValRs[21:15], 1'b0 };
assign	tFp8A_G	= { regValRs[14: 7]       };
assign	tFp8A_B	= { regValRs[ 6: 0], 1'b0 };
assign	tFp8B_A	= { regValRs[61:54]       };
assign	tFp8B_R	= { regValRs[53:47], 1'b0 };
assign	tFp8B_G	= { regValRs[46:39]       };
assign	tFp8B_B	= { regValRs[38:32], 1'b0 };

`ifndef def_true
assign	tAlphaA = tFp8A_A[7:3];
assign	tAlphaB = tFp8B_A[7:3];
`endif

wire[11:0]	tExpA_R;
wire[11:0]	tExpA_G;
wire[11:0]	tExpA_B;
wire[11:0]	tExpA_A;
wire[11:0]	tExpB_R;
wire[11:0]	tExpB_G;
wire[11:0]	tExpB_B;
wire[11:0]	tExpB_A;

wire		tExpIsSign;
assign		tExpIsSign = !regValRs[30];

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
//	if(regValRs[31])
	if(1'b1)
	begin
		tDoShrA		= !regValRs[30];

//		if(idUIxt[0])
		if(1'b1)
		begin
			tColorA_A	= { tFp8A_A, tFp8A_A[7:4] };
			tColorA_R	= { tFp8A_R, tFp8A_R[7:4] };
			tColorA_G	= { tFp8A_G, tFp8A_G[7:4] };
			tColorA_B	= { tFp8A_B, tFp8A_B[7:4] };
			tColorB_A	= { tFp8B_A, tFp8B_A[7:4] };
			tColorB_R	= { tFp8B_R, tFp8B_R[7:4] };
			tColorB_G	= { tFp8B_G, tFp8B_G[7:4] };
			tColorB_B	= { tFp8B_B, tFp8B_B[7:4] };
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
	else
	begin
		tColorA_R	= { regValRs[29:20], 2'b0 };
		tColorA_G	= { regValRs[19:10], 2'b0 };
		tColorA_B	= { regValRs[ 9: 0], 2'b0 };
		tColorB_R	= { regValRs[61:52], 2'b0 };
		tColorB_G	= { regValRs[51:42], 2'b0 };
		tColorB_B	= { regValRs[41:32], 2'b0 };

		if(idUIxt[0])
		begin
			tColorA_A	= 12'hFFF;
			tColorB_A	= 12'hFFF;
		end
		else
		begin
			tColorA_A	= 12'h3C0;
			tColorB_A	= 12'h3C0;
		end
	end
end

always @*
begin
	tDoInterp	= 0;
	tDoAlpha	= 0;
	tDoShr		= 0;

//	if(regValRs[31])
	if(1'b1)
	begin
//		tDoShr		= regValRs[30];		
		tDoShr		= 0;
		tDoAlpha	= 1;
		tRgbPerm	= regValRs[63:62];

`ifndef def_true
		if(idUIxt[0])
		begin
			tInterpA_A = { tInterpA_A8, tInterpA_A8[7:4] };
			tInterpB_A = { tInterpB_A8, tInterpB_A8[7:4] };
		end
		else
		begin
			tInterpA_A = tExpInterpA_A;
			tInterpB_A = tExpInterpB_A;
		end
`endif
	end
	else
	begin
		tDoShr		= regValRs[30];

		tDoInterp	= ( regValRs[63:62] == 2'b00 );
		tRgbPerm	= regValRs[63:62];

`ifndef def_true
		tInterpA_A	= tColorA_A;
		tInterpB_A	= tColorB_A;
`endif
	end

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
	tSelIxG = tDoAlpha ? tSelIxRB : tSelIxGA;
	tSelIxB = tSelIxRB;
	tSelIxA = tSelIxGA;
	
	if(tDoInterp)
	begin
		tSelIxR = tSelIxGA;
		tSelIxG = tSelIxGA;
		tSelIxB = tSelIxGA;
	end

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

// `ifndef def_true
`ifdef def_true
	if(tDoShr)
	begin
		tValSr = { 1'b0, tValSr[11:1] };
		tValSg = { 1'b0, tValSg[11:1] };
		tValSb = { 1'b0, tValSb[11:1] };
//		if(tDoAlpha)
//			tValSa = { 1'b0, tValSa[11:1] };
	end
`endif

	tValTr = tValSr;
	tValTg = tValSg;
	tValTb = tValSb;
	tValTa = tValSa;

`ifndef def_true
// `ifdef def_true
	case( { tDoAlpha, tRgbPerm } )
		3'b000: begin
		end
		3'b001: begin
			tValTg = tValSb;
			tValTb = tValSg;
		end
		3'b010: begin
		end
		3'b011: begin
			tValTr = tValSg;
			tValTg = tValSr;
		end
		3'b100: begin
		end
		3'b101: begin
			tValTr = tValSa;
			tValTa = tValSr;
		end
		3'b110: begin
			tValTg = tValSa;
			tValTa = tValSg;
		end
		3'b111: begin
			tValTb = tValSa;
			tValTa = tValSb;
		end
	endcase
`endif

	tRegOutVal = {
		tValTa, tValTa[11:8],
		tValTr, tValTr[11:8],
		tValTg, tValTg[11:8],
		tValTb, tValTb[11:8] };
end

endmodule
