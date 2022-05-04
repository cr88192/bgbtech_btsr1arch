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
FPU Execute Unit (Wide)

The FPU is essentially a slave to the main Execute Unit.
It will recieve copies of some inputs, but it is the main Execute unit which will initiate the action.

opUCmd:
  [7:6]=CC (AL/NV/CT/CF)
  [5:0]=Opcode Bits

opUIxt:
  [7:6]=Resv
  [  4]=Use GPRs
  [3:0]=Depends on opcode


The Wide variant will span two lanes and implement FP-SIMD.

 */


`include "CoreDefs.v"

`ifdef jx2_fpu_longdbl
`include "FpuAddG.v"
`include "FpuMulG.v"
`else
`include "FpuAdd.v"
`include "FpuMul.v"
`endif

`include "FpuConvS2D.v"
`include "FpuConvH2D.v"
`include "FpuConvD2S.v"
`include "FpuConvD2H.v"
// `include "FpuConvD2I.v"

module FpuExOpW(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmdA,		regIdIxtA,
	opCmdB,		regIdIxtB,
	regValImmA,	regValImmB,

	regIdRsA,	regValRsA,
	regIdRtA,	regValRtA,
	regIdRnA,	regValRnA,

	regIdRsB,	regValRsB,
	regIdRtB,	regValRtB,
	regIdRnB,	regValRnB,

	regOutOK,	regOutSrT,
	regInFpsr,	regOutFpsr,

	regInSr,	braFlush,
	exHold,

	regValGRnA,	regValGRnB

//	regValLdGRn,
//	memDataLd,	memDataOK
	);

input			clock;
input			reset;

input[8:0]		opCmdA;			//command opcode
input[8:0]		opCmdB;			//command opcode
input[8:0]		regIdIxtA;		//ALU Index / Opcode Extension
input[8:0]		regIdIxtB;		//ALU Index / Opcode Extension

input[32:0]		regValImmA;
input[32:0]		regValImmB;

`input_gpr		regIdRsA;
`input_gpr		regIdRtA;
`input_gpr		regIdRnA;
`input_gpr		regIdRsB;
`input_gpr		regIdRtB;
`input_gpr		regIdRnB;

input[63:0]		regValRsA;		//Rs input value (Lane 1)
input[63:0]		regValRtA;		//Rt input value (Lane 1)
input[63:0]		regValRnA;		//Rn input value (Lane 1)
input[63:0]		regValRsB;		//Rs input value (Lane 2)
input[63:0]		regValRtB;		//Rt input value (Lane 2)
input[63:0]		regValRnB;		//Rn input value (Lane 2)

// input[63:0]		regValGRm;		//Rm input value (GPR)
output[63:0]	regValGRnA;		//Rn output value (Lane 1)
output[63:0]	regValGRnB;		//Rn output value (Lane 2)
// output[63:0]	regValLdGRn;	//Rn output (Mem Load)
output			regOutSrT;

// output[63:0]	regOutVal;		//Ro output value
// `output_gpr		regOutId;		//Ro, register to write (FPR)
output[1:0]		regOutOK;		//execute status

// input[63:0]		memDataLd;		//memory data (load)
// input[1:0]		memDataOK;		//memory status (load)

input[63:0]		regInSr;		//input SR
input			braFlush;
input			exHold;

input[15:0]		regInFpsr;
output[15:0]	regOutFpsr;

reg[15:0]		tRegOutFpsr;
assign		regOutFpsr = tRegOutFpsr;

// reg[63:0]	tRegOutVal;			//Rn output value
// `reg_gpr	tRegOutId;			//Rn, value to write
reg[1:0]	tRegOutOK;			//execute status
reg			tRegOutSrT;
reg[63:0]	tRegValGRn;			//GRn Out
reg[63:0]	tRegValGRnA;		//GRn Out
reg[63:0]	tRegValGRnB;		//GRn Out
// reg[63:0]	tRegValLdGRn;		//GRn Out

// reg[63:0]	tRegOutVal2;		//Rn output value
// `reg_gpr	tRegOutId2;			//Rn, value to write
reg[1:0]	tRegOutOK2;			//execute status
reg			tRegOutSrT2;
reg[63:0]	tRegValGRn2;			//GRn Out
reg[63:0]	tRegValLdGRn2;		//GRn Out

// assign		regOutVal		= tRegOutVal;
// assign		regOutId		= tRegOutId;
assign		regOutOK		= tRegOutOK;
assign		regOutSrT		= tRegOutSrT;
assign		regValGRnA		= tRegValGRnA;
assign		regValGRnB		= tRegValGRnB;
// assign		regValLdGRn		= tRegValLdGRn;

`ifndef def_true
reg[8:0]		tOpCmd;			//command opcode
reg[8:0]		tRegIdIxt;		//ALU Index / Opcode Extension
reg[7:0]		tRegValRMode;
`reg_gpr		tRegIdRs;
`reg_gpr		tRegIdRt;
`reg_gpr		tRegIdRn;
reg[63:0]		tRegValRs;		//Rs input value (FPR)
reg[63:0]		tRegValRt;		//Rt input value (FPR)
reg[63:0]		tRegValRn;		//Rn input value (FPR, Duplicate)
reg[63:0]		tRegInSr;
reg				tBraFlush;
`endif

`ifdef def_true
wire[8:0]		tOpCmd;			//command opcode
wire[8:0]		tRegIdIxt;		//ALU Index / Opcode Extension
wire[7:0]		tRegValRMode;
`wire_gpr		tRegIdRs;
`wire_gpr		tRegIdRt;
`wire_gpr		tRegIdRn;
wire[63:0]		tRegValRs;		//Rs input value (FPR)
wire[63:0]		tRegValRt;		//Rt input value (FPR)
wire[63:0]		tRegValRn;		//Rn input value (FPR, Duplicate)

wire[8:0]		tOpCmdA;			//command opcode
wire[8:0]		tRegIdIxtA;		//ALU Index / Opcode Extension
wire[7:0]		tRegRModeA;		//Rounding Mode

`wire_gpr		tRegIdRsA;
`wire_gpr		tRegIdRtA;
`wire_gpr		tRegIdRnA;
wire[63:0]		tRegValRsA;		//Rs input value (FPR)
wire[63:0]		tRegValRtA;		//Rt input value (FPR)
wire[63:0]		tRegValRnA;		//Rn input value (FPR, Duplicate)

wire[8:0]		tOpCmdB;			//command opcode
wire[8:0]		tRegIdIxtB;		//ALU Index / Opcode Extension
wire[7:0]		tRegRModeB;		//Rounding Mode
`wire_gpr		tRegIdRsB;
`wire_gpr		tRegIdRtB;
`wire_gpr		tRegIdRnB;
wire[63:0]		tRegValRsB;		//Rs input value (FPR)
wire[63:0]		tRegValRtB;		//Rt input value (FPR)
wire[63:0]		tRegValRnB;		//Rn input value (FPR, Duplicate)

wire[63:0]		tRegInSr;
wire			tBraFlush;
`endif

