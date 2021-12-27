/*

PF IF ID1 ID2 EX1 EX2 EX3 WB

 */

`include "CoreDefs.v"

`include "minicore/McRbiMemL1A.v"

`include "minicore/McRegCR.v"
`include "minicore/McDecOp.v"
`include "minicore/McRegGPR.v"

`include "minicore/McExEX1.v"
`include "minicore/McExEX2.v"
`include "minicore/McExEX3.v"

`include "minicore/McExALU.v"
`include "ExMulC.v"
`include "ExMulW.v"

`ifdef jx2_enable_prebra
`include "DecPreBra.v"
`endif

`ifdef jx2_mc_enable_fpu
`include "minicore/McFpuExOp.v"
`endif

/* verilator lint_off DEFPARAM */

module McExUnit(
	clock,
	reset,
	timers,
	
	memAddrInA,		memAddrA,
	memDataIn,		memDataOut,
	memOpmIn,		memOpm,
	memSeqIn,		memSeqOut,
	unitNodeId,
	
	dbgOutPc,		dbgOutIstr,
	dbgExHold1,		dbgExHold2,

	dbgDcInAddr,	dbgDcInOpm,
	dbgDcOutVal,	dbgDcInVal,
	dbgDcOutOK,
	
	dbgOutStatus1,	dbgOutStatus2,
	dbgOutStatus3,	dbgOutStatus4,
	dbgOutStatus5,	dbgOutStatus6,
	dbgOutStatus7,	dbgOutStatus8
	);

input			clock;

(* max_fanout = 50 *)
	input			reset;

input[11:0]		timers;

output[95:0]		memAddrA;
input[95:0]			memAddrInA;

`wire_l2addr		memAddrOut;
`wire_l2addr		memAddrIn;

`ifdef jx2_enable_l2addr96
assign			memAddrIn = memAddrInA;
assign			memAddrA = memAddrOut;
`else
assign			memAddrIn = memAddrInA[47:0];
assign			memAddrA = { UV48_00, memAddrOut };
`endif

`input_tile		memDataIn;
`output_tile	memDataOut;

output[15:0]	memOpm;
input[15:0]		memOpmIn;

output[15:0]	memSeqOut;
input[15:0]		memSeqIn;

input[7:0]		unitNodeId;
	

output[47:0]	dbgOutPc;
output[95:0]	dbgOutIstr;

output			dbgExHold1;
output			dbgExHold2;

`output_mvaddr	dbgDcInAddr;
output[4:0]		dbgDcInOpm;
output[63:0]	dbgDcOutVal;
output[63:0]	dbgDcInVal;
output[ 1:0]	dbgDcOutOK;

output			dbgOutStatus1;
output			dbgOutStatus2;
output			dbgOutStatus3;
output			dbgOutStatus4;
output			dbgOutStatus5;
output			dbgOutStatus6;
output			dbgOutStatus7;
output			dbgOutStatus8;

parameter		isAltCore = 0;
defparam		ex1.isAltCore = isAltCore;

(* max_fanout = 100 *)
	reg				exHold1;
(* max_fanout = 100 *)
	reg				exHold2;

reg				exHold1L;

reg				exHold1A;
reg				exHold1B;
reg				exHold1C;
reg				exHold1D;

reg				exHold1B1;
reg				exHold1B2;
reg				exHold1B3;

reg				exHold1C1;
reg				exHold1C2;
reg				exHold1C3;

reg				tDbgExHold1;
reg				tDbgExHold2;

assign		dbgExHold1 = tDbgExHold1;
assign		dbgExHold2 = tDbgExHold2;

reg			tDbgOutStatus1;
reg			tDbgOutStatus2;
reg			tDbgOutStatus3;
reg			tDbgOutStatus4;
reg			tDbgOutStatus5;
reg			tDbgOutStatus6;
reg			tDbgOutStatus7;
reg			tDbgOutStatus8;

reg			tDbgOutStatus1B;
reg			tDbgOutStatus2B;
reg			tDbgOutStatus3B;
reg			tDbgOutStatus4B;
reg			tDbgOutStatus5B;
reg			tDbgOutStatus6B;
reg			tDbgOutStatus7B;
reg			tDbgOutStatus8B;

`ifndef def_true
assign		dbgOutStatus1 = tDbgOutStatus1B;
assign		dbgOutStatus2 = tDbgOutStatus2B;
assign		dbgOutStatus3 = tDbgOutStatus3B;
assign		dbgOutStatus4 = tDbgOutStatus4B;
assign		dbgOutStatus5 = tDbgOutStatus5B;
assign		dbgOutStatus6 = tDbgOutStatus6B;
assign		dbgOutStatus7 = tDbgOutStatus7B;
assign		dbgOutStatus8 = tDbgOutStatus8B;
`endif

reg			tDbgOutStatus1C;
reg			tDbgOutStatus2C;
reg			tDbgOutStatus3C;
reg			tDbgOutStatus4C;
reg			tDbgOutStatus5C;
reg			tDbgOutStatus6C;
reg			tDbgOutStatus7C;
reg			tDbgOutStatus8C;

`ifdef def_true
assign		dbgOutStatus1 = tDbgOutStatus1C;
assign		dbgOutStatus2 = tDbgOutStatus2C;
assign		dbgOutStatus3 = tDbgOutStatus3C;
assign		dbgOutStatus4 = tDbgOutStatus4C;
assign		dbgOutStatus5 = tDbgOutStatus5C;
assign		dbgOutStatus6 = tDbgOutStatus6C;
assign		dbgOutStatus7 = tDbgOutStatus7C;
assign		dbgOutStatus8 = tDbgOutStatus8C;
`endif


/* IF */

wire[63:0]		gprOutDlr;
wire[63:0]		gprOutDhr;

wire[63:0]		crOutMmcr;
wire[63:0]		crOutKrr;
wire[63:0]		crOutSr;
reg [63:0]		crInSr;

reg[47:0]		ifValPc;
reg[47:0]		ifLastPc;

`wire_mvaddr		ifValPcW = ifValPc;
`wire_mvaddr		ifLastPcW = ifLastPc;

// wire[63:0]	ifIstrWord;	//source instruction word
wire[95:0]		ifIstrWord;	//source instruction word
wire[1:0]		ifOutPcOK;
wire[3:0]		ifOutPcStep;
reg[3:0]		ifLastPcStep;
reg				ifInPcHold;
wire[3:0]		ifOutPcSxo;

reg				ifValBraOk;
reg				ifNxtValBraOk;

wire[1:0]			ifInPcWxe;

assign	ifInPcWxe = 2'b00;

// assign	dbgOutPc	= ifLastPcW;
assign	dbgOutPc	= ifLastPc;
assign	dbgOutIstr	= ifIstrWord;

`reg_mvaddr		dcInAddr;
reg [4:0]		dcInOpm;
wire[63:0]		dcOutVal;
reg [63:0]		dcInVal;
wire[ 1:0]		dcOutOK;
reg				dcInHold;
wire			dcOutHold;
wire			dcBusWait;

// wire[63:0]		dcOutValB;
// reg [63:0]		dcInValB;

reg [63:0]		dcInTraPc;

reg			tDeadlockLatch;
reg			tNxtDeadlockLatch;

assign	dbgDcInAddr	= dcInAddr;
assign	dbgDcInOpm	= dcInOpm;
assign	dbgDcOutVal	= dcOutVal;
assign	dbgDcInVal	= dcInVal;
assign	dbgDcOutOK	= dcOutOK;

wire[127:0]		memRegExc;
wire[63:0]		memRegTraPc;

McRbiMemL1A		memL1(
	clock,			reset,

	ifValPcW,		ifIstrWord,
	ifOutPcOK,		ifOutPcStep,
	ifInPcHold,		ifInPcWxe,
	ifOutPcSxo,

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutOK,		dcInHold,
	dcOutHold,		dcBusWait,

	gprOutDlr,		gprOutDhr,
	crOutMmcr,		crOutKrr,		crOutSr,

	memRegExc,		memRegTraPc,
	dcInTraPc,		tDeadlockLatch,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpm,
	memSeqIn,		memSeqOut,
	unitNodeId
	);


/* ID1 */

wire[47:0]		gprValGbr;
wire[47:0]		gprValTbr;
wire[63:0]		gprValLr;
wire[63:0]		gprValCm;

reg[47:0]		id1ValBPc;
reg[47:0]		id1ValPc;
reg[95:0]		id1IstrWord;	//source instruction word
reg[95:0]		id1IstrWordL1;	//source instruction word
reg[95:0]		id1IstrWordL2;	//source instruction word
reg[3:0]		id1IstrSxo;


`wire_gpr		id1IdRegN;
`wire_gpr		id1IdRegM;
`wire_gpr		id1IdRegO;
wire[32:0]		id1IdImm;
wire[32:0]		id1IdImmB;
wire[8:0]		id1IdUCmd;
wire[8:0]		id1IdUIxt;

McDecOp	decOp(
	clock,	reset,
	id1IstrWord[63:0],
	{ id1IstrWordL2[31:0], id1IstrWordL1[31:0] },
	crOutSr,	id1IstrSxo,
	id1IdRegN,	id1IdRegM,
	id1IdRegO,	id1IdImm,	id1IdImmB,
	id1IdUCmd,	id1IdUIxt
	);

reg[47:0]		ex1ValBPc;
reg[2:0]		ex1ValBraDir;

wire[ 4:0]		ex1MemOpm;

wire[47:0]		id1PreBraPc;
wire[1:0]		id1PreBra;

`ifndef jx2_enable_prebra
assign		id1PreBraPc	= 0;
assign		id1PreBra	= 0;
`endif

`ifdef jx2_enable_prebra

// `ifdef jx2_prebra_rts
`ifndef def_true
reg				id1BraPipelineLr;
reg				id1BraPipelineR1;
reg				id1BraPipelineDbl;
`else
wire			id1BraPipelineLr;
wire			id1BraPipelineR1;
wire			id1BraPipelineDbl;
assign		id1BraPipelineLr = 1'b1;
assign		id1BraPipelineR1 = 1'b1;
assign		id1BraPipelineDbl = 1'b1;
`endif

reg[7:0]			id1BraPipelineLrL;

DecPreBra	preBra(
	clock,				reset,
	id1IstrWord[63:0],	id1ValBPc,	id1ValPc,
	id1PreBraPc,		id1PreBra,
	gprValLr,			ifLastPc,
	gprOutDlr,			gprOutDhr,
	ex1ValBPc,			ex1ValBraDir,
	id1BraPipelineLrL,	ex1MemOpm);
`endif
	

/* ID2 */

reg[47:0]		id2ValBPc;
reg[47:0]		id2PreBraPc;
reg[1:0]		id2PreBra;
reg[95:0]		id2IstrWord;	//source instruction word

wire	crIsIsrEdge;
assign	crIsIsrEdge	= 1'b0;


/* ID2, GPR */

reg				gprEx1Flush;
reg				gprEx2Flush;
reg				gprEx3Flush;

reg				gprEx1DualLane;
reg				gprEx2DualLane;
reg				gprEx3DualLane;

reg[8:0]		id2IdUCmd;
reg[8:0]		id2IdUIxt;

