
/*
IF, ID1, ID2, EX, [MA*, WB]

 */

`include "Jx2CoreDefs.v"

`include "Jx2MemCache.v"

`include "Jx2RegGpr.v"
`include "Jx2RegFpr.v"
`include "Jx2DecOp.v"
`include "Jx2ExOp.v"
`include "Jx2FpuExOp.v"

`include "Jx2ConvFpuS2D.v"
`include "Jx2ConvFpuH2D.v"
`include "Jx2ConvFpuD2S.v"
`include "Jx2ConvFpuD2H.v"
`include "Jx2ConvFpuD2I.v"

`ifdef JX2_MEM_USELSQ
`include "Jx2ExLsq.v"
`endif


module Jx2ExUnit(
	/* verilator lint_off UNUSED */
	clock,		reset,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

input			clock;			//clock
input			reset;			//reset

input[127:0]	memInData;	//memory PC data
output[127:0]	memOutData;	//memory PC data
output[31:0]	memAddr;	//memory PC address
output[4:0]		memOpm;		//memory PC operation mode
input[1:0]		memOK;		//memory PC OK

input[31:0]		mmioInData;		//mmio data in
output[31:0]	mmioOutData;	//mmio data out
output[31:0]	mmioAddr;		//mmio address
output[4:0]		mmioOpm;		//mmio read
input[1:0]		mmioOK;			//mmio OK


/* Conjoined Memory Cache */

wire[15:0]		memRegOutExc;		//Raise Exception
wire[63:0]		memRegOutTea;		//Exception TEA

reg[63:0]		memRegInMMCR;		//MMU Control Register
reg[63:0]		memRegInKRR;		//Keyring Register
reg[63:0]		memRegInSR;			//Status Register

/* D1 Cache */

reg [63: 0]		dcRegInAddr;		//input address
reg [ 4: 0]		dcRegInOpm;
reg [63: 0]		dcRegInData;		//input data (store)
wire[63: 0]		dcRegOutData;		//output data (load)
wire[ 1: 0]		dcRegOutOK;			//set if we have a valid value.

/* I1 Cache */

reg [63: 0]		icRegInPc;		//input PC address
wire[47: 0]		icRegOutPcVal;	//output PC value
wire[ 1: 0]		icRegOutPcOK;		//set if we have a valid value.
wire[ 1: 0]		icRegStepPc;

Jx2MemCache cache(
	clock,			reset,

	dcRegInAddr,
	dcRegInData,	dcRegOutData,
	dcRegInOpm,		dcRegOutOK,

	icRegInPc,		icRegOutPcVal,
	icRegOutPcOK,	icRegStepPc,

	memRegOutExc,	memRegOutTea,
	memRegInMMCR,	memRegInKRR,
	memRegInSR,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);


`ifdef JX2_MEM_USELSQ

/* Load Store Queue */

reg [63: 0]		lsqExAddr;			//input address
reg [ 6: 0]		lsqExIdRn;			//register to load to
reg [ 4: 0]		lsqExOpm;
wire[63: 0]		lsqExDataO;			//
reg [63: 0]		lsqExDataI;			//
wire[ 1: 0]		lsqExOK;			//set if we have a valid value.

wire[63: 0]		lsqMemAddr;			//input address
wire[ 4: 0]		lsqMemOpm;			//memory operation
reg [63: 0]		lsqMemDataI;		//input data (store)
wire[63: 0]		lsqMemDataO;		//output data (load)
reg [ 1: 0]		lsqMemOK;			//set if we have a valid value.

wire[63:0]		lsqRegOutVal;		//Rn output value
wire[6:0]		lsqRegOutId;		//Rn, value to write
wire[1:0]		lsqRegOutOK;		//execute status