reg[8:0]		tOpCmdL;			//command opcode
reg[8:0]		tRegIdIxtL;			//ALU Index / Opcode Extension
reg[7:0]		tRegValRModeL;
reg				tExCmdLaneBL;

`reg_gpr		tRegIdRsL;
`reg_gpr		tRegIdRtL;
`reg_gpr		tRegIdRnL;
reg[63:0]		tRegValRsL;			//Rs input value (FPR)
reg[63:0]		tRegValRtL;			//Rt input value (FPR)
reg[63:0]		tRegValRnL;			//Rn input value (FPR, Duplicate)
reg[63:0]		tRegInSrL;
reg				tBraFlushL;

reg[63:0]		tRegValRcpL;		//Reciprocal (Working Value)
reg[63:0]		tRegValRcpL2;		//Reciprocal (Working Value)
reg[63:0]		tRegValRcpL3;		//Reciprocal (Working Value)
reg[63:0]		tRegValRcpL4;		//Reciprocal (Working Value)
reg[63:0]		tRegValRcpL5;		//Reciprocal (Working Value)
reg[63:0]		tRegValRcpL6;		//Reciprocal (Working Value)
reg[63:0]		tRegValRcp;			//Reciprocal (Working Value)

reg[63:0]		tRegValRcpS;		//Reciprocal (Working Value)
reg[63:0]		tRegValRcpSL;		//Reciprocal (Working Value)
reg				tRegValRcpDoS;		//Reciprocal (Working Value)
reg				tRegValRcpDoSL;		//Reciprocal (Working Value)

reg[63:0]		tRegDifRcpL;		//Reciprocal Delta
reg[63:0]		tRegDifRcp;			//Reciprocal Delta
reg[63:0]		tRegDifRcpA;		//Reciprocal Delta
reg[63:0]		tRegTgtRcpL;		//Reciprocal Target
reg[63:0]		tRegTgtRcp;			//Reciprocal Target


`reg_gpr		tRegIdRsAL;
`reg_gpr		tRegIdRtAL;
`reg_gpr		tRegIdRnAL;
reg[63:0]		tRegValRsAL;		//Rs input value (FPR)
reg[63:0]		tRegValRtAL;		//Rt input value (FPR)
reg[63:0]		tRegValRnAL;		//Rn input value (FPR, Duplicate)


`reg_gpr		tRegIdRsBL;
`reg_gpr		tRegIdRtBL;
`reg_gpr		tRegIdRnBL;
reg[63:0]		tRegValRsBL;		//Rs input value (FPR)
reg[63:0]		tRegValRtBL;		//Rt input value (FPR)
reg[63:0]		tRegValRnBL;		//Rn input value (FPR, Duplicate)
	

wire[31:0]		cnvRegValGRm;

assign			cnvRegValGRm = tRegIdIxt[3] ?
//	regValGRm[63:32] : regValGRm[31: 0];
	tRegValRs[63:32] : tRegValRs[31: 0];

`ifdef jx2_enable_fmov
// wire[63:0]		memDataLd_S2D;		//memory data (Single To Double)
// FpuConvS2D mem_cnv_s2d(memDataLd[31:0], memDataLd_S2D);
`endif

reg[63:0]		ctlInDlr_S2D_L;		//memory data (Single To Double)
reg[63:0]		ctlInDlr_H2D_L;		//memory data (Half To Double)
reg[31:0]		fstcx_D2S_L;
reg[15:0]		fstcx_D2H_L;
// reg[63:0]		fstcx_D2I_L;

wire[63:0]		ctlInDlr_S2D;		//memory data (Single To Double)
// FpuConvS2D dlr_cnv_s2d(regValGRm[31:0], ctlInDlr_S2D);
FpuConvS2D dlr_cnv_s2d(cnvRegValGRm, ctlInDlr_S2D);

wire[63:0]		ctlInDlr_H2D;		//memory data (Half To Double)
// FpuConvH2D dlr_cnv_h2d(regValGRm[15:0], ctlInDlr_H2D);
FpuConvH2D dlr_cnv_h2d(cnvRegValGRm[15:0], ctlInDlr_H2D);

wire[31:0]		fstcx_D2S;
wire[15:0]		fstcx_D2H;
FpuConvD2S mem_cnv_d2s(tRegValRs[63:0], fstcx_D2S);
FpuConvD2H mem_cnv_d2h(tRegValRs[63:0], fstcx_D2H);

// wire[63:0]		fstcx_D2I;
// FpuConvD2I mem_cnv_d2i(clock, reset, exHold, tRegValRs[63:0], fstcx_D2I);

reg[31:0]	tVecCnvRsI;
reg[31:0]	tVecCnvRtI;
wire[63:0]	tVecCnvRsO;
wire[63:0]	tVecCnvRtO;
FpuConvS2D vec_cnv_rs(tVecCnvRsI, tVecCnvRsO);
FpuConvS2D vec_cnv_rt(tVecCnvRtI, tVecCnvRtO);

reg[63:0]	tVecCnvRnI;
wire[31:0]	tVecCnvRnO;
FpuConvD2S vec_cnv_rn(tVecCnvRnI, tVecCnvRnO);

`ifdef def_true
reg[15:0]	tVechCnvRsI;
reg[15:0]	tVechCnvRtI;
wire[63:0]	tVechCnvRsO;
wire[63:0]	tVechCnvRtO;
FpuConvH2D vech_cnv_rs(tVechCnvRsI, tVechCnvRsO);
FpuConvH2D vech_cnv_rt(tVechCnvRtI, tVechCnvRtO);

// reg[63:0]	tVechCnvRnI;
wire[15:0]	tVechCnvRnO;
FpuConvD2H vech_cnv_rn(tVecCnvRnI, tVechCnvRnO);

reg[63:0]	tVecdCnvRs;
reg[63:0]	tVecdCnvRt;
reg[63:0]	tVecdCnvRn;