`reg_gpr		gprIdRs;		//Source A, ALU / Base
`reg_gpr		gprIdRt;		//Source B, ALU / Index
`reg_gpr		gprIdRm;		//Source C, MemStore

wire[63:0]		gprValRs;
wire[63:0]		gprValRt;
wire[63:0]		gprValRm;

`reg_gpr		gprIdRn1;
reg[63:0]		gprValRn1;
`reg_gpr		gprIdRn2;
reg[63:0]		gprValRn2;
`reg_gpr		gprIdRn3;
reg[63:0]		gprValRn3;

reg [47:0]		gprValPc;
reg [32:0]		gprValImm;
reg [32:0]		gprValImmB;

assign		gprValGbr = crOutGbr;
assign		gprValLr = crOutLr;
assign		gprValCm = crValCm;

reg [63:0]		gprInDlr;
reg [63:0]		gprInDhr;
wire[63:0]		gprOutSp;
reg [63:0]		gprInSp;

`ifdef jx2_sprs_elrehr
wire[63:0]		gprOutElr;
reg [63:0]		gprInElr;
wire[63:0]		gprOutEhr;
reg [63:0]		gprInEhr;
wire[63:0]		gprOutBp;
reg [63:0]		gprInBp;
`endif

McRegGPR regGpr(
	clock,
	reset,
//	exHold2,
	exHold2 && !crIsIsrEdge,

	id2IdUCmd,
	id2IdUIxt,

	gprIdRs,		//Source A, ALU / Base
	gprIdRt,		//Source B, ALU / Index
	gprIdRm,		//Source C, MemStore
	gprValRs,		//Source A Value
	gprValRt,		//Source B Value
	gprValRm,		//Source C Value

	gprIdRn1,		//Destination ID (EX1)
	gprValRn1,		//Destination Value (EX1)
	gprIdRn2,		//Destination ID (EX2)
	gprValRn2,		//Destination Value (EX2)
	gprIdRn3,		//Destination ID (EX3)
	gprValRn3,		//Destination Value (EX3)
	
	gprValPc,		//PC Value (Synthesized)
	gprValGbr,		//GBR Value (CR)
	gprValImm,		//Immediate (Decode)
	gprValImmB,		//Immediate (Decode)
	gprValLr,		//LR Value (CR)
	gprValCm,		//Cm Port (CR)
	
	gprOutDlr,	gprInDlr,
	gprOutDhr,	gprInDhr,
	gprOutSp,	gprInSp
	);

/* ID2, CR */

`reg_gpr	crIdCm;		//Source ID
wire[63:0]	crValCm;		//Source Value
`reg_gpr	crIdCn1;		//Destination ID (EX1)
reg [63:0]	crValCn1;		//Destination Value (EX1)
`reg_gpr	crIdCn2;		//Destination ID (EX2)
reg [63:0]	crValCn2;		//Destination Value (EX2)
`reg_gpr	crIdCn3;		//Destination ID (EX3)
reg [63:0]	crValCn3;		//Destination Value (EX3)
wire[47:0]	crOutPc;
reg [47:0]	crInPc;
wire[63:0]	crOutLr;
reg [63:0]	crInLr;

wire[63:0]	crOutExsr;
reg [63:0]	crInExsr;
wire[47:0]	crOutSpc;
reg [47:0]	crInSpc;
wire[47:0]	crOutSsp;
reg [47:0]	crInSsp;
wire[63:0]	crOutTea;
reg [63:0]	crInTea;
wire[63:0]	crOutTeaHi;
reg [63:0]	crInTeaHi;

wire[15:0]	crOutFpsr;
wire[15:0]	crInFpsr;

assign	crInFpsr = crOutFpsr;

wire[47:0]	crOutVbr;
wire[47:0]	crOutGbr;
wire[47:0]	crOutTbr;

McRegCR regCr(
	clock,	reset,
//	exHold2,
	exHold2 && !crIsIsrEdge,

	crIdCm,		//Source ID
	crValCm,		//Source Value

	crIdCn1,		//Destination ID (EX1)
	crValCn1,		//Destination Value (EX1)
	crIdCn2,		//Destination ID (EX2)
	crValCn2,		//Destination Value (EX2)
	crIdCn3,		//Destination ID (EX3)
	crValCn3,		//Destination Value (EX3)

	gprEx1Flush,	//Flush EX1
	gprEx2Flush,	//Flush EX2
	gprEx3Flush,	//Flush EX3

	gprValPc,		//PC Value (Synthesized)

	crOutPc,	crInPc,
	crOutLr,	crInLr,
	crOutSr,	crInSr,

	crOutExsr,	crInExsr,
	crOutSpc,	crInSpc,
	crOutSsp,	crInSsp,
	crOutTea,	crInTea,
	crOutTeaHi,	crInTeaHi,
	crOutFpsr,	crInFpsr,

	crOutVbr,
	crOutGbr,
	crOutTbr,

	crOutMmcr,
	crOutKrr

	);



wire[47:0]		id2RegValSp;		//SP Value (Capture)
wire[63:0]		id2RegValLr;		//LR Value (Capture)


/* EX1 */	

reg[47:0]		ex1RegValSp;		//SP Value (Capture)
reg[47:0]		ex2RegValSp;		//SP Value (Capture)
reg[47:0]		ex3RegValSp;		//SP Value (Capture)

reg[63:0]		ex1RegValLr;		//LR Value (Capture)
reg[63:0]		ex2RegValLr;		//LR Value (Capture)
reg[63:0]		ex3RegValLr;		//LR Value (Capture)

reg[63:0]		ex2RegInSr;

reg[127:0]		tNxtRegExc;
reg[127:0]		tRegExc;


wire[63:0]		ex1MulVal;
wire[63:0]		ex1MulWVal;

// `ifdef jx2_enable_fpu
`wire_gpr		ex1RegIdFRn;
wire[63:0]		ex1RegValFRn;
wire[63:0]		ex1FpuValGRn;
wire[63:0]		exB1FpuValGRn;
wire[63:0]		exC1FpuValGRn;
wire[63:0]		ex1FpuValLdGRn;
wire[1:0]		ex1FpuOK;
// wire			ex1FpuSrT;
// `endif

`ifndef jx2_enable_fpu
assign	ex1FpuValGRn	= UV64_XX;
assign	exB1FpuValGRn	= UV64_XX;
assign	ex1FpuValLdGRn	= UV64_XX;
assign	ex1FpuOK		= UMEM_OK_READY;
assign	ex1FpuSrT		= 0;
`endif

`ifndef jx2_use_fpu_w
assign	exB1FpuValGRn	= UV64_XX;
`endif

assign	exC1FpuValGRn	= UV64_XX;

reg[63:0]		ex2MemDataIn;
// reg[63:0]		ex2MemDataInB;
reg[1:0]		ex2MemDataOK;


// reg[31:0]		ex1ValBPc;

reg[8:0]		ex1OpUCmd;
reg[8:0]		ex1OpUIxt;
wire[1:0]		ex1Hold;
reg[47:0]		ex1PreBraPc;
reg[1:0]		ex1PreBra;
// reg[31:0]	ex1IstrWord;	//source instruction word
reg[95:0]		ex1IstrWord;	//source instruction word
reg				ex1BraFlush;
reg				ex1TrapFlush;
reg[11:0]		ex1Timers;

`reg_gpr		ex1RegIdRs;		//Source A, ALU / Base
`reg_gpr		ex1RegIdRt;		//Source B, ALU / Index
`reg_gpr		ex1RegIdRm;		//Source C, MemStore
reg[63:0]		ex1RegValRs;		//Source A Value
reg[63:0]		ex1RegValRt;		//Source B Value
reg[63:0]		ex1RegValRm;		//Source C Value

reg[63:0]		exB1RegValRs;		//Source A Value

reg[63:0]		ex1RegValCRm;		//Source C Value (CR)

`wire_gpr		ex1RegIdRn1;		//Destination ID (EX1)
wire[63:0]		ex1RegValRn1;		//Destination Value (EX1)
//wire[4:0]		ex1RegIdCn1;		//Destination ID (CR, EX1)
`wire_gpr		ex1RegIdCn1;		//Destination ID (CR, EX1)
wire[63:0]		ex1RegValCn1;		//Destination Value (CR, EX1)

`wire_gpr		ex1HldIdRn1;		//Held Dest ID (EX1)
`wire_gpr		ex1HldIdCn1;		//Held Dest ID (CR, EX1)
	
reg[47:0]		ex1RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex1RegValImm;		//Immediate (Decode)
	
wire[63:0]		ex1RegOutDlr;
reg[63:0]		ex1RegInDlr;
wire[63:0]		ex1RegOutDhr;
reg[63:0]		ex1RegInDhr;
wire[63:0]		ex1RegOutSp;
reg[63:0]		ex1RegInSp;

wire[63:0]		ex1RegOutLr;
reg[63:0]		ex1RegInLr;
wire[63:0]		ex1RegOutSr;
reg[63:0]		ex1RegInSr;

wire[47:0]		ex1OutPcHi;
wire[47:0]		ex1OutLrHi;
wire[47:0]		ex1OutGbrHi;

`wire_mvaddr	ex1MemAddr;
wire[63:0]		ex1MemDataOut;
wire[63:0]		ex1MemDataOutB;

wire[ 7:0]		ex1RegOutSchm;
reg [ 7:0]		ex1RegInSchm;
wire[127:0]		ex1TrapExc;

wire[8:0]		ex1OpUCmd2;

wire			ex1AluSrJcmpT;

McExEX1		ex1(
	clock,			reset,
	ex1OpUCmd,		ex1OpUIxt,
	ex1Hold,		ex1TrapExc,
	ex1Timers,		ex1OpUCmd2,

	ex1RegIdRs,		ex1RegIdRt,		ex1RegIdRm,
	ex1RegValRs,	ex1RegValRt,	ex1RegValRm,
	exB1RegValRs,
	
	ex1RegValCRm,

	ex1RegIdRn1,	ex1RegValRn1,
	ex1RegIdCn1,	ex1RegValCn1,
	ex1HldIdRn1,	ex1HldIdCn1,

	{	ex1RegInSr[15:4],
		ex1RegInSr[27:26],
		ex1RegInSr[1:0],
		ex1RegValPc },

	ex1RegValImm,
	ex1FpuValGRn,	0,
	ex1BraFlush,
	ex1PreBraPc,	ex1PreBra,
	ex1AluSrJcmpT,
	
	ex1RegOutDlr,	ex1RegInDlr,
	ex1RegOutDhr,	ex1RegInDhr,
	ex1RegOutSp,	ex1RegInSp,

	ex1RegOutLr,	ex1RegInLr,
	ex1RegOutSr,	ex1RegInSr,
	ex1RegOutSchm,	ex1RegInSchm,

	ex1MemAddr,		ex1MemOpm,
	ex1MemDataOut,	ex1MemDataOutB,
	ex2MemDataOK,	tRegExc[63:0]
	);

wire[7:0]	exB1ValCarryD;
assign	exB1ValCarryD = 0;

wire[69:0]	ex1ValAlu;

McExALU	exAlu(
	clock,				reset,
	ex1RegValRs,		ex1RegValRt,	
	exB1RegValRs,		ex1RegValRm,
	ex1OpUCmd,			ex1OpUIxt,
	exHold2,			{ ex1RegInSr[7:4], ex1RegInSr[1:0] },
	ex1ValAlu[63:0],	ex1ValAlu[69:64],
	exB1ValCarryD,		ex1AluSrJcmpT);

