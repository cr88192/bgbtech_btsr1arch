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
Add With Scale

idUIxt:
  [7:6]=CC (AL/NV/CT/CF)
  [5:4]=Type (B/W/L/Q)
  [  3]=?
  [  2]=ZExt (0=SX, 1=ZX)
  [1:0]=Scale (B/W/L/Q)

 */

`ifndef HAS_EXAGUC
`define HAS_EXAGUC

module ExAGUC(
	regValRm,
	regValRi,
	regValImm,
	regValXm,
	idUCmd,
	idUIxt,
	regOutAddr,
	addrEnJq,
	regBoundX,
	regOutIsOob,
	regOutXLeaHi,
	regOutXLeaTag
	);

// input[47:0]		regValRm;
// input[47:0]		regValRi;
input[63:0]		regValRm;
input[63:0]		regValRi;
input[15:0]		regValImm;
input[47:0]		regValXm;

input[8:0]		idUCmd;
input[8:0]		idUIxt;
input			addrEnJq;
input[33:0]		regBoundX;

output[47:0]	regOutAddr;
output			regOutIsOob;

output[63:0]	regOutXLeaHi;
output[15:0]	regOutXLeaTag;

reg[47:0]	tRegValRi;
reg[47:0]	tRiSc0;
reg[47:0]	tRiSc1;
reg[47:0]	tRiSc2;
reg[47:0]	tRiSc3;
reg[47:0]	tRiSc;

reg[15:0]	tRiDi;

reg[47:0]	tAddrSc0;
// reg[47:0]	tAddrSc1;
// reg[47:0]	tAddrSc2;
// reg[47:0]	tAddrSc3;

