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
ALU

Perform some ALU Operations
May Sign or Zero Extend output.

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

`ifdef jx2_enable_clz
`include "ExOpClz.v"
`endif

`ifdef jx2_do_convfp16_alu

`ifdef jx2_enable_convrgb32f
`include "ExConv_Fp8Exp12.v"
`include "ExConv_Fp12Pck8.v"
`endif

`ifdef jx2_enable_convrgb30a
`include "ExConv_Rgb30aExp.v"
// `include "ExConv_Rgb30aPck.v"
`endif

`ifdef jx2_enable_convfp16
`include "ExConv_Fp16Exp32.v"
`include "ExConv_Fp32Pck16.v"
`endif

`ifdef jx2_enable_convfp16al
`include "ExConv_Fp16PckAl.v"
`include "ExConv_Fp16UPckAl.v"
`endif

`endif

`ifdef jx2_enable_rgb5minmax_alu
`include "ExConv_Rgb5MinMax.v"
`endif

`ifdef jx2_enable_conv_vubtof16
`include "ExConv_VecUbToFp16.v"
`include "ExConv_VecFp16ToUb.v"
`endif

`ifdef jx2_enable_conv_psqrta
`include "ExConv_PSqrta.v"
`endif

`ifdef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
`include "ExBtcUtx1.v"
`endif
`endif

`ifdef jx2_enable_packbcd
// `include "ExMiscDaa64.v"
`include "ExBcdAdd64.v"
`endif


module ExALU(
	/* verilator lint_off UNUSED */
	clock,
	reset,
	regValRs,
	regValRt,
	regValXs,
	regValRp,
	idUCmd,
	idUIxt,
	exHold,
	regInSrST,
	regOutVal,
	regOutSrST,
	regInCarryD,
	regOutJcmpT
	);

input			clock;
input			reset;

input[63:0]		regValRs;
input[63:0]		regValRt;
input[63:0]		regValXs;
input[63:0]		regValRp;
input[8:0]		idUCmd;
input[8:0]		idUIxt;
input			exHold;
input[5:0]		regInSrST;

output[63:0]	regOutVal;
output[8:0]		regOutSrST;
input[7:0]		regInCarryD;
output			regOutJcmpT;

wire			regInSrT;
wire			regInSrS;
assign		regInSrT = regInSrST[0];
assign		regInSrS = regInSrST[1];

wire			regInSrP;
wire			regInSrQ;
wire			regInSrR;
wire			regInSrO;
assign		regInSrP = regInSrST[2];
assign		regInSrQ = regInSrST[3];
assign		regInSrR = regInSrST[4];
assign		regInSrO = regInSrST[5];

wire			tOpIsWx;

reg[8:0]		idUIxt2;


reg[63:0]	tRegOutVal2;
reg			tRegOutSrT2;
reg			tRegOutSrS2;
reg			tRegOutDoSrT2;
reg			tRegOutDoSrS2;
reg			tRegOutDoSrP2;

reg			tRegOutSrP2;
reg			tRegOutSrQ2;
reg			tRegOutSrR2;
reg			tRegOutSrO2;

assign	regOutVal = tRegOutVal2;
assign	regOutSrST = {
	tRegOutDoSrP2,
	tRegOutDoSrS2,
	tRegOutDoSrT2,
	tRegOutSrO2, tRegOutSrR2,
	tRegOutSrQ2, tRegOutSrP2,
	tRegOutSrS2, tRegOutSrT2
	};

reg			tRegOutJcmpT;
assign		regOutJcmpT = tRegOutJcmpT;

reg[63:0]	tRegOutVal;
reg			tRegOutSrT;
reg			tRegOutSrS;
reg			tRegOutDoSrT;
reg			tRegOutDoSrS;

reg			tRegOutSrP;
reg			tRegOutSrQ;
reg			tRegOutSrR;
reg			tRegOutSrO;
reg			tRegOutDoSrP;

// `ifdef def_true
// `ifndef def_true
`ifdef jx2_enable_clz
wire[7:0]		tClzVal;
wire[63:0]		tClzRsVal2;
ExOpClz	clz(
	clock,		reset,
	idUCmd,		idUIxt,
	regValRs,	tClzVal,	tClzRsVal2);
`endif


`ifdef jx2_do_convfp16_alu

wire[1:0]		tFp8ExpIsSign;
assign		tFp8ExpIsSign = { 1'b0, idUIxt[0] };

`ifdef jx2_enable_convrgb32f
wire[63:0]	tRegRgb32Upck64F;
ExConv_Fp8Exp12		conv_exp32a(
	regValRs[ 7: 0], tRegRgb32Upck64F[15: 4], tFp8ExpIsSign);
ExConv_Fp8Exp12		conv_exp32b(
	regValRs[15: 8], tRegRgb32Upck64F[31:20], tFp8ExpIsSign);
ExConv_Fp8Exp12		conv_exp32c(
	regValRs[23:16], tRegRgb32Upck64F[47:36], tFp8ExpIsSign);
ExConv_Fp8Exp12		conv_exp32d(
	regValRs[31:24], tRegRgb32Upck64F[63:52], tFp8ExpIsSign);
assign	tRegRgb32Upck64F[ 3: 0]=0;
assign	tRegRgb32Upck64F[19:16]=0;
assign	tRegRgb32Upck64F[35:32]=0;
assign	tRegRgb32Upck64F[51:48]=0;

wire[31:0]	tRegRgb32Pck64F;
ExConv_Fp12Pck8		conv_pck32a(
	regValRs[15: 4], tRegRgb32Pck64F[ 7: 0], tFp8ExpIsSign);
ExConv_Fp12Pck8		conv_pck32b(
	regValRs[31:20], tRegRgb32Pck64F[15: 8], tFp8ExpIsSign);
ExConv_Fp12Pck8		conv_pck32c(
	regValRs[47:36], tRegRgb32Pck64F[23:16], tFp8ExpIsSign);
ExConv_Fp12Pck8		conv_pck32d(
	regValRs[63:52], tRegRgb32Pck64F[31:24], tFp8ExpIsSign);

`endif

`ifdef jx2_enable_convrgb30a
wire[63:0]	tRegRgb30aUpck64F;
ExConv_Rgb30aExp	conv_upck30a(
	regValRs[31:0], regValRs[31:30],
	tRegRgb30aUpck64F);

//wire[31:0]	tRegRgb30aPck64F;
//ExConv_Rgb30aPck	conv_pck30a(
//	regValRs[63:0], tRegRgb30aPck64F);
`endif

`ifdef jx2_enable_convfp16
//wire[63:0]	tRegFp16Upck32L;
//wire[63:0]	tRegFp16Upck32H;
wire[63:0]	tRegFp16Upck32;

wire[63:0]	tRegFp32Pck16;
wire[31:0]	tRegFp32Pck16L;
wire[31:0]	tRegFp32Pck16H;

