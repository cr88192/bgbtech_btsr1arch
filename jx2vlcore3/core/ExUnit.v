/*

IF ID1 ID2 EX1 EX2 WB

 */

`include "CoreDefs.v"

`include "MemL1A.v"
`include "DecOp.v"
`include "RegGPR.v"
`include "RegFPR.v"
`include "RegCR.v"

`include "ExEX1.v"
`include "ExEX2.v"
`include "ExMul.v"
`include "FpuExOp.v"
`include "DecPreBra.v"

module ExUnit(
	clock,
	reset,
	
	memAddr,	memOpm,
	memDataIn,	memDataOut,
	memOK
	);

input			clock;
input			reset;

output[31:0]	memAddr;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
output[4:0]		memOpm;
input[1:0]		memOK;

reg				exHold1;
reg				exHold2;


/* IF */

wire[63:0]		gprOutDlr;
wire[63:0]		gprOutDhr;

wire[63:0]		crOutMmcr;
wire[63:0]		crOutKrr;

reg[31:0]		ifValPc;
wire[63:0]		ifIstrWord;	//source instruction word
wire[1:0]		ifOutPcOK;
wire[1:0]		ifOutPcStep;

reg [31:0]		dcInAddr;
reg [ 4:0]		dcInOpm;
wire[63:0]		dcOutVal;
reg [63:0]		dcInVal;
wire[ 1:0]		dcOutOK;

MemL1A		memL1(
	clock,			reset,

	ifValPc,		ifIstrWord,
	ifOutPcOK,		ifOutPcStep,

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutOK,

	gprOutDlr,		gprOutDhr,
	crOutMmcr,		crOutKrr,

	memAddr,		memOpm,
	memDataIn,		memDataOut,
	memOK
	);


/* ID1 */

wire[31:0]		gprValGbr;
wire[31:0]		gprValLr;


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

wire[31:0]		id1PreBraPc;
wire			id1PreBra;

DecPreBra	preBra(
	id1IstrWord,	id1ValPc,
	id1PreBraPc,	id1PreBra,
	gprValLr);
	

/* ID2 */

reg[7:0]		id2IdUCmd;
reg[7:0]		id2IdUIxt;
reg				id2PreBra;


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

wire[5:0]		gprIdFRs;
wire[5:0]		gprIdFRt;
assign	gprIdFRs = (id2IdUCmd[5:0]==JX2_UCMD_MOV_RM) ? gprIdRm :  gprIdRs;
assign	gprIdFRt = gprIdRt;

wire[63:0]		gprValFRs;
wire[63:0]		gprValFRt;
reg[5:0]		gprIdFRn;
reg[63:0]		gprValFRn;

RegFPR	regFpr(
	clock,	reset,
	gprIdFRs,	gprValFRs,
	gprIdFRt,	gprValFRt,
	gprIdFRn,	gprValFRn,
	crOutSr,	exHold2
	);


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

wire[5:0]		ex1RegIdFRn;
wire[63:0]		ex1RegValFRn;
wire[63:0]		ex1FpuValGRn;
wire[1:0]		ex1FpuOK;
wire			ex1FpuSrT;

reg[63:0]		ex2MemDataIn;
reg[1:0]		ex2MemDataOK;


reg[7:0]		ex1OpUCmd;
reg[7:0]		ex1OpUIxt;
wire			ex1Hold;
reg				ex1PreBra;

reg[5:0]		ex1RegIdRs;		//Source A, ALU / Base
reg[5:0]		ex1RegIdRt;		//Source B, ALU / Index
reg[5:0]		ex1RegIdRm;		//Source C, MemStore
reg[63:0]		ex1RegValRs;		//Source A Value
reg[63:0]		ex1RegValRt;		//Source B Value
reg[63:0]		ex1RegValRm;		//Source C Value

reg[63:0]		ex1RegValFRs;		//Source A Value (FPR)
reg[63:0]		ex1RegValFRt;		//Source B Value (FPR)
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
	
	ex1RegOutDlr,	ex1RegInDlr,
	ex1RegOutDhr,	ex1RegInDhr,
	ex1RegOutSp,	ex1RegInSp,

	ex1RegOutLr,	ex1RegInLr,
	ex1RegOutSr,	ex1RegInSr,

	ex1MemAddr,		ex1MemOpm,
	ex1MemDataOut
	);

ExMul	ex1Mul(
	clock,			reset,
	ex1RegValRs[31:0],	ex1RegValRt[31:0],
	ex1MulVal,		ex1OpUIxt);

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


/* EX2 */

reg[7:0]		ex2OpUCmd;
reg[7:0]		ex2OpUIxt;
wire			ex2Hold;
reg				ex2PreBra;

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
reg[63:0]		ex2RegMulRes;		//Multiplier Result
// reg[63:0]		ex2RegFpuGRn;		//FPU GPR Result

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
	ex2RegMulRes,	ex1FpuValGRn,
	
	ex2RegOutDlr,	ex2RegInDlr,
	ex2RegOutDhr,	ex2RegInDhr,
	ex2RegOutSp,	ex2RegInSp,

	ex2RegOutLr,	ex2RegInLr,
	ex2RegOutSr,	ex2RegInSr,

	ex2MemDataIn,	ex2MemDataOK
	);


reg[31:0]	tValStepPc;
reg[31:0]	tValNextPc;

reg[7:0]	opBraFlushMask;
reg[7:0]	nxtBraFlushMask;

always @*
begin
	exHold1		= 0;
	exHold2		= 0;
	
	if(ex1Hold)
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
			(id1IdUCmd[5:0]==JX2_UCMD_MOV_RM)))
	begin
		exHold1		= exHold1 | (ex1HldIdRn1 != JX2_GR_ZZR);
	end

	if(ex1HldIdCn1 == crIdCm)
	begin
		exHold1		= exHold1 | ({1'b1, ex1HldIdCn1} != JX2_CR_ZZR);
	end

	tValStepPc		= { 29'b0, ifOutPcStep, 1'b0 };
	tValNextPc		= crOutPc + tValStepPc;

	nxtBraFlushMask	= { 1'b0, opBraFlushMask[7:1] };
	if(({1'b1, ex1RegIdCn1} == JX2_CR_PC) && !ex1PreBra)
	begin
		tValNextPc = ex1RegValCn1[31:0];
		nxtBraFlushMask = 8'h07;
	end
	if(({1'b1, ex2RegIdCn2} == JX2_CR_PC) && !ex2PreBra)
	begin
		tValNextPc = ex2RegValCn2[31:0];
		nxtBraFlushMask = 8'h07;
	end
	if(id1PreBra)
	begin
		tValNextPc = id1PreBraPc;
		nxtBraFlushMask[2] = 1;
	end

	if(reset)
	begin
		tValNextPc = UV32_00;
		nxtBraFlushMask = 8'h07;
	end

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
	
	gprIdFRn		= ex1RegIdFRn;
	gprValFRn		= ex1RegValFRn;


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
	ex1RegInSr		= crOutSr;

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

	if(!exHold1)
	begin
		/* IF */
	
		opBraFlushMask	<= nxtBraFlushMask;
		ifValPc			<= tValNextPc;


		/* ID1 */

		id1ValPc		<= ifValPc + tValStepPc;
	//	id1ValPc		<= tValNextPc;
		id1IstrWord		<= ifIstrWord;	//source instruction word


		/* ID2 */

		gprIdRs			<= id1IdRegM;
		gprIdRt			<= id1IdRegO;
		gprIdRm			<= id1IdRegN;
		gprValPc		<= id1ValPc;
		gprValImm		<= id1IdImm;
		crIdCm			<= id1IdRegM[4:0];
		id2IdUCmd		<= id1IdUCmd;
		id2IdUIxt		<= id1IdUIxt;
		id2PreBra		<= id1PreBra;


		/* EX1 */

//		ex1OpUCmd		<= id2OpUCmd;
		ex1OpUCmd		<= {
			opBraFlushMask[0] ? JX2_IXC_NV : ex1OpUCmd[7:6],
			ex1OpUCmd[5:0] };
		ex1OpUIxt		<= id2IdUIxt;
		ex1PreBra		<= id2PreBra;

		ex1RegValPc		<= gprValPc;
		ex1RegValImm	<= gprValImm;

		ex1RegIdRs		<= gprIdRs;
		ex1RegIdRt		<= gprIdRt;
		ex1RegIdRm		<= gprIdRm;
		ex1RegValRs		<= gprValRs;
		ex1RegValRt		<= gprValRt;
		ex1RegValRm		<= gprValRm;

		ex1RegValFRs	<= gprValFRs;
		ex1RegValFRt	<= gprValFRt;
		ex1RegValCRm	<= crValCm;
	end
	else
		if(!exHold2)
	begin
		ex1OpUCmd		<= { JX2_IXC_NV, ex1OpUCmd[5:0] };
	end

	if(!exHold2)
	begin
		/* EX2 */

		ex2OpUCmd		<= ex1OpUCmd;
		ex2OpUIxt		<= ex1OpUIxt;
		ex2PreBra		<= ex1PreBra;

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