`endif

reg[63:0]	tVecRnA;
reg[63:0]	tVecRnB;
reg[63:0]	tVechRn;
reg[63:0]	tNxtVecRnA;
reg[63:0]	tNxtVecRnB;
reg[63:0]	tNxtVechRn;

reg[63:0]	tVecdRnA;
reg[63:0]	tVecdRnB;
reg[63:0]	tNxtVecdRnA;
reg[63:0]	tNxtVecdRnB;

reg[7:0]	tRegAddRMode;	//Rounding Mode
reg[63:0]	tRegAddRs;		//Rn input value
reg[63:0]	tRegAddRt;		//Rn input value
reg[63:0]	tRegAddRsHi;	//Rn input value (High Bits, LD Ext)
reg[63:0]	tRegAddRtHi;	//Rn input value (High Bits, LD Ext)
reg			tRegAddIsLD;	//Is Long-Double

reg[7:0]	tRegMulRMode;	//Rounding Mode
reg[63:0]	tRegMulRs;		//Rn input value
reg[63:0]	tRegMulRt;		//Rn input value
reg[63:0]	tRegMulRsHi;	//Rn input value (High Bits, LD Ext)
reg[63:0]	tRegMulRtHi;	//Rn input value (High Bits, LD Ext)

wire		tExCmdIsSimd;
wire		tExCmdIsMac;
wire	tFpuIsFpu3;
wire	tFpuIsFldcx;
wire	tFpuIsFstcx;

// reg			tAddExHold;
wire			tAddExHold;
wire			tMulExHold;
// assign		tAddExHold = exHold && !tExCmdIsSimd && !tExCmdIsMac;
// assign		tMulExHold = exHold && !tExCmdIsSimd;

assign		tAddExHold = 0;
assign		tMulExHold = 0;

wire[63:0]	tRegAddVal;		//Rn output value
wire[63:0]	tRegAddValHi;	//Rn output value
wire[3:0]	tRegAddExOp;	
wire[3:0]	tRegAddExOpA;
wire[3:0]	tRegAddSimdExOp;	
wire[1:0]	tRegAddExOK;	

reg[3:0]	tRegAddExOpL;

reg[63:0]	tRegMulValL;	//Rn output value (last cycle)
wire[63:0]	tRegMulVal;		//Rn output value
wire[63:0]	tRegMulValHi;		//Rn output value
wire[1:0]	tRegMulExOK;	
wire[3:0]	tRegMulExOp;
wire[3:0]	tRegMulExOpA;

`ifdef jx2_fpu_longdbl

FpuAddG	fpu_add(
	clock,		reset,
	tAddExHold,
	tRegAddRt,	tRegAddRtHi,
	tRegAddRs,	tRegAddRsHi,
	tRegAddVal,	tRegAddValHi,
	tRegAddExOp,
	tRegAddExOK, tRegMulRMode);

FpuMulG	fpu_mul(
	clock,			reset,		tMulExHold,
//	tRegAddRt,		tRegAddRtHi,
//	tRegAddRs,		tRegAddRsHi,
	tRegMulRt,		tRegMulRtHi,
	tRegMulRs,		tRegMulRsHi,
	tRegMulVal,		tRegMulValHi,
	tRegMulExOp,	tRegMulExOK,
	tRegMulRMode
	);

`else

assign	tRegAddValHi = 64'h0;
assign	tRegMulValHi = 64'h0;

FpuAdd	fpu_add(
	clock,		reset,
	tAddExHold,
	tRegAddRt,	tRegAddRs,
	tRegAddVal,
	tRegAddExOp,
	tRegAddExOK,
	tRegMulRMode);

FpuMul	fpu_mul(
	clock,			reset,		tMulExHold,
//	tRegAddRt,		tRegAddRs,	tRegMulVal);
	tRegMulRt,		tRegMulRs,	tRegMulVal,
	tRegMulExOp,	tRegMulExOK,
	tRegMulRMode);

`endif

wire tExCmdIsCnvLd;

wire	tFpuIsLongDbl;
assign	tFpuIsLongDbl = tRegIdIxtL[5];

assign	tRegAddSimdExOp	=
	(tRegIdIxtL[3:0]==JX2_UCIX_FPU_PADD[3:0]) ? 4'h1 :
	(tRegIdIxtL[3:0]==JX2_UCIX_FPU_PSUB[3:0]) ? 4'h2 :
	4'h0;

wire[3:0]	tRegAddCnvLdExOp;

`ifndef def_true
assign	tFpuIsFpu3 = (tOpCmd[5:0]==JX2_UCMD_FPU3);
assign	tFpuIsFldcx = (tOpCmd[5:0]==JX2_UCMD_FLDCX);
assign	tFpuIsFstcx = (tOpCmd[5:0]==JX2_UCMD_FSTCX);

assign	tRegAddCnvLdExOp	=
	(tFpuIsFldcx && (tRegIdIxt[3:0]==1)) ? 4'h5 :
	(tFpuIsFstcx && (tRegIdIxt[3:0]==1)) ? 4'hD :
	(tFpuIsFldcx && (tRegIdIxt[3:0]==2)) ? 4'hB :
	(tFpuIsFstcx && (tRegIdIxt[3:0]==2)) ? 4'hC :
	4'h0;

assign	tRegAddExOpA	=
	(tExCmdIsSimd) ? tRegAddSimdExOp :
	(tExCmdIsCnvLd) ? tRegAddCnvLdExOp :
	{ tFpuIsLongDbl,
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FADD[3:0])) ? 3'h1 :
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FSUB[3:0])) ? 3'h2 :
	(tFpuIsFpu3 && (tRegIdIxt[3:0]==JX2_UCIX_FPU_FMUL[3:0])) ? 3'h7 :
	(tFpuIsFldcx && (tRegIdIxt[3:0]==4'h2)) ? 3'h3 :
	(tFpuIsFstcx && (tRegIdIxt[3:0]==4'h2)) ? 3'h4 :
	3'h0 };

assign	tRegMulExOpA	=
	(tExCmdIsSimd) ? tRegAddSimdExOp :
	(tExCmdIsCnvLd) ? tRegAddCnvLdExOp :
	{ tFpuIsLongDbl, 3'h0 };

assign	tRegAddExOp	=
	(exHold && !tExCmdIsSimd) ? tRegAddExOpL : tRegAddExOpA;

assign	tRegMulExOp = tRegMulExOpA;


assign	tExCmdIsCnvLd =
	(	(tOpCmd[5:0]==JX2_UCMD_FLDCX)	||
		(tOpCmd[5:0]==JX2_UCMD_FSTCX)	)	&&
	(	(tRegIdIxt[5:0]==JX2_UCIX_FPCX_XE)	||
		(tRegIdIxt[5:0]==JX2_UCIX_FPCX_XI) );

assign	tExCmdIsSimd =
	(tOpCmdL[5:0]==JX2_UCMD_FPU3) 	&&
	(	(tRegIdIxtL[3:0]==4'h5) ||
		(tRegIdIxtL[3:0]==4'h6) ||
		(tRegIdIxtL[3:0]==4'h7)	);

assign	tExCmdIsMac =
	(tOpCmdL[5:0]==JX2_UCMD_FPU3) 	&&
	(	(tRegIdIxtL[3:0]==4'h9) ||
		(tRegIdIxtL[3:0]==4'hB) ||
		(tRegIdIxtL[3:0]==4'hD)	);

`endif


reg	tMulFlipSign;

