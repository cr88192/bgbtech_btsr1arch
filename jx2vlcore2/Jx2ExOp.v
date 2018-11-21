`include "Jx2CoreDefs.v"

`include "Jx2ExShad32.v"
`include "Jx2ExShad64.v"

`ifndef JX2_EX_MERGEAGU
`ifdef JX2_EX_AGU48
`include "Jx2ExCsAdd48F.v"
`else
`include "Jx2ExCsAdd64F.v"
`endif
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

reg[63:0]		regRnSxt;
reg[63:0]		immValRiB;
reg[31:0]		immValRiA;
reg[63:0]		regDlrA;

reg[31:0]		regMulA;
reg[31:0]		regMulB;
reg[1:0]		regMulOp;

reg[31:0]		regMulDlr;
reg[31:0]		regMulDhr;
reg[1:0]		regMulOpDr;

reg[3:0]		regHoldFixedCnt;
reg				regHoldFixed;
reg				regRnIsGpr;

// reg[7:0]		opCmdA;
// reg[7:0]		opCmdB;
// reg[7:0]		opCmdC;

wire[31:0]		regValFRmSs;	//FRm input value (Single Precision)
wire[31:0]		regValFRnSs;	//FRn input value (Single Precision)

Jx2ConvFpuD2S	fd2s_rm(regValFRm, regValFRmSs);
Jx2ConvFpuD2S	fd2s_rn(regValFRn, regValFRnSs);

// wire[15:0]		regValFRmSh;	//FRm input value (Single Precision)
wire[15:0]		regValFRnSh;	//FRn input value (Single Precision)

// Jx2ConvFpuD2H	fd2h_rm(regValFRm, regValFRmSh);
Jx2ConvFpuD2H	fd2h_rn(regValFRn, regValFRnSh);

wire[63:0]		regValFRnSi;	//FRn input value (Int)
Jx2ConvFpuD2I	fd2i_rn(
	clock,		reset,
	regValFRn,	regValFRnSi);

reg		tRegValFRn_IsNaN;
reg		tRegValFRm_IsNaN;

reg[63:0]		regValRm_T0;
reg[63:0]		regValRm_T1;
reg[63:0]		regValRi_T0;
reg[63:0]		regValRi_T1;

reg[32:0]		tAddRmRi_A0;
reg[32:0]		tAddRmRi_A1;
reg[32:0]		tAddRmRi_B0;
reg[32:0]		tAddRmRi_B1;
reg[32:0]		tSubRmRi_A0;
reg[32:0]		tSubRmRi_A1;
reg[32:0]		tSubRmRi_B0;
reg[32:0]		tSubRmRi_B1;

reg[16:0]		tAdd0RmRi_A0;
reg[16:0]		tAdd0RmRi_A1;
reg[16:0]		tAdd0RmRi_B0;
reg[16:0]		tAdd0RmRi_B1;
reg[16:0]		tAdd0RmRi_C0;
reg[16:0]		tAdd0RmRi_C1;
reg[16:0]		tAdd0RmRi_D0;
reg[16:0]		tAdd0RmRi_D1;

reg[16:0]		tSub0RmRi_A0;
reg[16:0]		tSub0RmRi_A1;
reg[16:0]		tSub0RmRi_B0;
reg[16:0]		tSub0RmRi_B1;
reg[16:0]		tSub0RmRi_C0;
reg[16:0]		tSub0RmRi_C1;
reg[16:0]		tSub0RmRi_D0;
reg[16:0]		tSub0RmRi_D1;

reg[64:0]		tAddRmRi0;
reg[64:0]		tAddRmRi1;
reg[64:0]		tSubRmRi0;
reg[64:0]		tSubRmRi1;
reg				aluIs2C;

`ifdef JX2_EX_ALU_2C
reg[64:0]		tNextAddRmRi0;
reg[64:0]		tNextAddRmRi1;
reg[64:0]		tNextSubRmRi0;
reg[64:0]		tNextSubRmRi1;
`endif

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

reg				tRegOkLatch;
reg				tNextOkLatch;
reg				tJointOkLatch;

reg				tOpMiss1;
reg				tOpMiss2;
reg				tOpMiss3;

reg[3:0]		tOpState;
reg[3:0]		tNextOpState;

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

	regMulA		= UV32_XX;
	regMulB		= UV32_XX;
	regMulOp	= 0;
//	tShOp		= 0;
	
	tNextOpSticky1	= 0;
	tNextOpSticky2	= 0;
	tNextOpSticky3	= 0;

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
	
	immValRiA = immValRi[31:0];
	regDlrA = ctlInDlr;
//	regDlrI4 = {regDlrA[59:0], immValRiA[3:0]};

//	regMaRbVal = regIdIxt[3] ? regValRm : regValRn;
	regMaRbVal = regValRb;

`ifdef JX2_EX_MERGEAGU
// `ifdef def_true
	case(regIdIxtB[2:0])
		3'b000: regMaIxVal2=regValRiB;
		3'b001: regMaIxVal2={regValRiB[62:0], 1'b0};
		3'b010: regMaIxVal2={regValRiB[61:0], 2'b0};
		3'b011: regMaIxVal2={regValRiB[60:0], 3'b0};
		3'b100: regMaIxVal2=~regValRiB;
		3'b101: regMaIxVal2=UV64_XX;
		3'b110: regMaIxVal2=UV64_XX;
		3'b111: regMaIxVal2={regValRiB[59:0], 4'b0};
	endcase
