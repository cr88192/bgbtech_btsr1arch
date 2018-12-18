`include "Jx2CoreDefs.v"

`include "Jx2ExShad32.v"
`include "Jx2ExShad64.v"
`include "Jx2ExMul.v"

`include "Jx2ExOpAlu.v"
`include "Jx2ExOpCmp.v"

`include "Jx2ExSwapN.v"

`ifndef JX2_EX_MERGEAGU
`ifdef JX2_EX_AGU48
`include "Jx2ExCsAdd48F.v"
`else
`include "Jx2ExCsAdd64F.v"
`endif
`endif

`ifdef JX2_EX_GPRSIMD_HF
`include "Jx2FphfVecOp.v"
`endif

module Jx2ExOp(
	/* verilator lint_off UNUSED */
	clock,		reset,
	opCmdA,		opCmdB,		opCmdC,
	regIdIxt,	regIdIxtB,
	regIdRm,	regValRm,	regValRmB,	regValRmC,
	regIdRn,	regValRn,	regValRb,
	regIdRi,	regValRi,	regValRiB,	regValRiC,
	regIdCm,	regValCm,
	immValRi,	idInGenPc,
	regOutId,	regOutVal,
	regOutIdCn,	regOutValCn,
	regOutOK,

	regValFRm,	regValFRn,

	memAddr,	memData,
	memOpm,		memOpCmd2,
	memDataLd,	memDataOK,
	memOutId,

	ctlInSp,	ctlOutSp,
	ctlInPc,	ctlOutPc,
	ctlInPr,	ctlOutPr,
	ctlInSr,	ctlOutSr,
	ctlInVbr,	ctlOutVbr,
	ctlInDlr,	ctlOutDlr,
	ctlInDhr,	ctlOutDhr,
	ctlInGbr,	ctlOutGbr,
	ctlInTbr,	ctlOutTbr
	);

input			clock;
input			reset;

input[7:0]		opCmdA;			//command opcode
input[7:0]		opCmdB;			//command opcode (alternate)
input[7:0]		opCmdC;			//command opcode (alternate)

input[7:0]		regIdIxt;		//ALU Index / Opcode Extension
input[7:0]		regIdIxtB;		//ALU Index / Opcode Extension (alternate)


input[6:0]		regIdRm;
input[6:0]		regIdRn;
input[6:0]		regIdRi;
input[6:0]		regIdCm;

input[63:0]		regValRm;		//Rm input value
input[63:0]		regValRn;		//Rn input value
input[63:0]		regValRi;		//Ri input value

input[63:0]		regValRmB;		//Rm input value (alternate)
input[63:0]		regValRiB;		//Ri input value (alternate)
input[63:0]		regValRb;		//Rb input value (synthetic)

input[63:0]		regValRmC;		//Rm input value (alternate)
input[63:0]		regValRiC;		//Ri input value (alternate)

input[63:0]		regValCm;		//Cm input value
input[32:0]		immValRi;		//immediate/disp value
input[63:0]		idInGenPc;		//PC to next instruction

output[63:0]	regOutVal;		//Rn output value
output[6:0]		regOutId;		//Rn, register to write
output[1:0]		regOutOK;		//execute status

output[63:0]	regOutValCn;	//Rn output value
output[6:0]		regOutIdCn;		//Rn, value to write

input[63:0]		regValFRm;		//FRm input value
input[63:0]		regValFRn;		//FRn input value

/* Memory */
output[63:0]	memAddr;		//memory address
output[63:0]	memData;		//memory data (store)
output[4:0]		memOpm;			//mem op mode
output[7:0]		memOpCmd2;		//mem EX chain
output[6:0]		memOutId;		//Rn, register to write

input[63:0]		memDataLd;		//memory data (load)
input[1:0]		memDataOK;		//memory status (load)


/* Special Registers */
input[63:0]		ctlInSp;		//SP in
output[63:0]	ctlOutSp;		//SP out

input[63:0]		ctlInPc;		//PC in
output[63:0]	ctlOutPc;		//PC out

input[63:0]		ctlInPr;		//PR in
output[63:0]	ctlOutPr;		//PR out

input[63:0]		ctlInSr;		//SR in
output[63:0]	ctlOutSr;		//SR out

input[63:0]		ctlInVbr;
output[63:0]	ctlOutVbr;

input[63:0]		ctlInDlr;		//DLR in
output[63:0]	ctlOutDlr;		//DLR out

input[63:0]		ctlInDhr;		//DHR in
output[63:0]	ctlOutDhr;		//DHR out

input[63:0]		ctlInGbr;
output[63:0]	ctlOutGbr;

input[63:0]		ctlInTbr;
output[63:0]	ctlOutTbr;


reg[63:0]	tCtlOutSp;		//SP out
reg[63:0]	tCtlOutPc;		//PC out
reg[63:0]	tCtlOutPr;		//PR out
reg[63:0]	tCtlOutSr;		//SR out
reg[63:0]	tCtlOutVbr;
reg[63:0]	tCtlOutDlr;		//MACL out
reg[63:0]	tCtlOutDhr;		//MACH out
reg[63:0]	tCtlOutGbr;
reg[63:0]	tCtlOutTbr;

assign	ctlOutSp	= tCtlOutSp;
assign	ctlOutPc	= tCtlOutPc;
assign	ctlOutPr	= tCtlOutPr;
assign	ctlOutSr	= tCtlOutSr;
assign	ctlOutVbr	= tCtlOutVbr;
assign	ctlOutDlr	= tCtlOutDlr;
assign	ctlOutDhr	= tCtlOutDhr;
assign	ctlOutGbr	= tCtlOutGbr;
assign	ctlOutTbr	= tCtlOutTbr;

reg[63:0]	tRegOutVal;		//Rn output value
reg[6:0]	tRegOutId;		//Rn, value to write
reg[1:0]	tRegOutOK;		//execute status
reg[7:0]	tMemOpCmd2;		//mem EX chain
reg[6:0]	tMemOutId;		//memory load register (LSQ)

reg[63:0]	tRegOutValCn;	//Rn output value
reg[6:0]	tRegOutIdCn;	//Rn, value to write

assign	regOutVal	= tRegOutVal;
assign	regOutId	= tRegOutId;
assign	regOutOK	= tRegOutOK;
assign	memOpCmd2	= tMemOpCmd2;
assign	memOutId	= tMemOutId;

// reg[63:0]	tMemAddr2;		//memory address
// reg[63:0]	tMemData2;		//memory data (store)
// reg[4:0]	tMemOpm2;		//mem op mode

assign	memAddr		= tMemAddr;
assign	memData		= tMemData;
assign	memOpm		= tMemOpm;

reg[63:0]	tMemAddr;		//memory address
reg[63:0]	tMemData;		//memory data (store)
reg[4:0]	tMemOpm;		//mem op mode

assign	regOutValCn	= tRegOutValCn;
assign	regOutIdCn	= tRegOutIdCn;

reg[63:0]		memHoldDataLd;
reg[63:0]		memNextDataLd;


wire[31:0]	tShadOut;
wire[63:0]	tShadQOut;
// reg[2:0]	tShOp;

wire[2:0]	tShOp;
assign		tShOp = opCmdA[0] ? 3'h3 : 3'h2;

wire[63:0]	tShadValIn;
wire[7:0]	tShadValShl;
assign		tShadValIn = regValRm;
assign		tShadValShl = regValRi[7:0];

Jx2ExShad32		shad(
	clock, 				reset,
	tShadValIn[31:0],	tShadValShl,
	tShadOut,			tShOp);
Jx2ExShad64		shadq(
	clock, 				reset,
	tShadValIn[63:0],	tShadValShl,
	tShadQOut,			tShOp);

wire[63:0]		tSwapOutVal;
Jx2ExSwapN		swapn(
	clock,		reset,
	regIdIxt,
	regValRm,	tSwapOutVal);

reg[63:0]		regMaRbVal;
reg[63:0]		regMaIxVal;
reg[63:0]		regMaIxVal2;
reg[3:0]		regMaIxt;
reg[2:0]		regMaBty;
// reg[63:0]		regMaAddr;

`ifndef JX2_EX_MERGEAGU
wire[63:0]		regMaAddr;
`ifdef JX2_EX_AGU48
Jx2ExCsAdd48F		maAdd(regMaRbVal, regMaIxVal2, regMaAddr);
`else
Jx2ExCsAdd64F	maAdd(regMaRbVal, regMaIxVal2, regMaAddr);
`endif
`else
reg[63:0]		regMaAddr;
`endif

reg[63:0]		regLastMaAddr;

reg[63:0]		regMaIxVal3;

reg[63:0]		regBraAddr;


reg[63:0]		regRnSxt;
reg[63:0]		immValRiB;
reg[31:0]		immValRiA;
reg[63:0]		regDlrA;

reg[63:0]		regMulA;
reg[63:0]		regMulB;
reg[2:0]		regMulOp;
// reg				regMulIsPMul;

wire[63:0]		regMulDlr;
wire[63:0]		regMulDhr;
wire[2:0]		regMulOpDr;

Jx2ExMul	exMul(
	clock, reset,
	regMulA,		regMulB,
	regMulOp,	
//	regMulIsPMul,

	regMulDlr,		regMulDhr,
	regMulOpDr		);


reg[3:0]		regHoldFixedCnt;
reg				regHoldFixed;
reg				regRnIsGpr;

// reg[7:0]		opCmdA;
// reg[7:0]		opCmdB;
// reg[7:0]		opCmdC;

wire[31:0]		regValFRmSs;	//FRm input value (Single Precision)
// wire[31:0]		regValFRnSs;	//FRn input value (Single Precision)

Jx2ConvFpuD2S	fd2s_rm(regValFRm, regValFRmSs);
// Jx2ConvFpuD2S	fd2s_rn(regValFRn, regValFRnSs);

wire[15:0]		regValFRmSh;	//FRm input value (Single Precision)
// wire[15:0]		regValFRnSh;	//FRn input value (Single Precision)

Jx2ConvFpuD2H	fd2h_rm(regValFRm, regValFRmSh);
// Jx2ConvFpuD2H	fd2h_rn(regValFRn, regValFRnSh);

wire[63:0]		regValFRmSi;	//FRn input value (Int)
Jx2ConvFpuD2I	fd2i_rn(
	clock,		reset,
	regValFRm,	regValFRmSi);

reg		tRegValFRn_IsNaN;
reg		tRegValFRm_IsNaN;

`ifdef JX2_EX_GPRSIMD_HF
wire[63:0]		regFphfOutVal;
wire[1:0]		regFphfOutOK;
reg[1:0]		regFphfCmd;

