/*

IF ID1 ID2 EX1 EX2 WB

 */

`include "CoreDefs.v"

`include "MemL1A.v"
`include "DecOp.v"
`include "RegGPR.v"
`include "RegCR.v"

`include "ExEX1.v"
`include "ExEX2.v"
`include "ExALU.v"
// `include "ExMul.v"
`include "ExMulB.v"

`ifdef jx2_enable_prebra
`include "DecPreBra.v"
`endif

`ifdef jx2_enable_fpu
`ifdef jx2_enable_fprs
`include "RegFPR.v"
`endif
`include "FpuExOp.v"
`endif

module ExUnit(
	clock,
	reset,
	
	memAddr,		memOpm,
	memDataIn,		memDataOut,
	memOK,
	
	dbgOutPc,		dbgOutIstr,
	dbgExHold1,		dbgExHold2,

	dbgDcInAddr,	dbgDcInOpm,
	dbgDcOutVal,	dbgDcInVal,
	dbgDcOutOK
	);

input			clock;
input			reset;

output[31:0]	memAddr;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
output[4:0]		memOpm;
input[1:0]		memOK;

output[31:0]	dbgOutPc;
output[63:0]	dbgOutIstr;

output			dbgExHold1;
output			dbgExHold2;

output[31:0]	dbgDcInAddr;
output[ 4:0]	dbgDcInOpm;
output[63:0]	dbgDcOutVal;
output[63:0]	dbgDcInVal;
output[ 1:0]	dbgDcOutOK;

reg				exHold1;
reg				exHold2;

assign		dbgExHold1 = exHold1;
assign		dbgExHold2 = exHold2;


/* IF */

wire[63:0]		gprOutDlr;
wire[63:0]		gprOutDhr;

wire[63:0]		crOutMmcr;
wire[63:0]		crOutKrr;

reg[31:0]		ifValPc;
reg[31:0]		ifLastPc;
wire[63:0]		ifIstrWord;	//source instruction word
wire[1:0]		ifOutPcOK;
wire[1:0]		ifOutPcStep;
reg[1:0]		ifLastPcStep;
reg				ifInPcHold;

assign	dbgOutPc	= ifLastPc;
assign	dbgOutIstr	= ifIstrWord;

reg [31:0]		dcInAddr;
reg [ 4:0]		dcInOpm;
wire[63:0]		dcOutVal;
reg [63:0]		dcInVal;
wire[ 1:0]		dcOutOK;
reg				dcInHold;

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
	ifInPcHold,

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutOK,		dcInHold,

	gprOutDlr,		gprOutDhr,
	crOutMmcr,		crOutKrr,
	memRegExc,

	memAddr,		memOpm,
	memDataIn,		memDataOut,
	memOK
	);

/* ID1 */

wire[31:0]		gprValGbr;
wire[31:0]		gprValLr;


reg[31:0]		id1ValBPc;
reg[31:0]		id1ValPc;
reg[63:0]		id1IstrWord;	//source instruction word

wire[5:0]		id1IdRegN;
wire[5:0]		id1IdRegM;
wire[5:0]		id1IdRegO;
wire[32:0]		id1IdImm;
wire[7:0]		id1IdUCmd;
wire[7:0]		id1IdUIxt;

DecOp	decOp(
	clock,	reset,
	id1IstrWord,
	id1IdRegN,	id1IdRegM,
	id1IdRegO,	id1IdImm,
	id1IdUCmd,	id1IdUIxt
	);