Jx2ExLsq	memLsq(
	clock,			reset,

	lsqExAddr,		lsqExIdRn,
	lsqExDataO,		lsqExDataI,
	lsqExOpm,		lsqExOK,

	lsqMemAddr,
	lsqMemDataI,	lsqMemDataO,
	lsqMemOpm,		lsqMemOK,

	gprRegIdRm,		gprRegIdRn,
	gprRegIdRi,

	lsqRegOutId,	lsqRegOutVal,
	lsqRegOutOK
	);

`endif
	

/* Decode */

reg [63: 0]		decRegPc;		//input PC address
reg [47: 0]		decIstrWord;
reg [ 1: 0]		decRegStepPc;
reg [63: 0]		decRegNextPc;	//next PC

wire[ 6:0]		decIdRegN;
wire[ 6:0]		decIdRegM;
wire[ 6:0]		decIdRegO;
wire[32:0]		decIdImm;
wire[ 7:0]		decIdUCmd;
wire[ 7:0]		decIdUIxt;

Jx2DecOp	decOp(
	clock,			reset,
	decIstrWord,
	decIdRegN,		decIdRegM,	decIdRegO,
	decIdImm,		decIdUCmd,
	decIdUIxt
	);


/* Register File */
reg[ 7:0]		gprIdUCmd;
reg[ 7:0]		gprIdUIxt;

reg [ 6:0]		gprRegIdRm;
reg [ 6:0]		gprRegIdRn;
reg [ 6:0]		gprRegIdRi;
wire[63:0]		gprRegValRm;
wire[63:0]		gprRegValRn;
wire[63:0]		gprRegValRi;
reg [ 6:0]		gprRegIdRo;
reg [63:0]		gprRegValRo;
reg [63:0]		gprRegSrVal;
reg				gprRegExHold;

wire[63:0]		gprRegValCm;

reg [ 6:0]		gprRegIdCo;
reg [63:0]		gprRegValCo;

reg[32:0]		gprIdImm;
reg[63:0]		gprIdValPc;
reg[47:0]		gprIstrWord;
reg[ 1:0]		gprRegStepPc;
reg [63: 0]		gprRegNextPc;	//next PC

reg[63:0]		gprExNextSp2;		//
reg[63:0]		gprExNextPc2;		//PC in
reg[63:0]		gprExNextPr2;		//PR in
reg[63:0]		gprExNextSr2;		//SR in
reg[63:0]		gprExNextVbr2;		//VBR
reg[63:0]		gprExNextDlr2;		//DLR
reg[63:0]		gprExNextDhr2;		//DHR
reg[63:0]		gprExNextGbr2;		//GBR
reg[63:0]		gprExNextTbr2;		//TBR
reg[63:0]		gprExNextExc2;		//EXSR
reg[63:0]		gprExNextTea2;		//TEA

wire[63:0]		gprOregSp;			//SP (R15)
wire[63:0]		gprOregPc;			//PC
wire[63:0]		gprOregPr;			//PR
wire[63:0]		gprOregSr;			//SR
wire[63:0]		gprOregVbr;			//VBR
wire[63:0]		gprOregDlr;			//DLR
wire[63:0]		gprOregDhr;			//DHR
wire[63:0]		gprOregGbr;			//GBR
wire[63:0]		gprOregTbr;			//TBR
wire[63:0]		gprOregExc;			//EXSR
wire[63:0]		gprOregTea;			//TEA

wire[63:0]		gprOregMmcr;		//MMCR
wire[63:0]		gprOregKrr;			//KRR

Jx2RegGpr	gprFile(
	clock,			reset,

	gprRegIdRm,		gprRegValRm,
	gprRegIdRn,		gprRegValRn,
	gprRegIdRi,		gprRegValRi,
	gprRegIdRo,		gprRegValRo,
	gprRegIdRm,		gprRegValCm,
	gprRegIdCo,		gprRegValCo,

	gprRegSrVal,	gprRegExHold,
	gprIdImm,		gprIdValPc,
	gprRegStepPc,	gprRegNextPc,

	gprOregSp,		gprExNextSp2,
	gprOregPc,		gprExNextPc2,
	gprOregPr,		gprExNextPr2,
	gprOregSr,		gprExNextSr2,
	gprOregVbr,		gprExNextVbr2,
	gprOregDlr,		gprExNextDlr2,
	gprOregDhr,		gprExNextDhr2,
	gprOregGbr,		gprExNextGbr2,
	gprOregTbr,		gprExNextTbr2,
	gprOregExc,		gprExNextExc2,
	gprOregTea,		gprExNextTea2,
	gprOregMmcr,
	gprOregKrr
	);


/* FPU Register File */

wire[63:0]		fprRegValRm;
wire[63:0]		fprRegValRn;

reg [ 6:0]		fprRegIdRo;
reg [63:0]		fprRegValRo;

Jx2RegFpr	fpr(
	clock,			reset,
	gprRegIdRm,		fprRegValRm,
	gprRegIdRi,		fprRegValRn,
	fprRegIdRo,		fprRegValRo,
	gprRegSrVal,	gprRegExHold	
	);


/* Execute Unit */

reg[7:0]		exOpCmd;		//command opcode
reg[7:0]		exRegIdIxt;		//ALU Index / Opcode Extension

reg[7:0]		exOpCmdB;		//command opcode
reg[7:0]		exRegIdIxtB;	//ALU Index / Opcode Extension
reg[7:0]		exOpCmdC;		//command opcode

reg[6:0]		exRegIdRm;
reg[6:0]		exRegIdRn;
reg[6:0]		exRegIdRi;
reg[47:0]		exIstrWord;
reg[63:0]		exIdCurPc;		//PC to current instruction
reg[ 1:0]		exRegStepPc;

reg[63:0]		exRegValRm;		//Rm input value
reg[63:0]		exRegValRmB;	//Rm input value (alternate)
reg[63:0]		exRegValRmC;	//Rm input value (alternate)
reg[63:0]		exRegValRn;		//Rn input value
reg[63:0]		exRegValRi;		//Ri input value
reg[63:0]		exRegValRiB;	//Ri input value (alternate)
reg[63:0]		exRegValRiC;	//Ri input value (alternate)
reg[63:0]		exRegValRb;		//Rb input value (synthetic)
reg[63:0]		exRegValCm;		//Rm input value
reg[32:0]		exImmValRi;		//immediate/disp value
reg[63:0]		exIdInGenPc;	//PC to next instruction

wire[63:0]		exRegOutVal;	//Rn output value
wire[6:0]		exRegOutId;		//Rn, value to write
wire[1:0]		exRegOutOK;		//execute status

wire[63:0]		exRegOutValCn;	//Cn output value
wire[6:0]		exRegOutIdCn;	//Cn, value to write

reg[63:0]		exRegValFRm;	//Rm input value
reg[63:0]		exRegValFRn;	//Rn input value

wire[63:0]		exMemAddr;		//memory address
wire[63:0]		exMemData;		//memory data (store)
wire[4:0]		exMemOpm;		//mem op mode
wire[7:0]		exMemOpCmd2;	//mem EX chain
wire[6:0]		exMemOutId;		//
reg [63:0]		exMemDataLd;	//memory data (load)
reg [1:0]		exMemDataOK;	//memory status (load)

reg [63:0]		exCtlInSp;		//SP in
wire[63:0]		exCtlOutSp;		//SP out
reg [63:0]		exCtlInPc;		//PC in
wire[63:0]		exCtlOutPc;		//PC out
reg [63:0]		exCtlInPr;		//PR in
wire[63:0]		exCtlOutPr;		//PR out
reg [63:0]		exCtlInSr;		//SR in
wire[63:0]		exCtlOutSr;		//SR out
reg [63:0]		exCtlInVbr;
wire[63:0]		exCtlOutVbr;
reg [63:0]		exCtlInDlr;		//MACL in
wire[63:0]		exCtlOutDlr;	//MACL out
reg [63:0]		exCtlInDhr;		//MACH in
wire[63:0]		exCtlOutDhr;	//MACH out
reg [63:0]		exCtlInGbr;
wire[63:0]		exCtlOutGbr;
reg [63:0]		exCtlInTbr;
wire[63:0]		exCtlOutTbr;


Jx2ExOp		exOp(
	clock,			reset,

	exOpCmd,		exOpCmdB,		exOpCmdC,
	exRegIdIxt,		exRegIdIxtB,
	exRegIdRm,		exRegValRm,		exRegValRmB,	exRegValRmC,
	exRegIdRn,		exRegValRn,		exRegValRb,
	exRegIdRi,		exRegValRi,		exRegValRiB,	exRegValRiC,
	exRegIdRm,		exRegValCm,
	exImmValRi,		exIdInGenPc,
	exRegOutId,		exRegOutVal,
	exRegOutIdCn,	exRegOutValCn,
	exRegOutOK,

	exRegValFRm,	exRegValFRn,

	exMemAddr,		exMemData,
	exMemOpm,		exMemOpCmd2,
	exMemDataLd,	exMemDataOK,
	exMemOutId,

	exCtlInSp,		exCtlOutSp,
	exCtlInPc,		exCtlOutPc,
	exCtlInPr,		exCtlOutPr,
	exCtlInSr,		exCtlOutSr,
	exCtlInVbr,		exCtlOutVbr,
	exCtlInDlr,		exCtlOutDlr,
	exCtlInDhr,		exCtlOutDhr,
	exCtlInGbr,		exCtlOutGbr,
	exCtlInTbr,		exCtlOutTbr
	);


/* FPU Execute */

reg[7:0]		exOpCmdFp;			//command opcode
reg[7:0]		exRegIdIxtFp;		//ALU Index / Opcode Extension

wire[63:0]		exRegOutValFn;		//FRn output value
wire[6:0]		exRegOutIdFn;		//FRn, value to write
wire[1:0]		exRegOutFpOK;		//execute status (FPU)
wire[63:0]		exCtlOutFpDlr;		//DLR out (FPU)

Jx2FpuExOp	exFpOp(
	clock,		reset,
	exOpCmdFp,		exRegIdIxtFp,
	exRegIdRm,		exRegValFRm,
	exRegIdRi,		exRegValFRn,
	exRegIdRn,		exRegValFRn,
	exRegOutIdFn,	exRegOutValFn,
	exRegOutFpOK,

	exMemDataLd,	exMemDataOK,
	exCtlInDlr,		exCtlOutFpDlr
	);


/* Common */

reg		exHold;
reg		exLastHold;
reg		exNextPc;
reg		exBranchFlush;

reg[15:0]	exRegExc;
reg[63:0]	exRegTea;
reg[ 7:0]	exRegExcVbrOfs;

reg[63:0]	exRegNextPc;

always @*
begin
	exHold			= 0;
	if(exRegOutOK==UMEM_OK_HOLD)
	begin
`ifndef JX2_QUIET
		if(!exLastHold)
			$display("ExOp Hold");
