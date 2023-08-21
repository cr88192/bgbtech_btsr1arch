/*
Edge Walker Module

Purpose is to walk a pair of edges and draw spans into a framebuffer.

This module will need a connection to a Data-Cache along with an MMIO bus interface. The Data-Cache will use OPM/OK signaling.

Framebuffer will be RGB555, with a 16-bit Z buffer.
Textures will be in UTX2 format, with Morton ordering.

Framebuffer will have a stride, with positions given in scanline and pixel.
Framebuffer and texture buffers are to be located in physically-mapped address ranges.

Will walk spans in terms of a left and right pixel offset.
Should support basic blending operations.
 

Control Regs:
	00: Control
		(15: 0): State Flags
			(11: 8): Src Blend Op
			(15:12): Dst Blend Op
			
			(   7): ZMask
			(   6): ZTest
			(   5): ClrMask
			(   4): AlphaTest
			(   3): -
			(   2): -
			(   1): Probe (Always Read as 0)
			(   0): Busy / Initiate
		(31:16): Raster Stride
	01:
		(15: 0): Y start
		(31:16): Y end
	02: Framebuffer
		(31: 0): Base Address (RGB555)
		(63:32): Base Address (Z)
	03: Texture
		(31: 0): Base Address
		(47:32): MBZ
		(52:48): Size (Bits)
		(63:53): MBZ

	04:	Base Left  X,Z
	05: Base Right X,Z
	06: Step Left  X,Z
	07: Step Right X,Z

	08:	Base Left  S,T
	09: Base Right S,T
	0A: Step Left  S,T
	0B: Step Right S,T

	0C: Base Left  R,G,B,A
	0D: Base Right R,G,B,A
	0E: Step Left  R,G,B,A
	0F: Step Right R,G,B,A

X, S, T: Fixed Point 16.16 (extract 10.6)
Z: Fixed Point, 0.32 (extract 0.16)
RGBA: Logical 8.8 or 0.15 (Unit Range)

 */

`include "CoreDefs.v"

`include "ExBtcUtx1.v"

module ModEdgeWalk(
	clock, reset,
	dsDataIn, dsDataOut, dsAddr, dsOpm, dsOK,
	busDataIn, busDataOut, busAddr, busOpm, busOK
	);

input			clock;
input			reset;

input[63:0]		dsDataIn;
output[63:0]	dsDataOut;
output[31:0]	dsAddr;
output[4:0]		dsOpm;
input[1:0]		dsOK;

input[63:0]		busDataIn;
output[63:0]	busDataOut;
input[31:0]		busAddr;
input[4:0]		busOpm;
output[1:0]		busOK;

reg[63:0]		tDsDataIn;
reg[63:0]		tDsDataOut;
reg[31:0]		tDsAddr;
reg[4:0]		tDsOpm;
reg[1:0]		tDsOK;

reg[63:0]		tDsDataOut2;
reg[31:0]		tDsAddr2;
reg[4:0]		tDsOpm2;

assign dsDataOut	= tDsDataOut2;
assign dsAddr		= tDsAddr2;
assign dsOpm		= tDsOpm2;


reg[63:0]		tBusDataIn;
reg[63:0]		tBusDataOut;
reg[31:0]		tBusAddr;
reg[4:0]		tBusOpm;
reg[1:0]		tBusOK;

reg[63:0]		tBusDataOut2;
reg[1:0]		tBusOK2;

assign busDataOut	= tBusDataOut2;
assign busOK		= tBusOK2;

reg				tBusCSel;
reg				tBusCSelOE;
reg				tBusCSelWR;

reg[63:0]		tRegCtrl0L;
reg[63:0]		tRegCtrl3L;

reg[63:0]		tRegCtrl0;
reg[63:0]		tNxtRegCtrl0;
reg[63:0]		tRegCtrl1;
reg[63:0]		tNxtRegCtrl1;
reg[63:0]		tRegCtrl2;
reg[63:0]		tNxtRegCtrl2;
reg[63:0]		tRegCtrl3;
reg[63:0]		tNxtRegCtrl3;
reg[63:0]		tRegCtrl4;
reg[63:0]		tNxtRegCtrl4;
reg[63:0]		tRegCtrl5;
reg[63:0]		tNxtRegCtrl5;
reg[63:0]		tRegCtrl6;
reg[63:0]		tNxtRegCtrl6;
reg[63:0]		tRegCtrl7;
reg[63:0]		tNxtRegCtrl7;
reg[63:0]		tRegCtrl8;
reg[63:0]		tNxtRegCtrl8;
reg[63:0]		tRegCtrl9;
reg[63:0]		tNxtRegCtrl9;
reg[63:0]		tRegCtrl10;
reg[63:0]		tNxtRegCtrl10;
reg[63:0]		tRegCtrl11;
reg[63:0]		tNxtRegCtrl11;
reg[63:0]		tRegCtrl12;
reg[63:0]		tNxtRegCtrl12;
reg[63:0]		tRegCtrl13;
reg[63:0]		tNxtRegCtrl13;
reg[63:0]		tRegCtrl14;
reg[63:0]		tNxtRegCtrl14;
reg[63:0]		tRegCtrl15;
reg[63:0]		tNxtRegCtrl15;
reg[63:0]		tRegCtrl16;
reg[63:0]		tNxtRegCtrl16;


reg[63:0]		tFifoCtrl0[15:0];
reg[63:0]		tFifoCtrl1[15:0];
reg[63:0]		tFifoCtrl2[15:0];
reg[63:0]		tFifoCtrl3[15:0];
reg[63:0]		tFifoCtrl4[15:0];
reg[63:0]		tFifoCtrl5[15:0];
reg[63:0]		tFifoCtrl6[15:0];
reg[63:0]		tFifoCtrl7[15:0];
reg[63:0]		tFifoCtrl8[15:0];
reg[63:0]		tFifoCtrl9[15:0];
reg[63:0]		tFifoCtrl10[15:0];
reg[63:0]		tFifoCtrl11[15:0];
reg[63:0]		tFifoCtrl12[15:0];
reg[63:0]		tFifoCtrl13[15:0];
reg[63:0]		tFifoCtrl14[15:0];
reg[63:0]		tFifoCtrl15[15:0];
reg[63:0]		tFifoCtrl16[15:0];

reg[63:0]		tStFifoCtrl0;
reg[63:0]		tStFifoCtrl1;
reg[63:0]		tStFifoCtrl2;
reg[63:0]		tStFifoCtrl3;
reg[63:0]		tStFifoCtrl4;
reg[63:0]		tStFifoCtrl5;
reg[63:0]		tStFifoCtrl6;
reg[63:0]		tStFifoCtrl7;
reg[63:0]		tStFifoCtrl8;
reg[63:0]		tStFifoCtrl9;
reg[63:0]		tStFifoCtrl10;
reg[63:0]		tStFifoCtrl11;
reg[63:0]		tStFifoCtrl12;
reg[63:0]		tStFifoCtrl13;
reg[63:0]		tStFifoCtrl14;
reg[63:0]		tStFifoCtrl15;
reg[63:0]		tStFifoCtrl16;

reg[63:0]		tNxtFifoCtrl0;
reg[63:0]		tNxtFifoCtrl1;
reg[63:0]		tNxtFifoCtrl2;
reg[63:0]		tNxtFifoCtrl3;
reg[63:0]		tNxtFifoCtrl4;
reg[63:0]		tNxtFifoCtrl5;
reg[63:0]		tNxtFifoCtrl6;
reg[63:0]		tNxtFifoCtrl7;
reg[63:0]		tNxtFifoCtrl8;
reg[63:0]		tNxtFifoCtrl9;
reg[63:0]		tNxtFifoCtrl10;
reg[63:0]		tNxtFifoCtrl11;
reg[63:0]		tNxtFifoCtrl12;
reg[63:0]		tNxtFifoCtrl13;
reg[63:0]		tNxtFifoCtrl14;
reg[63:0]		tNxtFifoCtrl15;
reg[63:0]		tNxtFifoCtrl16;

reg[3:0]		tFifoStRov;
reg[3:0]		tFifoRqRov;
reg[3:0]		tNxtFifoStRov;
reg[3:0]		tNxtFifoRqRov;

wire			tCtrlZTest;
wire			tCtrlZMask;
wire			tCtrlCMask;
wire			tCtrlATest;
wire[7:0]		tCtrlBlendMode;
wire			tCtrlLinear;
wire[2:0]		tCtrlZFcn;

assign		tCtrlZTest		= tRegCtrl0[6];
assign		tCtrlZMask		= tRegCtrl0[7];
assign		tCtrlCMask		= tRegCtrl0[5];
assign		tCtrlATest		= tRegCtrl0[4];
assign		tCtrlBlendMode	= tRegCtrl0[15:8];
assign		tCtrlLinear		= tRegCtrl0[16];
assign		tCtrlZFcn		= tRegCtrl0[19:17];

wire[11:0]		tClipX0;
wire[11:0]		tClipY0;
wire[11:0]		tClipX1;
wire[11:0]		tClipY1;

assign		tClipX0	= tRegCtrl16[11: 0];
assign		tClipY0	= tRegCtrl16[27:16];
assign		tClipX1	= tRegCtrl16[43:32];
assign		tClipY1	= tRegCtrl16[59:48];


reg[63:0]		tMemBlockFbRgb;
reg[15:0]		tMemBlockFbRgbIx;
reg[31:0]		tMemBlockFbRgbAddr;
reg				tMemBlockFbRgbDirty;

reg[63:0]		tMemBlockFbZ;
reg[15:0]		tMemBlockFbZIx;
reg[31:0]		tMemBlockFbZAddr;
reg				tMemBlockFbZDirty;

reg[63:0]		tMemBlockUtxA;
reg[63:0]		tMemBlockUtxB;
reg[63:0]		tMemBlockUtxC;
reg[63:0]		tMemBlockUtxD;

reg[15:0]		tMemBlockUtxIxA;
reg[15:0]		tMemBlockUtxIxB;
reg[15:0]		tMemBlockUtxIxC;
reg[15:0]		tMemBlockUtxIxD;

reg[31:0]		tMemBlockUtxAddr;

reg[15:0]		tReqBlockFbRgbIx;
reg[15:0]		tReqBlockFbZIx;

reg[15:0]		tReqBlockUtxIx;
reg[15:0]		tReqBlockUtxIxA;
reg[15:0]		tReqBlockUtxIxB;
reg[15:0]		tReqBlockUtxIxC;
reg[15:0]		tReqBlockUtxIxD;

reg[31:0]		tReqBlockFbRgbAddr;
reg[31:0]		tReqBlockFbZAddr;
reg[31:0]		tReqBlockUtxAddr;

reg[63:0]		tNxtMemBlockFbRgb;
reg[15:0]		tNxtMemBlockFbRgbIx;
reg[31:0]		tNxtMemBlockFbRgbAddr;

reg[63:0]		tNxtMemBlockFbZ;
reg[15:0]		tNxtMemBlockFbZIx;
reg[31:0]		tNxtMemBlockFbZAddr;

reg[63:0]		tNxtMemBlockUtxA;
reg[63:0]		tNxtMemBlockUtxB;
reg[63:0]		tNxtMemBlockUtxC;
reg[63:0]		tNxtMemBlockUtxD;

reg[15:0]		tNxtMemBlockUtxIxA;
reg[15:0]		tNxtMemBlockUtxIxB;
reg[15:0]		tNxtMemBlockUtxIxC;
reg[15:0]		tNxtMemBlockUtxIxD;

reg[31:0]		tNxtMemBlockUtxAddr;

reg[31:0]		tNxtReqBlockFbRgbMul;

reg[15:0]		tNxtReqBlockFbRgbIx;
reg[15:0]		tNxtReqBlockFbZIx;

reg[15:0]		tNxtReqBlockUtxIx;
reg[15:0]		tNxtReqBlockUtxIxA;
reg[15:0]		tNxtReqBlockUtxIxB;
reg[15:0]		tNxtReqBlockUtxIxC;
reg[15:0]		tNxtReqBlockUtxIxD;

reg				tReqBlockUtxReady;


reg				tNxtMemBlockFbRgbDirty;
reg				tNxtMemBlockFbZDirty;
reg[31:0]		tNxtReqBlockFbRgbAddr;
reg[31:0]		tNxtReqBlockFbZAddr;
reg[31:0]		tNxtReqBlockUtxAddr;

reg				tMissFbRgb;
reg				tMissFbZ;
reg				tMissUtx;
reg				tMissUtxA;
reg				tMissUtxB;
reg				tMissUtxC;
reg				tMiss;

reg				tMissFbRgbLatch;
reg				tMissFbZLatch;
reg				tMissUtxLatch;
reg				tMissFbRgbDone;
reg				tMissFbZDone;
reg				tMissUtxDone;

reg				tNxtMissFbRgbLatch;
reg				tNxtMissFbZLatch;
reg				tNxtMissUtxLatch;
reg				tNxtMissFbRgbDone;
reg				tNxtMissFbZDone;
reg				tNxtMissUtxDone;