`else
	case(regIdIxtB[1:0])
		2'b00: regMaIxVal2=regValRiB;
		2'b01: regMaIxVal2={regValRiB[62:0], 1'b0};
		2'b10: regMaIxVal2={regValRiB[61:0], 2'b0};
//		2'b11: regMaIxVal2={regValRiB[60:0], 3'b0};
		2'b11: regMaIxVal2=regIdIxtB[2] ?
			{regValRiB[59:0], 4'b0} :
			{regValRiB[60:0], 3'b0};
	endcase
`endif

//	tMemAddr	= regMaAddr;

	tRegValFRn_IsNaN =
		(regValFRn[62:52]==11'h7FF) &&
		(regValFRn[51: 0]!=0);
	tRegValFRm_IsNaN =
		(regValFRm[62:52]==11'h7FF) &&
		(regValFRm[51: 0]!=0);
	
`ifndef JX2_EX_MERGEAGU
	regValRm_T0 = regValRmC;
	regValRm_T1 = regValRmC;
	regValRi_T0 = regValRiC;
	regValRi_T1 = regValRiC;
`else
	regValRm_T0 = regValRb;
	regValRm_T1 = regValRb;
	regValRi_T0 = regMaIxVal2;
	regValRi_T1 = regMaIxVal2;
`endif

	tAdd0RmRi_A0 =
		{ 1'b0, regValRm_T0[15:0] } +
		{ 1'b0, regValRi_T0[15:0] } + 0;
	tAdd0RmRi_A1 =
		{ 1'b0, regValRm_T0[15:0] } +
		{ 1'b0, regValRi_T0[15:0] } + 1;
	tAdd0RmRi_B0 =
		{ 1'b0, regValRm_T0[31:16] } +
		{ 1'b0, regValRi_T0[31:16] } + 0;
	tAdd0RmRi_B1 =
		{ 1'b0, regValRm_T0[31:16] } +
		{ 1'b0, regValRi_T0[31:16] } + 1;
	tAdd0RmRi_C0 =
		{ 1'b0, regValRm_T0[47:32] } +
		{ 1'b0, regValRi_T0[47:32] } + 0;
	tAdd0RmRi_C1 =
		{ 1'b0, regValRm_T0[47:32] } +
		{ 1'b0, regValRi_T0[47:32] } + 1;
	tAdd0RmRi_D0 =
		{ 1'b0, regValRm_T0[63:48] } +
		{ 1'b0, regValRi_T0[63:48] } + 0;
	tAdd0RmRi_D1 =
		{ 1'b0, regValRm_T0[63:48] } +
		{ 1'b0, regValRi_T0[63:48] } + 1;

	tAddRmRi_A0 = {
		tAdd0RmRi_A0[16] ? tAdd0RmRi_B1 : tAdd0RmRi_B0,
		tAdd0RmRi_A0[15:0] };
	tAddRmRi_A1 = {
		tAdd0RmRi_A1[16] ? tAdd0RmRi_B1 : tAdd0RmRi_B0,
		tAdd0RmRi_A1[15:0] };
	tAddRmRi_B0 = {
		tAdd0RmRi_C0[16] ? tAdd0RmRi_D1 : tAdd0RmRi_D0,
		tAdd0RmRi_C0[15:0] };
	tAddRmRi_B1 = {
		tAdd0RmRi_C1[16] ? tAdd0RmRi_D1 : tAdd0RmRi_D0,
		tAdd0RmRi_C1[15:0] };

`ifdef JX2_EX_ALU_2C
	tNextAddRmRi0 = {
		tAddRmRi_A0[32] ? tAddRmRi_B1 : tAddRmRi_B0,
		tAddRmRi_A0[31:0] };
	tNextAddRmRi1 = {
		tAddRmRi_A1[32] ? tAddRmRi_B1 : tAddRmRi_B0,
		tAddRmRi_A1[31:0] };
	aluIs2C = 1;
`else
	tAddRmRi0 = {
		tAddRmRi_A0[32] ? tAddRmRi_B1 : tAddRmRi_B0,
		tAddRmRi_A0[31:0] };
	tAddRmRi1 = {
		tAddRmRi_A1[32] ? tAddRmRi_B1 : tAddRmRi_B0,
		tAddRmRi_A1[31:0] };
	aluIs2C = 0;
`endif

`ifndef JX2_EX_MERGEAGU
	tSub0RmRi_A0 =
		{ 1'b0,  regValRm_T1[15:0] } +
		{ 1'b0, ~regValRi_T1[15:0] } + 0;
	tSub0RmRi_A1 =
		{ 1'b0,  regValRm_T1[15:0] } +
		{ 1'b0, ~regValRi_T1[15:0] } + 1;
	tSub0RmRi_B0 =
		{ 1'b0,  regValRm_T1[31:16] } +
		{ 1'b0, ~regValRi_T1[31:16] } + 0;
	tSub0RmRi_B1 =
		{ 1'b0,  regValRm_T1[31:16] } +
		{ 1'b0, ~regValRi_T1[31:16] } + 1;
	tSub0RmRi_C0 =
		{ 1'b0,  regValRm_T1[47:32] } +
		{ 1'b0, ~regValRi_T1[47:32] } + 0;
	tSub0RmRi_C1 =
		{ 1'b0,  regValRm_T1[47:32] } +
		{ 1'b0, ~regValRi_T1[47:32] } + 1;
	tSub0RmRi_D0 =
		{ 1'b0,  regValRm_T1[63:48] } +
		{ 1'b0, ~regValRi_T1[63:48] } + 0;
	tSub0RmRi_D1 =
		{ 1'b0,  regValRm_T1[63:48] } +
		{ 1'b0, ~regValRi_T1[63:48] } + 1;

	tSubRmRi_A0 = {
		tSub0RmRi_A0[16] ? tSub0RmRi_B1 : tSub0RmRi_B0,
		tSub0RmRi_A0[15:0] };
	tSubRmRi_A1 = {
		tSub0RmRi_A1[16] ? tSub0RmRi_B1 : tSub0RmRi_B0,
		tSub0RmRi_A1[15:0] };

	tSubRmRi_B0 = {
		tSub0RmRi_C0[16] ? tSub0RmRi_D1 : tSub0RmRi_D0,
		tSub0RmRi_C0[15:0] };
	tSubRmRi_B1 = {
		tSub0RmRi_C1[16] ? tSub0RmRi_D1 : tSub0RmRi_D0,
		tSub0RmRi_C1[15:0] };

`ifdef JX2_EX_ALU_2C
	tNextSubRmRi0 = {
		tSubRmRi_A0[32] ? tSubRmRi_B1 : tSubRmRi_B0,
		tSubRmRi_A0[31:0] };
	tNextSubRmRi1 = {
		tSubRmRi_A1[32] ? tSubRmRi_B1 : tSubRmRi_B0,
		tSubRmRi_A1[31:0] };
`else
	tSubRmRi0 = {
		tSubRmRi_A0[32] ? tSubRmRi_B1 : tSubRmRi_B0,
		tSubRmRi_A0[31:0] };
	tSubRmRi1 = {
		tSubRmRi_A1[32] ? tSubRmRi_B1 : tSubRmRi_B0,
		tSubRmRi_A1[31:0] };
`endif

`endif

`ifdef JX2_EX_MERGEAGU
	regMaAddr	= tAddRmRi0[63:0];
`endif

	tMemAddr	= regMaAddr;
	tMemData	= regValRm;
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
			else
			begin
				tRegOutVal	= regLastMaAddr;
				tRegOutId	= regIdRn;
			end
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
			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]-8 };
	//		tCtlOutSp	= ctlInSp-8;
			tMemAddr	= tCtlOutSp;
			tMemData	= regValRn;
			tMemOpm		= UMEM_OPM_WR_Q;
			tOpDoMovX	= 1;
		end

		JX2_UCMD_MOV_POP: begin
//			tCtlOutSp	= ctlInSp+4;
			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]+8 };
//			tCtlOutSp	= ctlInSp+8;
			tMemAddr	= ctlInSp;
			tMemOpm		= UMEM_OPM_RD_Q;
			tOpDoMovX_MR	= 1;
//			$display("POP %X %X", ctlInSp, memDataLd);
		end

		JX2_UCMD_RET: begin
			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]+8 };
//			tCtlOutSp	= ctlInSp+8;
			tMemAddr	= ctlInSp;
			tMemOutId	= JX2_REG_IMM;

//			tNextOpSticky2 = tRegOpSticky2;
			tNextOpSticky2 = tOpSticky2;
//			tNextOpSticky3 = tRegOpSticky3;
			tNextOpSticky3 = tOpSticky3;

//			if(!tJointOkLatch && !tNextOpSticky2)
			if(!tJointOkLatch && !tOpSticky2)
			begin
`ifndef JX2_QUIET
				$display("RET: OK Latch, OK=%X", memDataOK);
`endif
				tMemOpm		= UMEM_OPM_READY;
				tRegOutOK	= UMEM_OK_HOLD;
			end
			else
//				if(memDataOK!=UMEM_OK_OK)
//				if((memDataOK!=UMEM_OK_OK) && !tRegOpSticky2)
				if((memDataOK!=UMEM_OK_OK) && !tOpSticky2)
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tMemOpm		= UMEM_OPM_RD_Q;
//				tNextOpSticky2	= 1;
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
			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]-8 };
			tMemAddr	= tCtlOutSp;
			tMemData	= regValCm;
			tMemOpm		= UMEM_OPM_WR_Q;
			tOpDoMovX	= 1;
		end

		JX2_UCMD_MOV_POPC: begin
			tCtlOutSp	= { ctlInSp[63:32], ctlInSp[31:0]+8 };
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
		if(!tJointOkLatch && !tOpSticky2)
		begin
			tMemOpm		= UMEM_OPM_READY;
			tRegOutOK	= UMEM_OK_HOLD;
		end
		else
		if((memDataOK!=UMEM_OK_OK) && !tOpSticky2)
		begin
			tRegOutOK	= UMEM_OK_HOLD;
		end
		else
		begin
			tMemOpm		= UMEM_OPM_READY;
			tNextOpSticky2	= 1;
		end

`ifndef JX2_MEM_USELSQ
		if(tOpDoMovX_MR)
		begin
//			tRegOutVal	= memDataLd;
			tRegOutVal	= memNextDataLd;
			tRegOutId	= regIdRn;
		end
`endif
	end
	


	case(opCmdB)

		JX2_UCMD_ALU_TST: begin
			tCtlOutSr[0] = ((regValRn[31:0] & regValRm[31:0])==0);
		end
		JX2_UCMD_ALU_TSTQ: begin
			tCtlOutSr[0] = ((regValRn[63:0] & regValRm[63:0])==0);
		end

		JX2_UCMD_ALU_ADC3: begin
			if(aluIs2C && (tOpState!=1))
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end
			else
//			if(regRnIsGpr)
			begin
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
		end

		JX2_UCMD_ALU_SBB3: begin
			if(aluIs2C && (tOpState!=1))
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end
			else
//			if(regRnIsGpr)
			begin
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
		end


		JX2_UCMD_ALU_ADD3: begin
			if(aluIs2C && (tOpState!=1))
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end
			else
//			if(regRnIsGpr)
			begin
				tRegOutVal = tAddRmRi0[63:0];
				tRegOutId	= regIdRn;
				
//				$display("ADD3 %X+%X=%X", regValRm, regValRi, tRegOutVal);
			end
		end
		JX2_UCMD_ALU_SUB3: begin
			if(aluIs2C && (tOpState!=1))
			begin
				tRegOutOK	= UMEM_OK_HOLD;
				tNextOpState = 1;
			end
			else
//			if(regRnIsGpr)
			begin
`ifndef JX2_EX_MERGEAGU
				tRegOutVal = tSubRmRi1[63:0];
				tRegOutId	= regIdRn;
`else
				tRegOutVal = tAddRmRi1[63:0];
				tRegOutId	= regIdRn;
`endif
			end
		end
		JX2_UCMD_ALU_AND3: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= regValRm & regValRi;
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_OR3: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= regValRm | regValRi;
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_XOR3: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= regValRm ^ regValRi;
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_MUL3: begin
			regMulA		= regValRm[31:0];
			regMulB		= regValRi[31:0];
			regMulOp	= 2'h2;
			tRegOutVal	= { regMulDlr[31]?UV32_FF:UV32_00, regMulDlr };
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

		JX2_UCMD_ALU_CSELT: begin
			if(regRnIsGpr)
			begin
				tRegOutVal	= ctlInSr[0] ? regValRm : regValRi;
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_SHLD3: begin
//			tShOp = 3'h1;
			if(regRnIsGpr)
			begin
				tRegOutVal	= {UV32_00, tShadOut[31:0]};
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_SHAD3: begin
//			tShOp = 3'h2;
			if(regRnIsGpr)
			begin
				tRegOutVal	= {tShadOut[31]?UV32_FF:UV32_00, tShadOut[31:0]};
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_SHLDQ3: begin
//			tShOp = 3'h1;
			if(regRnIsGpr)
			begin
				tRegOutVal	= tShadQOut[63:0];
				tRegOutId	= regIdRn;
			end
		end
		JX2_UCMD_ALU_SHADQ3: begin
//			tShOp = 3'h2;
			if(regRnIsGpr)
			begin
				tRegOutVal	= tShadQOut[63:0];
				tRegOutId	= regIdRn;
			end
		end

		JX2_UCMD_ALU_MULU: begin
			regMulA		= regValRn[31:0];
			regMulB		= regValRm[31:0];
			regMulOp	= 2'h1;
			tCtlOutDlr	= { UV32_00, regMulDlr };
			tCtlOutDhr	= { UV32_00, regMulDhr };

			if(regMulOpDr!=regMulOp)
				tRegOutOK	= UMEM_OK_HOLD;

		end

		JX2_UCMD_ALU_MULS: begin
			regMulA		= regValRn[31:0];
			regMulB		= regValRm[31:0];
			regMulOp	= 2'h2;
			tCtlOutDlr	= { regMulDlr[31]?UV32_FF:UV32_00, regMulDlr };
			tCtlOutDhr	= { regMulDhr[31]?UV32_FF:UV32_00, regMulDhr };
			
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
//			tCtlOutDlr	= { ctlInDlr[55:0], immValRi[7:0] };

			tRegOutVal	= { regValRn[55:0], immValRi[7:0] };
			tRegOutId	= regIdRn;
//			if(regIdRn==JX2_REG_DLR)
//				tCtlOutDlr	= tRegOutVal;
//			else
//				tRegOutId	= regIdRn;
		end

		JX2_UCMD_ALU_LDISH16: begin
//			tCtlOutDlr	= { ctlInDlr[47:0], immValRi[15:0] };

			tRegOutVal	= { regValRn[47:0], immValRi[15:0] };
			tRegOutId	= regIdRn;
//			if(regIdRn==JX2_REG_DLR)
//				tCtlOutDlr	= tRegOutVal;
//			else
//				tRegOutId	= regIdRn;
		end
		JX2_UCMD_ALU_LDISH32: begin
//			tCtlOutDlr	= { ctlInDlr[31:0], immValRi[31:0] };

			tRegOutVal	= { regValRn[31:0], immValRi[31:0] };
			tRegOutId	= regIdRn;
//			if(regIdRn==JX2_REG_DLR)
//				tCtlOutDlr	= tRegOutVal;
//			else
//				tRegOutId	= regIdRn;
		end

		JX2_UCMD_ALU_CMPEQ: begin
			tCtlOutSr[0] = (regValRn[31:0] == regValRm[31:0]);
	//		$display("CMPEQ %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPHI: begin
			tCtlOutSr[0] = (regValRn[31:0] > regValRm[31:0]);
	//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPGT: begin
			tCtlOutSr[0] = (regValRn[31:0] > regValRm[31:0]) ^
				(regValRn[31] ^ regValRm[31]);
	//		$display("CMPGT %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPGE: begin
			tCtlOutSr[0] = (regValRn[31:0] >= regValRm[31:0]) ^
				(regValRn[31] ^ regValRm[31]);
	//		$display("CMPGE %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPHS: begin
			tCtlOutSr[0] = (regValRn[31:0] >= regValRm[31:0]);
	//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end

		JX2_UCMD_ALU_CMPQEQ: begin
			tCtlOutSr[0] = (regValRn == regValRm);
	//		$display("CMPEQ %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPQHI: begin
			tCtlOutSr[0] = (regValRn > regValRm);
	//		$display("CMPHI %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end
		JX2_UCMD_ALU_CMPQGT: begin
			tCtlOutSr[0] = (regValRn > regValRm) ^
				(regValRn[63] ^ regValRm[63]);
	//		$display("CMPGT %X %X %X", regValRn, regValRm, tCtlOutSr[0]);
		end

		default: begin
			tOpMiss2	= 1;
		end

	endcase


	case(opCmdC)

		JX2_UCMD_CF_BRA: begin
	//		$display("BRA %X", regMaAddr);
			tCtlOutPc	= regMaAddr;
//			tCtlOutPc	= regMaAddrPc;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
		JX2_UCMD_CF_BSR: begin
	//		$display("BSR %X", regMaAddr);
			tCtlOutPr	= idInGenPc;
			tCtlOutPc	= regMaAddr;
//			tCtlOutPc	= regMaAddrPc;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
		JX2_UCMD_CF_BT: begin
	//		$display("BT %X %X", regMaAddr, ctlInSr[0]);
			if(ctlInSr[0])
			begin
	//			$display("BT %X", regMaAddr);
				tCtlOutPc	= regMaAddr;
//				tCtlOutPc	= regMaAddrPc;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end
		JX2_UCMD_CF_BF: begin
	//		$display("BF %X %X", regMaAddr, ctlInSr[0]);
			if(!ctlInSr[0])
			begin
	//			$display("BF %X", regMaAddr);
				tCtlOutPc	= regMaAddr;
//				tCtlOutPc	= regMaAddrPc;
				tMemOpm		= UMEM_OPM_CTRLF;
			end
		end

		JX2_UCMD_CF_JMP: begin
	//		$display("JMP %X", regMaAddr);
			tCtlOutPc	= regValRn;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
		JX2_UCMD_CF_JSR: begin
	//		$display("JSR %X", regMaAddr);
			tCtlOutPr	= idInGenPc;
			tCtlOutPc	= regValRn;
			tMemOpm		= UMEM_OPM_CTRLF;
		end
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
			tCtlOutSr[0] = (regValFRn == regValFRm) &&
				!tRegValFRn_IsNaN &&
				!tRegValFRm_IsNaN;
		end

		JX2_UCMD_FPU_FCMPGT: begin
			tCtlOutSr[0] = (regValFRn > regValFRm) ^
				(regValFRn[31] | regValFRm[31]);
	//		$display("FCMPGT %X %X %X", regValFRn, regValFRm, tCtlOutSr[0]);
		end

		JX2_UCMD_FPU_FMOV: begin
			/* Ignored Here, Handled by FPU */
		end

		JX2_UCMD_FPU_FSTCF: begin
			tCtlOutDlr	= {UV32_00, regValFRnSs};
		end

		JX2_UCMD_FPU_FSTCD: begin
			tCtlOutDlr	= regValFRn;
		end

		JX2_UCMD_FPU_FSTCI: begin
			tCtlOutDlr	= regValFRnSi;

			if(regHoldFixedCnt<3)
			begin
				regHoldFixed = 1;
				tRegOutOK	= UMEM_OK_HOLD;
			end

		end

		JX2_UCMD_FPU_FSTCH: begin
			tCtlOutDlr	= {UV48_00, regValFRnSh};
		end

		default: begin
			tOpMiss3	= 1;
		end

	endcase

	if(tOpMiss1 && tOpMiss2 && tOpMiss3)
	begin
		if(!tRegOpSticky1)
			$display("Missing Op Handler %X", opCmdA);
		tNextOpSticky1	= 1;
		tRegOutOK	= UMEM_OK_HOLD;
	end

end


reg[31:0]		regMulA1;
reg[31:0]		regMulB1;
reg[1:0]		regMulOp1;

// reg[31:0]	regMulPiAA;
// reg[31:0]	regMulPiAB;
// reg[31:0]	regMulPiBA;
// reg[31:0]	regMulPiBB;

reg[15:0]	regMulPiAA;
reg[15:0]	regMulPiAB;
reg[15:0]	regMulPiBA;
reg[15:0]	regMulPiBB;

reg[31:0]	regMulPiAC;
reg[31:0]	regMulPiBC;

// reg[31:0]	regMulPiD;

reg[1:0]	regMulOpPi;
reg[1:0]	regMulOpPj;

reg[31:0]	regMulPpA;
reg[31:0]	regMulPpB;
reg[31:0]	regMulPpC;
reg[31:0]	regMulPpD;
reg[31:0]	regMulPpE;

reg[1:0]	regMulOpPp;

reg[31:0]	regMulPpA1;
reg[31:0]	regMulPpB1;
reg[31:0]	regMulPpC1;
reg[31:0]	regMulPpD1;


reg[31:0]	regMulPqA;
reg[31:0]	regMulPqB;
reg[1:0]	regMulOpPq;

always @*
begin
// / *
//	regMulPiAA	= { 16'h0, regMulA[15: 0] };
//	regMulPiAB	= { 16'h0, regMulA[31:16] };
//	regMulPiBA	= { 16'h0, regMulB[15: 0] };
//	regMulPiBB	= { 16'h0, regMulB[31:16] };

	regMulPiAA	= regMulA1[15: 0];
	regMulPiAB	= regMulA1[31:16];
	regMulPiBA	= regMulB1[15: 0];
	regMulPiBB	= regMulB1[31:16];

//	regMulPiAC	= { regMulA[31]?16'hFFFF:16'h0000, regMulA[31:16] };
//	regMulPiBC	= { regMulB[31]?16'hFFFF:16'h0000, regMulB[31:16] };
// * /

	regMulPiAC	= regMulA1[31]?(-regMulB1):UV32_00;
	regMulPiBC	= regMulB1[31]?(-regMulA1):UV32_00;

//	regMulPpA1	= regMulPiAA * regMulPiBA;
//	regMulPpB1	= regMulPiAA * regMulPiBB;
//	regMulPpC1	= regMulPiAB * regMulPiBA;
//	regMulPpD1	= regMulPiAB * regMulPiBB;

	regMulOpPi	= regMulOp1;

	{ regMulPqB, regMulPqA } =
		{UV32_00, regMulPpA} +
		{UV16_00, regMulPpB, UV16_00} +
		{UV16_00, regMulPpC, UV16_00} +
		{regMulPpD, UV32_00} +
		{regMulPpE, UV32_00};

	regMulOpPq = regMulOpPp;
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

`ifdef JX2_EX_ALU_2C
	tAddRmRi0		<= tNextAddRmRi0;
	tAddRmRi1		<= tNextAddRmRi1;
	tSubRmRi0		<= tNextSubRmRi0;
	tSubRmRi1		<= tNextSubRmRi1;
`endif


	regMulA1	<= regMulA;
	regMulB1	<= regMulB;
	regMulOp1	<= regMulOp;

	regMulPpA	<= { 16'h0, regMulPiAA } * { 16'h0, regMulPiBA };
	regMulPpB	<= { 16'h0, regMulPiAA } * { 16'h0, regMulPiBB };
	regMulPpC	<= { 16'h0, regMulPiAB } * { 16'h0, regMulPiBA };
	regMulPpD	<= { 16'h0, regMulPiAB } * { 16'h0, regMulPiBB };

	case(regMulOp1)
		2'h0: begin
			regMulOpPp	<= 0;
			regMulOpDr	<= 0;
		end

		2'h1: begin
			regMulPpE	<= 0;
			regMulOpPp	<= regMulOpPi;
			regMulDlr	<= regMulPqA;
			regMulDhr	<= regMulPqB;
			regMulOpDr	<= regMulOpPq;
		end

		2'h2: begin
			regMulPpE	<= regMulPiAC + regMulPiBC;
			regMulOpPp	<= regMulOpPi;
			regMulDlr	<= regMulPqA;
			regMulDhr	<= regMulPqB;
			regMulOpDr	<= regMulOpPq;
			
`ifndef JX2_QUIET
			$display("Jx2ExOp: Mul A: %X %X", regMulA, regMulB);
			$display("Jx2ExOp: Mul B: %X %X %X %X",
				regMulPpA, regMulPpB,
				regMulPpC, regMulPpD);
			$display("Jx2ExOp: Mul C: %X %X", regMulDlr, regMulDhr);
`endif
			
		end

		2'h3: begin
			regMulOpPp	<= 0;
			regMulOpDr	<= 0;
		end
	endcase
end

endmodule