wire[9:0]	tRegFp16UPckE = idUIxt[1] ?
	( idUIxt[0] ? { 5'h0, regValRs[62:58] } : regValRs[57:48] ) : 10'h000;
//	( (idUIxt[0] || tOpIsWx) ?
//		{ 5'h0, regValRs[62:58] } : regValRs[57:48] ) : 10'h000;

wire[31:0]	tRegFp16UPckT = idUIxt[0] ? regValRs[63:32] : regValRs[31: 0];
// wire[31:0]	tRegFp16UPckT = (idUIxt[0] || tOpIsWx) ?
// 	regValRs[63:32] : regValRs[31: 0];
ExConv_Fp16Exp32	conv_fp16upcka(
	tRegFp16UPckT[15: 0], tRegFp16UPckE[4:0], tRegFp16Upck32[31: 0]);
ExConv_Fp16Exp32	conv_fp16upckb(
	tRegFp16UPckT[31:16], tRegFp16UPckE[9:5], tRegFp16Upck32[63:32]);

ExConv_Fp32Pck16	conv_fp16pcka(regValRs[31: 0], tRegFp32Pck16L[15: 0]);
ExConv_Fp32Pck16	conv_fp16pckb(regValRs[63:32], tRegFp32Pck16L[31:16]);
ExConv_Fp32Pck16	conv_fp16pckc(regValXs[31: 0], tRegFp32Pck16H[15: 0]);
ExConv_Fp32Pck16	conv_fp16pckd(regValXs[63:32], tRegFp32Pck16H[31:16]);

//assign		tRegFp32Pck16 = tOpIsWx ?
//	{ tRegFp32Pck16H, tRegFp32Pck16L } :
//	{ UV32_00, tRegFp32Pck16L };

assign		tRegFp32Pck16 =
	{ tOpIsWx ? tRegFp32Pck16H : UV32_00, tRegFp32Pck16L };

`endif

wire[31:0]	tRegFp16PckAL;
wire[63:0]	tRegFp16UPckAL;

`ifdef jx2_enable_convfp16al
ExConv_Fp16PckAl	conv_fp16pckAl0(regValRs[15: 0], tRegFp16PckAL[ 7: 0]);
ExConv_Fp16PckAl	conv_fp16pckAl1(regValRs[31:16], tRegFp16PckAL[15: 8]);
ExConv_Fp16PckAl	conv_fp16pckAl2(regValRs[47:32], tRegFp16PckAL[23:16]);
ExConv_Fp16PckAl	conv_fp16pckAl3(regValRs[63:48], tRegFp16PckAL[31:24]);

ExConv_Fp16UPckAl	conv_fp16upckAl0(regValRs[ 7: 0], tRegFp16UPckAL[15: 0]);
ExConv_Fp16UPckAl	conv_fp16upckAl1(regValRs[15: 8], tRegFp16UPckAL[31:16]);
ExConv_Fp16UPckAl	conv_fp16upckAl2(regValRs[23:16], tRegFp16UPckAL[47:32]);
ExConv_Fp16UPckAl	conv_fp16upckAl3(regValRs[31:24], tRegFp16UPckAL[63:48]);
`else
assign	tRegFp16PckAL = UV32_00;
assign	tRegFp16UPckAL = UV64_00;
`endif

`endif

`ifdef jx2_enable_rgb5minmax_alu
wire[31:0]	tRegRgb5MinMax;
ExConv_Rgb5MinMax	rgb5minmax(regValRs, tRegRgb5MinMax);
`endif

`ifdef jx2_enable_conv_vubtof16
// wire[31:0]	tRegVub16UPckT =
//	idUIxt[0] ? regValRs[63:32] : regValRs[31: 0];
wire[63:0]	tRegVubUpck;
wire[63:0]	tRegVubPck;

ExConv_VecUbToFp16 vub2f(regValRs, tRegVubUpck, idUIxt[3:0]);
ExConv_VecFp16ToUb vf2ub(regValRs, tRegVubPck, idUIxt[3:0]);
`endif

`ifdef jx2_enable_conv_psqrta
wire[63:0]	tRegPsqrt;
ExConv_PSqrta psqrta(regValRs, tRegPsqrt, idUIxt[3:0]);
`endif


`ifdef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
wire[63:0]	tValUtx1;
ExBtcUtx1	exUtx1(
	regValRs[63:0],
	regValXs[63:0],
	regValRt[3:0],
	idUIxt, tValUtx1,
	1'b0);
`endif
`endif

// `ifdef jx2_enable_packbcd
`ifndef def_true
wire[63:0]		tValDaa;
wire			tValDaaSrT;

ExMiscDaa64 daa1(regValRs, tValDaa, regInSrT, tValDaaSrT, idUIxt[0]);
`endif

parameter		noBcd = 0;
parameter		noAlux = 0;

`ifdef jx2_enable_packbcd
wire[63:0]		tValBcdAdd;
wire			tValBcdAddSrT;

ExBcdAdd64	bcdAdd1(
	regValRs, regValRt, tValBcdAdd,
	regInSrT, tValBcdAddSrT,
//	regInSrS, tValBcdAddSrT,
	idUIxt[0]);
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

reg[1:0]	tAddCa1A0;
reg[1:0]	tAddCa1A1;
reg[1:0]	tAddCa1B0;
reg[1:0]	tAddCa1B1;

reg[1:0]	tSubCa1A0;
reg[1:0]	tSubCa1A1;
reg[1:0]	tSubCa1B0;
reg[1:0]	tSubCa1B1;

reg[3:0]	tAddCa2A0;
reg[3:0]	tAddCa2A1;
reg[3:0]	tSubCa2A0;
reg[3:0]	tSubCa2A1;

reg[4:0]	tAddCa2_Add;
reg[4:0]	tAddCa2_Adc;
reg[4:0]	tSubCa2_Sub;
reg[4:0]	tSubCa2_Sbb;

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
reg			tResult1Tv;
reg			tResultw1T;
reg			tResultb1T;
reg			tResultw1Tv;
reg			tResultb1Tv;

reg[32:0]	tResult1B;
reg			tResult1S;
reg			tResult1Sv;

reg[64:0]	tResult2A;
reg			tResult2T;
reg			tResult2Tv;

reg			tResult1P;
reg			tResult1Q;
reg			tResult1R;
reg			tResult1O;

reg			tResultb1P;
reg			tResultb1Q;
reg			tResultb1R;
reg			tResultb1O;

reg			tAdd1SF;
reg			tAdd2SF;
reg			tAdd1BSF;

reg			tSub1ZF;
reg			tSub1CF;
reg			tSub1SF;
reg			tSub1VF;
reg			tTst1ZF;

reg			tTst1WZF_A;
reg			tTst1WZF_B;
reg			tTst1WZF_C;
reg			tTst1WZF_D;

reg			tSubAZF;
reg			tSubACF;
reg[12:0]	tSubAA0;
reg[12:0]	tSubAA1;

reg			tSubTZF;
reg			tSubTHZF;
reg			tSubTLZF;
reg			tSubTNZF;
reg			tSubNaZF;

reg			tSubABZF;

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

reg			tSub1WZF_A;
reg			tSub1WCF_A;
reg			tSub1WSF_A;
reg			tSub1WZF_B;
reg			tSub1WCF_B;
reg			tSub1WSF_B;
reg			tSub1WZF_C;
reg			tSub1WCF_C;
reg			tSub1WSF_C;
reg			tSub1WZF_D;
reg			tSub1WCF_D;
reg			tSub1WSF_D;

reg			tSub1BZF_A;
reg			tSub1BZF_B;
reg			tSub1BZF_C;
reg			tSub1BZF_D;

reg			tSub1BZF_E;
reg			tSub1BZF_F;
reg			tSub1BZF_G;
reg			tSub1BZF_H;

reg			tSub1SxVF;
reg			tSub2SxVF;
reg			tSub1BSxVF;

reg			tFCmpRsIsNaN;
reg			tFCmpRtIsNaN;
reg			tFCmpRsIsZero;
reg			tFCmpRtIsZero;
reg			tFCmpEqP;
reg			tFCmpxEqP;
reg			tFCmpGtP;
reg			tFCmpxGtP;

reg			tFCmpxRsIsNaN;
reg			tFCmpxRtIsNaN;

reg			tFCmpGtP_FA;
reg			tFCmpGtP_FB;

reg			tFCmpGtP_HA;
reg			tFCmpGtP_HB;
reg			tFCmpGtP_HC;
reg			tFCmpGtP_HD;

reg[32:0]	tResultu1A;
reg[32:0]	tResultu1B;
reg[64:0]	tResultu2A;

reg[64:0]	tResult_Add64;
reg[64:0]	tResult_Sub64;
// reg[64:0]	tResult_Adc64;
// reg[64:0]	tResult_Sbb64;

reg[32:0]	tResult_Add32;
reg[32:0]	tResult_Sub32;
// reg[32:0]	tResult_Adc32;
// reg[32:0]	tResult_Sbb32;

reg[32:0]	tResult1W;
reg[64:0]	tResult2W;
reg[32:0]	tResultShufB;
reg[64:0]	tResultShufW;

reg[32:0]	tResultb1W;
reg[64:0]	tResultb2W;

reg[63:0]	tRegConvVal;
reg			tRegConvSrT;
reg			tRegConvSrS;
reg			tRegConvSrTv;
reg			tRegConvSrSv;

// reg			tOpIsWx;

assign	tOpIsWx =
		(idUIxt[8:6] == JX2_IUC_WX) ||
		(idUIxt[8:6] == JX2_IUC_WT) ||
		(idUIxt[8:6] == JX2_IUC_WF) ||
		(idUIxt[8:6] == JX2_IUC_WXA);


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

`ifdef jx2_enable_cmptag

	tSubAA1 = { 1'b0, regValRs[59:48] } + { 1'b0, ~regValRt[11: 0] } + 1;
	tSubABZF	= regValRt[31:12]==0;

	tSubTHZF	=
		((regValRs[63:60]==regValRt[4:1]) && !regValRt[  0]) ||
		((regValRs[63:61]==regValRt[4:2]) && (regValRt[1:0]==2'b01)) ||
		((regValRs[63:62]==regValRt[4:3]) && (regValRt[2:0]==3'b011)) ||
		((regValRs[63   ]==regValRt[4  ]) && (regValRt[2:0]==3'b111)) ;
	tSubTLZF	=
		((regValRs[ 3: 0]==regValRt[4:1]) && !regValRt[  0]) ||
		((regValRs[ 2: 0]==regValRt[4:2]) && (regValRt[1:0]==2'b01)) ||
		((regValRs[ 1: 0]==regValRt[4:3]) && (regValRt[2:0]==3'b011)) ||
		((regValRs[ 0   ]==regValRt[4  ]) && (regValRt[2:0]==3'b111)) ;
	tSubTZF	= regValRt[5] ? tSubTLZF : tSubTHZF;

	tSubTNZF	=
		((regValRs[51:48]==regValRt[4:1]) && !regValRt[  0]) ||
		((regValRs[51:49]==regValRt[4:2]) && (regValRt[1:0]==2'b01)) ||
		((regValRs[51:50]==regValRt[4:3]) && (regValRt[2:0]==3'b011)) ||
		((regValRs[51   ]==regValRt[4  ]) && (regValRt[2:0]==3'b111)) ;

	tSubNaZF = tSubTNZF && (regValRs[62:52]==11'h7FF);

`else

	tSubAA1		= 0;
	tSubABZF	= 0;
	tSubTZF		= 0;

`endif


//	tOpIsWx = (idUIxt[7:6] == 2'b11);
//	tOpIsWx =
//		(idUIxt[8:6] == JX2_IUC_WX) ||
//		(idUIxt[8:6] == JX2_IUC_WT) ||
//		(idUIxt[8:6] == JX2_IUC_WF) ||
//		(idUIxt[8:6] == JX2_IUC_WXA);

//	if(noAlux)
//		tOpIsWx = 0;

`ifdef def_true
	tAddCa1A0 = { tAdd1A0[16]?tAdd1B1[16]:tAdd1B0[16], tAdd1A0[16] };
	tAddCa1A1 = { tAdd1A1[16]?tAdd1B1[16]:tAdd1B0[16], tAdd1A1[16] };
	tAddCa1B0 = { tAdd1C0[16]?tAdd1D1[16]:tAdd1D0[16], tAdd1C0[16] };
	tAddCa1B1 = { tAdd1C1[16]?tAdd1D1[16]:tAdd1D0[16], tAdd1C1[16] };

	tSubCa1A0 = { tSub1A0[16]?tSub1B1[16]:tSub1B0[16], tSub1A0[16] };
	tSubCa1A1 = { tSub1A1[16]?tSub1B1[16]:tSub1B0[16], tSub1A1[16] };
	tSubCa1B0 = { tSub1C0[16]?tSub1D1[16]:tSub1D0[16], tSub1C0[16] };
	tSubCa1B1 = { tSub1C1[16]?tSub1D1[16]:tSub1D0[16], tSub1C1[16] };

	tAddCa2A0 = { tAddCa1A0[1]?tAddCa1B1:tAddCa1B0, tAddCa1A0 };
	tAddCa2A1 = { tAddCa1A1[1]?tAddCa1B1:tAddCa1B0, tAddCa1A1 };
	tSubCa2A0 = { tSubCa1A0[1]?tSubCa1B1:tSubCa1B0, tSubCa1A0 };
	tSubCa2A1 = { tSubCa1A1[1]?tSubCa1B1:tSubCa1B0, tSubCa1A1 };

	tAdd3A0 = {	tAddCa2A0[3],
				tAddCa2A0[2] ? tAdd1D1[15:0] :	tAdd1D0[15:0],
				tAddCa2A0[1] ? tAdd1C1[15:0] :	tAdd1C0[15:0],
				tAddCa2A0[0] ? tAdd1B1[15:0] :	tAdd1B0[15:0],
												tAdd1A0[15:0] };
	tAdd3A1 = {	tAddCa2A1[3],
				tAddCa2A1[2] ? tAdd1D1[15:0] :	tAdd1D0[15:0],
				tAddCa2A1[1] ? tAdd1C1[15:0] :	tAdd1C0[15:0],
				tAddCa2A1[0] ? tAdd1B1[15:0] :	tAdd1B0[15:0],
												tAdd1A1[15:0] };

	tSub3A0 = {	tSubCa2A0[3],	
				tSubCa2A0[2] ? tSub1D1[15:0] :	tSub1D0[15:0],
				tSubCa2A0[1] ? tSub1C1[15:0] :	tSub1C0[15:0],
				tSubCa2A0[0] ? tSub1B1[15:0] :	tSub1B0[15:0],
												tSub1A0[15:0] };
	tSub3A1 = {	tSubCa2A1[3],
				tSubCa2A1[2] ? tSub1D1[15:0] :	tSub1D0[15:0],
				tSubCa2A1[1] ? tSub1C1[15:0] :	tSub1C0[15:0],
				tSubCa2A1[0] ? tSub1B1[15:0] :	tSub1B0[15:0],
												tSub1A1[15:0] };

	tAdd2A0 = { tAddCa1A0[1], tAdd3A0[31:0] };
	tAdd2A1 = { tAddCa1A1[1], tAdd3A1[31:0] };
	tSub2A0 = { tSubCa1A0[1], tSub3A0[31:0] };
	tSub2A1 = { tSubCa1A1[1], tSub3A1[31:0] };

	tAddCa2_Add = { tAddCa2A0, 1'b0 };
	tSubCa2_Sub = { tSubCa2A1, 1'b1 };
	tAddCa2_Adc = { regInSrT ? tAddCa2A1 : tAddCa2A0, regInSrT };
	tSubCa2_Sbb = { regInSrT ? tSubCa2A0 : tSubCa2A1, !regInSrT };

`ifdef jx2_alu_wx
//	if(tOpIsWx)
	if(tOpIsWx && !noAlux)
	begin
		tAddCa2_Add = {
			regInCarryD[0] ? tAddCa2A1 : tAddCa2A0,
			regInCarryD[0] };
		tSubCa2_Sub = {
			regInCarryD[3] ? tSubCa2A1 : tSubCa2A0,
			regInCarryD[3] };
	end
`endif

	if(idUIxt[1])
	begin
		tAddCa2_Add = tAddCa2_Adc;
		tSubCa2_Sub = tSubCa2_Sbb;
	end

	tResult_Add64	= {	tAddCa2_Add[3],
				tAddCa2_Add[3] ? tAdd1D1[15:0] : tAdd1D0[15:0],
				tAddCa2_Add[2] ? tAdd1C1[15:0] : tAdd1C0[15:0],
				tAddCa2_Add[1] ? tAdd1B1[15:0] : tAdd1B0[15:0],
				tAddCa2_Add[0] ? tAdd1A1[15:0] : tAdd1A0[15:0]};
	tResult_Sub64	= {	tSubCa2_Sub[3],
				tSubCa2_Sub[3] ? tSub1D1[15:0] : tSub1D0[15:0],
				tSubCa2_Sub[2] ? tSub1C1[15:0] : tSub1C0[15:0],
				tSubCa2_Sub[1] ? tSub1B1[15:0] : tSub1B0[15:0],
				tSubCa2_Sub[0] ? tSub1A1[15:0] : tSub1A0[15:0] };

	tResult_Add32 = { tAddCa2_Add[2], tResult_Add64[31:0] };
	tResult_Sub32 = { tSubCa2_Sub[2], tResult_Sub64[31:0] };
`endif

	tSub1BZF_A	= (tSub2A1[ 7: 0]==0);
	tSub1BZF_B	= (tSub2A1[15: 8]==0);
	tSub1BZF_C	= (tSub2A1[23:16]==0);
	tSub1BZF_D	= (tSub2A1[31:24]==0);
	tSub1BZF_E	= (tSub3A1[39:32]==0);
	tSub1BZF_F	= (tSub3A1[47:40]==0);
	tSub1BZF_G	= (tSub3A1[55:48]==0);
	tSub1BZF_H	= (tSub3A1[63:56]==0);

	tSub1WZF_A	= (tSub2A1[15: 0]==0);
	tSub1WZF_B	= (tSub2A1[31:16]==0);
	tSub1WZF_C	= (tSub3A1[47:32]==0);
	tSub1WZF_D	= (tSub3A1[63:48]==0);

	tSub1WCF_A	= tSub1A0[16];
	tSub1WCF_B	= tSub1B0[16];
	tSub1WCF_C	= tSub1C0[16];
	tSub1WCF_D	= tSub1D0[16];

	tSub1WSF_A	= tSub1A0[15];
	tSub1WSF_B	= tSub1B0[15];
	tSub1WSF_C	= tSub1C0[15];
	tSub1WSF_D	= tSub1D0[15];

	tSubACF		= tSubAA1[12] && tSubABZF;
	tSubAZF		= (tSubAA1[11:0]==0) && tSubABZF;

//	tSub1ZF		= (tSub2A1[15:0]==0) && (tSub2A1[31:16]==0);
//	tSub1BZF	= (tSub3A1[47:32]==0) && (tSub3A1[63:48]==0);
	tSub1ZF		= tSub1WZF_A && tSub1WZF_B;
	tSub1BZF	= tSub1WZF_C && tSub1WZF_D;

	tSub2ZF		= tSub1ZF && tSub1BZF;

//	tSub1CF = tSub2A1[32];
//	tSub2CF = tSub3A1[64];
	tSub1CF = tSubCa2A1[1];
	tSub2CF = tSubCa2A1[3];

	tSub1SF = tSub2A1[31];
	tSub2SF = tSub3A1[63];

	tSub1BCF = tSub2B1[32];
	tSub1BSF = tSub2B1[31];
	
	tAdd1SF = tAdd2A1[31];
	tAdd2SF = tAdd3A1[63];
	tAdd1BSF = tAdd2B1[31];

	tTst1WZF_A	= ((regValRs[15: 0]&regValRt[15: 0])==0);
	tTst1WZF_B	= ((regValRs[31:16]&regValRt[31:16])==0);
	tTst1WZF_C	= ((regValRs[47:32]&regValRt[47:32])==0);
	tTst1WZF_D	= ((regValRs[63:48]&regValRt[63:48])==0);

	tTst1ZF		= tTst1WZF_A && tTst1WZF_B;
	tTst1BZF	= tTst1WZF_C && tTst1WZF_D;

//	tTst1ZF =
//		((regValRs[15: 0]&regValRt[15: 0])==0) &&
//		((regValRs[31:16]&regValRt[31:16])==0) ;
//	tTst1BZF =
//		((regValRs[47:32]&regValRt[47:32])==0) &&
//		((regValRs[63:48]&regValRt[63:48])==0) ;

	tTst2ZF =
		tTst1ZF && tTst1BZF;

`ifdef jx2_fcmp_alu
	tFCmpRsIsNaN	= (regValRs[62:52]==11'h7FF) && (regValRs[51:48]!=0);
	tFCmpRtIsNaN	= (regValRt[62:52]==11'h7FF) && (regValRt[51:48]!=0);

	tFCmpRsIsZero	= (regValRs[62:52]==11'h000);
	tFCmpRtIsZero	= (regValRt[62:52]==11'h000);

`ifdef jx2_fpu_longdbl
	tFCmpxRsIsNaN	= (regValRs[62:48]==15'h7FFF) && (regValRs[47:44]!=0);
	tFCmpxRtIsNaN	= (regValRt[62:48]==15'h7FFF) && (regValRt[47:44]!=0);
`else
	tFCmpxRsIsNaN	= tFCmpRsIsNaN;
	tFCmpxRtIsNaN	= tFCmpRtIsNaN;
`endif

//	tFCmpEqP		= tSub2ZF && !tFCmpRsIsNaN;
	tFCmpEqP		= (tSub2ZF || (tFCmpRsIsZero && tFCmpRtIsZero)) && 
		!tFCmpRsIsNaN;

`ifdef jx2_fpu_longdbl
//	tFCmpxEqP		= tSub2ZF && !tFCmpxRsIsNaN;
	tFCmpxEqP		= tSub2ZF && !tFCmpxRsIsNaN && regInCarryD[4];
//	tFCmpGtP;
`else
	tFCmpxEqP		= tFCmpEqP;
`endif

	casez({regValRs[63], regValRt[63], tSub2SF, tSub2ZF})
		4'b0000: tFCmpGtP=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP=0;	/* s==t */
		4'b001z: tFCmpGtP=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP=0;	/* (s<0) && (t>0) */

//		4'b110z: tFCmpGtP=1;	/* (s-t)>0 */
//		4'b1110: tFCmpGtP=0;	/* (s-t)<0 */
		4'b110z: tFCmpGtP=0;	/* (s-t)>0 */
//		4'b1100: tFCmpGtP=0;	/* (s-t)>0 */
//		4'b1101: tFCmpGtP=1;	/* (s-t)>0 */
		4'b1110: tFCmpGtP=1;	/* (s-t)<0 */

		4'b1111: tFCmpGtP=0;	/* s==t */
	endcase
	
`ifdef jx2_fpu_longdbl
	tFCmpxGtP = tFCmpGtP && (tSub2ZF && (regInCarryD[3] && !regInCarryD[4]));
`else
	tFCmpxGtP = tFCmpGtP;
`endif

`ifdef def_true

	casez({regValRs[31], regValRt[31], tSub1SF, tSub1ZF})
		4'b0000: tFCmpGtP_FA=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_FA=0;	/* s==t */
		4'b001z: tFCmpGtP_FA=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_FA=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_FA=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_FA=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_FA=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_FA=0;	/* s==t */
	endcase

	tFCmpGtP_FB		= tFCmpGtP;

	casez({regValRs[15], regValRt[15], tSub1WSF_A, tSub1WZF_A})
		4'b0000: tFCmpGtP_HA=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_HA=0;	/* s==t */
		4'b001z: tFCmpGtP_HA=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_HA=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_HA=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_HA=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_HA=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_HA=0;	/* s==t */
	endcase
	casez({regValRs[31], regValRt[31], tSub1WSF_B, tSub1WZF_B})
		4'b0000: tFCmpGtP_HB=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_HB=0;	/* s==t */
		4'b001z: tFCmpGtP_HB=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_HB=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_HB=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_HB=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_HB=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_HB=0;	/* s==t */
	endcase
	casez({regValRs[47], regValRt[47], tSub1WSF_C, tSub1WZF_C})
		4'b0000: tFCmpGtP_HC=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_HC=0;	/* s==t */
		4'b001z: tFCmpGtP_HC=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_HC=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_HC=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_HC=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_HC=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_HC=0;	/* s==t */
	endcase
	casez({regValRs[63], regValRt[63], tSub1WSF_D, tSub1WZF_D})
		4'b0000: tFCmpGtP_HD=1;	/* (s-t)>0 */
		4'b0001: tFCmpGtP_HD=0;	/* s==t */
		4'b001z: tFCmpGtP_HD=0;	/* (s-t)<0 */
		4'b01zz: tFCmpGtP_HD=1;	/* (s>0) && (t<0) */
		4'b10zz: tFCmpGtP_HD=0;	/* (s<0) && (t>0) */
		4'b110z: tFCmpGtP_HD=0;	/* (s-t)>0 */
		4'b1110: tFCmpGtP_HD=1;	/* (s-t)<0 */
		4'b1111: tFCmpGtP_HD=0;	/* s==t */
	endcase

`endif

`endif

`ifdef def_true
	case({regValRs[31], regValRt[31], tSub1SF})
		3'b000: tSub1VF=0;
		3'b001: tSub1VF=0;
		3'b010: tSub1VF=0;
		3'b011: tSub1VF=1;
		3'b100: tSub1VF=1;
		3'b101: tSub1VF=0;
		3'b110: tSub1VF=0;
		3'b111: tSub1VF=0;
	endcase

	case({regValRs[63], regValRt[63], tSub2SF})
		3'b000: tSub2VF=0;
		3'b001: tSub2VF=0;
		3'b010: tSub2VF=0;
		3'b011: tSub2VF=1;
		3'b100: tSub2VF=1;
		3'b101: tSub2VF=0;
		3'b110: tSub2VF=0;
		3'b111: tSub2VF=0;
	endcase

`ifdef jx2_enable_gsv
	case({regValRs[63], regValRt[63], tSub1BSF})
		3'b000: tSub1BVF=0;
		3'b001: tSub1BVF=0;
		3'b010: tSub1BVF=0;
		3'b011: tSub1BVF=1;
		3'b100: tSub1BVF=1;
		3'b101: tSub1BVF=0;
		3'b110: tSub1BVF=0;
		3'b111: tSub1BVF=0;
	endcase
`else
	tSub1BVF=1'bX;
`endif

	tSub1SxVF = tSub1SF ^ tSub1VF;
	tSub2SxVF = tSub2SF ^ tSub2VF;
	tSub1BSxVF = tSub1BSF ^ tSub1BVF;

`endif

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
		2'b00: tResultShufW[15: 0]=regValRs[15: 0];
		2'b01: tResultShufW[15: 0]=regValRs[31:16];
		2'b10: tResultShufW[15: 0]=regValRs[47:32];
		2'b11: tResultShufW[15: 0]=regValRs[63:48];
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

`ifndef def_true

//	case({regValRs[31], regValRt[31], tSub1SF})
	case({regValRs[31], regValRt[31], tAdd1SF})
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
//	case({regValRs[63], regValRt[63], tSub1BSF})
	case({regValRs[63], regValRt[63], tAdd1BSF})
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

//	case({regValRs[63], regValRt[63], tSub2SF})
	case({regValRs[63], regValRt[63], tAdd2SF})
		3'b000: tSub2VF=0;
		3'b001: tSub2VF=1;
		3'b010: tSub2VF=0;
		3'b011: tSub2VF=0;
		3'b100: tSub2VF=0;
		3'b101: tSub2VF=0;
		3'b110: tSub2VF=1;
		3'b111: tSub2VF=0;
	endcase

`ifdef def_true
//	case({regValRs[31], regValRt[31], tSub1SF})
	case({regValRs[31], regValRt[31], tAdd1SF})
		3'b000: tSub1SxVF=0;
		3'b001: tSub1SxVF=0;
		3'b010: tSub1SxVF=0;
		3'b011: tSub1SxVF=1;
		3'b100: tSub1SxVF=0;
		3'b101: tSub1SxVF=1;
		3'b110: tSub1SxVF=1;
		3'b111: tSub1SxVF=1;
	endcase

`ifdef jx2_enable_gsv
//	case({regValRs[63], regValRt[63], tSub1BSF})
	case({regValRs[63], regValRt[63], tAdd1BSF})
		3'b000: tSub1BSxVF=0;
		3'b001: tSub1BSxVF=0;
		3'b010: tSub1BSxVF=0;
		3'b011: tSub1BSxVF=1;
		3'b100: tSub1BSxVF=0;
		3'b101: tSub1BSxVF=1;
		3'b110: tSub1BSxVF=1;
		3'b111: tSub1BSxVF=1;
	endcase
`else
	tSub1BVF=1'bX;
`endif

	case({regValRs[63], regValRt[63], tSub2SF})
		3'b000: tSub2SxVF=0;
		3'b001: tSub2SxVF=0;
		3'b010: tSub2SxVF=0;
		3'b011: tSub2SxVF=1;
		3'b100: tSub2SxVF=0;
		3'b101: tSub2SxVF=1;
		3'b110: tSub2SxVF=1;
		3'b111: tSub2SxVF=1;
	endcase
`endif

`endif


	tRegOutJcmpT = 0;

`ifdef jx2_alu_jcmp
	case(idUIxt[4:0])
		5'h00: tRegOutJcmpT = tSub1ZF;							/* EQ */
		5'h01: tRegOutJcmpT = !tSub1ZF;							/* NE */
		5'h02: tRegOutJcmpT = tSub1ZF || (tSub1SF^tSub1VF);		/* LE */
		5'h03: tRegOutJcmpT = !tSub1ZF && !(tSub1SF^tSub1VF);	/* GT */
		5'h04: tRegOutJcmpT = (tSub1SF^tSub1VF);				/* LT */
		5'h05: tRegOutJcmpT = !(tSub1SF^tSub1VF);				/* GE */
		5'h06: tRegOutJcmpT = !tSub1CF;							/* B  */
		5'h07: tRegOutJcmpT = tSub1CF;							/* HS */
		5'h08: tRegOutJcmpT = tTst1ZF;							/* TSTT */
		5'h09: tRegOutJcmpT = !tTst1ZF;							/* TSTF */

		5'h10: tRegOutJcmpT = tSub2ZF;							/* EQ */
		5'h11: tRegOutJcmpT = !tSub2ZF;							/* NE */
		5'h12: tRegOutJcmpT = tSub2ZF || (tSub2SF^tSub2VF);		/* LE */
		5'h13: tRegOutJcmpT = !tSub2ZF && !(tSub2SF^tSub2VF);	/* GT */
		5'h14: tRegOutJcmpT = (tSub2SF^tSub2VF);				/* LT */
		5'h15: tRegOutJcmpT = !(tSub2SF^tSub2VF);				/* GE */
		5'h16: tRegOutJcmpT = !tSub2CF;							/* B  */
		5'h17: tRegOutJcmpT = tSub2CF;							/* HS */
		5'h18: tRegOutJcmpT = tTst2ZF;							/* TSTT */
		5'h19: tRegOutJcmpT = !tTst2ZF;							/* TSTF */

		default: tRegOutJcmpT = 0;					/* - */
	endcase
`endif

	tRegOutDoSrS=0;
	tRegOutDoSrT=0;
	tRegOutDoSrP=0;

	tResult1A=UV33_00;
	tResult2A=UV65_00;
	tResult1T=regInSrT;
	tResult2T=regInSrT;
	tResultw1T=regInSrT;
	tResultb1T=regInSrT;
	tRegConvSrT=regInSrT;
	tRegConvSrS=regInSrS;
	tRegConvSrTv=0;
	tRegConvSrSv=0;

	tResult1Tv=0;
	tResult2Tv=0;
	tResultw1Tv=0;
	tResultb1Tv=0;
	tResult1Sv=0;

	tResult1B=UV33_00;
	tResult1S=regInSrS;

	tResult1W=UV33_00;
	tResult2W=UV65_00;

	tResultb1W = UV33_00;
	tResultb2W = UV65_00;

	tResult1P=regInSrP;
	tResult1Q=regInSrQ;
	tResult1R=regInSrR;
	tResult1O=regInSrO;

	tResultb1P=regInSrP;
	tResultb1Q=regInSrQ;
	tResultb1R=regInSrR;
	tResultb1O=regInSrO;
	
//	tOpIsWx = (idUIxt[7:6] == 2'b11) && !noAlux;

	case(idUIxt[3:0])
		4'h0: begin		/* ADD */
//			tResult1A=tAdd2A0;
//			tResult2A=tAdd3A0;
			tResult1A=tResult_Add32;
			tResult2A=tResult_Add64;
			tResult1T=regInSrT;
			tResult2T=regInSrT;
			
`ifdef jx2_alu_wx
//			if(tOpIsWx)
//			begin
//				tResult2A = regInCarryD[0] ? tAdd3A1 : tAdd3A0;
//			end
`endif

			tResult1B=tAdd2B0;
			tResult1S=regInSrS;
			
			tResult2W = { 1'b0,
				tAdd1D0[15:0], tAdd1C0[15:0],
				tAdd1B0[15:0], tAdd1A0[15:0] };
		end
		4'h1: begin		/* SUB */
//			tResult1A=tSub2A1;
//			tResult2A=tSub3A1;
			tResult1A=tResult_Sub32;
			tResult2A=tResult_Sub64;
			tResult1T=regInSrT;
			tResult2T=regInSrT;

`ifdef jx2_alu_wx
//			if(tOpIsWx)
//			begin
//				tResult2A = regInCarryD[3] ? tSub3A1 : tSub3A0;
//			end
`endif

			tResult1B=tSub2B1;
			tResult1S=regInSrS;

			tResult2W = { 1'b0,
                tSub1D1[15:0], tSub1C1[15:0],
				tSub1B1[15:0], tSub1A1[15:0] };
		end
		4'h2: begin		/* ADC */
//			tResult1A=regInSrT ? tAdd2A1 : tAdd2A0;
//			tResult2A=regInSrT ? tAdd3A1 : tAdd3A0;
			tResult1A=tResult_Add32;
			tResult2A=tResult_Add64;
			tResult1T=tResult1A[32];
			tResult2T=tResult2A[64];
			tResult1Tv=1;
			tResult2Tv=1;

`ifdef jx2_alu_wx
//			if(tOpIsWx)
//			begin
//				tResult2A = (regInSrT ? regInCarryD[1] : regInCarryD[0]) ?
//					tAdd3A1 : tAdd3A0;
//			end
`endif

			tResult1B=regInSrS ? tAdd2B1 : tAdd2B0;
			tResult1S=tResult1B[32];

			tResult2W = { 1'b0, regValRs[63:32], regValRt[31:0] };
		end
		4'h3: begin		/* SBB */
//			tResult1A=regInSrT ? tSub2A0 : tSub2A1;
//			tResult2A=regInSrT ? tSub3A0 : tSub3A1;
			tResult1A=tResult_Sub32;
			tResult2A=tResult_Sub64;
			tResult1T=!tResult1A[32];
			tResult2T=!tResult2A[64];
			tResult1Tv=1;
			tResult2Tv=1;

`ifdef jx2_alu_wx
//			if(tOpIsWx)
//			begin
//				tResult2A = (regInSrT ? regInCarryD[2] : regInCarryD[3]) ?
//					tSub3A1 : tSub3A0;
//			end
`endif

			tResult1B=regInSrS ? tSub2B0 : tSub2B1;
			tResult1S=!tResult1B[32];
			tResult1Sv=1;

			tResult2W = { 1'b0, regValRs[31:0], regValRt[63:32] };
		end
		
		4'h4: begin		/* TST */
			tResult1A=UV33_00;
			tResult2A=UV65_00;
			tResult1T=tTst1ZF;
			tResult2T=tTst2ZF;
			tResult1S=tTst1BZF;
			tResult1Tv=1;
			tResult2Tv=1;
			tResult1Sv=1;

			tResult1P=tTst1WZF_A;
			tResult1Q=tTst1WZF_A;
			tResult1R=tTst1WZF_A;
			tResult1O=tTst1WZF_A;
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
//			tResult1A=UV33_00;
//			tResult2A=UV65_00;
			tResult1T=!tSub1ZF;
			tResult2T=!tSub2ZF;
			tResult1S=!tSub1BZF;
			tResult1Tv=1;
			tResult2Tv=1;
			tResult1Sv=1;

			tResult1P=!tSub1WZF_A;
			tResult1Q=!tSub1WZF_B;
			tResult1R=!tSub1WZF_C;
			tResult1O=!tSub1WZF_D;
			
			tResultw1T =
				tResult1P || tResult1Q ||
				tResult1R || tResult1O ;
			tResultw1Tv=1;

			casez( { tResult1O, tResult1R, tResult1Q, tResult1P } )
				4'bzzz1: tResult1W = { 29'h0, 4'h0};
				4'bzz10: tResult1W = { 29'h0, 4'h1};
				4'bz100: tResult1W = { 29'h0, 4'h2};
				4'b1000: tResult1W = { 29'h0, 4'h3};
				4'b0000: tResult1W = { 29'h0, 4'h4};
			endcase

			tResultb1P=!tSub1BZF_A;
			tResultb1Q=!tSub1BZF_B;
			tResultb1R=!tSub1BZF_C;
			tResultb1O=!tSub1BZF_D;

			tResultb1T =
				!tSub1BZF_A || !tSub1BZF_B ||
				!tSub1BZF_C || !tSub1BZF_D ||
				!tSub1BZF_E || !tSub1BZF_F ||
				!tSub1BZF_G || !tSub1BZF_H ;
			tResultb1Tv=1;

			casez( { 
					!tSub1BZF_H, !tSub1BZF_G, !tSub1BZF_F, !tSub1BZF_E,
					!tSub1BZF_D, !tSub1BZF_C, !tSub1BZF_B, !tSub1BZF_A } )
				8'bzzzzzzz1: tResultb1W = { 29'h0, 4'h0};
				8'bzzzzzz10: tResultb1W = { 29'h0, 4'h1};
				8'bzzzzz100: tResultb1W = { 29'h0, 4'h2};
				8'bzzzz1000: tResultb1W = { 29'h0, 4'h3};
				8'bzzz10000: tResultb1W = { 29'h0, 4'h4};
				8'bzz100000: tResultb1W = { 29'h0, 4'h5};
				8'bz1000000: tResultb1W = { 29'h0, 4'h6};
				8'b10000000: tResultb1W = { 29'h0, 4'h7};
				8'b00000000: tResultb1W = { 29'h0, 4'h8};
			endcase

//			tResultb1T =
//				tResultb1P || tResultb1Q ||
//				tResultb1R || tResultb1O ;
//			casez( { tResultb1O, tResultb1R, tResultb1Q, tResultb1P } )
//				4'bzzz1: tResultb1W = { 29'h0, 4'h0};
//				4'bzz10: tResultb1W = { 29'h0, 4'h1};
//				4'bz100: tResultb1W = { 29'h0, 4'h2};
//				4'b1000: tResultb1W = { 29'h0, 4'h3};
//				4'b0000: tResultb1W = { 29'h0, 4'h4};
//			endcase
		end
		4'h9: begin		/* CMPHS */
//			tResult1A=UV33_00;
//			tResult2A=UV65_00;
//			tResult1T=!tSub1CF;
//			tResult2T=!tSub2CF;
//			tResult1S=!tSub1BCF;
			tResult1T=tSub1CF;
			tResult2T=tSub2CF;
			tResult1S=tSub1BCF;
			tResult1Tv=1;
			tResult2Tv=1;
			tResult1Sv=1;

			tResult1P=tSub1WCF_A;
			tResult1Q=tSub1WCF_B;
			tResult1R=tSub1WCF_C;
			tResult1O=tSub1WCF_D;
			
			tResultb1T	= tSubACF;
			tResultb1Tv	= 1;
		end
		4'hA: begin		/* CMPGE */
//			tResult1A=UV33_00;
//			tResult2A=UV65_00;
//			tResult1T=tSub1ZF || (tSub1SF^tSub1VF);
//			tResult2T=tSub2ZF || (tSub2SF^tSub2VF);
//			tResult1S=tSub1BZF || (tSub1BSF^tSub1BVF);
//			tResult1T=tSub1ZF || tSub1SxVF;
//			tResult2T=tSub2ZF || tSub2SxVF;
//			tResult1S=tSub1BZF || tSub1BSxVF;

			tResult1T=!(tSub1SF^tSub1VF);
			tResult2T=!(tSub2SF^tSub2VF);
			tResult1S=!(tSub1BSF^tSub1BVF);
			tResult1Tv=1;
			tResult2Tv=1;
			tResult1Sv=1;

			tResult1P=!tSub1WSF_A || tSub1WZF_A;
			tResult1Q=!tSub1WSF_B || tSub1WZF_B;
			tResult1R=!tSub1WSF_C || tSub1WZF_C;
			tResult1O=!tSub1WSF_D || tSub1WZF_D;

			tResultb1T	= tSubAZF;		//Array Equal
			tResultb1Tv	= 1;

			tResult2W = { 1'b0, regValRs[63:0] };
			casez(regValRt[4:0])
				5'bzzzz0: tResult2W[63:60]=regValRt[4:1];
				5'bzzz01: tResult2W[63:61]=regValRt[4:2];
				5'bzz011: tResult2W[63:62]=regValRt[4:3];
				5'bz0111: tResult2W[63   ]=regValRt[4];
				default: begin end
			endcase

		end

		4'hB: begin		/* SLTxx */
`ifdef jx2_enable_riscv
//			tResult1T=tSub1CF && !tSub1ZF;

			if(idUIxt[4])
				tResult2T=!(tSub2CF && !tSub2ZF);
			else
				tResult2T=(tSub2SF^tSub2VF);

			tResult1Tv=1;
			tResult2Tv=1;

			tResult1A= { UV32_00, tResult2T };
			tResult2A= { UV64_00, tResult2T };
`endif
//			tResult2W = { 1'b0, regValRs[63:48], regValRt[47:0] };
//			tResult2W = { 1'b0, regValRt[63:48], regValRs[47:0] };
			tResult2W = { 1'b0, regValRt[15: 0], regValRs[47:0] };

			tResultb1T	= tSubNaZF;		//Tag Equal
			tResultb1Tv	= 1;
		end

`ifndef def_true
		4'hB: begin		/* NOR */
//			tResult1A={1'b0, ~(regValRs[31: 0] | regValRt[31: 0])};
//			tResult2A={1'b0, ~(regValRs[63:32] | regValRt[63:32]),
//				tResult1A[31:0]};
//			tResult1T=regInSrT;
//			tResult2T=regInSrT;

//			tResult1A=UV33_00;
//			tResult2A=UV65_00;
//			tResult1T=regInSrT;
//			tResult2T=regInSrT;
		end
`endif

		4'hC: begin		/* CMPEQ */
//			tResult1A=UV33_00;
//			tResult2A=UV65_00;
			tResult1T=tSub1ZF;
			tResult2T=tSub2ZF;
			tResult1S=tSub1BZF;

			tResult1Tv=1;
			tResult2Tv=1;
			tResult1Sv=1;

`ifdef jx2_alu_wx
			if(tOpIsWx)
//			if(tOpIsWx && !noAlux)
			begin
				tResult2T = tSub2ZF && regInCarryD[4];
			end
`endif

			tResult1P=tSub1WZF_A;
			tResult1Q=tSub1WZF_B;
			tResult1R=tSub1WZF_C;
			tResult1O=tSub1WZF_D;
			
			casez( { tResult1O, tResult1R, tResult1Q, tResult1P } )
				4'bzzz1: tResult1W = { 29'h0, 4'h0};
				4'bzz10: tResult1W = { 29'h0, 4'h1};
				4'bz100: tResult1W = { 29'h0, 4'h2};
				4'b1000: tResult1W = { 29'h0, 4'h3};
				4'b0000: tResult1W = { 29'h0, 4'h4};
			endcase

`ifndef def_true
			tResultb1P=tSub1BZF_A;
			tResultb1Q=tSub1BZF_B;
			tResultb1R=tSub1BZF_C;
			tResultb1O=tSub1BZF_D;
			tResultb1T =
				tResultb1P || tResultb1Q ||
				tResultb1R || tResultb1O ;
			casez( { tResultb1O, tResultb1R, tResultb1Q, tResultb1P } )
				4'bzzz1: tResultb1W = { 29'h0, 4'h0};
				4'bzz10: tResultb1W = { 29'h0, 4'h1};
				4'bz100: tResultb1W = { 29'h0, 4'h2};
				4'b1000: tResultb1W = { 29'h0, 4'h3};
				4'b0000: tResultb1W = { 29'h0, 4'h4};
			endcase
`endif

`ifdef def_true
			tResultb1P=tSub1BZF_A;
			tResultb1Q=tSub1BZF_B;
			tResultb1R=tSub1BZF_C;
			tResultb1O=tSub1BZF_D;
			tResultb1T =
				tSub1BZF_A || tSub1BZF_B ||
				tSub1BZF_C || tSub1BZF_D ||
				tSub1BZF_E || tSub1BZF_F ||
				tSub1BZF_G || tSub1BZF_H ;
			tResultb1Tv=1;

			casez( { 
					tSub1BZF_H, tSub1BZF_G, tSub1BZF_F, tSub1BZF_E,
					tSub1BZF_D, tSub1BZF_C, tSub1BZF_B, tSub1BZF_A } )
				8'bzzzzzzz1: tResultb1W = { 29'h0, 4'h0};
				8'bzzzzzz10: tResultb1W = { 29'h0, 4'h1};
				8'bzzzzz100: tResultb1W = { 29'h0, 4'h2};
				8'bzzzz1000: tResultb1W = { 29'h0, 4'h3};
				8'bzzz10000: tResultb1W = { 29'h0, 4'h4};
				8'bzz100000: tResultb1W = { 29'h0, 4'h5};
				8'bz1000000: tResultb1W = { 29'h0, 4'h6};
				8'b10000000: tResultb1W = { 29'h0, 4'h7};
				8'b00000000: tResultb1W = { 29'h0, 4'h8};
			endcase
`endif

		end
		4'hD: begin		/* CMPHI */
//			tResult1A=UV33_00;
//			tResult2A=UV65_00;
//			tResult1T=!tSub1CF && !tSub1ZF;
//			tResult2T=!tSub2CF && !tSub2ZF;
//			tResult1S=!tSub1BCF && !tSub1BZF;

			tResult1T=tSub1CF && !tSub1ZF;
			tResult2T=tSub2CF && !tSub2ZF;
			tResult1S=tSub1BCF && !tSub1BZF;

			tResult1Tv=1;
			tResult1Tv=1;
			tResult1Sv=1;

`ifdef jx2_alu_wx
			if(tOpIsWx)
//			if(tOpIsWx && !noAlux)
			begin
				tResult2T = tSub2CF ||
					(tSub2ZF && (regInCarryD[3] && !regInCarryD[4]));
			end
`endif

			tResult1P=tSub1WCF_A && !tSub1WZF_A;
			tResult1Q=tSub1WCF_B && !tSub1WZF_B;
			tResult1R=tSub1WCF_C && !tSub1WZF_C;
			tResult1O=tSub1WCF_D && !tSub1WZF_D;

			tResultb1T	= tSubACF && !tSubAZF;
			tResultb1Tv	= 1;
		end
		4'hE: begin		/* CMPGT */
//			tResult1A=UV33_00;
//			tResult2A=UV65_00;

//			tResult1T=(tSub1SF^tSub1VF);
//			tResult2T=(tSub2SF^tSub2VF);
//			tResult1S=(tSub1BSF^tSub1BVF);

			tResult1T=!tSub1ZF && !(tSub1SF^tSub1VF);
			tResult2T=!tSub2ZF && !(tSub2SF^tSub2VF);
			tResult1S=!tSub1BZF && !(tSub1BSF^tSub1BVF);
			tResult1Tv=1;
			tResult2Tv=1;

`ifdef jx2_alu_wx
			if(tOpIsWx)
//			if(tOpIsWx && !noAlux)
			begin
				tResult2T = (!tSub2ZF && !(tSub2SF^tSub2VF)) ||
					(tSub2ZF && (regInCarryD[3] && !regInCarryD[4]));
			end
`endif

			tResult1P=!tSub1WSF_A && !tSub1WZF_A;
			tResult1Q=!tSub1WSF_B && !tSub1WZF_B;
			tResult1R=!tSub1WSF_C && !tSub1WZF_C;
			tResult1O=!tSub1WSF_D && !tSub1WZF_D;

//			tResult1T=tSub1SxVF;
//			tResult2T=tSub2SxVF;
//			tResult1S=tSub1BSxVF;

			tResultb1T	= tSubTZF;		//Tag Equal
			tResultb1Tv	= 1;
		end
		4'hF: begin		/* CSELT */
			tResult1A={1'b0, regInSrT ? regValRs[31: 0] : regValRt[31: 0] };
			tResult2A={1'b0, regInSrT ? regValRs[63: 0] : regValRt[63: 0] };
			tResult1B={1'b0, regInSrS ? regValRs[31: 0] : regValRt[31: 0] };

			tResult2W = { 1'b0,
				regInSrO ? regValRs[63:48] : regValRt[63:48],
				regInSrR ? regValRs[47:32] : regValRt[47:32],
				regInSrQ ? regValRs[31:16] : regValRt[31:16],
				regInSrP ? regValRs[15: 0] : regValRt[15: 0] };

`ifdef jx2_enable_bitsel
			tResultb2W = { 1'b0,
				(regValRs &   regValRt ) |
				(regValRp & (~regValRt))
				};
`endif
		end
	endcase

`ifdef jx2_enable_aluunary
	tResultu1A=0;
	tResultu1B=0;
	tResultu2A=0;

	casez(idUIxt[3:0])
`ifdef jx2_enable_clz
//		4'b00zz: begin
		4'b000z: begin
			tResultu1A = { UV25_00, tClzVal };
			tResultu2A = { UV57_00, tClzVal };
		end
//		4'b01zz: begin
//		4'b001z: begin
		4'b0010: begin
			tResultu1A = { 1'b0, tClzRsVal2[63:32] };
			tResultu2A = { 1'b0, tClzRsVal2[63: 0] };
		end
`endif

`ifdef jx2_enable_pmort
		4'b0011: begin
			tResultu1B = { 1'b0,
				regValRs[47], regValRs[15], regValRs[46], regValRs[14],
				regValRs[45], regValRs[13], regValRs[44], regValRs[12],
				regValRs[43], regValRs[11], regValRs[42], regValRs[10],
				regValRs[41], regValRs[ 9], regValRs[40], regValRs[ 8],
				regValRs[39], regValRs[ 7], regValRs[38], regValRs[ 6],
				regValRs[37], regValRs[ 5], regValRs[36], regValRs[ 4],
				regValRs[35], regValRs[ 3], regValRs[34], regValRs[ 2],
				regValRs[33], regValRs[ 1], regValRs[32], regValRs[ 0]
			};
			tResultu1A = { 1'b0,
				regValRs[63], regValRs[31], regValRs[62], regValRs[30],
				regValRs[61], regValRs[29], regValRs[60], regValRs[28],
				regValRs[59], regValRs[27], regValRs[58], regValRs[26],
				regValRs[57], regValRs[25], regValRs[56], regValRs[24],
				regValRs[55], regValRs[23], regValRs[54], regValRs[22],
				regValRs[53], regValRs[21], regValRs[52], regValRs[20],
				regValRs[51], regValRs[19], regValRs[50], regValRs[18],
				regValRs[49], regValRs[17], regValRs[48], regValRs[16]
			};
			tResultu2A = { tResultu1A, tResultu1B[31:0] };
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
//	if(idUCmd[5:0]==JX2_UCMD_ALUW3)
	if(	(idUCmd[5:0]==JX2_UCMD_ALUW3) ||
		(idUCmd[5:0]==JX2_UCMD_ALUCMPW))
	begin
//		tResult1A	= tResultu1A;
//		tResult1B	= tResultu1B;
		tResult1A	= tResult1W;
//		tResult1B	= tResult2W[63:32];
		tResult2A	= tResult2W;
		
		tResult1T	= tResultw1T;
		tResult1Tv	= tResultw1Tv;
		
		if(	(idUCmd[5:0]==JX2_UCMD_ALUW3) &&
			(idUIxt[5:0]==JX2_UCIX_ALUW_MOVTA16))
		begin
			$display("JX2_UCIX_ALUW_MOVTA16: %X, Rs=%X, Rt=%X",
				tResult2A, regValRs, regValRt);
		end
	end

//	if(idUCmd[5:0]==JX2_UCMD_ALUB3)
	if(	(idUCmd[5:0]==JX2_UCMD_ALUB3) ||
		(idUCmd[5:0]==JX2_UCMD_ALUCMPB))
	begin
		tResult1A	= tResultb1W;
		tResult2A	= tResultb2W;
		
		tResult1T	= tResultb1T;
		tResult1Tv	= tResultb1Tv;

		tResult1P	= tResultb1P;
		tResult1Q	= tResultb1Q;
		tResult1R	= tResultb1R;
		tResult1O	= tResultb1O;
	end
`endif

`ifdef jx2_fcmp_alu
	if(idUCmd[5:0]==JX2_UCMD_FCMP)
	begin
	
//		$display("Rs=%X Rt=%X SgA=%d SgB=%d SZ=%d ZF=%d   Gt=%d",
//			regValRs, regValRt,
//			regValRs[63], regValRt[63], tSub2SF, tSub2ZF,
//			tFCmpGtP);
	
		if(idUIxt[3:0]==JX2_UCIX_FPU_CMPEQ[3:0])
		begin
			tResult1T	= tFCmpEqP;
			tResult2T	= tFCmpEqP;
			tResult1Tv	= 1;
			tResult2Tv	= 1;

`ifdef jx2_alu_wx
			if(idUIxt[5:4]==2'b10)
			begin
				tResult1T	= tFCmpxEqP;
				tResult2T	= tFCmpxEqP;
			end
`endif
			
			if(idUIxt[5:4]==2'b11)
			begin
				tResult1T	= tSub1ZF;
				tResult1S	= tSub1BZF;
				tResult1P	= tSub1WZF_A;
				tResult1Q	= tSub1WZF_B;
				tResult1R	= tSub1WZF_C;
				tResult1O	= tSub1WZF_D;
			end
		end
		else
		begin
			tResult1T = tFCmpGtP;
			tResult2T = tFCmpGtP;
			tResult1S = tSub2ZF;
			tResult1Tv = 1;
			tResult2Tv = 1;
			tResult1Sv = 1;

`ifdef jx2_use_fpu_fpimm
			if(idUIxt[3:0]==JX2_UCIX_FPU_CMPGE[3:0])
			begin
				tResult1T = tFCmpGtP || tFCmpEqP;
				tResult2T = tFCmpGtP || tFCmpEqP;
			end
`endif

`ifdef jx2_alu_wx
			if(idUIxt[5:4]==2'b10)
			begin
				tResult1T	= tFCmpxGtP;
				tResult2T	= tFCmpxGtP;
			end
`endif

			if(idUIxt[5:4]==2'b11)
			begin
				tResult1T	= tFCmpGtP_FA;
				tResult1S	= tFCmpGtP_FB;
				tResult1P	= tFCmpGtP_HA;
				tResult1Q	= tFCmpGtP_HB;
				tResult1R	= tFCmpGtP_HC;
				tResult1O	= tFCmpGtP_HD;
			end
		end
	end
`endif

`ifdef jx2_enable_conv2_alu
	tRegConvVal = UV64_00;
	case(idUIxt[5:0])

`ifdef jx2_do_convfp16_alu

`ifdef jx2_enable_convrgb32f
		JX2_UCIX_CONV_RGB32PCK64FU: begin
			tRegConvVal = { UV32_00, tRegRgb32Pck64F };
		end
		JX2_UCIX_CONV_RGB32PCK64FS: begin
			tRegConvVal = { UV32_00, tRegRgb32Pck64F };
		end
		JX2_UCIX_CONV_RGB32UPCK64FU: begin
			tRegConvVal = tRegRgb32Upck64F;
		end
		JX2_UCIX_CONV_RGB32UPCK64FS: begin
			tRegConvVal = tRegRgb32Upck64F;
		end
`endif

`ifdef jx2_enable_convrgb30a
		JX2_UCIX_CONV_RGB30APCK64F: begin
//			tRegConvVal = { UV32_00, tRegRgb30aPck64F };
		end
		JX2_UCIX_CONV_RGB30AUPCK64F: begin
			tRegConvVal = tRegRgb30aUpck64F;
		end
`endif

`ifdef jx2_enable_convfp16
		JX2_UCIX_CONV_FP16UPCK32L: begin
`ifndef def_true
			if(idUCmd[5:0]==JX2_UCMD_CONV2_RR)
			begin
				$display("JX2_UCIX_CONV_FP16UPCK32L: %X-%X %X",
					regValXs, regValRs, tRegFp16Upck32);
			end
`endif
			tRegConvVal = tRegFp16Upck32;
		end
		JX2_UCIX_CONV_FP16UPCK32H: begin
			tRegConvVal = tRegFp16Upck32;
		end
		JX2_UCIX_CONV_FP16EUPCK32L: begin
			tRegConvVal = tRegFp16Upck32;
		end
		JX2_UCIX_CONV_FP16EUPCK32H: begin
//			tRegConvVal = tRegFp16Upck32;
			tRegConvVal = { UV32_00, tRegFp16Upck32[31:0] };
		end
		JX2_UCIX_CONV_FP16PCK32: begin
//			tRegConvVal = { UV32_00, tRegFp32Pck16 };
			tRegConvVal = tRegFp32Pck16;
		end
		
		JX2_UCIX_CONV_FP16PCKAL: begin
			tRegConvVal = { UV32_00, tRegFp16PckAL };
		end
		JX2_UCIX_CONV_FP16UPCKAL: begin
			tRegConvVal = tRegFp16UPckAL;
		end
`endif

`endif

`ifdef jx2_enable_rgb5minmax_alu
		JX2_UCIX_CONV_RGB5MINMAX: begin
			tRegConvVal = { UV32_00, tRegRgb5MinMax };
		end
`endif


`ifdef jx2_do_btcutx_alu
`ifdef jx2_enable_btcutx
		JX2_UCIX_CONV2_BLKUTX1, JX2_UCIX_CONV2_BLKUTX2: begin
			tRegConvVal		= tValUtx1;
		end
`ifdef jx2_enable_btcutx3
		JX2_UCIX_CONV2_BLKUTX3H, JX2_UCIX_CONV2_BLKUTX3L: begin
			tRegConvVal		= tValUtx1;
		end
`endif

`endif
`endif

// wire[63:0]		tValDaa;
// wire			tValDaaSrT;

// `ifdef jx2_enable_packbcd
`ifndef def_true
		JX2_UCIX_CONV2_DAAQ, JX2_UCIX_CONV2_DASQ: begin
			tRegConvVal		= tValDaa;
//			tRegConvSrT		= tValDaaSrT;
			tRegConvSrS		= tValDaaSrS;
		end
`endif
		
`ifdef jx2_enable_packbcd
		JX2_UCIX_CONV2_BCDADD, JX2_UCIX_CONV2_BCDSUB:
		begin
			if(!noBcd)
			begin
				tRegConvVal		= tValBcdAdd;
				tRegConvSrT		= tValBcdAddSrT;
				tRegConvSrTv	= 1;
	//			tRegConvSrS		= tValBcdAddSrT;

	//			if(idUCmd[5:0]==JX2_UCMD_CONV2_RR)
	//			begin
	//				$display("BCDADC %X %X %d->%d", regValRs, regValRt,
	//					regInSrT, tRegConvSrT);
	//			end
			end
		end
`endif

		JX2_UCIX_CONV2_ROTCL:
		begin		
//			tRegConvVal		= {	regValRs[62:0], regInSrT };
			tRegConvVal		= {	UV32_00, regValRs[30:0], regInSrT };
			tRegConvSrT		= regValRs[31];
			tRegConvSrTv	= 1;

//			if(idUCmd[5:0]==JX2_UCMD_CONV2_RR)
//			begin
//				$display("ROTCL %X %d", regValRs, tRegConvSrT);
//			end
		end
		JX2_UCIX_CONV2_ROTCR:
		begin
			tRegConvVal		= {	UV32_00, regInSrT, regValRs[31:1] };
			tRegConvSrT		= regValRs[0];
			tRegConvSrTv	= 1;
		end
		JX2_UCIX_CONV2_ROTCLQ:
		begin
			tRegConvVal		= {	regValRs[62:0], regInSrT };
			tRegConvSrT		= regValRs[63];
			tRegConvSrTv	= 1;
		end
		JX2_UCIX_CONV2_ROTCRQ:
		begin
			tRegConvVal		= {	regInSrT, regValRs[63:1] };
			tRegConvSrT		= regValRs[0];
			tRegConvSrTv	= 1;
		end

`ifdef jx2_enable_conv_vubtof16
// wire[63:0]	tRegVubUpck;
// wire[31:0]	tRegVubPck;

		JX2_UCIX_CONV2_VUBTOF16L, JX2_UCIX_CONV2_VSBTOF16L,
		JX2_UCIX_CONV2_VUWTOF32L, JX2_UCIX_CONV2_VSWTOF32L,
		JX2_UCIX_CONV2_VUBTOF16H, JX2_UCIX_CONV2_VSBTOF16H,
		JX2_UCIX_CONV2_VUWTOF32H, JX2_UCIX_CONV2_VSWTOF32H,
		JX2_UCIX_CONV2_VUWTOF16, JX2_UCIX_CONV2_VSWTOF16:
		begin
			tRegConvVal		= tRegVubUpck;
		end

		JX2_UCIX_CONV2_VF16TOUB, JX2_UCIX_CONV2_VF16TOSB,
		JX2_UCIX_CONV2_VF32TOUW, JX2_UCIX_CONV2_VF32TOSW,
		JX2_UCIX_CONV2_VF16TOUW, JX2_UCIX_CONV2_VF16TOSW:
		begin
			tRegConvVal		= tRegVubPck;
		end
`endif

`ifdef jx2_enable_conv_psqrta
		JX2_UCIX_CONV2_PSQRTSAH, JX2_UCIX_CONV2_PSQRTUAH,
		JX2_UCIX_CONV2_PSQRTSAF, JX2_UCIX_CONV2_PSQRTUAF,
		JX2_UCIX_CONV2_PRCPAH, JX2_UCIX_CONV2_PRELUH,
		JX2_UCIX_CONV2_PRCPAF, JX2_UCIX_CONV2_PRELUF:
		begin
			tRegConvVal		= tRegPsqrt;
		end
`endif

		default: begin
			tRegConvVal = UV64_00;
		end

	endcase
`endif

	tRegOutSrP = regInSrP;
	tRegOutSrQ = regInSrQ;
	tRegOutSrR = regInSrR;
	tRegOutSrO = regInSrO;

`ifdef jx2_enable_gsv
	if(idUCmd[5:0]==JX2_UCMD_ALUW3)
	begin
		if(idUIxt[3])
		begin
			tRegOutSrP = tResult1P;
			tRegOutSrQ = tResult1Q;
			tRegOutSrR = tResult1R;
			tRegOutSrO = tResult1O;
		end
	end
`endif

	if(idUIxt[5])
	begin
`ifdef jx2_enable_gsv
//		if(idUIxt[4])
		if(idUIxt[4] && (idUCmd[5:0]==JX2_UCMD_ALU3))
		begin
			tRegOutVal = { tResult1B[31:0], tResult1A[31:0] };
			tRegOutSrT = tResult1T;
			tRegOutSrS = tResult1S;
			tRegOutDoSrT = tResult1Tv;
			tRegOutDoSrS = tResult1Sv;
		end
		else
		begin
			tRegOutVal = tResult2A[63:0];
			tRegOutSrT = tResult2T;
			tRegOutSrS = regInSrS;

			tRegOutDoSrT = tResult1Tv;
			
`ifdef jx2_enable_pred_s
			if(idUIxt[4] &&	(
				(idUCmd[5:0]==JX2_UCMD_ALUCMP)	|| 
				(idUCmd[5:0]==JX2_UCMD_FCMP)	))
			begin
				tRegOutSrS = tResult2T;
				tRegOutSrT = regInSrT;
			end
`endif
		end
`else
		tRegOutVal = tResult2A[63:0];
		tRegOutSrT = tResult2T;
		tRegOutSrS = regInSrS;
		tRegOutDoSrT = tResult2Tv;

`ifdef jx2_enable_pred_s
		if(idUIxt[4] && (
			(idUCmd[5:0]==JX2_UCMD_ALUCMP)	||
			(idUCmd[5:0]==JX2_UCMD_FCMP)	))
		begin
			tRegOutSrS = tResult2T;
			tRegOutSrT = regInSrT;
		end
`endif

`endif
	end
	else
	begin
//		if(idUIxt[4])
//			tRegOutVal = { UV32_00, tResult1A[31:0] };
//		else
//			tRegOutVal = { tResult1A[31]?UV32_FF:UV32_00, tResult1A[31:0] };

		tRegOutVal = {
			(tResult1A[31] && !idUIxt[4]) ? UV32_FF : UV32_00,
			tResult1A[31:0] };

		tRegOutSrT = tResult1T;
		tRegOutSrS = regInSrS;
		tRegOutDoSrT = tResult1Tv;

`ifdef jx2_enable_pred_s
		if(idUIxt[4] && (
			(idUCmd[5:0]==JX2_UCMD_ALUCMP)	||
			(idUCmd[5:0]==JX2_UCMD_FCMP)	))
		begin
			tRegOutSrS = tResult1T;
			tRegOutSrT = regInSrT;
		end
`endif

	end

`ifdef jx2_enable_conv2_alu
	if(idUCmd[5:0]==JX2_UCMD_CONV2_RR)
	begin
		tRegOutVal = tRegConvVal;
		tRegOutSrT = tRegConvSrT;
		tRegOutSrS = tRegConvSrS;
		tRegOutDoSrT = tRegConvSrTv;
		tRegOutDoSrS = tRegConvSrSv;
	end
`endif

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
		tRegOutDoSrT2	<= tRegOutDoSrT;
		tRegOutDoSrS2	<= tRegOutDoSrS;
		tRegOutDoSrP2	<= tRegOutDoSrP;

		tRegOutSrP2		<= tRegOutSrP;
		tRegOutSrQ2		<= tRegOutSrQ;
		tRegOutSrR2		<= tRegOutSrR;
		tRegOutSrO2		<= tRegOutSrO;
	end
end

endmodule
