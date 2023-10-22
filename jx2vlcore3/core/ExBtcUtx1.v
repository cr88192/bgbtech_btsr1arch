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
Extract a pixel from a UTX1 block.

The UTX1 format:
  (31:16): PixBits
  (15:12): D (Ymax-Ymin)
  (11: 8): R (R, Center)
  ( 7: 4): G (G, Center)
  ( 3: 0): B (B, Center)

 */

`ifndef has_ExBtcUtx1
`define has_ExBtcUtx1

`define jx2_utx2_interp

`ifdef jx2_utx2_interp
`include "ExScAddSc511_8F.v"
`endif

`ifdef jx2_enable_btcutx3
// `include "ExBtcUtx3.v"
`include "ExBtcUtx3B.v"
`endif

`include "ExConv_Fp8Exp12.v"

module ExBtcUtx1(
	/* verilator lint_off UNUSED */
	regValRs,
	regValXs,
	regValRt,
	idUIxt,
	regOutVal,
	isLdTex
	);

input[63:0]		regValRs;
input[63:0]		regValXs;
input[ 3:0]		regValRt;
input[8:0]		idUIxt;
input			isLdTex;

output[63:0]	regOutVal;

reg[63:0]	tRegOutVal;
assign	regOutVal = tRegOutVal;


`ifdef jx2_enable_btcutx3
wire[63:0]	tUtx3OutVal;
// ExBtcUtx3	utx3(regValRs, regValXs, regValRt, idUIxt, tUtx3OutVal);
ExBtcUtx3B	utx3(regValRs, regValXs, regValRt, idUIxt, tUtx3OutVal);
`endif


wire[31:0]	tSelRgb32F;
wire[11:0]	tSelRgb32F_A12;
wire[11:0]	tSelRgb32F_R12;
wire[11:0]	tSelRgb32F_G12;
wire[11:0]	tSelRgb32F_B12;
assign	tSelRgb32F = regValRt[0] ? regValRs[63:32] : regValRs[31: 0];

ExConv_Fp8Exp12		fp8expa(tSelRgb32F[31:24], tSelRgb32F_A12, 2'b00);
ExConv_Fp8Exp12		fp8expr(tSelRgb32F[23:16], tSelRgb32F_R12, 2'b00);
ExConv_Fp8Exp12		fp8expg(tSelRgb32F[15: 8], tSelRgb32F_G12, 2'b00);
ExConv_Fp8Exp12		fp8expb(tSelRgb32F[ 7: 0], tSelRgb32F_B12, 2'b00);


reg[15:0]	tValPb;
reg[15:0]	tValPa;
reg[ 7:0]	tValCr;
reg[ 7:0]	tValCg;
reg[ 7:0]	tValCb;
reg[ 7:0]	tValDy;

reg[ 7:0]	tValMr;
reg[ 7:0]	tValMg;
reg[ 7:0]	tValMb;
reg[ 7:0]	tValMa;

reg[ 7:0]	tValNr;
reg[ 7:0]	tValNg;
reg[ 7:0]	tValNb;
reg[ 7:0]	tValNa;

reg[ 7:0]	tValSr;
reg[ 7:0]	tValSg;
reg[ 7:0]	tValSb;
reg[ 7:0]	tValSa;

reg[ 3:0]	tValSelIx;
reg			tValSelB;
reg			tValSelA;
reg			tDoInterp;
reg			tDoAlpha;
reg			tDoUtx3;

reg[15:0]	tSelRgb16;
reg[3:0]	tSelRgb16_E4;

reg[3:0]	tSelRgb16_E4A;
reg[3:0]	tSelRgb16_E4R;
reg[3:0]	tSelRgb16_E4G;
reg[3:0]	tSelRgb16_E4B;

reg[4:0]	tSelRgb16_E5A;
reg[4:0]	tSelRgb16_E5R;
reg[4:0]	tSelRgb16_E5G;
reg[4:0]	tSelRgb16_E5B;

reg[3:0]	tSelRgb16_F4A;
reg[3:0]	tSelRgb16_F4R;
reg[3:0]	tSelRgb16_F4G;
reg[3:0]	tSelRgb16_F4B;

reg[31:0]	tSelRgb32;
reg[31:0]	tSelRgb32F8;

reg[63:0]	tSelRgb64;
reg			tDoSelPix;

`ifdef jx2_utx2_interp