`endif
		exHold		= 1;
	end
	if(exRegOutFpOK==UMEM_OK_HOLD)
	begin
`ifndef JX2_QUIET
		if(!exLastHold)
			$display("FpuOp Hold");
`endif
		exHold		= 1;
	end

`ifndef JX2_MEM_USELSQ			//Use Load-Store Queue
	if(dcRegOutOK==UMEM_OK_HOLD)
	begin
`ifndef JX2_QUIET
		if(!exLastHold)
			$display("D$ Hold");
`endif
		exHold		= 1;
	end
`endif

	if(icRegOutPcOK==UMEM_OK_HOLD)
	begin
`ifndef JX2_QUIET
		if(!exLastHold)
			$display("I$ Hold");
`endif
		exHold		= 1;
	end

`ifdef JX2_MEM_USELSQ			//Use Load-Store Queue
	if(lsqRegOutOK==UMEM_OK_HOLD)
	begin
`ifndef JX2_QUIET
		if(!exLastHold)
			$display("LSQ Hold");
`endif
		exHold		= 1;
	end
`endif


`ifndef JX2_QUIET
	if(exLastHold && !exHold)
		$display("Hold Release");
`endif

	exRegExc=memRegOutExc;
	exRegTea=memRegOutTea;

//	exRegExc = 0;

//	if((exMemOpm==0) &&
//				((exMemDataOK==UMEM_OK_OK) ||
//				(dcRegOutOK==UMEM_OK_OK)))
//		exHold		= 1;

`ifdef JX2_MEM_USELSQ
	exMemDataLd		= lsqExDataO;
	exMemDataOK		= lsqExOK;
	lsqMemDataI		= dcRegOutData;
	lsqMemOK		= dcRegOutOK;

	dcRegInAddr		= lsqMemAddr;
	dcRegInOpm		= lsqMemOpm;
	dcRegInData		= lsqMemDataO;