`ifdef jx2_enable_prebra
wire[31:0]		id1PreBraPc;
wire			id1PreBra;

DecPreBra	preBra(
	id1IstrWord,	id1ValPc,
	id1PreBraPc,	id1PreBra,
	gprValLr);
`else
wire			id1PreBra;
assign		id1PreBra = 0;
`endif
	

/* ID2 */

reg[31:0]		id2ValBPc;
reg[7:0]		id2IdUCmd;
reg[7:0]		id2IdUIxt;
reg				id2PreBra;
reg[31:0]		id2IstrWord;	//source instruction word


/* ID2, GPR */

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

reg [31:0]		gprValPc;
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
wire[63:0]		gprOutElr;
reg [63:0]		gprInElr;
wire[63:0]		gprOutEhr;
reg [63:0]		gprInEhr;
wire[63:0]		gprOutSp;
reg [63:0]		gprInSp;
wire[63:0]		gprOutBp;
reg [63:0]		gprInBp;

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
	gprOutElr,	gprInElr,
	gprOutEhr,	gprInEhr,
	gprOutSp,	gprInSp,
	gprOutBp,	gprInBp
	);


/* ID2, FPR */

wire[63:0]		crOutSr;

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

assign			gprValFRs = id2IdIxt[4] ? gprValRs : gprValFRs2;
assign			gprValFRt = id2IdIxt[4] ? gprValRt : gprValFRt2;

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
wire[31:0]	crOutPc;
reg [31:0]	crInPc;
wire[31:0]	crOutLr;
reg [31:0]	crInLr;
// wire[63:0]	crOutSr;
reg [63:0]	crInSr;
wire[31:0]	crOutVbr;
wire[31:0]	crOutGbr;
wire[31:0]	crOutTbr;
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

	crOutVbr,
	crOutGbr,
	crOutTbr,
	crOutMmcr,
	crOutKrr
	);


/* EX1 */	


wire[63:0]		ex1MulVal;

// `ifdef jx2_enable_fpu
wire[5:0]		ex1RegIdFRn;
wire[63:0]		ex1RegValFRn;
wire[63:0]		ex1FpuValGRn;
wire[1:0]		ex1FpuOK;
wire			ex1FpuSrT;
// `endif

`ifndef jx2_enable_fpu
assign	ex1FpuValGRn	= UV64_XX;
assign	ex1FpuOK		= UMEM_OK_READY;
assign	ex1FpuSrT		= 0;
`endif

reg[63:0]		ex2MemDataIn;
reg[1:0]		ex2MemDataOK;


reg[31:0]		ex1ValBPc;

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
	
reg[31:0]		ex1RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex1RegValImm;		//Immediate (Decode)
	
wire[63:0]		ex1RegOutDlr;
reg[63:0]		ex1RegInDlr;
wire[63:0]		ex1RegOutDhr;
reg[63:0]		ex1RegInDhr;
wire[63:0]		ex1RegOutSp;
reg[63:0]		ex1RegInSp;

wire[31:0]		ex1RegOutLr;
reg[31:0]		ex1RegInLr;
wire[63:0]		ex1RegOutSr;
reg[63:0]		ex1RegInSr;

wire[31:0]		ex1MemAddr;
wire[ 4:0]		ex1MemOpm;
wire[63:0]		ex1MemDataOut;

ExEX1	ex1(
	clock,			reset,
	ex1OpUCmd,		ex1OpUIxt,
	ex1Hold,

	ex1RegIdRs,		ex1RegIdRt,		ex1RegIdRm,
	ex1RegValRs,	ex1RegValRt,	ex1RegValRm,
	ex1RegValFRs,	ex1RegValFRt,	ex1RegValCRm,

	ex1RegIdRn1,	ex1RegValRn1,
	ex1RegIdCn1,	ex1RegValCn1,
	ex1HldIdRn1,	ex1HldIdCn1,
	
	ex1RegValPc,	ex1RegValImm,
	ex1FpuValGRn,	ex1FpuSrT,
	ex1BraFlush,
	
	ex1RegOutDlr,	ex1RegInDlr,
	ex1RegOutDhr,	ex1RegInDhr,
	ex1RegOutSp,	ex1RegInSp,

	ex1RegOutLr,	ex1RegInLr,
	ex1RegOutSr,	ex1RegInSr,

	ex1MemAddr,		ex1MemOpm,
	ex1MemDataOut
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
	clock,			reset,
	ex1RegValRs[31:0],	ex1RegValRt[31:0],
	ex1MulVal,		ex1OpUIxt);