`ifdef def_true
assign	tFpuIsFpu3 = (tOpCmdL[5:0]==JX2_UCMD_FPU3);
assign	tFpuIsFldcx = (tOpCmdL[5:0]==JX2_UCMD_FLDCX);
assign	tFpuIsFstcx = (tOpCmdL[5:0]==JX2_UCMD_FSTCX);

assign	tRegAddCnvLdExOp	=
	(tFpuIsFldcx && (tRegIdIxtL[3:0]==1)) ? 4'h5 :
	(tFpuIsFstcx && (tRegIdIxtL[3:0]==1)) ? 4'hD :
	(tFpuIsFldcx && (tRegIdIxtL[3:0]==2)) ? 4'hB :
	(tFpuIsFstcx && (tRegIdIxtL[3:0]==2)) ? 4'hC :
	4'h0;

assign	tRegAddExOpA	=
	(tExCmdIsSimd) ? tRegAddSimdExOp :
	(tExCmdIsCnvLd) ? tRegAddCnvLdExOp :
	{ tFpuIsLongDbl,
	(tFpuIsFpu3 && (tRegIdIxtL[3:0]==JX2_UCIX_FPU_FADD[3:0])) ? 3'h1 :
	(tFpuIsFpu3 && (tRegIdIxtL[3:0]==JX2_UCIX_FPU_FSUB[3:0])) ? 3'h2 :
	(tFpuIsFpu3 && (tRegIdIxtL[3:0]==JX2_UCIX_FPU_FMUL[3:0])) ? 3'h7 :
	(tFpuIsFldcx && (tRegIdIxtL[3:0]==4'h2)) ? 3'h3 :
	(tFpuIsFstcx && (tRegIdIxtL[3:0]==4'h2)) ? 3'h4 :
	3'h0 };

assign	tRegMulExOpA	=
	(tExCmdIsSimd) ? tRegAddSimdExOp :
	(tExCmdIsCnvLd) ? tRegAddCnvLdExOp :
	{ tFpuIsLongDbl, 2'b00, tMulFlipSign };

assign	tRegAddExOp	= tRegAddExOpA;
assign	tRegMulExOp	= tRegMulExOpA;

assign	tExCmdIsCnvLd =
	(	(tOpCmdL[5:0]==JX2_UCMD_FLDCX)	||
		(tOpCmdL[5:0]==JX2_UCMD_FSTCX)	)	&&
	(	(tRegIdIxtL[5:0]==JX2_UCIX_FPCX_XE)	||
		(tRegIdIxtL[5:0]==JX2_UCIX_FPCX_XI) );

assign	tExCmdIsSimd =
	(tOpCmdL[5:0]==JX2_UCMD_FPU3) 	&&
	(	(tRegIdIxtL[3:0]==4'h5) ||
		(tRegIdIxtL[3:0]==4'h6) ||
		(tRegIdIxtL[3:0]==4'h7)	);

assign	tExCmdIsMac =
	(tOpCmdL[5:0]==JX2_UCMD_FPU3) 	&&
	(	(tRegIdIxtL[3:0]==4'h9) ||
		(tRegIdIxtL[3:0]==4'hB) ||
		(tRegIdIxtL[3:0]==4'hD)	);

`endif



reg			tOpEnable;
reg[5:0]	tOpUCmd1;

reg			tExHold;
reg			tExValidCmd;
reg[11:0]	tHoldCyc;
reg[11:0]	tDoHoldCyc;

reg			tDivHold;
reg			tDoHoldDiv;
reg			tDoHoldDivL;

`ifdef def_true
assign	tOpCmdA			= opCmdA;
assign	tRegIdIxtA		= regIdIxtA;
assign	tOpCmdB			= opCmdB;
assign	tRegIdIxtB		= regIdIxtB;
assign	tRegRModeA		= regValImmA[7:0];
assign	tRegRModeB		= regValImmB[7:0];

assign	tRegIdRsA		= regIdRsA;
assign	tRegIdRtA		= regIdRtA;
assign	tRegIdRnA		= regIdRnA;
assign	tRegValRsA		= regValRsA;
assign	tRegValRtA		= regValRtA;
assign	tRegValRnA		= regValRnA;

assign	tRegIdRsB		= regIdRsB;
assign	tRegIdRtB		= regIdRtB;
assign	tRegIdRnB		= regIdRnB;
assign	tRegValRsB		= regValRsB;
assign	tRegValRtB		= regValRtB;
assign	tRegValRnB		= regValRnB;

assign	tRegInSr		= regInSr;
assign	tBraFlush		= braFlush;
`endif


wire	tExCmdLaneA;
wire	tExCmdLaneB1;
wire	tExCmdLaneB;
reg		tExCmdVecW;

assign	tExCmdLaneA =
	(tOpCmdA[5:0]==JX2_UCMD_FPU3) 	||
//	(tOpCmdA[5:0]==JX2_UCMD_FCMP)	||
	(tOpCmdA[5:0]==JX2_UCMD_FLDCX)	||
	(tOpCmdA[5:0]==JX2_UCMD_FSTCX)	;

assign	tExCmdLaneB1 =
//	((tOpCmdB[5:0]==JX2_UCMD_FPU3) && !regIdIxtB[5])	||
	(tOpCmdB[5:0]==JX2_UCMD_FPU3)	||
//	(tOpCmdB[5:0]==JX2_UCMD_FCMP)	||
	(tOpCmdB[5:0]==JX2_UCMD_FLDCX)	||
	(tOpCmdB[5:0]==JX2_UCMD_FSTCX)	;

wire[7:0]		regRModeA;
wire[7:0]		regRModeB;

assign		regRModeA = regValImmA[7:0];
assign		regRModeB = regValImmB[7:0];


`ifdef jx2_fpu_lane2

assign	tExCmdLaneB = tExCmdLaneB1 && !tExCmdLaneA;
	
assign	tOpCmd			= tExCmdLaneB ? opCmdB : opCmdA;
assign	tRegIdIxt		= tExCmdLaneB ? regIdIxtB : regIdIxtA;
assign	tRegValRMode	= tExCmdLaneB ? regRModeB : regRModeA;

assign	tRegIdRs		= tExCmdLaneB ? regIdRsB : regIdRsA;
assign	tRegIdRt		= tExCmdLaneB ? regIdRtB : regIdRtA;
assign	tRegIdRn		= tExCmdLaneB ? regIdRnB : regIdRnA;
assign	tRegValRs		= tExCmdLaneB ? regValRsB : regValRsA;
assign	tRegValRt		= tExCmdLaneB ? regValRtB : regValRtA;
assign	tRegValRn		= tExCmdLaneB ? regValRnB : regValRnA;

`else

assign	tExCmdLaneB = 0;
	
assign	tOpCmd			= opCmdA;
assign	tRegIdIxt		= regIdIxtA;
assign	tRegValRMode	= regRModeA;

assign	tRegIdRs		= regIdRsA;
assign	tRegIdRt		= regIdRtA;
assign	tRegIdRn		= regIdRnA;
assign	tRegValRs		= regValRsA;
assign	tRegValRt		= regValRtA;
assign	tRegValRn		= regValRnA;