ExMulC	ex1Mul(
	clock,			reset,
	ex1RegValRs,	ex1RegValRt,		ex1RegValRm,
	ex1OpUCmd,		ex1OpUIxt,
	exHold2,		ex1MulVal
	);

ExMulW	ex1MulW(
	clock,				reset,
	ex1RegValRs,		ex1RegValRt,
	ex1OpUCmd,			ex1OpUIxt,
	exHold2,			ex1MulWVal,
	0
	);

wire[65:0]				ex1KrreLo;
wire[65:0]				ex1KrreHi;
assign		ex1KrreLo = 0;
assign		ex1KrreHi = 0;

`ifdef jx2_mc_enable_fpu

McFpuExOp	ex1Fpu(
	clock,			reset,

	ex1OpUCmd,		ex1OpUIxt,
	ex1RegValRs,	ex1RegValRt,
	ex1RegValRs,	ex1RegValFRn,
	ex1FpuOK,
	
	ex2RegInSr,		ex1BraFlush,
	exHold2,
	
	ex1RegValRs,	ex1FpuValGRn
	);

`else

assign	ex1FpuOK = 0;
assign	ex1FpuValGRn = 0;

`endif


/* EX2 */

reg[47:0]		ex2ValBPc;

reg[8:0]		ex2OpUCmd;
reg[8:0]		ex2OpUIxt;
wire[1:0]		ex2Hold;
reg[47:0]		ex2PreBraPc;
reg[1:0]		ex2PreBra;
// reg[31:0]	ex2IstrWord;	//source instruction word
reg[95:0]		ex2IstrWord;	//source instruction word

`reg_gpr		ex2RegIdRs;		//Source A, ALU / Base
`reg_gpr		ex2RegIdRt;		//Source B, ALU / Index
`reg_gpr		ex2RegIdRm;		//Source C, MemStore
reg[63:0]		ex2RegValRs;		//Source A Value
reg[63:0]		ex2RegValRt;		//Source B Value
reg[63:0]		ex2RegValRm;		//Source C Value

reg[63:0]		ex2RegValCRm;		//Source C Value (CR)

`reg_gpr		ex2RegIdRn1;		//Destination ID (EX1)
reg[63:0]		ex2RegValRn1;		//Destination Value (EX1)
`reg_gpr		ex2RegIdCn1;		//Destination ID (CR, EX1)
reg[63:0]		ex2RegValCn1;		//Destination Value (CR, EX1)
	
`wire_gpr		ex2RegIdRn2;		//Destination ID (EX1)
wire[63:0]		ex2RegValRn2;		//Destination Value (EX1)
//wire[4:0]		ex2RegIdCn2;		//Destination ID (CR, EX1)
`wire_gpr		ex2RegIdCn2;		//Destination ID (CR, EX1)
wire[63:0]		ex2RegValCn2;		//Destination Value (CR, EX1)
	
reg[47:0]		ex2RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex2RegValImm;		//Immediate (Decode)
reg[69:0]		ex2RegAluRes;		//Arithmetic Result
reg[63:0]		ex2RegMulRes;		//Multiplier Result
reg[63:0]		ex2RegMulWRes;		//Multiplier Result (Word)

reg[65:0]		ex2RegAluResB;		//Arithmetic Result (ALUB)

reg				ex2BraFlush;		//Flush EX2
reg				ex2TrapFlush;		//Flush EX2

wire[63:0]		ex2RegOutDlr;
reg[63:0]		ex2RegInDlr;
wire[63:0]		ex2RegOutDhr;
reg[63:0]		ex2RegInDhr;
wire[63:0]		ex2RegOutSp;
reg[63:0]		ex2RegInSp;

wire[63:0]		ex2RegOutLr;
reg[63:0]		ex2RegInLr;
wire[63:0]		ex2RegOutSr;

wire[7:0]		ex2RegOutSchm;
reg[7:0]		ex2RegInSchm;
reg[7:0]		ex2RegInLastSr;

McExEX2	ex2(
	clock,			reset,
	ex2OpUCmd,		ex2OpUIxt,
	ex2Hold,

	ex2RegIdRs,		ex2RegIdRt,		ex2RegIdRm,
	ex2RegValRs,	ex2RegValRt,	ex2RegValRm,
	ex2RegValCRm,

	ex2RegIdRn1,	ex2RegValRn1,
	ex2RegIdCn1,	ex2RegValCn1,

	ex2RegIdRn2,	ex2RegValRn2,
	ex2RegIdCn2,	ex2RegValCn2,
	
	ex2RegValPc,	ex2RegValImm,
	ex2RegAluRes,	ex2RegMulRes,
	ex2RegMulWRes,	ex1KrreLo,
	ex2RegAluResB,

	ex1FpuValGRn,
	
	ex2BraFlush,
	ex2RegInLastSr,
	
	ex2RegOutDlr,	ex2RegInDlr,
	ex2RegOutDhr,	ex2RegInDhr,
	ex2RegOutSp,	ex2RegInSp,

	ex2RegOutLr,	ex2RegInLr,
	ex2RegOutSr,	ex2RegInSr,
	ex2RegOutSchm,	ex2RegInSchm,

	ex2MemDataIn,
	ex2MemDataOK
	);


/* EX3 */

`reg_gpr		ewbRegIdRm;		//Destination Register, Writeback
`reg_gpr		ewb2RegIdRm;	//Destination Register, Writeback
`reg_gpr		ewb3RegIdRm;	//Destination Register, Writeback


reg[47:0]		ex3ValBPc;

reg[8:0]		ex3OpUCmd;
reg[8:0]		ex3OpUIxt;
wire[1:0]		ex3Hold;
reg[1:0]		ex3PreBra;
reg[95:0]		ex3IstrWord;	//source instruction word

`reg_gpr		ex3RegIdRs;		//Source A, ALU / Base
`reg_gpr		ex3RegIdRt;		//Source B, ALU / Index
`reg_gpr		ex3RegIdRm;		//Source C, MemStore
reg[63:0]		ex3RegValRs;		//Source A Value
reg[63:0]		ex3RegValRt;		//Source B Value
reg[63:0]		ex3RegValRm;		//Source C Value

`reg_gpr		ex3RegIdRn2;		//Destination ID (EX2)
reg[63:0]		ex3RegValRn2;		//Destination Value (EX2)
`reg_gpr		ex3RegIdCn2;		//Destination ID (CR, EX2)
reg[63:0]		ex3RegValCn2;		//Destination Value (CR, EX2)
	
`wire_gpr		ex3RegIdRn3;		//Destination ID (EX3)
wire[63:0]		ex3RegValRn3;		//Destination Value (EX3)
`wire_gpr		ex3RegIdCn3;		//Destination ID (CR, EX3)
wire[63:0]		ex3RegValCn3;		//Destination Value (CR, EX3)
	
reg[47:0]		ex3RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex3RegValImm;		//Immediate (Decode)
reg[69:0]		ex3RegAluRes;		//Arithmetic Result
reg[63:0]		ex3RegMulRes;		//Multiplier Result
reg[63:0]		ex3RegMulWRes;		//Multiplier Result (Word)
reg				ex3BraFlush;		//Flush EX2
reg				ex3TrapFlush;		//Flush EX2

reg[7:0]		ex3RegInLastSr;

wire[63:0]		ex3RegOutDlr;
reg[63:0]		ex3RegInDlr;
wire[63:0]		ex3RegOutDhr;
reg[63:0]		ex3RegInDhr;
wire[63:0]		ex3RegOutLr;
reg[63:0]		ex3RegInLr;

McExEX3	ex3(
	clock,			reset,
	ex3OpUCmd,		ex3OpUIxt,
	ex3Hold,		exHold2,

	ex3RegIdRs,		ex3RegIdRt,		ex3RegIdRm,
	ex3RegValRs,	ex3RegValRt,	ex3RegValRm,

	ex3RegIdRn2,	ex3RegValRn2,
	ex3RegIdCn2,	ex3RegValCn2,

	ex3RegIdRn3,	ex3RegValRn3,
	ex3RegIdCn3,	ex3RegValCn3,
	
	ex3RegValPc,	ex3RegValImm,
	ex3RegAluRes,	ex3RegMulRes,
	ex3RegMulWRes,	ex1KrreLo,
	ex1FpuValGRn,

	ex3BraFlush,
	ex3RegInLastSr,

	ex2MemDataIn,
	ex2MemDataOK
	);


reg[47:0]		exL4ValBPc;

reg[8:0]		exL4OpUCmd;
reg[8:0]		exL4OpUIxt;
wire[1:0]		exL4Hold;
reg[1:0]		exL4PreBra;
// reg[31:0]	exL4IstrWord;	//source instruction word
reg[95:0]		exL4IstrWord;	//source instruction word

`reg_gpr		exL4RegIdRs;		//Source A, ALU / Base
`reg_gpr		exL4RegIdRt;		//Source B, ALU / Index
`reg_gpr		exL4RegIdRm;		//Source C, MemStore
reg[63:0]		exL4RegValRs;		//Source A Value
reg[63:0]		exL4RegValRt;		//Source B Value
reg[63:0]		exL4RegValRm;		//Source C Value

`reg_gpr		exL4RegIdRn2;		//Destination ID (EX2)
reg[63:0]		exL4RegValRn2;		//Destination Value (EX2)
`reg_gpr		exL4RegIdCn2;		//Destination ID (CR, EX2)
reg[63:0]		exL4RegValCn2;		//Destination Value (CR, EX2)
		
reg[47:0]		exL4RegValPc;		//PC Value (Synthesized)
reg[32:0]		exL4RegValImm;		//Immediate (Decode)
reg				exL4BraFlush;		//Flush EX2
reg				exL4TrapFlush;		//Flush EX2

reg[31:0]		exL4RegInSr;

reg[63:0]		exL4RegInDlr;
reg[63:0]		exL4RegInDhr;
reg[63:0]		exL4RegInLr;
reg[63:0]		exL4RegInSp;


reg[31:0]	tValStepPc;

reg[47:0]	tValNextPc;
reg[47:0]	tValNextPcHi;
reg[47:0]	tOpNextPc;
reg[47:0]	tIsrNextPc;

reg[47:0]	tValBraPc;
reg[47:0]	tValBraPcHi;
reg[47:0]	tValNextBraPc;
reg[47:0]	tValNextBraPcHi;
reg[15:0]	tValBraSrT;
reg[15:0]	tValNextBraSrT;
reg[47:0]	tIsrBraPc;

reg[7:0]	opBraFlushMask;
reg[7:0]	nxtBraFlushMask;
reg[7:0]	tIsrBraFlushMask;

reg[7:0]	tRegExcOfs;

reg[127:0]	tDelayExc;
reg[127:0]	tNxtDelayExc;

reg[127:0]	tDelay2Exc;
reg[127:0]	tDelay3Exc;
reg[127:0]	tDelay4Exc;
reg[127:0]	tNxtDelay2Exc;
reg[127:0]	tNxtDelay3Exc;
reg[127:0]	tNxtDelay4Exc;

reg			tDelay2ExcAdv;
reg			tDelay3ExcAdv;
reg			tDelay4ExcAdv;

reg			tPreHold1;
reg			tNxtPreHold1;
reg			tLstPreHold1;
reg			tRegSkipExc;

`ifdef jx2_debug_hitmiss
reg[15:0]	tBraHitMiss;
reg[15:0]	tBraNxtHitMiss;

