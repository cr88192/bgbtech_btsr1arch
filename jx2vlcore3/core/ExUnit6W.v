/*

PF IF ID1 ID2 EX1 EX2 EX3 WB

Rework effort to create a 6-wide pipeline.
The pipeline may operate as either SMT or in a ganged configuration.

 */

`include "CoreDefs.v"

`include "ringbus/RbiMemL1A2W.v"

`include "RegCR.v"

`include "DecOpWx3.v"
`include "RegGPR_6R3W.v"

`include "ExEX1.v"
`include "ExEX2.v"
`include "ExEX3.v"

`include "ExALU.v"
`include "ExMulC.v"
`include "ExMulW.v"

`include "ExEXB1.v"
`include "ExEXB2.v"
`include "ExEXB3.v"

`include "ExALUB.v"

`ifdef jx2_enable_prebra
`include "DecPreBra.v"
`endif

`include "FpuExOpW.v"

`ifdef jx2_enable_ldekey
`include "ExModKrrEnc.v"
`endif

/* verilator lint_off DEFPARAM */

module ExUnit6W(
	clock,
	reset,
	timers,
	
	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
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

input[47:0]		memAddrIn;
output[47:0]	memAddrOut;

`input_tile		memDataIn;
`output_tile	memDataOut;

input[15:0]		memOpmIn;
output[15:0]	memOpmOut;

input[15:0]		memSeqIn;
output[15:0]	memSeqOut;

input[7:0]		unitNodeId;
	

output[47:0]	dbgOutPc;
output[95:0]	dbgOutIstr;

output			dbgExHold1;
output			dbgExHold2;

// output[31:0]	dbgDcInAddr;
output[47:0]	dbgDcInAddr;
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

reg			tDbgOutStatus1C;
reg			tDbgOutStatus2C;
reg			tDbgOutStatus3C;
reg			tDbgOutStatus4C;
reg			tDbgOutStatus5C;
reg			tDbgOutStatus6C;
reg			tDbgOutStatus7C;
reg			tDbgOutStatus8C;

assign		dbgOutStatus1 = tDbgOutStatus1C;
assign		dbgOutStatus2 = tDbgOutStatus2C;
assign		dbgOutStatus3 = tDbgOutStatus3C;
assign		dbgOutStatus4 = tDbgOutStatus4C;
assign		dbgOutStatus5 = tDbgOutStatus5C;
assign		dbgOutStatus6 = tDbgOutStatus6C;
assign		dbgOutStatus7 = tDbgOutStatus7C;
assign		dbgOutStatus8 = tDbgOutStatus8C;


/* IF */

wire[63:0]		gpaOutDlr;
wire[63:0]		gpaOutDhr;

wire[63:0]		gpbOutDlr;
wire[63:0]		gpbOutDhr;

wire[63:0]		craOutMmcr;
wire[63:0]		craOutKrr;
wire[63:0]		craOutSr;
reg [63:0]		craInSr;

wire[63:0]		crbOutKrr;
wire[63:0]		crbOutSr;
reg [63:0]		crbInSr;

reg[47:0]		ifaValPc;
reg[47:0]		ifaLastPc;
wire[95:0]		ifaIstrWord;	//source instruction word
wire[1:0]		ifaOutPcOK;
wire[3:0]		ifaOutPcStep;
reg[3:0]		ifaLastPcStep;
reg				ifaInPcHold;
wire[3:0]		ifaOutPcSxo;

reg				ifaValBraOk;
reg				ifaNxtValBraOk;

wire			ifaInPcWxe;


reg[47:0]		ifbValPc;
reg[47:0]		ifbLastPc;
wire[95:0]		ifbIstrWord;	//source instruction word
wire[3:0]		ifbOutPcStep;
reg[3:0]		ifbLastPcStep;
reg				ifbInPcHold;
wire[3:0]		ifbOutPcSxo;
reg				ifbValBraOk;
reg				ifbNxtValBraOk;
wire			ifbInPcWxe;


assign	ifaInPcWxe = craOutSr[27];
assign	ifbInPcWxe = crbOutSr[27];

assign	dbgOutPc	= ifaLastPc;
assign	dbgOutIstr	= ifaIstrWord;

wire[ 1:0]		dcaOutOK;
reg				dcaInHold;
wire			dcaOutHold;
wire			dcBusWait;

reg [63:0]		dcaInTraPc;



reg [47:0]		dcaInAddr;
reg [4:0]		dcaInOpm;
wire[63:0]		dcaOutVal;
wire[63:0]		dcaOutValB;
reg [63:0]		dcaInVal;
reg [63:0]		dcaInValB;

reg [47:0]		dcbInAddr;
reg [4:0]		dcbInOpm;
wire[63:0]		dcbOutVal;
wire[63:0]		dcbOutValB;
reg [63:0]		dcbInVal;
reg [63:0]		dcbInValB;

reg			tDeadlockLatch;
reg			tNxtDeadlockLatch;

assign	dbgDcInAddr	= dcaInAddr;
assign	dbgDcInOpm	= dcaInOpm;
assign	dbgDcOutVal	= dcaOutVal;
assign	dbgDcInVal	= dcaInVal;
assign	dbgDcOutOK	= dcaOutOK;

wire[63:0]		memRegExc;
wire[63:0]		memRegTraPc;

RbiMemL1A2W		memL1(
	clock,			reset,

	ifaValPc,		ifaIstrWord,
	ifaOutPcOK,		ifaOutPcStep,
	ifaInPcHold,	ifaInPcWxe,
	ifaOutPcSxo,

	ifbValPc,		ifbIstrWord,
	ifbOutPcOK,		ifbOutPcStep,
	ifbInPcHold,	ifbInPcWxe,
	ifbOutPcSxo,

	dcaOutOK,		dcaInHold,
	dcaOutHold,		dcBusWait,

	dcaInAddr,		dcaInOpm,
	dcaOutVal,		dcaInVal,
	dcaOutValB,		dcaInValB,

	dcbInAddr,		dcbInOpm,
	dcbOutVal,		dcbInVal,
	dcbOutValB,		dcbInValB,

	craOutMmcr,

	gpaOutDlr,		gpaOutDhr,
	craOutKrr,		craOutSr,

	gpbOutDlr,		gpbOutDhr,
	crbOutKrr,		crbOutSr,

	memRegExc,		memRegTraPc,
	dcaInTraPc,		tDeadlockLatch,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

/* ID1 */

wire[47:0]		gpaValGbr;
wire[63:0]		gpaValLr;
wire[63:0]		gpaValCm;

wire[47:0]		gpbValGbr;
wire[63:0]		gpbValLr;
wire[63:0]		gpbValCm;

reg[47:0]		ida1ValBPc;
reg[47:0]		ida1ValPc;
reg[95:0]		ida1IstrWord;	//source instruction word
reg[95:0]		ida1IstrWordL1;	//source instruction word
reg[95:0]		ida1IstrWordL2;	//source instruction word
reg[3:0]		ida1IstrSxo;

reg[47:0]		idb1ValBPc;
reg[47:0]		idb1ValPc;
reg[95:0]		idb1IstrWord;	//source instruction word
reg[95:0]		idb1IstrWordL1;	//source instruction word
reg[95:0]		idb1IstrWordL2;	//source instruction word
reg[3:0]		idb1IstrSxo;


`wire_gpr		idaA1IdRegM;
`wire_gpr		idaA1IdRegO;
`wire_gpr		idaA1IdRegN;
wire[32:0]		idaA1IdImm;
wire[8:0]		idaA1IdUCmd;
wire[8:0]		idaA1IdUIxt;

`wire_gpr		idbA1IdRegM;
`wire_gpr		idbA1IdRegO;
`wire_gpr		idbA1IdRegN;
wire[32:0]		idbA1IdImm;
wire[8:0]		idbA1IdUCmd;
wire[8:0]		idbA1IdUIxt;

`wire_gpr		idaB1IdRegM;
`wire_gpr		idaB1IdRegO;
`wire_gpr		idaB1IdRegN;
wire[32:0]		idaB1IdImm;
wire[8:0]		idaB1IdUCmd;
wire[8:0]		idaB1IdUIxt;

`wire_gpr		idbB1IdRegM;
`wire_gpr		idbB1IdRegO;
`wire_gpr		idbB1IdRegN;
wire[32:0]		idbB1IdImm;
wire[8:0]		idbB1IdUCmd;
wire[8:0]		idbB1IdUIxt;

`wire_gpr		idaC1IdRegM;
`wire_gpr		idaC1IdRegO;
`wire_gpr		idaC1IdRegN;
wire[32:0]		idaC1IdImm;
wire[8:0]		idaC1IdUCmd;
wire[8:0]		idaC1IdUIxt;

`wire_gpr		idbC1IdRegM;
`wire_gpr		idbC1IdRegO;
`wire_gpr		idbC1IdRegN;
wire[32:0]		idbC1IdImm;
wire[8:0]		idbC1IdUCmd;
wire[8:0]		idbC1IdUIxt;

DecOpWx3	decOpa(
	clock,			reset,
	ida1IstrWord,	craOutSr,
	ida1IstrSxo,

	idaA1IdRegM,		idaA1IdRegO,
	idaA1IdRegN,		idaA1IdImm,
	idaA1IdUCmd,		idaA1IdUIxt,

	idaB1IdRegM,		idaB1IdRegO,
	idaB1IdRegN,		idaB1IdImm,
	idaB1IdUCmd,		idaB1IdUIxt,

	idaC1IdRegM,		idaC1IdRegO,
	idaC1IdRegN,		idaC1IdImm,
	idaC1IdUCmd,		idaC1IdUIxt
	);

DecOpWx3	decOpb(
	clock,			reset,
	idb1IstrWord,	crbOutSr,
	idb1IstrSxo,

	idbA1IdRegM,		idbA1IdRegO,
	idbA1IdRegN,		idbA1IdImm,
	idbA1IdUCmd,		idbA1IdUIxt,

	idbB1IdRegM,		idbB1IdRegO,
	idbB1IdRegN,		idbB1IdImm,
	idbB1IdUCmd,		idbB1IdUIxt,

	idbC1IdRegM,		idbC1IdRegO,
	idbC1IdRegN,		idbC1IdImm,
	idbC1IdUCmd,		idbC1IdUIxt
	);


reg[47:0]		exaA1ValBPc;
reg[2:0]		exaA1ValBraDir;

reg[47:0]		exbA1ValBPc;
reg[2:0]		exbA1ValBraDir;

`ifdef jx2_enable_prebra
wire[47:0]		ida1PreBraPc;
wire[1:0]		ida1PreBra;

wire[47:0]		idb1PreBraPc;
wire[1:0]		idb1PreBra;

reg				ida1BraPipelineLr;
reg				ida1BraPipelineR1;
reg				ida1BraPipelineDbl;

reg				idb1BraPipelineLr;
reg				idb1BraPipelineR1;
reg				idb1BraPipelineDbl;

reg[3:0]			ida1BraPipelineLrL;
reg[3:0]			idb1BraPipelineLrL;

DecPreBra	preBra(
	clock,				reset,
	ida1IstrWord[63:0],	ida1ValBPc,	ida1ValPc,
	ida1PreBraPc,		ida1PreBra,
	gpaValLr,			ifaLastPc,
	gpaOutDlr,			gpaOutDhr,
	exaA1ValBPc,			exaA1ValBraDir,
	ida1BraPipelineLrL);

DecPreBra	preBraB(
	clock,				reset,
	idb1IstrWord[63:0],	idb1ValBPc,	idb1ValPc,
	idb1PreBraPc,		idb1PreBra,
	gpbValLr,			ifbLastPc,
	gpbOutDlr,			gpbOutDhr,
	exbA1ValBPc,		exbA1ValBraDir,
	idb1BraPipelineLrL);
`endif
	

/* ID2 */

reg[47:0]		ida2ValBPc;
reg[47:0]		ida2PreBraPc;
reg[1:0]		ida2PreBra;
reg[95:0]		ida2IstrWord;	//source instruction word

reg[47:0]		idb2ValBPc;
reg[47:0]		idb2PreBraPc;
reg[1:0]		idb2PreBra;
reg[95:0]		idb2IstrWord;	//source instruction word


/* ID2, GPR */

reg[8:0]		idaA2IdUCmd;
reg[8:0]		idaA2IdUIxt;
reg[8:0]		idaB2IdUCmd;
reg[8:0]		idaB2IdUIxt;
reg[8:0]		idaC2IdUCmd;
reg[8:0]		idaC2IdUIxt;

reg[8:0]		idbA2IdUCmd;
reg[8:0]		idbA2IdUIxt;
reg[8:0]		idbB2IdUCmd;
reg[8:0]		idbB2IdUIxt;
reg[8:0]		idbC2IdUCmd;
reg[8:0]		idbC2IdUIxt;


`reg_gpr		gpaIdRs;		//Source A, ALU / Base
`reg_gpr		gpaIdRt;		//Source B, ALU / Index
`reg_gpr		gpaIdRu;		//Source C
`reg_gpr		gpaIdRv;		//Source D, MemStore
`reg_gpr		gpaIdRm;		//Dest A
`reg_gpr		gpaIdRn;		//Dest B
`reg_gpr		gpaIdRo;		//Dest B

`reg_gpr		gpbIdRs;		//Source A, ALU / Base
`reg_gpr		gpbIdRt;		//Source B, ALU / Index
`reg_gpr		gpbIdRu;		//Source C
`reg_gpr		gpbIdRv;		//Source D, MemStore
`reg_gpr		gpbIdRm;		//Dest A
`reg_gpr		gpbIdRn;		//Dest B
`reg_gpr		gpbIdRo;		//Dest B


wire[63:0]		gpaValRs;
wire[63:0]		gpaValRt;
wire[63:0]		gpaValRu;
wire[63:0]		gpaValRv;

wire[63:0]		gpbValRs;
wire[63:0]		gpbValRt;
wire[63:0]		gpbValRu;
wire[63:0]		gpbValRv;


reg				gpaEx1Flush;
reg				gpaEx2Flush;
reg				gpaEx3Flush;

reg				gpbEx1Flush;
reg				gpbEx2Flush;
reg				gpbEx3Flush;

reg				gpaEx1DualLane;
reg				gpaEx2DualLane;
reg				gpaEx3DualLane;

reg				gpbEx1DualLane;
reg				gpbEx2DualLane;
reg				gpbEx3DualLane;

`reg_gpr		gpaIdRx;		//Source C
`reg_gpr		gpaIdRy;		//Source D, MemStore
wire[63:0]		gpaValRx;
wire[63:0]		gpaValRy;

`reg_gpr		gpbIdRx;		//Source C
`reg_gpr		gpbIdRy;		//Source D, MemStore
wire[63:0]		gpbValRx;
wire[63:0]		gpbValRy;

wire[63:0]		gpaValRm;
wire[63:0]		gpaValRn;
wire[63:0]		gpaValRo;

wire[63:0]		gpbValRm;
wire[63:0]		gpbValRn;
wire[63:0]		gpbValRo;

assign		gpaValRm	= gpaValRy;
assign		gpaValRn	= gpaValRx;
assign		gpaValRo	= gpaValRy;

assign		gpbValRm	= gpbValRy;
assign		gpbValRn	= gpbValRx;
assign		gpbValRo	= gpbValRy;


`reg_gpr		gpaIdRn1;
reg[63:0]		gpaValRn1;
`reg_gpr		gpaIdRn2;
reg[63:0]		gpaValRn2;
`reg_gpr		gpaIdRn3;
reg[63:0]		gpaValRn3;

`reg_gpr		gpbIdRn1;
reg[63:0]		gpbValRn1;
`reg_gpr		gpbIdRn2;
reg[63:0]		gpbValRn2;
`reg_gpr		gpbIdRn3;
reg[63:0]		gpbValRn3;


`reg_gpr		gpaIdRnB1;
reg[63:0]		gpaValRnB1;
`reg_gpr		gpaIdRnB2;
reg[63:0]		gpaValRnB2;
`reg_gpr		gpaIdRnB3;
reg[63:0]		gpaValRnB3;

`reg_gpr		gpbIdRnB1;
reg[63:0]		gpbValRnB1;
`reg_gpr		gpbIdRnB2;
reg[63:0]		gpbValRnB2;
`reg_gpr		gpbIdRnB3;
reg[63:0]		gpbValRnB3;


`reg_gpr		gpaIdRnC1;
reg[63:0]		gpaValRnC1;
`reg_gpr		gpaIdRnC2;
reg[63:0]		gpaValRnC2;
`reg_gpr		gpaIdRnC3;
reg[63:0]		gpaValRnC3;

`reg_gpr		gpbIdRnC1;
reg[63:0]		gpbValRnC1;
`reg_gpr		gpbIdRnC2;
reg[63:0]		gpbValRnC2;
`reg_gpr		gpbIdRnC3;
reg[63:0]		gpbValRnC3;


reg [47:0]		gpaValPc;
reg [32:0]		gpaValImm;
reg [32:0]		gpaValImmB;
reg [32:0]		gpaValImmC;

reg [47:0]		gpbValPc;
reg [32:0]		gpbValImm;
reg [32:0]		gpbValImmB;
reg [32:0]		gpbValImmC;

assign		gpaValGbr = craOutGbr;
assign		gpaValLr = craOutLr;
assign		gpaValCm = craValCm;

assign		gpbValGbr = crbOutGbr;
assign		gpbValLr = crbOutLr;
assign		gpbValCm = crbValCm;

reg [63:0]		gpaInDlr;
reg [63:0]		gpaInDhr;
wire[63:0]		gpaOutSp;
reg [63:0]		gpaInSp;

reg [63:0]		gpbInDlr;
reg [63:0]		gpbInDhr;
wire[63:0]		gpbOutSp;
reg [63:0]		gpbInSp;

wire[47:0]		craOutPc;
reg [47:0]		craInPc;
wire[63:0]		craOutLr;
reg [63:0]		craInLr;

wire[47:0]		crbOutPc;
reg [47:0]		crbInPc;
wire[63:0]		crbOutLr;
reg [63:0]		crbInLr;

wire[63:0]		craOutExsr;
reg [63:0]		craInExsr;
wire[63:0]		craOutTea;
reg [63:0]		craInTea;

wire[47:0]		craOutSpc;
reg [47:0]		craInSpc;
wire[47:0]		craOutSsp;
reg [47:0]		craInSsp;

wire[47:0]		crbOutSpc;
reg [47:0]		crbInSpc;
wire[47:0]		crbOutSsp;
reg [47:0]		crbInSsp;

wire[15:0]		craOutFpsr;
wire[15:0]		craInFpsr;

wire[15:0]		crbOutFpsr;
wire[15:0]		crbInFpsr;

wire[47:0]		craOutVbr;
wire[47:0]		craOutGbr;
wire[47:0]		craOutTbr;

wire[47:0]		crbOutGbr;
wire[47:0]		crbOutTbr;


RegGCR_12R6W regGcr(
	clock,
	reset,

	exHold2,		exHold2,
	idaA2IdUCmd,	idaA2IdUIxt,
	idbA2IdUCmd,	idbA2IdUIxt,

	gpaIdRs,		gpaIdRt,
	gpaValRs,		gpaValRt,
	gpaIdRu,		gpaIdRv,
	gpaValRu,		gpaValRv,
	gpaIdRx,		gpaIdRy,
	gpaValRx,		gpaValRy,

	gpbIdRs,		gpbIdRt,
	gpbValRs,		gpbValRt,
	gpbIdRu,		gpbIdRv,
	gpbValRu,		gpbValRv,
	gpbIdRx,		gpbIdRy,
	gpbValRx,		gpbValRy,

	gpaIdRn1,		gpaValRn1,		exaA1Hold,
	gpaIdRnB1,		gpaValRnB1,		exaB1Hold,
	gpaIdRnC1,		gpaValRnC1,		exaC1Hold,

	gpbIdRn1,		gpbValRn1,		exbA1Hold,
	gpbIdRnB1,		gpbValRnB1,		exbB1Hold,
	gpbIdRnC1,		gpbValRnC1,		exbC1Hold,

	gpaIdRn2,		gpaValRn2,		exaA2Hold,
	gpaIdRnB2,		gpaValRnB2,		exaB2Hold,
	gpaIdRnC2,		gpaValRnC2,		exaC2Hold,

	gpbIdRn2,		gpbValRn2,		exbA2Hold,
	gpbIdRnB2,		gpbValRnB2,		exbB2Hold,
	gpbIdRnC2,		gpbValRnC2,		exbC2Hold,

	gpaIdRn3,		gpaValRn3,		exaA3Hold,
	gpaIdRnB3,		gpaValRnB3,		exaB3Hold,
	gpaIdRnC3,		gpaValRnC3,		exaC3Hold,

	gpbIdRn3,		gpbValRn3,		exbA3Hold,
	gpbIdRnB3,		gpbValRnB3,		exbB3Hold,
	gpbIdRnC3,		gpbValRnC3,		exbC3Hold,

	gpaValPc,		//PC Value (Synthesized)
	gpaValImm,		//Immediate (Decode, A)
	gpaValImmB,		//Immediate (Decode, B)
	gpaValImmC,		//Immediate (Decode, C)

	gpbValPc,		//PC Value (Synthesized)
	gpbValImm,		//Immediate (Decode, A)
	gpbValImmB,		//Immediate (Decode, B)
	gpbValImmC,		//Immediate (Decode, C)

	gpaEx1Flush,	//Flush EX1
	gpaEx2Flush,	//Flush EX2
	gpaEx3Flush,	//Flush EX3

	gpbEx1Flush,	//Flush EX1
	gpbEx2Flush,	//Flush EX2
	gpbEx3Flush,	//Flush EX3

	gpaEx1DualLane,
	gpaEx2DualLane,
	gpaEx3DualLane,

	gpbEx1DualLane,
	gpbEx2DualLane,
	gpbEx3DualLane,

	gpaOutDlr,	gpaInDlr,
	gpaOutDhr,	gpaInDhr,
	gpaOutSp,	gpaInSp,

	gpbOutDlr,	gpbInDlr,
	gpbOutDhr,	gpbInDhr,
	gpbOutSp,	gpbInSp,

	craOutPc,	craInPc,
	craOutLr,	craInLr,
	craOutSr,	craInSr,

	crbOutPc,	crbInPc,
	crbOutLr,	crbInLr,
	crbOutSr,	crbInSr,

	craOutExsr,	craInExsr,
	craOutTea,	craInTea,

	craOutSpc,	craInSpc,
	craOutSsp,	craInSsp,
	craOutFpsr,	craInFpsr,

	crbOutSpc,	crbInSpc,
	crbOutSsp,	crbInSsp,
	crbOutFpsr,	crbInFpsr,

	craOutVbr,
	craOutMmcr,

	craOutGbr,
	craOutTbr,
	craOutKrr,

	crbOutGbr,
	crbOutTbr,
	crbOutKrr
	);


/* EX1 */	

reg[63:0]		exaA2RegInSr;

reg[63:0]		tNxtRegExc;
reg[63:0]		tRegExc;

wire[63:0]		exaA1MulVal;
wire[63:0]		exaA1MulWVal;

`wire_gpr		exaA1RegIdFRn;
wire[63:0]		exaA1RegValFRn;
wire[63:0]		exaA1FpuValGRn;
wire[63:0]		exaB1FpuValGRn;
wire[63:0]		exaC1FpuValGRn;
wire[63:0]		exaA1FpuValLdGRn;
wire[1:0]		exaA1FpuOK;
wire			exaA1FpuSrT;

assign	exaC1FpuValGRn	= UV64_XX;

reg[63:0]		exaA2MemDataIn;
reg[63:0]		exaA2MemDataInB;
reg[1:0]		exaA2MemDataOK;

reg[8:0]		exaA1OpUCmd;
reg[8:0]		exaA1OpUIxt;
wire[1:0]		exaA1Hold;
reg[47:0]		exaA1PreBraPc;
reg[1:0]		exaA1PreBra;
reg[95:0]		exaA1IstrWord;	//source instruction word
reg				exaA1BraFlush;
reg				exaA1TrapFlush;
reg[11:0]		exaA1Timers;

`reg_gpr		exaA1RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaA1RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaA1RegIdRm;		//Source C, MemStore
reg[63:0]		exaA1RegValRs;		//Source A Value
reg[63:0]		exaA1RegValRt;		//Source B Value
reg[63:0]		exaA1RegValRm;		//Source C Value

reg[63:0]		exaB1RegValRs;		//Source A Value

reg[63:0]		exaA1RegValCRm;		//Source C Value (CR)

`wire_gpr		exaA1RegIdRn1;		//Destination ID (EX1)
wire[63:0]		exaA1RegValRn1;		//Destination Value (EX1)
`wire_gpr		exaA1RegIdCn1;		//Destination ID (CR, EX1)
wire[63:0]		exaA1RegValCn1;		//Destination Value (CR, EX1)

`wire_gpr		exaA1HldIdRn1;		//Held Dest ID (EX1)
`wire_gpr		exaA1HldIdCn1;		//Held Dest ID (CR, EX1)
	
reg[47:0]		exaA1RegValPc;		//PC Value (Synthesized)
reg[32:0]		exaA1RegValImm;		//Immediate (Decode)
	
wire[63:0]		exaA1RegOutDlr;
reg[63:0]		exaA1RegInDlr;
wire[63:0]		exaA1RegOutDhr;
reg[63:0]		exaA1RegInDhr;
wire[63:0]		exaA1RegOutSp;
reg[63:0]		exaA1RegInSp;

wire[63:0]		exaA1RegOutLr;
reg[63:0]		exaA1RegInLr;
wire[63:0]		exaA1RegOutSr;
reg[63:0]		exaA1RegInSr;

wire[47:0]		exaA1MemAddr;
wire[ 4:0]		exaA1MemOpm;
wire[63:0]		exaA1MemDataOut;
wire[63:0]		exaA1MemDataOutB;

wire[ 7:0]		exaA1RegOutSchm;
reg [ 7:0]		exaA1RegInSchm;
wire[15:0]		exaA1TrapExc;

wire[8:0]		exaA1OpUCmd2;

ExEX1	ex1(
	clock,			reset,
	exaA1OpUCmd,	exaA1OpUIxt,
	exaA1Hold,		exaA1TrapExc,
	exaA1Timers,	exaA1OpUCmd2,

	exaA1RegIdRs,	exaA1RegIdRt,		exaA1RegIdRm,
	exaA1RegValRs,	exaA1RegValRt,	exaA1RegValRm,
	exaB1RegValRs,
	
	exaA1RegValCRm,

	exaA1RegIdRn1,	exaA1RegValRn1,
	exaA1RegIdCn1,	exaA1RegValCn1,
	exaA1HldIdRn1,	exaA1HldIdCn1,

	{	exaA1RegInSr[15:4],
		exaA1RegInSr[27:26],
		exaA1RegInSr[1:0],
		exaA1RegValPc },

	exaA1RegValImm,
	exaA1FpuValGRn,	exaA1FpuSrT,
	exaA1BraFlush,
	exaA1PreBraPc,	exaA1PreBra,
	
	exaA1RegOutDlr,	exaA1RegInDlr,
	exaA1RegOutDhr,	exaA1RegInDhr,
	exaA1RegOutSp,	exaA1RegInSp,

	exaA1RegOutLr,	exaA1RegInLr,
	exaA1RegOutSr,	exaA1RegInSr,
	exaA1RegOutSchm,	exaA1RegInSchm,

	exaA1MemAddr,		exaA1MemOpm,
	exaA1MemDataOut,	exaA1MemDataOutB,
	exaA2MemDataOK,	tRegExc
	);

wire[7:0]	exaB1ValCarryD;

wire[69:0]	exaA1ValAlu;
ExALU	exAlu(
	clock,				reset,
	exaA1RegValRs,		exaA1RegValRt,		exaB1RegValRs,
	exaA1OpUCmd,			exaA1OpUIxt,
	exHold2,			{ exaA1RegInSr[7:4], exaA1RegInSr[1:0] },
	exaA1ValAlu[63:0],	exaA1ValAlu[69:64],
	exaB1ValCarryD);

ExMulC	exaA1Mul(
	clock,			reset,
	exaA1RegValRs,	exaA1RegValRt,
	exaA1OpUCmd,		exaA1OpUIxt,
	exHold2,		exaA1MulVal
	);

ExMulW	exaA1MulW(
	clock,				reset,
	exaA1RegValRs,		exaA1RegValRt,
	exaA1OpUCmd,			exaA1OpUIxt,
	exHold2,			exaA1MulWVal,
	0
	);


`ifdef def_true

/* EX1, SMT B */

reg[63:0]		exbA2RegInSr;

wire[63:0]		exbA1MulVal;
wire[63:0]		exbA1MulWVal;

`wire_gpr		exbA1RegIdFRn;
wire[63:0]		exbA1RegValFRn;
wire[63:0]		exbA1FpuValGRn;
wire[63:0]		exbB1FpuValGRn;
wire[63:0]		exbC1FpuValGRn;
wire[63:0]		exbA1FpuValLdGRn;
wire[1:0]		exbA1FpuOK;
wire			exbA1FpuSrT;

assign	exbC1FpuValGRn	= UV64_XX;

reg[63:0]		exbA2MemDataIn;
reg[63:0]		exbA2MemDataInB;
reg[1:0]		exbA2MemDataOK;

reg[8:0]		exbA1OpUCmd;
reg[8:0]		exbA1OpUIxt;
wire[1:0]		exbA1Hold;
reg[47:0]		exbA1PreBraPc;
reg[1:0]		exbA1PreBra;
reg[95:0]		exbA1IstrWord;	//source instruction word
reg				exbA1BraFlush;
reg				exbA1TrapFlush;

`reg_gpr		exbA1RegIdRs;		//Source A, ALU / Base
`reg_gpr		exbA1RegIdRt;		//Source B, ALU / Index
`reg_gpr		exbA1RegIdRm;		//Source C, MemStore
reg[63:0]		exbA1RegValRs;		//Source A Value
reg[63:0]		exbA1RegValRt;		//Source B Value
reg[63:0]		exbA1RegValRm;		//Source C Value

reg[63:0]		exbB1RegValRs;		//Source A Value

reg[63:0]		exbA1RegValCRm;		//Source C Value (CR)

`wire_gpr		exbA1RegIdRn1;		//Destination ID (EX1)
wire[63:0]		exbA1RegValRn1;		//Destination Value (EX1)
`wire_gpr		exbA1RegIdCn1;		//Destination ID (CR, EX1)
wire[63:0]		exbA1RegValCn1;		//Destination Value (CR, EX1)

`wire_gpr		exbA1HldIdRn1;		//Held Dest ID (EX1)
`wire_gpr		exbA1HldIdCn1;		//Held Dest ID (CR, EX1)
	
reg[47:0]		exbA1RegValPc;		//PC Value (Synthesized)
reg[32:0]		exbA1RegValImm;		//Immediate (Decode)
	
wire[63:0]		exbA1RegOutDlr;
reg[63:0]		exbA1RegInDlr;
wire[63:0]		exbA1RegOutDhr;
reg[63:0]		exbA1RegInDhr;
wire[63:0]		exbA1RegOutSp;
reg[63:0]		exbA1RegInSp;

wire[63:0]		exbA1RegOutLr;
reg[63:0]		exbA1RegInLr;
wire[63:0]		exbA1RegOutSr;
reg[63:0]		exbA1RegInSr;

wire[47:0]		exbA1MemAddr;
wire[ 4:0]		exbA1MemOpm;
wire[63:0]		exbA1MemDataOut;
wire[63:0]		exbA1MemDataOutB;

wire[ 7:0]		exbA1RegOutSchm;
reg [ 7:0]		exbA1RegInSchm;
wire[15:0]		exbA1TrapExc;

wire[8:0]		exbA1OpUCmd2;

ExEX1	exb1(
	clock,			reset,
	exbA1OpUCmd,	exbA1OpUIxt,
	exbA1Hold,		exbA1TrapExc,
	exbA1Timers,	exbA1OpUCmd2,

	exbA1RegIdRs,	exbA1RegIdRt,	exbA1RegIdRm,
	exbA1RegValRs,	exbA1RegValRt,	exbA1RegValRm,
	exbB1RegValRs,
	
	exbA1RegValCRm,

	exbA1RegIdRn1,	exbA1RegValRn1,
	exbA1RegIdCn1,	exbA1RegValCn1,
	exbA1HldIdRn1,	exbA1HldIdCn1,

	{	exbA1RegInSr[15:4],
		exbA1RegInSr[27:26],
		exbA1RegInSr[1:0],
		exbA1RegValPc },

	exbA1RegValImm,
	exbA1FpuValGRn,		exbA1FpuSrT,
	exbA1BraFlush,
	exbA1PreBraPc,		exbA1PreBra,
	
	exbA1RegOutDlr,		exbA1RegInDlr,
	exbA1RegOutDhr,		exbA1RegInDhr,
	exbA1RegOutSp,		exbA1RegInSp,

	exbA1RegOutLr,		exbA1RegInLr,
	exbA1RegOutSr,		exbA1RegInSr,
	exbA1RegOutSchm,	exbA1RegInSchm,

	exbA1MemAddr,		exbA1MemOpm,
	exbA1MemDataOut,	exbA1MemDataOutB,
	exbA2MemDataOK,		tRegExc
	);

wire[7:0]	exbB1ValCarryD;

wire[69:0]	exbA1ValAlu;
ExALU	exbAlu1(
	clock,				reset,
	exbA1RegValRs,		exbA1RegValRt,		exbB1RegValRs,
	exbA1OpUCmd,		exbA1OpUIxt,
	exHold2,			{ exbA1RegInSr[7:4], exbA1RegInSr[1:0] },
	exbA1ValAlu[63:0],	exbA1ValAlu[69:64],
	exbB1ValCarryD);

ExMulC	exbA1Mul(
	clock,			reset,
	exbA1RegValRs,	exbA1RegValRt,
	exbA1OpUCmd,	exbA1OpUIxt,
	exHold2,		exbA1MulVal
	);

ExMulW	exbA1MulW(
	clock,				reset,
	exbA1RegValRs,		exbA1RegValRt,
	exbA1OpUCmd,		exbA1OpUIxt,
	exHold2,			exbA1MulWVal,
	0
	);

`endif


wire[65:0]				ex1KrreLo;
wire[65:0]				ex1KrreHi;

// `ifdef def_true
`ifdef jx2_enable_ldekey
ExModKrrEnc	ex1KrrEnc(
	clock,				reset,
	exaA1OpUCmd,			exaA1OpUIxt,
	exHold2,
	exaA1RegInDlr,		exaA1RegInDhr,	craOutKrr,
	ex1KrreLo,			ex1KrreHi);
`else
// assign		ex1KrreLo = UV66_00;
assign		ex1KrreLo = {
		((exaA1OpUCmd[5:0]==JX2_UCMD_OP_IXT) &&
		(exaA1OpUIxt[5:0]==JX2_UCIX_IXT_LDEKRR)) ? 2'b11 : 2'b00,
		UV64_00 };
assign		ex1KrreHi = UV66_00;
`endif

reg[8:0]		exaB1OpUCmd;
reg[8:0]		exaB1OpUIxt;
reg[32:0]		exaB1RegValImm;		//Immediate (Decode)

`reg_gpr		exaB1RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaB1RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaB1RegIdRm;		//Source C, MemStore
reg[63:0]		exaB1RegValRt;		//Source B Value
reg[63:0]		exaB1RegValRm;		//Source C Value

reg[8:0]		exbB1OpUCmd;
reg[8:0]		exbB1OpUIxt;
reg[32:0]		exbB1RegValImm;		//Immediate (Decode)

`reg_gpr		exbB1RegIdRs;		//Source A, ALU / Base
`reg_gpr		exbB1RegIdRt;		//Source B, ALU / Index
`reg_gpr		exbB1RegIdRm;		//Source C, MemStore
reg[63:0]		exbB1RegValRt;		//Source B Value
reg[63:0]		exbB1RegValRm;		//Source C Value

assign	exaA1FpuValLdGRn = UV64_00;

FpuExOp2W	exaA1Fpu(
	clock,				reset,

	exaA1OpUCmd,		exaA1OpUIxt,
	exaB1OpUCmd,		exaB1OpUIxt,
	exaA1RegValImm,		exaB1RegValImm,

	exbA1OpUCmd,		exbA1OpUIxt,
	exbB1OpUCmd,		exbB1OpUIxt,
	exbA1RegValImm,		exbB1RegValImm,

	exaA1RegIdRs,		exaA1RegValRs,
	exaA1RegIdRt,		exaA1RegValRt,
	exaA1RegIdRm,		exaA1RegValRm,

	exbA1RegIdRs,		exbA1RegValRs,
	exbA1RegIdRt,		exbA1RegValRt,
	exbA1RegIdRm,		exbA1RegValRm,

	exaB1RegIdRs,		exaB1RegValRs,
	exaB1RegIdRt,		exaB1RegValRt,
	exaB1RegIdRm,		exaB1RegValRm,

	exbB1RegIdRs,		exbB1RegValRs,
	exbB1RegIdRt,		exbB1RegValRt,
	exbB1RegIdRm,		exbB1RegValRm,

	exaA1FpuOK,			exaA1FpuSrT,
	craOutFpsr,			craInFpsr,
	
	exbA1FpuOK,			exbA1FpuSrT,
	crbOutFpsr,			crbInFpsr,
	
	exaA2RegInSr,		exaA1BraFlush,
	exbA2RegInSr,		exbA1BraFlush,
	exHold2,			exHold2,
	
	exaA1FpuValGRn,		exaB1FpuValGRn,
	exbA1FpuValGRn,		exbB1FpuValGRn
	);


wire[63:0] exaA2BlintVal=0;

/* EX2 */

reg[47:0]		exaA2ValBPc;

reg[8:0]		exaA2OpUCmd;
reg[8:0]		exaA2OpUIxt;
wire[1:0]		exaA2Hold;
reg[47:0]		exaA2PreBraPc;
reg[1:0]		exaA2PreBra;
reg[95:0]		exaA2IstrWord;	//source instruction word

`reg_gpr		exaA2RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaA2RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaA2RegIdRm;		//Source C, MemStore
reg[63:0]		exaA2RegValRs;		//Source A Value
reg[63:0]		exaA2RegValRt;		//Source B Value
reg[63:0]		exaA2RegValRm;		//Source C Value

reg[63:0]		exaA2RegValCRm;		//Source C Value (CR)

`reg_gpr		exaA2RegIdRn1;		//Destination ID (EX1)
reg[63:0]		exaA2RegValRn1;		//Destination Value (EX1)
`reg_gpr		exaA2RegIdCn1;		//Destination ID (CR, EX1)
reg[63:0]		exaA2RegValCn1;		//Destination Value (CR, EX1)
	
`wire_gpr		exaA2RegIdRn2;		//Destination ID (EX1)
wire[63:0]		exaA2RegValRn2;		//Destination Value (EX1)
//wire[4:0]		exaA2RegIdCn2;		//Destination ID (CR, EX1)
`wire_gpr		exaA2RegIdCn2;		//Destination ID (CR, EX1)
wire[63:0]		exaA2RegValCn2;		//Destination Value (CR, EX1)
	
reg[47:0]		exaA2RegValPc;		//PC Value (Synthesized)
reg[32:0]		exaA2RegValImm;		//Immediate (Decode)
reg[69:0]		exaA2RegAluRes;		//Arithmetic Result
reg[63:0]		exaA2RegMulRes;		//Multiplier Result
reg[63:0]		exaA2RegMulWRes;		//Multiplier Result (Word)

reg[65:0]		exaA2RegAluResB;		//Arithmetic Result (ALUB)

reg				exaA2BraFlush;		//Flush EX2
reg				exaA2TrapFlush;		//Flush EX2

wire[63:0]		exaA2RegOutDlr;
reg[63:0]		exaA2RegInDlr;
wire[63:0]		exaA2RegOutDhr;
reg[63:0]		exaA2RegInDhr;
wire[63:0]		exaA2RegOutSp;
reg[63:0]		exaA2RegInSp;

wire[63:0]		exaA2RegOutLr;
reg[63:0]		exaA2RegInLr;
wire[63:0]		exaA2RegOutSr;

wire[7:0]		exaA2RegOutSchm;
reg[7:0]		exaA2RegInSchm;
reg[7:0]		exaA2RegInLastSr;

ExEX2	exa2(
	clock,			reset,
	exaA2OpUCmd,		exaA2OpUIxt,
	exaA2Hold,

	exaA2RegIdRs,		exaA2RegIdRt,		exaA2RegIdRm,
	exaA2RegValRs,	exaA2RegValRt,	exaA2RegValRm,
	exaA2RegValCRm,

	exaA2RegIdRn1,	exaA2RegValRn1,
	exaA2RegIdCn1,	exaA2RegValCn1,

	exaA2RegIdRn2,	exaA2RegValRn2,
	exaA2RegIdCn2,	exaA2RegValCn2,
	
	exaA2RegValPc,	exaA2RegValImm,
	exaA2RegAluRes,	exaA2RegMulRes,
	exaA2RegMulWRes,	ex1KrreLo,
	exaA2RegAluResB,	exaA2BlintVal,

	exaA1FpuValGRn,	exaA1FpuValLdGRn,
	exaA1FpuSrT,		exaA1FpuOK,
	
	exaA2BraFlush,
	exaA2RegInLastSr,
	
	exaA2RegOutDlr,	exaA2RegInDlr,
	exaA2RegOutDhr,	exaA2RegInDhr,
	exaA2RegOutSp,	exaA2RegInSp,

	exaA2RegOutLr,	exaA2RegInLr,
	exaA2RegOutSr,	exaA2RegInSr,
	exaA2RegOutSchm,	exaA2RegInSchm,

	exaA2MemDataIn,
	exaA2MemDataInB,
	exaA2MemDataOK
	);


/* EX3 */

`reg_gpr		wbaARegIdRm;		//Destination Register, Writeback
`reg_gpr		wbaBRegIdRm;	//Destination Register, Writeback
`reg_gpr		wbaCRegIdRm;	//Destination Register, Writeback


reg[47:0]		exaA3ValBPc;

reg[8:0]		exaA3OpUCmd;
reg[8:0]		exaA3OpUIxt;
wire[1:0]		exaA3Hold;
reg[1:0]		exaA3PreBra;
reg[95:0]		exaA3IstrWord;	//source instruction word

`reg_gpr		exaA3RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaA3RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaA3RegIdRm;		//Source C, MemStore
reg[63:0]		exaA3RegValRs;		//Source A Value
reg[63:0]		exaA3RegValRt;		//Source B Value
reg[63:0]		exaA3RegValRm;		//Source C Value

`reg_gpr		exaA3RegIdRn2;		//Destination ID (EX2)
reg[63:0]		exaA3RegValRn2;		//Destination Value (EX2)
//reg[4:0]		exaA3RegIdCn2;		//Destination ID (CR, EX2)
`reg_gpr		exaA3RegIdCn2;		//Destination ID (CR, EX2)
reg[63:0]		exaA3RegValCn2;		//Destination Value (CR, EX2)
	
`wire_gpr		exaA3RegIdRn3;		//Destination ID (EX3)
wire[63:0]		exaA3RegValRn3;		//Destination Value (EX3)
`wire_gpr		exaA3RegIdCn3;		//Destination ID (CR, EX3)
wire[63:0]		exaA3RegValCn3;		//Destination Value (CR, EX3)
	
reg[47:0]		exaA3RegValPc;		//PC Value (Synthesized)
reg[32:0]		exaA3RegValImm;		//Immediate (Decode)
//reg[65:0]		exaA3RegAluRes;		//Arithmetic Result
reg[69:0]		exaA3RegAluRes;		//Arithmetic Result
reg[63:0]		exaA3RegMulRes;		//Multiplier Result
reg[63:0]		exaA3RegMulWRes;		//Multiplier Result (Word)
reg				exaA3BraFlush;		//Flush EX2
reg				ex3TrapFlush;		//Flush EX2

reg[7:0]		exaA3RegInLastSr;

wire[63:0]		exaA3RegOutDlr;
reg[63:0]		exaA3RegInDlr;
wire[63:0]		exaA3RegOutDhr;
reg[63:0]		exaA3RegInDhr;
wire[63:0]		exaA3RegOutLr;
reg[63:0]		exaA3RegInLr;

ExEX3	exa3(
	clock,			reset,
	exaA3OpUCmd,		exaA3OpUIxt,
	exaA3Hold,		exHold2,

	exaA3RegIdRs,	exaA3RegIdRt,	exaA3RegIdRm,
	exaA3RegValRs,	exaA3RegValRt,	exaA3RegValRm,

	exaA3RegIdRn2,	exaA3RegValRn2,
	exaA3RegIdCn2,	exaA3RegValCn2,

	exaA3RegIdRn3,	exaA3RegValRn3,
	exaA3RegIdCn3,	exaA3RegValCn3,
	
	exaA3RegValPc,	exaA3RegValImm,
	exaA3RegAluRes,	exaA3RegMulRes,
	exaA3RegMulWRes,	ex1KrreLo,
	exaA1FpuValGRn,

	exaA3BraFlush,
	exaA3RegInLastSr,

	exaA2MemDataIn,
	exaA2MemDataInB,
	exaA2MemDataOK
	);

/* EX1, Lane 2 */

wire[63:0]		exaB1MulWVal;

wire[1:0]		exaB1Hold;

`wire_gpr		exaB1RegIdRn1;		//Destination ID (EX1)
wire[63:0]		exaB1RegValRn1;		//Destination Value (EX1)
`wire_gpr		exaB1HldIdRn1;		//Held Dest ID (EX1)

//reg[32:0]		exaB1RegValImm;		//Immediate (Decode)

wire[8:0]		exaB1OpUCmd2;

ExEXB1	exb1(
	clock,			reset,
	exaB1OpUCmd,		exaB1OpUIxt,
	exaB1Hold,		exaB1OpUCmd2,
	
	exaB1RegIdRs,	exaB1RegIdRt,
	exaB1RegIdRm,	exaB1RegValRs,
	exaB1RegValRt,	exaB1RegValRm,
	exaA1RegValRs,

	exaB1RegIdRn1,	exaB1RegValRn1,
	exaB1HldIdRn1,
	
	exaA1RegValPc,	exaB1RegValImm,
	exaA1BraFlush,
	exaA1RegInSr,
	1
	);
	

wire[65:0]	exaB1ValAlu;

ExALUB	exaB1Alu(
	clock,				reset,
	exaB1RegValRs,		exaB1RegValRt,
	exaB1OpUCmd,			exaB1OpUIxt,
	exHold2,			exaA1RegInSr[1:0],
	exaB1ValAlu[63:0],	exaB1ValAlu[65:64],
	exaB1ValCarryD);

ExMulW	exaB1MulW(
	clock,				reset,
	exaB1RegValRs,		exaB1RegValRt,
	exaB1OpUCmd,			exaB1OpUIxt,
	exHold2,			exaB1MulWVal,
	1
	);


/* EX2, Lane 2 */

reg[8:0]		exaB2OpUCmd;
reg[8:0]		exaB2OpUIxt;
wire[1:0]		exaB2Hold;

`reg_gpr		exaB2RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaB2RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaB2RegIdRm;		//Source C, MemStore
reg[63:0]		exaB2RegValRs;		//Source A Value
reg[63:0]		exaB2RegValRt;		//Source B Value
reg[63:0]		exaB2RegValRm;		//Source C Value

`reg_gpr		exaB2RegIdRn1;		//Destination ID (EX1)
reg[63:0]		exaB2RegValRn1;		//Destination Value (EX1)
`wire_gpr		exaB2RegIdRn2;		//Destination ID (EX1)
wire[63:0]		exaB2RegValRn2;		//Destination Value (EX1)
	
reg[32:0]		exaB2RegValImm;		//Immediate (Decode)
reg[65:0]		exaB2RegAluRes;		//Arithmetic Result
reg[63:0]		exaB2RegMulWRes;		//Word Multiply Result

ExEXB2		exaB2(
	clock,		reset,
	exaB2OpUCmd,	exaB2OpUIxt,
	exaB2Hold,
	
	exaB2RegIdRs,	exaB2RegIdRt,
	exaB2RegIdRm,	exaB2RegValRs,
	exaB2RegValRt,	exaB2RegValRm,
	exaB2RegIdRn1,	exaB2RegValRn1,
	exaB2RegIdRn2,	exaB2RegValRn2,
	
	exaA2RegValPc,
	exaB2RegValImm,	exaB2RegAluRes,
	exaB2RegMulWRes,	ex1KrreHi,
	exaB1FpuValGRn,
	exaA2BraFlush,
//	exaA2BraFlush || reset,

	exaA2RegInLastSr,	exaA2RegInSr,
	exaA2MemDataIn,	exaA2MemDataInB
	);

reg[8:0]		exaB3OpUCmd;
reg[8:0]		exaB3OpUIxt;
wire[1:0]		exaB3Hold;

`reg_gpr		exaB3RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaB3RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaB3RegIdRm;		//Source C, MemStore
reg[63:0]		exaB3RegValRs;		//Source A Value
reg[63:0]		exaB3RegValRt;		//Source B Value
reg[63:0]		exaB3RegValRm;		//Source C Value

`reg_gpr		exaB3RegIdRn2;		//Destination ID (EX1)
reg[63:0]		exaB3RegValRn2;		//Destination Value (EX1)
`wire_gpr		exaB3RegIdRn3;		//Destination ID (EX1)
wire[63:0]		exaB3RegValRn3;		//Destination Value (EX1)
	
reg[32:0]		exaB3RegValImm;		//Immediate (Decode)
reg[65:0]		exaB3RegAluRes;		//Arithmetic Result
reg[63:0]		exaB3RegMulWRes;		//Word Multiply Result

ExEXB3		exaB3(
	clock,		reset,
	exaB3OpUCmd,	exaB3OpUIxt,
	exaB3Hold,
	
	exaB3RegIdRs,	exaB3RegIdRt,
	exaB3RegIdRm,	exaB3RegValRs,
	exaB3RegValRt,	exaB3RegValRm,
	exaB3RegIdRn2,	exaB3RegValRn2,
	exaB3RegIdRn3,	exaB3RegValRn3,
	
	exaA3RegValPc,
	exaB3RegValImm,	exaB3RegAluRes,
	exaB3RegMulWRes,	ex1KrreHi,
	exaB1FpuValGRn,
//	exaA3BraFlush || reset,
	exaA3BraFlush,

	exaA3RegInLastSr,
	exaA2MemDataIn,	exaA2MemDataInB
	);


/* EX1, Lane 3 */

wire[63:0]		exaC1MulWVal;

reg[8:0]		exaC1OpUCmd;
reg[8:0]		exaC1OpUIxt;
wire[1:0]		exaC1Hold;

`reg_gpr		exaC1RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaC1RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaC1RegIdRm;		//Source C, MemStore
reg[63:0]		exaC1RegValRs;		//Source A Value
reg[63:0]		exaC1RegValRt;		//Source B Value
reg[63:0]		exaC1RegValRm;		//Source C Value

`wire_gpr		exaC1RegIdRn1;		//Destination ID (EX1)
wire[63:0]		exaC1RegValRn1;		//Destination Value (EX1)
`wire_gpr		exaC1HldIdRn1;		//Held Dest ID (EX1)

reg[32:0]		exaC1RegValImm;		//Immediate (Decode)
wire[8:0]		exaC1OpUCmd2;

ExEXB1	exaC1(
	clock,			reset,
	exaC1OpUCmd,		exaC1OpUIxt,
	exaC1Hold,		exaC1OpUCmd2,
	
	exaC1RegIdRs,	exaC1RegIdRt,
	exaC1RegIdRm,	exaC1RegValRs,
	exaC1RegValRt,	exaC1RegValRm,
	UV64_00,

	exaC1RegIdRn1,	exaC1RegValRn1,
	exaC1HldIdRn1,
	
	exaA1RegValPc,	exaC1RegValImm,
//	exaA1BraFlush || reset,
	exaA1BraFlush,
	exaA1RegInSr,
	2
	);
	

wire[65:0]	exaC1ValAlu;
wire[ 7:0]	exaC1ValCarryD;

ExALUB	exaC1AluC(
	clock,				reset,
	exaC1RegValRs,		exaC1RegValRt,
	exaC1OpUCmd,		exaC1OpUIxt,
	exHold2,			exaA1RegInSr[1:0],
	exaC1ValAlu[63:0],	exaC1ValAlu[65:64],
	exaC1ValCarryD);

ExMulW	exaC1MulW(
	clock,				reset,
	exaC1RegValRs,		exaC1RegValRt,
	exaC1OpUCmd,			exaC1OpUIxt,
	exHold2,			exaC1MulWVal,
	2
	);


/* EX2, Lane 3 */

reg[8:0]		exaC2OpUCmd;
reg[8:0]		exaC2OpUIxt;
wire[1:0]		exaC2Hold;

`reg_gpr		exaC2RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaC2RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaC2RegIdRm;		//Source C, MemStore
reg[63:0]		exaC2RegValRs;		//Source A Value
reg[63:0]		exaC2RegValRt;		//Source B Value
reg[63:0]		exaC2RegValRm;		//Source C Value

`reg_gpr		exaC2RegIdRn1;		//Destination ID (EX1)
reg[63:0]		exaC2RegValRn1;		//Destination Value (EX1)
`wire_gpr		exaC2RegIdRn2;		//Destination ID (EX1)
wire[63:0]		exaC2RegValRn2;		//Destination Value (EX1)
	
reg[32:0]		exaC2RegValImm;		//Immediate (Decode)
reg[65:0]		exaC2RegAluRes;		//Arithmetic Result
reg[63:0]		exaC2RegMulWRes;		//Word Multiply Result

ExEXB2		exaC2(
	clock,		reset,
	exaC2OpUCmd,	exaC2OpUIxt,
	exaC2Hold,
	
	exaC2RegIdRs,	exaC2RegIdRt,
	exaC2RegIdRm,	exaC2RegValRs,
	exaC2RegValRt,	exaC2RegValRm,
	exaC2RegIdRn1,	exaC2RegValRn1,
	exaC2RegIdRn2,	exaC2RegValRn2,
	
	exaA2RegValPc,
	exaC2RegValImm,	exaC2RegAluRes,
	exaC2RegMulWRes,	ex1KrreHi,
	exaC1FpuValGRn,
	exaA2BraFlush,

	exaA2RegInLastSr,	exaA2RegInSr,
	exaA2MemDataIn,	exaA2MemDataInB
	);

reg[8:0]		exaC3OpUCmd;
reg[8:0]		exaC3OpUIxt;
wire[1:0]		exaC3Hold;

`reg_gpr		exaC3RegIdRs;		//Source A, ALU / Base
`reg_gpr		exaC3RegIdRt;		//Source B, ALU / Index
`reg_gpr		exaC3RegIdRm;		//Source C, MemStore
reg[63:0]		exaC3RegValRs;		//Source A Value
reg[63:0]		exaC3RegValRt;		//Source B Value
reg[63:0]		exaC3RegValRm;		//Source C Value

`reg_gpr		exaC3RegIdRn2;		//Destination ID (EX1)
reg[63:0]		exaC3RegValRn2;		//Destination Value (EX1)
`wire_gpr		exaC3RegIdRn3;		//Destination ID (EX1)
wire[63:0]		exaC3RegValRn3;		//Destination Value (EX1)
	
reg[32:0]		exaC3RegValImm;		//Immediate (Decode)
reg[65:0]		exaC3RegAluRes;		//Arithmetic Result
reg[63:0]		exaC3RegMulWRes;		//Word Multiply Result

ExEXB3		exaC3(
	clock,		reset,
	exaC3OpUCmd,	exaB3OpUIxt,
	exaC3Hold,
	
	exaC3RegIdRs,		exaC3RegIdRt,
	exaC3RegIdRm,		exaC3RegValRs,
	exaC3RegValRt,		exaC3RegValRm,
	exaC3RegIdRn2,		exaC3RegValRn2,
	exaC3RegIdRn3,		exaC3RegValRn3,
	
	exaA3RegValPc,
	exaC3RegValImm,		exaB3RegAluRes,
	exaC3RegMulWRes,	ex1KrreHi,
	exaC1FpuValGRn,
	exaA3BraFlush,

	exaA3RegInLastSr,
	exaA2MemDataIn,	exaA2MemDataInB
	);


reg[31:0]	tIfaValStepPc;

reg[47:0]	tIfaValNextPc;
reg[47:0]	tIfaOpNextPc;
reg[47:0]	tIsrNextPc;

reg[47:0]	tValBraPc;
reg[47:0]	tValNextBraPc;
reg[15:0]	tValBraSrT;
reg[15:0]	tValNextBraSrT;
reg[47:0]	tIsrBraPc;

reg[7:0]	opaBraFlushMask;
reg[7:0]	nxtOpaBraFlushMask;
reg[7:0]	tIsrBraFlushMask;

reg[7:0]	tRegExcOfs;

reg[63:0]	tDelayExc;
reg[63:0]	tNxtDelayExc;

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

reg[ 5:0]	tIfaValAddPc0;
reg[26:0]	tIfaValPcAdd1;

reg[47:0]		braInSpc;
reg[63:0]		braInExsr;
reg[63:0]		braInTea;
reg[63:0]		braInSr;
reg[63:0]		braInLr;
reg[63:0]		braInDlr;
reg[63:0]		braInDhr;
reg[47:0]		braInSp;
reg[47:0]		braInSsp;
reg				braIsIsr;

reg[47:0]		braNxtInSpc;
reg[63:0]		braNxtInExsr;
reg[63:0]		braNxtInTea;
reg[63:0]		braNxtInSr;
reg[63:0]		braNxtInLr;
reg[63:0]		braNxtInDlr;
reg[63:0]		braNxtInDhr;
reg[47:0]		braNxtInSp;
reg[47:0]		braNxtInSsp;
reg				braNxtIsIsr;

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

	ifaNxtValBraOk	= 0;

	craInExsr		= craOutExsr;
	craInSpc		= craOutSpc;
	craInSsp		= craOutSsp;
	craInTea		= craOutTea;
	tValNextBraPc	= UV48_00;
	tValNextBraSrT	= {
		craOutSr[15: 4],
		craOutSr[27:26],
		craOutSr[ 1: 0] };


	exaA1TrapFlush	= 0;
	exaA2TrapFlush	= 0;
	ex3TrapFlush	= 0;

	gpaEx1DualLane	= (exaA1OpUIxt[7:6] == 2'b11);
	gpaEx2DualLane	= (exaA2OpUIxt[7:6] == 2'b11);
	gpaEx3DualLane	= (exaA3OpUIxt[7:6] == 2'b11);


`ifdef jx2_debug_hitmiss
	tBraNxtHitMiss	= tBraHitMiss;
	tBraNxtCycCnt	= tBraCycCnt;
`endif

	exHold2	=
		(exaA1Hold[0])	||	(exaA2Hold[0])	||
		(exaA3Hold[0])	||	dcaOutHold		||
		(ifaOutPcOK[1])	||	(exaA1FpuOK[1]);

	exHold1A	= exHold2;

`ifdef jx2_enable_wex3w

`ifdef def_true
	exHold1B1	=
		((	(exaA1RegIdRm == gpaIdRs) ||
			(exaA1RegIdRm == gpaIdRt) ||
			(exaA1RegIdRm == gpaIdRu) ||
			(exaA1RegIdRm == gpaIdRv) ||
			(exaA1RegIdRm == gpaIdRx) ||
			(exaA1RegIdRm == gpaIdRy)	) &&
			(exaA1Hold[1]));
	exHold1B2	=
		((	(exaB1RegIdRm == gpaIdRs) ||
			(exaB1RegIdRm == gpaIdRt) ||
			(exaB1RegIdRm == gpaIdRu) ||
			(exaB1RegIdRm == gpaIdRv) ||
			(exaB1RegIdRm == gpaIdRx) ||
			(exaB1RegIdRm == gpaIdRy)	) &&
			(exaB1Hold[1]));
	exHold1B3	=
		((	(exaC1RegIdRm == gpaIdRs) ||
			(exaC1RegIdRm == gpaIdRt) ||
			(exaC1RegIdRm == gpaIdRu) ||
			(exaC1RegIdRm == gpaIdRv) ||
			(exaC1RegIdRm == gpaIdRx) ||
			(exaC1RegIdRm == gpaIdRy)	) &&
			(exaC1Hold[1]));

	exHold1C1	=
		((	(exaA2RegIdRm == gpaIdRs) ||
			(exaA2RegIdRm == gpaIdRt) ||
			(exaA2RegIdRm == gpaIdRu) ||
			(exaA2RegIdRm == gpaIdRv) ||
			(exaA2RegIdRm == gpaIdRx) ||
			(exaA2RegIdRm == gpaIdRy)	) &&
			(exaA2Hold[1]));
	exHold1C2	=
		((	(exaB2RegIdRm == gpaIdRs) ||
			(exaB2RegIdRm == gpaIdRt) ||
			(exaB2RegIdRm == gpaIdRu) ||
			(exaB2RegIdRm == gpaIdRv) ||
			(exaB2RegIdRm == gpaIdRx) ||
			(exaB2RegIdRm == gpaIdRy)	) &&
			(exaB2Hold[1]));
	exHold1C3	=
		((	(exaC2RegIdRm == gpaIdRs) ||
			(exaC2RegIdRm == gpaIdRt) ||
			(exaC2RegIdRm == gpaIdRu) ||
			(exaC2RegIdRm == gpaIdRv) ||
			(exaC2RegIdRm == gpaIdRx) ||
			(exaC2RegIdRm == gpaIdRy)	) &&
			(exaC2Hold[1]));

	exHold1B	=
		(( exHold1B1 || exHold1B2 || exHold1B3) && !exaA1BraFlush) ||
		(( exHold1C1 || exHold1C2 || exHold1C3) && !exaA2BraFlush) ;

`endif

`else
	exHold1B1	=
		(	(exaA1RegIdRm == gpaIdRs) ||
			(exaA1RegIdRm == gpaIdRt) ||
			(exaA1RegIdRm == gpaIdRm)	) &&
		exaA1Hold[1];

	exHold1C1	=
		(	(exaA2RegIdRm == gpaIdRs) ||
			(exaA2RegIdRm == gpaIdRt) ||
			(exaA2RegIdRm == gpaIdRm)	) &&
		exaA2Hold[1];

	exHold1B	=
		(exHold1B1 && !exaA1BraFlush) ||
		(exHold1C1 && !exaA2BraFlush) ;

`endif

	exHold1C = 0;

`ifdef def_true

	/* Hack for fix MemOp+Branch bug;
	   TODO: Find/Fix actual cause of bug.
	 */
	if(	((exaA1OpUCmd[5:0]==JX2_UCMD_MOV_MR) ||
		 (exaA1OpUCmd[5:0]==JX2_UCMD_MOV_RM))	&&
		((idaA2IdUCmd[5:0]==JX2_UCMD_JMP) ||
		 (idaA2IdUCmd[5:0]==JX2_UCMD_JSR) ||
		 (idaA2IdUCmd[5:0]==JX2_UCMD_BRA) ||
		 (idaA2IdUCmd[5:0]==JX2_UCMD_BSR)))
			exHold1C = 1;

`endif

`ifdef jx2_prebra_rts
		ida1BraPipelineLr =
			(gpaIdRm    == JX2_GR_LR) ||
			(exaA1RegIdRm == JX2_GR_LR) ||
			(exaA2RegIdRm == JX2_GR_LR) ||
			(exaA3RegIdRm == JX2_GR_LR) ||
			(wbaARegIdRm == JX2_GR_LR) ||

			(exaA1OpUCmd[5:0]==JX2_UCMD_JSR) ||
			(exaA1OpUCmd[5:0]==JX2_UCMD_BSR) ||
			(exaA2OpUCmd[5:0]==JX2_UCMD_JSR) ||
			(exaA2OpUCmd[5:0]==JX2_UCMD_BSR) ||
			(exaA3OpUCmd[5:0]==JX2_UCMD_JSR) ||
			(exaA3OpUCmd[5:0]==JX2_UCMD_BSR);

		ida1BraPipelineR1 =
			(gpaIdRm    == JX2_GR_DHR) ||
			(exaA1RegIdRm == JX2_GR_DHR) ||
			(exaA2RegIdRm == JX2_GR_DHR) ||
			(exaA3RegIdRm == JX2_GR_DHR) ||
			(wbaARegIdRm == JX2_GR_DHR) ;
		
		ida1BraPipelineDbl =
			(ida1PreBra!=0) ||
			(ida2PreBra!=0) ||
			(exaA1PreBra!=0) ||
			(exaA2PreBra!=0) ||
			(exaA3PreBra!=0) ;
`endif

`ifdef jx2_enable_xgpr

`else

	if( (exaA1RegIdCn1 != JX2_CR_ZZR) &&
			(exaA1RegIdCn1 != JX2_CR_PC) &&
			(exaA1RegIdCn1[5:4] == 2'b11))
		exHold1D = 1;

	if( (exaA2RegIdCn2 != JX2_CR_ZZR) &&
			(exaA2RegIdCn2 != JX2_CR_PC) &&
			(exaA2RegIdCn2[5:4] == 2'b11))
		exHold1D = 1;

	if( (exaA3RegIdCn3 != JX2_CR_ZZR) &&
			(exaA3RegIdCn3 != JX2_CR_PC) &&
			(exaA3RegIdCn3[5:4] == 2'b11))
		exHold1D = 1;

`endif

	exHold1		= exHold1A || exHold1B || exHold1C || exHold1D;

	tDbgOutStatus1	= exHold1A;
	tDbgOutStatus2	= exHold1B;
	tDbgOutStatus3	= exHold1C;
	tDbgOutStatus4	= exHold1D;

	tDbgOutStatus5	= ifaOutPcOK[1];
	tDbgOutStatus6	= dcaOutOK[1];
	tDbgOutStatus7	= dcBusWait;
	tDbgOutStatus8	= memOpmOut[7:0] != 0;

`ifdef jx2_debug_expipe
	if(exHold1 &&
		!((exaA1OpUCmd[5:0]==JX2_UCMD_OP_IXT) && 
			(exaA1OpUIxt[5:0]==JX2_UCIX_IXT_BREAK)))
	begin
		$display("ExHold: A=%d B=%d C=%d D=%d",
			exHold1A, exHold1B, exHold1C, exHold1D);
	
		if(exHold2)
		begin
			$display("ExHold2: Ex1=%d Ex2=%d I$=%d FPU=%d D$=%d",
				exaA1Hold, exaA2Hold, ifaOutPcOK[1], exaA1FpuOK[1], dcaOutHold);
			$display("\tEX1=%d EX2=%d EX3=%d",
				(exaA1Hold[0]), (exaA2Hold[0]), (exaA3Hold[0]));
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
				exaA1Hold[0], exaA2Hold[0], exaA3Hold[0]);
			$display("ExUnit: D$=%d I$=%d FPU=%d",
				dcaOutHold, ifaOutPcOK[1], exaA1FpuOK[1]);

`ifdef jx2_enable_wex
			$display("ID1: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X OpA=%X-%X OpB=%X-%X F=%d",
				ida1ValBPc,	ida1ValPc,
				ida1IstrWord[15: 0], ida1IstrWord[31:16],
				ida1IstrWord[47:32], ida1IstrWord[63:48],
				ida1IstrWord[79:64], ida1IstrWord[95:80],
				idaA1IdUCmd, idaA1IdUIxt,
				idaB1IdUCmd, idaB1IdUIxt,
				opaBraFlushMask[2]);
			$display("     Rs=%X Rt=%X Ru=%X Rv=%X Rm=%X Rn=%X",
				idaA1IdRegM, idaA1IdRegO, idaB1IdRegM, idaB1IdRegO,
				idaA1IdRegN, idaB1IdRegN);
`ifdef jx2_enable_wex3w
			$display("     Rx=%X Ry=%X Ro=%X",
				idaC1IdRegM, idaC1IdRegO, idaC1IdRegN);
`endif

			$display("ID2: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X OpA=%X-%X OpB=%X-%X F=%d",
				ida2ValBPc,	gpaValPc,
				ida2IstrWord[15: 0], ida2IstrWord[31:16],
				ida2IstrWord[47:32], ida2IstrWord[63:48],
				ida2IstrWord[79:64], ida2IstrWord[95:80],
				idaA2IdUCmd, idaA2IdUIxt, idaB2IdUCmd, idaB2IdUIxt,
				opaBraFlushMask[1]);
			$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
				gpaIdRs, gpaValRs,
				gpaIdRt, gpaValRt,
				gpaIdRm, gpaValRm);
			$display("     Ru=%X(%X) Rv=%X(%X) Rn=%X(%X)",
				gpaIdRu, gpaValRu,
				gpaIdRv, gpaValRv,
				gpaIdRn, gpaValRn);
`ifdef jx2_enable_wex3w
			$display("     Rx=%X(%X) Ry=%X(%X) Ro=%X(%X)",
				gpaIdRx, gpaValRx,
				gpaIdRy, gpaValRy,
				gpaIdRo, gpaValRo);
`endif

`else
			$display("ID1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
				ida1ValBPc,	ida1ValPc,
				ida1IstrWord[15: 0], ida1IstrWord[31:16],
				id1IdUCmd, id1IdUIxt, opaBraFlushMask[2]);
			$display("     Rs=%X Rt=%X Rn=%X",
				id1IdRegM, id1IdRegO, id1IdRegN);

			$display("ID2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
				ida2ValBPc,	gpaValPc,
				ida2IstrWord[15: 0], ida2IstrWord[31:16],
				ida2IdUCmd, ida2IdUIxt, opaBraFlushMask[1]);
			$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
				gpaIdRs, gpaValRs,
				gpaIdRt, gpaValRt,
				gpaIdRm, gpaValRm);
`endif

			$display("EX1: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				exaA1ValBPc,	exaA1RegValPc,
				exaA1IstrWord[15: 0], exaA1IstrWord[31:16],
				exaA1IstrWord[47:32], exaA1IstrWord[63:48],
				exaA1IstrWord[79:64], exaA1IstrWord[95:80],
				exaA1OpUCmd, exaA1OpUIxt, exaA1BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn0=%X(%X)",
				exaA1RegIdRs, exaA1RegValRs,
				exaA1RegIdRt, exaA1RegValRt,
				exaA1RegIdRm, exaA1RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				exaA1RegIdRn1, exaA1RegValRn1,
				exaA1RegIdCn1, exaA1RegValCn1);

			$display("EX2: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				exaA2ValBPc,	exaA2RegValPc,
				exaA2IstrWord[15: 0], exaA2IstrWord[31:16],
				exaA2IstrWord[47:32], exaA2IstrWord[63:48],
				exaA2IstrWord[79:64], exaA2IstrWord[95:80],
				exaA2OpUCmd, exaA2OpUIxt, exaA2BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaA2RegIdRs, exaA2RegValRs,
				exaA2RegIdRt, exaA2RegValRt,
				exaA2RegIdRm, exaA2RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				exaA2RegIdRn2, exaA2RegValRn2,
				exaA2RegIdCn2, exaA2RegValCn2);

			if(exaB2OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX2-B: Op=%X-%X",
					exaB2OpUCmd, exaB2OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exaB2RegIdRs, exaB2RegValRs,
					exaB2RegIdRt, exaB2RegValRt,
					exaB2RegIdRm, exaB2RegValRm);
				$display("     Rn1=%X(%X)",
					exaB2RegIdRn2, exaB2RegValRn2);
			end

			if(exaC2OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX2-C: Op=%X-%X",
					exaC2OpUCmd, exaC2OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exaC2RegIdRs, exaC2RegValRs,
					exaC2RegIdRt, exaC2RegValRt,
					exaC2RegIdRm, exaC2RegValRm);
				$display("     Rn1=%X(%X)",
					exaC2RegIdRn2, exaC2RegValRn2);
			end

			$display("EX3: PC0=%X PC2=%X D=%X-%X-%X-%X-%X-%X Op=%X-%X F=%d",
				exaA3ValBPc,	exaA3RegValPc,
				exaA3IstrWord[15: 0], exaA3IstrWord[31:16],
				exaA3IstrWord[47:32], exaA3IstrWord[63:48],
				exaA3IstrWord[79:64], exaA3IstrWord[95:80],
				exaA3OpUCmd, exaA3OpUIxt, exaA3BraFlush);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaA3RegIdRs, exaA3RegValRs,
				exaA3RegIdRt, exaA3RegValRt,
				exaA3RegIdRm, exaA3RegValRm);
			$display("     Rn1=%X(%X) Cn1=%X(%X)",
				exaA3RegIdRn3, exaA3RegValRn3,
				exaA3RegIdCn3, exaA3RegValCn3);

			if(exaB3OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX3-B: Op=%X-%X",
					exaB3OpUCmd, exaB3OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exaB3RegIdRs, exaB3RegValRs,
					exaB3RegIdRt, exaB3RegValRt,
					exaB3RegIdRm, exaB3RegValRm);
				$display("     Rn1=%X(%X)",
					exaB3RegIdRn2, exaB3RegValRn2);
			end

			if(exaC3OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX3-C: Op=%X-%X",
					exaC3OpUCmd, exaC3OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exaC3RegIdRs, exaC3RegValRs,
					exaC3RegIdRt, exaC3RegValRt,
					exaC3RegIdRm, exaC3RegValRm);
				$display("     Rn1=%X(%X)",
					exaC3RegIdRn2, exaC3RegValRn2);
			end

			$display("");

		end
	end
`endif

	if(tDelayExc[15] && !tRegExc[15])
		tNxtRegExc = tDelayExc;

	if(memRegExc[15] && !tRegExc[15])
	begin
//		$display("memRegExc: %X", memRegExc);
		tNxtRegExc = memRegExc;
	end

`ifndef def_true
	if(memBusExc[15] && !craOutSr[28] && !tRegExc[15])
	begin
		if((memBusExc[15:12]!=4'hC) || craOutSr[3])
			tNxtRegExc = memBusExc;
	end
	else if(memBusExc[15])
	begin
		$display("Missed IRQ Flush=%d H1=%d H2=%d SR.BL=%d SR=%X",
			opaBraFlushMask[0],
			exHold1, exHold2, craOutSr[28], craOutSr);
	end
`endif

	if(exaA1TrapExc[15] && !tRegExc[15])
		tNxtRegExc = { UV48_00, exaA1TrapExc };

	if(ex1KrreLo[65:64]==2'b11)
		tNxtRegExc = { UV48_00, 16'h800D };

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

		if(!tNxtRegExc[15] && !ifaValBraOk)
		begin
//			$display("EXC Sustain %X", tRegExc);
			tNxtRegExc = tRegExc;
		end
	end

	if(tNxtRegExc[15])
	begin
		if(craOutSr[28] && craOutSr[29] &&
			(tNxtRegExc[15:12]==4'hA) && ifaValBraOk)
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
	end

	ifaInPcHold	= exHold1;
	dcaInHold	= exHold2;

//	exHold1		= 0;
//	exHold2		= 0;

	/* Advance PC */
	tIfaValStepPc		= { 28'b0, ifaOutPcStep };

	tIfaValAddPc0		= {2'b0, ifaOutPcStep} + { 1'b0, ifaLastPc[4:0] };
	tIfaValPcAdd1		= ifaLastPc[31:5] + 1;
	tIfaValNextPc		= { ifaLastPc[47:32],
		tIfaValAddPc0[5] ? tIfaValPcAdd1 : ifaLastPc[31:5],
		tIfaValAddPc0[4:0] };

	tIfaOpNextPc		= tIfaValNextPc;
	/* Hold current PC if branching. */
	if(opaBraFlushMask[3])
		tIfaValNextPc		= ifaLastPc;

	tIsrNextPc		= tIfaValNextPc;

	nxtOpaBraFlushMask	= { 1'b0, opaBraFlushMask[7:1] };

`ifdef jx2_enable_prebra
	if((ida1PreBra!=0) && !opaBraFlushMask[2])		//ID1
	begin
		tIfaValNextPc = ida1PreBraPc;
`ifdef jx2_bra2stage
		nxtOpaBraFlushMask[2] = 1;
`else
		nxtOpaBraFlushMask[2] = 1;
`endif
	end
`endif

	exaA1ValBraDir = 3'b001;
	if((exaA1OpUCmd[5:0]==JX2_UCMD_BRA) && exaA1OpUCmd[7] &&
		!exaA1BraFlush && !exHold2)
	begin
		exaA1ValBraDir[1:0] = exaA1OpUCmd[7:6];
		exaA1ValBraDir[2]	= (exaA1OpUCmd[6]^exaA2RegOutSr[0]);

`ifdef jx2_debug_hitmiss
		if(exaA1RegIdCn1 == JX2_CR_PC)
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
	begin
		$display("ExUnit: Branch Hit/Miss Count (Mid=8000): %X",
			tBraHitMiss);
		tBraNxtHitMiss	= 16'h8000;
	end
`endif

	/* Handle if EX1 unit has initiated a branch. */
	if(exaA1RegIdCn1 == JX2_CR_PC)
	begin
		tValNextBraPc = exaA1RegValCn1[47:0];
		tValNextBraSrT = exaA1RegValCn1[63:48];
		nxtOpaBraFlushMask = JX2_BRA_FLUSHMSK;

		if(exaA1BraFlush)
		begin
			$display("EX1 Invalid BRA %X", exaA1RegValCn1);
		end
	end

	if(reset)
	begin
		tValNextBraPc = UV48_00;
		nxtOpaBraFlushMask = JX2_BRA_FLUSHMSK;
		tNxtDeadlockLatch	= 0;
	end
	
	case(tRegExc[14:13])
		2'b00: tRegExcOfs=1;
		2'b01: tRegExcOfs=3;
		2'b10: tRegExcOfs=2;
		2'b11: tRegExcOfs=4;
	endcase

	craInSr			= exaA1RegOutSr;
	gpaInSp			= exaA1RegOutSp;
	craInLr			= exaA1RegOutLr;

	gpaInDlr		= exaA1RegOutDlr;
	gpaInDhr		= exaA1RegOutDhr;

	tIsrBraPc			= tValNextBraPc;
	tIsrBraFlushMask	= nxtOpaBraFlushMask;

	braNxtInSpc		= craOutSpc;
	braNxtInExsr	= craOutExsr;
	braNxtInTea		= craOutTea;
	braNxtInSr		= exaA1RegOutSr;
	braNxtInLr		= exaA1RegOutLr;
	braNxtInDlr		= exaA1RegOutDlr;
	braNxtInDhr		= exaA1RegOutDhr;
	braNxtInSp		= exaA1RegOutSp[47:0];
	braNxtInSsp		= craOutSsp;
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
		if(craOutSr[28])
			tRegSkipExc		= 1;
		if(exHold1 || exHold2)
		begin
			tRegSkipExc		= 1;
			tNxtDelayExc	= tRegExc;
		end
	end
	else
	begin
		if(craOutSr[28] && craOutSr[29] &&
			(tRegExc[15:12]==4'hA) && ifaValBraOk)
		begin
			tRegSkipExc		= 1;
		end
	end
	
	if(tRegExc[15] && !tRegSkipExc)
	begin
		if(tRegExc[15:10]==6'b111111)
		begin
//			$display("ExUnit: RTE, SPC=%X", craOutSpc);
			tValNextBraPc	= craOutSpc;
			nxtOpaBraFlushMask = JX2_BRA_FLUSHMSK;

`ifndef jx2_isr2stage
			craInExsr		= craOutExsr;
			craInSr			= { craOutSr[63:32], craOutExsr[63:32] };
`endif

//			if(craOutSr[29])
			if(craOutSr[29] && craOutSr[30])
			begin
`ifndef jx2_isr2stage
				gpaInSp			= { UV16_00, craOutSsp };
				craInSsp			= exaA1RegOutSp[47:0];
`endif

`ifdef jx2_isr2stage
				braNxtInSp		= craOutSsp;
				braNxtInSsp		= exaA1RegOutSp[47:0];
`endif
			end

`ifdef jx2_isr2stage
			braNxtInExsr	= craOutExsr;
			braNxtInSr		= { craOutSr[63:32], craOutExsr[63:32] };
			braNxtIsIsr		= 1;
`endif

		end
		else if(!craOutSr[28] || !ifaValBraOk)
		begin
// `ifndef def_true
`ifdef def_true
			if(tRegExc[15:12]!=4'b1010)
			begin
				$display("ExUnit: Fault %X, id2.PC=%X ex1.PC=%X ex2.PC=%X",
					tRegExc, ida2ValBPc, exaA1ValBPc, exaA2ValBPc);
			end
`endif
		
			tValNextBraPc = {
				craOutVbr[47:8],
				craOutVbr[7: 3]+tRegExcOfs[4:0],
				craOutVbr[2:0] };
			nxtOpaBraFlushMask = JX2_BRA_FLUSHMSK;

			if(tRegExc[15:12]!=4'b1010)
			begin

// `ifndef def_true
`ifdef def_true
				if(craOutSr[30] && (craOutSr[29] || craOutSr[28]))
				begin
					$display("Fault, RB Set, SPC=%X SR=",
						craOutSpc, craOutSr);
				end
`endif

				if(!craOutSr[28])
				begin
					$display("ISR VBR=%X PC=%X, SPC=%X",
						craOutVbr, tValNextBraPc, craInSpc);
				end
			end

			if(!craOutSr[28])
//			if(!craOutSr[28] && !braIsIsr)
			begin

`ifndef jx2_isr2stage
				craInExsr		= { craOutSr[31:8], exaA2RegOutSr[7:0],
					UV16_00, tRegExc[15:0] };
				craInTea			= { UV16_00, tRegExc[63:16] };
				craInSr			= exaA1RegOutSr;
				craInSr[30:28]	= 3'b111;

				craInLr			= craOutLr;
//					gpaInDlr		= gpaOutDlr;
//					gpaInDhr		= gpaOutDhr;
`endif

`ifdef jx2_isr2stage
				braNxtInExsr		= {
					craOutSr[31:8], exaA2RegOutSr[7:0],
					UV16_00, tRegExc[15:0] };
				braNxtInTea			=  { UV16_00, tRegExc[63:16] };
				braNxtInSr			= exaA1RegOutSr;
				braNxtInSr[30:28]	= 3'b111;
				braNxtInLr			= craOutLr;
//				braNxtInDlr			= exaA1RegOutDlr;
//				braNxtInDhr			= exaA1RegOutDhr;
				braNxtIsIsr			= 1;
`endif

//`ifndef def_true
`ifdef def_true
				if(!exaA3BraFlush)
				begin
//					$display("ISR from EX3");

`ifndef jx2_isr2stage
					craInSpc			= exaA3ValBPc;
					craInExsr[39:32]	= exaA3RegInLastSr;
					craInLr			= exaA3RegInLr;
					gpaInDlr		= gpaOutDlr;
					gpaInDhr		= gpaOutDhr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= exaA3ValBPc;
					braNxtInExsr[39:32]	= exaA3RegInLastSr;
					braNxtInLr			= exaA3RegInLr;
					braNxtInDlr			= gpaOutDlr;
					braNxtInDhr			= gpaOutDhr;
`endif

					ex3TrapFlush	= 1;
					exaA2TrapFlush	= 1;
					exaA1TrapFlush	= 1;

				end
				else
`endif
//`ifndef def_true
`ifdef def_true
				if(!exaA2BraFlush)
				begin
					$display("ISR from EX2");
`ifndef jx2_isr2stage
					craInSpc			= exaA2ValBPc;
					craInExsr[39:32]	= exaA2RegInLastSr;
					craInLr			= exaA2RegInLr;
					gpaInDlr		= gpaOutDlr;
					gpaInDhr		= gpaOutDhr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= exaA2ValBPc;
					braNxtInExsr[39:32]	= exaA2RegInLastSr;
					braNxtInLr			= exaA2RegInLr;
					braNxtInDlr			= gpaOutDlr;
					braNxtInDhr			= gpaOutDhr;
`endif

					exaA2TrapFlush	= 1;
					exaA1TrapFlush	= 1;

				end
				else
`endif
				if(!exaA1BraFlush)
				begin
					$display("ISR from EX1");

`ifndef jx2_isr2stage
					craInSpc			= exaA1ValBPc;
					craInExsr[39:32]	= exaA2RegOutSr[7:0];
//					craInLr			= exaA1RegInLr;
					craInLr			= craOutLr;
					gpaInDlr		= gpaOutDlr;
					gpaInDhr		= gpaOutDhr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= exaA1ValBPc;
					braNxtInExsr[39:32]	= exaA2RegOutSr[7:0];
					braNxtInLr			= craOutLr;
					braNxtInDlr			= gpaOutDlr;
					braNxtInDhr			= gpaOutDhr;
`endif

					exaA1TrapFlush	= 1;

				end
//				else if(!opaBraFlushMask[0])
//				else if(!nxtOpaBraFlushMask[0])
				else if(!tIsrBraFlushMask[0])
				begin
					$display("ISR from ID2");

`ifndef jx2_isr2stage
					craInSpc			= ida2ValBPc;
//					craInExsr[39:32]	= craOutSr[7:0];
					craInExsr[39:32]	= exaA2RegOutSr[7:0];
					craInLr			= craOutLr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ida2ValBPc;
					braNxtInExsr[39:32]	= exaA2RegOutSr[7:0];
					braNxtInLr			= craOutLr;
`endif

				end
//				else if(!opaBraFlushMask[1])
//				else if(!nxtOpaBraFlushMask[1])
				else if(!tIsrBraFlushMask[1])
				begin
//					$display("ISR from ID1, Msk=%X", opaBraFlushMask);
					$display("ISR from ID1, Msk=%X/%X",
						nxtOpaBraFlushMask, opaBraFlushMask);
					$display("  EX1 PC=%X %X",
						exaA1ValBPc, exaA1IstrWord);
					$display("  ID2 PC=%X %X",
						ida2ValBPc, ida2IstrWord);
					$display("  ID1 PC=%X %X",
						ida1ValBPc, ida1IstrWord);
					$display("  IF PC=%X", ifaLastPc);

`ifndef jx2_isr2stage
					craInSpc			= ida1ValBPc;
//					craInExsr[39:32]	= craOutSr[7:0];
					craInExsr[39:32]	= exaA2RegOutSr[7:0];
					craInLr			= craOutLr;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ida1ValBPc;
					braNxtInExsr[39:32]	= exaA2RegOutSr[7:0];
					braNxtInLr			= craOutLr;
`endif
				end
				else
				begin
//					$display("ISR from IF");
//					$display("ISR from IF, Msk=%X", opaBraFlushMask);
					$display("ISR from IF, Msk=%X/%X",
						nxtOpaBraFlushMask, opaBraFlushMask);
					$display("  EX1 PC=%X %X",
						exaA1ValBPc, exaA1IstrWord);
					$display("  ID2 PC=%X %X",
						ida2ValBPc, ida2IstrWord);
					$display("  ID1 PC=%X %X",
						ida1ValBPc, ida1IstrWord);
					$display("  IF PC=%X / %X", ifaLastPc, tIsrNextPc);

`ifndef jx2_isr2stage
					craInSpc			= ifaLastPc;
					craInExsr[39:32]	= craOutSr[7:0];
//					craInLr			= craOutLr;

//					tIsrBraPc			= tValNextBraPc;
//					tIsrBraFlushMask	= nxtOpaBraFlushMask;
`endif

`ifdef jx2_isr2stage
					braNxtInSpc			= ifaLastPc;
					braNxtInExsr[39:32]	= craOutSr[7:0];
`endif

`ifdef jx2_bra2stage
					if(opaBraFlushMask[4])
					begin
`ifndef jx2_isr2stage
						craInSpc = tIsrBraPc;
`else
						braNxtInSpc		= tIsrBraPc;
`endif
					end
`else
					if(tIsrBraFlushMask[3])
					begin
`ifndef jx2_isr2stage
						craInSpc = tIsrBraPc;
`else
						braNxtInSpc		= tIsrBraPc;
`endif
					end
`endif

				end

//				$display("ISR VBR=%X PC=%X, SPC=%X",
//					craOutVbr, tValNextBraPc, craInSpc);

				if(tRegExc[15:12]==4'b1110)
				begin
`ifndef jx2_isr2stage
	//				craInSpc			= ida1ValBPc;
					craInSpc			= exaA1ValBPc;
`else
					braNxtInSpc		= exaA1ValBPc;
`endif
				end

//				if(!craOutSr[29])
				if(!(craOutSr[29] && craOutSr[30]))
				begin
`ifndef jx2_isr2stage
					craInSsp			= exaA1RegOutSp[47:0];
					gpaInSp			= { UV16_00, craOutSsp };
`else
					braNxtInSp		= craOutSsp;
					braNxtInSsp		= exaA1RegOutSp[47:0];
`endif
				end

			end
		end
		else
		begin
`ifndef def_true
			$display("ExUnit: Double-Fault EXC=%X, id2.PC=%X ex1.PC=%X ex2.PC=%X",
				tRegExc, ida2ValBPc, exaA1ValBPc, exaA2ValBPc);
`endif
		end
	end


`ifdef jx2_isr2stage

// `ifndef def_true
`ifdef def_true
	if(braIsIsr)
	begin
		craInSpc		= braInSpc;
		craInExsr	= braInExsr;
		craInTea		= braInTea;
		craInSr		= braInSr;
		craInLr		= braInLr;
		gpaInDlr	= braInDlr;
		gpaInDhr	= braInDhr;
		gpaInSp		= { UV16_00, braInSp };
		craInSsp		= braInSsp;
	end
`endif

// `ifdef def_true
`ifndef def_true
	if(braNxtIsIsr)
	begin
		craInSpc		= braNxtInSpc;
		craInExsr	= braNxtInExsr;
		craInTea		= braNxtInTea;
		craInSr		= braNxtInSr;
		craInLr		= braNxtInLr;
		gpaInDlr	= braNxtInDlr;
		gpaInDhr	= braNxtInDhr;
		gpaInSp		= { UV16_00, braNxtInSp };
		craInSsp		= braNxtInSsp;
	end
`endif

`endif


`ifdef jx2_bra2stage
	if(opaBraFlushMask[4])
	begin
//		if(craOutSr[28])
//			$display("Branch %X", tValNextBraPc);
		tIfaValNextPc = tValBraPc;
		
		ifaNxtValBraOk = 1;
	end
`else
	if(nxtOpaBraFlushMask[3])
	begin
//		if(craOutSr[28])
//			$display("Branch %X", tValNextBraPc);
		tIfaValNextPc = tValNextBraPc;
		ifaNxtValBraOk = 1;
	end
`endif

	ifaValPc			= tIfaValNextPc;

	craInPc			= tIfaValNextPc;
//	craInSr			= exaA1RegOutSr;
//	craInLr			= exaA1RegOutLr;

	gpaIdRn1		= exaA1RegIdRn1;
	gpaValRn1		= exaA1RegValRn1;
	gpaIdRn2		= exaA2RegIdRn2;
	gpaValRn2		= exaA2RegValRn2;
	gpaIdRn3		= exaA3RegIdRn3;
	gpaValRn3		= exaA3RegValRn3;

	gpaIdRnB1		= exaB1RegIdRn1;
	gpaValRnB1		= exaB1RegValRn1;
	gpaIdRnB2		= exaB2RegIdRn2;
	gpaValRnB2		= exaB2RegValRn2;
	gpaIdRnB3		= exaB3RegIdRn3;
	gpaValRnB3		= exaB3RegValRn3;
	exaB3RegAluRes	= 0;
	
	exaB2RegAluRes	= exaB1ValAlu;

	gpaIdRnC1		= exaC1RegIdRn1;
	gpaValRnC1		= exaC1RegValRn1;
	gpaIdRnC2		= exaC2RegIdRn2;
	gpaValRnC2		= exaC2RegValRn2;
	gpaIdRnC3		= exaC3RegIdRn3;
	gpaValRnC3		= exaC3RegValRn3;
	exaC3RegAluRes	= 0;
	
	exaC2RegAluRes	= exaC1ValAlu;

//`ifndef def_true
`ifndef jx2_gprs_mergecm
	craIdCn1			= exaA1RegIdCn1;
	craValCn1		= exaA1RegValCn1;
	craIdCn2			= exaA2RegIdCn2;
	craValCn2		= exaA2RegValCn2;
	craIdCn3			= exaA3RegIdCn3;
	craValCn3		= exaA3RegValCn3;
`endif

// `ifdef def_true
`ifdef jx2_gprs_mergecm
	craIdCn1			= exaA1RegIdRn1;
	craValCn1		= exaA1RegValRn1;
	craIdCn2			= exaA2RegIdRn2;
	craValCn2		= exaA2RegValRn2;
	craIdCn3			= exaA3RegIdRn3;
	craValCn3		= exaA3RegValRn3;
`endif

	gpaEx1Flush = exaA1BraFlush || exaA1TrapFlush;
	gpaEx2Flush = exaA2BraFlush || exaA2TrapFlush;
	gpaEx3Flush = exaA3BraFlush || ex3TrapFlush;


	/* ID2 */

	/* EX1 */
	exaA1RegInDlr		= gpaOutDlr;
	exaA1RegInDhr		= gpaOutDhr;
	exaA1RegInSp		= gpaOutSp;
	exaA1RegInLr		= craOutLr;

	exaA1RegInSr		= exaA2RegOutSr;
	exaA1RegInSchm	= 0;

	exaA2RegAluRes	= exaA1ValAlu;
	exaA2RegMulRes	= exaA1MulVal;
	exaA2RegMulWRes	= exaA1MulWVal;
	exaA2RegAluResB	= exaB1ValAlu;

	exaA3RegMulRes	= exaA1MulVal;

	exaA3RegAluRes	= 0;
	exaA3RegMulWRes	= 0;

	dcaInAddr		= exaA1MemAddr;
	dcaInOpm			= exaA1MemOpm;
	dcaInVal			= exaA1MemDataOut;
	dcaInValB		= exaB1RegValRn1;
	dcaInTraPc		= { UV16_00, exaA1ValBPc };

	exaB2RegMulWRes	= exaB1MulWVal;
	exaB3RegMulWRes	= 0;
	exaC2RegMulWRes	= exaC1MulWVal;
	exaC3RegMulWRes	= 0;

	/* EX2 */

	exaA2MemDataIn	= dcaOutVal;
	exaA2MemDataInB	= dcaOutValB;
	exaA2MemDataOK	= dcaOutOK;

	exaA2RegInSp		= gpaOutSp;
	exaA2RegInSr		= craOutSr;
	exaA2RegInSchm	= 0;

end



always @(posedge clock)
begin

	tPreHold1		<= tNxtPreHold1;
	tLstPreHold1	<= tPreHold1;
	exHold1L		<= exHold1;

	tDelayExc		<= tNxtDelayExc;

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
		tValBraSrT		<= tValNextBraSrT;
`endif

`ifdef jx2_debug_expipe
		$display("IF : PC=%X D=%X-%X-%X-%X Step=%X PC2=%X F=%d", ifaLastPc,
			ifaIstrWord[15: 0], ifaIstrWord[31:16],
			ifaIstrWord[47:32], ifaIstrWord[63:48],
			ifaOutPcStep, tIfaValNextPc, opaBraFlushMask[3]);

`ifdef jx2_enable_wex
		$display("ID1: PC0=%X PC2=%X D=%X-%X OpA=%X-%X OpB=%X-%X F=%d",
			ida1ValBPc,	ida1ValPc,
			ida1IstrWord[15: 0], ida1IstrWord[31:16],
			idaA1IdUCmd, idaA1IdUIxt, idaB1IdUCmd, idaB1IdUIxt, opaBraFlushMask[2]);
		$display("     Rs=%X Rt=%X Ru=%X Rv=%X Rm=%X Rn=%X",
			idaA1IdRegM, idaA1IdRegO, idaB1IdRegM, idaB1IdRegO,
			idaA1IdRegN, idaB1IdRegN);
`ifdef jx2_enable_wex3w
		$display("     Rx=%X Ry=%X Ro=%X",
			idaC1IdRegM, idaC1IdRegO, idaC1IdRegN);
`endif

		$display("ID2: PC0=%X PC2=%X D=%X-%X OpA=%X-%X OpB=%X-%X F=%d",
			ida2ValBPc,	gpaValPc,
			ida2IstrWord[15: 0], ida2IstrWord[31:16],
			idaA2IdUCmd, idaA2IdUIxt, idaB2IdUCmd, idaB2IdUIxt,
			opaBraFlushMask[1]);
		$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
			gpaIdRs, gpaValRs,
			gpaIdRt, gpaValRt,
			gpaIdRm, gpaValRm);
		$display("     Ru=%X(%X) Rv=%X(%X) Rn=%X(%X)",
			gpaIdRu, gpaValRu,
			gpaIdRv, gpaValRv,
			gpaIdRn, gpaValRn);
`ifdef jx2_enable_wex3w
		$display("     Rx=%X(%X) Ry=%X(%X) Ro=%X(%X)",
			gpaIdRx, gpaValRx,
			gpaIdRy, gpaValRy,
			gpaIdRo, gpaValRo);
`endif

`else
		$display("ID1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ida1ValBPc,	ida1ValPc,
			ida1IstrWord[15: 0], ida1IstrWord[31:16],
			id1IdUCmd, id1IdUIxt, opaBraFlushMask[2]);
		$display("     Rs=%X Rt=%X Rn=%X",
			id1IdRegM, id1IdRegO, id1IdRegN);

		$display("ID2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			ida2ValBPc,	gpaValPc,
			ida2IstrWord[15: 0], ida2IstrWord[31:16],
			ida2IdUCmd, ida2IdUIxt, opaBraFlushMask[1]);
		$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
			gpaIdRs, gpaValRs,
			gpaIdRt, gpaValRt,
			gpaIdRm, gpaValRm);
`endif
`endif

		/* IF */
		ifaLastPc		<= ifaValPc;
		ifaLastPcStep	<= ifaOutPcStep;
		opaBraFlushMask	<= nxtOpaBraFlushMask;
		ifaValBraOk		<= ifaNxtValBraOk;


		/* ID1 */

		ida1ValBPc		<= ifaLastPc;
		ida1ValPc		<= tIfaOpNextPc;
		ida1IstrWord		<= ifaIstrWord;
		ida1IstrSxo		<= ifaOutPcSxo;
		ida1BraPipelineLrL	<= {
			1'b1,
			ida1BraPipelineDbl,
			ida1BraPipelineR1, ida1BraPipelineLr };

		/* ID2 */

`ifdef jx2_enable_wex
		gpaIdRs			<= idaA1IdRegM;
		gpaIdRt			<= idaA1IdRegO;
		gpaIdRu			<= idaB1IdRegM;
		gpaIdRv			<= idaB1IdRegO;
		gpaIdRm			<= idaA1IdRegN;
		gpaIdRn			<= idaB1IdRegN;

		gpaValImm		<= idaA1IdImm;
		gpaValImmB		<= idaB1IdImm;

		idaA2IdUCmd		<= idaA1IdUCmd;
		idaA2IdUIxt		<= idaA1IdUIxt;
		idaB2IdUCmd		<= idaB1IdUCmd;
		idaB2IdUIxt		<= idaB1IdUIxt;

		gpaIdRx			<= idaC1IdRegM;
		gpaIdRy			<= idaC1IdRegO;
		gpaIdRo			<= idaC1IdRegN;
		gpaValImmC		<= idaC1IdImm;
		idaC2IdUCmd		<= idaC1IdUCmd;
		idaC2IdUIxt		<= idaC1IdUIxt;

		craIdCm			<= idaA1IdRegM;

		gpaValPc		<= ida1ValPc;
		ida2PreBraPc	<= ida1PreBraPc;
		ida2PreBra		<= ida1PreBra;

		ida1IstrWordL1	<= nxtOpaBraFlushMask[2] ? UV96_00: ida1IstrWord[95:0];
		ida1IstrWordL2	<= ida1IstrWordL1;

`ifdef jx2_debug_keepinstr
		ida2IstrWord	<= ida1IstrWord;
		ida2ValBPc		<= ida1ValBPc;
`else
		ida2IstrWord	<= UV96_XX;
		ida2ValBPc		<= UV48_XX;
`endif

`else
		gpaIdRs			<= id1IdRegM;
		gpaIdRt			<= id1IdRegO;
		gpaIdRm			<= id1IdRegN;
		gpaValPc		<= ida1ValPc;
		gpaValImm		<= id1IdImm;
		craIdCm			<= id1IdRegM;


		ida1IstrWordL1	<= nxtOpaBraFlushMask[2] ? UV32_00: ida1IstrWord[31:0];
		ida1IstrWordL2	<= ida1IstrWordL1;

		ida2IdUCmd		<= id1IdUCmd;
		ida2IdUIxt		<= id1IdUIxt;
		ida2PreBraPc	<= ida1PreBraPc;
		ida2PreBra		<= ida1PreBra;

`ifdef jx2_debug_keepinstr
		ida2IstrWord	<= ida1IstrWord[31:0];
		ida2ValBPc		<= ida1ValBPc;
`else
		ida2IstrWord	<= UV32_XX;
		ida2ValBPc		<= UV48_XX;
`endif

`endif

		/* EX1 */

`ifdef jx2_debug_keepinstr
		exaA1ValBPc		<= ida2ValBPc;
		exaA1IstrWord	<= ida2IstrWord;
`else
		exaA1ValBPc		<= UV48_XX;
		exaA1IstrWord	<= UV32_XX;
`endif

		exaA1PreBraPc	<= ida2PreBraPc;
		exaA1PreBra		<= exHold1L ? { 1'b1, ida2PreBra[0] } : ida2PreBra;

		exaA1RegValPc	<= gpaValPc;
		exaA1RegValImm	<= gpaValImm;
		exaA1BraFlush	<= nxtOpaBraFlushMask[0];
		
		exaA1Timers		<= timers[11:0];

		exaA1OpUCmd		<= idaA2IdUCmd;
		exaA1OpUIxt		<= idaA2IdUIxt;
		exaB1OpUCmd		<= idaB2IdUCmd;
		exaB1OpUIxt		<= idaB2IdUIxt;
		exaB1RegValImm	<= gpaValImmB;

		exaA1RegIdRs	<= gpaIdRs;
		exaA1RegIdRt	<= gpaIdRt;
		exaA1RegIdRm	<= gpaIdRm;
		exaA1RegValRs	<= gpaValRs;
		exaA1RegValRt	<= gpaValRt;
		exaA1RegValRm	<= gpaValRm;

		exaB1RegIdRs	<= gpaIdRu;
		exaB1RegIdRt	<= gpaIdRv;
		exaB1RegIdRm	<= gpaIdRn;
		exaB1RegValRs	<= gpaValRu;
		exaB1RegValRt	<= gpaValRv;
		exaB1RegValRm	<= gpaValRn;

		exaC1OpUCmd		<= idaC2IdUCmd;
		exaC1OpUIxt		<= idaC2IdUIxt;
		exaC1RegValImm	<= gpaValImmC;

		exaC1RegIdRs	<= gpaIdRx;
		exaC1RegIdRt	<= gpaIdRy;
		exaC1RegIdRm	<= gpaIdRo;
		exaC1RegValRs	<= gpaValRx;
		exaC1RegValRt	<= gpaValRy;
		exaC1RegValRm	<= gpaValRo;

		exaA1RegValCRm	<= craValCm;

	end
	else
		if(!exHold2)
	begin
		exaA1OpUCmd		<= { JX2_IXC_AL, JX2_UCMD_NOP };
		exaA1BraFlush		<= 1;

		exaB1OpUCmd		<= { JX2_IXC_AL, JX2_UCMD_NOP };
		exaB1OpUIxt		<= UV9_00;

		exaC1OpUCmd		<= { JX2_IXC_AL, JX2_UCMD_NOP };
		exaC1OpUIxt		<= UV9_00;

`ifdef def_true
		exaA1ValBPc		<= exaA1ValBPc;
		exaA1OpUIxt		<= UV9_00;
		exaA1PreBraPc		<= 0;
		exaA1PreBra		<= 0;
		exaA1IstrWord		<= UV96_XX;

		exaA1RegValPc	<= UV48_XX;
		exaA1RegValImm	<= UV33_XX;

		exaA1RegIdRs	<= JX2_GR_ZZR;
		exaA1RegIdRt	<= JX2_GR_ZZR;
		exaA1RegIdRm	<= JX2_GR_ZZR;
		exaA1RegValRs	<= UV64_XX;
		exaA1RegValRt	<= UV64_XX;
		exaA1RegValRm	<= UV64_XX;

		exaA1RegValCRm	<= UV64_XX;

		exaB1RegIdRs	<= JX2_GR_ZZR;
		exaB1RegIdRt	<= JX2_GR_ZZR;
		exaB1RegIdRm	<= JX2_GR_ZZR;
		exaB1RegValRs	<= UV64_XX;
		exaB1RegValRt	<= UV64_XX;
		exaB1RegValRm	<= UV64_XX;
		exaB1RegValImm	<= UV33_XX;

		exaC1RegIdRs	<= JX2_GR_ZZR;
		exaC1RegIdRt	<= JX2_GR_ZZR;
		exaC1RegIdRm	<= JX2_GR_ZZR;
		exaC1RegValRs	<= UV64_XX;
		exaC1RegValRt	<= UV64_XX;
		exaC1RegValRm	<= UV64_XX;
		exaC1RegValImm	<= UV33_XX;

`endif
	end

	if(!exHold2)
	begin

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_expipe
		$display("EX1: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			exaA1ValBPc,	exaA1RegValPc,
			exaA1IstrWord[15: 0], exaA1IstrWord[31:16],
			exaA1OpUCmd, exaA1OpUIxt, exaA1BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn0=%X(%X)",
			exaA1RegIdRs, exaA1RegValRs,
			exaA1RegIdRt, exaA1RegValRt,
			exaA1RegIdRm, exaA1RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			exaA1RegIdRn1, exaA1RegValRn1,
			exaA1RegIdCn1, exaA1RegValCn1);

		$display("EX2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			exaA2ValBPc,	exaA2RegValPc,
			exaA2IstrWord[15: 0], exaA2IstrWord[31:16],
			exaA2OpUCmd, exaA2OpUIxt, exaA2BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			exaA2RegIdRs, exaA2RegValRs,
			exaA2RegIdRt, exaA2RegValRt,
			exaA2RegIdRm, exaA2RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			exaA2RegIdRn2, exaA2RegValRn2,
			exaA2RegIdCn2, exaA2RegValCn2);

`ifdef jx2_enable_wex
		if(exaB2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-B: Op=%X-%X",
				exaB2OpUCmd, exaB2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaB2RegIdRs, exaB2RegValRs,
				exaB2RegIdRt, exaB2RegValRt,
				exaB2RegIdRm, exaB2RegValRm);
			$display("     Rn1=%X(%X)",
				exaB2RegIdRn2, exaB2RegValRn2);
		end

`ifdef jx2_enable_wex3w
		if(exaC2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-C: Op=%X-%X",
				exaC2OpUCmd, exaC2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaC2RegIdRs, exaC2RegValRs,
				exaC2RegIdRt, exaC2RegValRt,
				exaC2RegIdRm, exaC2RegValRm);
			$display("     Rn1=%X(%X)",
				exaC2RegIdRn2, exaC2RegValRn2);
		end
`endif

`endif


		$display("EX3: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			exaA3ValBPc,	exaA3RegValPc,
			exaA3IstrWord[15: 0], exaA3IstrWord[31:16],
			exaA3OpUCmd, exaA3OpUIxt, exaA3BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			exaA3RegIdRs, exaA3RegValRs,
			exaA3RegIdRt, exaA3RegValRt,
			exaA3RegIdRm, exaA3RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			exaA3RegIdRn3, exaA3RegValRn3,
			exaA3RegIdCn3, exaA3RegValCn3);

`ifdef jx2_enable_wex
		if(exaB3OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX3-B: Op=%X-%X",
				exaB3OpUCmd, exaB3OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaB3RegIdRs, exaB3RegValRs,
				exaB3RegIdRt, exaB3RegValRt,
				exaB3RegIdRm, exaB3RegValRm);
			$display("     Rn1=%X(%X)",
				exaB3RegIdRn3, exaB3RegValRn3);
		end

`ifdef jx2_enable_wex3w
		if(exaC3OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX3-C: Op=%X-%X",
				exaC3OpUCmd, exaC3OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaC3RegIdRs, exaC3RegValRs,
				exaC3RegIdRt, exaC3RegValRt,
				exaC3RegIdRm, exaC3RegValRm);
			$display("     Rn1=%X(%X)",
				exaC3RegIdRn3, exaC3RegValRn3);
		end
`endif

`endif

		$display("");
`endif

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_exopipe
		$display("EX2: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			exaA2ValBPc,	exaA2RegValPc,
			exaA2IstrWord[15: 0], exaA2IstrWord[31:16],
			exaA2OpUCmd, exaA2OpUIxt, exaA2BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			exaA2RegIdRs, exaA2RegValRs,
			exaA2RegIdRt, exaA2RegValRt,
			exaA2RegIdRm, exaA2RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			exaA2RegIdRn2, exaA2RegValRn2,
			exaA2RegIdCn2, exaA2RegValCn2);

`ifdef jx2_enable_wex

		if(exaB2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-B: Op=%X-%X",
				exaB2OpUCmd, exaB2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaB2RegIdRs, exaB2RegValRs,
				exaB2RegIdRt, exaB2RegValRt,
				exaB2RegIdRm, exaB2RegValRm);
			$display("     Rn1=%X(%X)",
				exaB2RegIdRn2, exaB2RegValRn2);
		end

`ifdef jx2_enable_wex3w
		if(exaC2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-C: Op=%X-%X",
				exaC2OpUCmd, exaC2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaC2RegIdRs, exaC2RegValRs,
				exaC2RegIdRt, exaC2RegValRt,
				exaC2RegIdRm, exaC2RegValRm);
			$display("     Rn1=%X(%X)",
				exaC2RegIdRn2, exaC2RegValRn2);
		end
`endif
`endif

		$display("EX3: PC0=%X PC2=%X D=%X-%X Op=%X-%X F=%d",
			exaA3ValBPc,	exaA3RegValPc,
			exaA3IstrWord[15: 0], exaA3IstrWord[31:16],
			exaA3OpUCmd, exaA3OpUIxt, exaA3BraFlush);
		$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
			exaA3RegIdRs, exaA3RegValRs,
			exaA3RegIdRt, exaA3RegValRt,
			exaA3RegIdRm, exaA3RegValRm);
		$display("     Rn1=%X(%X) Cn1=%X(%X)",
			exaA3RegIdRn3, exaA3RegValRn3,
			exaA3RegIdCn3, exaA3RegValCn3);

`ifdef jx2_enable_wex
		if(exaB3OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX3-B: Op=%X-%X",
				exaB3OpUCmd, exaB3OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaB3RegIdRs, exaB3RegValRs,
				exaB3RegIdRt, exaB3RegValRt,
				exaB3RegIdRm, exaB3RegValRm);
			$display("     Rn1=%X(%X)",
				exaB3RegIdRn3, exaB3RegValRn3);
		end

`ifdef jx2_enable_wex3w
		if(exaC3OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX3-C: Op=%X-%X",
				exaC3OpUCmd, exaC3OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exaC3RegIdRs, exaC3RegValRs,
				exaC3RegIdRt, exaC3RegValRt,
				exaC3RegIdRm, exaC3RegValRm);
			$display("     Rn1=%X(%X)",
				exaC3RegIdRn3, exaC3RegValRn3);
		end
`endif
`endif

//		$display("");
`endif

		/* EX2 */
`ifdef jx2_debug_keepinstr
		exaA2IstrWord		<= exaA1IstrWord;
		exaA2ValBPc		<= exaA1ValBPc;
`else
		exaA2IstrWord		<= UV32_XX;
		exaA2ValBPc		<= UV48_XX;
`endif

		exaA2OpUCmd			<= exaA1OpUCmd2;
		exaA2OpUIxt			<= exaA1OpUIxt;
		exaA2PreBraPc		<= exaA1PreBraPc;
		exaA2PreBra			<= exaA1PreBra;
		exaA2BraFlush		<= exaA1BraFlush || exaA1TrapFlush;
		exaA2RegInLastSr	<= exaA1RegInSr[7:0];

		exaA2RegIdRs		<= exaA1RegIdRs;
		exaA2RegIdRt		<= exaA1RegIdRt;
		exaA2RegIdRm		<= exaA1RegIdRm;
		exaA2RegValRs		<= exaA1RegValRs;
		exaA2RegValRt		<= exaA1RegValRt;
		exaA2RegValRm		<= exaA1RegValRm;
		exaA2RegValCRm		<= exaA1RegValCRm;
		exaA2RegIdRn1		<= exaA1RegIdRn1;
		exaA2RegValRn1		<= exaA1RegValRn1;
		exaA2RegIdCn1		<= exaA1RegIdCn1;
		exaA2RegValCn1		<= exaA1RegValCn1;
		exaA2RegValPc		<= exaA1RegValPc;
		exaA2RegValImm		<= exaA1RegValImm;

		exaA2RegInLr		<= exaA1RegOutLr;
		exaA2RegInDlr		<= exaA1RegOutDlr;
		exaA2RegInDhr		<= exaA1RegOutDhr;

		exaB2OpUCmd			<= exaB1OpUCmd2;
		exaB2OpUIxt			<= exaB1OpUIxt;

		exaB2RegIdRs		<= exaB1RegIdRs;
		exaB2RegIdRt		<= exaB1RegIdRt;
		exaB2RegIdRm		<= exaB1RegIdRm;
		exaB2RegValRs		<= exaB1RegValRs;
		exaB2RegValRt		<= exaB1RegValRt;
		exaB2RegValRm		<= exaB1RegValRm;
		exaB2RegIdRn1		<= exaB1RegIdRn1;
		exaB2RegValRn1		<= exaB1RegValRn1;
		exaB2RegValImm		<= exaB1RegValImm;

		exaC2OpUCmd			<= exaC1OpUCmd2;
		exaC2OpUIxt			<= exaC1OpUIxt;

		exaC2RegIdRs		<= exaC1RegIdRs;
		exaC2RegIdRt		<= exaC1RegIdRt;
		exaC2RegIdRm		<= exaC1RegIdRm;
		exaC2RegValRs		<= exaC1RegValRs;
		exaC2RegValRt		<= exaC1RegValRt;
		exaC2RegValRm		<= exaC1RegValRm;
		exaC2RegIdRn1		<= exaC1RegIdRn1;
		exaC2RegValRn1		<= exaC1RegValRn1;
		exaC2RegValImm		<= exaC1RegValImm;


		/* EX3 */
`ifdef jx2_debug_keepinstr
		exaA3IstrWord		<= exaA2IstrWord;
		exaA3ValBPc			<= exaA2ValBPc;
`else
		exaA3IstrWord		<= UV32_XX;
		exaA3ValBPc			<= UV48_XX;
`endif

		exaA3OpUCmd			<= exaA2OpUCmd;
		exaA3OpUIxt			<= exaA2OpUIxt;
		exaA3PreBra			<= exaA2PreBra;
		exaA3BraFlush		<= exaA2BraFlush || exaA2TrapFlush;
		exaA3RegInLastSr	<= exaA2RegInLastSr;

		exaA3RegIdRs		<= exaA2RegIdRs;
		exaA3RegIdRt		<= exaA2RegIdRt;
		exaA3RegIdRm		<= exaA2RegIdRm;
		exaA3RegValRs		<= exaA2RegValRs;
		exaA3RegValRt		<= exaA2RegValRt;
		exaA3RegValRm		<= exaA2RegValRm;
		exaA3RegIdRn2		<= exaA2RegIdRn2;
		exaA3RegValRn2		<= exaA2RegValRn2;
		exaA3RegIdCn2		<= exaA2RegIdCn2;
		exaA3RegValCn2		<= exaA2RegValCn2;
		exaA3RegValPc		<= exaA2RegValPc;
		exaA3RegValImm		<= exaA2RegValImm;

		exaA3RegInLr		<= exaA2RegOutLr;
		exaA3RegInDlr		<= exaA2RegOutDlr;
		exaA3RegInDhr		<= exaA2RegOutDhr;

		exaB3OpUCmd			<= exaB2OpUCmd;
		exaB3OpUIxt			<= exaB2OpUIxt;

		exaB3RegIdRs		<= exaB2RegIdRs;
		exaB3RegIdRt		<= exaB2RegIdRt;
		exaB3RegIdRm		<= exaB2RegIdRm;
		exaB3RegValRs		<= exaB2RegValRs;
		exaB3RegValRt		<= exaB2RegValRt;
		exaB3RegValRm		<= exaB2RegValRm;
		exaB3RegIdRn2		<= exaB2RegIdRn2;
		exaB3RegValRn2		<= exaB2RegValRn2;
		exaB3RegValImm		<= exaB2RegValImm;

		exaC3OpUCmd			<= exaC2OpUCmd;
		exaC3OpUIxt			<= exaC2OpUIxt;

		exaC3RegIdRs		<= exaC2RegIdRs;
		exaC3RegIdRt		<= exaC2RegIdRt;
		exaC3RegIdRm		<= exaC2RegIdRm;
		exaC3RegValRs		<= exaC2RegValRs;
		exaC3RegValRt		<= exaC2RegValRt;
		exaC3RegValRm		<= exaC2RegValRm;
		exaC3RegIdRn2		<= exaC2RegIdRn2;
		exaC3RegValRn2		<= exaC2RegValRn2;
		exaC3RegValImm		<= exaC2RegValImm;

		/* WB */
		wbaARegIdRm		<= exaA3RegIdRm;
		wbaBRegIdRm		<= exaC2RegIdRm;
		wbaCRegIdRm		<= exaC3RegIdRm;
	
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