`endif

`ifndef def_true
`wire_gpr		tRegIdRsH;
`wire_gpr		tRegIdRtH;
`wire_gpr		tRegIdRnH;
wire[63:0]		tRegValRsH;			//Rs input value (FPR)
wire[63:0]		tRegValRtH;			//Rt input value (FPR)
wire[63:0]		tRegValRnH;			//Rn input value (FPR, Duplicate)

wire[63:0]		tRegValRsBH;		//Rs input value (FPR)
wire[63:0]		tRegValRtBH;		//Rt input value (FPR)

assign		tRegIdRsH = exHold ? tRegIdRsL : tRegIdRs;
assign		tRegIdRtH = exHold ? tRegIdRtL : tRegIdRt;
assign		tRegIdRnH = exHold ? tRegIdRnL : tRegIdRn;
assign		tRegValRsH = exHold ? tRegValRsL : tRegValRs;
assign		tRegValRtH = exHold ? tRegValRtL : tRegValRt;
assign		tRegValRnH = exHold ? tRegValRnL : tRegValRn;

assign		tRegValRsBH = exHold ? tRegValRsBL : tRegValRsB;
assign		tRegValRtBH = exHold ? tRegValRtBL : tRegValRtB;
`endif

reg			tValSgX;
reg[11:0]	tValRcpBias;
reg[12:0]	tValRcpBiasB;


always @*
begin
//	tRegOutVal		= UV64_XX;
//	tRegOutId		= JX2_GR_ZZR;
	tRegOutOK		= UMEM_OK_READY;
	tRegValGRn		= UV64_XX;
	tRegValGRnA		= UV64_XX;
	tRegValGRnB		= UV64_XX;
//	tRegValGRn		= UV64_00;
//	tRegValGRnA		= UV64_00;
//	tRegValGRnB		= UV64_00;
	tExHold			= 0;
	tExValidCmd		= 0;
	tExCmdVecW		= 0;
	tRegOutSrT		= tRegInSr[0];
	tMulFlipSign	= 0;

	tNxtVecRnA		= tVecRnA;
	tNxtVecRnB		= tVecRnB;
	tNxtVechRn		= tVechRn;
	tNxtVecdRnA		= tVecdRnA;
	tNxtVecdRnB		= tVecdRnB;
	tDoHoldCyc		= 0;
	tDoHoldDiv		= 0;
	
	tRegValRcp		= tRegValRcpL;
	tRegDifRcp		= tRegDifRcpL;
//	tRegTgtRcp		= tRegTgtRcpL;
	tRegTgtRcp		= tRegValRsL;

	tRegValRcpS		= UV64_XX;
	tRegValRcpDoS	= 0;

	tRegOutFpsr		= regInFpsr;

`ifndef def_true
	casez( { tBraFlushL || reset, tOpCmdL[7:6] } )
		3'b000: 	tOpEnable = 1;
		3'b001: 	tOpEnable = 0;
		3'b010: 	tOpEnable = tRegInSr[0];
		3'b011: 	tOpEnable = !tRegInSr[0];
		3'b1zz:		tOpEnable = 0;
	endcase
`endif

`ifdef def_true
//	casez( { opBraFlush, opUCmd[7:6], regInSr[0] } )
//	casez( { tBraFlushL || reset, tOpCmdL[7:6], regInSr[0] } )
	casez( { tBraFlushL || reset, tOpCmdL[7:6], tRegInSrL[0] } )
		4'b000z: 	tOpEnable = 1;
		4'b001z: 	tOpEnable = 0;
		4'b0100: 	tOpEnable = 0;
		4'b0101: 	tOpEnable = 1;
		4'b0110: 	tOpEnable = 1;
		4'b0111: 	tOpEnable = 0;
		4'b1zzz: 	tOpEnable = 0;
	endcase
`endif

	tOpUCmd1	= tOpEnable ? tOpCmdL[5:0] : JX2_UCMD_NOP;

// `ifdef def_true
`ifndef def_true
	tRegAddRs	= tRegValRs;
	tRegAddRt	= tRegValRt;
	tRegAddRsHi	= tRegValRsB;
	tRegAddRtHi	= tRegValRtB;

	tRegMulRs	= tRegValRs;
	tRegMulRt	= tRegValRt;
	tRegMulRsHi	= tRegValRsB;
	tRegMulRtHi	= tRegValRtB;
`endif

`ifndef def_true
	tRegAddRs	= tRegValRsH;
	tRegAddRt	= tRegValRtH;
	tRegAddRsHi	= tRegValRsBH;
	tRegAddRtHi	= tRegValRtBH;

	tRegMulRs	= tRegValRsH;
	tRegMulRt	= tRegValRtH;
	tRegMulRsHi	= tRegValRsBH;
	tRegMulRtHi	= tRegValRtBH;
`endif

// `ifdef def_true
`ifndef def_true
//	if(exHold)
//	if(tHoldCyc!=0)
	begin
		tRegAddRs	= tRegValRsL;
		tRegAddRt	= tRegValRtL;
		tRegAddRsHi	= tRegValRsBL;
		tRegAddRtHi	= tRegValRtBL;

		tRegMulRs	= tRegValRsL;
		tRegMulRt	= tRegValRtL;
		tRegMulRsHi	= tRegValRsBL;
		tRegMulRtHi	= tRegValRtBL;
	end
`endif


// `ifndef def_true
`ifdef def_true
	tRegAddRs	= tRegValRsL;
	tRegAddRt	= tRegValRtL;
	tRegAddRsHi	= tRegValRsBL;
	tRegAddRtHi	= tRegValRtBL;

	tRegMulRs	= tRegValRsL;
	tRegMulRt	= tRegValRtL;
	tRegMulRsHi	= tRegValRsBL;
	tRegMulRtHi	= tRegValRtBL;
`endif

	tRegAddRMode	= tRegValRModeL;
	tRegMulRMode	= tRegValRModeL;