reg[15:0]	tBraCycCnt;
reg[15:0]	tBraNxtCycCnt;
`endif

reg[15:0]	tHoldCycCnt;
reg[15:0]	tHoldNxtCycCnt;

reg			tExcLatch;
reg			tNxtExcLatch;

reg[ 5:0]	tValAddPc0;
reg[26:0]	tValPcAdd1;

reg[47:0]		braInSpc;
reg[63:0]		braInExsr;
reg[63:0]		braInTea;
reg[63:0]		braInTeaHi;
reg[63:0]		braInSr;
reg[63:0]		braInLr;
reg[63:0]		braInDlr;
reg[63:0]		braInDhr;
reg[47:0]		braInSp;
reg[47:0]		braInSsp;
reg				braIsIsr;

reg[47:0]		braIsrSp;

reg[47:0]		braNxtInSpc;
reg[63:0]		braNxtInExsr;
reg[63:0]		braNxtInTea;
reg[63:0]		braNxtInTeaHi;
reg[63:0]		braNxtInSr;
reg[63:0]		braNxtInLr;
reg[63:0]		braNxtInDlr;
reg[63:0]		braNxtInDhr;
reg[47:0]		braNxtInSp;
reg[47:0]		braNxtInSsp;
reg				braNxtIsIsr;

assign		id2RegValSp		=
			(ex1RegIdRn1==JX2_GR_SP) ?
			ex1RegValRn1[47:0] :
			(ex2RegIdRn2==JX2_GR_SP) ?
			ex2RegValRn2[47:0] :
			(ex3RegIdRn2==JX2_GR_SP) ?
			ex3RegValRn2[47:0] :
			gprOutSp[47:0];

assign		id2RegValLr		=
			(ex1RegIdRn1==JX2_GR_LR) ?
			ex1RegValRn1[63:0] :
			(ex2RegIdRn2==JX2_GR_LR) ?
			ex2RegValRn2[63:0] :
			(ex3RegIdRn2==JX2_GR_LR) ?
			ex3RegValRn2[63:0] :
			crOutLr[63:0];


always @*
begin
	exHold1			= 0;
	exHold1A		= 0;
	exHold1B		= 0;
	exHold1C		= 0;
	exHold1D		= 0;

	exHold1B1		= 0;
	exHold1B2		= 0;
	exHold1B3		= 0;
	exHold1C1		= 0;
	exHold1C2		= 0;
	exHold1C3		= 0;

	exHold2			= 0;
	tNxtRegExc		= 0;
	tRegExcOfs		= 0;
	tNxtPreHold1	= 0;
	
	tDbgOutStatus1	= 0;
	tDbgOutStatus2	= 0;
	tDbgOutStatus3	= 0;
	tDbgOutStatus4	= 0;

	tDbgOutStatus5	= 0;
	tDbgOutStatus6	= 0;
	tDbgOutStatus7	= 0;
	tDbgOutStatus8	= 0;

	tHoldNxtCycCnt		= 0;
	tNxtDeadlockLatch	= tDeadlockLatch;
//	tNxtExcLatch		= tExcLatch;

	ifNxtValBraOk	= 0;

	crInExsr		= crOutExsr;
	crInSpc			= crOutSpc;
	crInSsp			= crOutSsp;
	crInTea			= crOutTea;
	crInTeaHi		= crOutTeaHi;

	tValNextBraPc	= UV48_00;
	tValNextBraPcHi	= UV48_00;
	tValNextBraSrT	= {
		crOutSr[15: 4],
		crOutSr[27:26],
		crOutSr[ 1: 0] };


	ex1TrapFlush	= 0;
	ex2TrapFlush	= 0;
	ex3TrapFlush	= 0;

	gprEx1DualLane	= (ex1OpUIxt[7:6] == 2'b11);
	gprEx2DualLane	= (ex2OpUIxt[7:6] == 2'b11);
	gprEx3DualLane	= (ex3OpUIxt[7:6] == 2'b11);


`ifdef jx2_debug_hitmiss
	tBraNxtHitMiss	= tBraHitMiss;
//	tBraNxtCycCnt	= tBraCycCnt + 1;
	tBraNxtCycCnt	= tBraCycCnt;
`endif

	exHold2	=
		(ex1Hold[0])	||	(ex2Hold[0])	||
		(ex3Hold[0])	||	dcOutHold		||
		(ifOutPcOK[1])	||	(ex1FpuOK[1]);

	exHold1A	= exHold2;

	exHold1B1	=
		(	(ex1RegIdRm == gprIdRs) ||
			(ex1RegIdRm == gprIdRt) ||
			(ex1RegIdRm == gprIdRm)	) &&
		ex1Hold[1];

	exHold1C1	=
		(	(ex2RegIdRm == gprIdRs) ||
			(ex2RegIdRm == gprIdRt) ||
			(ex2RegIdRm == gprIdRm)	) &&
		ex2Hold[1];

	exHold1B	=
		(exHold1B1 && !ex1BraFlush) ||
		(exHold1C1 && !ex2BraFlush) ;


	exHold1C = 0;

	if(	((ex1OpUCmd[5:0]==JX2_UCMD_MOV_MR) ||
		 (ex1OpUCmd[5:0]==JX2_UCMD_MOV_RM))	&&
		((id2IdUCmd[5:0]==JX2_UCMD_JMP) ||
		 (id2IdUCmd[5:0]==JX2_UCMD_JSR) ||
		 (id2IdUCmd[5:0]==JX2_UCMD_BRA) ||
		 (id2IdUCmd[5:0]==JX2_UCMD_BSR)))
			exHold1C = 1;

`ifdef jx2_enable_xgpr

`ifndef def_true
	if( (ex1RegIdCn1 != JX2_CR_ZZR) &&
			(ex1RegIdCn1 != JX2_CR_PC) &&
			(ex1RegIdCn1[6:5] == 2'b11))
		exHold1D = 1;

	if( (ex2RegIdCn2 != JX2_CR_ZZR) &&
			(ex2RegIdCn2 != JX2_CR_PC) &&
			(ex2RegIdCn2[6:5] == 2'b11))
		exHold1D = 1;

	if( (ex3RegIdCn3 != JX2_CR_ZZR) &&
			(ex3RegIdCn3 != JX2_CR_PC) &&
			(ex3RegIdCn3[6:5] == 2'b11))
		exHold1D = 1;
`endif

`else

	if( (ex1RegIdCn1 != JX2_CR_ZZR) &&
			(ex1RegIdCn1 != JX2_CR_PC) &&
			(ex1RegIdCn1[5:4] == 2'b11))
		exHold1D = 1;

	if( (ex2RegIdCn2 != JX2_CR_ZZR) &&
			(ex2RegIdCn2 != JX2_CR_PC) &&
			(ex2RegIdCn2[5:4] == 2'b11))
		exHold1D = 1;

	if( (ex3RegIdCn3 != JX2_CR_ZZR) &&
			(ex3RegIdCn3 != JX2_CR_PC) &&
			(ex3RegIdCn3[5:4] == 2'b11))
		exHold1D = 1;

`endif

	exHold1		= exHold1A || exHold1B || exHold1C || exHold1D;

	tDbgOutStatus1	= exHold1A;
	tDbgOutStatus2	= exHold1B;
	tDbgOutStatus3	= exHold1C;
	tDbgOutStatus4	= exHold1D;

	tDbgOutStatus5	= ifOutPcOK[1] || (ifOutPcStep==0);
	tDbgOutStatus6	= dcOutOK[1];
	tDbgOutStatus7	= dcBusWait;
	tDbgOutStatus8	= memOpm[7:0] != 0;

`ifdef jx2_debug_expipe
	if(exHold1 &&
		!((ex1OpUCmd[5:0]==JX2_UCMD_OP_IXT) && 
			(ex1OpUIxt[5:0]==JX2_UCIX_IXT_BREAK)))
	begin
		$display("ExHold: A=%d B=%d C=%d D=%d",
			exHold1A, exHold1B, exHold1C, exHold1D);
	
		if(exHold2)
		begin
			$display("ExHold2: Ex1=%d Ex2=%d I$=%d FPU=%d D$=%d",
				ex1Hold, ex2Hold, ifOutPcOK[1], ex1FpuOK[1], dcOutHold);
			$display("\tEX1=%d EX2=%d EX3=%d",
				(ex1Hold[0]), (ex2Hold[0]), (ex3Hold[0]));
		end
	end
`endif

`ifdef def_true
	if(exHold1)
	begin
		tHoldNxtCycCnt	= tHoldCycCnt + 1;
		if(tHoldCycCnt==65535)
		begin
			tNxtDeadlockLatch	= 1;
			tHoldNxtCycCnt		= 65535;
		end
		else
		if(tHoldCycCnt>65280)
		begin
//			tHoldNxtCycCnt	= tHoldCycCnt;
			tHoldNxtCycCnt	= 65535;
			$display("ExUnit: Deadlock Detected");

			$display("ExUnit: 1A=%d 1B=%d 1C=%d 1D=%d",
				exHold1A, exHold1B, exHold1C, exHold1D);

			$display("ExUnit: 1B1=%d 1B2=%d 1B3=%d",
				exHold1B1, exHold1B2, exHold1B3);

			$display("ExUnit: 1C1=%d 1C2=%d 1C3=%d",
				exHold1C1, exHold1C2, exHold1C3);

			$display("ExUnit: EX1=%d EX2=%d EX3=%d",
				ex1Hold[0], ex2Hold[0], ex3Hold[0]);
			$display("ExUnit: D$=%d I$=%d FPU=%d",
				dcOutHold, ifOutPcOK[1], ex1FpuOK[1]);

			$display("ID1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
				id1ValBPc,	id1ValPc,
				id1IstrWord[15: 0], id1IstrWord[31:16],
				id1IdUCmd, id1IdUIxt, opBraFlushMask[2]);
			$display("     Rs=%X Rt=%X Rn=%X",
				id1IdRegM, id1IdRegO, id1IdRegN);

			$display("ID2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
				id2ValBPc,	gprValPc,
				id2IstrWord[15: 0], id2IstrWord[31:16],
				id2IdUCmd, id2IdUIxt, opBraFlushMask[1]);
			$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
				gprIdRs, gprValRs,
				gprIdRt, gprValRt,
				gprIdRm, gprValRm);

			$display("EX1: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				ex1ValBPc,	ex1RegValPc,
				ex1IstrWord[15: 0], ex1IstrWord[31:16],
				ex1IstrWord[47:32], ex1IstrWord[63:48],
				ex1IstrWord[79:64], ex1IstrWord[95:80],
				ex1OpUCmd, ex1OpUIxt, ex1BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn0=%X(%X)",
				ex1RegIdRs, ex1RegValRs,
				ex1RegIdRt, ex1RegValRt,
				ex1RegIdRm, ex1RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				ex1RegIdRn1, ex1RegValRn1,
				ex1RegIdCn1, ex1RegValCn1);

			$display("EX2: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				ex2ValBPc,	ex2RegValPc,
				ex2IstrWord[15: 0], ex2IstrWord[31:16],
				ex2IstrWord[47:32], ex2IstrWord[63:48],
				ex2IstrWord[79:64], ex2IstrWord[95:80],
				ex2OpUCmd, ex2OpUIxt, ex2BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				ex2RegIdRs, ex2RegValRs,
				ex2RegIdRt, ex2RegValRt,
				ex2RegIdRm, ex2RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				ex2RegIdRn2, ex2RegValRn2,
				ex2RegIdCn2, ex2RegValCn2);

			$display("EX3: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				ex3ValBPc,	ex3RegValPc,
				ex3IstrWord[15: 0], ex3IstrWord[31:16],
				ex3IstrWord[47:32], ex3IstrWord[63:48],
				ex3IstrWord[79:64], ex3IstrWord[95:80],
				ex3OpUCmd, ex3OpUIxt, ex3BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				ex3RegIdRs, ex3RegValRs,
				ex3RegIdRt, ex3RegValRt,
				ex3RegIdRm, ex3RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				ex3RegIdRn3, ex3RegValRn3,
				ex3RegIdCn3, ex3RegValCn3);

			$display("");

		end
	end
`endif

	tNxtDelay2Exc	= tDelay2Exc;
	tNxtDelay3Exc	= tDelay3Exc;
	tNxtDelay4Exc	= tDelay4Exc;
	
	tDelay2ExcAdv	= 0;
	tDelay3ExcAdv	= 0;