`ifndef JX2_QUIET
	if(dcRegInOpm!=0)
	begin
// /*
		$display("ExUnit LSQ Addr %X %X", dcRegInAddr, exMemAddr);
		$display("ExUnit LSQ DataO %X %X", exMemData, dcRegInData);
		$display("ExUnit LSQ DataI %X %X", dcRegOutData, exMemDataLd);

		$display("ExUnit LSQ Opm %X %X", exMemOpm, dcRegInOpm);
		$display("ExUnit LSQ OK %X %X", dcRegOutOK, exMemDataOK);
// */
	end
`endif

`else
	exMemDataLd		= dcRegOutData;
	exMemDataOK		= dcRegOutOK;
`endif

//	lsqRegOutVal;		//Rn output value
//	lsqRegOutId;		//Rn, value to write
//	lsqRegOutOK;		//execute status


//	dcRegInAddr		= exMemAddr;
//	dcRegInOpm		= exMemOpm;
//	dcRegInData		= exMemData;

//	icMemPcOK		= UMEM_OK_HOLD;
//	dcMemPcOK		= UMEM_OK_HOLD;
//	icMemPcData		= memRegOutData;
//	dcMemPcDataI	= memRegOutData;

/*
	memRegInAddr	= 0;
	memRegOutAddr	= 0;
//	memRegInData	= UV128_XX;
	memRegInData	= UV256_XX;
	memRegInOpm		= UMEM_OPM_READY;

	if(icMemPcOpm!=0)
	begin
		icMemPcData		= memRegOutData;
		icMemPcOK		= memRegOutOK;
//		memRegInAddr	= icMemPcAddr;
		memRegOutAddr	= icMemPcAddr;
//		memRegInData	= UV128_XX;
		memRegInData	= UV256_XX;
		memRegInOpm		= icMemPcOpm;
	end
	else
	if(dcMemPcOpm!=0)
	begin
		dcMemPcDataI	= memRegOutData;
		dcMemPcOK		= memRegOutOK;
		memRegOutAddr	= dcMemPcAddrI;
		memRegInAddr	= dcMemPcAddrO;
		memRegInData	= dcMemPcDataO;
		memRegInOpm		= dcMemPcOpm;
	end
	else
	begin
		icMemPcOK		= UMEM_OK_READY;
		dcMemPcOK		= UMEM_OK_READY;
	end
*/

	gprRegIdRo		= exRegOutId;
	gprRegValRo		= exRegOutVal;

	gprRegIdCo		= exRegOutIdCn;
	gprRegValCo		= exRegOutValCn;

	fprRegIdRo		= exRegOutIdFn;
	fprRegValRo		= exRegOutValFn;