`ifdef jx2_agu_ridisp
reg[17:0]	tAddrSc0A;
`else
reg[16:0]	tAddrSc0A;
`endif

// reg[16:0]	tAddrSc1A;
// reg[16:0]	tAddrSc2A;
// reg[16:0]	tAddrSc3A;

reg[16:0]	tAddrSc0B;
// reg[16:0]	tAddrSc1B;
// reg[16:0]	tAddrSc2B;
// reg[16:0]	tAddrSc3B;

reg[16:0]	tAddrSc0B0;
// reg[16:0]	tAddrSc1B0;
// reg[16:0]	tAddrSc2B0;
// reg[16:0]	tAddrSc3B0;
reg[16:0]	tAddrSc0B1;
// reg[16:0]	tAddrSc1B1;
// reg[16:0]	tAddrSc2B1;
// reg[16:0]	tAddrSc3B1;

reg[16:0]	tAddrSc0C0;
// reg[16:0]	tAddrSc1C0;
// reg[16:0]	tAddrSc2C0;
// reg[16:0]	tAddrSc3C0;
reg[16:0]	tAddrSc0C1;
// reg[16:0]	tAddrSc1C1;
// reg[16:0]	tAddrSc2C1;
// reg[16:0]	tAddrSc3C1;

reg[15:0]	tAddrSc0C;
// reg[15:0]	tAddrSc1C;
// reg[15:0]	tAddrSc2C;
// reg[15:0]	tAddrSc3C;

reg			tCaVal0A;
reg			tCaVal0B;
reg			tCaVal0C;

reg			tBndOob;
reg			tBndOor;

reg[47:0]	tAddr;
reg			tRegOutIsOob;
reg			tIsStore;
reg			tIsConst;

reg			tIsLdTex;

reg[27:0]	tBoundUp;
reg[27:0]	tBoundDn;
reg[27:0]	tBoundUp1;
reg[27:0]	tBoundDn1;

reg[63:0]	tRegOutXLeaHi;
reg[15:0]	tRegOutXLeaTag;
assign	regOutXLeaHi = tRegOutXLeaHi;
assign	regOutXLeaTag = tRegOutXLeaTag;

assign		regOutAddr = tAddr;
assign		regOutIsOob = tRegOutIsOob;

reg[11:0]	tRegBoundAdj;
reg[12:0]	tRegBoundSum;
reg[19:0]	tRegBndAdjAddP;
reg			tRegBndAdjC0;

reg[11:0]	tRegTexS;
reg[11:0]	tRegTexT;
reg[23:0]	tRegTexMort;
reg[23:0]	tRegTexMask;
reg[23:0]	tRegTexPix;
reg[23:0]	tRegTexBix;


always @*
begin
//	tRegValRi = regValRi;
//	tRegValRi = { regValRi[31] ? 16'hFFFF : 16'h0000, regValRi[31:0] };
	tRegValRi = { regValRi[32] ? 15'h7FFF : 15'h0000, regValRi[32:0] };

	tRiSc0 = tRegValRi;
	tRiSc1 = { tRegValRi[46:0], 1'b0 };
	tRiSc2 = { tRegValRi[45:0], 2'b0 };
	tRiSc3 = { tRegValRi[44:0], 3'b0 };

	case(idUIxt[1:0])
		2'b00:	tRiSc=tRiSc0;
		2'b01:	tRiSc=tRiSc1;
		2'b10:	tRiSc=tRiSc2;
		2'b11:	tRiSc=tRiSc3;
	endcase

	tRegOutXLeaHi	= { regBoundX[27:12], regValXm };
	tRegOutXLeaTag	= 0;
	tRegBoundAdj	= 0;


`ifdef jx2_agu_ldtex
	tRegTexS = regValRi[27:16];
	tRegTexT = regValRi[59:48];
	tRegTexMort = {
		tRegTexT[11], tRegTexS[11],	tRegTexT[10], tRegTexS[10],
		tRegTexT[ 9], tRegTexS[ 9],	tRegTexT[ 8], tRegTexS[ 8],
		tRegTexT[ 7], tRegTexS[ 7],	tRegTexT[ 6], tRegTexS[ 6],
		tRegTexT[ 5], tRegTexS[ 5],	tRegTexT[ 4], tRegTexS[ 4],
		tRegTexT[ 3], tRegTexS[ 3],	tRegTexT[ 2], tRegTexS[ 2],
		tRegTexT[ 1], tRegTexS[ 1],	tRegTexT[ 0], tRegTexS[ 0]
		};
	casez(regValRm[56:52])
		5'h00: tRegTexMask = 24'h000000;
		5'h01: tRegTexMask = 24'h000001;
		5'h02: tRegTexMask = 24'h000003;
		5'h03: tRegTexMask = 24'h000007;
		5'h04: tRegTexMask = 24'h00000F;
		5'h05: tRegTexMask = 24'h00001F;
		5'h06: tRegTexMask = 24'h00003F;
		5'h07: tRegTexMask = 24'h00007F;
		5'h08: tRegTexMask = 24'h0000FF;
		5'h09: tRegTexMask = 24'h0001FF;
		5'h0A: tRegTexMask = 24'h0003FF;
		5'h0B: tRegTexMask = 24'h0007FF;
		5'h0C: tRegTexMask = 24'h000FFF;
		5'h0D: tRegTexMask = 24'h001FFF;
		5'h0E: tRegTexMask = 24'h003FFF;
		5'h0F: tRegTexMask = 24'h007FFF;
		5'h10: tRegTexMask = 24'h00FFFF;
		5'h11: tRegTexMask = 24'h01FFFF;
		5'h12: tRegTexMask = 24'h03FFFF;
		5'h13: tRegTexMask = 24'h07FFFF;
		5'h14: tRegTexMask = 24'h0FFFFF;
		5'h15: tRegTexMask = 24'h1FFFFF;
		5'h16: tRegTexMask = 24'h3FFFFF;
		5'h17: tRegTexMask = 24'h7FFFFF;
		5'h18: tRegTexMask = 24'hFFFFFF;
		default:	tRegTexMask = 24'hFFFFFF;
	endcase
	
	tRegTexPix = tRegTexMort & tRegTexMask;
	
	tRegTexBix = { 4'h0, tRegTexPix[23:4] };

	tIsLdTex = (idUCmd[5:0] == JX2_UCMD_FMOV_MR) &&
		(idUIxt[5:4]==2'b11);

	if(tIsLdTex)
	begin
//		tRiSc = { 21'h00, tRegTexBix, 3'b000 };

		casez(regValRm[59:57])
//			3'b00z: tRiSc = { 21'h00, tRegTexBix, 3'h0 };
//			3'b01z: tRiSc = { 20'h00, tRegTexBix, 4'h0 };

			3'b00z: tRiSc = { 25'h00, tRegTexPix[23:4], 3'h0 };
			3'b01z: tRiSc = { 24'h00, tRegTexPix[23:4], 4'h0 };
			3'b10z: tRiSc = { 23'h00, tRegTexPix[23:2], 3'h0 };
			3'b11z: tRiSc = { 22'h00, tRegTexPix[23:1], 3'h0 };
		endcase
	end
`endif


`ifdef jx2_agu_ribound

	tRegOutIsOob	= 0;
	tIsConst		= 0;
	tIsStore = (idUCmd[5:0] == JX2_UCMD_MOV_RM);

	tBoundUp	= regBoundX[27:0];
	tBoundDn	= 0;
	tBndOor		= 0;
	
	tRegBndAdjAddP	= (regValRm[19:0] + tRiSc[19:0]) ^
		(regValRm[19:0] ^ tRiSc[19:0]);
	
	if(regBoundX[28])
	begin
		case(regBoundX[26:24])
			3'b000: tRegBoundAdj = tRiSc[15: 4] +
				{ 11'h0, tRegBndAdjAddP[4] };
			3'b001: tRegBoundAdj = tRiSc[17: 6] +
				{ 11'h0, tRegBndAdjAddP[6] };
			3'b010: tRegBoundAdj = tRiSc[19: 8] +
				{ 11'h0, tRegBndAdjAddP[8] };
			3'b011: tRegBoundAdj = tRiSc[21:10] +
				{ 11'h0, tRegBndAdjAddP[10] };
			3'b100: tRegBoundAdj = tRiSc[23:12] +
				{ 11'h0, tRegBndAdjAddP[12] };
			3'b101: tRegBoundAdj = tRiSc[25:14] +
				{ 11'h0, tRegBndAdjAddP[14] };
			3'b110: tRegBoundAdj = tRiSc[27:16] +
				{ 11'h0, tRegBndAdjAddP[16] };
			3'b111: tRegBoundAdj = tRiSc[29:18] +
				{ 11'h0, tRegBndAdjAddP[18] };
		endcase

		if(regBoundX[24])
		begin
			tBoundUp1 = { 14'h0000,  regBoundX[11: 0], 2'b0 };
			tBoundDn1 = { 14'h3FFF, ~regBoundX[23:12], 2'b0 };
		end
		else
		begin
			tBoundUp1 = { 16'h0000,  regBoundX[11: 0] };
			tBoundDn1 = { 16'hFFFF, ~regBoundX[23:12] };
		end

		tRegBoundSum =
			{ 1'b0, regBoundX[11: 0] } +
			{ 1'b0, regBoundX[23:12] };
		tBndOor		= tRegBoundSum[12];


		tIsConst = regBoundX[27];
	
		case(regBoundX[26:25])
			2'b00: begin
				tBoundUp = tBoundUp1;
				tBoundDn = tBoundDn1;
			end
			2'b01: begin
				tBoundUp = { tBoundUp1[23:0], 4'h0 };
				tBoundDn = { tBoundDn1[23:0], 4'h0 };
			end
			2'b10: begin
				tBoundUp = { tBoundUp1[19:0], 8'h0 };
				tBoundDn = { tBoundDn1[19:0], 8'h0 };
			end
			2'b11: begin
				tBoundUp = { tBoundUp1[15:0], 12'h0 };
				tBoundDn = { tBoundDn1[15:0], 12'h0 };
			end
		endcase
	end

//	tBndOob =
//		(tRiSc[47:32]!=0) ||
//		(tRiSc[31:4]>=regBoundX[27:0]);

	tBndOob =
		(	!tRiSc[47] &&
			((tRiSc[47:32]!=0) ||
			(tRiSc[31:4]>=tBoundUp[27:0]))) ||
		(	tRiSc[47] &&
			((tRiSc[47:32]!=16'hFFFF) ||
			(tRiSc[31:4]<=tBoundDn[27:0]))) ||
		(tIsConst && tIsStore);

	if(regBoundX[31:28]==4'h1)
		tBndOob = 1;

//	tBndOob =
//		(tRegValRi[47:28]!=0) ||
//		(tRegValRi[27:0]>=regBoundX[27:0]);

	tRegOutIsOob = (tBndOob || tBndOor) && (regBoundX[33:32]==2'b11);

	if(regBoundX[33:32]==2'b11)
	begin
		tRegOutXLeaTag[15:12]=regBoundX[31:28];
		tRegOutXLeaTag[11: 0]=regBoundX[11: 0]-tRegBoundAdj;
		tRegOutXLeaHi [63:60]=regBoundX[27:24];
		tRegOutXLeaHi [59:48]=regBoundX[23:12]+tRegBoundAdj;
		if(tBndOob || tBndOor)
		begin
			/* If OOB, Invalidate Pointer */
			tRegOutXLeaTag[15:8]=8'h1F;
		end
	end

`else

	tRegOutIsOob = 0;

`endif

	tRiDi = 0;

	if(idUIxt[3])
		tRiDi = regValImm;

`ifdef jx2_agu_ridisp
	tAddrSc0A  =
		{ 2'b0, regValRm[15: 0] } +
		{ 2'b0, tRiSc[15: 0] } +
		{ 2'b0, tRiDi[15: 0] } ;
`else
	tAddrSc0A  = { 1'b0, regValRm[15: 0] } + { 1'b0, tRiSc[15: 0] };
`endif

	tAddrSc0B0 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc[31:16] } + 0;
	tAddrSc0B1 = { 1'b0, regValRm[31:16] } + { 1'b0, tRiSc[31:16] } + 1;
	tAddrSc0C0 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc[47:32] } + 0;
	tAddrSc0C1 = { 1'b0, regValRm[47:32] } + { 1'b0, tRiSc[47:32] } + 1;

`ifdef jx2_agu_ridisp
	tAddrSc0B = (tAddrSc0A[17:16]!=0) ? tAddrSc0B1 : tAddrSc0B0;
`else
	tAddrSc0B = tAddrSc0A[16] ? tAddrSc0B1 : tAddrSc0B0;
`endif

	tCaVal0A = 0;
	tCaVal0B = tAddrSc0A[16];
	tCaVal0C = tCaVal0B ? tAddrSc0B1[16] : tAddrSc0B0[16];

	tAddrSc0C = 16'h0000;
	if(addrEnJq)
	begin
//		tAddrSc0C = tAddrSc0B[16] ? tAddrSc0C1[15:0] : tAddrSc0C0[15:0];
		tAddrSc0C = tCaVal0C ? tAddrSc0C1[15:0] : tAddrSc0C0[15:0];
	end

	tAddrSc0 = { tAddrSc0C[15:0], tAddrSc0B[15:0], tAddrSc0A[15:0] };
	tAddr = tAddrSc0;
end

endmodule

`endif