reg[11:0]		tScanY;
reg[11:0]		tNxtScanY;
reg[11:0]		tScanLimY;
reg[11:0]		tNxtScanLimY;

reg[15:0]		tScanLfX;
reg[15:0]		tNxtScanLfX;
reg[15:0]		tScanRtX;
reg[15:0]		tNxtScanRtX;
reg[15:0]		tScanStepLfX;
reg[15:0]		tNxtScanStepLfX;
reg[15:0]		tScanStepRtX;
reg[15:0]		tNxtScanStepRtX;

reg[15:0]		tScanLfZ;
reg[15:0]		tNxtScanLfZ;
reg[15:0]		tScanRtZ;
reg[15:0]		tNxtScanRtZ;
reg[15:0]		tScanStepLfZ;
reg[15:0]		tNxtScanStepLfZ;
reg[15:0]		tScanStepRtZ;
reg[15:0]		tNxtScanStepRtZ;

reg[15:0]		tScanLfS;
reg[15:0]		tNxtScanLfS;
reg[15:0]		tScanRtS;
reg[15:0]		tNxtScanRtS;
reg[15:0]		tScanStepLfS;
reg[15:0]		tNxtScanStepLfS;
reg[15:0]		tScanStepRtS;
reg[15:0]		tNxtScanStepRtS;

reg[15:0]		tScanLfT;
reg[15:0]		tNxtScanLfT;
reg[15:0]		tScanRtT;
reg[15:0]		tNxtScanRtT;
reg[15:0]		tScanStepLfT;
reg[15:0]		tNxtScanStepLfT;
reg[15:0]		tScanStepRtT;
reg[15:0]		tNxtScanStepRtT;

reg[11:0]		tScanLfR;
reg[11:0]		tNxtScanLfR;
reg[11:0]		tScanRtR;
reg[11:0]		tNxtScanRtR;
reg[11:0]		tScanStepLfR;
reg[11:0]		tNxtScanStepLfR;
reg[11:0]		tScanStepRtR;
reg[11:0]		tNxtScanStepRtR;

reg[11:0]		tScanLfG;
reg[11:0]		tNxtScanLfG;
reg[11:0]		tScanRtG;
reg[11:0]		tNxtScanRtG;
reg[11:0]		tScanStepLfG;
reg[11:0]		tNxtScanStepLfG;
reg[11:0]		tScanStepRtG;
reg[11:0]		tNxtScanStepRtG;

reg[11:0]		tScanLfB;
reg[11:0]		tNxtScanLfB;
reg[11:0]		tScanRtB;
reg[11:0]		tNxtScanRtB;
reg[11:0]		tScanStepLfB;
reg[11:0]		tNxtScanStepLfB;
reg[11:0]		tScanStepRtB;
reg[11:0]		tNxtScanStepRtB;

reg[11:0]		tScanLfA;
reg[11:0]		tNxtScanLfA;
reg[11:0]		tScanRtA;
reg[11:0]		tNxtScanRtA;
reg[11:0]		tScanStepLfA;
reg[11:0]		tNxtScanStepLfA;
reg[11:0]		tScanStepRtA;
reg[11:0]		tNxtScanStepRtA;

reg[31:0]		tScanDiffX;
reg[31:0]		tScanDiffZ;
reg[31:0]		tScanDiffS;
reg[31:0]		tScanDiffT;
reg[31:0]		tScanDiffR;
reg[31:0]		tScanDiffG;
reg[31:0]		tScanDiffB;
reg[31:0]		tScanDiffA;

reg[31:0]		tScanMulDiffZ;
reg[31:0]		tScanMulDiffS;
reg[31:0]		tScanMulDiffT;
reg[31:0]		tScanMulDiffR;
reg[31:0]		tScanMulDiffG;
reg[31:0]		tScanMulDiffB;
reg[31:0]		tScanMulDiffA;

reg[15:0]		tScanScaleDiffZ;
reg[15:0]		tScanScaleDiffS;
reg[15:0]		tScanScaleDiffT;
reg[11:0]		tScanScaleDiffR;
reg[11:0]		tScanScaleDiffG;
reg[11:0]		tScanScaleDiffB;
reg[11:0]		tScanScaleDiffA;

reg[15:0]		tScanRcp0X;
reg[15:0]		tScanRcp1X;
reg[31:0]		tScanRcpX;
// reg[3:0]		tScanRcpiX;
reg[4:0]		tScanRcpiX;

reg[15:0]		tScanStepPixZ;
reg[15:0]		tNxtScanStepPixZ;
reg[15:0]		tScanStepPixS;
reg[15:0]		tNxtScanStepPixS;
reg[15:0]		tScanStepPixT;
reg[15:0]		tNxtScanStepPixT;

reg[11:0]		tScanStepPixR;
reg[11:0]		tNxtScanStepPixR;
reg[11:0]		tScanStepPixG;
reg[11:0]		tNxtScanStepPixG;
reg[11:0]		tScanStepPixB;
reg[11:0]		tNxtScanStepPixB;
reg[11:0]		tScanStepPixA;
reg[11:0]		tNxtScanStepPixA;

reg[11:0]		tPixX;
reg[11:0]		tNxtPixX;
reg[11:0]		tPixLimX;
reg[11:0]		tNxtPixLimX;

reg[15:0]		tPixZ;
reg[15:0]		tNxtPixZ;
reg[15:0]		tPixStepZ;
reg[15:0]		tNxtPixStepZ;

reg[15:0]		tPixS;
reg[15:0]		tNxtPixS;
reg[15:0]		tPixStepS;
reg[15:0]		tNxtPixStepS;

reg[15:0]		tPixT;
reg[15:0]		tNxtPixT;
reg[15:0]		tPixStepT;
reg[15:0]		tNxtPixStepT;

reg[11:0]		tPixR;
reg[11:0]		tNxtPixR;
reg[11:0]		tPixStepR;
reg[11:0]		tNxtPixStepR;

reg[11:0]		tPixG;
reg[11:0]		tNxtPixG;
reg[11:0]		tPixStepG;
reg[11:0]		tNxtPixStepG;

reg[11:0]		tPixB;
reg[11:0]		tNxtPixB;
reg[11:0]		tPixStepB;
reg[11:0]		tNxtPixStepB;

reg[11:0]		tPixA;
reg[11:0]		tNxtPixA;
reg[11:0]		tPixStepA;
reg[11:0]		tNxtPixStepA;

reg[31:0]		tPixMortST;

reg[15:0]		tPixSp1;
reg[15:0]		tPixTp1;
reg[31:0]		tPixMortSTB;
reg[31:0]		tPixMortSTC;

reg				tDoScanStep;
reg				tDoPixStep;

reg[3:0]		tPixStepInh;
reg[3:0]		tNxtPixStepInh;

wire[63:0]		tPixUtxRgb0a;
wire[63:0]		tPixUtxRgb0b;
wire[63:0]		tPixUtxRgb0c;

reg[31:0]		tPixUtxRgb1s;
reg[31:0]		tPixUtxRgb1t;
reg[31:0]		tPixUtxRgb1;

reg[63:0]		tPixUtxRgb;
reg[63:0]		tPixUtxRgbA;
reg[63:0]		tPixUtxRgbB;
reg[63:0]		tPixUtxRgbC;

wire[63:0]		wMemBlockUtxA =
	(tReqBlockUtxIxA==tMemBlockUtxIxB) ? tMemBlockUtxB :
	(tReqBlockUtxIxA==tMemBlockUtxIxC) ? tMemBlockUtxC :
	(tReqBlockUtxIxA==tMemBlockUtxIxD) ? tMemBlockUtxD :
	tMemBlockUtxA;
wire[63:0]		wMemBlockUtxB =
	(tReqBlockUtxIxB==tMemBlockUtxIxB) ? tMemBlockUtxB :
	(tReqBlockUtxIxB==tMemBlockUtxIxC) ? tMemBlockUtxC :
	(tReqBlockUtxIxB==tMemBlockUtxIxD) ? tMemBlockUtxD :
	tMemBlockUtxA;
wire[63:0]		wMemBlockUtxC =
	(tReqBlockUtxIxC==tMemBlockUtxIxB) ? tMemBlockUtxB :
	(tReqBlockUtxIxC==tMemBlockUtxIxC) ? tMemBlockUtxC :
	(tReqBlockUtxIxC==tMemBlockUtxIxD) ? tMemBlockUtxD :
	tMemBlockUtxA;