`ifdef JX2_MEM_USELSQ
	if(lsqRegOutId!=JX2_REG_ZZR)
	begin
		if(exRegOutId!=JX2_REG_ZZR)
			exHold		= 1;

		gprRegIdRo		= lsqRegOutId;
		gprRegValRo		= lsqRegOutVal;
	end
`endif

	gprExNextSp2	= exCtlOutSp;
	gprExNextPc2	= exCtlOutPc;
//	gprExNextPc2	= exRegNextPc;

	gprExNextPr2	= exCtlOutPr;
	gprExNextSr2	= exCtlOutSr;
	gprExNextVbr2	= exCtlOutVbr;
	gprExNextDlr2	= exCtlOutDlr;
	gprExNextDhr2	= exCtlOutDhr;
	gprExNextGbr2	= exCtlOutGbr;
	gprExNextTbr2	= exCtlOutTbr;

	memRegInSR		= exCtlOutSr;
	memRegInMMCR	= gprOregMmcr;
	memRegInKRR		= gprOregKrr;

//	if(exHold)
//		exRegNextPc		= gprOregPc;
//	else
//		exRegNextPc		= (gprOregPc+2);

//	exRegNextPc		= gprOregPc+ {61'h0, icRegStepPc, 1'b0};
//	exRegNextPc		= gprOregPc+ {61'h0, dcRegStepPc, 1'b0};

	icRegInPc		= gprOregPc;

	exRegNextPc		= { gprOregPc[63:32],
		gprOregPc[31:0] + {29'h0, icRegStepPc, 1'b0} };

	exCtlInSp		= gprOregSp;