//wire			tOpIsFphf;
//assign			tOpIsFphf =
//	(opCmdA == JX2_UCMD_GSV_PFPHF);

//wire[1:0]		regFphfCmd;
//assign		regFphfCmd = tOpIsFphf ? regIdIxtB[1:0] : 0;

Jx2FphfVecOp	fphfVec(
	clock,	reset,
	regValRmB,
	regValRiB,
	regFphfOutVal,
	regFphfCmd,
	regFphfOutOK
	);
`endif

wire[32:0]		tNextAddRmRi_A0;
wire[32:0]		tNextAddRmRi_A1;
wire[32:0]		tNextAddRmRi_B0;
wire[32:0]		tNextAddRmRi_B1;
wire[32:0]		tNextSubRmRi_A0;
wire[32:0]		tNextSubRmRi_A1;
wire[32:0]		tNextSubRmRi_B0;
wire[32:0]		tNextSubRmRi_B1;

wire[63:0]		tNextAluPAddW;
wire[63:0]		tNextAluPAddL;
wire[63:0]		tNextAluPSubW;
wire[63:0]		tNextAluPSubL;

Jx2ExOpAlu		exAlu1(
	clock, reset,

	regValRmC,		regValRn,
	regValRiC,

	tNextAddRmRi_A0,	tNextAddRmRi_A1,
	tNextAddRmRi_B0,	tNextAddRmRi_B1,
	tNextSubRmRi_A0,	tNextSubRmRi_A1,
	tNextSubRmRi_B0,	tNextSubRmRi_B1,

	tNextAluPAddW,	tNextAluPAddL,
	tNextAluPSubW,	tNextAluPSubL
	);

reg[32:0]		tAddRmRi_A0;
reg[32:0]		tAddRmRi_A1;
reg[32:0]		tAddRmRi_B0;
reg[32:0]		tAddRmRi_B1;
reg[32:0]		tSubRmRi_A0;
reg[32:0]		tSubRmRi_A1;
reg[32:0]		tSubRmRi_B0;
reg[32:0]		tSubRmRi_B1;

reg[64:0]		tAddRmRi0;
reg[64:0]		tAddRmRi1;
reg[64:0]		tSubRmRi0;
reg[64:0]		tSubRmRi1;
reg				aluIs2C;

reg[63:0]		tAluPAddW;
reg[63:0]		tAluPAddL;
reg[63:0]		tAluPSubW;
reg[63:0]		tAluPSubL;
reg				aluSimdIs2C;


// reg[63:0]		tCmpValRm;
// reg[63:0]		tCmpValRn;

wire			tCmpRmRnEqL;
wire			tCmpRmRnEqQa;
wire			tCmpRmRnHiL;
wire			tCmpRmRnHiQa;
wire			tCmpRmRnHsL;
wire			tCmpRmRnHsQa;
wire			tTstRmRnL;
wire			tTstRmRnQ;

wire[3:0]		tCmpRmRnEqPW;
wire[3:0]		tCmpRmRnHiPW;
wire[3:0]		tCmpRmRnHsPW;
wire[1:0]		tCmpRmRnEqPL;
wire[1:0]		tCmpRmRnHiPL;
wire[1:0]		tCmpRmRnHsPL;


Jx2ExOpCmp		exCmp(
	clock,			reset,
	regValRmC,		regValRn,
	tCmpRmRnEqL,	tCmpRmRnEqQa,
	tCmpRmRnHiL,	tCmpRmRnHiQa,
	tCmpRmRnHsL,	tCmpRmRnHsQa,
	tTstRmRnL,		tTstRmRnQ,

	tCmpRmRnEqPW,	tCmpRmRnEqPL,
	tCmpRmRnHiPW,	tCmpRmRnHiPL,
	tCmpRmRnHsPW,	tCmpRmRnHsPL
	);
	
/*
reg				tFCmpRmRnEq0;
reg				tFCmpRmRnEq1;
reg				tFCmpRmRnEq2;
reg				tFCmpRmRnEq3;

reg				tFCmpRmRnHi0;
reg				tFCmpRmRnHi1;
reg				tFCmpRmRnHi2;
reg				tFCmpRmRnHi3;

reg				tFCmpRmRnEq01;
reg				tFCmpRmRnEq23;
reg				tFCmpRmRnEqQ;

reg				tFCmpRmRnHi01;
reg				tFCmpRmRnHi23;
reg				tFCmpRmRnHiL;
reg				tFCmpRmRnHiQ;
*/

wire			tFCmpRmRnEqL;
wire			tFCmpRmRnEqQa;
wire			tFCmpRmRnHiL;
wire			tFCmpRmRnHiQa;
wire			tFCmpRmRnHsL;
wire			tFCmpRmRnHsQa;

wire			tFTstRmRnL;
wire			tFTstRmRnQ;

wire[3:0]		tFCmpRmRnEqPW;
wire[3:0]		tFCmpRmRnHiPW;
wire[3:0]		tFCmpRmRnHsPW;
wire[1:0]		tFCmpRmRnEqPL;
wire[1:0]		tFCmpRmRnHiPL;
wire[1:0]		tFCmpRmRnHsPL;

Jx2ExOpCmp		exFCmp(
	clock,			reset,
	regValFRm,		regValFRn,
	tFCmpRmRnEqL,	tFCmpRmRnEqQa,
	tFCmpRmRnHiL,	tFCmpRmRnHiQa,
	tFCmpRmRnHsL,	tFCmpRmRnHsQa,
	tFTstRmRnL,		tFTstRmRnQ,

	tFCmpRmRnEqPW,	tFCmpRmRnEqPL,
	tFCmpRmRnHiPW,	tFCmpRmRnHiPL,
	tFCmpRmRnHsPW,	tFCmpRmRnHsPL
	);


reg[3:0]		tLastCmpRmRnEqPW;
reg[3:0]		tLastCmpRmRnHiPW;
reg[3:0]		tLastCmpRmRnHsPW;
reg[1:0]		tLastCmpRmRnEqPL;
reg[1:0]		tLastCmpRmRnHiPL;
reg[1:0]		tLastCmpRmRnHsPL;

wire			tCmpRmRnEqQ;
wire			tCmpRmRnHiQ;
wire			tCmpRmRnHsQ;

assign	tCmpRmRnEqQ =
		tLastCmpRmRnEqPL[0] &&
		tLastCmpRmRnEqPL[1];

assign		tCmpRmRnHiQ =
		tLastCmpRmRnHiPW[3] ||
		(tLastCmpRmRnEqPW[3] && tLastCmpRmRnHiPW[2]) ||
		(tLastCmpRmRnEqPW[3] && tLastCmpRmRnEqPW[2] &&
			tLastCmpRmRnHiPL[0]);
assign		tCmpRmRnHsQ =
		tLastCmpRmRnHiPW[3] ||
		(tLastCmpRmRnEqPW[3] && tLastCmpRmRnHsPW[2]) ||
		(tLastCmpRmRnEqPW[3] && tLastCmpRmRnEqPW[2] &&
			tLastCmpRmRnHsPL[0]);



reg[3:0]		tLastFCmpRmRnEqPW;
reg[3:0]		tLastFCmpRmRnHiPW;
reg[3:0]		tLastFCmpRmRnHsPW;
reg[1:0]		tLastFCmpRmRnEqPL;
reg[1:0]		tLastFCmpRmRnHiPL;
reg[1:0]		tLastFCmpRmRnHsPL;

wire			tFCmpRmRnEqQ;
wire			tFCmpRmRnHiQ;
wire			tFCmpRmRnHsQ;

assign	tFCmpRmRnEqQ =
		tLastFCmpRmRnEqPL[0] &&
		tLastFCmpRmRnEqPL[1];

assign		tFCmpRmRnHiQ =
		tLastFCmpRmRnHiPW[3] ||
		(tLastFCmpRmRnEqPW[3] && tLastFCmpRmRnHiPW[2]) ||
		(tLastFCmpRmRnEqPW[3] && tLastFCmpRmRnEqPW[2] &&
			tLastFCmpRmRnHiPL[0]);
assign		tFCmpRmRnHsQ =
		tLastFCmpRmRnHiPW[3] ||
		(tLastFCmpRmRnEqPW[3] && tLastFCmpRmRnHsPW[2]) ||
		(tLastFCmpRmRnEqPW[3] && tLastFCmpRmRnEqPW[2] &&
			tLastFCmpRmRnHsPL[0]);


reg[63:0]		tRegValSpP8;
reg[63:0]		tRegValSpN8;

reg				tRegOpSticky1;
reg				tRegOpSticky2;
reg				tRegOpSticky3;
reg				tNextOpSticky1;
reg				tNextOpSticky2;
reg				tNextOpSticky3;

reg				tOpSticky1;
reg				tOpSticky2;
reg				tOpSticky3;

reg[63:0]		idLastGenPc;
reg				isNewPc;
reg				tOpDoMovX;			//Do shared MOV.X mechanics.
reg				tOpDoMovX_MR;		//Do shared MOV.X mechanics.
reg				tOpDoMovX_Inh;		//Inhibit (MOVcc Fail)
reg				tOpDoAri2C;

reg				tRegOkLatch;
reg				tNextOkLatch;
reg				tJointOkLatch;

reg				tJcmpStatus;

reg				tOpMiss1;
reg				tOpMiss2;
reg				tOpMiss3;

reg[3:0]		tOpState;
reg[3:0]		tNextOpState;

reg[63:0]		tCpuidDlr0;
reg[63:0]		tCpuidDhr0;

reg[63:0]		tCpuidDlr[15:0];
reg[63:0]		tCpuidDhr[15:0];

initial begin
	tCpuidDlr0[31: 0] = 32'h32584A42;
	tCpuidDlr0[63:32] = 32'h31304C56;
	tCpuidDhr0[31: 0] = UV32_00;
	tCpuidDhr0[63:32] = UV32_00;

`ifndef JX2_MEM_NOMMU
	tCpuidDhr0[0] = 1;		//Has MMU
