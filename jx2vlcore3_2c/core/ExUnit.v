/*

IF ID1 ID2 EX1 EX2 WB

 */

`include "CoreDefs.v"

`include "MemL1A.v"
`include "RegCR.v"

`ifdef jx2_enable_wex3w
`include "DecOpWx3.v"
`include "RegGPR_6R3W.v"
`else
`ifdef jx2_enable_wex2w
`include "DecOpWx2.v"
`include "RegGPR_4R2W.v"
`else
`include "DecOp.v"
`include "RegGPR.v"
`endif
`endif

`include "ExEX1.v"
`include "ExEX2.v"
`include "ExALU.v"
// `include "ExMul.v"
`include "ExMulB.v"
`include "ExMulW.v"

`ifdef jx2_enable_wex
`include "ExEXB1.v"
`include "ExEXB2.v"
`include "ExALUB.v"
`endif

`ifdef jx2_enable_prebra
`include "DecPreBra.v"
`endif

`ifdef jx2_enable_fpu
`ifdef jx2_enable_fprs
`include "RegFPR.v"
`endif
`ifdef jx2_use_fpu_w
`include "FpuExOpW.v"
`else
`include "FpuExOp.v"
`endif
`endif

/* verilator lint_off DEFPARAM */

module ExUnit(
	clock,
	reset,
	
	memAddr,		memAddrB,
	memDataIn,		memDataOut,
	memOpm,			memOK,
	memBusExc,
	
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
input			reset;

output[47:0]	memAddr;
output[47:0]	memAddrB;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
output[4:0]		memOpm;
input[1:0]		memOK;
input[63:0]		memBusExc;

// output[31:0]	dbgOutPc;
output[47:0]	dbgOutPc;
output[95:0]	dbgOutIstr;

output			dbgExHold1;
output			dbgExHold2;

// output[31:0]	dbgDcInAddr;
output[47:0]	dbgDcInAddr;
output[ 4:0]	dbgDcInOpm;
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
// defparam		ex1.cpuid.isAltCore = isAltCore;
defparam		ex1.isAltCore = isAltCore;


reg				exHold1;
reg				exHold2;

reg				exHold1A;
reg				exHold1B;
reg				exHold1C;
reg				exHold1D;

assign		dbgExHold1 = exHold1;
assign		dbgExHold2 = exHold2;

reg			tDbgOutStatus1;
reg			tDbgOutStatus2;
reg			tDbgOutStatus3;
reg			tDbgOutStatus4;
reg			tDbgOutStatus5;
reg			tDbgOutStatus6;
reg			tDbgOutStatus7;
reg			tDbgOutStatus8;
assign		dbgOutStatus1 = tDbgOutStatus1;
assign		dbgOutStatus2 = tDbgOutStatus2;
assign		dbgOutStatus3 = tDbgOutStatus3;
assign		dbgOutStatus4 = tDbgOutStatus4;
assign		dbgOutStatus5 = tDbgOutStatus5;
assign		dbgOutStatus6 = tDbgOutStatus6;
assign		dbgOutStatus7 = tDbgOutStatus7;
assign		dbgOutStatus8 = tDbgOutStatus8;



/* IF */

wire[63:0]		gprOutDlr;
wire[63:0]		gprOutDhr;

wire[63:0]		crOutMmcr;
wire[63:0]		crOutKrr;
wire[63:0]		crOutSr;

reg[47:0]		ifValPc;
reg[47:0]		ifLastPc;
// wire[63:0]		ifIstrWord;	//source instruction word
wire[95:0]		ifIstrWord;	//source instruction word
wire[1:0]		ifOutPcOK;
wire[2:0]		ifOutPcStep;
reg[2:0]		ifLastPcStep;
reg				ifInPcHold;

wire			ifInPcWxe;

`ifdef jx2_enable_wex
assign	ifInPcWxe = crOutSr[27];
`else
assign	ifInPcWxe = 1'b0;
`endif

assign	dbgOutPc	= ifLastPc;
assign	dbgOutIstr	= ifIstrWord;

reg [47:0]		dcInAddr;
reg [ 4:0]		dcInOpm;
wire[63:0]		dcOutVal;
reg [63:0]		dcInVal;
wire[ 1:0]		dcOutOK;
reg				dcInHold;

wire[63:0]		dcOutValB;
reg [63:0]		dcInValB;

assign	dbgDcInAddr	= dcInAddr;
assign	dbgDcInOpm	= dcInOpm;
assign	dbgDcOutVal	= dcOutVal;
assign	dbgDcInVal	= dcInVal;
assign	dbgDcOutOK	= dcOutOK;


wire[63:0]		memRegExc;

MemL1A		memL1(
	clock,			reset,

	ifValPc,		ifIstrWord,
	ifOutPcOK,		ifOutPcStep,
	ifInPcHold,		ifInPcWxe,

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dcOutOK,		dcInHold,

	gprOutDlr,		gprOutDhr,
	crOutMmcr,		crOutKrr,		crOutSr,
	memRegExc,

	memAddr,		memAddrB,
	memDataIn,		memDataOut,
	memOpm,			memOK
	);

/* ID1 */

wire[47:0]		gprValGbr;
wire[47:0]		gprValLr;


reg[47:0]		id1ValBPc;
reg[47:0]		id1ValPc;
// reg[63:0]		id1IstrWord;	//source instruction word
reg[95:0]		id1IstrWord;	//source instruction word

`ifdef jx2_enable_wex

wire[5:0]		idA1IdRegM;
wire[5:0]		idA1IdRegO;
wire[5:0]		idA1IdRegN;
wire[32:0]		idA1IdImm;
wire[7:0]		idA1IdUCmd;
wire[7:0]		idA1IdUIxt;

wire[5:0]		idB1IdRegM;
wire[5:0]		idB1IdRegO;
wire[5:0]		idB1IdRegN;
wire[32:0]		idB1IdImm;
wire[7:0]		idB1IdUCmd;
wire[7:0]		idB1IdUIxt;

`ifdef jx2_enable_wex3w
wire[5:0]		idC1IdRegM;
wire[5:0]		idC1IdRegO;
wire[5:0]		idC1IdRegN;
wire[32:0]		idC1IdImm;
wire[7:0]		idC1IdUCmd;
wire[7:0]		idC1IdUIxt;

DecOpWx3	decOp(
	clock,	reset,
	id1IstrWord,	ifInPcWxe,

	idA1IdRegM,		idA1IdRegO,
	idA1IdRegN,		idA1IdImm,
	idA1IdUCmd,		idA1IdUIxt,

	idB1IdRegM,		idB1IdRegO,
	idB1IdRegN,		idB1IdImm,
	idB1IdUCmd,		idB1IdUIxt,

	idC1IdRegM,		idC1IdRegO,
	idC1IdRegN,		idC1IdImm,
	idC1IdUCmd,		idC1IdUIxt
	);
`endif

`ifdef jx2_enable_wex2w
DecOpWx2	decOp(
	clock,	reset,
	id1IstrWord[63:0],	ifInPcWxe,

	idA1IdRegM,		idA1IdRegO,
	idA1IdRegN,		idA1IdImm,
	idA1IdUCmd,		idA1IdUIxt,

	idB1IdRegM,		idB1IdRegO,
	idB1IdRegN,		idB1IdImm,
	idB1IdUCmd,		idB1IdUIxt
	);
`endif

`else

wire[5:0]		id1IdRegN;
wire[5:0]		id1IdRegM;
wire[5:0]		id1IdRegO;
wire[32:0]		id1IdImm;
wire[7:0]		id1IdUCmd;
wire[7:0]		id1IdUIxt;

DecOp	decOp(
	clock,	reset,
	id1IstrWord[63:0],
	id1IdRegN,	id1IdRegM,
	id1IdRegO,	id1IdImm,
	id1IdUCmd,	id1IdUIxt
	);

`endif

reg[47:0]		ex1ValBPc;
reg[2:0]		ex1ValBraDir;

`ifdef jx2_enable_prebra
wire[47:0]		id1PreBraPc;
wire			id1PreBra;

DecPreBra	preBra(
	clock,				reset,
	id1IstrWord[63:0],	id1ValBPc,	id1ValPc,
	id1PreBraPc,		id1PreBra,
	gprValLr,			ifLastPc,
	ex1ValBPc,			ex1ValBraDir);
`else
wire			id1PreBra;
assign		id1PreBra = 0;
`endif
	

/* ID2 */

reg[47:0]		id2ValBPc;
// reg[7:0]		id2IdUCmd;
// reg[7:0]		id2IdUIxt;
reg				id2PreBra;
reg[31:0]		id2IstrWord;	//source instruction word


/* ID2, GPR */

// `ifdef jx2_enable_wex2w
`ifdef jx2_enable_wex

reg[7:0]		idA2IdUCmd;
reg[7:0]		idA2IdUIxt;
reg[7:0]		idB2IdUCmd;
reg[7:0]		idB2IdUIxt;
`ifdef jx2_enable_wex3w
reg[7:0]		idC2IdUCmd;
reg[7:0]		idC2IdUIxt;
`endif

reg[5:0]		gprIdRs;		//Source A, ALU / Base
reg[5:0]		gprIdRt;		//Source B, ALU / Index
reg[5:0]		gprIdRu;		//Source C
reg[5:0]		gprIdRv;		//Source D, MemStore
reg[5:0]		gprIdRm;		//Dest A
reg[5:0]		gprIdRn;		//Dest B
reg[5:0]		gprIdRo;		//Dest B

wire[63:0]		gprValRs;
wire[63:0]		gprValRt;
wire[63:0]		gprValRu;
wire[63:0]		gprValRv;

`ifdef jx2_enable_wex3w

reg[5:0]		gprIdRx;		//Source C
reg[5:0]		gprIdRy;		//Source D, MemStore
wire[63:0]		gprValRx;
wire[63:0]		gprValRy;

wire[63:0]		gprValRm;
wire[63:0]		gprValRn;
wire[63:0]		gprValRo;
assign		gprValRm	= gprValRy;
assign		gprValRn	= gprValRx;
assign		gprValRo	= gprValRy;

`else

wire[63:0]		gprValRm;
wire[63:0]		gprValRn;
assign		gprValRm	= gprValRv;
assign		gprValRn	= gprValRv;

`endif

reg[5:0]		gprIdRn1;
reg[63:0]		gprValRn1;
reg[5:0]		gprIdRn2;
reg[63:0]		gprValRn2;

reg[5:0]		gprIdRnB1;
reg[63:0]		gprValRnB1;
reg[5:0]		gprIdRnB2;
reg[63:0]		gprValRnB2;

`ifdef jx2_enable_wex3w
reg[5:0]		gprIdRnC1;
reg[63:0]		gprValRnC1;
reg[5:0]		gprIdRnC2;
reg[63:0]		gprValRnC2;
`endif

reg [47:0]		gprValPc;
reg [32:0]		gprValImm;
reg [32:0]		gprValImmB;
reg [32:0]		gprValImmC;

assign		gprValGbr = crOutGbr;
assign		gprValLr = crOutLr;

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

`ifdef jx2_enable_wex3w

RegGPR_6R3W regGpr(
	clock,
	reset,
	exHold2,

	gprIdRs,		//Source A, ALU / Base
	gprIdRt,		//Source B, ALU / Index
	gprIdRu,		//Source C
	gprIdRv,		//Source D
	gprIdRx,		//Source E
	gprIdRy,		//Source F, MemStore
	gprValRs,		//Source A Value
	gprValRt,		//Source B Value
	gprValRu,		//Source C Value
	gprValRv,		//Source D Value
	gprValRx,		//Source E Value
	gprValRy,		//Source F Value

	gprIdRn1,		//Destination ID (EX1, L1)
	gprValRn1,		//Destination Value (EX1, L1)
	gprIdRn2,		//Destination ID (EX2, L1)
	gprValRn2,		//Destination Value (EX2, L1)
	
	gprIdRnB1,		//Destination ID (EX1, L2)
	gprValRnB1,		//Destination Value (EX1, L2)
	gprIdRnB2,		//Destination ID (EX2, L2)
	gprValRnB2,		//Destination Value (EX2, L2)
	
	gprIdRnC1,		//Destination ID (EX1, L3)
	gprValRnC1,		//Destination Value (EX1, L3)
	gprIdRnC2,		//Destination ID (EX2, L3)
	gprValRnC2,		//Destination Value (EX2, L3)
	
	gprValPc,		//PC Value (Synthesized)
	gprValGbr,		//GBR Value (CR)
	gprValImm,		//Immediate (Decode, A)
	gprValImmB,		//Immediate (Decode, B)
	gprValImmC,		//Immediate (Decode, C)
	gprValLr,		//LR Value (CR)
	
	gprOutDlr,	gprInDlr,
	gprOutDhr,	gprInDhr,
`ifdef jx2_sprs_elrehr
	gprOutElr,	gprInElr,
	gprOutEhr,	gprInEhr,
	gprOutBp,	gprInBp,
`endif
	gprOutSp,	gprInSp
	);

`else
RegGPR_4R2W regGpr(
	clock,
	reset,
	exHold2,

	gprIdRs,		//Source A, ALU / Base
	gprIdRt,		//Source B, ALU / Index
	gprIdRu,		//Source C
	gprIdRv,		//Source D, MemStore
	gprValRs,		//Source A Value
	gprValRt,		//Source B Value
	gprValRu,		//Source C Value
	gprValRv,		//Source D Value

	gprIdRn1,		//Destination ID (EX1, L1)
	gprValRn1,		//Destination Value (EX1, L1)
	gprIdRn2,		//Destination ID (EX2, L1)
	gprValRn2,		//Destination Value (EX2, L1)
	
	gprIdRnB1,		//Destination ID (EX1, L2)
	gprValRnB1,		//Destination Value (EX1, L2)
	gprIdRnB2,		//Destination ID (EX2, L2)
	gprValRnB2,		//Destination Value (EX2, L2)
	
	gprValPc,		//PC Value (Synthesized)
	gprValGbr,		//GBR Value (CR)
	gprValImm,		//Immediate (Decode, A)
	gprValImmB,		//Immediate (Decode, B)
	gprValLr,		//LR Value (CR)
	
	gprOutDlr,	gprInDlr,
	gprOutDhr,	gprInDhr,
`ifdef jx2_sprs_elrehr
	gprOutElr,	gprInElr,
	gprOutEhr,	gprInEhr,
	gprOutBp,	gprInBp,
`endif
	gprOutSp,	gprInSp
	);
`endif

`else

reg[7:0]		id2IdUCmd;
reg[7:0]		id2IdUIxt;

reg[5:0]		gprIdRs;		//Source A, ALU / Base
reg[5:0]		gprIdRt;		//Source B, ALU / Index
reg[5:0]		gprIdRm;		//Source C, MemStore

wire[63:0]		gprValRs;
wire[63:0]		gprValRt;
wire[63:0]		gprValRm;

reg[5:0]		gprIdRn1;
reg[63:0]		gprValRn1;
reg[5:0]		gprIdRn2;
reg[63:0]		gprValRn2;

reg [47:0]		gprValPc;
reg [32:0]		gprValImm;
// reg [31:0]		gprValGbr;
// reg [31:0]		gprValLr;

// wire[31:0]		gprValGbr;
// wire[31:0]		gprValLr;
assign		gprValGbr = crOutGbr;
assign		gprValLr = crOutLr;

// wire[63:0]		gprOutDlr;
reg [63:0]		gprInDlr;
// wire[63:0]		gprOutDhr;
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

RegGPR regGpr(
	clock,
	reset,
	exHold2,

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
	
	gprValPc,		//PC Value (Synthesized)
	gprValGbr,		//GBR Value (CR)
	gprValImm,		//Immediate (Decode)
	gprValLr,		//LR Value (CR)
	
	gprOutDlr,	gprInDlr,
	gprOutDhr,	gprInDhr,
`ifdef jx2_sprs_elrehr
	gprOutElr,	gprInElr,
	gprOutEhr,	gprInEhr,
	gprOutBp,	gprInBp,
`endif
	gprOutSp,	gprInSp
	);

`endif

/* ID2, FPR */

// wire[63:0]		crOutSr;

`ifdef jx2_enable_fprs

wire[5:0]		gprIdFRs;
wire[5:0]		gprIdFRt;
assign	gprIdFRs = (id2IdUCmd[5:0]==JX2_UCMD_MOV_RM) ? gprIdRm :  gprIdRs;
assign	gprIdFRt = gprIdRt;

wire[63:0]		gprValFRs;
wire[63:0]		gprValFRt;
reg[5:0]		gprIdFRn;
reg[63:0]		gprValFRn;

wire[63:0]		gprValFRs2;
wire[63:0]		gprValFRt2;

wire			id2FprIsGpr;
wire			id2UCmdIsFmov;

assign		id2UCmdIsFmov =
	(id2IdUCmd[5:0]==JX2_UCMD_FMOV_RM) ||
	(id2IdUCmd[5:0]==JX2_UCMD_FMOV_MR) ;
assign		id2FprIsGpr =  id2UCmdIsFmov ? id2IdIxt[2] : id2IdIxt[4];

// assign		gprValFRs = id2IdIxt[4] ? gprValRs : gprValFRs2;
// assign		gprValFRt = id2IdIxt[4] ? gprValRt : gprValFRt2;

assign			gprValFRs = id2FprIsGpr ? gprValRs : gprValFRs2;
assign			gprValFRt = id2FprIsGpr ? gprValRt : gprValFRt2;

RegFPR	regFpr(
	clock,	reset,
	gprIdFRs,	gprValFRs2,
	gprIdFRt,	gprValFRt2,
	gprIdFRn,	gprValFRn,
	crOutSr,	exHold2
	);

`else

`ifdef jx2_enable_fpu

wire[63:0]		gprValFRs;
wire[63:0]		gprValFRt;

assign			gprValFRs = gprValRs;
assign			gprValFRt = gprValRt;

`endif

`endif

/* ID2, CR */

reg [ 4:0]	crIdCm;		//Source ID
wire[63:0]	crValCm;		//Source Value
reg [ 4:0]	crIdCn1;		//Destination ID (EX1)
reg [63:0]	crValCn1;		//Destination Value (EX1)
reg [ 4:0]	crIdCn2;		//Destination ID (EX2)
reg [63:0]	crValCn2;		//Destination Value (EX2)
wire[47:0]	crOutPc;
reg [47:0]	crInPc;
wire[47:0]	crOutLr;
reg [47:0]	crInLr;
// wire[63:0]	crOutSr;
reg [63:0]	crInSr;

wire[63:0]	crOutExsr;
reg [63:0]	crInExsr;
wire[47:0]	crOutSpc;
reg [47:0]	crInSpc;
wire[47:0]	crOutSsp;
reg [47:0]	crInSsp;
wire[47:0]	crOutTea;
reg [47:0]	crInTea;

wire[47:0]	crOutVbr;
wire[47:0]	crOutGbr;
wire[47:0]	crOutTbr;
// wire[63:0]	crOutMmcr;
// wire[63:0]	crOutKrr;

RegCR regCr(
	clock,	reset,	exHold2,

	crIdCm,		//Source ID
	crValCm,		//Source Value

	crIdCn1,		//Destination ID (EX1)
	crValCn1,		//Destination Value (EX1)
	crIdCn2,		//Destination ID (EX2)
	crValCn2,		//Destination Value (EX2)

	crOutPc,	crInPc,
	crOutLr,	crInLr,
	crOutSr,	crInSr,

	crOutExsr,	crInExsr,
	crOutSpc,	crInSpc,
	crOutSsp,	crInSsp,
	crOutTea,	crInTea,

	crOutVbr,
	crOutGbr,
	crOutTbr,
	crOutMmcr,
	crOutKrr
	);


/* EX1 */	

reg[63:0]		ex2RegInSr;


wire[63:0]		ex1MulVal;
wire[63:0]		ex1MulWVal;

// `ifdef jx2_enable_fpu
wire[5:0]		ex1RegIdFRn;
wire[63:0]		ex1RegValFRn;
wire[63:0]		ex1FpuValGRn;
wire[63:0]		exB1FpuValGRn;
wire[63:0]		exC1FpuValGRn;
wire[63:0]		ex1FpuValLdGRn;
wire[1:0]		ex1FpuOK;
wire			ex1FpuSrT;
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
reg[63:0]		ex2MemDataInB;
reg[1:0]		ex2MemDataOK;


// reg[31:0]		ex1ValBPc;

reg[7:0]		ex1OpUCmd;
reg[7:0]		ex1OpUIxt;
wire			ex1Hold;
reg				ex1PreBra;
reg[31:0]		ex1IstrWord;	//source instruction word
reg				ex1BraFlush;

reg[5:0]		ex1RegIdRs;		//Source A, ALU / Base
reg[5:0]		ex1RegIdRt;		//Source B, ALU / Index
reg[5:0]		ex1RegIdRm;		//Source C, MemStore
reg[63:0]		ex1RegValRs;		//Source A Value
reg[63:0]		ex1RegValRt;		//Source B Value
reg[63:0]		ex1RegValRm;		//Source C Value

// `ifdef jx2_enable_fpu
reg[63:0]		ex1RegValFRs;		//Source A Value (FPR)
reg[63:0]		ex1RegValFRt;		//Source B Value (FPR)
// `endif

reg[63:0]		ex1RegValCRm;		//Source C Value (CR)

wire[5:0]		ex1RegIdRn1;		//Destination ID (EX1)
wire[63:0]		ex1RegValRn1;		//Destination Value (EX1)
wire[4:0]		ex1RegIdCn1;		//Destination ID (CR, EX1)
wire[63:0]		ex1RegValCn1;		//Destination Value (CR, EX1)

wire[5:0]		ex1HldIdRn1;		//Held Dest ID (EX1)
wire[4:0]		ex1HldIdCn1;		//Held Dest ID (CR, EX1)
	
reg[47:0]		ex1RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex1RegValImm;		//Immediate (Decode)
	
wire[63:0]		ex1RegOutDlr;
reg[63:0]		ex1RegInDlr;
wire[63:0]		ex1RegOutDhr;
reg[63:0]		ex1RegInDhr;
wire[63:0]		ex1RegOutSp;
reg[63:0]		ex1RegInSp;

wire[47:0]		ex1RegOutLr;
reg[47:0]		ex1RegInLr;
wire[63:0]		ex1RegOutSr;
reg[63:0]		ex1RegInSr;

wire[47:0]		ex1MemAddr;
wire[ 4:0]		ex1MemOpm;
wire[63:0]		ex1MemDataOut;
wire[63:0]		ex1MemDataOutB;

wire[ 7:0]		ex1RegOutSchm;
reg [ 7:0]		ex1RegInSchm;
wire[15:0]		ex1TrapExc;

ExEX1	ex1(
	clock,			reset,
	ex1OpUCmd,		ex1OpUIxt,
	ex1Hold,		ex1TrapExc,

	ex1RegIdRs,		ex1RegIdRt,		ex1RegIdRm,
	ex1RegValRs,	ex1RegValRt,	ex1RegValRm,
	ex1RegValFRs,	ex1RegValFRt,	ex1RegValCRm,

	ex1RegIdRn1,	ex1RegValRn1,
	ex1RegIdCn1,	ex1RegValCn1,
	ex1HldIdRn1,	ex1HldIdCn1,
	
	ex1RegValPc,	ex1RegValImm,
	ex1FpuValGRn,	ex1FpuSrT,
	ex1BraFlush,	ex1PreBra,
	
	ex1RegOutDlr,	ex1RegInDlr,
	ex1RegOutDhr,	ex1RegInDhr,
	ex1RegOutSp,	ex1RegInSp,

	ex1RegOutLr,	ex1RegInLr,
	ex1RegOutSr,	ex1RegInSr,
	ex1RegOutSchm,	ex1RegInSchm,

	ex1MemAddr,		ex1MemOpm,
	ex1MemDataOut,	ex1MemDataOutB
	);

wire[65:0]	ex1ValAlu;
// wire		ex1AluSrT;
ExALU	exAlu(
	clock,				reset,
	ex1RegValRs,		ex1RegValRt,
	ex1OpUCmd,			ex1OpUIxt,
	exHold2,			ex1RegInSr[1:0],
//	ex1ValAlu,			ex1AluSrT);
	ex1ValAlu[63:0],	ex1ValAlu[65:64]);

// ExMul	ex1Mul(
ExMulB	ex1Mul(
	clock,				reset,
	ex1RegValRs[31:0],	ex1RegValRt[31:0],
	ex1OpUCmd,			ex1OpUIxt,
	exHold2,			ex1MulVal
	);

ExMulW	ex1MulW(
	clock,				reset,
	ex1RegValRs,		ex1RegValRt,
	ex1OpUCmd,			ex1OpUIxt,
	exHold2,			ex1MulWVal
	);

`ifdef jx2_enable_fpu

`ifdef jx2_use_fpu_w

assign	ex1FpuValLdGRn = UV64_00;

FpuExOpW	ex1Fpu(
	clock,			reset,

	ex1OpUCmd,		ex1OpUIxt,
	exB1OpUCmd,		exB1OpUIxt,

	ex1RegIdRs,		ex1RegValRs,
	ex1RegIdRt,		ex1RegValRt,
	ex1RegIdRm,		ex1RegValRm,

	exB1RegIdRs,	exB1RegValRs,
	exB1RegIdRt,	exB1RegValRt,
	exB1RegIdRm,	exB1RegValRm,

	ex1FpuOK,		ex1FpuSrT,
	
	ex2RegInSr,		ex1BraFlush,
	exHold2,
	
	ex1FpuValGRn,	exB1FpuValGRn
	);
`else

FpuExOp	ex1Fpu(
	clock,			reset,

	ex1OpUCmd,		ex1OpUIxt,
	ex1RegIdRs,		ex1RegValFRs,
	ex1RegIdRt,		ex1RegValFRt,
	ex1RegIdRm,		ex1RegValRs,
	ex1RegIdFRn,	ex1RegValFRn,
	ex1FpuOK,		ex1FpuSrT,
	
//	ex1RegInSr,		ex1BraFlush,
	ex2RegInSr,		ex1BraFlush,
	exHold2,
	
	ex1RegValRs,	ex1FpuValGRn,
	ex1FpuValLdGRn,
	ex2MemDataIn,	ex2MemDataOK
	);
`endif

`endif

/* EX2 */

reg[47:0]		ex2ValBPc;

reg[7:0]		ex2OpUCmd;
reg[7:0]		ex2OpUIxt;
wire			ex2Hold;
reg				ex2PreBra;
reg[31:0]		ex2IstrWord;	//source instruction word

reg[5:0]		ex2RegIdRs;		//Source A, ALU / Base
reg[5:0]		ex2RegIdRt;		//Source B, ALU / Index
reg[5:0]		ex2RegIdRm;		//Source C, MemStore
reg[63:0]		ex2RegValRs;		//Source A Value
reg[63:0]		ex2RegValRt;		//Source B Value
reg[63:0]		ex2RegValRm;		//Source C Value

reg[63:0]		ex2RegValFRs;		//Source A Value (FPR)
reg[63:0]		ex2RegValFRt;		//Source B Value (FPR)
reg[63:0]		ex2RegValCRm;		//Source C Value (CR)

reg[5:0]		ex2RegIdRn1;		//Destination ID (EX1)
reg[63:0]		ex2RegValRn1;		//Destination Value (EX1)
reg[4:0]		ex2RegIdCn1;		//Destination ID (CR, EX1)
reg[63:0]		ex2RegValCn1;		//Destination Value (CR, EX1)
	
wire[5:0]		ex2RegIdRn2;		//Destination ID (EX1)
wire[63:0]		ex2RegValRn2;		//Destination Value (EX1)
wire[4:0]		ex2RegIdCn2;		//Destination ID (CR, EX1)
wire[63:0]		ex2RegValCn2;		//Destination Value (CR, EX1)
	
reg[47:0]		ex2RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex2RegValImm;		//Immediate (Decode)
reg[65:0]		ex2RegAluRes;		//Arithmetic Result
reg[63:0]		ex2RegMulRes;		//Multiplier Result
reg[63:0]		ex2RegMulWRes;		//Multiplier Result (Word)
// reg[63:0]		ex2RegFpuGRn;		//FPU GPR Result
reg				ex2BraFlush;		//Flush EX2

wire[63:0]		ex2RegOutDlr;
reg[63:0]		ex2RegInDlr;
wire[63:0]		ex2RegOutDhr;
reg[63:0]		ex2RegInDhr;
wire[63:0]		ex2RegOutSp;
reg[63:0]		ex2RegInSp;

wire[47:0]		ex2RegOutLr;
reg[47:0]		ex2RegInLr;
wire[63:0]		ex2RegOutSr;
// reg[63:0]		ex2RegInSr;

wire[7:0]		ex2RegOutSchm;
reg[7:0]		ex2RegInSchm;
reg[7:0]		ex2RegInLastSr;

// reg[63:0]		ex2MemDataIn;
// reg[1:0]		ex2MemDataOK;

ExEX2	ex2(
	clock,			reset,
	ex2OpUCmd,		ex2OpUIxt,
	ex2Hold,

	ex2RegIdRs,		ex2RegIdRt,		ex2RegIdRm,
	ex2RegValRs,	ex2RegValRt,	ex2RegValRm,
	ex2RegValFRs,	ex2RegValFRt,	ex2RegValCRm,

	ex2RegIdRn1,	ex2RegValRn1,
	ex2RegIdCn1,	ex2RegValCn1,

	ex2RegIdRn2,	ex2RegValRn2,
	ex2RegIdCn2,	ex2RegValCn2,
	
	ex2RegValPc,	ex2RegValImm,
	ex2RegAluRes,	ex2RegMulRes,
	ex2RegMulWRes,

	ex1FpuValGRn,	ex1FpuValLdGRn,
	ex1FpuSrT,		ex1FpuOK,
	
	ex2BraFlush,
	ex2RegInLastSr,
	
	ex2RegOutDlr,	ex2RegInDlr,
	ex2RegOutDhr,	ex2RegInDhr,
	ex2RegOutSp,	ex2RegInSp,

	ex2RegOutLr,	ex2RegInLr,
	ex2RegOutSr,	ex2RegInSr,
	ex2RegOutSchm,	ex2RegInSchm,

	ex2MemDataIn,
	ex2MemDataInB,
	ex2MemDataOK
	);


`ifdef jx2_enable_wex
/* EX1, Lane 2 */

wire[63:0]		exB1MulWVal;

reg[7:0]		exB1OpUCmd;
reg[7:0]		exB1OpUIxt;
wire			exB1Hold;

reg[5:0]		exB1RegIdRs;		//Source A, ALU / Base
reg[5:0]		exB1RegIdRt;		//Source B, ALU / Index
reg[5:0]		exB1RegIdRm;		//Source C, MemStore
reg[63:0]		exB1RegValRs;		//Source A Value
reg[63:0]		exB1RegValRt;		//Source B Value
reg[63:0]		exB1RegValRm;		//Source C Value

wire[5:0]		exB1RegIdRn1;		//Destination ID (EX1)
wire[63:0]		exB1RegValRn1;		//Destination Value (EX1)
wire[5:0]		exB1HldIdRn1;		//Held Dest ID (EX1)

reg[32:0]		exB1RegValImm;		//Immediate (Decode)

ExEXB1	exb1(
	clock,			reset,
	exB1OpUCmd,		exB1OpUIxt,
	exB1Hold,
	
	exB1RegIdRs,	exB1RegIdRt,
	exB1RegIdRm,	exB1RegValRs,
	exB1RegValRt,	exB1RegValRm,

	exB1RegIdRn1,	exB1RegValRn1,
	exB1HldIdRn1,
	
	ex1RegValPc,	exB1RegValImm,
	ex1BraFlush,	ex1RegInSr
	);
	

wire[65:0]	exB1ValAlu;
// ExALU	exAluB(
ExALUB	exAluB(
	clock,				reset,
	exB1RegValRs,		exB1RegValRt,
	exB1OpUCmd,			exB1OpUIxt,
	exHold2,			ex1RegInSr[1:0],
	exB1ValAlu[63:0],	exB1ValAlu[65:64]);

ExMulW	exB1MulW(
	clock,				reset,
	exB1RegValRs,		exB1RegValRt,
	exB1OpUCmd,			exB1OpUIxt,
	exHold2,			exB1MulWVal
	);


`endif

`ifdef jx2_enable_wex

/* EX2, Lane 2 */

reg[7:0]		exB2OpUCmd;
reg[7:0]		exB2OpUIxt;
wire			exB2Hold;

reg[5:0]		exB2RegIdRs;		//Source A, ALU / Base
reg[5:0]		exB2RegIdRt;		//Source B, ALU / Index
reg[5:0]		exB2RegIdRm;		//Source C, MemStore
reg[63:0]		exB2RegValRs;		//Source A Value
reg[63:0]		exB2RegValRt;		//Source B Value
reg[63:0]		exB2RegValRm;		//Source C Value

reg[5:0]		exB2RegIdRn1;		//Destination ID (EX1)
reg[63:0]		exB2RegValRn1;		//Destination Value (EX1)
wire[5:0]		exB2RegIdRn2;		//Destination ID (EX1)
wire[63:0]		exB2RegValRn2;		//Destination Value (EX1)
	
reg[32:0]		exB2RegValImm;		//Immediate (Decode)
reg[65:0]		exB2RegAluRes;		//Arithmetic Result
reg[63:0]		exB2RegMulWRes;		//Word Multiply Result

ExEXB2		exb2(
	clock,		reset,
	exB2OpUCmd,	exB2OpUIxt,
	exB2Hold,
	
	exB2RegIdRs,	exB2RegIdRt,
	exB2RegIdRm,	exB2RegValRs,
	exB2RegValRt,	exB2RegValRm,
	exB2RegIdRn1,	exB2RegValRn1,
	exB2RegIdRn2,	exB2RegValRn2,
	
	ex2RegValPc,
	exB2RegValImm,	exB2RegAluRes,
	exB2RegMulWRes,
	exB1FpuValGRn,
	ex2BraFlush,

	ex2RegInLastSr,	ex2RegInSr,
	ex2MemDataIn,	ex2MemDataInB
	);

`endif


`ifdef jx2_enable_wex3w
/* EX1, Lane 3 */

wire[63:0]		exC1MulWVal;

reg[7:0]		exC1OpUCmd;
reg[7:0]		exC1OpUIxt;
wire			exC1Hold;

reg[5:0]		exC1RegIdRs;		//Source A, ALU / Base
reg[5:0]		exC1RegIdRt;		//Source B, ALU / Index
reg[5:0]		exC1RegIdRm;		//Source C, MemStore
reg[63:0]		exC1RegValRs;		//Source A Value
reg[63:0]		exC1RegValRt;		//Source B Value
reg[63:0]		exC1RegValRm;		//Source C Value

wire[5:0]		exC1RegIdRn1;		//Destination ID (EX1)
wire[63:0]		exC1RegValRn1;		//Destination Value (EX1)
wire[5:0]		exC1HldIdRn1;		//Held Dest ID (EX1)

reg[32:0]		exC1RegValImm;		//Immediate (Decode)

ExEXB1	exc1(
	clock,			reset,
	exC1OpUCmd,		exC1OpUIxt,
	exC1Hold,
	
	exC1RegIdRs,	exC1RegIdRt,
	exC1RegIdRm,	exC1RegValRs,
	exC1RegValRt,	exC1RegValRm,

	exC1RegIdRn1,	exC1RegValRn1,
	exC1HldIdRn1,
	
	ex1RegValPc,	exC1RegValImm,
	ex1BraFlush,	ex1RegInSr
	);
	

wire[65:0]	exC1ValAlu;
// ExALU	exAluC(
ExALUB	exAluC(
	clock,				reset,
	exC1RegValRs,		exC1RegValRt,
	exC1OpUCmd,			exC1OpUIxt,
	exHold2,			ex1RegInSr[1:0],
	exC1ValAlu[63:0],	exC1ValAlu[65:64]);

ExMulW	exC1MulW(
	clock,				reset,
	exC1RegValRs,		exC1RegValRt,
	exC1OpUCmd,			exC1OpUIxt,
	exHold2,			exC1MulWVal
	);


`endif

`ifdef jx2_enable_wex3w

/* EX2, Lane 3 */

reg[7:0]		exC2OpUCmd;
reg[7:0]		exC2OpUIxt;
wire			exC2Hold;

reg[5:0]		exC2RegIdRs;		//Source A, ALU / Base
reg[5:0]		exC2RegIdRt;		//Source B, ALU / Index
reg[5:0]		exC2RegIdRm;		//Source C, MemStore
reg[63:0]		exC2RegValRs;		//Source A Value
reg[63:0]		exC2RegValRt;		//Source B Value
reg[63:0]		exC2RegValRm;		//Source C Value

reg[5:0]		exC2RegIdRn1;		//Destination ID (EX1)
reg[63:0]		exC2RegValRn1;		//Destination Value (EX1)
wire[5:0]		exC2RegIdRn2;		//Destination ID (EX1)
wire[63:0]		exC2RegValRn2;		//Destination Value (EX1)
	
reg[32:0]		exC2RegValImm;		//Immediate (Decode)
reg[65:0]		exC2RegAluRes;		//Arithmetic Result
reg[63:0]		exC2RegMulWRes;		//Word Multiply Result

ExEXB2		exc2(
	clock,		reset,
	exC2OpUCmd,	exC2OpUIxt,
	exC2Hold,
	
	exC2RegIdRs,	exC2RegIdRt,
	exC2RegIdRm,	exC2RegValRs,
	exC2RegValRt,	exC2RegValRm,
	exC2RegIdRn1,	exC2RegValRn1,
	exC2RegIdRn2,	exC2RegValRn2,
	
	ex2RegValPc,
	exC2RegValImm,	exC2RegAluRes,
	exC2RegMulWRes,
	exC1FpuValGRn,
	ex2BraFlush,

	ex2RegInLastSr,	ex2RegInSr,
	ex2MemDataIn,	ex2MemDataInB
	);

`endif


reg[31:0]	tValStepPc;

reg[47:0]	tValNextPc;
reg[47:0]	tOpNextPc;

reg[47:0]	tValBraPc;
reg[47:0]	tValNextBraPc;

reg[7:0]	opBraFlushMask;
reg[7:0]	nxtBraFlushMask;

reg[63:0]	tNxtRegExc;
reg[63:0]	tRegExc;
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

always @*
begin
	exHold1			= 0;
	exHold1A		= 0;
	exHold1B		= 0;
	exHold1C		= 0;
	exHold1D		= 0;
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

	tHoldNxtCycCnt	= 0;

	crInExsr		= crOutExsr;
	crInSpc			= crOutSpc;
	crInSsp			= crOutSsp;
	crInTea			= crOutTea;
//	tValNextBraPc	= UV32_XX;
//	tValNextBraPc	= UV32_00;
	tValNextBraPc	= UV48_00;

`ifdef jx2_debug_hitmiss
	tBraNxtHitMiss	= tBraHitMiss;
//	tBraNxtCycCnt	= tBraCycCnt + 1;
	tBraNxtCycCnt	= tBraCycCnt;
`endif

`ifndef def_true
	if(ex1Hold)
		exHold2		= 1;
	if(ex2Hold)
		exHold2		= 1;
	if(ifOutPcOK[1])
		exHold2		= 1;
//	if(ex1FpuOK[1])
//		exHold2		= 1;
`endif

//	tDbgOutStatus1	= 0;
//	tDbgOutStatus2	= 0;

	exHold2	=
		(ex1Hold)		|	(ex2Hold)		|
//		(ifOutPcOK[1])	|	(ex1FpuOK[1])	;
		(ifOutPcOK[1])	;
	exHold1A	= exHold2;

//	if(exHold2)
//		exHold1A	= 1;

//	if(	(ex1HldIdRn1 == gprIdRs) ||
//		(ex1HldIdRn1 == gprIdRt) ||
//		((ex1HldIdRn1 == gprIdRm) &&
//			(id2IdUCmd[5:0]==JX2_UCMD_MOV_RM)))

`ifndef def_true
	if(	(ex1RegIdRm == gprIdRs) ||
		(ex1RegIdRm == gprIdRt) ||
		(ex1RegIdRm == gprIdRm)	)
	begin
//		exHold1B	= exHold1 || (ex1HldIdRn1 != JX2_GR_ZZR);
		exHold1B	= (ex1HldIdRn1 != JX2_GR_ZZR);
//		exHold1B	= exHold1 || (ex1RegIdRm != JX2_GR_ZZR);
	end
`endif

`ifdef jx2_enable_wex3w
	exHold1B	=
		((	(ex1RegIdRm == gprIdRs) ||
			(ex1RegIdRm == gprIdRt) ||
			(ex1RegIdRm == gprIdRu) ||
			(ex1RegIdRm == gprIdRv) ||
			(ex1RegIdRm == gprIdRx) ||
			(ex1RegIdRm == gprIdRy)	) &&
			(ex1HldIdRn1 != JX2_GR_ZZR)) ||
		((	(exB1RegIdRm == gprIdRs) ||
			(exB1RegIdRm == gprIdRt) ||
			(exB1RegIdRm == gprIdRu) ||
			(exB1RegIdRm == gprIdRv) ||
			(exB1RegIdRm == gprIdRx) ||
			(exB1RegIdRm == gprIdRy)	) &&
			(exB1HldIdRn1 != JX2_GR_ZZR)) ||
		((	(exC1RegIdRm == gprIdRs) ||
			(exC1RegIdRm == gprIdRt) ||
			(exC1RegIdRm == gprIdRu) ||
			(exC1RegIdRm == gprIdRv) ||
			(exC1RegIdRm == gprIdRx) ||
			(exC1RegIdRm == gprIdRy)	) &&
			(exC1HldIdRn1 != JX2_GR_ZZR));
`else
`ifdef jx2_enable_wex2w
	exHold1B	=
		((	(ex1RegIdRm == gprIdRs) ||
			(ex1RegIdRm == gprIdRt) ||
			(ex1RegIdRm == gprIdRu) ||
			(ex1RegIdRm == gprIdRv)	) &&
			(ex1HldIdRn1 != JX2_GR_ZZR)) ||
		((	(exB1RegIdRm == gprIdRs) ||
			(exB1RegIdRm == gprIdRt) ||
			(exB1RegIdRm == gprIdRu) ||
			(exB1RegIdRm == gprIdRv)	) &&
			(exB1HldIdRn1 != JX2_GR_ZZR));
`else
	exHold1B	=
		(	(ex1RegIdRm == gprIdRs) ||
			(ex1RegIdRm == gprIdRt) ||
			(ex1RegIdRm == gprIdRm)	) &&
		(ex1HldIdRn1 != JX2_GR_ZZR);
`endif
`endif
	
//	if((ex1HldIdRn1 == JX2_GR_SP) ||
//		(ex1HldIdRn1 == JX2_GR_DLR) ||
//		(ex1HldIdRn1 == JX2_GR_DHR))

//	if(ex1HldIdRn1 == JX2_GR_SP)
//	if(ex1RegIdRm == JX2_GR_SP)
//		exHold1C = 1;

	exHold1C = ((ex1RegOutSchm[JX2_SCHM_SP]) ||
		(ex1RegIdRm == JX2_GR_SP)) && !ex1BraFlush;
//		(ex1HldIdRn1 == JX2_GR_SP);

`ifndef def_true
// `ifdef def_true
	if(gprIdRm == JX2_GR_SP)
		tNxtPreHold1 = 1;

	if(tPreHold1 && !tLstPreHold1)
//	if(tPreHold1)
	begin
		exHold1C = 1;
		tNxtPreHold1 = 0;
	end
`endif

//	if(ex1HldIdCn1 == crIdCm)
//	begin
//		exHold1C		= exHold1 | ({1'b1, ex1HldIdCn1} != JX2_CR_ZZR);
//	end

//	if( ({1'b1, ex1HldIdCn1} != JX2_CR_ZZR) &&
//			({1'b1, ex1HldIdCn1} != JX2_CR_PC))
//		exHold1C = 1;

//	if(ex1RegOutSchm[JX2_SCHM_SP])
//		exHold1C = 1;

	if( ({1'b1, ex1RegIdCn1} != JX2_CR_ZZR) &&
			({1'b1, ex1RegIdCn1} != JX2_CR_PC))
		exHold1D = 1;

	if( ({1'b1, ex2RegIdCn2} != JX2_CR_ZZR) &&
			({1'b1, ex2RegIdCn2} != JX2_CR_PC))
		exHold1D = 1;

`ifndef def_true
	if(ex1RegOutSchm[JX2_SCHM_DLR])
	begin
		if(	(gprIdRs==JX2_GR_DLR) ||
			(gprIdRt==JX2_GR_DLR) ||
			(gprIdRm==JX2_GR_DLR) )
				exHold1C = 1;
	end
	if(ex1RegOutSchm[JX2_SCHM_DHR])
	begin
		if(	(gprIdRs==JX2_GR_DHR) ||
			(gprIdRt==JX2_GR_DHR) ||
			(gprIdRm==JX2_GR_DHR) )
				exHold1C = 1;
	end
	if(ex1RegOutSchm[JX2_SCHM_SP])
	begin
		if(	(gprIdRs==JX2_GR_SP) ||
			(gprIdRt==JX2_GR_SP) ||
			(gprIdRm==JX2_GR_SP) )
				exHold1C = 1;
	end
`endif

//	exHold1		= exHold1A | exHold1B | exHold1C | exHold1D;
	exHold1		= exHold1A || exHold1B || exHold1C || exHold1D;

	tDbgOutStatus1	= exHold1A;
	tDbgOutStatus2	= exHold1B;
	tDbgOutStatus3	= exHold1C;
	tDbgOutStatus4	= exHold1D;

	tDbgOutStatus5	= ifOutPcOK[1];
	tDbgOutStatus6	= dcOutOK[1];
	tDbgOutStatus7	= memOK[1];
	tDbgOutStatus8	= memOpm[3] || memOpm[4];

`ifdef jx2_debug_expipe
// `ifndef def_true
//	if(exHold1)
	if(exHold1 &&
		!((ex1OpUCmd[5:0]==JX2_UCMD_OP_IXT) && 
			(ex1OpUIxt[5:0]==JX2_UCIX_IXT_BREAK)))
	begin
		$display("ExHold: A=%d B=%d C=%d D=%d",
			exHold1A, exHold1B, exHold1C, exHold1D);

//		exHold2	=
//			(ex1Hold)		|	(ex2Hold)		|
//			(ifOutPcOK[1])	|	(ex1FpuOK[1])	;
	
		if(exHold2)
		begin
			$display("ExHold2: Ex1=%d Ex2=%d I$=%d FPU=%d",
				ex1Hold, ex2Hold, ifOutPcOK[1], ex1FpuOK[1]);
		end
	end
`endif

`ifdef def_true
	if(exHold1)
	begin
		tHoldNxtCycCnt	= tHoldCycCnt + 1;
		if(tHoldCycCnt==65535)
		begin
			tHoldNxtCycCnt	= 65535;
		end
		else
		if(tHoldCycCnt>65280)
		begin
//			tHoldNxtCycCnt	= tHoldCycCnt;
			tHoldNxtCycCnt	= 65535;
			$display("ExUnit: Deadlock Detected");

`ifdef jx2_enable_wex
			$display("ID1: PC0=%X PC2=%X D=%X-%X OpA=%X-%X OpB=%X-%X F=%d",
				id1ValBPc,	id1ValPc,
				id1IstrWord[15: 0], id1IstrWord[31:16],
				idA1IdUCmd, idA1IdUIxt,
				idB1IdUCmd, idB1IdUIxt,
				opBraFlushMask[2]);
			$display("     Rs=%X Rt=%X Ru=%X Rv=%X Rm=%X Rn=%X",
				idA1IdRegM, idA1IdRegO, idB1IdRegM, idB1IdRegO,
				idA1IdRegN, idB1IdRegN);
`ifdef jx2_enable_wex3w
			$display("     Rx=%X Ry=%X Ro=%X",
				idC1IdRegM, idC1IdRegO, idC1IdRegN);
`endif

			$display("ID2: PC0=%X PC2=%X D=%X-%X OpA=%X-%X OpB=%X-%X F=%d",
				id2ValBPc,	gprValPc,
				id2IstrWord[15: 0], id2IstrWord[31:16],
				idA2IdUCmd, idA2IdUIxt, idB2IdUCmd, idB2IdUIxt,
				opBraFlushMask[1]);
			$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
				gprIdRs, gprValRs,
				gprIdRt, gprValRt,
				gprIdRm, gprValRm);
			$display("     Ru=%X(%X) Rv=%X(%X) Rn=%X(%X)",
				gprIdRu, gprValRu,
				gprIdRv, gprValRv,
				gprIdRn, gprValRn);
`ifdef jx2_enable_wex3w
			$display("     Rx=%X(%X) Ry=%X(%X) Ro=%X(%X)",
				gprIdRx, gprValRx,
				gprIdRy, gprValRy,
				gprIdRo, gprValRo);
`endif

`else
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
`endif

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

`ifdef jx2_enable_wex
			if(exB2OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX2-B: Op=%X-%X",
					exB2OpUCmd, exB2OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exB2RegIdRs, exB2RegValRs,
					exB2RegIdRt, exB2RegValRt,
					exB2RegIdRm, exB2RegValRm);
				$display("     Rn1=%X(%X)",
					exB2RegIdRn2, exB2RegValRn2);
			end

`ifdef jx2_enable_wex3w
			if(exC2OpUCmd[5:0]!=6'h00)
			begin
				$display("  EX2-C: Op=%X-%X",
					exC2OpUCmd, exC2OpUIxt);
				$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
					exC2RegIdRs, exC2RegValRs,
					exC2RegIdRt, exC2RegValRt,
					exC2RegIdRm, exC2RegValRm);
				$display("     Rn1=%X(%X)",
					exC2RegIdRn2, exC2RegValRn2);
			end
`endif
`endif

			$display("");

		end
	end
`endif

//	case( {1'b1, ex1HldIdCn1} )
//		JX2_CR_LR:	 exHold1 = 1;
//		JX2_CR_SR:	 exHold1 = 1;
//		JX2_CR_GBR:	 exHold1 = 1;
//		JX2_CR_TBR:	 exHold1 = 1;
//		default: begin
//		end
//	endcase

	if(tDelayExc[15])
		tNxtRegExc = tDelayExc;

	if(memRegExc[15])
		tNxtRegExc = memRegExc;

//	if(memBusExc[15] && (!opBraFlushMask[3]) &&
//		!exHold1 && !exHold2 && (ex1MemOpm==0))
//	if(memBusExc[15] && (!opBraFlushMask[0]) &&
//		!exHold1 && !exHold2 && !crOutSr[28])
//	if(memBusExc[15] && (!opBraFlushMask[3]) &&
//		!exHold2 && !crOutSr[28])
	if(memBusExc[15] && !crOutSr[28])
	begin
//		if((memBusExc[15:12]!=4'hC) || crOutSr[3])
		if((memBusExc[15:12]!=4'hC) || crOutSr[3])
			tNxtRegExc = memBusExc;
	end
	else if(memBusExc[15])
	begin
		$display("Missed IRQ Flush=%d H1=%d H2=%d SR.BL=%d SR=%X",
			opBraFlushMask[0],
			exHold1, exHold2, crOutSr[28], crOutSr);
	end

	if(ex1TrapExc[15])
		tNxtRegExc = { UV48_00, ex1TrapExc };

	if(reset)
	begin
		exHold1		= 0;
		exHold2		= 0;
	end


	ifInPcHold	= exHold1;
	dcInHold	= exHold2;

//	exHold1		= 0;
//	exHold2		= 0;

	/* Advance PC */
//	tValStepPc		= { 29'b0, ifOutPcStep, 1'b0 };
	tValStepPc		= { 28'b0, ifOutPcStep, 1'b0 };
//	tValNextPc		= crOutPc + tValStepPc;
//	tValNextPc		= ifValPc + tValStepPc;
//	tValNextPc		= ifLastPc + tValStepPc;
	tValNextPc		= { ifLastPc[47:32], ifLastPc[31:0] + tValStepPc };
	tOpNextPc		= tValNextPc;
	/* Hold current PC if branching. */
//	if(opBraFlushMask[2])
	if(opBraFlushMask[3])
//	if(opBraFlushMask[2] || exHold1)
//	if(opBraFlushMask[3] || exHold1)
//		tValNextPc		= ifValPc;
		tValNextPc		= ifLastPc;

	nxtBraFlushMask	= { 1'b0, opBraFlushMask[7:1] };

`ifdef jx2_enable_prebra
// `ifndef def_true
//	if(id1PreBra)
//	if(id1PreBra && !opBraFlushMask[3])		//IF
	if(id1PreBra && !opBraFlushMask[2])		//ID1
//	if(id1PreBra && !opBraFlushMask[1])		//ID2
//	if(id1PreBra && !opBraFlushMask[0])		//EX1
	begin
//		$display("ExUnit PreBra %X", id1PreBraPc);
		tValNextPc = id1PreBraPc;
//		tValNextBraPc = id1PreBraPc;
`ifdef jx2_bra2stage
		nxtBraFlushMask[2] = 1;
//		nxtBraFlushMask[3] = 1;
//		nxtBraFlushMask[4] = 1;
`else
//		nxtBraFlushMask[1] = 1;
		nxtBraFlushMask[2] = 1;
//		nxtBraFlushMask[3] = 1;
`endif
//		nxtBraFlushMask = 8'h0F;
//		nxtBraFlushMask = JX2_BRA_FLUSHMSK;
	end
`endif

//	if((idA1IdUCmd[5:0]==JX2_UCMD_BRA) && idA1IdUCmd[7] && id1PreBra)
//	begin
//		$display("ExUnit: Predict CC Branch ID1");
//	end

	ex1ValBraDir = 3'b001;
//	if(ex1OpUCmd[5:0]==JX2_UCMD_BRA)
//	if(ex1OpUCmd[5:0]==JX2_UCMD_BRA)
	if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && ex1OpUCmd[7] &&
		!ex1BraFlush && !exHold2)
	begin
		ex1ValBraDir[1:0] = ex1OpUCmd[7:6];
//		ex1ValBraDir[2]	= !(ex1OpUCmd[6]^ex2RegOutSr[0]);
		ex1ValBraDir[2]	= (ex1OpUCmd[6]^ex2RegOutSr[0]);

`ifdef jx2_debug_hitmiss
		if({1'b1, ex1RegIdCn1} == JX2_CR_PC)
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

//	if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && ex1OpUCmd[7] && ex1PreBra)
//	begin
//		$display("ExUnit: Predict CC Branch EX1");
//	end

	/* Handle if EX1 unit has initiated a branch. */
//	nxtBraFlushMask	= { 1'b0, opBraFlushMask[7:1] };
//	if(({1'b1, ex1RegIdCn1} == JX2_CR_PC) && !ex1PreBra)
	if({1'b1, ex1RegIdCn1} == JX2_CR_PC)
	begin
//		$display("EX1 BRA %X", ex1RegValCn1);
//		tValNextPc = ex1RegValCn1[31:0];
//		tValNextBraPc = ex1RegValCn1[31:0];
		tValNextBraPc = ex1RegValCn1[47:0];
//		nxtBraFlushMask = 8'h07;
//		nxtBraFlushMask = 8'h0F;
		nxtBraFlushMask = JX2_BRA_FLUSHMSK;
//		ex1ValBraDir[2]	= 1;

`ifndef def_true
//		if(ex1PreBra)
		if((ex1OpUCmd[5:0]==JX2_UCMD_BRA) && ex1OpUCmd[7])
			$display("ExUnit: Branch Miss, CC=%X, Rat=%X, PC=%X Istr=%X-%X",
				ex1OpUCmd[7:6], tBraHitMiss,
				ex1ValBPc,
				ex1IstrWord[15: 0], ex1IstrWord[31:16]);
`endif
	end

`ifndef def_true
	/* EX2 can branch, ignore if already in a branch. */
	if(({1'b1, ex2RegIdCn2} == JX2_CR_PC) && !ex2PreBra &&
		!opBraFlushMask[0])
	begin
//		$display("EX2 BRA %X", ex2RegValCn2);
//		tValNextPc = ex2RegValCn2[31:0];
//		tValNextBraPc = ex2RegValCn2[31:0];
		tValNextBraPc = ex2RegValCn2[47:0];
//		nxtBraFlushMask = 8'h07;
//		nxtBraFlushMask = 8'h0F;
		nxtBraFlushMask = JX2_BRA_FLUSHMSK;
	end
`endif

	if(reset)
	begin
//		tValNextPc = UV32_00;
//		tValNextBraPc = UV32_00;
		tValNextBraPc = UV48_00;
//		nxtBraFlushMask = 8'h07;
//		nxtBraFlushMask = 8'h0F;
		nxtBraFlushMask = JX2_BRA_FLUSHMSK;
	end
	
	case(tRegExc[14:13])
		2'b00: tRegExcOfs=1;
		2'b01: tRegExcOfs=3;
		2'b10: tRegExcOfs=2;
		2'b11: tRegExcOfs=4;
	endcase

	crInSr			= ex1RegOutSr;
	gprInSp			= ex1RegOutSp;

	if(reset)
	begin
		crInSr			= 0;
		gprInSp			= 64'h00000DFF0;
	end

//	tDelayExc		<= tNxtDelayExc;

	tNxtDelayExc	= 0;
	tRegSkipExc		= 0;
	if(tRegExc[15:12]==4'hC)
	begin
//		if(opBraFlushMask[0] || crOutSr[28])
		if(crOutSr[28])
			tRegSkipExc		= 1;
		if(exHold1 || exHold2)
//		if(opBraFlushMask[0] || exHold1 || exHold2)
//		if(exHold2)
		begin
			tRegSkipExc		= 1;
			tNxtDelayExc	= tRegExc;
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
			crInExsr		= crOutExsr;
			crInSr			= { crOutSr[63:32], crOutExsr[63:32] };
//			gprInSp			= { UV32_00, crOutSsp };
			gprInSp			= { UV16_00, crOutSsp };
//			crInSpc			= crOutSpc;
			crInSr[28]		= 0;
		end
		else if(!crOutSr[28])
		begin
			$display("ExUnit: Fault %X, id2.PC=%X ex1.PC=%X ex2.PC=%X",
				tRegExc, id2ValBPc, ex1ValBPc, ex2ValBPc);
		
	//		tValNextPc = crOutVbr;
	//		tValNextPc = {
			tValNextBraPc = {
	//			crOutVbr[31:11],
	//			crOutVbr[10: 3]+tRegExcOfs,
//				crOutVbr[31:8],
				crOutVbr[47:8],
				crOutVbr[7: 3]+tRegExcOfs[4:0],
				crOutVbr[2:0] };
	//		nxtBraFlushMask = 8'h07;
	//		nxtBraFlushMask = 8'h0F;
			nxtBraFlushMask = JX2_BRA_FLUSHMSK;

//			crInExsr		= { crOutSr[31:0], UV16_00, tRegExc[15:0] };
			crInExsr		= { ex1RegOutSr[31:0], UV16_00, tRegExc[15:0] };
//			crInTea			= tRegExc[47:16];
			crInTea			= tRegExc[63:16];
	//		crInSpc			= ifLastPc;
//			crInSpc			= ex2ValBPc;
//			crInSpc			= ex1ValBPc;
//			crInSpc			= id2ValBPc;

			if(!ex2BraFlush)
				crInSpc			= ex2ValBPc;
			else if(!ex1BraFlush)
				crInSpc			= ex1ValBPc;
			else if(!opBraFlushMask[0])
				crInSpc			= id2ValBPc;
			else if(!opBraFlushMask[1])
				crInSpc			= id1ValBPc;
			else
				crInSpc		= ifLastPc;

			$display("ISR PC=%X, SPC=%X", tValNextBraPc, crInSpc);

			if(tRegExc[15:12]==4'b1110)
			begin
//				crInSpc			= id1ValBPc;
				crInSpc			= ex1ValBPc;
			end

//			crInSsp			= gprOutSp[31:0];
//			crInSsp			= ex1RegOutSp[31:0];
			crInSsp			= ex1RegOutSp[47:0];
	//		crInSsr			= crOutSr;
//			crInSr			= crOutSr;
			crInSr			= ex1RegOutSr;
			crInSr[30:28]	= 3'b111;
		end
		else
		begin
			$display("ExUnit: Double-Fault EXC=%X, id2.PC=%X ex1.PC=%X ex2.PC=%X",
				tRegExc, id2ValBPc, ex1ValBPc, ex2ValBPc);
		end
	end

`ifdef jx2_bra2stage
	if(opBraFlushMask[4])
	begin
//		if(crOutSr[28])
//			$display("Branch %X", tValNextBraPc);
		tValNextPc = tValBraPc;
	end
`else
	if(nxtBraFlushMask[3])
	begin
//		if(crOutSr[28])
//			$display("Branch %X", tValNextBraPc);
		tValNextPc = tValNextBraPc;
	end
`endif

	ifValPc			= tValNextPc;

	crInPc			= tValNextPc;
//	crInSr			= ex1RegOutSr;
	crInLr			= ex1RegOutLr;

	gprIdRn1		= ex1RegIdRn1;
	gprValRn1		= ex1RegValRn1;
	gprIdRn2		= ex2RegIdRn2;
	gprValRn2		= ex2RegValRn2;

`ifdef jx2_enable_wex
	gprIdRnB1		= exB1RegIdRn1;
	gprValRnB1		= exB1RegValRn1;
	gprIdRnB2		= exB2RegIdRn2;
	gprValRnB2		= exB2RegValRn2;
	
	exB2RegAluRes	= exB1ValAlu;
`endif

`ifdef jx2_enable_wex3w
	gprIdRnC1		= exC1RegIdRn1;
	gprValRnC1		= exC1RegValRn1;
	gprIdRnC2		= exC2RegIdRn2;
	gprValRnC2		= exC2RegValRn2;
	
	exC2RegAluRes	= exC1ValAlu;
`endif

	crIdCn1			= ex1RegIdCn1;
	crValCn1		= ex1RegValCn1;
	crIdCn2			= ex2RegIdCn2;
	crValCn2		= ex2RegValCn2;
	
// `ifdef jx2_enable_fpu
`ifdef jx2_enable_fprs
	gprIdFRn		= ex1RegIdFRn;
	gprValFRn		= ex1RegValFRn;
`endif


	/* ID2 */

//	gprValGbr		= crOutGbr;
	gprInDlr		= ex1RegOutDlr;
	gprInDhr		= ex1RegOutDhr;
//	gprInSp			= ex1RegOutSp;

`ifdef jx2_sprs_elrehr
	gprInElr		= gprOutElr;
	gprInEhr		= gprOutEhr;
	gprInBp			= gprOutBp;
`endif


	/* EX1 */
//	ex1RegInDlr		= gprOutDlr;
//	ex1RegInDhr		= gprOutDhr;
//	ex1RegInSp		= gprOutSp;
//	ex1RegInLr		= crOutLr;
	ex1RegInDlr		= ex2RegOutDlr;
	ex1RegInDhr		= ex2RegOutDhr;
	ex1RegInSp		= ex2RegOutSp;
	ex1RegInLr		= ex2RegOutLr;

//	ex1RegInSr		= crOutSr;
	ex1RegInSr		= ex2RegOutSr;
//	ex1RegInSchm	= ex2RegOutSchm;
	ex1RegInSchm	= 0;

	ex2RegAluRes	= ex1ValAlu;
	ex2RegMulRes	= ex1MulVal;
	ex2RegMulWRes	= ex1MulWVal;
//	ex2RegFpuGRn	= ex1FpuValGRn;

	dcInAddr		= ex1MemAddr;
	dcInOpm			= ex1MemOpm;
	dcInVal			= ex1MemDataOut;
//	dcInValB		= ex1MemDataOutB;
	dcInValB		= exB1RegValRn1;

//	if(exB1RegIdRn1 == JX2_GR_DCINB)
//		dcInValB	= exB1RegValRn1;

`ifdef jx2_enable_wex
	exB2RegMulWRes	= exB1MulWVal;
`ifdef jx2_enable_wex3w
	exC2RegMulWRes	= exC1MulWVal;
`endif
`endif

	/* EX2 */

	ex2MemDataIn	= dcOutVal;
	ex2MemDataInB	= dcOutValB;
	ex2MemDataOK	= dcOutOK;

	ex2RegInDlr		= gprOutDlr;
	ex2RegInDhr		= gprOutDhr;
	ex2RegInSp		= gprOutSp;
	ex2RegInLr		= crOutLr;
	ex2RegInSr		= crOutSr;
	ex2RegInSchm	= 0;

end



always @(posedge clock)
begin

`ifndef jx2_enable_fpu
		ex1RegValFRs	<= UV64_XX;
		ex1RegValFRt	<= UV64_XX;
`endif

	tPreHold1		<= tNxtPreHold1;
	tLstPreHold1	<= tPreHold1;

//	tRegExc			<= tNxtRegExc;
	tDelayExc		<= tNxtDelayExc;

	tHoldCycCnt		<= tHoldNxtCycCnt;

`ifdef jx2_debug_hitmiss
	tBraHitMiss		<= tBraNxtHitMiss;
	tBraCycCnt		<= tBraNxtCycCnt;
`endif

	if(reset)
	begin
//		ifValPc			<= UV32_00;
//		opBraFlushMask	<= 8'h07;
//		opBraFlushMask	<= 8'h0F;
		opBraFlushMask	<= JX2_BRA_FLUSHMSK;
		ifLastPc		<= 0;
		ifLastPcStep	<= 0;
		ex1BraFlush		<= 1;
		ex2BraFlush		<= 1;

		id1IstrWord		<= 96'h300030003000300030003000;

`ifdef jx2_enable_wex
		gprIdRs			<= JX2_GR_ZZR;
		gprIdRt			<= JX2_GR_ZZR;
		gprIdRu			<= JX2_GR_ZZR;
		gprIdRv			<= JX2_GR_ZZR;
		gprIdRm			<= JX2_GR_ZZR;
		gprIdRn			<= JX2_GR_ZZR;

		idA2IdUCmd		<= UV8_00;
		idA2IdUIxt		<= UV8_00;
		idB2IdUCmd		<= UV8_00;
		idB2IdUIxt		<= UV8_00;

`ifdef jx2_enable_wex3w
		gprIdRx			<= JX2_GR_ZZR;
		gprIdRy			<= JX2_GR_ZZR;
		gprIdRo			<= JX2_GR_ZZR;
		idC2IdUCmd		<= UV8_00;
		idC2IdUIxt		<= UV8_00;
`endif
`endif

		ex1OpUCmd		<= UV8_00;
		ex1OpUIxt		<= UV8_00;
		ex1PreBra		<= 0;

//		ex1IstrWord		<= UV32_XX;
//		ex1RegValPc		<= UV32_XX;
//		ex1RegValImm	<= UV33_XX;

		ex1RegIdRs		<= JX2_GR_ZZR;
		ex1RegIdRt		<= JX2_GR_ZZR;
		ex1RegIdRm		<= JX2_GR_ZZR;

`ifdef jx2_enable_wex
		exB1OpUCmd		<= UV8_00;
		exB1OpUIxt		<= UV8_00;
		exB1RegIdRs		<= JX2_GR_ZZR;
		exB1RegIdRt		<= JX2_GR_ZZR;
		exB1RegIdRm		<= JX2_GR_ZZR;

`ifdef jx2_enable_wex3w
		exC1OpUCmd		<= UV8_00;
		exC1OpUIxt		<= UV8_00;
		exC1RegIdRs		<= JX2_GR_ZZR;
		exC1RegIdRt		<= JX2_GR_ZZR;
		exC1RegIdRm		<= JX2_GR_ZZR;
`endif
`endif

	end
	else
		if(!exHold1)
//		if(!exHold1 || (tNxtRegExc[15:13]==3'b111))
//		if(!opBraFlushMask[3])
	begin
		tRegExc			<= tNxtRegExc;

`ifdef jx2_bra2stage
		tValBraPc		<= tValNextBraPc;
`endif

// `ifndef def_true
// `ifdef def_true
`ifdef jx2_debug_expipe
		$display("IF : PC=%X D=%X-%X-%X-%X Step=%X PC2=%X F=%d", ifLastPc,
			ifIstrWord[15: 0], ifIstrWord[31:16],
			ifIstrWord[47:32], ifIstrWord[63:48],
			ifOutPcStep, tValNextPc, opBraFlushMask[3]);

`ifdef jx2_enable_wex
		$display("ID1: PC0=%X PC2=%X D=%X-%X OpA=%X-%X OpB=%X-%X F=%d",
			id1ValBPc,	id1ValPc,
			id1IstrWord[15: 0], id1IstrWord[31:16],
			idA1IdUCmd, idA1IdUIxt, idB1IdUCmd, idB1IdUIxt, opBraFlushMask[2]);
//		$display("     RsA=%X RtA=%X RnA=%X",
//			idA1IdRegM, idA1IdRegO, idA1IdRegN);
//		$display("     RsB=%X RtB=%X RnB=%X",
//			idB1IdRegM, idB1IdRegO, idB1IdRegN);
		$display("     Rs=%X Rt=%X Ru=%X Rv=%X Rm=%X Rn=%X",
			idA1IdRegM, idA1IdRegO, idB1IdRegM, idB1IdRegO,
			idA1IdRegN, idB1IdRegN);
`ifdef jx2_enable_wex3w
		$display("     Rx=%X Ry=%X Ro=%X",
			idC1IdRegM, idC1IdRegO, idC1IdRegN);
`endif

		$display("ID2: PC0=%X PC2=%X D=%X-%X OpA=%X-%X OpB=%X-%X F=%d",
			id2ValBPc,	gprValPc,
			id2IstrWord[15: 0], id2IstrWord[31:16],
			idA2IdUCmd, idA2IdUIxt, idB2IdUCmd, idB2IdUIxt,
			opBraFlushMask[1]);
		$display("     Rs=%X(%X) Rt=%X(%X) Rm=%X(%X)",
			gprIdRs, gprValRs,
			gprIdRt, gprValRt,
			gprIdRm, gprValRm);
		$display("     Ru=%X(%X) Rv=%X(%X) Rn=%X(%X)",
			gprIdRu, gprValRu,
			gprIdRv, gprValRv,
			gprIdRn, gprValRn);
`ifdef jx2_enable_wex3w
		$display("     Rx=%X(%X) Ry=%X(%X) Ro=%X(%X)",
			gprIdRx, gprValRx,
			gprIdRy, gprValRy,
			gprIdRo, gprValRo);
`endif

`else
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
`endif
//		$display("");
`endif

`ifndef def_true
// `ifdef def_true
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

//		$display("");
`endif

		/* IF */
		ifLastPc		<= ifValPc;
		ifLastPcStep	<= ifOutPcStep;
		opBraFlushMask	<= nxtBraFlushMask;
//		ifValPc			<= tValNextPc;


		/* ID1 */

//		id1ValBPc		<= ifValPc;
		id1ValBPc		<= ifLastPc;
//		id1ValPc		<= ifValPc + tValStepPc;
//		id1ValPc		<= tValNextPc;
		id1ValPc		<= tOpNextPc;
		id1IstrWord		<= ifIstrWord;


		/* ID2 */

`ifdef jx2_enable_wex
		gprIdRs			<= idA1IdRegM;
		gprIdRt			<= idA1IdRegO;
		gprIdRu			<= idB1IdRegM;
		gprIdRv			<= idB1IdRegO;
		gprIdRm			<= idA1IdRegN;
		gprIdRn			<= idB1IdRegN;

		gprValImm		<= idA1IdImm;
		gprValImmB		<= idB1IdImm;

		idA2IdUCmd		<= idA1IdUCmd;
		idA2IdUIxt		<= idA1IdUIxt;
		idB2IdUCmd		<= idB1IdUCmd;
		idB2IdUIxt		<= idB1IdUIxt;

`ifdef jx2_enable_wex3w
		gprIdRx			<= idC1IdRegM;
		gprIdRy			<= idC1IdRegO;
		gprIdRo			<= idC1IdRegN;
		gprValImmC		<= idC1IdImm;
		idC2IdUCmd		<= idC1IdUCmd;
		idC2IdUIxt		<= idC1IdUIxt;
`endif

		crIdCm			<= idA1IdRegM[4:0];

		gprValPc		<= id1ValPc;
		id2ValBPc		<= id1ValBPc;
		id2PreBra		<= id1PreBra;
		id2IstrWord		<= id1IstrWord[31:0];
`else
		gprIdRs			<= id1IdRegM;
		gprIdRt			<= id1IdRegO;
		gprIdRm			<= id1IdRegN;
		gprValPc		<= id1ValPc;
		gprValImm		<= id1IdImm;
		crIdCm			<= id1IdRegM[4:0];

		id2ValBPc		<= id1ValBPc;
		id2IdUCmd		<= id1IdUCmd;
		id2IdUIxt		<= id1IdUIxt;
		id2PreBra		<= id1PreBra;
		id2IstrWord		<= id1IstrWord[31:0];
`endif

		/* EX1 */

		ex1ValBPc		<= id2ValBPc;
//		ex1OpUCmd		<= id2IdUCmd;
//		ex1OpUCmd		<= {
//			opBraFlushMask[0] ? JX2_IXC_NV : id2IdUCmd[7:6],
//			id2IdUCmd[5:0] };
//		ex1OpUIxt		<= id2IdUIxt;
		ex1PreBra		<= id2PreBra;
		ex1IstrWord		<= id2IstrWord;

		ex1RegValPc		<= gprValPc;
		ex1RegValImm	<= gprValImm;
		ex1BraFlush		<= nxtBraFlushMask[0];

`ifdef jx2_enable_wex
		ex1OpUCmd		<= idA2IdUCmd;
		ex1OpUIxt		<= idA2IdUIxt;
		exB1OpUCmd		<= idB2IdUCmd;
		exB1OpUIxt		<= idB2IdUIxt;
		exB1RegValImm	<= gprValImmB;

		ex1RegIdRs		<= gprIdRs;
		ex1RegIdRt		<= gprIdRt;
		ex1RegIdRm		<= gprIdRm;
		ex1RegValRs		<= gprValRs;
		ex1RegValRt		<= gprValRt;
		ex1RegValRm		<= gprValRm;

		exB1RegIdRs		<= gprIdRu;
		exB1RegIdRt		<= gprIdRv;
		exB1RegIdRm		<= gprIdRn;
		exB1RegValRs	<= gprValRu;
		exB1RegValRt	<= gprValRv;
		exB1RegValRm	<= gprValRn;

`ifdef jx2_enable_wex3w
		exC1OpUCmd		<= idC2IdUCmd;
		exC1OpUIxt		<= idC2IdUIxt;
		exC1RegValImm	<= gprValImmC;

		exC1RegIdRs		<= gprIdRx;
		exC1RegIdRt		<= gprIdRy;
		exC1RegIdRm		<= gprIdRo;
		exC1RegValRs	<= gprValRx;
		exC1RegValRt	<= gprValRy;
		exC1RegValRm	<= gprValRo;
`endif

`else
		ex1OpUCmd		<= id2IdUCmd;
		ex1OpUIxt		<= id2IdUIxt;

		ex1RegIdRs		<= gprIdRs;
		ex1RegIdRt		<= gprIdRt;
		ex1RegIdRm		<= gprIdRm;
		ex1RegValRs		<= gprValRs;
		ex1RegValRt		<= gprValRt;
		ex1RegValRm		<= gprValRm;
`endif

		ex1RegValCRm	<= crValCm;

`ifdef jx2_enable_fpu
		ex1RegValFRs	<= gprValFRs;
		ex1RegValFRt	<= gprValFRt;
`endif

	end
	else
		if(!exHold2)
	begin
//		ex1OpUCmd		<= { JX2_IXC_NV, ex1OpUCmd[5:0] };
		ex1OpUCmd		<= { JX2_IXC_NV, JX2_UCMD_NOP };
		ex1BraFlush		<= 1;

`ifdef jx2_enable_wex
		exB1OpUCmd		<= { JX2_IXC_NV, JX2_UCMD_NOP };
		exB1OpUIxt		<= UV8_00;
`endif

`ifdef jx2_enable_wex3w
		exC1OpUCmd		<= { JX2_IXC_NV, JX2_UCMD_NOP };
		exC1OpUIxt		<= UV8_00;
`endif

`ifdef def_true
//		ex1ValBPc		<= UV32_XX;
		ex1ValBPc		<= ex1ValBPc;
//		ex1OpUCmd		<= UV8_XX;
//		ex1OpUIxt		<= UV8_XX;
		ex1OpUIxt		<= UV8_00;
		ex1PreBra		<= 0;
		ex1IstrWord		<= UV32_XX;

//		ex1RegValPc		<= UV32_XX;
		ex1RegValPc		<= UV48_XX;
		ex1RegValImm	<= UV33_XX;

		ex1RegIdRs		<= JX2_GR_ZZR;
		ex1RegIdRt		<= JX2_GR_ZZR;
		ex1RegIdRm		<= JX2_GR_ZZR;
		ex1RegValRs		<= UV64_XX;
		ex1RegValRt		<= UV64_XX;
		ex1RegValRm		<= UV64_XX;

		ex1RegValCRm	<= UV64_XX;

`ifdef jx2_enable_fpu
		ex1RegValFRs	<= UV64_XX;
		ex1RegValFRt	<= UV64_XX;
`endif

`ifdef jx2_enable_wex
		exB1RegValImm	<= UV33_XX;

		exB1RegIdRs		<= JX2_GR_ZZR;
		exB1RegIdRt		<= JX2_GR_ZZR;
		exB1RegIdRm		<= JX2_GR_ZZR;
		exB1RegValRs	<= UV64_XX;
		exB1RegValRt	<= UV64_XX;
		exB1RegValRm	<= UV64_XX;
`endif

`ifdef jx2_enable_wex3w
		exC1RegValImm	<= UV33_XX;

		exC1RegIdRs		<= JX2_GR_ZZR;
		exC1RegIdRt		<= JX2_GR_ZZR;
		exC1RegIdRm		<= JX2_GR_ZZR;
		exC1RegValRs	<= UV64_XX;
		exC1RegValRt	<= UV64_XX;
		exC1RegValRm	<= UV64_XX;
`endif

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

`ifdef jx2_enable_wex
		if(exB2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-B: Op=%X-%X",
				exB2OpUCmd, exB2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exB2RegIdRs, exB2RegValRs,
				exB2RegIdRt, exB2RegValRt,
				exB2RegIdRm, exB2RegValRm);
			$display("     Rn1=%X(%X)",
				exB2RegIdRn2, exB2RegValRn2);
		end

`ifdef jx2_enable_wex3w
		if(exC2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-C: Op=%X-%X",
				exC2OpUCmd, exC2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exC2RegIdRs, exC2RegValRs,
				exC2RegIdRt, exC2RegValRt,
				exC2RegIdRm, exC2RegValRm);
			$display("     Rn1=%X(%X)",
				exC2RegIdRn2, exC2RegValRn2);
		end
`endif
`endif

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

`ifdef jx2_enable_wex

		if(exB2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-B: Op=%X-%X",
				exB2OpUCmd, exB2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exB2RegIdRs, exB2RegValRs,
				exB2RegIdRt, exB2RegValRt,
				exB2RegIdRm, exB2RegValRm);
			$display("     Rn1=%X(%X)",
				exB2RegIdRn2, exB2RegValRn2);
		end

`ifdef jx2_enable_wex3w
		if(exC2OpUCmd[5:0]!=6'h00)
		begin
			$display("  EX2-C: Op=%X-%X",
				exC2OpUCmd, exC2OpUIxt);
			$display("     Rs=%X(%X) Rt=%X(%X) Rn=%X(%X)",
				exC2RegIdRs, exC2RegValRs,
				exC2RegIdRt, exC2RegValRt,
				exC2RegIdRm, exC2RegValRm);
			$display("     Rn1=%X(%X)",
				exC2RegIdRn2, exC2RegValRn2);
		end
`endif
`endif

//		$display("");
`endif

		/* EX2 */
		ex2OpUCmd		<= ex1OpUCmd;
		ex2OpUIxt		<= ex1OpUIxt;
		ex2PreBra		<= ex1PreBra;
		ex2IstrWord		<= ex1IstrWord;
		ex2ValBPc		<= ex1ValBPc;
		ex2BraFlush		<= ex1BraFlush;
		ex2RegInLastSr	<= ex1RegInSr[7:0];

		ex2RegIdRs		<= ex1RegIdRs;
		ex2RegIdRt		<= ex1RegIdRt;
		ex2RegIdRm		<= ex1RegIdRm;
		ex2RegValRs		<= ex1RegValRs;
		ex2RegValRt		<= ex1RegValRt;
		ex2RegValRm		<= ex1RegValRm;
		ex2RegValFRs	<= ex1RegValFRs;
		ex2RegValFRt	<= ex1RegValFRt;
		ex2RegValCRm	<= ex1RegValCRm;
		ex2RegIdRn1		<= ex1RegIdRn1;
		ex2RegValRn1	<= ex1RegValRn1;
		ex2RegIdCn1		<= ex1RegIdCn1;
		ex2RegValCn1	<= ex1RegValCn1;
		ex2RegValPc		<= ex1RegValPc;
		ex2RegValImm	<= ex1RegValImm;

`ifdef jx2_enable_wex
		exB2OpUCmd		<= exB1OpUCmd;
		exB2OpUIxt		<= exB1OpUIxt;

		exB2RegIdRs		<= exB1RegIdRs;
		exB2RegIdRt		<= exB1RegIdRt;
		exB2RegIdRm		<= exB1RegIdRm;
		exB2RegValRs	<= exB1RegValRs;
		exB2RegValRt	<= exB1RegValRt;
		exB2RegValRm	<= exB1RegValRm;
		exB2RegIdRn1	<= exB1RegIdRn1;
		exB2RegValRn1	<= exB1RegValRn1;
		exB2RegValImm	<= exB1RegValImm;

`ifdef jx2_enable_wex3w
		exC2OpUCmd		<= exC1OpUCmd;
		exC2OpUIxt		<= exC1OpUIxt;

		exC2RegIdRs		<= exC1RegIdRs;
		exC2RegIdRt		<= exC1RegIdRt;
		exC2RegIdRm		<= exC1RegIdRm;
		exC2RegValRs	<= exC1RegValRs;
		exC2RegValRt	<= exC1RegValRt;
		exC2RegValRm	<= exC1RegValRm;
		exC2RegIdRn1	<= exC1RegIdRn1;
		exC2RegValRn1	<= exC1RegValRn1;
		exC2RegValImm	<= exC1RegValImm;
`endif

`endif

		/* WB */
	
	end

end

endmodule