//	exCtlInPc		= (regCurPc+2);
	exCtlInPc		= exRegNextPc;
	exCtlInPr		= gprOregPr;
	exCtlInSr		= gprOregSr;
	exCtlInVbr		= gprOregVbr;
	exCtlInDlr		= gprOregDlr;
	exCtlInDhr		= gprOregDhr;
	exCtlInGbr		= gprOregGbr;
	exCtlInTbr		= gprOregTbr;

//	gprExNextPc2	= exRegNextPc;
//	if((exMemOpm==UMEM_OPM_CTRLF) && !exBranchFlush)
//		gprExNextPc2	= exCtlOutPc;

	gprExNextExc2	= gprOregExc;
	gprExNextTea2	= gprOregTea;

	gprRegExHold	= exHold;

/*
	if(exBranchFlush)
	begin
		gprExNextPc2 = exRegNextPc;
	end
	else
*/

// /*
	if(exRegExc[15])
	begin
`ifndef JX2_QUIET
		$display("Trap %X", exRegExc);
`endif
	
		case(exRegExc[14:13])
			2'b00: exRegExcVbrOfs = 8'h08;
			2'b01: exRegExcVbrOfs = 8'h18;
			2'b10: exRegExcVbrOfs = 8'h10;
			2'b11: exRegExcVbrOfs = 8'h20;
		endcase

		gprExNextPc2 = { gprOregVbr[63:16],
			gprOregVbr[15:0]+{UV8_00, exRegExcVbrOfs} };
		gprExNextSr2[30:28]=3'b111;
		
		gprExNextExc2[15: 0]	= exRegExc[15:0];
		gprExNextExc2[63:16]	= exIdCurPc[47:0];
		gprExNextTea2			= exRegTea;
	end
// */

end

reg[31:0]		regCurPc;

always @ (posedge clock)
begin
//	gprRegExHold		<= exHold;
	exLastHold			<= exHold;
	
	if(exHold)
	begin
//		$display("Jx2ExUnit: Hold Ex=%d Dc=%d Ic=%d",
//			exRegOutOK, dcRegOutOK, icRegOutPcOK);
	end
	else
	begin

	/* IF */
//		icRegInPc		<= gprOregPc;

	/* ID1 */
		decRegPc		<= icRegInPc;
//		decIstrWord		<= exBranchFlush ? 48'h300030003000 : icRegOutPcVal;
		decIstrWord		<= icRegOutPcVal;