ExBtcUtx1	exUtx1a(
	wMemBlockUtxA,
	64'h0,
	tPixMortST[19:16],
	{ 3'b000, JX2_UCIX_CONV2_BLKUTX2 },
	tPixUtxRgb0a,
	1'b1
	);

ExBtcUtx1	exUtx1b(
	wMemBlockUtxB,
	64'h0,
	tPixMortSTB[19:16],
	{ 3'b000, JX2_UCIX_CONV2_BLKUTX2 },
	tPixUtxRgb0b,
	1'b1
	);

ExBtcUtx1	exUtx1c(
	wMemBlockUtxC,
	64'h0,
	tPixMortSTC[19:16],
	{ 3'b000, JX2_UCIX_CONV2_BLKUTX2 },
	tPixUtxRgb0c,
	1'b1
	);

reg[63:0]		tPixModRgb;
reg[63:0]		tPixModRgbL;

reg[33:0]		tPixModRgbA;
reg[33:0]		tPixModRgbR;
reg[33:0]		tPixModRgbG;
reg[33:0]		tPixModRgbB;

reg[15:0]		tPixDstZ;

reg[15:0]		tPixDstRgb5;
reg[63:0]		tPixDestRgb;
reg[63:0]		tPixDestRgbL;
reg[63:0]		tPixBlendRgb;
reg[63:0]		tPixBlendRgbL;


reg[15:0]		tPixBlendRgb_ScSrcS;
reg[15:0]		tPixBlendRgb_ScDstS;

reg[33:0]		tPixBlendRgb_ScSrcA;
reg[33:0]		tPixBlendRgb_ScSrcR;
reg[33:0]		tPixBlendRgb_ScSrcG;
reg[33:0]		tPixBlendRgb_ScSrcB;
reg[33:0]		tPixBlendRgb_ScDstA;
reg[33:0]		tPixBlendRgb_ScDstR;
reg[33:0]		tPixBlendRgb_ScDstG;
reg[33:0]		tPixBlendRgb_ScDstB;
reg[63:0]		tPixBlendRgb_ScSrc;
reg[63:0]		tPixBlendRgb_ScDst;

reg[15:0]		tPixUpdRgb;
reg				tPixUpdRgbAf;
reg				tPixDoUpdRgb;
reg				tPixDoUpdZ;

reg[23:0]		tHeldCyc;
reg[23:0]		tNxtHeldCyc;

reg			tRegZLtP;
reg			tRegZEqP;
reg			tRegZtP;

always @*
begin
	tBusDataOut		= 0;
	tBusOK			= 0;

	tDsDataOut		= tDsDataOut2;
	tDsAddr			= tDsAddr2;
	tDsOpm			= tDsOpm2;
	
	tBusCSel		= tBusAddr[27:12]==16'h000C;
	tBusCSelOE		= tBusCSel && tBusOpm[3];
	tBusCSelWR		= tBusCSel && tBusOpm[4];

	tNxtRegCtrl0			= tRegCtrl0;
	tNxtRegCtrl1			= tRegCtrl1;
	tNxtRegCtrl2			= tRegCtrl2;
	tNxtRegCtrl3			= tRegCtrl3;
	tNxtRegCtrl4			= tRegCtrl4;
	tNxtRegCtrl5			= tRegCtrl5;
	tNxtRegCtrl6			= tRegCtrl6;
	tNxtRegCtrl7			= tRegCtrl7;
	tNxtRegCtrl8			= tRegCtrl8;
	tNxtRegCtrl9			= tRegCtrl9;
	tNxtRegCtrl10			= tRegCtrl10;
	tNxtRegCtrl11			= tRegCtrl11;
	tNxtRegCtrl12			= tRegCtrl12;
	tNxtRegCtrl13			= tRegCtrl13;
	tNxtRegCtrl14			= tRegCtrl14;
	tNxtRegCtrl15			= tRegCtrl15;
	tNxtRegCtrl16			= tRegCtrl16;

	tNxtFifoCtrl0			= tStFifoCtrl0;
	tNxtFifoCtrl1			= tStFifoCtrl1;
	tNxtFifoCtrl2			= tStFifoCtrl2;
	tNxtFifoCtrl3			= tStFifoCtrl3;
	tNxtFifoCtrl4			= tStFifoCtrl4;
	tNxtFifoCtrl5			= tStFifoCtrl5;
	tNxtFifoCtrl6			= tStFifoCtrl6;
	tNxtFifoCtrl7			= tStFifoCtrl7;
	tNxtFifoCtrl8			= tStFifoCtrl8;
	tNxtFifoCtrl9			= tStFifoCtrl9;
	tNxtFifoCtrl10			= tStFifoCtrl10;
	tNxtFifoCtrl11			= tStFifoCtrl11;
	tNxtFifoCtrl12			= tStFifoCtrl12;
	tNxtFifoCtrl13			= tStFifoCtrl13;
	tNxtFifoCtrl14			= tStFifoCtrl14;
	tNxtFifoCtrl15			= tStFifoCtrl15;
	tNxtFifoCtrl16			= tStFifoCtrl16;

	tNxtFifoStRov			= tFifoStRov;
	tNxtFifoRqRov			= tFifoRqRov;


	tNxtMemBlockFbRgb		= tMemBlockFbRgb;
	tNxtMemBlockFbZ			= tMemBlockFbZ;
	tNxtMemBlockUtxA		= tMemBlockUtxA;
	tNxtMemBlockUtxB		= tMemBlockUtxB;
	tNxtMemBlockUtxC		= tMemBlockUtxC;
	tNxtMemBlockUtxD		= tMemBlockUtxD;

	tNxtMemBlockFbRgbIx		= tMemBlockFbRgbIx;
	tNxtMemBlockFbZIx		= tMemBlockFbZIx;

	tNxtMemBlockUtxIxA		= tMemBlockUtxIxA;
	tNxtMemBlockUtxIxB		= tMemBlockUtxIxB;
	tNxtMemBlockUtxIxC		= tMemBlockUtxIxC;
	tNxtMemBlockUtxIxD		= tMemBlockUtxIxD;

	tNxtMemBlockFbRgbAddr	= tMemBlockFbRgbAddr;
	tNxtMemBlockFbZAddr		= tMemBlockFbZAddr;
	tNxtMemBlockUtxAddr		= tMemBlockUtxAddr;

	tNxtMemBlockFbRgbDirty	= tMemBlockFbRgbDirty;
	tNxtMemBlockFbZDirty	= tMemBlockFbZDirty;

	tNxtReqBlockFbRgbIx		= tReqBlockFbRgbIx;
	tNxtReqBlockFbZIx		= tReqBlockFbZIx;

	tNxtReqBlockUtxIx		= tReqBlockUtxIx;
	tNxtReqBlockUtxIxA		= tReqBlockUtxIxA;
	tNxtReqBlockUtxIxB		= tReqBlockUtxIxB;
	tNxtReqBlockUtxIxC		= tReqBlockUtxIxC;
	tNxtReqBlockUtxIxD		= tReqBlockUtxIxD;

	tNxtReqBlockFbRgbAddr	= tReqBlockFbRgbAddr;
	tNxtReqBlockFbZAddr		= tReqBlockFbZAddr;
	tNxtReqBlockUtxAddr		= tReqBlockUtxAddr;

	tNxtMissFbRgbLatch		= tMissFbRgbLatch;
	tNxtMissFbZLatch		= tMissFbZLatch;
	tNxtMissUtxLatch		= tMissUtxLatch;
	tNxtMissFbRgbDone		= tMissFbRgbDone;
	tNxtMissFbZDone			= tMissFbZDone;
	tNxtMissUtxDone			= tMissUtxDone;


	tNxtScanY		= tScanY;
	tNxtScanLimY	= tScanLimY;

	tNxtScanLfX		= tScanLfX;
	tNxtScanRtX		= tScanRtX;
	tNxtScanStepLfX	= tScanStepLfX;
	tNxtScanStepRtX	= tScanStepRtX;

	tNxtScanLfZ		= tScanLfZ;
	tNxtScanRtZ		= tScanRtZ;
	tNxtScanStepLfZ	= tScanStepLfZ;
	tNxtScanStepRtZ	= tScanStepRtZ;

	tNxtScanLfS		= tScanLfS;
	tNxtScanRtS		= tScanRtS;
	tNxtScanStepLfS	= tScanStepLfS;
	tNxtScanStepRtS	= tScanStepRtS;

	tNxtScanLfT		= tScanLfT;
	tNxtScanRtT		= tScanRtT;
	tNxtScanStepLfT	= tScanStepLfT;
	tNxtScanStepRtT	= tScanStepRtT;

	tNxtScanLfR		= tScanLfR;
	tNxtScanRtR		= tScanRtR;
	tNxtScanStepLfR	= tScanStepLfR;
	tNxtScanStepRtR	= tScanStepRtR;

	tNxtScanLfG		= tScanLfG;
	tNxtScanRtG		= tScanRtG;
	tNxtScanStepLfG	= tScanStepLfG;
	tNxtScanStepRtG	= tScanStepRtG;

	tNxtScanLfB		= tScanLfB;
	tNxtScanRtB		= tScanRtB;
	tNxtScanStepLfB	= tScanStepLfB;
	tNxtScanStepRtB	= tScanStepRtB;

	tNxtScanLfA		= tScanLfA;
	tNxtScanRtA		= tScanRtA;
	tNxtScanStepLfA	= tScanStepLfA;
	tNxtScanStepRtA	= tScanStepRtA;

	tNxtScanStepPixZ	= tScanStepPixZ;
	tNxtScanStepPixS	= tScanStepPixS;
	tNxtScanStepPixS	= tScanStepPixT;
	tNxtScanStepPixR	= tScanStepPixR;
	tNxtScanStepPixG	= tScanStepPixG;
	tNxtScanStepPixB	= tScanStepPixB;
	tNxtScanStepPixA	= tScanStepPixA;

	tScanDiffX			=
		{ tScanRtX[15]?16'hFFFF:16'h0000, tScanRtX } -
		{ tScanLfX[15]?16'hFFFF:16'h0000, tScanLfX } ;
//	tScanDiffZ			=
//		{ tScanRtZ[15]?16'hFFFF:16'h0000, tScanRtZ } -
//		{ tScanLfZ[15]?16'hFFFF:16'h0000, tScanLfZ } ;

	tScanDiffZ			=
		{ 16'h0000, tScanRtZ } -
		{ 16'h0000, tScanLfZ } ;

	tScanDiffS			=
		{ tScanRtS[15]?16'hFFFF:16'h0000, tScanRtS } -
		{ tScanLfS[15]?16'hFFFF:16'h0000, tScanLfS } ;
	tScanDiffT			=
		{ tScanRtT[15]?16'hFFFF:16'h0000, tScanRtT } -
		{ tScanLfT[15]?16'hFFFF:16'h0000, tScanLfT } ;

	tScanDiffR			= {20'b0, tScanRtR} - {20'b0, tScanLfR};
	tScanDiffG			= {20'b0, tScanRtG} - {20'b0, tScanLfG};
	tScanDiffB			= {20'b0, tScanRtB} - {20'b0, tScanLfB};
	tScanDiffA			= {20'b0, tScanRtA} - {20'b0, tScanLfA};

`ifndef def_true
	case(tScanDiffX[9:6])
		4'h0: tScanRcp0X			= 16'hFFFF;
		4'h1: tScanRcp0X			= 16'hFFFF;
		4'h2: tScanRcp0X			= 16'h8000;
		4'h3: tScanRcp0X			= 16'h5555;
		4'h4: tScanRcp0X			= 16'h4000;
		4'h5: tScanRcp0X			= 16'h3333;
		4'h6: tScanRcp0X			= 16'h2AAA;
		4'h7: tScanRcp0X			= 16'h2492;
		4'h8: tScanRcp0X			= 16'h2000;
		4'h9: tScanRcp0X			= 16'h1C71;
		4'hA: tScanRcp0X			= 16'h1999;
		4'hB: tScanRcp0X			= 16'h1745;
		4'hC: tScanRcp0X			= 16'h1555;
		4'hD: tScanRcp0X			= 16'h13B1;
		4'hE: tScanRcp0X			= 16'h1249;
		4'hF: tScanRcp0X			= 16'h1111;
	endcase

	casez(tScanDiffX[14:10])
		5'b0000z: tScanRcpiX=tScanDiffX[ 9: 6];
		5'b0001z: tScanRcpiX=tScanDiffX[10: 7];
		5'b001zz: tScanRcpiX=tScanDiffX[11: 8];
		5'b01zzz: tScanRcpiX=tScanDiffX[12: 9];
		5'b1zzzz: tScanRcpiX=tScanDiffX[13:10];
	endcase

	case(tScanRcpiX)
		4'h0: tScanRcp1X			= 16'h1000;
		4'h1: tScanRcp1X			= 16'h0F0F;
		4'h2: tScanRcp1X			= 16'h0E38;
		4'h3: tScanRcp1X			= 16'h0D79;
		4'h4: tScanRcp1X			= 16'h0CCC;
		4'h5: tScanRcp1X			= 16'h0C30;
		4'h6: tScanRcp1X			= 16'h0BA2;
		4'h7: tScanRcp1X			= 16'h0B2A;
		4'h8: tScanRcp1X			= 16'h0AAA;
		4'h9: tScanRcp1X			= 16'h0A3D;
		4'hA: tScanRcp1X			= 16'h09D8;
		4'hB: tScanRcp1X			= 16'h097B;
		4'hC: tScanRcp1X			= 16'h0924;
		4'hD: tScanRcp1X			= 16'h08D3;
		4'hE: tScanRcp1X			= 16'h0888;
		4'hF: tScanRcp1X			= 16'h0842;
	endcase
	
	casez(tScanDiffX[14:10])
		5'b00000: tScanRcpX		= { 16'h0, tScanRcp0X };
		5'b00001: tScanRcpX		= { 16'h0, tScanRcp1X };
		5'b0001z: tScanRcpX		= { 17'b0, tScanRcp1X[15: 1] };
		5'b001zz: tScanRcpX		= { 18'b0, tScanRcp1X[15: 2] };
		5'b01zzz: tScanRcpX		= { 19'b0, tScanRcp1X[15: 3] };
		5'b1zzzz: tScanRcpX		= { 20'b0, tScanRcp1X[15: 4] };
	endcase
`endif


`ifdef def_true
	case(tScanDiffX[10:6])
		5'h00: tScanRcp0X			= 16'hFFFF;
		5'h01: tScanRcp0X			= 16'hFFFF;
		5'h02: tScanRcp0X			= 16'h8000;
		5'h03: tScanRcp0X			= 16'h5555;
		5'h04: tScanRcp0X			= 16'h4000;
		5'h05: tScanRcp0X			= 16'h3333;
		5'h06: tScanRcp0X			= 16'h2AAA;
		5'h07: tScanRcp0X			= 16'h2492;
		5'h08: tScanRcp0X			= 16'h2000;
		5'h09: tScanRcp0X			= 16'h1C71;
		5'h0A: tScanRcp0X			= 16'h1999;
		5'h0B: tScanRcp0X			= 16'h1745;
		5'h0C: tScanRcp0X			= 16'h1555;
		5'h0D: tScanRcp0X			= 16'h13B1;
		5'h0E: tScanRcp0X			= 16'h1249;
		5'h0F: tScanRcp0X			= 16'h1111;
		5'h10: tScanRcp0X			= 16'h1000;
		5'h11: tScanRcp0X			= 16'h0F0F;
		5'h12: tScanRcp0X			= 16'h0E38;
		5'h13: tScanRcp0X			= 16'h0D79;
		5'h14: tScanRcp0X			= 16'h0CCC;
		5'h15: tScanRcp0X			= 16'h0C30;
		5'h16: tScanRcp0X			= 16'h0BA2;
		5'h17: tScanRcp0X			= 16'h0B2A;
		5'h18: tScanRcp0X			= 16'h0AAA;
		5'h19: tScanRcp0X			= 16'h0A3D;
		5'h1A: tScanRcp0X			= 16'h09D8;
		5'h1B: tScanRcp0X			= 16'h097B;
		5'h1C: tScanRcp0X			= 16'h0924;
		5'h1D: tScanRcp0X			= 16'h08D3;
		5'h1E: tScanRcp0X			= 16'h0888;
		5'h1F: tScanRcp0X			= 16'h0842;
	endcase

	casez(tScanDiffX[15:11])
		5'b0000z: tScanRcpiX=tScanDiffX[10: 6];
		5'b0001z: tScanRcpiX=tScanDiffX[11: 7];
		5'b001zz: tScanRcpiX=tScanDiffX[12: 8];
		5'b01zzz: tScanRcpiX=tScanDiffX[13: 9];
		5'b1zzzz: tScanRcpiX=tScanDiffX[14:10];
	endcase

	case(tScanRcpiX)
		5'h00: tScanRcp1X			= 16'h0800;
		5'h01: tScanRcp1X			= 16'h07C1;
		5'h02: tScanRcp1X			= 16'h0787;
		5'h03: tScanRcp1X			= 16'h0750;
		5'h04: tScanRcp1X			= 16'h071C;
		5'h05: tScanRcp1X			= 16'h06EB;
		5'h06: tScanRcp1X			= 16'h06BC;
		5'h07: tScanRcp1X			= 16'h0690;
		5'h08: tScanRcp1X			= 16'h0667;
		5'h09: tScanRcp1X			= 16'h063E;
		5'h0A: tScanRcp1X			= 16'h0618;
		5'h0B: tScanRcp1X			= 16'h05F4;
		5'h0C: tScanRcp1X			= 16'h05D1;
		5'h0D: tScanRcp1X			= 16'h05B0;
		5'h0E: tScanRcp1X			= 16'h0590;
		5'h0F: tScanRcp1X			= 16'h0572;
		5'h10: tScanRcp1X			= 16'h0555;
		5'h11: tScanRcp1X			= 16'h0539;
		5'h12: tScanRcp1X			= 16'h051E;
		5'h13: tScanRcp1X			= 16'h0505;
		5'h14: tScanRcp1X			= 16'h04EC;
		5'h15: tScanRcp1X			= 16'h04D4;
		5'h16: tScanRcp1X			= 16'h04BD;
		5'h17: tScanRcp1X			= 16'h04A7;
		5'h18: tScanRcp1X			= 16'h0492;
		5'h19: tScanRcp1X			= 16'h047D;
		5'h1A: tScanRcp1X			= 16'h0469;
		5'h1B: tScanRcp1X			= 16'h0456;
		5'h1C: tScanRcp1X			= 16'h0444;
		5'h1D: tScanRcp1X			= 16'h0432;
		5'h1E: tScanRcp1X			= 16'h0421;
		5'h1F: tScanRcp1X			= 16'h0410;
	endcase
	
	casez(tScanDiffX[15:11])
		5'b00000: tScanRcpX		= { 16'h0, tScanRcp0X };
		5'b00001: tScanRcpX		= { 16'h0, tScanRcp1X };
		5'b0001z: tScanRcpX		= { 17'b0, tScanRcp1X[15: 1] };
		5'b001zz: tScanRcpX		= { 18'b0, tScanRcp1X[15: 2] };
		5'b01zzz: tScanRcpX		= { 19'b0, tScanRcp1X[15: 3] };
		5'b1zzzz: tScanRcpX		= { 20'b0, tScanRcp1X[15: 4] };
	endcase
`endif

	
//	$display("Rcp %X -> %X", tScanDiffX[15:6], tScanRcpX);
	
	tScanMulDiffZ = tScanDiffZ * tScanRcpX;
	tScanMulDiffS = tScanDiffS * tScanRcpX;
	tScanMulDiffT = tScanDiffT * tScanRcpX;
	tScanMulDiffR = tScanDiffR * tScanRcpX;
	tScanMulDiffG = tScanDiffG * tScanRcpX;
	tScanMulDiffB = tScanDiffB * tScanRcpX;
	tScanMulDiffA = tScanDiffA * tScanRcpX;

//	$display("Lf/Rt G %X,%X D=%X M=%X",
//		tScanLfG, tScanRtG, tScanDiffG, tScanMulDiffG);

	tScanScaleDiffZ	= tScanMulDiffZ[31:16];
	tScanScaleDiffS	= tScanMulDiffS[31:16];
	tScanScaleDiffT	= tScanMulDiffT[31:16];
	tScanScaleDiffR	= tScanMulDiffR[27:16];
	tScanScaleDiffG	= tScanMulDiffG[27:16];
	tScanScaleDiffB	= tScanMulDiffB[27:16];
	tScanScaleDiffA	= tScanMulDiffA[27:16];

	tNxtPixX		= tPixX;
	tNxtPixLimX		= tPixLimX;

	tNxtPixZ		= tPixZ;
	tNxtPixStepZ	= tPixStepZ;
	tNxtPixS		= tPixS;
	tNxtPixStepS	= tPixStepS;
	tNxtPixT		= tPixT;
	tNxtPixStepT	= tPixStepT;

	tNxtPixR		= tPixR;
	tNxtPixStepR	= tPixStepR;
	tNxtPixG		= tPixG;
	tNxtPixStepG	= tPixStepG;
	tNxtPixB		= tPixB;
	tNxtPixStepB	= tPixStepB;
	tNxtPixA		= tPixA;
	tNxtPixStepA	= tPixStepA;

	tNxtPixStepInh	= tPixStepInh;

	tDoScanStep		= 0;
	tDoPixStep		= 0;
	
	if(tPixStepInh!=0)
	begin
		tNxtPixStepInh	= tPixStepInh - 1;
	end
	else
	begin
		tDoPixStep		= 1;
	end

	tMissFbRgb		= tMemBlockFbRgbIx != tReqBlockFbRgbIx;
	tMissFbZ		= tMemBlockFbRgbIx != tReqBlockFbZIx;

	tMissUtxA	=
		(tMemBlockUtxIxA != tReqBlockUtxIxA) &&
		(tMemBlockUtxIxB != tReqBlockUtxIxA) &&
		(tMemBlockUtxIxC != tReqBlockUtxIxA) &&
		(tMemBlockUtxIxD != tReqBlockUtxIxA) ;
	tMissUtxB	=
		(tMemBlockUtxIxA != tReqBlockUtxIxB) &&
		(tMemBlockUtxIxB != tReqBlockUtxIxB) &&
		(tMemBlockUtxIxC != tReqBlockUtxIxB) &&
		(tMemBlockUtxIxD != tReqBlockUtxIxB) ;
	tMissUtxC		=
		(tMemBlockUtxIxA != tReqBlockUtxIxC) &&
		(tMemBlockUtxIxB != tReqBlockUtxIxC) &&
		(tMemBlockUtxIxC != tReqBlockUtxIxC) &&
		(tMemBlockUtxIxD != tReqBlockUtxIxC) ;
	tMissUtx		= tMissUtxA || tMissUtxB || tMissUtxC;

//	tMissUtx		= tMemBlockUtxIx != tReqBlockUtxIx;
	tMiss			= tMissFbRgb || tMissFbZ || tMissUtx;

	if(tMissUtx)
	begin
//		$display("  Miss P A/B/C %d/%d/%d", tMissUtxA, tMissUtxB, tMissUtxC);
//		$display("  Miss IX A/B/C %X/%X/%X",
//			tReqBlockUtxIxA, tReqBlockUtxIxB, tReqBlockUtxIxC);
//		$display("  Miss BLK A/B/C/D %X/%X/%X/%X",
//			tMemBlockUtxIxA, tMemBlockUtxIxB,
//			tMemBlockUtxIxC, tMemBlockUtxIxD);
	end

	case(tPixX[1:0])
		2'b00: tPixDstRgb5 = tMemBlockFbRgb[15: 0];
		2'b01: tPixDstRgb5 = tMemBlockFbRgb[31:16];
		2'b10: tPixDstRgb5 = tMemBlockFbRgb[47:32];
		2'b11: tPixDstRgb5 = tMemBlockFbRgb[63:48];
	endcase

	case(tPixX[1:0])
		2'b00: tPixDstZ = tMemBlockFbZ[15: 0];
		2'b01: tPixDstZ = tMemBlockFbZ[31:16];
		2'b10: tPixDstZ = tMemBlockFbZ[47:32];
		2'b11: tPixDstZ = tMemBlockFbZ[63:48];
	endcase

	tPixDestRgb = {
		tPixDstRgb5[15] ?
			{ tPixDstRgb5[10], tPixDstRgb5[5], tPixDstRgb5[0], 13'h0 } :
			16'hFFFF,
		tPixDstRgb5[14:10], tPixDstRgb5[14:10], 6'h0,
		tPixDstRgb5[ 9: 5], tPixDstRgb5[ 9: 5], 6'h0,
		tPixDstRgb5[ 4: 0], tPixDstRgb5[ 4: 0], 6'h0
	};

	tPixUtxRgb1s =
	{
		{2'b0, tPixS[5]?tPixUtxRgbB[63:58]:tPixUtxRgbA[63:58] } +
		{3'b0, tPixS[4]?tPixUtxRgbB[63:59]:tPixUtxRgbA[63:59] } +
		{4'b0, tPixS[3]?tPixUtxRgbB[63:60]:tPixUtxRgbA[63:60] } +
		{4'b0, tPixS[2]?tPixUtxRgbB[63:60]:tPixUtxRgbA[63:60] } ,

		{2'b0, tPixS[5]?tPixUtxRgbB[47:42]:tPixUtxRgbA[47:42] } +
		{3'b0, tPixS[4]?tPixUtxRgbB[47:43]:tPixUtxRgbA[47:43] } +
		{4'b0, tPixS[3]?tPixUtxRgbB[47:44]:tPixUtxRgbA[47:44] } +
		{4'b0, tPixS[2]?tPixUtxRgbB[47:44]:tPixUtxRgbA[47:44] } ,

		{2'b0, tPixS[5]?tPixUtxRgbB[31:26]:tPixUtxRgbA[31:26] } +
		{3'b0, tPixS[4]?tPixUtxRgbB[31:27]:tPixUtxRgbA[31:27] } +
		{4'b0, tPixS[3]?tPixUtxRgbB[31:28]:tPixUtxRgbA[31:28] } +
		{4'b0, tPixS[2]?tPixUtxRgbB[31:28]:tPixUtxRgbA[31:28] } ,

		{2'b0, tPixS[5]?tPixUtxRgbB[15:10]:tPixUtxRgbA[15:10] } +
		{3'b0, tPixS[4]?tPixUtxRgbB[15:11]:tPixUtxRgbA[15:11] } +
		{4'b0, tPixS[3]?tPixUtxRgbB[15:12]:tPixUtxRgbA[15:12] } +
		{4'b0, tPixS[2]?tPixUtxRgbB[15:12]:tPixUtxRgbA[15:12] }
	};

	tPixUtxRgb1t =
	{
		{2'b0, tPixT[5]?tPixUtxRgbC[63:58]:tPixUtxRgbA[63:58] } +
		{3'b0, tPixT[4]?tPixUtxRgbC[63:59]:tPixUtxRgbA[63:59] } +
		{4'b0, tPixT[3]?tPixUtxRgbC[63:60]:tPixUtxRgbA[63:60] } +
		{4'b0, tPixT[2]?tPixUtxRgbC[63:60]:tPixUtxRgbA[63:60] } ,

		{2'b0, tPixT[5]?tPixUtxRgbC[47:42]:tPixUtxRgbA[47:42] } +
		{3'b0, tPixT[4]?tPixUtxRgbC[47:43]:tPixUtxRgbA[47:43] } +
		{4'b0, tPixT[3]?tPixUtxRgbC[47:44]:tPixUtxRgbA[47:44] } +
		{4'b0, tPixT[2]?tPixUtxRgbC[47:44]:tPixUtxRgbA[47:44] } ,

		{2'b0, tPixT[5]?tPixUtxRgbC[31:26]:tPixUtxRgbA[31:26] } +
		{3'b0, tPixT[4]?tPixUtxRgbC[31:27]:tPixUtxRgbA[31:27] } +
		{4'b0, tPixT[3]?tPixUtxRgbC[31:28]:tPixUtxRgbA[31:28] } +
		{4'b0, tPixT[2]?tPixUtxRgbC[31:28]:tPixUtxRgbA[31:28] } ,

		{2'b0, tPixT[5]?tPixUtxRgbC[15:10]:tPixUtxRgbA[15:10] } +
		{3'b0, tPixT[4]?tPixUtxRgbC[15:11]:tPixUtxRgbA[15:11] } +
		{4'b0, tPixT[3]?tPixUtxRgbC[15:12]:tPixUtxRgbA[15:12] } +
		{4'b0, tPixT[2]?tPixUtxRgbC[15:12]:tPixUtxRgbA[15:12] }
	};
	
	if(tPixS[5]==tPixT[5])
	begin
		tPixUtxRgb1 = {
			tPixUtxRgb1s[31:24] + tPixUtxRgb1t[31:24],
			tPixUtxRgb1s[23:16] + tPixUtxRgb1t[23:16],
			tPixUtxRgb1s[15: 8] + tPixUtxRgb1t[15: 8],
			tPixUtxRgb1s[ 7: 0] + tPixUtxRgb1t[ 7: 0]
		};
	end
	else
	if(tPixS[5])
	begin
		tPixUtxRgb1 = {
			tPixUtxRgb1s[30:24], 1'b0,
			tPixUtxRgb1s[22:16], 1'b0,
			tPixUtxRgb1s[14: 8], 1'b0,
			tPixUtxRgb1s[ 6: 0], 1'b0
		};
	end
	else
	begin
		tPixUtxRgb1 = {
			tPixUtxRgb1t[30:24], 1'b0,
			tPixUtxRgb1t[22:16], 1'b0,
			tPixUtxRgb1t[14: 8], 1'b0,
			tPixUtxRgb1t[ 6: 0], 1'b0
		};
	end


	tPixUtxRgb = {
		tPixUtxRgb1[31:24],	tPixUtxRgb1[31:24],
		tPixUtxRgb1[23:16],	tPixUtxRgb1[23:16],
		tPixUtxRgb1[15: 8],	tPixUtxRgb1[15: 8],
		tPixUtxRgb1[ 7: 0],	tPixUtxRgb1[ 7: 0]
		};

//	if(!tCtrlLinear)
//	begin
//		tPixUtxRgb = tPixUtxRgbA;
//	end

	tPixModRgbA	= { 1'b0, tPixUtxRgb[63:48] } * { 1'b0, tPixA[11:0], 4'b0 };
	tPixModRgbR	= { 1'b0, tPixUtxRgb[47:32] } * { 1'b0, tPixR[11:0], 4'b0 };
	tPixModRgbG	= { 1'b0, tPixUtxRgb[31:16] } * { 1'b0, tPixG[11:0], 4'b0 };
	tPixModRgbB	= { 1'b0, tPixUtxRgb[15: 0] } * { 1'b0, tPixB[11:0], 4'b0 };

//	$display("UTX RGB=%X Blk=%X", tPixUtxRgb, tMemBlockUtx);

	tPixModRgb	= {
		tPixModRgbA[31:16],	tPixModRgbR[31:16],
		tPixModRgbG[31:16],	tPixModRgbB[31:16]
	};

//	$display("MOD RGB %X", tPixModRgb);

	/* Edge */

	tPixBlendRgb = tPixModRgbL;

	case(tCtrlBlendMode[3:0])
		4'h0: tPixBlendRgb_ScSrcS = 16'hFFFF;
		4'h1: tPixBlendRgb_ScSrcS = 16'hFFFF;
		4'h2: tPixBlendRgb_ScSrcS = 16'hFFFF;
		4'h3: tPixBlendRgb_ScSrcS = 16'hFFFF;
		4'h4: tPixBlendRgb_ScSrcS = 16'hFFFF;
		4'h5: tPixBlendRgb_ScSrcS = 16'hFFFF;
		4'h6: tPixBlendRgb_ScSrcS = 16'h0000;
		4'h7: tPixBlendRgb_ScSrcS = 16'hFFFF;
		4'h8: tPixBlendRgb_ScSrcS = tPixModRgbL[63:48];
		4'h9: tPixBlendRgb_ScSrcS = tPixModRgbL[31:16];
		4'hA: tPixBlendRgb_ScSrcS = tPixDestRgbL[63:48];
		4'hB: tPixBlendRgb_ScSrcS = tPixDestRgbL[31:16];
		4'hC: tPixBlendRgb_ScSrcS = ~tPixModRgbL[63:48];
		4'hD: tPixBlendRgb_ScSrcS = ~tPixModRgbL[31:16];
		4'hE: tPixBlendRgb_ScSrcS = ~tPixDestRgbL[63:48];
		4'hF: tPixBlendRgb_ScSrcS = ~tPixDestRgbL[31:16];
	endcase

	case(tCtrlBlendMode[7:4])
		4'h0: tPixBlendRgb_ScDstS = 16'h0000;
		4'h1: tPixBlendRgb_ScDstS = 16'h0000;
		4'h2: tPixBlendRgb_ScDstS = 16'h0000;
		4'h3: tPixBlendRgb_ScDstS = 16'h0000;
		4'h4: tPixBlendRgb_ScDstS = 16'h0000;
		4'h5: tPixBlendRgb_ScDstS = 16'h0000;
		4'h6: tPixBlendRgb_ScDstS = 16'h0000;
		4'h7: tPixBlendRgb_ScDstS = 16'hFFFF;
		4'h8: tPixBlendRgb_ScDstS = tPixModRgbL[63:48];
		4'h9: tPixBlendRgb_ScDstS = tPixModRgbL[31:16];
		4'hA: tPixBlendRgb_ScDstS = tPixDestRgbL[63:48];
		4'hB: tPixBlendRgb_ScDstS = tPixDestRgbL[31:16];
		4'hC: tPixBlendRgb_ScDstS = ~tPixModRgbL[63:48];
		4'hD: tPixBlendRgb_ScDstS = ~tPixModRgbL[31:16];
		4'hE: tPixBlendRgb_ScDstS = ~tPixDestRgbL[63:48];
		4'hF: tPixBlendRgb_ScDstS = ~tPixDestRgbL[31:16];
	endcase

	tPixBlendRgb_ScSrcA =
		{ 1'b0, tPixModRgbL[63:48] } *
		{ 1'b0, tPixBlendRgb_ScSrcS };
	tPixBlendRgb_ScSrcR =
		{ 1'b0, tPixModRgbL[47:32] } *
		{ 1'b0, tPixBlendRgb_ScSrcS };
	tPixBlendRgb_ScSrcG =
		{ 1'b0, tPixModRgbL[31:16] } *
		{ 1'b0, tPixBlendRgb_ScSrcS };
	tPixBlendRgb_ScSrcB =
		{ 1'b0, tPixModRgbL[15: 0] } *
		{ 1'b0, tPixBlendRgb_ScSrcS };

	tPixBlendRgb_ScDstA =
		{ 1'b0, tPixDestRgbL[63:48] } *
		{ 1'b0, tPixBlendRgb_ScDstS };
	tPixBlendRgb_ScDstR =
		{ 1'b0, tPixDestRgbL[47:32] } *
		{ 1'b0, tPixBlendRgb_ScDstS };
	tPixBlendRgb_ScDstG =
		{ 1'b0, tPixDestRgbL[31:16] } *
		{ 1'b0, tPixBlendRgb_ScDstS };
	tPixBlendRgb_ScDstB =
		{ 1'b0, tPixDestRgbL[15: 0] } *
		{ 1'b0, tPixBlendRgb_ScDstS };

	tPixBlendRgb_ScSrc = {
		tPixBlendRgb_ScSrcA [31:16],
		tPixBlendRgb_ScSrcR [31:16],
		tPixBlendRgb_ScSrcG [31:16],
		tPixBlendRgb_ScSrcB [31:16] };
	tPixBlendRgb_ScDst = {
		tPixBlendRgb_ScDstA [31:16],
		tPixBlendRgb_ScDstR [31:16],
		tPixBlendRgb_ScDstG [31:16],
		tPixBlendRgb_ScDstB [31:16] };

	tPixBlendRgb = {
		tPixBlendRgb_ScSrc[63:48] + tPixBlendRgb_ScDst[63:48],
		tPixBlendRgb_ScSrc[47:32] + tPixBlendRgb_ScDst[47:32],
		tPixBlendRgb_ScSrc[31:16] + tPixBlendRgb_ScDst[31:16],
		tPixBlendRgb_ScSrc[15: 0] + tPixBlendRgb_ScDst[15: 0]
	};

	/* Edge */

	tPixUpdRgbAf = (tPixBlendRgbL[63:60]!=15);
	tPixUpdRgb = {
		tPixUpdRgbAf,
		tPixBlendRgbL[47:44],
		tPixUpdRgbAf ? tPixBlendRgbL[63] : tPixBlendRgbL[43],
		tPixBlendRgbL[31:28],
		tPixUpdRgbAf ? tPixBlendRgbL[62] : tPixBlendRgbL[27],
		tPixBlendRgbL[15:12],
		tPixUpdRgbAf ? tPixBlendRgbL[61] : tPixBlendRgbL[11] };

	tPixDoUpdRgb = 1;
	tPixDoUpdZ = 1;

	if(tCtrlATest && !tPixModRgbL[63])
//	if(1'b0)
	begin
		tPixDoUpdRgb = 0;
	end

	tRegZLtP = (tPixZ < tPixDstZ);
	tRegZEqP = (tPixZ == tPixDstZ);
	case(tCtrlZFcn)
		3'h0:		tRegZtP = tRegZLtP;					//LESS
		3'h1:		tRegZtP = !tRegZLtP;				//GEQUAL
		3'h2:		tRegZtP = (tRegZLtP || tRegZEqP);	//LEQUAL
		3'h3:		tRegZtP = !(tRegZLtP || tRegZEqP);	//GREATER
		3'h4:		tRegZtP = tRegZEqP;					//EQUAL
		3'h5:		tRegZtP = !tRegZEqP;				//NOTEQUAL
		3'h6:		tRegZtP = 1;						//ALWAYS
		3'h7:		tRegZtP = 0;						//NEVER
	endcase

//	if(((tPixDstZ < tPixZ) && tCtrlZTest) || !tDoPixStep || tMiss)
	if((!tRegZtP && tCtrlZTest) || !tDoPixStep || tMiss)
	begin
		tPixDoUpdRgb = 0;
		tPixDoUpdZ = 0;
	end
	
	if((tPixX<tClipX0) || (tPixX>tClipX1))
	begin
		tPixDoUpdRgb = 0;
		tPixDoUpdZ = 0;
	end

	if((tScanY<tClipY0) || (tScanY>tClipY1))
	begin
		tPixDoUpdRgb = 0;
		tPixDoUpdZ = 0;
	end
	
	if(!tCtrlZMask)
		tPixDoUpdZ = 0;
	if(!tCtrlCMask)
		tPixDoUpdRgb = 0;
	
	if(tPixDoUpdRgb)
	begin
		if(tPixX[1:0]==0)
			tNxtMemBlockFbRgb[15:0] = tPixUpdRgb;
		if(tPixX[1:0]==1)
			tNxtMemBlockFbRgb[31:16] = tPixUpdRgb;
		if(tPixX[1:0]==2)
			tNxtMemBlockFbRgb[47:32] = tPixUpdRgb;
		if(tPixX[1:0]==3)
			tNxtMemBlockFbRgb[63:48] = tPixUpdRgb;
		tNxtMemBlockFbRgbDirty	= 1;
	end

	if(tPixDoUpdZ)
	begin
		if(tPixX[1:0]==0)
			tNxtMemBlockFbZ[15:0] = tPixZ;
		if(tPixX[1:0]==1)
			tNxtMemBlockFbZ[31:16] = tPixZ;
		if(tPixX[1:0]==2)
			tNxtMemBlockFbZ[47:32] = tPixZ;
		if(tPixX[1:0]==3)
			tNxtMemBlockFbZ[63:48] = tPixZ;
		tNxtMemBlockFbZDirty	= 1;
	end

	
	tNxtReqBlockFbRgbMul =
//		( { 4'h0, tScanY[11:0] } * tRegCtrl0[31:16]) +
		( { 4'h0, tScanY[11:0] } * tRegCtrl1[47:32]) +
		{ 20'h0, tPixX[11:0] };
	
	tNxtReqBlockFbRgbIx	= tNxtReqBlockFbRgbMul[17:2];
	tNxtReqBlockFbZIx = tNxtReqBlockFbRgbIx;
	tNxtReqBlockFbRgbAddr = tRegCtrl2[31:0] +
		{ 13'b0, tNxtReqBlockFbRgbIx, 3'b0 } ;
	tNxtReqBlockFbZAddr = tRegCtrl2[31:0] +
		{ 13'b0, tNxtReqBlockFbZIx, 3'b0 } ;
	
//	tReqBlockFbRgbIx	<= tNxtReqBlockFbRgbIx;
//	tReqBlockFbZIx		<= tNxtReqBlockFbZIx;

	if(tCtrlLinear)
	begin
		tPixSp1 = tPixS + 16'h0040;
		tPixTp1 = tPixT + 16'h0040;
	end
	else
	begin
		tPixSp1 = tPixS;
		tPixTp1 = tPixT;
	end

	tPixMortST = {
		tPixT[15], tPixS[15], tPixT[14], tPixS[14],
		tPixT[13], tPixS[13], tPixT[12], tPixS[12],
		tPixT[11], tPixS[11], tPixT[10], tPixS[10],
		tPixT[ 9], tPixS[ 9], tPixT[ 8], tPixS[ 8],
		tPixT[ 7], tPixS[ 7], tPixT[ 6], tPixS[ 6],
		tPixT[ 5], tPixS[ 5], tPixT[ 4], tPixS[ 4],
		tPixT[ 3], tPixS[ 3], tPixT[ 2], tPixS[ 2],
		tPixT[ 1], tPixS[ 1], tPixT[ 0], tPixS[ 0]
		};

	tPixMortSTB = {
		tPixT[15], tPixSp1[15], tPixT[14], tPixSp1[14],
		tPixT[13], tPixSp1[13], tPixT[12], tPixSp1[12],
		tPixT[11], tPixSp1[11], tPixT[10], tPixSp1[10],
		tPixT[ 9], tPixSp1[ 9], tPixT[ 8], tPixSp1[ 8],
		tPixT[ 7], tPixSp1[ 7], tPixT[ 6], tPixSp1[ 6],
		tPixMortST[11:0]
		};

	tPixMortSTC = {
		tPixTp1[15], tPixS[15], tPixTp1[14], tPixS[14],
		tPixTp1[13], tPixS[13], tPixTp1[12], tPixS[12],
		tPixTp1[11], tPixS[11], tPixTp1[10], tPixS[10],
		tPixTp1[ 9], tPixS[ 9], tPixTp1[ 8], tPixS[ 8],
		tPixTp1[ 7], tPixS[ 7], tPixTp1[ 6], tPixS[ 6],
		tPixMortST[11:0]
		};

	case(tRegCtrl3[56:52])
		5'h00:		tNxtReqBlockUtxIxA = 0;	//1x1
		5'h01:		tNxtReqBlockUtxIxA = 0;	//1x2
		5'h02:		tNxtReqBlockUtxIxA = 0;	//2x2
		5'h03:		tNxtReqBlockUtxIxA = 0;	//2x4
		5'h04:		tNxtReqBlockUtxIxA = 0;	//4x4
		5'h05:		tNxtReqBlockUtxIxA = { 15'h0, tPixMortST[   20] };
		5'h06:		tNxtReqBlockUtxIxA = { 14'h0, tPixMortST[21:20] };
		5'h07:		tNxtReqBlockUtxIxA = { 13'h0, tPixMortST[22:20] };
		5'h08:		tNxtReqBlockUtxIxA = { 12'h0, tPixMortST[23:20] };
		5'h09:		tNxtReqBlockUtxIxA = { 11'h0, tPixMortST[24:20] };
		5'h0A:		tNxtReqBlockUtxIxA = { 10'h0, tPixMortST[25:20] };
		5'h0B:		tNxtReqBlockUtxIxA = {  9'h0, tPixMortST[26:20] };
		5'h0C:		tNxtReqBlockUtxIxA = {  8'h0, tPixMortST[27:20] };
		5'h0D:		tNxtReqBlockUtxIxA = {  7'h0, tPixMortST[28:20] };
		5'h0E:		tNxtReqBlockUtxIxA = {  6'h0, tPixMortST[29:20] };
		5'h0F:		tNxtReqBlockUtxIxA = {  5'h0, tPixMortST[30:20] };
		5'h10:		tNxtReqBlockUtxIxA = {  4'h0, tPixMortST[31:20] };
		default:	tNxtReqBlockUtxIxA = {  4'h0, tPixMortST[31:20] };
	endcase

	case(tRegCtrl3[56:52])
		5'h00:		tNxtReqBlockUtxIxB = 0;	//1x1
		5'h01:		tNxtReqBlockUtxIxB = 0;	//1x2
		5'h02:		tNxtReqBlockUtxIxB = 0;	//2x2
		5'h03:		tNxtReqBlockUtxIxB = 0;	//2x4
		5'h04:		tNxtReqBlockUtxIxB = 0;	//4x4
		5'h05:		tNxtReqBlockUtxIxB = { 15'h0, tPixMortSTB[   20] };
		5'h06:		tNxtReqBlockUtxIxB = { 14'h0, tPixMortSTB[21:20] };
		5'h07:		tNxtReqBlockUtxIxB = { 13'h0, tPixMortSTB[22:20] };
		5'h08:		tNxtReqBlockUtxIxB = { 12'h0, tPixMortSTB[23:20] };
		5'h09:		tNxtReqBlockUtxIxB = { 11'h0, tPixMortSTB[24:20] };
		5'h0A:		tNxtReqBlockUtxIxB = { 10'h0, tPixMortSTB[25:20] };
		5'h0B:		tNxtReqBlockUtxIxB = {  9'h0, tPixMortSTB[26:20] };
		5'h0C:		tNxtReqBlockUtxIxB = {  8'h0, tPixMortSTB[27:20] };
		5'h0D:		tNxtReqBlockUtxIxB = {  7'h0, tPixMortSTB[28:20] };
		5'h0E:		tNxtReqBlockUtxIxB = {  6'h0, tPixMortSTB[29:20] };
		5'h0F:		tNxtReqBlockUtxIxB = {  5'h0, tPixMortSTB[30:20] };
		5'h10:		tNxtReqBlockUtxIxB = {  4'h0, tPixMortSTB[31:20] };
		default:	tNxtReqBlockUtxIxB = {  4'h0, tPixMortSTB[31:20] };
	endcase

	case(tRegCtrl3[56:52])
		5'h00:		tNxtReqBlockUtxIxC = 0;	//1x1
		5'h01:		tNxtReqBlockUtxIxC = 0;	//1x2
		5'h02:		tNxtReqBlockUtxIxC = 0;	//2x2
		5'h03:		tNxtReqBlockUtxIxC = 0;	//2x4
		5'h04:		tNxtReqBlockUtxIxC = 0;	//4x4
		5'h05:		tNxtReqBlockUtxIxC = { 15'h0, tPixMortSTC[   20] };
		5'h06:		tNxtReqBlockUtxIxC = { 14'h0, tPixMortSTC[21:20] };
		5'h07:		tNxtReqBlockUtxIxC = { 13'h0, tPixMortSTC[22:20] };
		5'h08:		tNxtReqBlockUtxIxC = { 12'h0, tPixMortSTC[23:20] };
		5'h09:		tNxtReqBlockUtxIxC = { 11'h0, tPixMortSTC[24:20] };
		5'h0A:		tNxtReqBlockUtxIxC = { 10'h0, tPixMortSTC[25:20] };
		5'h0B:		tNxtReqBlockUtxIxC = {  9'h0, tPixMortSTC[26:20] };
		5'h0C:		tNxtReqBlockUtxIxC = {  8'h0, tPixMortSTC[27:20] };
		5'h0D:		tNxtReqBlockUtxIxC = {  7'h0, tPixMortSTC[28:20] };
		5'h0E:		tNxtReqBlockUtxIxC = {  6'h0, tPixMortSTC[29:20] };
		5'h0F:		tNxtReqBlockUtxIxC = {  5'h0, tPixMortSTC[30:20] };
		5'h10:		tNxtReqBlockUtxIxC = {  4'h0, tPixMortSTC[31:20] };
		default:	tNxtReqBlockUtxIxC = {  4'h0, tPixMortSTC[31:20] };
	endcase

//	if(!tCtrlLinear)
//	begin
//		tNxtReqBlockUtxIxB = tNxtReqBlockUtxIxA;
//		tNxtReqBlockUtxIxC = tNxtReqBlockUtxIxA;
//	end

//	tNxtReqBlockUtxIxB = tNxtReqBlockUtxIx;
//	tNxtReqBlockUtxIxC = tNxtReqBlockUtxIx;

//	tNxtReqBlockUtxIx = { 4'h0, tPixMortST[31:20] };

	if(tMissUtxLatch)
	begin
		tNxtReqBlockUtxIx = tReqBlockUtxIx;
	end
	else
	begin
		tNxtReqBlockUtxIx = tReqBlockUtxIx;
		if(tMissUtxA)
			tNxtReqBlockUtxIx = tReqBlockUtxIxA;
		if(tMissUtxB)
			tNxtReqBlockUtxIx = tReqBlockUtxIxB;
		if(tMissUtxC)
			tNxtReqBlockUtxIx = tReqBlockUtxIxC;
	end

	tReqBlockUtxReady = (tNxtReqBlockUtxIx == tReqBlockUtxIx);


	tNxtReqBlockUtxAddr = tRegCtrl3[31:0] + { 13'b0, tNxtReqBlockUtxIx, 3'b0 };

	if(tMiss)
	begin
		tDoPixStep		= 0;
		tNxtPixStepInh	= 5;
	end

	if(tScanY[11])
	begin
//		$display("ModEdgeWalk: Adv Y, Y is Negative");
		tDoScanStep = 1;
	end
	else
		if((tScanY<tClipY0) || (tScanY>tClipY1))
	begin
//		$display("ModEdgeWalk: Adv Y, Y Outside Clip");
		tDoScanStep = 1;
	end

//	if(tScanY >= tScanLimY)
	if((tScanY > tScanLimY) && !(tScanY[11] && !tScanLimY[11]))
	begin
		if(tRegCtrl0[0])
			$display("ModEdgeWalk: Request Done");
		tNxtRegCtrl0[0] = 0;
		tDoPixStep = 0;
		tDoScanStep = 0;
		
		if((tFifoRqRov != tFifoStRov) && (tRegCtrl0[0]==0))
		begin
			$display("ModEdgeWalk: Advance Request");
			tNxtFifoRqRov = tFifoRqRov + 1;
		end
	end
	
	if(tDoPixStep)
	begin
//		$display("ModEdgeWalk: Scan X Step X=%X Y=%X", tPixX, tScanY);

		tNxtPixX		= tPixX + 1;
//		if(tPixX >= tPixLimX)
		if((tPixX >= tPixLimX) && !(tPixX[11] && !tPixLimX[11]))
		begin
//			$display("ModEdgeWalk: Scan X Step, Adv Y %X %X",
//				tPixX, tPixLimX);
			tDoScanStep = 1;
		end
		tNxtPixStepInh	= 5;

		tNxtPixZ		= tPixZ + tPixStepZ;
		tNxtPixS		= tPixS + tPixStepS;
		tNxtPixT		= tPixT + tPixStepT;
		tNxtPixR		= tPixR + tPixStepR;
		tNxtPixG		= tPixG + tPixStepG;
		tNxtPixB		= tPixB + tPixStepB;
		tNxtPixA		= tPixA + tPixStepA;

//		$display(  "ModEdgeWalk: Pix S=%X T=%X Z=%X",
//			tPixS, tPixT, tPixZ);

//		$display(  "ModEdgeWalk: Pix Step S=%X T=%X Z=%X",
//			tPixStepS, tPixStepT, tPixStepZ);

//		$display(  "ModEdgeWalk: PixRGBA=%X",
//			{tPixA,tPixR,tPixG,tPixB});
	end

	tNxtScanStepPixZ	= tScanScaleDiffZ;
	tNxtScanStepPixS	= tScanScaleDiffS;
	tNxtScanStepPixT	= tScanScaleDiffT;
	tNxtScanStepPixR	= tScanScaleDiffR;
	tNxtScanStepPixG	= tScanScaleDiffG;
	tNxtScanStepPixB	= tScanScaleDiffB;
	tNxtScanStepPixA	= tScanScaleDiffA;

	if(tDoScanStep)
	begin
		$display("ModEdgeWalk: Scan Y Step Y=%X", tScanY);
		if(tPixLimX!=12'h7FF)
		begin
			tNxtScanY		= tScanY + 1;

			tNxtScanLfX		= tScanLfX + tScanStepLfX;
			tNxtScanRtX		= tScanRtX + tScanStepRtX;
			tNxtScanLfZ		= tScanLfZ + tScanStepLfZ;
			tNxtScanRtZ		= tScanRtZ + tScanStepRtZ;

	//		$display(  "ModEdgeWalk: LfX=%X RtX=%X", tNxtScanLfX, tNxtScanRtX);

			tNxtScanLfS		= tScanLfS + tScanStepLfS;
			tNxtScanRtS		= tScanRtS + tScanStepRtS;
			tNxtScanLfT		= tScanLfT + tScanStepLfT;
			tNxtScanRtT		= tScanRtT + tScanStepRtT;

			tNxtScanLfR		= tScanLfR + tScanStepLfR;
			tNxtScanRtR		= tScanRtR + tScanStepRtR;
			tNxtScanLfG		= tScanLfG + tScanStepLfG;
			tNxtScanRtG		= tScanRtG + tScanStepRtG;
			tNxtScanLfB		= tScanLfB + tScanStepLfB;
			tNxtScanRtB		= tScanRtB + tScanStepRtB;
			tNxtScanLfA		= tScanLfA + tScanStepLfA;
			tNxtScanRtA		= tScanRtA + tScanStepRtA;
		end

		tNxtPixX		= { 2'b00, tScanLfX[15:6] };
		tNxtPixLimX		= { 2'b00, tScanRtX[15:6] };

		tNxtPixZ		= tScanLfZ;
		tNxtPixS		= tScanLfS;
		tNxtPixT		= tScanLfT;
		tNxtPixR		= tScanLfR;
		tNxtPixG		= tScanLfG;
		tNxtPixB		= tScanLfB;
		tNxtPixA		= tScanLfA;
		
		tNxtPixStepZ	= tScanStepPixZ;
		tNxtPixStepS	= tScanStepPixS;
		tNxtPixStepT	= tScanStepPixT;
		tNxtPixStepR	= tScanStepPixR;
		tNxtPixStepG	= tScanStepPixG;
		tNxtPixStepB	= tScanStepPixB;
		tNxtPixStepA	= tScanStepPixA;

//		$display(  "ModEdgeWalk: LfST=%X RtST=%X",
//			{tScanLfS,tScanLfT},
//			{tScanRtS,tScanRtT});

//		$display(  "ModEdgeWalk: Step S=%X T=%X Z=%X",
//			tScanStepPixS,tScanStepPixT, tScanStepPixZ);

//		$display(  "ModEdgeWalk: LfRGBA=%X RtRGBA=%X",
//			{tScanLfA,tScanLfR,tScanLfG,tScanLfB},
//			{tScanRtA,tScanRtR,tScanRtG,tScanRtB});
	end

	tNxtHeldCyc = tHeldCyc-1;

	if(tMissFbRgb || tMissFbRgbLatch)
	begin
		if(tDsOK==UMEM_OK_OK)
		begin
			if(tDsOpm2 == UMEM_OPM_WR_Q)
			begin
				tNxtMemBlockFbRgbDirty = 0;
			end
			if(tDsOpm2 == UMEM_OPM_RD_Q)
			begin
//				$display("ModEdgeWalk: Fetch RGB Ix=%X A=%X", 
//					tReqBlockFbRgbIx, tReqBlockFbRgbAddr);
				tNxtMemBlockFbRgb		= tDsDataIn;
				tNxtMemBlockFbRgbIx		= tReqBlockFbRgbIx;
				tNxtMemBlockFbRgbAddr	= tReqBlockFbRgbAddr;
				tNxtMemBlockFbRgbDirty	= 0;
//				tNxtMissFbRgbLatch		= 0;
				tNxtMissFbRgbDone		= 1;
			end

			tDsOpm = UMEM_OPM_READY;
		end
		else
			if(tDsOK==UMEM_OK_READY)
		begin
			tDsDataOut = tMemBlockFbRgb;
			if(tMissFbRgbDone)
			begin
				tNxtMissFbRgbLatch		= 0;
				tNxtMissFbRgbDone		= 0;
//				tNxtHeldCyc				= 4096;
				tDsOpm = UMEM_OPM_READY;
			end
			else
			if(tMemBlockFbRgbDirty)
			begin
				tDsOpm = UMEM_OPM_WR_Q;
				tDsAddr = tMemBlockFbRgbAddr;
				tNxtMissFbRgbLatch		= 1;
			end
			else
			begin
				tDsOpm = UMEM_OPM_RD_Q;
				tDsAddr = tReqBlockFbRgbAddr;
				tNxtMissFbRgbLatch		= 1;
			end
		end
	end
	else
		if(tMissFbZ || tMissFbZLatch)
	begin
		if(tDsOK==UMEM_OK_OK)
		begin
			if(tDsOpm2 == UMEM_OPM_WR_Q)
			begin
				tNxtMemBlockFbZDirty = 0;
			end
			if(tDsOpm2 == UMEM_OPM_RD_Q)
			begin
				tNxtMemBlockFbZ			= tDsDataIn;
				tNxtMemBlockFbZIx		= tReqBlockFbZIx;
				tNxtMemBlockFbZAddr		= tReqBlockFbZAddr;
				tNxtMemBlockFbZDirty	= 0;
//				tNxtMissFbZLatch		= 0;
				tNxtMissFbZDone			= 1;
			end

			tDsOpm = UMEM_OPM_READY;
		end
		else
			if(tDsOK==UMEM_OK_READY)
		begin
			tDsDataOut = tMemBlockFbZ;
			if(tMissFbZDone)
			begin
				tNxtMissFbZLatch		= 0;
				tNxtMissFbZDone			= 0;
//				tNxtHeldCyc				= 4096;
				tDsOpm = UMEM_OPM_READY;
			end
			else
			if(tMemBlockFbRgbDirty)
			begin
				tDsOpm = UMEM_OPM_WR_Q;
				tDsAddr = tMemBlockFbZAddr;
				tNxtMissFbZLatch		= 1;
			end
			else
			begin
				tDsOpm = UMEM_OPM_RD_Q;
				tDsAddr = tReqBlockFbZAddr;
				tNxtMissFbZLatch		= 1;
			end
		end
	end
	else
		if((tMissUtx && tReqBlockUtxReady) || tMissUtxLatch)
	begin
		if(tDsOK==UMEM_OK_OK)
		begin
			$display("ModEdgeWalk: Fetch UTX Ix=%X A=%X", 
				tReqBlockUtxIx, tReqBlockUtxAddr);

//			tNxtMissUtxLatch		= 0;
			tNxtMissUtxDone			= 1;

			if(!tMissUtxDone)
			begin
				tNxtMemBlockUtxAddr		= tReqBlockUtxAddr;

				tNxtMemBlockUtxA		= tDsDataIn;
				tNxtMemBlockUtxIxA		= tReqBlockUtxIx;

				tNxtMemBlockUtxB		= tMemBlockUtxA;
				tNxtMemBlockUtxIxB		= tMemBlockUtxIxA;

				tNxtMemBlockUtxC		= tMemBlockUtxB;
				tNxtMemBlockUtxIxC		= tMemBlockUtxIxB;

				tNxtMemBlockUtxD		= tMemBlockUtxC;
				tNxtMemBlockUtxIxD		= tMemBlockUtxIxC;
			end

			tDsOpm = UMEM_OPM_READY;
		end
		else
			if(tDsOK==UMEM_OK_READY)
		begin
			if(tMissUtxDone)
			begin
				tNxtMissUtxLatch	= 0;
				tNxtMissUtxDone		= 0;
//				tNxtHeldCyc			= 4096;
				tDsOpm				= UMEM_OPM_READY;
			end
			else
			begin
				tDsOpm				= UMEM_OPM_RD_Q;
				tDsAddr				= tReqBlockUtxAddr;
				tNxtMissUtxLatch	= 1;
			end
		end
	end
	else
	begin
		tDsOpm = UMEM_OPM_READY;
		tNxtMissFbRgbDone			= 0;
		tNxtMissFbZDone				= 0;
		tNxtMissUtxDone				= 0;
//		tNxtHeldCyc					= 131072;
		tNxtHeldCyc					= 4096;
	end

	if(tHeldCyc[23:8]==0)
	begin
		$display("ModEdgeWalk: Miss Timeout %d", tHeldCyc);

		tNxtMemBlockFbRgbIx		= tReqBlockFbRgbIx;
		tNxtMemBlockFbRgbAddr	= tReqBlockFbRgbAddr;

		tNxtMemBlockFbZIx		= tReqBlockFbZIx;
		tNxtMemBlockFbZAddr		= tReqBlockFbZAddr;

		tNxtMemBlockUtxIxA		= tReqBlockUtxIx;
		tNxtMemBlockUtxAddr		= tReqBlockUtxAddr;

		tNxtMemBlockUtxB		= tMemBlockUtxA;
		tNxtMemBlockUtxIxB		= tMemBlockUtxIxA;

		tNxtMemBlockUtxC		= tMemBlockUtxB;
		tNxtMemBlockUtxIxC		= tMemBlockUtxIxB;

		tNxtMemBlockUtxD		= tMemBlockUtxC;
		tNxtMemBlockUtxIxD		= tMemBlockUtxIxC;

		if(tMissFbRgbLatch)
		begin
			$display("  FbRGB A=%X", tReqBlockFbRgbAddr);
		end

		if(tMissFbZLatch)
		begin
			$display("  FbZ A=%X", tReqBlockFbZAddr);
		end

		if(tMissUtxLatch)
		begin
			$display("  UTX A=%X", tReqBlockUtxAddr);
		end

		tDsOpm						= UMEM_OPM_READY;

		tNxtMissFbRgbDone			= 0;
		tNxtMissFbZDone				= 0;
		tNxtMissUtxDone				= 0;

		tNxtMissFbRgbLatch			= 0;
		tNxtMissFbZLatch			= 0;
		tNxtMissUtxLatch			= 0;
	end

	if(tBusCSelOE)
	begin
		case(tBusAddr[7:3])
			5'h00:		tBusDataOut	= tStFifoCtrl0;
			5'h01:		tBusDataOut	= tStFifoCtrl1;
			5'h02:		tBusDataOut	= tStFifoCtrl2;
			5'h03:		tBusDataOut	= tStFifoCtrl3;
			5'h04:		tBusDataOut	= tStFifoCtrl4;
			5'h05:		tBusDataOut	= tStFifoCtrl5;
			5'h06:		tBusDataOut	= tStFifoCtrl6;
			5'h07:		tBusDataOut	= tStFifoCtrl7;
			5'h08:		tBusDataOut	= tStFifoCtrl8;
			5'h09:		tBusDataOut	= tStFifoCtrl9;
			5'h0A:		tBusDataOut	= tStFifoCtrl10;
			5'h0B:		tBusDataOut	= tStFifoCtrl11;
			5'h0C:		tBusDataOut	= tStFifoCtrl12;
			5'h0D:		tBusDataOut	= tStFifoCtrl13;
			5'h0E:		tBusDataOut	= tStFifoCtrl14;
			5'h0F:		tBusDataOut	= tStFifoCtrl15;
			5'h10:		tBusDataOut	= tStFifoCtrl16;
			default:	tBusDataOut	= 0;
		endcase
		
		if(tBusAddr[11:8]!=0)
			tBusDataOut		= 0;

//		if((tBusAddr[7:3]!=0) || !tStFifoCtrl0[0])
		if(tBusAddr[7:3]!=0)
		begin
			$display("ModEdgeWalk: Get %X = %X", tBusAddr[7:3], tBusDataOut);
		end

		tBusOK			= 1;
	end

	if(tBusCSelWR)
	begin
		$display("ModEdgeWalk: Set %X = %X", tBusAddr[7:3], tBusDataIn);
		if(tBusAddr[11:8]==0)
		begin
			case(tBusAddr[7:3])
				5'h00: tNxtFifoCtrl0 = tBusDataIn;
				5'h01: tNxtFifoCtrl1 = tBusDataIn;
				5'h02: tNxtFifoCtrl2 = tBusDataIn;
				5'h03: tNxtFifoCtrl3 = tBusDataIn;
				5'h04: tNxtFifoCtrl4 = tBusDataIn;
				5'h05: tNxtFifoCtrl5 = tBusDataIn;
				5'h06: tNxtFifoCtrl6 = tBusDataIn;
				5'h07: tNxtFifoCtrl7 = tBusDataIn;
				5'h08: tNxtFifoCtrl8 = tBusDataIn;
				5'h09: tNxtFifoCtrl9 = tBusDataIn;
				5'h0A: tNxtFifoCtrl10 = tBusDataIn;
				5'h0B: tNxtFifoCtrl11 = tBusDataIn;
				5'h0C: tNxtFifoCtrl12 = tBusDataIn;
				5'h0D: tNxtFifoCtrl13 = tBusDataIn;
				5'h0E: tNxtFifoCtrl14 = tBusDataIn;
				5'h0F: tNxtFifoCtrl15 = tBusDataIn;
				5'h10: tNxtFifoCtrl16 = tBusDataIn;
				default: begin end
			endcase
		end

		if(tNxtFifoCtrl0[0] && !tStFifoCtrl0[0])
		begin
			$display("Mod Edge Walk: Add Request %X", tFifoStRov);
			tNxtFifoStRov = tFifoStRov + 1;
		end

		tBusOK			= 1;
	end
	else
	begin
		tNxtFifoCtrl0[0] = (tFifoStRov + 1) == tFifoRqRov;
		tNxtFifoCtrl0[1] = 0;
		tNxtFifoCtrl0[2] = 0;
		tNxtFifoCtrl0[3] = 0;

		if((tScanY <= tScanLimY) || (tScanY[11] && !tScanLimY[11]))
			tNxtFifoCtrl0[2] = 1;
		if(tFifoStRov != tFifoRqRov)
			tNxtFifoCtrl0[2] = 1;
	end
	
	if(tNxtFifoRqRov != tFifoRqRov)
	begin
		$display("ModEdgeWalk: Advance Request %X", tFifoRqRov);
		tNxtRegCtrl0  = tFifoCtrl0 [tFifoRqRov];
		tNxtRegCtrl1  = tFifoCtrl1 [tFifoRqRov];
		tNxtRegCtrl2  = tFifoCtrl2 [tFifoRqRov];
		tNxtRegCtrl3  = tFifoCtrl3 [tFifoRqRov];
		tNxtRegCtrl4  = tFifoCtrl4 [tFifoRqRov];
		tNxtRegCtrl5  = tFifoCtrl5 [tFifoRqRov];
		tNxtRegCtrl6  = tFifoCtrl6 [tFifoRqRov];
		tNxtRegCtrl7  = tFifoCtrl7 [tFifoRqRov];
		tNxtRegCtrl8  = tFifoCtrl8 [tFifoRqRov];
		tNxtRegCtrl9  = tFifoCtrl9 [tFifoRqRov];
		tNxtRegCtrl10 = tFifoCtrl10[tFifoRqRov];
		tNxtRegCtrl11 = tFifoCtrl11[tFifoRqRov];
		tNxtRegCtrl12 = tFifoCtrl12[tFifoRqRov];
		tNxtRegCtrl13 = tFifoCtrl13[tFifoRqRov];
		tNxtRegCtrl14 = tFifoCtrl14[tFifoRqRov];
		tNxtRegCtrl15 = tFifoCtrl15[tFifoRqRov];
		tNxtRegCtrl16 = tFifoCtrl16[tFifoRqRov];
		$display("  ModEdgeWalk: Ctrl0=%X", tNxtRegCtrl0);
		$display("  ModEdgeWalk: Ctrl1=%X", tNxtRegCtrl1);
		$display("  ModEdgeWalk: Ctrl2=%X", tNxtRegCtrl2);
		$display("  ModEdgeWalk: Ctrl3=%X", tNxtRegCtrl3);
	end
	
	tNxtRegCtrl0[1] = 0;
	
//	if(tNxtRegCtrl0[0] && !tRegCtrl0[0])
	if(tRegCtrl0[0] && !tRegCtrl0L[0])
	begin
		$display("ModEdgeWalk: Begin Request");

		tNxtScanY		= tRegCtrl1[11: 0];
		tNxtScanLimY	= tRegCtrl1[27:16];
		$display("ModEdgeWalk: Ystrt=%X Yend=%X", tNxtScanY, tNxtScanLimY);

		tNxtScanLfX		= tRegCtrl4[57:42];
		tNxtScanRtX		= tRegCtrl5[57:42];
		tNxtScanStepLfX	= tRegCtrl6[57:42];
		tNxtScanStepRtX	= tRegCtrl7[57:42];

		tNxtScanLfZ		= tRegCtrl4[31:16];
		tNxtScanRtZ		= tRegCtrl5[31:16];
		tNxtScanStepLfZ	= tRegCtrl6[31:16];
		tNxtScanStepRtZ	= tRegCtrl7[31:16];

		tNxtScanLfS		= tRegCtrl8 [25:10];
		tNxtScanRtS		= tRegCtrl9 [25:10];
		tNxtScanStepLfS	= tRegCtrl10[25:10];
		tNxtScanStepRtS	= tRegCtrl11[25:10];

		tNxtScanLfT		= tRegCtrl8 [57:42];
		tNxtScanRtT		= tRegCtrl9 [57:42];
		tNxtScanStepLfT	= tRegCtrl10[57:42];
		tNxtScanStepRtT	= tRegCtrl11[57:42];

		tNxtScanLfR		= tRegCtrl12[47:36];
		tNxtScanRtR		= tRegCtrl13[47:36];
		tNxtScanStepLfR	= tRegCtrl14[47:36];
		tNxtScanStepRtR	= tRegCtrl15[47:36];

		tNxtScanLfG		= tRegCtrl12[31:20];
		tNxtScanRtG		= tRegCtrl13[31:20];
		tNxtScanStepLfG	= tRegCtrl14[31:20];
		tNxtScanStepRtG	= tRegCtrl15[31:20];

		tNxtScanLfB		= tRegCtrl12[15: 4];
		tNxtScanRtB		= tRegCtrl13[15: 4];
		tNxtScanStepLfB	= tRegCtrl14[15: 4];
		tNxtScanStepRtB	= tRegCtrl15[15: 4];

		tNxtScanLfA		= tRegCtrl12[63:52];
		tNxtScanRtA		= tRegCtrl13[63:52];
		tNxtScanStepLfA	= tRegCtrl14[63:52];
		tNxtScanStepRtA	= tRegCtrl15[63:52];

		tNxtPixX		= 12'h7FF;
		tNxtPixLimX		= 12'h7FF;

		if(tRegCtrl3[31:0]!=tRegCtrl3L[31:0])
		begin
			tNxtMemBlockUtxIxA	= 16'hFFFF;
			tNxtMemBlockUtxIxB	= 16'hFFFF;
			tNxtMemBlockUtxIxC	= 16'hFFFF;
			tNxtMemBlockUtxIxD	= 16'hFFFF;
		end
	end
	
	if(reset)
	begin
		tNxtRegCtrl0[0] = 0;
		tNxtScanY		= 0;
		tNxtScanLimY	= 0;
	end
end

always @(posedge clock)
begin
	tDsDataOut2		<= tDsDataOut;
	tDsAddr2		<= tDsAddr;
	tDsOpm2			<= tDsOpm;

	tDsDataIn		<= dsDataIn;
	tDsOK			<= dsOK;

	tBusDataOut2	<= tBusDataOut;
	tBusOK2			<= tBusOK;

	tBusDataIn		<= busDataIn;
	tBusAddr		<= busAddr;
	tBusOpm			<= busOpm;

	tHeldCyc		<= tNxtHeldCyc;

	tRegCtrl0L		<= tRegCtrl0;
	tRegCtrl3L		<= tRegCtrl3;

	tRegCtrl0		<= tNxtRegCtrl0;
	tRegCtrl1		<= tNxtRegCtrl1;
	tRegCtrl2		<= tNxtRegCtrl2;
	tRegCtrl3		<= tNxtRegCtrl3;
	tRegCtrl4		<= tNxtRegCtrl4;
	tRegCtrl5		<= tNxtRegCtrl5;
	tRegCtrl6		<= tNxtRegCtrl6;
	tRegCtrl7		<= tNxtRegCtrl7;
	tRegCtrl8		<= tNxtRegCtrl8;
	tRegCtrl9		<= tNxtRegCtrl9;
	tRegCtrl10		<= tNxtRegCtrl10;
	tRegCtrl11		<= tNxtRegCtrl11;
	tRegCtrl12		<= tNxtRegCtrl12;
	tRegCtrl13		<= tNxtRegCtrl13;
	tRegCtrl14		<= tNxtRegCtrl14;
	tRegCtrl15		<= tNxtRegCtrl15;
	tRegCtrl16		<= tNxtRegCtrl16;

	tStFifoCtrl0	<= tNxtFifoCtrl0;
	tStFifoCtrl1	<= tNxtFifoCtrl1;
	tStFifoCtrl2	<= tNxtFifoCtrl2;
	tStFifoCtrl3	<= tNxtFifoCtrl3;
	tStFifoCtrl4	<= tNxtFifoCtrl4;
	tStFifoCtrl5	<= tNxtFifoCtrl5;
	tStFifoCtrl6	<= tNxtFifoCtrl6;
	tStFifoCtrl7	<= tNxtFifoCtrl7;
	tStFifoCtrl8	<= tNxtFifoCtrl8;
	tStFifoCtrl9	<= tNxtFifoCtrl9;
	tStFifoCtrl10	<= tNxtFifoCtrl10;
	tStFifoCtrl11	<= tNxtFifoCtrl11;
	tStFifoCtrl12	<= tNxtFifoCtrl12;
	tStFifoCtrl13	<= tNxtFifoCtrl13;
	tStFifoCtrl14	<= tNxtFifoCtrl14;
	tStFifoCtrl15	<= tNxtFifoCtrl15;
	tStFifoCtrl16	<= tNxtFifoCtrl16;

	tFifoStRov		<= tNxtFifoStRov;
	tFifoRqRov		<= tNxtFifoRqRov;

	if(tNxtFifoStRov != tFifoStRov)
	begin
		tFifoCtrl0[tFifoStRov]	<= tNxtFifoCtrl0;

		tFifoCtrl1[tFifoStRov]	<= tStFifoCtrl1;
		tFifoCtrl2[tFifoStRov]	<= tStFifoCtrl2;
		tFifoCtrl3[tFifoStRov]	<= tStFifoCtrl3;
		tFifoCtrl4[tFifoStRov]	<= tStFifoCtrl4;
		tFifoCtrl5[tFifoStRov]	<= tStFifoCtrl5;
		tFifoCtrl6[tFifoStRov]	<= tStFifoCtrl6;
		tFifoCtrl7[tFifoStRov]	<= tStFifoCtrl7;
		tFifoCtrl8[tFifoStRov]	<= tStFifoCtrl8;
		tFifoCtrl9[tFifoStRov]	<= tStFifoCtrl9;
		tFifoCtrl10[tFifoStRov]	<= tStFifoCtrl10;
		tFifoCtrl11[tFifoStRov]	<= tStFifoCtrl11;
		tFifoCtrl12[tFifoStRov]	<= tStFifoCtrl12;
		tFifoCtrl13[tFifoStRov]	<= tStFifoCtrl13;
		tFifoCtrl14[tFifoStRov]	<= tStFifoCtrl14;
		tFifoCtrl15[tFifoStRov]	<= tStFifoCtrl15;
		tFifoCtrl16[tFifoStRov]	<= tStFifoCtrl16;
	end

	tMemBlockFbRgb			<= tNxtMemBlockFbRgb;
	tMemBlockFbZ			<= tNxtMemBlockFbZ;

	tMemBlockUtxA			<= tNxtMemBlockUtxA;
	tMemBlockUtxB			<= tNxtMemBlockUtxB;
	tMemBlockUtxC			<= tNxtMemBlockUtxC;
	tMemBlockUtxD			<= tNxtMemBlockUtxD;

	tMemBlockFbRgbIx		<= tNxtMemBlockFbRgbIx;
	tMemBlockFbZIx			<= tNxtMemBlockFbZIx;

	tMemBlockUtxIxA			<= tNxtMemBlockUtxIxA;
	tMemBlockUtxIxB			<= tNxtMemBlockUtxIxB;
	tMemBlockUtxIxC			<= tNxtMemBlockUtxIxC;
	tMemBlockUtxIxD			<= tNxtMemBlockUtxIxD;

	tMemBlockFbRgbAddr		<= tNxtMemBlockFbRgbAddr;
	tMemBlockFbZAddr		<= tNxtMemBlockFbZAddr;
	tMemBlockUtxAddr		<= tNxtMemBlockUtxAddr;

	tMemBlockFbRgbDirty		<= tNxtMemBlockFbRgbDirty;
	tMemBlockFbZDirty		<= tNxtMemBlockFbZDirty;

	tReqBlockFbRgbIx		<= tNxtReqBlockFbRgbIx;
	tReqBlockFbZIx			<= tNxtReqBlockFbZIx;

	tReqBlockUtxIx			<= tNxtReqBlockUtxIx;
	tReqBlockUtxIxA			<= tNxtReqBlockUtxIxA;
	tReqBlockUtxIxB			<= tNxtReqBlockUtxIxB;
	tReqBlockUtxIxC			<= tNxtReqBlockUtxIxC;
	tReqBlockUtxIxD			<= tNxtReqBlockUtxIxD;

	tReqBlockFbRgbAddr		<= tNxtReqBlockFbRgbAddr;
	tReqBlockFbZAddr		<= tNxtReqBlockFbZAddr;
	tReqBlockUtxAddr		<= tNxtReqBlockUtxAddr;

	tScanY				<= tNxtScanY;
	tScanLimY			<= tNxtScanLimY;

	tScanLfX			<= tNxtScanLfX;
	tScanRtX			<= tNxtScanRtX;
	tScanStepLfX		<= tNxtScanStepLfX;
	tScanStepRtX		<= tNxtScanStepRtX;

	tScanLfZ			<= tNxtScanLfZ;
	tScanRtZ			<= tNxtScanRtZ;
	tScanStepLfZ		<= tNxtScanStepLfZ;
	tScanStepRtZ		<= tNxtScanStepRtZ;

	tScanLfS			<= tNxtScanLfS;
	tScanRtS			<= tNxtScanRtS;
	tScanStepLfS		<= tNxtScanStepLfS;
	tScanStepRtS		<= tNxtScanStepRtS;

	tScanLfT			<= tNxtScanLfT;
	tScanRtT			<= tNxtScanRtT;
	tScanStepLfT		<= tNxtScanStepLfT;
	tScanStepRtT		<= tNxtScanStepRtT;

	tScanLfR			<= tNxtScanLfR;
	tScanRtR			<= tNxtScanRtR;
	tScanStepLfR		<= tNxtScanStepLfR;
	tScanStepRtR		<= tNxtScanStepRtR;

	tScanLfG			<= tNxtScanLfG;
	tScanRtG			<= tNxtScanRtG;
	tScanStepLfG		<= tNxtScanStepLfG;
	tScanStepRtG		<= tNxtScanStepRtG;

	tScanLfB			<= tNxtScanLfB;
	tScanRtB			<= tNxtScanRtB;
	tScanStepLfB		<= tNxtScanStepLfB;
	tScanStepRtB		<= tNxtScanStepRtB;

	tScanLfA			<= tNxtScanLfA;
	tScanRtA			<= tNxtScanRtA;
	tScanStepLfA		<= tNxtScanStepLfA;
	tScanStepRtA		<= tNxtScanStepRtA;

	tScanStepPixZ		<= tNxtScanStepPixZ;
	tScanStepPixS		<= tNxtScanStepPixS;
	tScanStepPixT		<= tNxtScanStepPixT;
	tScanStepPixR		<= tNxtScanStepPixR;
	tScanStepPixG		<= tNxtScanStepPixG;
	tScanStepPixB		<= tNxtScanStepPixB;
	tScanStepPixA		<= tNxtScanStepPixA;

	tPixX				<= tNxtPixX;
	tPixLimX			<= tNxtPixLimX;

	tPixZ				<= tNxtPixZ;
	tPixStepZ			<= tNxtPixStepZ;
	tPixS				<= tNxtPixS;
	tPixStepS			<= tNxtPixStepS;
	tPixT				<= tNxtPixT;
	tPixStepT			<= tNxtPixStepT;

	tPixR				<= tNxtPixR;
	tPixStepR			<= tNxtPixStepR;
	tPixG				<= tNxtPixG;
	tPixStepG			<= tNxtPixStepG;
	tPixB				<= tNxtPixB;
	tPixStepB			<= tNxtPixStepB;
	tPixA				<= tNxtPixA;
	tPixStepA			<= tNxtPixStepA;

	tPixUtxRgbA			<= tPixUtxRgb0a;
	tPixUtxRgbB			<= tPixUtxRgb0b;
	tPixUtxRgbC			<= tPixUtxRgb0c;

	tPixModRgbL			<= tPixModRgb;
	tPixDestRgbL		<= tPixDestRgb;
	tPixBlendRgbL		<= tPixBlendRgb;

	tMissFbRgbLatch		<= tNxtMissFbRgbLatch;
	tMissFbZLatch		<= tNxtMissFbZLatch;
	tMissUtxLatch		<= tNxtMissUtxLatch;
	tMissFbRgbDone		<= tNxtMissFbRgbDone;
	tMissFbZDone		<= tNxtMissFbZDone;
	tMissUtxDone		<= tNxtMissUtxDone;

	tPixStepInh			<= tNxtPixStepInh;
end

endmodule