`endif
`ifndef JX2_FPU_NOFPU
	tCpuidDhr0[1] = 1;		//Has FPU
`endif
`ifdef JX2_EX_GPRSIMD
	tCpuidDhr0[2] = 1;		//Has GSV
`endif
`ifdef JX2_EX_GPRSIMD_HF
	tCpuidDhr0[3] = 1;		//GSV Has Half-Float
`endif
`ifdef JX2_EX_ALU_ADDSL
	tCpuidDhr0[4] = 1;		//Has ADDx.L
`endif
`ifdef JX2_EX_ALU_JMPCC
	tCpuidDhr0[5] = 1;		//Has JMPcc
`endif
`ifndef JX2_FPU_NOFDIV
	tCpuidDhr0[6] = 1;		//Has FDIV
`endif
`ifdef JX2_EX_GPRSIMD_F
	tCpuidDhr0[7] = 1;		//GSV Has Float32
`endif

	tCpuidDlr[ 0]	= tCpuidDlr0;	tCpuidDhr[ 0]	= tCpuidDhr0;
	tCpuidDlr[ 1]	= UV64_00;		tCpuidDhr[ 1]	= UV64_00;
	tCpuidDlr[ 2]	= UV64_00;		tCpuidDhr[ 2]	= UV64_00;
	tCpuidDlr[ 3]	= UV64_00;		tCpuidDhr[ 3]	= UV64_00;
	tCpuidDlr[ 4]	= UV64_00;		tCpuidDhr[ 4]	= UV64_00;
	tCpuidDlr[ 5]	= UV64_00;		tCpuidDhr[ 5]	= UV64_00;
	tCpuidDlr[ 6]	= UV64_00;		tCpuidDhr[ 6]	= UV64_00;
	tCpuidDlr[ 7]	= UV64_00;		tCpuidDhr[ 7]	= UV64_00;
	tCpuidDlr[ 8]	= UV64_00;		tCpuidDhr[ 8]	= UV64_00;
	tCpuidDlr[ 9]	= UV64_00;		tCpuidDhr[ 9]	= UV64_00;
	tCpuidDlr[10]	= UV64_00;		tCpuidDhr[10]	= UV64_00;
	tCpuidDlr[11]	= UV64_00;		tCpuidDhr[11]	= UV64_00;
	tCpuidDlr[12]	= UV64_00;		tCpuidDhr[12]	= UV64_00;
	tCpuidDlr[13]	= UV64_00;		tCpuidDhr[13]	= UV64_00;
	tCpuidDlr[14]	= UV64_00;		tCpuidDhr[14]	= UV64_00;
	tCpuidDlr[15]	= UV64_00;		tCpuidDhr[15]	= UV64_00;
end

always @*
begin
	tCtlOutSp	= ctlInSp;
	tCtlOutPc	= ctlInPc;
	tCtlOutPr	= ctlInPr;
	tCtlOutSr	= ctlInSr;
	tCtlOutVbr	= ctlInVbr;
	tCtlOutDlr	= ctlInDlr;
	tCtlOutDhr	= ctlInDhr;
	tCtlOutGbr	= ctlInGbr;
	tCtlOutTbr	= ctlInTbr;

	tRegOutVal	= UV64_XX;
	tRegOutId	= JX2_REG_ZZR;
	tRegOutOK	= UMEM_OK_OK;
//	tMemAddr	= 0;
	tMemData	= UV64_XX;
	tMemOpm		= UMEM_OPM_READY;
	tMemOpCmd2	= JX2_UCMD_NOP;

	tRegOutValCn	= UV64_XX;
	tRegOutIdCn		= JX2_REG_ZZR;

//	opCmdA		= opCmd;
//	opCmdB		= opCmd;
//	opCmdC		= opCmdA;

	tOpMiss1		= 0;
	tOpMiss2		= 0;
	tOpMiss3		= 0;
	tNextOpState	= 0;

	regRnSxt	= regValRn[63] ? UV64_FF : UV64_00;
	immValRiB	= { immValRi[32] ? UV32_FF : UV32_00, immValRi[31:0]};

//	regMulA			= UV32_XX;
//	regMulB			= UV32_XX;
//	regMulC			= UV32_XX;
//	regMulD			= UV32_XX;
	regMulA			= UV64_XX;
	regMulB			= UV64_XX;
	regMulOp		= 0;
//	regMulIsPMul	= 0;
//	tShOp			= 0;
	
	tNextOpSticky1	= 0;
	tNextOpSticky2	= 0;
	tNextOpSticky3	= 0;
	
	tJcmpStatus		= 1'bX;

	isNewPc =
		(idInGenPc[ 7:0] != idLastGenPc[ 7:0]);
//		(idInGenPc[ 7:0] != idLastGenPc[ 7:0]) ||
//		(idInGenPc[15:8] != idLastGenPc[15:8]);

	tOpSticky1	= tRegOpSticky1 && !isNewPc;
	tOpSticky2	= tRegOpSticky2 && !isNewPc;
	tOpSticky3	= tRegOpSticky3 && !isNewPc;


	tNextOkLatch	= (memDataOK!=UMEM_OK_OK);
	tJointOkLatch	= tNextOkLatch || tRegOkLatch;
	
	memNextDataLd	= memDataLd;
	if(tOpSticky2)
		memNextDataLd	= memHoldDataLd;
	
	tOpDoMovX		= 0;
	tOpDoMovX_MR	= 0;
	tOpDoMovX_Inh	= 0;
	tOpDoAri2C		= 0;
	
	if(regIdIxtB[7])
	begin
		tOpDoMovX_Inh	= !ctlInSr[0] ^ regIdIxtB[6];
	end
	
	immValRiA = immValRi[31:0];
	regDlrA = ctlInDlr;
//	regDlrI4 = {regDlrA[59:0], immValRiA[3:0]};

//	regMaRbVal = regIdIxt[3] ? regValRm : regValRn;
	regMaRbVal = regValRm;
//	regMaRbVal = regValRb;

	case(regIdIxtB[1:0])
		2'b00: regMaIxVal2=regValRiB;
		2'b01: regMaIxVal2={regValRiB[62:0], 1'b0};
		2'b10: regMaIxVal2={regValRiB[61:0], 2'b0};
//		2'b11: regMaIxVal2={regValRiB[60:0], 3'b0};
		2'b11: regMaIxVal2=regIdIxtB[2] ?
			{regValRiB[59:0], 4'b0} :
			{regValRiB[60:0], 3'b0};
	endcase

//	tMemAddr	= regMaAddr;

	regBraAddr	= { idInGenPc[63:32],
		idInGenPc[31:0]+immValRi[31:0] };

	tRegValFRn_IsNaN =
		(regValFRn[62:52]==11'h7FF) &&
//		(regValFRn[51: 0]!=0);
		(regValFRn[51:48]!=0);
	tRegValFRm_IsNaN =
		(regValFRm[62:52]==11'h7FF) &&
//		(regValFRm[51: 0]!=0);
		(regValFRm[51:48]!=0);
	

	tSubRmRi0 = {
		tSubRmRi_A0[32] ? tSubRmRi_B1 : tSubRmRi_B0,
		tSubRmRi_A0[31:0] };
	tSubRmRi1 = {
		tSubRmRi_A1[32] ? tSubRmRi_B1 : tSubRmRi_B0,
		tSubRmRi_A1[31:0] };

	tAddRmRi0 = {
		tAddRmRi_A0[32] ? tAddRmRi_B1 : tAddRmRi_B0,
		tAddRmRi_A0[31:0] };
	tAddRmRi1 = {
		tAddRmRi_A1[32] ? tAddRmRi_B1 : tAddRmRi_B0,
		tAddRmRi_A1[31:0] };

	aluIs2C = 1;

	aluSimdIs2C = 1;
//	aluSimdIs2C = 0;

	tRegValSpP8	= { ctlInSp[63:32], ctlInSp[31:0]+8 };
	tRegValSpN8	= { ctlInSp[63:32], ctlInSp[31:0]-8 };

	tMemAddr	= regMaAddr;
//	tMemData	= regValRm;
	tMemData	= regValRn;
	tMemOutId	= regIdRn;

`ifndef JX2_QUIET
//	if((regIdIxtB!=0) && !tRegOpSticky1)
	if((regIdIxtB!=0) && !tRegOpSticky1 && (opCmdA==JX2_UCMD_LEA_MR))
