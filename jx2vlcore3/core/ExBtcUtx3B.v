/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Extract a pixel from a UTX3(B) block.

Basic Format:
* ( 31: 0): ColorA
* ( 63:32): ColorB
* ( 95:64): Selectors (RGB)
* (127:96): Selectors (Alpha)

Selectors for RGB and Alpha will be 4x4x2:
* 00: ColorB
* 01: 2/3*ColorB + 1/3*ColorA
* 10: 1/3*ColorB + 2/3*ColorA
* 11: ColorA

 */

`include "ExScAddSc511B_8F.v"
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
input[8:0]		idUIxt;

output[63:0]	regOutVal;

reg[63:0]	tRegOutVal;
assign	regOutVal = tRegOutVal;

reg[7:0]	tValSr;
reg[7:0]	tValSg;
reg[7:0]	tValSb;
reg[7:0]	tValSa;

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

wire[7:0]		tColorA_R;
wire[7:0]		tColorA_G;
wire[7:0]		tColorA_B;
wire[7:0]		tColorA_A;
wire[7:0]		tColorB_R;
wire[7:0]		tColorB_G;
wire[7:0]		tColorB_B;
wire[7:0]		tColorB_A;

wire[7:0]	tInterpA_R;
wire[7:0]	tInterpA_G;
wire[7:0]	tInterpA_B;
wire[7:0]	tInterpB_R;
wire[7:0]	tInterpB_G;
wire[7:0]	tInterpB_B;

ExScAddSc511B_8F	interp_Ar(tColorB_R, tColorA_R, tInterpA_R);
ExScAddSc511B_8F	interp_Ag(tColorB_G, tColorA_G, tInterpA_G);
ExScAddSc511B_8F	interp_Ab(tColorB_B, tColorA_B, tInterpA_B);
ExScAddSc511B_8F	interp_Br(tColorA_R, tColorB_R, tInterpB_R);
ExScAddSc511B_8F	interp_Bg(tColorA_G, tColorB_G, tInterpB_G);
ExScAddSc511B_8F	interp_Bb(tColorA_B, tColorB_B, tInterpB_B);

wire[7:0]	tInterpA_A;
wire[7:0]	tInterpB_A;
ExScAddSc511B_8F	interp_Aa(tColorB_A, tColorA_A, tInterpA_A);
ExScAddSc511B_8F	interp_Ba(tColorA_A, tColorB_A, tInterpB_A);

assign	tColorB_A	= regValRs[63:56];
assign	tColorB_R	= regValRs[55:48];
assign	tColorB_G	= regValRs[47:40];
assign	tColorB_B	= regValRs[39:32];
assign	tColorA_A	= regValRs[31:24];
assign	tColorA_R	= regValRs[23:16];
assign	tColorA_G	= regValRs[15: 8];
assign	tColorA_B	= regValRs[ 7: 0];


wire[11:0]	tExp8_R;
wire[11:0]	tExp8_G;
wire[11:0]	tExp8_B;
wire[11:0]	tExp8_A;

wire[1:0]		tExpIsSign;
assign		tExpIsSign = 2'b10;

ExConv_Fp8Exp12		exp8_Ar(tValSr, tExp8_R, tExpIsSign);
ExConv_Fp8Exp12		exp8_Ag(tValSg, tExp8_G, tExpIsSign);
ExConv_Fp8Exp12		exp8_Ab(tValSb, tExp8_B, tExpIsSign);
ExConv_Fp8Exp12		exp8_Aa(tValSa, tExp8_A, tExpIsSign);

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
end

always @*
begin
//	tValTa = idUIxt[0] ? { tValSa, 4'h0 } : tExp8_A;
//	tValTr = idUIxt[0] ? { tValSr, 4'h0 } : tExp8_R;
//	tValTg = idUIxt[0] ? { tValSg, 4'h0 } : tExp8_G;
//	tValTb = idUIxt[0] ? { tValSb, 4'h0 } : tExp8_B;

//	tValTa = { tValSa, 4'h0 };
//	tValTr = { tValSr, 4'h0 };
//	tValTg = { tValSg, 4'h0 };
//	tValTb = { tValSb, 4'h0 };

	if(idUIxt[0])
	begin
		tValTa = { tValSa, 4'h0 };
		tValTr = { tValSr, 4'h0 };
		tValTg = { tValSg, 4'h0 };
		tValTb = { tValSb, 4'h0 };
	end
	else
	begin
		tValTa = tExp8_A;
		tValTr = tExp8_R;
		tValTg = tExp8_G;
		tValTb = tExp8_B;
	end

	tRegOutVal = {
		tValTa, 4'h0,	tValTr, 4'h0,
		tValTg, 4'h0,	tValTb, 4'h0 };
end

endmodule