//		decRegStepPc	<= exBranchFlush ? 2'b00 : icRegStepPc;
		decRegStepPc	<= icRegStepPc;
		decRegNextPc	<= exRegNextPc;

//		decIstrWord		<= icRegOutPcVal;

`ifndef JX2_QUIET

//		$display("ID1 %X %X", decRegPc, decIstrWord);

/*
		$display("ID1 %X %X-%X-%X", decRegPc,
			decIstrWord[15:0],
			decIstrWord[31:16],
			decIstrWord[47:32]
			);
*/

// /*
		$display("ID1 %X %X-%X-%X", icRegInPc,
			icRegOutPcVal[15:0],
			icRegOutPcVal[31:16],
			icRegOutPcVal[47:32]
			);
// */

`endif

	/* ID2 */
		gprRegIdRm		<= decIdRegM;
		gprRegIdRn		<= decIdRegN;
		gprRegIdRi		<= decIdRegO;
		gprRegSrVal		<= gprOregSr;
		gprIdImm		<= decIdImm;
		gprIdValPc		<= decRegPc;
		gprIstrWord		<= decIstrWord;
		gprRegStepPc	<= decRegStepPc;
		gprRegNextPc	<= decRegNextPc;

		gprIdUCmd		<= decIdUCmd;
		gprIdUIxt		<= decIdUIxt;

`ifndef JX2_QUIET

//		$display("ID2 %X %X %X-%X Rm=%X(%X) Rn=%X(%X) Imm=%X",
//			gprIdValPc, gprIstrWord,
//			gprIdUCmd, gprIdUIxt,
//			gprRegIdRm, gprRegValRm, gprRegIdRn, gprRegValRn,
//			gprIdImm);

/*
		$display("ID2 %X %X-%X-%X %X-%X",
			gprIdValPc,
			gprIstrWord[15:0],
			gprIstrWord[31:16],
			gprIstrWord[47:32],
			gprIdUCmd, gprIdUIxt);
*/

// /*
		$display("ID2 %X %X-%X-%X %X-%X",
			decRegPc,
			decIstrWord[15:0],
			decIstrWord[31:16],
			decIstrWord[47:32],
			decIdUCmd, decIdUIxt);
// */

`endif

	/* EX */
		exOpCmd			<= gprIdUCmd;
		exOpCmdB		<= gprIdUCmd;
		exOpCmdC		<= gprIdUCmd;
		exOpCmdFp		<= gprIdUCmd;

		exRegIdIxt		<= gprIdUIxt;
		exRegIdIxtB		<= gprIdUIxt;

		exRegIdIxtFp	<= gprIdUIxt;

		exRegIdRm		<= gprRegIdRm;
		exRegIdRn		<= gprRegIdRn;
		exRegIdRi		<= gprRegIdRi;
//		exRegIdCm		<= gprRegIdCm;
		exIdCurPc		<= gprIdValPc;
		exRegStepPc		<= gprRegStepPc;

		exRegValRm		<= gprRegValRm;
		exRegValRn		<= gprRegValRn;
		exRegValRi		<= gprRegValRi;
		exRegValRmB		<= gprRegValRm;
		exRegValRiB		<= gprRegValRi;
		exRegValRmC		<= gprRegValRm;
		exRegValRiC		<= gprRegValRi;
		exRegValRb		<= gprIdUIxt[3] ? gprRegValRm : gprRegValRn;
//		exRegValRb		<= UV64_XX;
		exRegValCm		<= gprRegValCm;
		exImmValRi		<= gprIdImm;
//		exIdInGenPc		<= (gprIdValPc+2);
//		exIdInGenPc		<= (gprIdValPc + {61'h0, gprRegStepPc, 1'b0} );
		exIdInGenPc		<= gprRegNextPc;
		exIstrWord		<= gprIstrWord;

		exRegValFRm		<= fprRegValRm;
		exRegValFRn		<= fprRegValRn;