wire[15:0]		tColorA;
wire[15:0]		tColorB;
assign		tColorA		= regValRs[15: 0];
assign		tColorB		= regValRs[31:16];

wire[4:0]		tAlphaA;
wire[4:0]		tAlphaB;
//assign	tAlphaA = { tColorA[10], tColorA[5],
//					tColorA[ 0], tColorA[10],
//					tColorA[ 5] };
//assign	tAlphaB = { tColorB[10], tColorB[5],
//					tColorB[ 0], tColorB[10],
//					tColorB[ 5] };

assign	tAlphaA = { tColorA[10], tColorA[5],
					tColorA[ 0], 2'b00 };
assign	tAlphaB = { tColorB[10], tColorB[5],
					tColorB[ 0], 2'b00 };

wire[31:0]	tInterpA;
wire[31:0]	tInterpB;

ExScAddSc511_8F	interp_Ar(tColorB[14:10], tColorA[14:10], tInterpA[23:16]);
ExScAddSc511_8F	interp_Ag(tColorB[ 9: 5], tColorA[ 9: 5], tInterpA[15: 8]);
ExScAddSc511_8F	interp_Ab(tColorB[ 4: 0], tColorA[ 4: 0], tInterpA[ 7: 0]);
ExScAddSc511_8F	interp_Aa(tAlphaB[ 4: 0], tAlphaA[ 4: 0], tInterpA[31:24]);

ExScAddSc511_8F	interp_Br(tColorA[14:10], tColorB[14:10], tInterpB[23:16]);
ExScAddSc511_8F	interp_Bg(tColorA[ 9: 5], tColorB[ 9: 5], tInterpB[15: 8]);
ExScAddSc511_8F	interp_Bb(tColorA[ 4: 0], tColorB[ 4: 0], tInterpB[ 7: 0]);
ExScAddSc511_8F	interp_Ba(tAlphaA[ 4: 0], tAlphaB[ 4: 0], tInterpB[31:24]);

`endif

always @*
begin
	tDoInterp	= 0;
	tDoAlpha	= 0;
	tDoUtx3		= 0;
	tDoSelPix	= 0;

	tValPb = UV16_XX;
	tValPa = UV16_XX;
	tValMr = UV8_XX;
	tValMg = UV8_XX;
	tValMb = UV8_XX;
	tValMa = UV8_XX;
	tValNr = UV8_XX;
	tValNg = UV8_XX;
	tValNb = UV8_XX;
	tValNa = UV8_XX;

//reg[15:0]	tSelRgb16;
//reg[31:0]	tSelRgb32;

`ifdef def_true
	case(regValRt[1:0])
		2'b00: tSelRgb16 = regValRs[15: 0];
		2'b01: tSelRgb16 = regValRs[31:16];
		2'b10: tSelRgb16 = regValRs[47:32];
		2'b11: tSelRgb16 = regValRs[63:48];
	endcase
	
	if(regValRt[0])
		tSelRgb32 = regValRs[63:32];
	else
		tSelRgb32 = regValRs[31: 0];

	tSelRgb32F8	= tSelRgb32;

	if(idUIxt[5:0]==JX2_UCIX_CONV2_BLKRGB15A)
//	if(!idUIxt[1])
	begin
		tDoSelPix	= 1;
		tSelRgb32	=
			{ (tSelRgb16[15]) ?
					{ tSelRgb16[10], tSelRgb16[5], tSelRgb16[0], UV5_00 } :
					UV8_FF,
				tSelRgb16[14:10], tSelRgb16[14:12],
				tSelRgb16[ 9: 5], tSelRgb16[ 9: 7],
				tSelRgb16[ 4: 0], tSelRgb16[ 4: 2] };
	end

	if(idUIxt[5:0]==JX2_UCIX_CONV2_BLKRGBA32)
	begin
		tDoSelPix	= 1;
	end

// `ifdef def_true
`ifndef def_true
	if(idUIxt[5:0]==JX2_UCIX_CONV2_BLKRGB15F)
	begin
		tSelRgb16_E4 = tSelRgb16[15:12];

		casez(tSelRgb16[11: 8])
			4'b1zzz: begin
				tSelRgb16_E4R = tSelRgb16_E4;
				tSelRgb16_F4R = { tSelRgb16[10: 8], 1'b0 };
			end
			4'b01zz: begin
				tSelRgb16_E4R = tSelRgb16_E4-1;
				tSelRgb16_F4R = { tSelRgb16[9: 8], 2'b0 };
			end
			4'b001z: begin
				tSelRgb16_E4R = tSelRgb16_E4-2;
				tSelRgb16_F4R = { tSelRgb16[8], 3'b0 };
			end
			4'b0001: begin
				tSelRgb16_E4R = tSelRgb16_E4-3;
				tSelRgb16_F4R = 0;
			end
			4'b0000: begin
				tSelRgb16_E4R = 0;
				tSelRgb16_F4R = 0;
			end
		endcase

		casez(tSelRgb16[7: 4])
			4'b1zzz: begin
				tSelRgb16_E4G = tSelRgb16_E4;
				tSelRgb16_F4G = { tSelRgb16[6: 4], 1'b0 };
			end
			4'b01zz: begin
				tSelRgb16_E4G = tSelRgb16_E4-1;
				tSelRgb16_F4G = { tSelRgb16[5: 4], 2'b0 };
			end
			4'b001z: begin
				tSelRgb16_E4G = tSelRgb16_E4-2;
				tSelRgb16_F4G = { tSelRgb16[4], 3'b0 };
			end
			4'b0001: begin
				tSelRgb16_E4G = tSelRgb16_E4-3;
				tSelRgb16_F4G = 0;
			end
			4'b0000: begin
				tSelRgb16_E4G = 0;
				tSelRgb16_F4G = 0;
			end
		endcase

		casez(tSelRgb16[3: 0])
			4'b1zzz: begin
				tSelRgb16_E4B = tSelRgb16_E4;
				tSelRgb16_F4B = { tSelRgb16[2: 0], 1'b0 };
			end
			4'b01zz: begin
				tSelRgb16_E4B = tSelRgb16_E4-1;
				tSelRgb16_F4B = { tSelRgb16[1: 0], 2'b0 };
			end
			4'b001z: begin
				tSelRgb16_E4B = tSelRgb16_E4-2;
				tSelRgb16_F4B = { tSelRgb16[0], 3'b0 };
			end
			4'b0001: begin
				tSelRgb16_E4B = tSelRgb16_E4-3;
				tSelRgb16_F4B = 0;
			end
			4'b0000: begin
				tSelRgb16_E4B = 0;
				tSelRgb16_F4B = 0;
			end
		endcase

//		tSelRgb32F8	=
//			{	8'h70,
//				tSelRgb16_E4R, tSelRgb16_F4R,
//				tSelRgb16_E4G, tSelRgb16_F4G,
//				tSelRgb16_E4B, tSelRgb16_F4B };

		tSelRgb16_E5R = {
			tSelRgb16_E4R[3], !tSelRgb16_E4R[3], tSelRgb16_E4R[2:0] };
		tSelRgb16_E5G = {
			tSelRgb16_E4G[3], !tSelRgb16_E4G[3], tSelRgb16_E4G[2:0] };
		tSelRgb16_E5B = {
			tSelRgb16_E4B[3], !tSelRgb16_E4B[3], tSelRgb16_E4B[2:0] };

		tSelRgb32F8	= { 2'b00,
			tSelRgb16_E5R, tSelRgb16_F4R, 1'b0,
			tSelRgb16_E5G, tSelRgb16_F4G, 1'b0,
			tSelRgb16_E5B, tSelRgb16_F4B, 1'b0 };
	end
`endif

	tSelRgb64 = {
		tSelRgb32[31:24], tSelRgb32[31:24],
		tSelRgb32[23:16], tSelRgb32[23:16],
		tSelRgb32[15: 8], tSelRgb32[15: 8],
		tSelRgb32[ 7: 0], tSelRgb32[ 7: 0]	};

	if(idUIxt[5:0]==JX2_UCIX_CONV2_RGB32UPCK64FU)
	begin
		tDoSelPix	= 1;
		tSelRgb64	= {
			tSelRgb32F_A12, 4'b0,
			tSelRgb32F_R12, 4'b0,
			tSelRgb32F_G12, 4'b0,
			tSelRgb32F_B12, 4'b0
			};
	end
	
`ifdef def_true
	if(idUIxt[5:0]==JX2_UCIX_CONV2_RGB30APCK64F)
	begin
		tDoSelPix	= 1;
		tSelRgb64	= regValRs;
	end
`endif

`ifndef def_true
	if(	(idUIxt[5:0]==JX2_UCIX_CONV2_BLKRGB15F) ||
		(idUIxt[5:0]==JX2_UCIX_CONV2_BLKRGB30A) )
	begin
		tSelRgb16_E4A = tSelRgb32F8[29:26];
		tSelRgb16_E4R = tSelRgb32F8[21:18];
		tSelRgb16_E4G = tSelRgb32F8[14:11];
		tSelRgb16_E4B = tSelRgb32F8[ 6: 3];
	
		tSelRgb16_E5A = {
			tSelRgb16_E4A[3], !tSelRgb16_E4A[3], tSelRgb16_E4A[2:0] };
		tSelRgb16_E5R = {
			tSelRgb16_E4R[3], !tSelRgb16_E4R[3], tSelRgb16_E4R[2:0] };
		tSelRgb16_E5G = {
			tSelRgb16_E4G[3], !tSelRgb16_E4G[3], tSelRgb16_E4G[2:0] };
		tSelRgb16_E5B = {
			tSelRgb16_E4B[3], !tSelRgb16_E4B[3], tSelRgb16_E4B[2:0] };

		tSelRgb16_F4A = tSelRgb32F8[25:22];
		tSelRgb16_F4R = { tSelRgb32F8[17:15], tSelRgb32F8[15] };
		tSelRgb16_F4G = tSelRgb32F8[10: 7];
		tSelRgb16_F4B = { tSelRgb32F8[ 2: 0], tSelRgb32F8[0] };

		if(tSelRgb32F8[31])
		begin
			tSelRgb64 = {
				tSelRgb16_F4A[0]&tSelRgb32F8[30],
					tSelRgb16_E5A, tSelRgb16_F4A, 6'h0,
				tSelRgb16_F4R[0]&tSelRgb32F8[30],
					tSelRgb16_E5R, tSelRgb16_F4R, 6'h0,
				tSelRgb16_F4G[0]&tSelRgb32F8[30],
					tSelRgb16_E5G, tSelRgb16_F4G, 6'h0,
				tSelRgb16_F4B[0]&tSelRgb32F8[30],
					tSelRgb16_E5B, tSelRgb16_F4B, 6'h0 };
		end
		else
		begin
			tSelRgb64 = {
				16'h3C00,
				tSelRgb16_F4R[0] & tSelRgb32F8[30],
					tSelRgb32F8[29:20], 5'h00,
				tSelRgb16_F4G[0] & tSelRgb32F8[30],
					tSelRgb32F8[19:10], 5'h00,
				tSelRgb16_F4B[0] & tSelRgb32F8[30],
					tSelRgb32F8[ 9: 0], 5'h00 };
		end
	end
`endif

	if(!isLdTex)
	begin
		tDoSelPix	= 0;
	end

`endif

`ifdef jx2_enable_btcutx3
	if(idUIxt[1])
	begin
		tDoUtx3		= 1;
	end
	else
`endif
		if(idUIxt[0])
	begin
`ifdef jx2_enable_btcutx2
		/* UTX2 */
		tValPb = {
			regValRs[63], regValRs[61], regValRs[59], regValRs[57],
			regValRs[55], regValRs[53], regValRs[51], regValRs[49],
			regValRs[47], regValRs[45], regValRs[43], regValRs[41],
			regValRs[39], regValRs[37], regValRs[35], regValRs[33]	};
		tValPa = {
			regValRs[62], regValRs[60], regValRs[58], regValRs[56],
			regValRs[54], regValRs[52], regValRs[50], regValRs[48],
			regValRs[46], regValRs[44], regValRs[42], regValRs[40],
			regValRs[38], regValRs[36], regValRs[34], regValRs[32]	};

		tValMr = { regValRs[30:26], regValRs[30:28] };
		tValMg = { regValRs[25:21], regValRs[25:23] };
		tValMb = { regValRs[20:16], regValRs[20:18] };
//		tValMa = 8'hFF;
		tValMa = { tAlphaB, 3'b0 };

		tValNr = { regValRs[14:10], regValRs[14:12] };
		tValNg = { regValRs[ 9: 5], regValRs[ 9: 7] };
		tValNb = { regValRs[ 4: 0], regValRs[ 4: 2] };
//		tValNa = 8'hFF;
		tValNa = { tAlphaA, 3'b0 };

		tDoInterp = !(regValRs[15] ^ regValRs[31]);
		
		if(regValRs[15])
		begin
			tDoAlpha = 1;
`ifndef def_true
			tValMa = {
				regValRs[26], regValRs[21], regValRs[16],
				regValRs[26], regValRs[21], regValRs[16],
				regValRs[26], regValRs[21]	};
			tValNa = {
				regValRs[10], regValRs[5], regValRs[0],
				regValRs[10], regValRs[5], regValRs[0],
				regValRs[10], regValRs[5]	};
`endif

// `ifdef def_true
`ifndef def_true
			tValMa = {
				regValRs[26], regValRs[21], regValRs[16], 5'h0	};
			tValNa = {
				regValRs[10], regValRs[ 5], regValRs[ 0], 5'h0	};
`endif

		end
`endif
	end
	else
	begin
`ifdef jx2_enable_btcutx1
		/* UTX1 */
		tValPb = regValRs[31:16];
		tValPa = 16'h0000;
		tValDy = { regValRs[15:12], regValRs[15:12] };
		tValCr = { regValRs[11: 8], regValRs[11: 8] };
		tValCg = { regValRs[ 7: 4], regValRs[ 7: 4] };
		tValCb = { regValRs[ 3: 0], regValRs[ 3: 0] };
		
		tValMr = tValCr - {1'b0, tValDy[7:1]};
		tValMg = tValCg - {1'b0, tValDy[7:1]};
		tValMb = tValCb - {1'b0, tValDy[7:1]};
		tValMa = 8'hFF;

		tValNr = tValCr + {1'b0, tValDy[7:1]};
		tValNg = tValCg + {1'b0, tValDy[7:1]};
		tValNb = tValCb + {1'b0, tValDy[7:1]};
		tValNa = 8'hFF;

//		tValNr = tValMr + tValDy;
//		tValNg = tValMg + tValDy;
//		tValNb = tValMb + tValDy;
//		tValNa = 8'hFF;
`endif
	end

//	tValSelIx = { regValRt[3], regValRt[1], regValRt[2], regValRt[0] };
	tValSelIx = regValRt[3:0];
	tValSelB = tValPb[tValSelIx];
	tValSelA = tValPa[tValSelIx];

`ifdef jx2_utx2_interp
	if(tDoInterp)
	begin
		case( { tValSelB, tValSelA } )
			2'b00: begin
				tValSa = tDoAlpha ? tValMa : 8'hFF;
				tValSr = tValMr;
				tValSg = tValMg;
				tValSb = tValMb;
			end
			2'b01: begin
				tValSa = tDoAlpha ? tInterpB[31:24] : 8'hFF;
				tValSr = tInterpB[23:16];
				tValSg = tInterpB[15: 8];
				tValSb = tInterpB[ 7: 0];
			end
			2'b10: begin
				tValSa = tDoAlpha ? tInterpA[31:24] : 8'hFF;
				tValSr = tInterpA[23:16];
				tValSg = tInterpA[15: 8];
				tValSb = tInterpA[ 7: 0];
			end
			2'b11: begin
				tValSa = tDoAlpha ? tValNa : 8'hFF;
				tValSr = tValNr;
				tValSg = tValNg;
				tValSb = tValNb;
			end
		endcase
	end
	else
	begin
		tValSa = tValSelA ? tValNa : tValMa;
		tValSr = tValSelB ? tValNr : tValMr;
		tValSg = tValSelB ? tValNg : tValMg;
		tValSb = tValSelB ? tValNb : tValMb;
	end
`else
	tValSr = tValSelB ? tValNr : tValMr;
	tValSg = tValSelB ? tValNg : tValMg;
	tValSb = tValSelB ? tValNb : tValMb;
	tValSa = tValSelA ? tValNa : tValMa;
`endif
	
	tRegOutVal = {
		tValSa, tValSa,
		tValSr, tValSr,
		tValSg, tValSg,
		tValSb, tValSb };

`ifdef jx2_enable_btcutx3
	if(tDoUtx3)
		tRegOutVal = tUtx3OutVal;
`endif

`ifdef def_true
	if(tDoSelPix)
		tRegOutVal = tSelRgb64;
`endif

end

endmodule

`endif