`ifdef jx2_enable_fpu

FpuExOp	ex1Fpu(
	clock,			reset,

	ex1OpUCmd,		ex1OpUIxt,
	ex1RegIdRs,		ex1RegValFRs,
	ex1RegIdRt,		ex1RegValFRt,
	ex1RegIdRm,		ex1RegValRs,
	ex1RegIdFRn,	ex1RegValFRn,
	ex1FpuOK,		ex1FpuSrT,
	
	ex1RegInSr,
	
	ex1RegValRs,	ex1FpuValGRn,
	ex2MemDataIn,	ex2MemDataOK
	);

`endif

/* EX2 */

reg[31:0]		ex2ValBPc;

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
	
reg[31:0]		ex2RegValPc;		//PC Value (Synthesized)
reg[32:0]		ex2RegValImm;		//Immediate (Decode)
reg[65:0]		ex2RegAluRes;		//Arithmetic Result
reg[63:0]		ex2RegMulRes;		//Multiplier Result
// reg[63:0]		ex2RegFpuGRn;		//FPU GPR Result
reg				ex2BraFlush;		//Flush EX2

wire[63:0]		ex2RegOutDlr;
reg[63:0]		ex2RegInDlr;
wire[63:0]		ex2RegOutDhr;
reg[63:0]		ex2RegInDhr;
wire[63:0]		ex2RegOutSp;
reg[63:0]		ex2RegInSp;

wire[31:0]		ex2RegOutLr;
reg[31:0]		ex2RegInLr;
wire[63:0]		ex2RegOutSr;
reg[63:0]		ex2RegInSr;

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
	ex1FpuValGRn,
	ex2BraFlush,
	
	ex2RegOutDlr,	ex2RegInDlr,
	ex2RegOutDhr,	ex2RegInDhr,
	ex2RegOutSp,	ex2RegInSp,

	ex2RegOutLr,	ex2RegInLr,
	ex2RegOutSr,	ex2RegInSr,

	ex2MemDataIn,	ex2MemDataOK
	);


reg[31:0]	tValStepPc;
reg[31:0]	tValNextPc;
reg[31:0]	tOpNextPc;

reg[7:0]	opBraFlushMask;
reg[7:0]	nxtBraFlushMask;

reg[63:0]	tNxtRegExc;
reg[63:0]	tRegExc;
reg[7:0]	tRegExcOfs;

always @*
begin
	exHold1		= 0;
	exHold2		= 0;
	tNxtRegExc	= 0;
	tRegExcOfs	= 0;
	
	if(ex1Hold)
		exHold2		= 1;
	if(ex2Hold)
		exHold2		= 1;
	if(ifOutPcOK[1])
		exHold2		= 1;
	if(ex1FpuOK[1])
		exHold2		= 1;

	if(exHold2)
		exHold1		= 1;

	if(	(ex1HldIdRn1 == gprIdRs) ||
		(ex1HldIdRn1 == gprIdRt) ||
		((ex1HldIdRn1 == gprIdRm) &&
//			(id1IdUCmd[5:0]==JX2_UCMD_MOV_RM)))
			(id2IdUCmd[5:0]==JX2_UCMD_MOV_RM)))
	begin
		exHold1		= exHold1 | (ex1HldIdRn1 != JX2_GR_ZZR);
	end

//	if(ex1HldIdCn1 == crIdCm)
//	begin
//		exHold1		= exHold1 | ({1'b1, ex1HldIdCn1} != JX2_CR_ZZR);
//	end