//	tDelay4ExcAdv	= 0;

// `ifndef def_true
`ifdef def_true
	if((tDelay2Exc[15:12]==4'hA) && (tRegExc[15:12]==4'hA))
	begin
		if(tDelay2Exc[37:30]==tRegExc[37:30])
		begin
			tDelay2ExcAdv	= 1;
			tDelay3ExcAdv	= 1;
		end
	end
	else
//	if(tDelay2Exc[15] && !tRegExc[15])
	if(tDelay2Exc[15] && !tRegExc[15] && !crOutSr[28])
	begin
		tNxtRegExc		= tDelay2Exc;
		tDelay2ExcAdv	= 1;
		tDelay3ExcAdv	= 1;
	end
	else	if(tDelay3Exc[15] && !tDelay2Exc[15])
	begin
		tDelay2ExcAdv	= 1;
		tDelay3ExcAdv	= 1;
	end
	else	if(tDelay4Exc[15] && !tDelay3Exc[15])
	begin
		tDelay3ExcAdv	= 1;
	end
`endif

	
	if(tDelay2ExcAdv)
	begin
		tNxtDelay2Exc	= tNxtDelay3Exc;
	end
	if(tDelay3ExcAdv)
	begin
		tNxtDelay3Exc	= tNxtDelay4Exc;
		tNxtDelay4Exc	= 0;
	end

//	if(tDelayExc[15])
	if(tDelayExc[15] && !tRegExc[15])
	begin