//	if(1'b1)
	begin
		$display("AGU %X+%X<<%d => %X",
			regValRb, regValRiB, regIdIxtB[1:0],
			regMaAddr);
	end
`endif

	regHoldFixed = 0;

	regRnIsGpr = 1;
	
//	if(!tRegOpSticky1)
//		$display("opCmdA: %X %X", opCmdA, opCmd);
	
	case(opCmdA)

		JX2_UCMD_NOP: begin
		end

		JX2_UCMD_INVOP: begin
			if(!tRegOpSticky1)
			begin
				$display("Jx2ExOp: Invalid Opcode");
			end
			tRegOutOK	= UMEM_OK_HOLD;
			tNextOpSticky1	= 1;
		end

		JX2_UCMD_MOV_RR: begin
			tRegOutVal	= regValRm;
			tRegOutId	= regIdRn;

`ifdef JX2_EX_GPRSIMD
// `ifndef def_true
			if(regIdIxt[2])
			begin
				case(regIdIxt[1:0])
					2'b00: tRegOutVal = { regValRm[63:32], regValRn[31: 0] };
					2'b01: tRegOutVal = { regValRn[63:32], regValRm[31: 0] };
					2'b10: tRegOutVal = { regValRn[63:32], regValRm[63:32] };
					2'b11: tRegOutVal = { regValRm[31: 0], regValRn[31: 0] };
				endcase
			end
`endif
		end
		JX2_UCMD_MOV_RC: begin
			tRegOutValCn	= regValRm;
			tRegOutIdCn		= regIdRn;
		end
		JX2_UCMD_MOV_CR: begin
			tRegOutVal	= regValCm;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_MOV_IR: begin
			tRegOutVal	= immValRiB;
			tRegOutId	= regIdRn;
		end

`ifdef JX2_EX_LEA_2C
		JX2_UCMD_LEA_MR: begin
			if(tOpState!=1)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end

			tRegOutVal	= regLastMaAddr;
			tRegOutId	= regIdRn;
		end
`else
		JX2_UCMD_LEA_MR: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= regMaAddr;
				tRegOutId	= regIdRn;
			end
		end
`endif

		JX2_UCMD_MOVB_RM: begin
//			tMemAddr	= regMaAddr;
//			tMemData	= regValRm;
			tMemOpm		= UMEM_OPM_WR_SB;
			tOpDoMovX	= 1;
	//		$display("MOV.B %X, (%X)", regValRm, regMaAddr);
		end
		JX2_UCMD_MOVW_RM: begin
//			tMemAddr	= regMaAddr;
//			tMemData	= regValRm;
			tMemOpm		= UMEM_OPM_WR_SW;
			tOpDoMovX	= 1;
	//		$display("MOV.W %X, (%X)", regValRm, regMaAddr);
		end
		JX2_UCMD_MOVL_RM: begin
//			tMemAddr	= regMaAddr;
//			tMemData	= regValRm;
			tMemOpm		= UMEM_OPM_WR_SL;
			tOpDoMovX	= 1;

	//		$display("MOV.L %X, (%X)", regValRm, regMaAddr);
		end

		JX2_UCMD_MOVQ_RM: begin
//			tMemAddr	= regMaAddr;
//			tMemData	= regValRm;
			tMemOpm		= UMEM_OPM_WR_Q;
			tOpDoMovX	= 1;

	//		$display("MOV.Q %X, (%X)", regValRm, regMaAddr);
		end

		JX2_UCMD_MOVB_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_SB;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			tOpDoMovX_MR	= 1;

	//		$display("MOV.B (%X), %X", regMaAddr, memDataLd);
		end
		JX2_UCMD_MOVW_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_SW;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			tOpDoMovX_MR	= 1;
	//		$display("MOV.W (%X), %X", regMaAddr, memDataLd);
		end
		JX2_UCMD_MOVL_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_SL;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			tOpDoMovX_MR	= 1;
//			$display("MOV.L (%X), %X", regMaAddr, memDataLd);
		end
		JX2_UCMD_MOVQ_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_Q;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			tOpDoMovX_MR	= 1;
//			$display("MOV.Q (%X), %X", regMaAddr, memDataLd);
		end

		JX2_UCMD_MOVUB_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_UB;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			tOpDoMovX_MR	= 1;
	//		$display("MOVU.B (%X), %X", regMaAddr, memDataLd);
		end
		JX2_UCMD_MOVUW_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_UW;
	//		tMemOpCmd2	= JX2_UCMD_MOVX_WB;
			tOpDoMovX_MR	= 1;
	//		$display("MOVU.W (%X), %X", regMaAddr, memDataLd);
		end

		JX2_UCMD_MOVUL_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_UL;
			tOpDoMovX_MR	= 1;
	//		$display("MOVU.L (%X), %X", regMaAddr, memDataLd);
		end


		JX2_UCMD_FMOVS_RM: begin