//	if( ({1'b1, ex1HldIdCn1} != JX2_CR_ZZR) &&
//			({1'b1, ex1HldIdCn1} != JX2_CR_PC))
//		exHold1 = 1;

	if( ({1'b1, ex1RegIdCn1} != JX2_CR_ZZR) &&
			({1'b1, ex1RegIdCn1} != JX2_CR_PC))
		exHold1 = 1;

	if( ({1'b1, ex2RegIdCn2} != JX2_CR_ZZR) &&
			({1'b1, ex2RegIdCn2} != JX2_CR_PC))
		exHold1 = 1;

//	case( {1'b1, ex1HldIdCn1} )
//		JX2_CR_LR:	 exHold1 = 1;
//		JX2_CR_SR:	 exHold1 = 1;
//		JX2_CR_GBR:	 exHold1 = 1;
//		JX2_CR_TBR:	 exHold1 = 1;
//		default: begin
//		end
//	endcase

	if(memRegExc[15])
		tNxtRegExc = memRegExc;


	ifInPcHold	= exHold1;
	dcInHold	= exHold2;

//	exHold1		= 0;
//	exHold2		= 0;

	/* Advance PC */
	tValStepPc		= { 29'b0, ifOutPcStep, 1'b0 };
//	tValNextPc		= crOutPc + tValStepPc;
//	tValNextPc		= ifValPc + tValStepPc;
	tValNextPc		= ifLastPc + tValStepPc;
	tOpNextPc		= tValNextPc;
	/* Hold current PC if branching. */
//	if(opBraFlushMask[2])
	if(opBraFlushMask[3])
//	if(opBraFlushMask[2] || exHold1)
//	if(opBraFlushMask[3] || exHold1)
//		tValNextPc		= ifValPc;
		tValNextPc		= ifLastPc;

`ifdef jx2_enable_prebra
//	if(id1PreBra)
	if(id1PreBra && !opBraFlushMask[2])
//	if(id1PreBra && !opBraFlushMask[1])
	begin
//		$display("PreBra %X", id1PreBraPc);
		tValNextPc = id1PreBraPc;
		nxtBraFlushMask[2] = 1;
		nxtBraFlushMask[3] = 1;
//		nxtBraFlushMask = 8'h0F;
	end
`endif

	/* Handle if EX1 unit has initiated a branch. */
	nxtBraFlushMask	= { 1'b0, opBraFlushMask[7:1] };
	if(({1'b1, ex1RegIdCn1} == JX2_CR_PC) && !ex1PreBra)
	begin
//		$display("EX1 BRA %X", ex1RegValCn1);
		tValNextPc = ex1RegValCn1[31:0];
//		nxtBraFlushMask = 8'h07;
		nxtBraFlushMask = 8'h0F;
	end

`ifndef def_true
	/* EX2 can branch, ignore if already in a branch. */
	if(({1'b1, ex2RegIdCn2} == JX2_CR_PC) && !ex2PreBra &&
		!opBraFlushMask[0])
	begin
//		$display("EX2 BRA %X", ex2RegValCn2);
		tValNextPc = ex2RegValCn2[31:0];
//		nxtBraFlushMask = 8'h07;
		nxtBraFlushMask = 8'h0F;
	end
`endif

	if(reset)
	begin
		tValNextPc = UV32_00;
//		nxtBraFlushMask = 8'h07;
		nxtBraFlushMask = 8'h0F;
	end
	
	case(tRegExc[14:13])
		2'b00: tRegExcOfs=1;
		2'b01: tRegExcOfs=3;
		2'b10: tRegExcOfs=4;
		2'b11: tRegExcOfs=2;
	endcase
	
	if(tRegExc[15])
	begin
		$display("ExUnit: Fault %X", tRegExc);
	
//		tValNextPc = crOutVbr;
		tValNextPc = {
//			crOutVbr[31:11],
//			crOutVbr[10: 3]+tRegExcOfs,
			crOutVbr[31:8],
			crOutVbr[7: 3]+tRegExcOfs[4:0],
			crOutVbr[2:0] };
//		nxtBraFlushMask = 8'h07;
		nxtBraFlushMask = 8'h0F;
	end

	ifValPc			= tValNextPc;

	crInPc			= tValNextPc;
	crInSr			= ex1RegOutSr;
	crInLr			= ex1RegOutLr;

	gprIdRn1		= ex1RegIdRn1;
	gprValRn1		= ex1RegValRn1;
	gprIdRn2		= ex2RegIdRn2;
	gprValRn2		= ex2RegValRn2;

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
	gprInSp			= ex1RegOutSp;
	gprInElr		= gprOutElr;
	gprInEhr		= gprOutEhr;
	gprInBp			= gprOutBp;


	/* EX1 */
	ex1RegInDlr		= gprOutDlr;
	ex1RegInDhr		= gprOutDhr;
	ex1RegInSp		= gprOutSp;
	ex1RegInLr		= crOutLr;
//	ex1RegInSr		= crOutSr;
	ex1RegInSr		= ex2RegOutSr;

	ex2RegAluRes	= ex1ValAlu;
	ex2RegMulRes	= ex1MulVal;
//	ex2RegFpuGRn	= ex1FpuValGRn;

	dcInAddr		= ex1MemAddr;
	dcInOpm			= ex1MemOpm;
	dcInVal			= ex1MemDataOut;


	/* EX2 */

	ex2MemDataIn	= dcOutVal;
	ex2MemDataOK	= dcOutOK;

	ex2RegInDlr		= gprOutDlr;
	ex2RegInDhr		= gprOutDhr;
	ex2RegInSp		= gprOutSp;
	ex2RegInLr		= crOutLr;
	ex2RegInSr		= crOutSr;

end

always @(posedge clock)
begin

`ifndef jx2_enable_fpu
		ex1RegValFRs	<= UV64_XX;
		ex1RegValFRt	<= UV64_XX;
`endif

	if(reset)
	begin
//		ifValPc			<= UV32_00;
//		opBraFlushMask	<= 8'h07;
		opBraFlushMask	<= 8'h0F;
	end
	else
		if(!exHold1)
	begin
		tRegExc		<= tNxtRegExc;

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
		$display("     Rs=%X(%X) Rt=%X(%X) Rn0=%X(%X)",
			gprIdRs, gprValRs,
			gprIdRt, gprValRt,
			gprIdRm, gprValRm);

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


		/* EX1 */

		ex1ValBPc		<= id2ValBPc;
		ex1OpUCmd		<= id2IdUCmd;
//		ex1OpUCmd		<= {
//			opBraFlushMask[0] ? JX2_IXC_NV : id2IdUCmd[7:6],
//			id2IdUCmd[5:0] };
		ex1OpUIxt		<= id2IdUIxt;
		ex1PreBra		<= id2PreBra;
		ex1IstrWord		<= id2IstrWord;

		ex1RegValPc		<= gprValPc;
		ex1RegValImm	<= gprValImm;
		ex1BraFlush		<= nxtBraFlushMask[0];

		ex1RegIdRs		<= gprIdRs;
		ex1RegIdRt		<= gprIdRt;
		ex1RegIdRm		<= gprIdRm;
		ex1RegValRs		<= gprValRs;
		ex1RegValRt		<= gprValRt;
		ex1RegValRm		<= gprValRm;

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

`ifdef def_true
		ex1ValBPc		<= UV32_XX;
//		ex1OpUCmd		<= UV8_XX;
		ex1OpUIxt		<= UV8_XX;
		ex1PreBra		<= 0;
		ex1IstrWord		<= UV32_XX;

		ex1RegValPc		<= UV32_XX;
		ex1RegValImm	<= UV33_XX;

		ex1RegIdRs		<= UV6_XX;
		ex1RegIdRt		<= UV6_XX;
		ex1RegIdRm		<= UV6_XX;
		ex1RegValRs		<= UV64_XX;
		ex1RegValRt		<= UV64_XX;
		ex1RegValRm		<= UV64_XX;

		ex1RegValCRm	<= UV64_XX;

`ifdef jx2_enable_fpu
		ex1RegValFRs	<= UV64_XX;
		ex1RegValFRt	<= UV64_XX;
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

//		$display("");
`endif

		/* EX2 */
		ex2OpUCmd		<= ex1OpUCmd;
		ex2OpUIxt		<= ex1OpUIxt;
		ex2PreBra		<= ex1PreBra;
		ex2IstrWord		<= ex1IstrWord;
		ex2ValBPc		<= ex1ValBPc;
		ex2BraFlush		<= ex1BraFlush;

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

		/* WB */
	
	end

end

endmodule