//	regInFpsr

	if(tOpUCmd1==JX2_UCMD_FPU3)
	begin
		if(tRegIdIxtL[5:2]==4'b0100)
		begin
			tRegAddRMode	= regInFpsr[7:0];
			tRegMulRMode	= regInFpsr[7:0];
			
			if(tRegAddExOK[0] && !tRegIdIxtL[1])
				tRegOutFpsr[8] = 1;
			if(tRegMulExOK[0] && tRegIdIxtL[1])
				tRegOutFpsr[8] = 1;
		end
	end

//	tAddExHold	= exHold;
	tRegAddIsLD	= 0;

`ifndef def_true
	tVecCnvRsI	= tRegValRsA[31:0];
	tVecCnvRtI	= tRegValRtA[31:0];
	tVecCnvRnI	= tRegAddVal;

	tVechCnvRsI	= tRegValRsA[15:0];
	tVechCnvRtI	= tRegValRtA[15:0];
//	tVechCnvRnI	= tRegAddVal;

	tVecdCnvRs	= tRegValRsA[63:0];
	tVecdCnvRt	= tRegValRtA[63:0];
//	tVecCnvRnI	= tRegAddVal;
`endif

`ifdef def_true
	tVecCnvRsI	= tRegValRsAL[31:0];
	tVecCnvRtI	= tRegValRtAL[31:0];
	tVecCnvRnI	= tRegAddVal;

	tVechCnvRsI	= tRegValRsAL[15:0];
	tVechCnvRtI	= tRegValRtAL[15:0];
//	tVechCnvRnI	= tRegAddVal;

	tVecdCnvRs	= tRegValRsAL[63:0];
	tVecdCnvRt	= tRegValRtAL[63:0];
//	tVecCnvRnI	= tRegAddVal;
`endif

	case(tHoldCyc)
		0: begin
		end
	
		1: begin
				tVecCnvRsI	= tRegValRsL[31:0];
				tVecCnvRtI	= tRegValRtL[31:0];
				tVechCnvRsI = tRegValRsL[15:0];
				tVechCnvRtI = tRegValRtL[15:0];
				tVecdCnvRs	= tRegValRsL[63:0];
				tVecdCnvRt	= tRegValRtL[63:0];
		end
		2: begin
				tVecCnvRsI = tRegValRsL[63:32];
				tVecCnvRtI = tRegValRtL[63:32];
				tVechCnvRsI = tRegValRsL[31:16];
				tVechCnvRtI = tRegValRtL[31:16];
				tVecdCnvRs	= tRegValRsBL[63:0];
				tVecdCnvRt	= tRegValRtBL[63:0];
		end
		3: begin
				tVecCnvRsI = tRegValRsBL[31:0];
				tVecCnvRtI = tRegValRtBL[31:0];
				tVechCnvRsI = tRegValRsL[47:32];
				tVechCnvRtI = tRegValRtL[47:32];
		end
		4: begin
				tVecCnvRsI = tRegValRsBL[63:32];
				tVecCnvRtI = tRegValRtBL[63:32];
				tVechCnvRsI = tRegValRsL[63:48];
				tVechCnvRtI = tRegValRtL[63:48];
		end
		
		5: begin
		end

		6: begin
			tNxtVecRnA [31:0] = tVecCnvRnO;
			tNxtVechRn [15:0] = tVechCnvRnO;
			tNxtVecdRnA[63:0] = tVecCnvRnI;
		end
		7: begin
			tNxtVecRnA [63:32] = tVecCnvRnO;
			tNxtVechRn [31:16] = tVechCnvRnO;
			tNxtVecdRnB[63: 0] = tVecCnvRnI;
		end
		8: begin
			tNxtVecRnB[31: 0] = tVecCnvRnO;
			tNxtVechRn[47:32] = tVechCnvRnO;
		end
		9: begin
			tNxtVecRnB[63:32] = tVecCnvRnO;
			tNxtVechRn[63:48] = tVechCnvRnO;
		end
		
		default: begin
		end
	endcase

`ifdef jx2_fpu_enable_fdiv
	tRegDifRcpA	= tRegTgtRcpL - tRegMulValL;

//	tRegDifRcp	= {
//		tRegDifRcpA[63], tRegDifRcpA[63], 
//		tRegDifRcpA[63], tRegDifRcpA[63], 
//		tRegDifRcpA[63:4] };

//	tRegDifRcp	= {
//		tRegDifRcpA[63], tRegDifRcpA[63], 
//		tRegDifRcpA[63], tRegDifRcpA[63:4],
//		!tRegDifRcpA[63] ^ 
//			(tRegDifRcpA[3:1]==(tRegDifRcpA[63]?3'b111:3'b000)) };
//		!tRegDifRcpA[63] };

	tRegDifRcp	= {
		tRegDifRcpA[63], tRegDifRcpA[63], 
		tRegDifRcpA[63], tRegDifRcpA[63], 
		tRegDifRcpA[63:5],
		!tRegDifRcpA[63] };

//	tRegDifRcp	= {
//		tRegDifRcpA[63], tRegDifRcpA[63], 
//		tRegDifRcpA[63], tRegDifRcpA[63], 
//		tRegDifRcpA[63], tRegDifRcpA[63:5] };

	tRegValRcp	= tRegValRcpL + tRegDifRcpL;
//	tRegValRcp	= tRegValRcpL6 + tRegDifRcpL;
//	tRegValRcp	= tRegValRcpL2 + tRegDifRcpL;

	if(tRegValRcpDoSL)
		tRegValRcp	= tRegValRcpSL;

	tDivHold	= (tRegDifRcpL[62:0] != 0) &&
				(tDoHoldDivL || (tHoldCyc>10));
				
`endif

	case(tOpUCmd1)
		JX2_UCMD_FPU3: begin
			tExValidCmd	= 1;

			case(tRegIdIxtL[3:0])
				4'h0: begin
//					tDoHoldCyc	= 5;
					tDoHoldCyc	= 6;
					tRegValGRn	= tRegAddVal;
`ifdef jx2_fpu_longdbl
					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnA	= tRegAddVal;
					tRegValGRnB	= tRegAddValHi;
`endif
				end

				4'h1: begin
//					tDoHoldCyc	= 5;
					tDoHoldCyc	= 6;
					tRegValGRn	= tRegAddVal;
`ifdef jx2_fpu_longdbl
					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnA	= tRegAddVal;
					tRegValGRnB	= tRegAddValHi;
`endif
				end

				4'h2: begin
//					tDoHoldCyc	= 5;
					tDoHoldCyc	= 6;
					tRegValGRn	= tRegMulVal;
`ifdef jx2_fpu_longdbl
					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnA	= tRegMulVal;
					tRegValGRnB	= tRegMulValHi;
`endif
				end

`ifdef jx2_fpu_enable_fdiv
				4'h3: begin
					tRegMulRs	= tRegValRcpL;
					tRegMulRt	= tRegValRtL;
					tRegTgtRcp	= tRegValRsL;

//					if(!tDoHoldDivL)
//						tDoHoldCyc	= 14;
//					tDoHoldCyc	= 140;
//					tDoHoldCyc	= 240;
//					tDoHoldCyc	= 280;
//					tDoHoldCyc	= 320;
					tDoHoldCyc	= 384;
//					tDoHoldCyc	= 420;
					
//					tValRcpBias = 0;
//					tValRcpBias = 12'hE7F;
					tValRcpBias = 12'hF5D;
//					tValRcpBiasB = 13'h0E7F;
					tValRcpBiasB = 13'h0F5D;
//					tValRcpBiasB = 13'h0F3D;
//					tValRcpBiasB = 13'h0EEE;

//					if(!tDoHoldDivL)
					if(tHoldCyc < 8)
					begin
						tValSgX = tRegValRsL[63] ^ tRegValRtL[63];
					
//						$display("Rs=%X Rt=%X SgX=%X",
//							tRegValRsL, tRegValRtL, tValSgX);

						tRegValRcpS	= { tValSgX, 
							tRegValRsL[62:40]-
							tRegValRtL[62:40]+
//							23'h3FF000, 40'h0000 };
//							{ 11'h3FE, tValRcpBias }, 40'h0000 };
							{ 10'h1FF, tValRcpBiasB }, 40'h0000 };

//						$display("Rcp=%X", tRegValRcpS);

						tRegValRcpDoS	= 1;
					end
					
//					$display("Rcp Diff=%X Rcp=%X Cyc=%d",
//						tRegDifRcp, tRegValRcp, tHoldCyc);

//					if(tRegDifRcp[52:6]==0)
//					begin
//						$display("Rcp L2=%X L3=%X L4=%X L5=%X",
//							tRegValRcpL2[15:0], tRegValRcpL3[15:0],
//							tRegValRcpL4[15:0], tRegValRcpL5[15:0]);
//					end

//					tDoHoldDiv	= tDivHold;
//					tRegValGRn	= tRegValRcpL;
					tRegValGRn	= tRegValRcpL2;
//					tRegValGRn	= tRegValRcpL6;

					if(tRegIdIxtL[4])
					begin
						/* FDIVA */
						tDoHoldCyc	= 3;
					end

`ifndef def_true
					if(tRegValRsL[62:52]==11'h7FF)
					begin
						/* Inf or NaN */
						tRegValGRn	= tRegValRsL;
						tDoHoldCyc	= 2;
					end else
						if(tRegValRtL[62:52]==11'h7FF)
					begin
						/* Inf or NaN */
						tRegValGRn	= tRegValRtL;
						tDoHoldCyc	= 2;
					end else
						if(tRegValRtL[62:52]==11'h000)
					begin
						/* Divide by Zero */
						tRegValGRn	= { tValSgX, 11'h7FF, 52'h00 };
						tDoHoldCyc	= 2;
					end
`endif

				end
`endif

				4'h4: begin
					tRegValGRn	= tRegValRs;
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;
				end

				4'h5: begin
					tRegAddRs	= tVecCnvRsO;
					tRegAddRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegAddVal;
					tDoHoldCyc = 10;

					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnB	= tVecRnB;
					tRegValGRnA	= tVecRnA;
					tRegValGRn	= tVecRnA;
					
					if(tRegIdIxtL[4])
					begin
						if(tRegIdIxtL[5])
						begin
							tRegAddRs	= tVecdCnvRs;
							tRegAddRt	= tVecdCnvRt;
							tRegValGRnB	= tVecdRnB;
							tRegValGRnA	= tVecdRnA;
							tRegValGRn	= tVecdRnA;
							tDoHoldCyc	= 8;
						end
						else
						begin
							tRegAddRs	= tVechCnvRsO;
							tRegAddRt	= tVechCnvRtO;
							tRegValGRnA	= tVechRn;
							tRegValGRnB	= tVechRn;
							tRegValGRn	= tVechRn;
						end
					end
					else
					begin
						if(!tExCmdVecW)
							tDoHoldCyc	= 8;
					end
				end
				4'h6: begin
					tRegAddRs	= tVecCnvRsO;
					tRegAddRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegAddVal;

					tDoHoldCyc = 10;

					tExCmdVecW = tRegIdIxtL[5];
					tRegValGRnB = tVecRnB;
					tRegValGRnA = tVecRnA;
					tRegValGRn	= tVecRnA;

					if(tRegIdIxtL[4])
					begin
						if(tRegIdIxtL[5])
						begin
							tRegAddRs	= tVecdCnvRs;
							tRegAddRt	= tVecdCnvRt;
							tRegValGRnB	= tVecdRnB;
							tRegValGRnA	= tVecdRnA;
							tRegValGRn	= tVecdRnA;
							tDoHoldCyc	= 8;
						end
						else
						begin
							tRegAddRs	= tVechCnvRsO;
							tRegAddRt	= tVechCnvRtO;
							tRegValGRnA	= tVechRn;
							tRegValGRnB	= tVechRn;
							tRegValGRn	= tVechRn;
						end
					end
					else
					begin
						if(!tExCmdVecW)
							tDoHoldCyc	= 8;
					end
				end
				4'h7: begin
//					tRegAddRs	= tVecCnvRsO;
//					tRegAddRt	= tVecCnvRtO;
					tRegMulRs	= tVecCnvRsO;
					tRegMulRt	= tVecCnvRtO;
					tVecCnvRnI	= tRegMulValL;

					tDoHoldCyc = 10;

					tExCmdVecW	= tRegIdIxtL[5];
					tRegValGRnB	= tVecRnB;
					tRegValGRnA	= tVecRnA;
					tRegValGRn	= tVecRnA;

					if(tRegIdIxtL[4])
					begin
						if(tRegIdIxtL[5])
						begin
//							tRegAddRs	= tVecdCnvRs;
//							tRegAddRt	= tVecdCnvRt;
							tRegMulRs	= tVecdCnvRs;
							tRegMulRt	= tVecdCnvRt;
							tRegValGRnB	= tVecdRnB;
							tRegValGRnA	= tVecdRnA;
							tRegValGRn	= tVecdRnA;
							tDoHoldCyc	= 8;
						end
						else
						begin
//							tRegAddRs	= tVechCnvRsO;
//							tRegAddRt	= tVechCnvRtO;
							tRegMulRs	= tVechCnvRsO;
							tRegMulRt	= tVechCnvRtO;
							tRegValGRnA	= tVechRn;
							tRegValGRnB	= tVechRn;
							tRegValGRn	= tVechRn;
						end
					end
					else
					begin
						if(!tExCmdVecW)
							tDoHoldCyc	= 8;
					end
				end

`ifdef jx2_fpu_fmac
				4'h9: begin
					tDoHoldCyc	= 10;
					tExCmdVecW	= tRegIdIxtL[5];

					tRegAddRs	= tRegValRnL;
					tRegAddRsHi	= tRegValRnBL;
					tRegAddRt	= tRegMulVal;
					tRegAddRtHi	= tRegMulValHi;

					tRegValGRn	= tRegAddVal;
					tRegValGRnA	= tRegAddVal;
					tRegValGRnB	= tRegAddValHi;
				end
`endif


`ifdef jx2_fpu_enable_fdiv
				4'hF: begin
					tRegMulRs	= tRegValRcpL;
					tRegMulRt	= tRegValRcpL;
//					tRegTgtRcp	= tRegValRsL;
					tRegTgtRcp	= { 1'b0, tRegValRsL[62:0] };

//					if(!tDoHoldDivL)
//						tDoHoldCyc	= 14;

//					tDoHoldCyc	= 140;
//					tDoHoldCyc	= 254;
//					tDoHoldCyc	= 280;
//					tDoHoldCyc	= 320;
					tDoHoldCyc	= 384;

//					if(!tDoHoldDivL)
//					if(tHoldCyc < tDoHoldCyc)
					if(tHoldCyc < 8)
					begin
//						tRegValRcp	= { tRegValRsL[63], 
//							63'h2000_0000_0000_0000 + tRegValRsL[63:1] };
//						tRegValRcp	= { tRegValRsL[63], 
//							31'h2000_0000 + tRegValRsL[63:33], 32'h00 };
//						tRegValRcpS	= { tRegValRsL[63], 
						tRegValRcpS	= { 1'b0, 
							7'h20 + { 1'b0, tRegValRsL[62:57] },
							tRegValRsL[56:41], 40'h00 };
						tRegValRcpDoS	= 1;

//						$display("Sqrt %X=%X", tRegValRsL, tRegValRcpS);
					end
					
//					$display("Sqrt Diff=%X", tRegDifRcp);

					if(tRegIdIxtL[4])
					begin
						/* FDIVA */
						tDoHoldCyc	= 3;
					end

//					tDoHoldDiv	= tDivHold;
//					tRegValGRn	= tRegValRcpL;
//					tRegValGRn	= tRegValRcpL2;
					tRegValGRn	= { tRegValRsL[63], tRegValRcpL2[62:0] };
//					tRegValGRn	= tRegValRcpL6;
				end
`endif
	
				default: begin
				end
			endcase
		end

		JX2_UCMD_FLDCX: begin
			tExValidCmd	= 1;

			case(tRegIdIxtL[2:0])
				3'h0: begin
					tRegValGRn	= ctlInDlr_S2D_L;
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;
				end
				3'h1: begin
					tRegValGRn	= tRegValRs;
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;

`ifdef jx2_fpu_longdbl
					if(tRegIdIxtL[5])
					begin
//						tDoHoldCyc = 5;
						tDoHoldCyc = 6;
						tExCmdVecW = 1;
						tRegValGRn	= tRegAddVal;
						tRegValGRnA	= tRegAddVal;
						tRegValGRnB	= tRegAddValHi;
					end
`endif
				end
				3'h2: begin
//					tDoHoldCyc = 5;
					tDoHoldCyc = 6;
					tRegValGRn	= tRegAddVal;
					tRegValGRnA	= tRegAddVal;
`ifdef jx2_fpu_longdbl
					tExCmdVecW = tRegIdIxtL[5];
					tRegValGRnB	= tRegAddValHi;
`endif
				end
				3'h3: begin
					tRegValGRn	= ctlInDlr_H2D_L;
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;
				end
				
				default: begin
//					tRegOutVal	= UV64_XX;
				end
			endcase
		end

		JX2_UCMD_FSTCX: begin
			tExValidCmd	= 1;

			case(tRegIdIxtL[3:0])
				4'h0: begin
					tRegValGRn	= { UV32_00, fstcx_D2S_L };
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;
				end

				4'h1: begin
					tRegValGRn	= tRegValRsL;
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;

`ifdef jx2_fpu_longdbl
					if(tRegIdIxtL[5])
					begin
//						tDoHoldCyc = 5;
						tDoHoldCyc = 6;
						tExCmdVecW = 0;
						tRegValGRn	= tRegAddVal;
						tRegValGRnA	= tRegAddVal;
						tRegValGRnB	= tRegAddVal;
					end
`endif
				end

				4'h2: begin
//					tDoHoldCyc = 5;
					tDoHoldCyc = 6;
					tRegValGRn	= tRegAddVal;
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;
				end

				4'h3: begin
					tRegValGRn	= { UV48_00, fstcx_D2H_L };
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;
				end

				4'h8: begin
					tRegValGRn	= { fstcx_D2S, tRegValRsL[31:0] };
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;
				end
				
				default: begin
					tRegValGRn	= UV64_XX;
//					tRegValGRn	= UV64_00;
					tRegValGRnA	= tRegValGRn;
					tRegValGRnB	= tRegValGRn;
				end
			endcase
		end

		default: begin
		end
	endcase

// `ifndef def_true
`ifdef def_true
	if(!tExCmdVecW)
	begin
		tRegValGRnB = tRegValGRn;
		tRegValGRnA = tRegValGRn;
	end
`endif

	if(tHoldCyc < tDoHoldCyc)
		tExHold = 1;
	
	if(tDoHoldDiv)
		tExHold = 1;

	if(reset)
		tExHold = 0;

//	if(tExHold)
//		$display("FPU Hold %d", tHoldCyc);

	tRegOutOK	= tExHold ? UMEM_OK_HOLD :
		(tExValidCmd ? UMEM_OK_OK : UMEM_OK_READY);

end

reg[1:0]	tLastRegMulExOK;

always @(posedge clock)
begin
	if(!exHold)
	begin
//		tRegOutVal2		<= tRegOutVal;
//		tRegOutId2		<= tRegOutId;
		tRegOutOK2		<= tRegOutOK;
		tRegOutSrT2		<= tRegOutSrT;
		tRegValGRn2		<= tRegValGRn;
//		tRegValLdGRn2	<= tRegValLdGRn;

		tRegAddExOpL	<= tRegAddExOp;

`ifdef def_true
		tOpCmdL			<= tOpCmd;
		tRegIdIxtL		<= tRegIdIxt;
		tExCmdLaneBL	<= tExCmdLaneB;

		tRegIdRsL		<= tRegIdRs;
		tRegIdRtL		<= tRegIdRt;
		tRegIdRnL		<= tRegIdRn;
		tRegValRsL		<= tRegValRs;
		tRegValRtL		<= tRegValRt;
		tRegValRnL		<= tRegValRn;
		tRegValRModeL	<= tRegValRMode;

		tRegIdRsAL		<= tRegIdRsA;
		tRegIdRtAL		<= tRegIdRtA;
		tRegIdRnAL		<= tRegIdRnA;
		tRegValRsAL		<= tRegValRsA;
		tRegValRtAL		<= tRegValRtA;
		tRegValRnAL		<= tRegValRnA;

		tRegIdRsBL		<= tRegIdRsB;
		tRegIdRtBL		<= tRegIdRtB;
		tRegIdRnBL		<= tRegIdRnB;
		tRegValRsBL		<= tRegValRsB;
		tRegValRtBL		<= tRegValRtB;
		tRegValRnBL		<= tRegValRnB;

		tRegInSrL		<= tRegInSr;
		tBraFlushL		<= tBraFlush;
`endif

		ctlInDlr_S2D_L	<= ctlInDlr_S2D;
		ctlInDlr_H2D_L	<= ctlInDlr_H2D;
		fstcx_D2S_L		<= fstcx_D2S;
		fstcx_D2H_L		<= fstcx_D2H;
//		fstcx_D2I_L		<= fstcx_D2I;
	end

	tVecRnA		<= tNxtVecRnA;
	tVecRnB		<= tNxtVecRnB;
	tVechRn		<= tNxtVechRn;
	tVecdRnA	<= tNxtVecdRnA;
	tVecdRnB	<= tNxtVecdRnB;
	tRegMulValL	<= tRegMulVal;


	tRegValRcpL		<= tRegValRcp;
	tRegValRcpL2	<= tRegValRcpL;
	tRegValRcpL3	<= tRegValRcpL2;
	tRegValRcpL4	<= tRegValRcpL3;
	tRegValRcpL5	<= tRegValRcpL4;
	tRegValRcpL6	<= tRegValRcpL5;
	tRegDifRcpL		<= tRegDifRcp;
	tRegTgtRcpL		<= tRegTgtRcp;
	tDoHoldDivL		<= tDoHoldDiv;

	tRegValRcpSL	<= tRegValRcpS;
	tRegValRcpDoSL	<= tRegValRcpDoS;

	if(tExHold)
		tHoldCyc <= tHoldCyc + 1;
	else if(exHold)
		tHoldCyc <= tHoldCyc;
	else
		tHoldCyc <= 0;
end

endmodule