//		tNxtRegExc = tDelayExc;

		if(tDelayExc[15:12]==4'h8)
		begin
			tNxtRegExc = tDelayExc;
		end
		else if(!crOutSr[28])
		begin
			if((tDelayExc[15:12]!=4'hC) || crOutSr[3])
				tNxtRegExc = tDelayExc;
		end
	end

//	if(memRegExc[15])
	if(memRegExc[15] && !tRegExc[15])
	begin
//		$display("memRegExc: %X", memRegExc);
//		tNxtRegExc = memRegExc;

		if(memRegExc[15:12]==4'h8)
		begin
			tNxtRegExc = memRegExc;
		end
		else if(!crOutSr[28])
		begin
			if((memRegExc[15:12]!=4'hC) || crOutSr[3])
				tNxtRegExc = memRegExc;
		end
// `ifndef def_true
`ifdef def_true
		else
		begin
			if((memRegExc[15:12]!=4'hC) || crOutSr[3])
			begin
//				if(!tDelay2Exc[15])
//					tNxtDelay2Exc = memRegExc;

				if(!tDelay4Exc[15])
					tNxtDelay4Exc = memRegExc;
			end
		end
`endif
	end
	else if(memRegExc[15])
	begin
		if(memRegExc[15:12]==4'h8)
		begin
//			if(!tDelay2Exc[15])
//				tNxtDelay2Exc = memRegExc;

//			tNxtDelay2Exc = memRegExc;
			if(!tDelay4Exc[15])
				tNxtDelay4Exc = memRegExc;
		end
//		else if(!crOutSr[28])
		else
		begin
			if((memRegExc[15:12]!=4'hC) || crOutSr[3])
			begin
//				if(!tDelay2Exc[15])
//					tNxtDelay2Exc = memRegExc;
//				tNxtDelay2Exc = memRegExc;
				if(!tDelay4Exc[15])
					tNxtDelay4Exc = memRegExc;
			end
		end
	end

	if(ex1TrapExc[15] && !tRegExc[15])
		tNxtRegExc = ex1TrapExc;

	if(reset)
	begin
		exHold1		= 0;
		exHold2		= 0;
		tNxtDeadlockLatch	= 0;
	end

	if(tNxtRegExc[15])
	begin
//		$display("tNxtRegExc %X", tNxtRegExc);
	end

	if(tRegExc[15])
	begin
//		$display("tRegExc %X", tRegExc);

		if(!tNxtRegExc[15] && !ifValBraOk)
		begin
//			$display("EXC Sustain %X", tRegExc);
			tNxtRegExc = tRegExc;
		end
	end

	if(tNxtRegExc[15])
	begin
		if(crOutSr[28] && crOutSr[29] &&
			(tNxtRegExc[15:12]==4'hA) && ifValBraOk)
		begin
//			$display("EXC Filter %X", tNxtRegExc);
//			tNxtRegExc = 0;
			tNxtRegExc[15] = 0;
		end
	end

	if(reset)
	begin
//		tNxtRegExc = 0;
		tNxtRegExc[15] = 0;

		tNxtDelayExc[15] = 0;
		tNxtDelay2Exc[15] = 0;
		tNxtDelay3Exc[15] = 0;
		tNxtDelay4Exc[15] = 0;
	end

	ifInPcHold	= exHold1;
	dcInHold	= exHold2;

//	exHold1		= 0;
//	exHold2		= 0;

	/* Advance PC */
	tValStepPc		= { 28'b0, ifOutPcStep };

	tValAddPc0		= {2'b0, ifOutPcStep} + { 1'b0, ifLastPc[4:0] };
	tValPcAdd1		= ifLastPc[31:5] + 1;
	tValNextPc		= { ifLastPc[47:32],
		tValAddPc0[5] ? tValPcAdd1 : ifLastPc[31:5],
		tValAddPc0[4:0] };

	tOpNextPc		= tValNextPc;
	/* Hold current PC if branching. */
	if(opBraFlushMask[3])
		tValNextPc		= ifLastPc;

	tIsrNextPc		= tValNextPc;

	nxtBraFlushMask	= { 1'b0, opBraFlushMask[7:1] };

`ifdef jx2_enable_prebra
	if((id1PreBra!=0) && !opBraFlushMask[2])		//ID1
	begin
		tValNextPc = id1PreBraPc;
`ifdef jx2_bra2stage
		nxtBraFlushMask[2] = 1;
`else
		nxtBraFlushMask[2] = 1;
`endif
	end
`endif

	ex1ValBraDir = 3'b001;
	if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && ex1OpUCmd[7] &&
		!ex1BraFlush && !exHold2)
	begin
		ex1ValBraDir[1:0] = ex1OpUCmd[7:6];
		ex1ValBraDir[2]	= (ex1OpUCmd[6]^ex2RegOutSr[0]);

`ifdef jx2_debug_hitmiss
//		if({1'b1, ex1RegIdCn1} == JX2_CR_PC)
		if(ex1RegIdCn1 == JX2_CR_PC)
		begin
			if(tBraHitMiss!=UV16_00)
				tBraNxtHitMiss = tBraHitMiss - 1;
		end
		else
		begin
			if(tBraHitMiss!=UV16_FF)
				tBraNxtHitMiss = tBraHitMiss + 1;
		end

		tBraNxtCycCnt	= tBraCycCnt + 1;
`endif

	end

`ifdef jx2_debug_hitmiss
	if(tBraCycCnt[15]!=tBraNxtCycCnt[15])
//	if(tBraCycCnt[12]!=tBraNxtCycCnt[12])
	begin
		$display("ExUnit: Branch Hit/Miss Count (Mid=8000): %X",
			tBraHitMiss);
		tBraNxtHitMiss	= 16'h8000;
	end
`endif

	/* Handle if EX1 unit has initiated a branch. */
	if(ex1RegIdCn1 == JX2_CR_PC)
	begin
		tValNextBraPc = ex1RegValCn1[47:0];
		tValNextBraSrT = ex1RegValCn1[63:48];
		nxtBraFlushMask = JX2_BRA_FLUSHMSK;

		if(ex1BraFlush)
		begin
			$display("EX1 Invalid BRA %X", ex1RegValCn1);
		end
	end

	if(reset)
	begin
		tValNextBraPc = UV48_00;
		nxtBraFlushMask = JX2_BRA_FLUSHMSK;
		tNxtDeadlockLatch	= 0;
	end
	
	case(tRegExc[14:13])
		2'b00: tRegExcOfs=1;
		2'b01: tRegExcOfs=3;
		2'b10: tRegExcOfs=2;
		2'b11: tRegExcOfs=4;
	endcase

//	crInSr			= ex1RegOutSr;
	crInSr			= ex2RegOutSr;
	gprInSp			= ex1RegOutSp;
	crInLr			= ex1RegOutLr;

	gprInDlr		= ex1RegOutDlr;
	gprInDhr		= ex1RegOutDhr;

	tIsrBraPc			= tValNextBraPc;
	tIsrBraFlushMask	= nxtBraFlushMask;

	braNxtInSpc		= crOutSpc;
	braNxtInExsr	= crOutExsr;
	braNxtInTea		= crOutTea;
	braNxtInTeaHi	= crOutTeaHi;
	braNxtInSr		= ex1RegOutSr;
	braNxtInLr		= ex1RegOutLr;
	braNxtInDlr		= ex1RegOutDlr;
	braNxtInDhr		= ex1RegOutDhr;
	braNxtInSp		= ex1RegOutSp[47:0];
	braNxtInSsp		= crOutSsp;
	braNxtIsIsr		= 0;

	tNxtExcLatch=0;
	if(tRegExc[15])
	begin
//		$display("tRegExc: %X l=%d", tRegExc, tExcLatch);
		if(!exHold2)
			tNxtExcLatch=1;
	end

	tNxtDelayExc	= 0;
	tRegSkipExc		= 0;
	if(tRegExc[15:12]==4'hC)
	begin
		if(crOutSr[28])
			tRegSkipExc		= 1;
		if(exHold1 || exHold2)
		begin
			tRegSkipExc		= 1;
			tNxtDelayExc	= tRegExc;
		end
	end
	else
	begin
		if(tRegExc[15] && tExcLatch &&
			(tRegExc[15:12] != 4'hF) &&
			(tRegExc[15:12] != 4'hA))
		begin
//			tRegSkipExc		= 1;
		end

		if(crOutSr[28] && crOutSr[29] &&
			(tRegExc[15:12]==4'hA) && ifValBraOk)
		begin
			tRegSkipExc		= 1;
		end
	end
	
//	if(tRegExc[15] && !crOutSr[28])
	if(tRegExc[15] && !tRegSkipExc)
	begin
		if(tRegExc[15:10]==6'b111111)
		begin
			$display("ExUnit: RTE, SPC=%X", crOutSpc);
			tValNextBraPc	= crOutSpc;
			nxtBraFlushMask = JX2_BRA_FLUSHMSK;

`ifndef jx2_isr2stage
			crInExsr		= crOutExsr;
			crInSr			= { crOutSr[63:32], crOutExsr[63:32] };
`endif

//			if(crOutSr[29])
			if(crOutSr[29] && crOutSr[30])
			begin
`ifndef jx2_isr2stage
				gprInSp			= { UV16_00, crOutSsp };
				crInSsp			= ex1RegOutSp[47:0];
`endif

`ifdef jx2_isr2stage
				braNxtInSp		= crOutSsp;
				braNxtInSsp		= ex1RegOutSp[47:0];
`endif
			end

`ifdef jx2_isr2stage
			braNxtInExsr	= crOutExsr;
//			braNxtInTea		= crOutTea;
			braNxtInSr		= { crOutSr[63:32], crOutExsr[63:32] };
//			braNxtInLr		= ex1RegOutLr;
//			braNxtInDlr		= ex1RegOutDlr;
//			braNxtInDhr		= ex1RegOutDhr;
			braNxtIsIsr		= 1;
`endif
		end
		else if(!crOutSr[28] || !ifValBraOk)
		begin
// `ifndef def_true
`ifdef def_true
			if(tRegExc[15:12]!=4'b1010)
			begin
				$display("ExUnit: Fault %X, id2.PC=%X ex1.PC=%X ex2.PC=%X",
					tRegExc, id2ValBPc, ex1ValBPc, ex2ValBPc);
			end
`endif
		
			tValNextBraPc = {
				crOutVbr[47:8],
				crOutVbr[7: 3]+tRegExcOfs[4:0],
				crOutVbr[2:0] };
			nxtBraFlushMask = JX2_BRA_FLUSHMSK;

			braIsrSp	= ex1RegOutSp[47:0];

			if(tRegExc[15:12]!=4'b1010)
			begin

// `ifndef def_true
`ifdef def_true
				if(crOutSr[30] && (crOutSr[29] || crOutSr[28]))
				begin
					$display("Fault, RB Set, SPC=%X SR=",
						crOutSpc, crOutSr);
				end
`endif

				if(!crOutSr[28])
				begin
					$display("ISR VBR=%X PC=%X, SPC=%X",
						crOutVbr, tValNextBraPc, crInSpc);
				end
			end

			if(!crOutSr[28])
//			if(!crOutSr[28] && !braIsIsr)
			begin

`ifndef jx2_isr2stage
				$display("ISR Top 1");
				crInExsr		= { crOutSr[31:8], ex2RegOutSr[7:0],
					UV16_00, tRegExc[15:0] };
				crInTea			= { UV16_00, tRegExc[63:16] };
				crInTeaHi		= { tRegExc[127:64] };
				crInSr			= ex1RegOutSr;
				crInSr[30:28]	= 3'b111;
				crInSr[27:26]	= 2'b00;

				crInLr			= crOutLr;
//					gprInDlr		= gprOutDlr;
//					gprInDhr		= gprOutDhr;
`endif

`ifdef jx2_isr2stage
				braNxtInExsr		= {
					crOutSr[31:8], ex2RegOutSr[7:0],
					UV16_00, tRegExc[15:0] };
				braNxtInTea			=  { UV16_00, tRegExc[63:16] };
				braNxtInTeaHi		=  { tRegExc[127:64] };
				braNxtInSr			= ex1RegOutSr;
				braNxtInSr[30:28]	= 3'b111;
				braNxtInSr[27:26]	= 2'b00;
				braNxtInLr			= crOutLr;
				braNxtIsIsr			= 1;
`endif

// `ifndef def_true
`ifdef def_true
				if(!ex3BraFlush)
				begin
					$display("ISR from EX3");

`ifndef jx2_isr2stage
					crInSpc			= ex3ValBPc;
					crInExsr[39:32]	= ex3RegInLastSr;
					crInLr			= ex3RegInLr;
//					crInLr			= ex3RegValLr;
					gprInDlr		= gprOutDlr;
					gprInDhr		= gprOutDhr;
					braIsrSp		= gprOutSp[47:0];
//					braIsrSp		= ex3RegValSp;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ex3ValBPc;
					braNxtInExsr[39:32]	= ex3RegInLastSr;
					braNxtInLr			= ex3RegInLr;
					braNxtInDlr			= gprOutDlr;
					braNxtInDhr			= gprOutDhr;
					braIsrSp			= gprOutSp[47:0];
`endif

					ex3TrapFlush	= 1;
					ex2TrapFlush	= 1;
					ex1TrapFlush	= 1;

				end
				else
`endif
//`ifndef def_true
`ifdef def_true
				if(!ex2BraFlush)
				begin
					$display("ISR from EX2");
`ifndef jx2_isr2stage
					crInSpc			= ex2ValBPc;
					crInExsr[39:32]	= ex2RegInLastSr;
					crInLr			= ex2RegInLr;
//					crInLr			= ex2RegValLr;
					gprInDlr		= gprOutDlr;
					gprInDhr		= gprOutDhr;
					braIsrSp		= gprOutSp[47:0];
//					braIsrSp		= ex2RegValSp;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ex2ValBPc;
					braNxtInExsr[39:32]	= ex2RegInLastSr;
					braNxtInLr			= ex2RegInLr;
					braNxtInDlr			= gprOutDlr;
					braNxtInDhr			= gprOutDhr;
					braIsrSp			= gprOutSp[47:0];
`endif

					ex2TrapFlush	= 1;
					ex1TrapFlush	= 1;

				end
				else
`endif
				if(!ex1BraFlush)
				begin
					$display("ISR from EX1");

`ifndef jx2_isr2stage
					crInSpc			= ex1ValBPc;
					crInExsr[39:32]	= ex2RegOutSr[7:0];
//					crInLr			= ex1RegInLr;
//					crInLr			= crOutLr;
					gprInDlr		= gprOutDlr;
					gprInDhr		= gprOutDhr;
//					braIsrSp		= gprOutSp;
//					braIsrSp		= ex2RegOutSp[47:0];
					braIsrSp		= ex1RegValSp;
					crInLr			= ex1RegValLr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ex1ValBPc;
					braNxtInExsr[39:32]	= ex2RegOutSr[7:0];
					braNxtInLr			= crOutLr;
					braNxtInDlr			= gprOutDlr;
					braNxtInDhr			= gprOutDhr;
//					braIsrSp			= gprOutSp;
//					braIsrSp			= ex2RegOutSp;
					braIsrSp			= ex1RegValSp;
`endif

					ex1TrapFlush	= 1;

				end
//				else if(!opBraFlushMask[0])
//				else if(!nxtBraFlushMask[0])
				else if(!tIsrBraFlushMask[0])
				begin
					$display("ISR from ID2");

`ifndef jx2_isr2stage
					crInSpc			= id2ValBPc;
//					crInExsr[39:32]	= crOutSr[7:0];
					crInExsr[39:32]	= ex2RegOutSr[7:0];
//					crInLr			= crOutLr;
//					crInLr			= ex1RegOutLr;
//					gprInDlr		= gprOutDlr;
//					gprInDhr		= gprOutDhr;
					gprInDlr		= ex1RegOutDlr;
					gprInDhr		= ex1RegOutDhr;
//					braIsrSp		= ex1RegOutSp[47:0];
					braIsrSp		= id2RegValSp;
					crInLr			= id2RegValLr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= id2ValBPc;
					braNxtInExsr[39:32]	= ex2RegOutSr[7:0];
//					braNxtInLr			= crOutLr;
					braNxtInLr			= ex1RegOutLr;
//					braNxtInDlr			= gprOutDlr;
//					braNxtInDhr			= gprOutDhr;
					braIsrSp			= ex1RegOutSp[47:0];
					braIsrSp			= id2RegValSp;
`endif

				end
//				else if(!opBraFlushMask[1])
//				else if(!nxtBraFlushMask[1])
				else if(!tIsrBraFlushMask[1])
				begin
//					$display("ISR from ID1, Msk=%X", opBraFlushMask);
					$display("ISR from ID1, Msk=%X/%X",
						nxtBraFlushMask, opBraFlushMask);
					$display("  EX1 PC=%X %X",
						ex1ValBPc, ex1IstrWord);
					$display("  ID2 PC=%X %X",
						id2ValBPc, id2IstrWord);
					$display("  ID1 PC=%X %X",
						id1ValBPc, id1IstrWord);
					$display("  IF PC=%X", ifLastPc);

`ifndef jx2_isr2stage
					crInSpc			= id1ValBPc;
//					crInExsr[39:32]	= crOutSr[7:0];
					crInExsr[39:32]	= ex2RegOutSr[7:0];
//					crInLr			= crOutLr;
//					crInLr			= ex1RegOutLr;
//					gprInDlr		= gprOutDlr;
//					gprInDhr		= gprOutDhr;
					gprInDlr		= ex1RegOutDlr;
					gprInDhr		= ex1RegOutDhr;
//					braIsrSp		= ex1RegOutSp[47:0];
					braIsrSp		= id2RegValSp;
					crInLr			= id2RegValLr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= id1ValBPc;
					braNxtInExsr[39:32]	= ex2RegOutSr[7:0];
//					braNxtInLr			= crOutLr;
					braNxtInLr			= ex1RegOutLr;
					braIsrSp			= ex1RegOutSp[47:0];
					braIsrSp			= id2RegValSp;
`endif
				end

// `ifndef def_true
`ifdef def_true
				else
				begin
//					$display("ISR from IF");
//					$display("ISR from IF, Msk=%X", opBraFlushMask);
					$display("ISR from IF, Msk=%X/%X",
						nxtBraFlushMask, opBraFlushMask);
					$display("  EX1 PC=%X %X",
						ex1ValBPc, ex1IstrWord);
					$display("  ID2 PC=%X %X",
						id2ValBPc, id2IstrWord);
					$display("  ID1 PC=%X %X",
						id1ValBPc, id1IstrWord);
					$display("  IF PC=%X / %X", ifLastPc, tIsrNextPc);

`ifndef jx2_isr2stage
					crInSpc			= ifLastPc;
//					crInExsr[39:32]	= crOutSr[7:0];
					crInExsr[39:32]	= ex2RegOutSr[7:0];
//					crInLr			= crOutLr;
//					crInLr			= ex1RegOutLr;
//					gprInDlr		= gprOutDlr;
//					gprInDhr		= gprOutDhr;
					gprInDlr		= ex1RegOutDlr;
					gprInDhr		= ex1RegOutDhr;
					braIsrSp		= ex1RegOutSp[47:0];
					braIsrSp		= id2RegValSp;
					crInLr			= id2RegValLr;

//					tIsrBraPc			= tValNextBraPc;
//					tIsrBraFlushMask	= nxtBraFlushMask;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ifLastPc;
//					braNxtInExsr[39:32]	= crOutSr[7:0];
					braNxtInExsr[39:32]	= ex2RegOutSr[7:0];
//					braNxtInLr			= crOutLr;
					braNxtInLr			= ex1RegOutLr;
					braIsrSp			= ex1RegOutSp[47:0];
					braIsrSp			= id2RegValSp;
`endif

`ifdef jx2_bra2stage
					if(opBraFlushMask[4])
					begin
						$display("ISR In Branch %X", tIsrBraPc);
					end
`else
					if(tIsrBraFlushMask[3])
					begin
						$display("ISR In Branch %X", tIsrBraPc);
					end
`endif

				end
`endif

//				$display("ISR VBR=%X PC=%X, SPC=%X",
//					crOutVbr, tValNextBraPc, crInSpc);

				if(tRegExc[15:12]==4'b1110)
				begin
`ifndef jx2_isr2stage
	//				crInSpc			= id1ValBPc;
					crInSpc			= ex1ValBPc;
`else
					braNxtInSpc		= ex1ValBPc;
`endif
				end

//				if(!crOutSr[29])
				if(!(crOutSr[29] && crOutSr[30]))
				begin
`ifndef jx2_isr2stage
//					crInSsp			= ex1RegOutSp[47:0];
					crInSsp			= braIsrSp[47:0];
					gprInSp			= { UV16_00, crOutSsp };
`else
//					braNxtInSsp		= ex1RegOutSp[47:0];
					braNxtInSsp		= braIsrSp[47:0];
					braNxtInSp		= crOutSsp;
`endif
				end

			end
		end
		else
		begin
`ifndef def_true
			$display("ExUnit: Double-Fault EXC=%X, id2.PC=%X ex1.PC=%X ex2.PC=%X",
				tRegExc, id2ValBPc, ex1ValBPc, ex2ValBPc);
`endif
		end
	end


`ifdef jx2_isr2stage

// `ifndef def_true
`ifdef def_true
	if(braIsIsr)
	begin
		crInSpc		= braInSpc;
		crInExsr	= braInExsr;
		crInTea		= braInTea;
		crInTeaHi	= braInTeaHi;
		crInSr		= braInSr;
		crInLr		= braInLr;
		gprInDlr	= braInDlr;
		gprInDhr	= braInDhr;
		gprInSp		= { UV16_00, braInSp };
		crInSsp		= braInSsp;
	end
`endif

// `ifdef def_true
`ifndef def_true
	if(braNxtIsIsr)
	begin
		crInSpc		= braNxtInSpc;
		crInExsr	= braNxtInExsr;
		crInTea		= braNxtInTea;
		crInTeaHi	= braNxtInTeaHi;
		crInSr		= braNxtInSr;
		crInLr		= braNxtInLr;
		gprInDlr	= braNxtInDlr;
		gprInDhr	= braNxtInDhr;
		gprInSp		= { UV16_00, braNxtInSp };
		crInSsp		= braNxtInSsp;
	end
`endif

`endif


`ifdef jx2_bra2stage
	if(opBraFlushMask[4])
	begin
//		if(crOutSr[28])
//			$display("Branch %X", tValNextBraPc);
		tValNextPc		= tValBraPc;
		tValNextPcHi	= tValBraPcHi;
		
		ifNxtValBraOk = 1;
	end
`else
	if(nxtBraFlushMask[3])
	begin
//		if(crOutSr[28])
//			$display("Branch %X", tValNextBraPc);
		tValNextPc		= tValNextBraPc;
		tValNextPcHi	= tValNextBraPcHi;
		ifNxtValBraOk	= 1;
	end
`endif

	ifValPc			= tValNextPc;
//	ifValPcHi		= tValNextPcHi;

	crInPc			= tValNextPc;

	gprIdRn1		= ex1RegIdRn1;
	gprValRn1		= ex1RegValRn1;
	gprIdRn2		= ex2RegIdRn2;
	gprValRn2		= ex2RegValRn2;
	gprIdRn3		= ex3RegIdRn3;
	gprValRn3		= ex3RegValRn3;

//`ifndef def_true
`ifndef jx2_gprs_mergecm
	crIdCn1			= ex1RegIdCn1;
	crValCn1		= ex1RegValCn1;
	crIdCn2			= ex2RegIdCn2;
	crValCn2		= ex2RegValCn2;
	crIdCn3			= ex3RegIdCn3;
	crValCn3		= ex3RegValCn3;
`endif

// `ifdef def_true
`ifdef jx2_gprs_mergecm
	crIdCn1			= ex1RegIdRn1;
	crValCn1		= ex1RegValRn1;
	crIdCn2			= ex2RegIdRn2;
	crValCn2		= ex2RegValRn2;

`ifdef jx2_enable_movc
	crIdCn3			= ex3RegIdRn3;
	crValCn3		= ex3RegValRn3;
`else
	crIdCn3			= ex3RegIdRn2;
	crValCn3		= ex3RegValRn2;
`endif

`endif

	gprEx1Flush = ex1BraFlush || ex1TrapFlush;
	gprEx2Flush = ex2BraFlush || ex2TrapFlush;
	gprEx3Flush = ex3BraFlush || ex3TrapFlush;


	/* ID2 */

	/* EX1 */
	ex1RegInDlr		= gprOutDlr;
	ex1RegInDhr		= gprOutDhr;
	ex1RegInSp		= gprOutSp;
	ex1RegInLr		= crOutLr;

	ex1RegInSr		= ex2RegOutSr;
	ex1RegInSchm	= 0;

	ex2RegAluRes	= ex1ValAlu;
	ex2RegMulRes	= ex1MulVal;
	ex2RegMulWRes	= ex1MulWVal;

	ex3RegMulRes	= ex1MulVal;

	ex3RegAluRes	= 0;
	ex3RegMulWRes	= 0;

	dcInAddr		= ex1MemAddr;
	dcInOpm			= ex1MemOpm;
	dcInVal			= ex1MemDataOut;
	dcInTraPc		= { UV16_00, ex1ValBPc };

	/* EX2 */

	ex2MemDataIn	= dcOutVal;
//	ex2MemDataInB	= dcOutValB;
	ex2MemDataOK	= dcOutOK;

	ex2RegInSp		= gprOutSp;
	ex2RegInSr		= crOutSr;
	ex2RegInSchm	= 0;

end



always @(posedge clock)
begin

	tPreHold1		<= tNxtPreHold1;
	tLstPreHold1	<= tPreHold1;
	exHold1L		<= exHold1;

//	tRegExc			<= tNxtRegExc;
	tDelayExc		<= tNxtDelayExc;
	tDelay2Exc		<= tNxtDelay2Exc;
	tDelay3Exc		<= tNxtDelay3Exc;
	tDelay4Exc		<= tNxtDelay4Exc;

	if(tNxtRegExc[15] && (tNxtRegExc[15:12] != 4'hC))
	begin
		tRegExc		<= tNxtRegExc;
	end

	tHoldCycCnt		<= tHoldNxtCycCnt;
	tDeadlockLatch	<= tNxtDeadlockLatch;
	tExcLatch		<= tNxtExcLatch;

`ifdef jx2_debug_hitmiss
	tBraHitMiss		<= tBraNxtHitMiss;
	tBraCycCnt		<= tBraNxtCycCnt;
`endif

`ifdef def_true
	braInSpc		<= braNxtInSpc;
	braInExsr		<= braNxtInExsr;
	braInTea		<= braNxtInTea;
	braInTeaHi		<= braNxtInTeaHi;
	braInSr			<= braNxtInSr;
	braInLr			<= braNxtInLr;
	braInDlr		<= braNxtInDlr;
	braInDhr		<= braNxtInDhr;
	braInSp			<= braNxtInSp;
	braInSsp		<= braNxtInSsp;
	braIsIsr		<= braNxtIsIsr;
`endif

	if(!exHold1)
	begin
		tRegExc			<= tNxtRegExc;

`ifdef jx2_bra2stage
		tValBraPc		<= tValNextBraPc;
		tValBraPcHi		<= tValNextBraPcHi;
		tValBraSrT		<= tValNextBraSrT;
`endif

`ifndef def_true
		braInSpc		<= braNxtInSpc;
		braInExsr		<= braNxtInExsr;
		braInTea		<= braNxtInTea;
		braInTeaHi		<= braNxtInTeaHi;
		braInSr			<= braNxtInSr;
		braInLr			<= braNxtInLr;
		braInDlr		<= braNxtInDlr;
		braInDhr		<= braNxtInDhr;
		braInSp			<= braNxtInSp;
		braInSsp		<= braNxtInSsp;
		braIsIsr		<= braNxtIsIsr;
`endif

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_expipe
		$display("IF : PC=%X D=%X-%X-%X-%X Step=%X PC2=%X F=%d", ifLastPc,
			ifIstrWord[15: 0], ifIstrWord[31:16],
			ifIstrWord[47:32], ifIstrWord[63:48],
			ifOutPcStep, tValNextPc, opBraFlushMask[3]);

		$display("ID1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			id1ValBPc,	id1ValPc,
			id1IstrWord[15: 0], id1IstrWord[31:16],
			id1IdUCmd, id1IdUIxt, opBraFlushMask[2]);
		$display("     Rs=%X Rt=%X Rn=%X",
			id1IdRegM, id1IdRegO, id1IdRegN);

		$display("ID2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			id2ValBPc,	gprValPc,
			id2IstrWord[15: 0], id2IstrWord[31:16],
			id2IdUCmd, id2IdUIxt, opBraFlushMask[1]);
		$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
			gprIdRs, gprValRs,
			gprIdRt, gprValRt,
			gprIdRm, gprValRm);
//		$display("");
`endif

		/* IF */
		ifLastPc		<= ifValPc;
		ifLastPcStep	<= ifOutPcStep;
		opBraFlushMask	<= nxtBraFlushMask;
		ifValBraOk		<= ifNxtValBraOk;


		/* ID1 */

		id1ValBPc		<= ifLastPc;
		id1ValPc		<= tOpNextPc;
		id1IstrWord		<= ifIstrWord;
		id1IstrSxo		<= ifOutPcSxo;

`ifdef jx2_enable_prebra
		id1BraPipelineLrL	<= {
			2'b00,
			crOutSr[27:26],
			1'b1,
			id1BraPipelineDbl,
			id1BraPipelineR1, id1BraPipelineLr };
`endif

		/* ID2 */

		gprIdRs			<= id1IdRegM;
		gprIdRt			<= id1IdRegO;
		gprIdRm			<= id1IdRegN;
		gprValPc		<= id1ValPc;
		gprValImm		<= id1IdImm;
		gprValImmB		<= id1IdImmB;
//		crIdCm			<= id1IdRegM[4:0];
		crIdCm			<= id1IdRegM;

		id1IstrWordL1	<= nxtBraFlushMask[2] ? UV96_00: id1IstrWord[95:0];
		id1IstrWordL2	<= id1IstrWordL1;

//		id2ValBPc		<= id1ValBPc;
		id2IdUCmd		<= id1IdUCmd;
		id2IdUIxt		<= id1IdUIxt;
		id2PreBraPc		<= id1PreBraPc;
		id2PreBra		<= id1PreBra;

`ifdef jx2_debug_keepinstr
		id2IstrWord		<= id1IstrWord[95:0];
		id2ValBPc		<= id1ValBPc;
`else
		id2IstrWord		<= UV96_XX;
		id2ValBPc		<= UV48_XX;
`endif

		/* EX1 */

`ifdef jx2_debug_keepinstr
		ex1ValBPc		<= id2ValBPc;
		ex1IstrWord		<= id2IstrWord;
`else
		ex1ValBPc		<= UV48_XX;
		ex1IstrWord		<= UV32_XX;
`endif

		ex1RegValSp		<= id2RegValSp;
		ex1RegValLr		<= id2RegValLr;

		ex1PreBraPc		<= id2PreBraPc;
		ex1PreBra		<= exHold1L ? { 1'b1, id2PreBra[0] } : id2PreBra;

		ex1RegValPc		<= gprValPc;
		ex1RegValImm	<= gprValImm;
		ex1BraFlush		<= nxtBraFlushMask[0];
		
		ex1Timers		<= timers[11:0];

		ex1OpUCmd		<= id2IdUCmd;
		ex1OpUIxt		<= id2IdUIxt;

		ex1RegIdRs		<= gprIdRs;
		ex1RegIdRt		<= gprIdRt;
		ex1RegIdRm		<= gprIdRm;
		ex1RegValRs		<= gprValRs;
		ex1RegValRt		<= gprValRt;
		ex1RegValRm		<= gprValRm;

		exB1RegValRs	<= 0;

		ex2RegAluResB	<= 0;

		ex1RegValCRm	<= crValCm;

	end
	else
		if(!exHold2)
	begin
		ex1OpUCmd		<= { JX2_IXC_AL, JX2_UCMD_NOP };
		ex1BraFlush		<= 1;

`ifdef def_true
		ex1ValBPc		<= ex1ValBPc;
		ex1OpUIxt		<= UV9_00;
		ex1PreBraPc		<= 0;
		ex1PreBra		<= 0;
		ex1IstrWord		<= UV96_XX;

		ex1RegValPc		<= UV48_XX;
		ex1RegValImm	<= UV33_XX;

		ex1RegValSp		<= id2RegValSp;
		ex1RegValLr		<= id2RegValLr;

		ex1RegIdRs		<= JX2_GR_ZZR;
		ex1RegIdRt		<= JX2_GR_ZZR;
		ex1RegIdRm		<= JX2_GR_ZZR;
		ex1RegValRs		<= UV64_XX;
		ex1RegValRt		<= UV64_XX;
		ex1RegValRm		<= UV64_XX;

		ex1RegValCRm	<= UV64_XX;
`endif
	end

	if(!exHold2)
	begin

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_expipe
		$display("EX1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex1ValBPc,	ex1RegValPc,
			ex1IstrWord[15: 0], ex1IstrWord[31:16],
			ex1OpUCmd, ex1OpUIxt, ex1BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn0=%X(%X)",
			ex1RegIdRs, ex1RegValRs,
			ex1RegIdRt, ex1RegValRt,
			ex1RegIdRm, ex1RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex1RegIdRn1, ex1RegValRn1,
			ex1RegIdCn1, ex1RegValCn1);

		$display("EX2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex2ValBPc,	ex2RegValPc,
			ex2IstrWord[15: 0], ex2IstrWord[31:16],
			ex2OpUCmd, ex2OpUIxt, ex2BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex2RegIdRs, ex2RegValRs,
			ex2RegIdRt, ex2RegValRt,
			ex2RegIdRm, ex2RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex2RegIdRn2, ex2RegValRn2,
			ex2RegIdCn2, ex2RegValCn2);

		$display("EX3: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex3ValBPc,	ex3RegValPc,
			ex3IstrWord[15: 0], ex3IstrWord[31:16],
			ex3OpUCmd, ex3OpUIxt, ex3BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex3RegIdRs, ex3RegValRs,
			ex3RegIdRt, ex3RegValRt,
			ex3RegIdRm, ex3RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex3RegIdRn3, ex3RegValRn3,
			ex3RegIdCn3, ex3RegValCn3);

		$display("");
`endif

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_exopipe
		$display("EX2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex2ValBPc,	ex2RegValPc,
			ex2IstrWord[15: 0], ex2IstrWord[31:16],
			ex2OpUCmd, ex2OpUIxt, ex2BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex2RegIdRs, ex2RegValRs,
			ex2RegIdRt, ex2RegValRt,
			ex2RegIdRm, ex2RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex2RegIdRn2, ex2RegValRn2,
			ex2RegIdCn2, ex2RegValCn2);

		$display("EX3: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ex3ValBPc,	ex3RegValPc,
			ex3IstrWord[15: 0], ex3IstrWord[31:16],
			ex3OpUCmd, ex3OpUIxt, ex3BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			ex3RegIdRs, ex3RegValRs,
			ex3RegIdRt, ex3RegValRt,
			ex3RegIdRm, ex3RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			ex3RegIdRn3, ex3RegValRn3,
			ex3RegIdCn3, ex3RegValCn3);

//		$display("");
`endif

		/* EX2 */
`ifdef jx2_debug_keepinstr
		ex2IstrWord		<= ex1IstrWord;
		ex2ValBPc		<= ex1ValBPc;
`else
		ex2IstrWord		<= UV32_XX;
		ex2ValBPc		<= UV48_XX;
`endif

//		ex2OpUCmd		<= ex1OpUCmd;
		ex2OpUCmd		<= ex1OpUCmd2;
		ex2OpUIxt		<= ex1OpUIxt;
		ex2PreBraPc		<= ex1PreBraPc;
		ex2PreBra		<= ex1PreBra;
//		ex2BraFlush		<= ex1BraFlush;
		ex2BraFlush		<= ex1BraFlush || ex1TrapFlush;
		ex2RegInLastSr	<= ex1RegInSr[7:0];

		ex2RegIdRs		<= ex1RegIdRs;
		ex2RegIdRt		<= ex1RegIdRt;
		ex2RegIdRm		<= ex1RegIdRm;
		ex2RegValRs		<= ex1RegValRs;
		ex2RegValRt		<= ex1RegValRt;
		ex2RegValRm		<= ex1RegValRm;
//		ex2RegValFRs	<= ex1RegValFRs;
//		ex2RegValFRt	<= ex1RegValFRt;
		ex2RegValCRm	<= ex1RegValCRm;
		ex2RegIdRn1		<= ex1RegIdRn1;
		ex2RegValRn1	<= ex1RegValRn1;
		ex2RegIdCn1		<= ex1RegIdCn1;
		ex2RegValCn1	<= ex1RegValCn1;
		ex2RegValPc		<= ex1RegValPc;
		ex2RegValImm	<= ex1RegValImm;

		ex2RegInLr		<= ex1RegOutLr;
		ex2RegInDlr		<= ex1RegOutDlr;
		ex2RegInDhr		<= ex1RegOutDhr;

		ex2RegValSp		<= ex1RegValSp;
		ex2RegValLr		<= ex1RegValLr;


		/* EX3 */
`ifdef jx2_debug_keepinstr
		ex3IstrWord		<= ex2IstrWord;
		ex3ValBPc		<= ex2ValBPc;
`else
		ex3IstrWord		<= UV32_XX;
		ex3ValBPc		<= UV48_XX;
`endif

		ex3OpUCmd		<= ex2OpUCmd;
		ex3OpUIxt		<= ex2OpUIxt;
		ex3PreBra		<= ex2PreBra;
//		ex3BraFlush		<= ex2BraFlush;
		ex3BraFlush		<= ex2BraFlush || ex2TrapFlush;
		ex3RegInLastSr	<= ex2RegInLastSr;
		ex3RegValSp		<= ex2RegValSp;
		ex3RegValLr		<= ex2RegValLr;

		ex3RegIdRs		<= ex2RegIdRs;
		ex3RegIdRt		<= ex2RegIdRt;
		ex3RegIdRm		<= ex2RegIdRm;
		ex3RegValRs		<= ex2RegValRs;
		ex3RegValRt		<= ex2RegValRt;
		ex3RegValRm		<= ex2RegValRm;
		ex3RegIdRn2		<= ex2RegIdRn2;
		ex3RegValRn2	<= ex2RegValRn2;
		ex3RegIdCn2		<= ex2RegIdCn2;
		ex3RegValCn2	<= ex2RegValCn2;
		ex3RegValPc		<= ex2RegValPc;
		ex3RegValImm	<= ex2RegValImm;

		ex3RegInLr		<= ex2RegOutLr;
		ex3RegInDlr		<= ex2RegOutDlr;
		ex3RegInDhr		<= ex2RegOutDhr;


		/* WB */
		ewbRegIdRm		<= ex3RegIdRm;

		/* EXL4 */
		if(!(ex3BraFlush || ex3TrapFlush || crOutSr[28]))
		begin
			/*
			 * Try to keep track of the last non-flushed state.
			 */
		
			exL4ValBPc		<= ex3ValBPc;
			exL4OpUCmd		<= ex3OpUCmd;
			exL4OpUIxt		<= ex3OpUIxt;
			exL4PreBra		<= ex3PreBra;
			exL4BraFlush	<= ex3BraFlush || ex3TrapFlush;
//			exL4RegInLastSr	<= ex3RegInLastSr;
			exL4RegInSr		<= crOutSr[31:0];

			exL4RegIdRs		<= ex3RegIdRs;
			exL4RegIdRt		<= ex3RegIdRt;
			exL4RegIdRm		<= ex3RegIdRm;
			exL4RegValRs	<= ex3RegValRs;
			exL4RegValRt	<= ex3RegValRt;
			exL4RegValRm	<= ex3RegValRm;
			exL4RegIdRn2	<= ex3RegIdRn2;
			exL4RegValRn2	<= ex3RegValRn2;
			exL4RegIdCn2	<= ex3RegIdCn2;
			exL4RegValCn2	<= ex3RegValCn2;
			exL4RegValPc	<= ex3RegValPc;
			exL4RegValImm	<= ex3RegValImm;

			exL4RegInSp		<= { gprOutSp[63:48], ex3RegValSp };

			exL4RegInLr		<= ex3RegValLr;

			exL4RegInDlr	<= gprOutDlr;
			exL4RegInDhr	<= gprOutDhr;
		end
	end

	tDbgExHold1			<= exHold1;
	tDbgExHold2			<= exHold2;

	tDbgOutStatus1B		<= tDbgOutStatus1;
	tDbgOutStatus2B		<= tDbgOutStatus2;
	tDbgOutStatus3B		<= tDbgOutStatus3;
	tDbgOutStatus4B		<= tDbgOutStatus4;
	tDbgOutStatus5B		<= tDbgOutStatus5;
	tDbgOutStatus6B		<= tDbgOutStatus6;
	tDbgOutStatus7B		<= tDbgOutStatus7;
	tDbgOutStatus8B		<= tDbgOutStatus8;

`ifdef def_true
	tDbgOutStatus1C		<= tDbgOutStatus1B;
	tDbgOutStatus2C		<= tDbgOutStatus2B;
	tDbgOutStatus3C		<= tDbgOutStatus3B;
	tDbgOutStatus4C		<= tDbgOutStatus4B;
	tDbgOutStatus5C		<= tDbgOutStatus5B;
	tDbgOutStatus6C		<= tDbgOutStatus6B;
	tDbgOutStatus7C		<= tDbgOutStatus7B;
	tDbgOutStatus8C		<= tDbgOutStatus8B;
`endif

end

endmodule