//		exBranchFlush	<= (exCtlOutPc!=exCtlInPc);
//		exBranchFlush	<= 0;

`ifndef JX2_QUIET

//		$display("EX %X %X %X-%X DLR=%X Rm=%X(%X) Rn=%X(%X) Ro=%X(%X)",
//			exIdCurPc[47:0], exIstrWord,
//			exOpCmd, exRegIdIxt, exCtlInDlr,
//			exRegIdRm, exRegValRm,
//			exRegIdRn, exRegValRn,
//			exRegOutId, exRegOutVal);

/*
		$display("EX  %X %X-%X-%X %X-%X",
			exIdCurPc[47:0],
			exIstrWord[15:0],
			exIstrWord[31:16],
			exIstrWord[47:32],
			exOpCmd, exRegIdIxt);

		$display("EX  DLR=%X Rm=%X(%X) Rn=%X(%X)",
			exCtlInDlr,
			exRegIdRm, exRegValRm,
			exRegIdRn, exRegValRn);

		$display("EX  Ri=%X(%X) Ro=%X(%X) Imm=%X",
			exRegIdRi, exRegValRi,
			exRegOutId, exRegOutVal,
			exImmValRi);
*/

// /*
		$display("EX  %X %X-%X-%X %X-%X",
			gprIdValPc[47:0],
			gprIstrWord[15:0],
			gprIstrWord[31:16],
			gprIstrWord[47:32],
			gprIdUCmd, gprIdUIxt);

		$display("EX  DLR=%X Rm=%X(%X) Rn=%X(%X)",
			gprOregDlr,
			gprRegIdRm, gprRegValRm,
			gprRegIdRn, gprRegValRn);

		$display("EX  Ri=%X(%X) Ro=%X(%X) Imm=%X",
			gprRegIdRi, gprRegValRi,
			exRegOutId, exRegOutVal,
			gprIdImm);
// */

`endif

//		if((exCtlOutPc!=exCtlInPc) ||
//			(exMemOpm==UMEM_OPM_CTRLF))
//		if(exMemOpm==UMEM_OPM_CTRLF)
//		if(((exMemOpm==UMEM_OPM_CTRLF) || (exRegExc[15])) && !exBranchFlush)
		if((exMemOpm==UMEM_OPM_CTRLF) || (exRegExc[15]))
		begin
`ifndef JX2_QUIET
			$display("BRA Flush %X->%X", exCtlInPc, exCtlOutPc);
`endif
			exBranchFlush	<= 1;
//			regCurPc		<= exCtlOutPc;

			decIstrWord		<= 48'h300030003000;
			gprIstrWord		<= 48'h300030003000;
			exIstrWord		<= 48'h300030003000;
			gprIdUCmd		<= JX2_UCMD_NOP;
			exOpCmd			<= JX2_UCMD_NOP;
			exOpCmdB		<= JX2_UCMD_NOP;
			exOpCmdC		<= JX2_UCMD_NOP;
			exOpCmdFp		<= JX2_UCMD_NOP;
		end
		else
		begin
			exBranchFlush	<= 0;
		end

	end

	/* MA */

//	exMemDataLd		<= dcRegOutData;
//	exMemDataOK		<= dcRegOutOK;

`ifdef JX2_MEM_USELSQ

	lsqExAddr		<= exMemAddr;
	lsqExOpm		<= exMemOpm;
	lsqExDataI		<= exMemData;
	lsqExIdRn		<= exMemOutId;

//	dcRegInAddr		<= lsqMemAddr;
//	dcRegInOpm		<= lsqMemOpm;
//	dcRegInData		<= lsqMemDataO;

`else

	dcRegInAddr		<= exMemAddr;
	dcRegInOpm		<= exMemOpm;
	dcRegInData		<= exMemData;

`endif

end


endmodule