//			tMemAddr	= regMaAddr;
	//		tMemData	= regValFRm;
			tMemData	= { 32'h0, regValFRmSs };
			tMemOpm		= UMEM_OPM_WR_SL;
			tOpDoMovX	= 1;

	//		$display("MOV.L %X, (%X)", regValRm, regMaAddr);
		end
		JX2_UCMD_FMOVD_RM: begin
//			tMemAddr	= regMaAddr;
			tMemData	= regValFRm;
			tMemOpm		= UMEM_OPM_WR_Q;
			tOpDoMovX	= 1;

	//		$display("MOV.L %X, (%X)", regValRm, regMaAddr);
		end

		JX2_UCMD_FMOVS_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_UL;
			tMemOutId	= JX2_REG_IMM;
			tOpDoMovX	= 1;

	//		$display("FMOV.S (%X), %X", regMaAddr, memDataLd);
		end

		JX2_UCMD_FMOVD_MR: begin
//			tMemAddr	= regMaAddr;
			tMemOpm		= UMEM_OPM_RD_Q;
			tMemOutId	= JX2_REG_IMM;
			tOpDoMovX	= 1;
	//		$display("FMOV.S (%X), %X", regMaAddr, memDataLd);
		end


		JX2_UCMD_MOV_PUSH: begin
	//		$display("PUSH %X %X", ctlInSp-4, regValRn);
	//		tCtlOutSp	= ctlInSp-4;
//			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]-8 };
			tCtlOutSp	= tRegValSpN8;
	//		tCtlOutSp	= ctlInSp-8;
			tMemAddr	= tCtlOutSp;
			tMemData	= regValRn;
			tMemOpm		= UMEM_OPM_WR_Q;
			tOpDoMovX	= 1;
		end

		JX2_UCMD_MOV_POP: begin
//			tCtlOutSp	= ctlInSp+4;
//			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]+8 };
			tCtlOutSp	= tRegValSpP8;
//			tCtlOutSp	= ctlInSp+8;
			tMemAddr	= ctlInSp;
			tMemOpm		= UMEM_OPM_RD_Q;
			tOpDoMovX_MR	= 1;
//			$display("POP %X %X", ctlInSp, memDataLd);
		end

		JX2_UCMD_RET: begin
			tCtlOutSp	= tRegValSpP8;
			tMemAddr	= ctlInSp;
			tMemOutId	= JX2_REG_IMM;

			tNextOpSticky2 = tOpSticky2;
			tNextOpSticky3 = tOpSticky3;

			if(!tJointOkLatch && !tOpSticky2)
			begin
`ifndef JX2_QUIET
				$display("RET: OK Latch, OK=%X", memDataOK);
`endif
				tMemOpm		= UMEM_OPM_READY;
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
				if((memDataOK!=UMEM_OK_OK) && !tOpSticky2)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tMemOpm		= UMEM_OPM_RD_Q;
			end
			else
			begin
				tNextOpSticky2	= 1;
				tMemOpm		= UMEM_OPM_READY;
`ifndef JX2_QUIET
				$display("RET1 %X", memDataLd);
`endif
			end

//			$display("RET %X", regMaAddr);
//			tCtlOutPc	= memDataLd;

//			if(memDataOK==UMEM_OK_OK)
			if((memDataOK==UMEM_OK_OK) && tJointOkLatch)
			begin
				tMemOpm		= UMEM_OPM_CTRLF;
//				tCtlOutPc	= memDataLd;
				tCtlOutPc	= memNextDataLd;

				if(memDataLd==0)
				begin
					if(!tRegOpSticky1)
						$display("RET NULL");
					tRegOutOK	= UMEM_OK_HOLD;
					tNextOpSticky1 = 1;
				end
				else
				begin
`ifndef JX2_QUIET
					$display("RET %X", memDataLd);
`endif
				end
			end
		end


		JX2_UCMD_MOV_PUSHC: begin
	//		$display("PUSHC %X %X", ctlInSp-4, regValRn);
//			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]-8 };
			tCtlOutSp	= tRegValSpN8;
			tMemAddr	= tCtlOutSp;
			tMemData	= regValCm;
			tMemOpm		= UMEM_OPM_WR_Q;
			tOpDoMovX	= 1;
		end

		JX2_UCMD_MOV_POPC: begin
//			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]+8 };
			tCtlOutSp	= tRegValSpP8;
			tMemAddr	= ctlInSp;
			tMemOutId	= JX2_REG_IMM;
			tMemOpm		= UMEM_OPM_RD_Q;
			tOpDoMovX	= 1;

//			$display("POPC %X %X", ctlInSp, memDataLd);
//			tRegOutValCn	= memDataLd;
			tRegOutValCn	= memNextDataLd;
			tRegOutIdCn		= regIdRn;
		end


		default: begin
			tOpMiss1	= 1;
		end
	
	endcase
	
	if(tOpDoMovX || tOpDoMovX_MR)
	begin
		if(!tJointOkLatch && !tOpSticky2 && !tOpDoMovX_Inh)
		begin
			tMemOpm		= UMEM_OPM_READY;
			tRegOutOK	= UMEM_OK_HOLD;
		end
		else
		if((memDataOK!=UMEM_OK_OK) && !tOpSticky2 && !tOpDoMovX_Inh)
		begin
			tRegOutOK	= UMEM_OK_HOLD;
		end
		else
		begin
			tMemOpm		= UMEM_OPM_READY;
			tNextOpSticky2	= 1;
		end

`ifndef JX2_MEM_USELSQ
		if(tOpDoMovX_MR && !tOpDoMovX_Inh)
		begin
//			tRegOutVal	= memDataLd;
			tRegOutVal	= memNextDataLd;
			tRegOutId	= regIdRn;
		end
`endif
	end
	

`ifdef JX2_EX_GPRSIMD_HF
	regFphfCmd	= 0;
`endif

	case(opCmdB)

		JX2_UCMD_ALU_ADC3: begin
			tOpDoAri2C=1;

			if(ctlInSr[0])
			begin
				tRegOutVal = tAddRmRi1[63:0];
				tCtlOutSr[0] = tAddRmRi1[64];
			end
			else
			begin
				tRegOutVal = tAddRmRi0[63:0];
				tCtlOutSr[0] = tAddRmRi0[64];
			end

			tRegOutId	= regIdRn;
		end

		JX2_UCMD_ALU_SBB3: begin
			tOpDoAri2C=1;

			if(!ctlInSr[0])
			begin
`ifndef JX2_EX_MERGEAGU
				tRegOutVal = tSubRmRi1[63:0];
				tCtlOutSr[0] = tSubRmRi1[64];
`else
				tRegOutVal = tAddRmRi1[63:0];
				tCtlOutSr[0] = tAddRmRi1[64];
`endif
			end
			else
			begin
`ifndef JX2_EX_MERGEAGU
				tRegOutVal = tSubRmRi0[63:0];
				tCtlOutSr[0] = tSubRmRi0[64];
`else
				tRegOutVal = tAddRmRi0[63:0];
				tCtlOutSr[0] = tAddRmRi0[64];
`endif
			end

			tRegOutId	= regIdRn;
		end


		JX2_UCMD_ALU_ADD3: begin
			tOpDoAri2C=1;
		
			tRegOutVal = tAddRmRi0[63:0];
			tRegOutId	= regIdRn;				
//			$display("ADD3 %X+%X=%X", regValRm, regValRi, tRegOutVal);
		end
		JX2_UCMD_ALU_SUB3: begin
			tOpDoAri2C=1;

`ifndef JX2_EX_MERGEAGU
			tRegOutVal = tSubRmRi1[63:0];
			tRegOutId	= regIdRn;
`else
			tRegOutVal = tAddRmRi1[63:0];
			tRegOutId	= regIdRn;
`endif
		end
		JX2_UCMD_ALU_AND3: begin
			tRegOutVal	= regValRm & regValRi;
			tRegOutId	= regIdRn;
		end
		JX2_UCMD_ALU_OR3: begin
			tRegOutVal	= regValRm | regValRi;
			tRegOutId	= regIdRn;
		end
		JX2_UCMD_ALU_XOR3: begin
			tRegOutVal	= regValRm ^ regValRi;
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_ALU_MUL3: begin
			regMulA		= regValRm;
			regMulB		= regValRi;
			regMulOp	= 3'h2;
			tRegOutVal	= { regMulDlr[31]?UV32_FF:UV32_00, regMulDlr[31:0] };
			tRegOutId	= regIdRn;
			
			if(regMulOpDr!=regMulOp)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
	//			$display("MULS %X:%X", tCtlOutDhr, tCtlOutDlr);
			end

		end

`ifdef JX2_EX_GPRSIMD
		JX2_UCMD_ALU_PADDW: begin
			if(aluSimdIs2C && (tOpState!=1))
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end

			tRegOutId	= regIdRn;
			tRegOutVal	= tAluPAddW;
		end
		JX2_UCMD_ALU_PSUBW: begin
			if(aluSimdIs2C && (tOpState!=1))
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end

			tRegOutId	= regIdRn;
			tRegOutVal	= tAluPSubW;
		end

		JX2_UCMD_ALU_PADDL: begin
			if(aluSimdIs2C && (tOpState!=1))
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end

			tRegOutId	= regIdRn;
			tRegOutVal	= tAluPAddL;
		end
		JX2_UCMD_ALU_PSUBL: begin
			if(aluSimdIs2C && (tOpState!=1))
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end

			tRegOutId	= regIdRn;
			tRegOutVal	= tAluPSubL;
		end

		JX2_UCMD_ALU_PMULUW: begin
			regMulA			= regValRm;
			regMulB			= regValRn;
			regMulOp		= 3'h5;
//			tRegOutVal		= regMulDlr;
//			tRegOutId		= regIdRn;

			tCtlOutDlr		= regMulDlr;
			tCtlOutDhr		= regMulDhr;
			
			if(regMulOpDr!=regMulOp)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
		end

`endif

`ifndef def_true
		JX2_UCMD_ALU_PMULLW: begin
//			regMulA			= regValRm[31: 0];
//			regMulB			= regValRi[31: 0];
//			regMulC			= regValRm[63:32];
//			regMulD			= regValRi[63:32];
			regMulA			= regValRm;
			regMulB			= regValRi;
			regMulOp		= 3'h5;
//			regMulIsPMul	= 1;
//			tRegOutVal		= { regMulDlrHi, regMulDlr };
			tRegOutVal		= regMulDlr;
			tRegOutId		= regIdRn;
			
			if(regMulOpDr!=regMulOp)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
		end

		JX2_UCMD_ALU_PMULHW: begin
//			regMulA			= regValRm[31: 0];
//			regMulB			= regValRi[31: 0];
//			regMulC			= regValRm[63:32];
//			regMulD			= regValRi[63:32];
			regMulA			= regValRm;
			regMulB			= regValRi;
			regMulOp		= 3'h5;
//			regMulIsPMul	= 1;
//			tRegOutVal		= { regMulDhrHi, regMulDhr };
			tRegOutVal		= regMulDhr;
			tRegOutId		= regIdRn;
			
			if(regMulOpDr!=regMulOp)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
		end
`endif

`ifdef JX2_EX_ALU_ADDSL
		JX2_UCMD_ALU_ADDSL: begin
			tRegOutVal	= {
				tAddRmRi_A0[31] ? UV32_FF : UV32_00,
				tAddRmRi_A0[31:0] };
			tRegOutId	= regIdRn;
		end
		JX2_UCMD_ALU_ADDUL: begin
			tRegOutVal	= { UV32_00, tAddRmRi_A0[31:0] };
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_ALU_SUBSL: begin
			tRegOutVal	= {
				tSubRmRi_A1[31] ? UV32_FF : UV32_00,
				tSubRmRi_A1[31:0] };
			tRegOutId	= regIdRn;
		end
		JX2_UCMD_ALU_SUBUL: begin
			tRegOutVal	= { UV32_00, tSubRmRi_A1[31:0] };
			tRegOutId	= regIdRn;
		end
`endif

`ifdef	JX2_EX_GPRSIMD_HF
		JX2_UCMD_GSV_PFPHF: begin
			regFphfCmd	= regIdIxtB[1:0];
			tRegOutOK	= regFphfOutOK;
			tRegOutVal	= regFphfOutVal;
			tRegOutId	= regIdRn;
		end
`endif

		JX2_UCMD_ALU_CSELT: begin
			tRegOutId	= regIdRn;
			tRegOutVal	= ctlInSr[0] ? regValRm : regValRi;
		end

`ifdef	JX2_EX_GPRSIMD
		JX2_UCMD_ALU_PSELT: begin
			tRegOutId	= regIdRn;
			case(regIdIxtB[1:0])
				2'b00: begin
					tRegOutVal	= {
						ctlInSr[7] ? regValRm[63:48] : regValRi[63:48],
						ctlInSr[6] ? regValRm[47:32] : regValRi[47:32],
						ctlInSr[5] ? regValRm[31:16] : regValRi[31:16],
						ctlInSr[4] ? regValRm[15: 0] : regValRi[15: 0] };
				end
				2'b01: begin
					tRegOutVal	= {
						!ctlInSr[7] ? regValRm[63:48] : regValRi[63:48],
						!ctlInSr[6] ? regValRm[47:32] : regValRi[47:32],
						!ctlInSr[5] ? regValRm[31:16] : regValRi[31:16],
						!ctlInSr[4] ? regValRm[15: 0] : regValRi[15: 0] };
				end
				2'b10: begin
					tRegOutVal	= {
						ctlInSr[1] ? regValRm[63:32] : regValRi[63:32],
						ctlInSr[0] ? regValRm[31: 0] : regValRi[31: 0] };
				end
				2'b11: begin
					tRegOutVal	= {
						!ctlInSr[1] ? regValRm[63:32] : regValRi[63:32],
						!ctlInSr[0] ? regValRm[31: 0] : regValRi[31: 0] };
				end
			endcase
		end
`endif

		JX2_UCMD_ALU_SHLD3: begin
			tRegOutVal	= {UV32_00, tShadOut[31:0]};
			tRegOutId	= regIdRn;
		end
		JX2_UCMD_ALU_SHAD3: begin
			tRegOutVal	= {tShadOut[31]?UV32_FF:UV32_00, tShadOut[31:0]};
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_ALU_SHLDQ3: begin
			tRegOutVal	= tShadQOut[63:0];
			tRegOutId	= regIdRn;
		end
		JX2_UCMD_ALU_SHADQ3: begin
			tRegOutVal	= tShadQOut[63:0];
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_ALU_MULU: begin
			regMulA		= regValRn;
			regMulB		= regValRm;
			regMulOp	= 3'h1;
			tCtlOutDlr	= { UV32_00, regMulDlr[31:0] };
			tCtlOutDhr	= { UV32_00, regMulDhr[31:0] };

			if(regMulOpDr!=regMulOp)
				tRegOutOK	= UMEM_OK_HOLD;

		end

		JX2_UCMD_ALU_MULS: begin
			regMulA		= regValRn;
			regMulB		= regValRm;
			regMulOp	= 3'h2;
			tCtlOutDlr	= { regMulDlr[31]?UV32_FF:UV32_00, regMulDlr[31:0] };
			tCtlOutDhr	= { regMulDhr[31]?UV32_FF:UV32_00, regMulDhr[31:0] };
			
			if(regMulOpDr!=regMulOp)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
			begin
	//			$display("MULS %X:%X", tCtlOutDhr, tCtlOutDlr);
			end

		end
		
		JX2_UCMD_ALU_LDIX: begin
			tCtlOutDlr	= immValRiB;
		end
		JX2_UCMD_ALU_LDISH: begin
			tRegOutVal	= { regValRn[55:0], immValRi[7:0] };
			tRegOutId	= regIdRn;
		end

		JX2_UCMD_ALU_LDISH16: begin
			tRegOutVal	= { regValRn[47:0], immValRi[15:0] };
			tRegOutId	= regIdRn;
		end
		JX2_UCMD_ALU_LDISH32: begin
			tRegOutVal	= { regValRn[31:0], immValRi[31:0] };
			tRegOutId	= regIdRn;
		end

		default: begin
			tOpMiss2	= 1;
		end

	endcase


	case(opCmdC)

		JX2_UCMD_ALU_TST: begin
			tCtlOutSr[0] = tTstRmRnL;
		end
		JX2_UCMD_ALU_TSTQ: begin
			tCtlOutSr[0] = tTstRmRnQ;
		end

		JX2_UCMD_ALU_CMPEQ: begin
			tCtlOutSr[1:0] = tCmpRmRnEqPL;
			tCtlOutSr[7:4] = tCmpRmRnEqPW;
	//		$display("CMPEQ %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPHI: begin
			tCtlOutSr[1:0] = tCmpRmRnHiPL;
			tCtlOutSr[7:4] = tCmpRmRnHiPW;
	//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPGT: begin
			tCtlOutSr[1:0] = tCmpRmRnHiPL ^
				{	regValRn[63] ^ regValRm[63],
					regValRn[31] ^ regValRm[31] };
			tCtlOutSr[7:4] = tCmpRmRnHiPW ^
				{	regValRn[63] ^ regValRm[63],
					regValRn[47] ^ regValRm[47],
					regValRn[31] ^ regValRm[31],
					regValRn[15] ^ regValRm[15] };
	//		$display("CMPGT %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPGE: begin
			tCtlOutSr[1:0] = tCmpRmRnHsPL ^
				{	regValRn[63] ^ regValRm[63],
					regValRn[31] ^ regValRm[31] };
			tCtlOutSr[7:4] = tCmpRmRnHsPW ^
				{	regValRn[63] ^ regValRm[63],
					regValRn[47] ^ regValRm[47],
					regValRn[31] ^ regValRm[31],
					regValRn[15] ^ regValRm[15] };
	//		$display("CMPGE %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPHS: begin
			tCtlOutSr[1:0] = tCmpRmRnHsPL;
			tCtlOutSr[7:4] = tCmpRmRnHsPW;
	//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end

		JX2_UCMD_ALU_CMPQEQ: begin
			tOpDoAri2C=1;
			tCtlOutSr[0] = tCmpRmRnEqQ;
	//		$display("CMPEQ %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPQHI: begin
			tOpDoAri2C=1;
			tCtlOutSr[0] = tCmpRmRnHiQ;
	//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPQGT: begin
			tOpDoAri2C=1;
			tCtlOutSr[0] = tCmpRmRnHiQ ^
				(regValRn[63] ^ regValRm[63]);
	//		$display("CMPGT %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end

		JX2_UCMD_ALU_CMPGTHF: begin
			tCtlOutSr[1:0] = tCmpRmRnHiPL ^
				{	regValRn[63] | regValRm[63],
					regValRn[31] | regValRm[31] };
			tCtlOutSr[7:4] = tCmpRmRnHiPW ^
				{	regValRn[63] | regValRm[63],
					regValRn[47] | regValRm[47],
					regValRn[31] | regValRm[31],
					regValRn[15] | regValRm[15] };
		end

		JX2_UCMD_CF_BRA: begin
			if(!tOpDoMovX_Inh)
			begin
		//		$display("BRA %X", regMaAddr);
				tCtlOutPc	= regMaAddr;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
		JX2_UCMD_CF_BSR: begin
	//		$display("BSR %X", regMaAddr);
			tCtlOutPr	= idInGenPc;
			tCtlOutPc	= regMaAddr;
			tMemOpm		= UMEM_OPM_CTRLF;
		end

`ifndef def_true
		JX2_UCMD_CF_BT: begin
	//		$display("BT %X %X", regMaAddr, ctlInSr[0]);
			if(ctlInSr[0])
			begin
	//			$display("BT %X", regMaAddr);
				tCtlOutPc	= regMaAddr;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
		JX2_UCMD_CF_BF: begin
	//		$display("BF %X %X", regMaAddr, ctlInSr[0]);
			if(!ctlInSr[0])
			begin
	//			$display("BF %X", regMaAddr);
				tCtlOutPc	= regMaAddr;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
`endif

		JX2_UCMD_CF_JMP: begin
			if(!tOpDoMovX_Inh)
			begin
		//		$display("JMP %X", regMaAddr);
				tCtlOutPc	= regValRn;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
		JX2_UCMD_CF_JSR: begin
	//		$display("JSR %X", regMaAddr);
			tCtlOutPr	= idInGenPc;
			tCtlOutPc	= regValRn;
			tMemOpm		= UMEM_OPM_CTRLF;
		end

`ifndef def_true
		JX2_UCMD_CF_JT: begin
			if(ctlInSr[0])
			begin
				tCtlOutPc	= regValRn;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
		JX2_UCMD_CF_JF: begin
			if(!ctlInSr[0])
			begin
				tCtlOutPc	= regValRn;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
`endif

`ifdef JX2_EX_ALU_JMPCC
		JX2_UCMD_CF_JCMPCC: begin
			case(regIdIxt[3:1])
				3'h0: begin
					tJcmpStatus = tCmpRmRnEqL ^ regIdIxt[0];
				end
				3'h1: begin
					tJcmpStatus = tCmpRmRnHiL ^
						(regValRn[31] ^ regValRm[31]) ^
						regIdIxt[0];
				end
				3'h2: begin
					tJcmpStatus = tCmpRmRnHiL ^ regIdIxt[0];
				end
				3'h3: begin
					tJcmpStatus = tTstRmRnL ^ regIdIxt[0];
				end
				3'h4: begin
					tJcmpStatus = tCmpRmRnEqQ ^ regIdIxt[0];
				end
				3'h5: begin
					tJcmpStatus = tCmpRmRnHiQ ^
						(regValRn[63] ^ regValRm[63]) ^
						regIdIxt[0];
				end
				3'h6: begin
					tJcmpStatus = tCmpRmRnHiQ ^ regIdIxt[0];
				end
				3'h7: begin
					tJcmpStatus = tTstRmRnQ ^ regIdIxt[0];
				end
			endcase

			if(tJcmpStatus)
			begin
				tCtlOutPc	= regBraAddr;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
`endif

		JX2_UCMD_ALU_EXTUB: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { UV56_00, regValRm[7:0] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_EXTUW: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { UV48_00, regValRm[15:0] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_EXTUL: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { UV32_00, regValRm[31:0] };
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_EXTSB: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRm[7]?UV56_FF:UV56_00, regValRm[7:0] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_EXTSW: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRm[15]?UV48_FF:UV48_00, regValRm[15:0] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_EXTSL: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRm[31]?UV32_FF:UV32_00, regValRm[31:0] };
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_SWAPN: begin
			if(regRnIsGpr)
			begin
				tRegOutId	= regIdRn;
				tRegOutVal	= tSwapOutVal;
			end
		end

		JX2_UCMD_ALU_NOT: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= ~regValRn;
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_ROTL: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRn[62:0], regValRn[31] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_ROTR: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= { regValRn[0], regValRn[63:1] };
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_ROTCL: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[63];
				tRegOutVal		= { regValRn[62:0], ctlInSr[0] };
				tRegOutId		= regIdRn;
			end
		end
		JX2_UCMD_ALU_ROTCR: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[0];
				tRegOutVal		= { ctlInSr[0], regValRn[63:1] };
				tRegOutId		= regIdRn;
			end
		end

		JX2_UCMD_ALU_SHLL: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[63];
				tRegOutVal		= { regValRn[62:0], 1'b0 };
				tRegOutId		= regIdRn;
			end
		end
		JX2_UCMD_ALU_SHLR: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[0];
				tRegOutVal		= { 1'b0, regValRn[63:1] };
				tRegOutId		= regIdRn;
			end
		end
		JX2_UCMD_ALU_SHAR: begin
			if(regRnIsGpr)
			begin
				tCtlOutSr[0]	= regValRn[0];
				tRegOutVal		= { regValRn[63], regValRn[63:1] };
				tRegOutId		= regIdRn;
			end
		end

		JX2_UCMD_ALU_SHLLN: begin
			if(regRnIsGpr)
			begin
				tRegOutId		= regIdRn;
				case(regIdIxt[4:0])
					5'h00: tRegOutVal = { regValRn[62:0], 1'b0 };
					5'h01: tRegOutVal = { regValRn[61:0], 2'b0 };
					5'h02: tRegOutVal = { regValRn[59:0], 4'b0 };
					5'h03: tRegOutVal = { regValRn[55:0], 8'b0 };
					5'h04: tRegOutVal = {  1'b0, regValRn[63: 1] };
					5'h05: tRegOutVal = {  2'b0, regValRn[63: 2] };
					5'h06: tRegOutVal = {  4'b0, regValRn[63: 4] };
					5'h07: tRegOutVal = {  8'b0, regValRn[63: 8] };
					5'h08: tRegOutVal = { regRnSxt[   0], regValRn[63: 1] };
					5'h09: tRegOutVal = { regRnSxt[ 1:0], regValRn[63: 2] };
					5'h0A: tRegOutVal = { regRnSxt[ 3:0], regValRn[63: 4] };
					5'h0B: tRegOutVal = { regRnSxt[ 7:0], regValRn[63: 8] };
					5'h0C: tRegOutVal = { regValRn[47:0], 16'b0 };
					5'h0D: tRegOutVal = { 16'b0, regValRn[63:16] };
					5'h0E: tRegOutVal = { regRnSxt[15:0], regValRn[63:16] };
					5'h0F: tRegOutVal = regRnSxt;
					5'h10: tRegOutVal = { regValRn[31:0], 32'b0 };
					5'h11: tRegOutVal = { 32'b0, regValRn[63:32] };
					5'h12: tRegOutVal = { regRnSxt[31:0], regValRn[63:32] };
					default:	tRegOutVal = UV64_XX;
				endcase
			end
		end

		JX2_UCMD_OP_IXS: begin
			case(regIdIxt[7:0])
				JX2_UCMD_IXS_NOP: begin
				end
				JX2_UCMD_IXS_MOVT: begin
					tRegOutId		= regIdRn;
					tRegOutVal		= { UV63_00, ctlInSr[0] };
				end
				JX2_UCMD_IXS_MOVNT: begin
					tRegOutId		= regIdRn;
					tRegOutVal		= { UV63_00, !ctlInSr[0] };
				end
				
				JX2_UCMD_IXS_LDSRMSK: begin
					tCtlOutSr[1:0]	= regValRn[1:0];
					tCtlOutSr[7:4]	= regValRn[7:4];
					if(ctlInSr[30])
						tCtlOutSr[3:2]	= regValRn[3:2];
				end
//				JX2_UCMD_IXS_PLDMSK: begin
//					tCtlOutSr[1:0]	= regValRn[1:0];
//					tCtlOutSr[7:4]	= regValRn[3:0];
//				end
				
				default: begin
					$display("Jx2ExOp: Missing %X-%X", opCmdA, regIdIxt);
					tRegOutOK	= UMEM_OK_HOLD;
				end
			endcase
		end

		JX2_UCMD_OP_IXT: begin
			case(regIdIxt[7:0])
				JX2_UCMD_IX_NOP: begin
				end

				JX2_UCMD_IX_RTS: begin
					tCtlOutPc	= ctlInPr;
					tMemOpm		= UMEM_OPM_CTRLF;
				end

				JX2_UCMD_IX_BREAK: begin
					if(!tRegOpSticky1)
						$display("BREAK");
					tNextOpSticky1=1;
	//				tCtlOutPc	= ctlInPr;
					tRegOutOK	= UMEM_OK_HOLD;
				end

				JX2_UCMD_IX_CLRT: begin
					tCtlOutSr[0] = 1'b0;
				end
				JX2_UCMD_IX_SETT: begin
					tCtlOutSr[0] = 1'b1;
				end
				JX2_UCMD_IX_CLRS: begin
					tCtlOutSr[1] = 1'b0;
				end
				JX2_UCMD_IX_SETS: begin
					tCtlOutSr[1] = 1'b1;
				end

				JX2_UCMD_IX_NOTT: begin
					tCtlOutSr[0] = !ctlInSr[0];
				end
				JX2_UCMD_IX_NOTS: begin
					tCtlOutSr[1] = !ctlInSr[1];
				end

				JX2_UCMD_IX_RTE: begin
					if(!ctlInSr[29])
					begin
						if(!tRegOpSticky1)
						begin
							$display("Jx2ExOp: Invalid use of RTE");
						end
						tRegOutOK	= UMEM_OK_HOLD;
						tNextOpSticky1	= 1;
					end

					tCtlOutSr[29] = 0;
					tMemOpm		= UMEM_OPM_CTRLF;
				end

				JX2_UCMD_IX_LDTLB: begin
					tMemAddr	= ctlInDhr;
					tMemData	= ctlInDlr;
					tMemOpm		= UMEM_OPM_LDTLB;
					tMemOutId	= JX2_REG_IMM;
					if(memDataOK!=UMEM_OK_OK)
					begin
						tRegOutOK	= UMEM_OK_HOLD;
					end else begin
						tMemOpm		= UMEM_OPM_READY;
					end
				end

				JX2_UCMD_IX_PLDMSK: begin
					tCtlOutSr[1:0]	= regIdRn[1:0];
					tCtlOutSr[7:4]	= regIdRn[3:0];
				end
				
				JX2_UCMD_IX_CPUID: begin
					tCtlOutDlr = tCpuidDlr[regIdRn[3:0]];
					tCtlOutDhr = tCpuidDhr[regIdRn[3:0]];

`ifndef def_true
					case(regIdRn[3:0])
					4'h0: begin
						tCtlOutDlr[31: 0] = 32'h32584A42;
						tCtlOutDlr[63:32] = 32'h31304C56;
						tCtlOutDhr[31: 0] = UV32_00;
						tCtlOutDhr[63:32] = UV32_00;

`ifndef JX2_MEM_NOMMU
						tCtlOutDhr[0] = 1;		//Has MMU
`endif
`ifndef JX2_FPU_NOFPU
						tCtlOutDhr[1] = 1;		//Has FPU
`endif
`ifdef JX2_EX_GPRSIMD
						tCtlOutDhr[2] = 1;		//Has GSV
`endif
`ifdef JX2_EX_GPRSIMD_HF
						tCtlOutDhr[3] = 1;		//GSV Has Half-Float
`endif
`ifdef JX2_EX_ALU_ADDSL
						tCtlOutDhr[4] = 1;		//Has ADDx.L
`endif
`ifdef JX2_EX_ALU_JMPCC
						tCtlOutDhr[5] = 1;		//Has JMPcc
`endif
`ifndef JX2_FPU_NOFDIV
						tCtlOutDhr[6] = 1;		//Has FDIV
`endif
`ifdef JX2_EX_GPRSIMD_F
						tCtlOutDhr[7] = 1;		//GSV Has Float32
`endif
					end
					default: begin
						tCtlOutDlr[31: 0] = UV32_00;
						tCtlOutDlr[63:32] = UV32_00;
						tCtlOutDhr[31: 0] = UV32_00;
						tCtlOutDhr[63:32] = UV32_00;
					end
					endcase
`endif

				end

				default: begin
					$display("Jx2ExOp: Missing %X-%X", opCmdA, regIdIxt);
					tRegOutOK	= UMEM_OK_HOLD;
				end
			endcase
		end

		JX2_UCMD_FPU_FADD: begin
			/* Ignored Here, Handled by FPU */
		end
		JX2_UCMD_FPU_FSUB: begin
			/* Ignored Here, Handled by FPU */
		end
		JX2_UCMD_FPU_FMUL: begin
			/* Ignored Here, Handled by FPU */
		end
		JX2_UCMD_FPU_FDIV: begin
			/* Ignored Here, Handled by FPU */
		end

		JX2_UCMD_FPU_FCMPEQ: begin
			tOpDoAri2C = 1;
			tCtlOutSr[0] = tFCmpRmRnEqQ &&
				!tRegValFRn_IsNaN &&
				!tRegValFRm_IsNaN;
		end

		JX2_UCMD_FPU_FCMPGT: begin
			tOpDoAri2C = 1;
			tCtlOutSr[0] = tFCmpRmRnHiQ ^
				(regValFRn[31] | regValFRm[31]);
	//		$display("FCMPGT %X %X %X", regValFRn, regValFRm, tCtlOutSr[0]);
		end

		JX2_UCMD_FPU_FMOV: begin
			/* Ignored Here, Handled by FPU */
		end

//		JX2_UCMD_FPU_FMOV_GF: begin
//			/* Ignored Here, Handled by FPU */
//		end

//		JX2_UCMD_FPU_FMOV_FG: begin
//			tRegOutId		= regIdRn;
//			tRegOutVal		= regValFRm;
//		end


		JX2_UCMD_FPU_FSTCF: begin
//			tCtlOutDlr	= {UV32_00, regValFRnSs};
			tRegOutId	= regIdRn;
			tRegOutVal	= {UV32_00, regValFRmSs};
		end

		JX2_UCMD_FPU_FSTCD: begin
//			tCtlOutDlr	= regValFRn;
			tRegOutId	= regIdRn;
			tRegOutVal	= regValFRm;
		end

		JX2_UCMD_FPU_FSTCI: begin
//			tCtlOutDlr	= regValFRnSi;
			tRegOutId	= regIdRn;
			tRegOutVal	= regValFRmSi;

			if(regHoldFixedCnt<3)
			begin
				regHoldFixed = 1;
				tRegOutOK	= UMEM_OK_HOLD;
			end

		end

		JX2_UCMD_FPU_FSTCH: begin
//			tCtlOutDlr	= {UV48_00, regValFRnSh};
			tRegOutId	= regIdRn;
			tRegOutVal	= {UV48_00, regValFRmSh};
		end

		default: begin
			tOpMiss3	= 1;
		end

	endcase

	if(tOpDoAri2C)
	begin
		if(aluIs2C && (tOpState!=1))
		begin
			tRegOutOK	= UMEM_OK_HOLD;
			tNextOpState = 1;
		end
	end

	if(tOpMiss1 && tOpMiss2 && tOpMiss3)
	begin
		if(!tRegOpSticky1)
			$display("Missing Op Handler %X", opCmdA);
		tNextOpSticky1	= 1;
		tRegOutOK	= UMEM_OK_HOLD;
	end

end

always @ (posedge clock)
begin

// /*
	if(regHoldFixed)
		regHoldFixedCnt		<= (regHoldFixedCnt+1);
	else
		regHoldFixedCnt		<= 0;
// */

	idLastGenPc		<= idInGenPc;
	regLastMaAddr	<= regMaAddr;
	memHoldDataLd	<= memNextDataLd;

//	tMemAddr2	<= tMemAddr;
//	tMemData2	<= tMemData;
//	tMemOpm2	<= tMemOpm;

	tRegOpSticky1	<= tNextOpSticky1;
	tRegOpSticky2	<= tNextOpSticky2;
	tRegOpSticky3	<= tNextOpSticky3;
	tOpState		<= tNextOpState;

	tRegOkLatch		<= tNextOkLatch;

	tAddRmRi_A0		<= tNextAddRmRi_A0;
	tAddRmRi_A1		<= tNextAddRmRi_A1;
	tAddRmRi_B0		<= tNextAddRmRi_B0;
	tAddRmRi_B1		<= tNextAddRmRi_B1;
	tSubRmRi_A0		<= tNextSubRmRi_A0;
	tSubRmRi_A1		<= tNextSubRmRi_A1;
	tSubRmRi_B0		<= tNextSubRmRi_B0;
	tSubRmRi_B1		<= tNextSubRmRi_B1;

	tAluPAddW		<= tNextAluPAddW;
	tAluPAddL		<= tNextAluPAddL;
	tAluPSubW		<= tNextAluPSubW;
	tAluPSubL		<= tNextAluPSubL;

	tLastCmpRmRnEqPW	<= tCmpRmRnEqPW;
	tLastCmpRmRnHiPW	<= tCmpRmRnHiPW;
	tLastCmpRmRnHsPW	<= tCmpRmRnHsPW;
	tLastCmpRmRnEqPL	<= tCmpRmRnEqPL;
	tLastCmpRmRnHiPL	<= tCmpRmRnHiPL;
	tLastCmpRmRnHsPL	<= tCmpRmRnHsPL;

	tLastFCmpRmRnEqPW	<= tFCmpRmRnEqPW;
	tLastFCmpRmRnHiPW	<= tFCmpRmRnHiPW;
	tLastFCmpRmRnHsPW	<= tFCmpRmRnHsPW;
	tLastFCmpRmRnEqPL	<= tFCmpRmRnEqPL;
	tLastFCmpRmRnHiPL	<= tFCmpRmRnHiPL;
	tLastFCmpRmRnHsPL	<= tFCmpRmRnHsPL;

end


endmodule
