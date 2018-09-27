
/*
IF, ID1, ID2, EX, [MA*, WB]

 */

`include "Jx2CoreDefs.v"

`include "Jx2MemTileB.v"
`include "Jx2DcTileB.v"
`include "Jx2IcTileB.v"

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

reg[47:0]		memRegInAddr;		//input PC address
reg[127:0]		memRegInData;		//input data (store)
reg[4:0]		memRegInOpm;		//Operation Size/Type
wire[127:0]		memRegOutData;		//output data (load)
wire[1:0]		memRegOutOK;		//set if operation suceeds

Jx2MemTileB	memTile(
	clock, reset,
	memRegInAddr,	memRegInData,
	memRegOutData,	memRegOutOK,
	memRegInOpm,
	
	memInData,		memOutData,		memAddr,
	memOpm,			memOK,
	mmioInData,		mmioOutData,	mmioAddr,
	mmioOpm,		mmioOK
	);


/* D1 Cache */

reg [63: 0]		dcRegInAddr;		//input address
reg [ 4: 0]		dcRegInOpm;
reg [63: 0]		dcRegInData;		//input data (store)
wire[63: 0]		dcRegOutData;		//output data (load)
wire[ 1: 0]		dcRegOutOK;			//set if we have a valid value.

reg [127:0]		dcMemPcDataI;		//memory data in
wire[127:0]		dcMemPcDataO;		//memory data out
reg [  1:0]		dcMemPcOK;			//memory OK
wire[ 47:0]		dcMemPcAddr;		//memory address
wire[  4:0]		dcMemPcOpm;			//memory output-enable

Jx2DcTileB	dcTile(
	clock, reset,
	dcRegInAddr,		dcRegInOpm,
	dcRegInData,		dcRegOutData,
	dcRegOutOK,
	dcMemPcAddr,		dcMemPcOpm,
	dcMemPcDataI,		dcMemPcDataO,
	dcMemPcOK
	);


/* I1 Cache */

reg [63: 0]		icRegInPc;		//input PC address
wire[47: 0]		icRegOutPcVal;	//output PC value
wire[ 1: 0]		icRegOutPcOK;		//set if we have a valid value.
wire[ 1: 0]		icRegStepPc;

reg [127:0]		icMemPcData;		//memory PC data
reg [  1:0]		icMemPcOK;		//memory PC OK
wire[ 47:0]		icMemPcAddr;		//memory PC address
wire[  4:0]		icMemPcOpm;		//memory PC output-enable

Jx2IcTileB	icTile(
	clock, reset,
	icRegInPc,
	icRegOutPcVal,	icRegOutPcOK,
	icRegStepPc,
	icMemPcData,	icMemPcAddr,
	icMemPcOpm,		icMemPcOK
	);


/* Decode */

reg [63: 0]		decRegPc;		//input PC address
reg [47: 0]		decIstrWord;
reg [ 1: 0]		decRegStepPc;

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

reg[63:0]		gprExNextSp2;		//
reg[63:0]		gprExNextPc2;		//PC in
reg[63:0]		gprExNextPr2;		//PR in
reg[63:0]		gprExNextSr2;		//SR in
reg[63:0]		gprExNextVbr2;		//VBR
reg[63:0]		gprExNextDlr2;		//DLR
reg[63:0]		gprExNextDhr2;		//DHR
reg[63:0]		gprExNextGbr2;		//GBR
reg[63:0]		gprExNextTbr2;		//TBR

wire[63:0]		gprOregSp;			//SP (R15)
wire[63:0]		gprOregPc;			//PC
wire[63:0]		gprOregPr;			//PR
wire[63:0]		gprOregSr;			//SR
wire[63:0]		gprOregVbr;			//VBR
wire[63:0]		gprOregDlr;			//DLR
wire[63:0]		gprOregDhr;			//DHR
wire[63:0]		gprOregGbr;			//GBR
wire[63:0]		gprOregTbr;			//TBR

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

	gprOregSp,		gprExNextSp2,
	gprOregPc,		gprExNextPc2,
	gprOregPr,		gprExNextPr2,
	gprOregSr,		gprExNextSr2,
	gprOregVbr,		gprExNextVbr2,
	gprOregDlr,		gprExNextDlr2,
	gprOregDhr,		gprExNextDhr2,
	gprOregGbr,		gprExNextGbr2,
	gprOregTbr,		gprExNextTbr2
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

reg[6:0]		exRegIdRm;
reg[6:0]		exRegIdRn;
reg[6:0]		exRegIdRi;
reg[47:0]		exIstrWord;
reg[63:0]		exIdCurPc;		//PC to current instruction
reg[ 1:0]		exRegStepPc;

reg[63:0]		exRegValRm;		//Rm input value
reg[63:0]		exRegValRn;		//Rn input value
reg[63:0]		exRegValRi;		//Rn input value
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

	exOpCmd,		exRegIdIxt,
	exRegIdRm,		exRegValRm,
	exRegIdRn,		exRegValRn,
	exRegIdRi,		exRegValRi,
	exRegIdRm,		exRegValCm,
	exImmValRi,		exIdInGenPc,
	exRegOutId,		exRegOutVal,
	exRegOutIdCn,	exRegOutValCn,
	exRegOutOK,

	exRegValFRm,	exRegValFRn,

	exMemAddr,		exMemData,
	exMemOpm,		exMemOpCmd2,
	exMemDataLd,	exMemDataOK,

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

wire[63:0]		exRegOutValFn;		//FRn output value
wire[6:0]		exRegOutIdFn;		//FRn, value to write
wire[1:0]		exRegOutFpOK;		//execute status (FPU)
wire[63:0]		exCtlOutFpDlr;		//DLR out (FPU)

Jx2FpuExOp	exFpOp(
	clock,		reset,
	exOpCmd,		exRegIdIxt,
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
reg		exNextPc;
reg		exBranchFlush;

reg[63:0]	exRegNextPc;

always @*
begin
	exHold			= 0;
	if(exRegOutOK==UMEM_OK_HOLD)
		exHold		= 1;
	if(exRegOutFpOK==UMEM_OK_HOLD)
		exHold		= 1;
	if(dcRegOutOK==UMEM_OK_HOLD)
		exHold		= 1;
	if(icRegOutPcOK==UMEM_OK_HOLD)
		exHold		= 1;

//	if((exMemOpm==0) &&
//				((exMemDataOK==UMEM_OK_OK) ||
//				(dcRegOutOK==UMEM_OK_OK)))
//		exHold		= 1;

	exMemDataLd		= dcRegOutData;
	exMemDataOK		= dcRegOutOK;

//	dcRegInAddr		= exMemAddr;
//	dcRegInOpm		= exMemOpm;
//	dcRegInData		= exMemData;

	icMemPcOK		= UMEM_OK_HOLD;
	dcMemPcOK		= UMEM_OK_HOLD;
	icMemPcData		= memRegOutData;
	dcMemPcDataI	= memRegOutData;

	memRegInAddr	= 0;
	memRegInData	= UV128_XX;
	memRegInOpm		= UMEM_OPM_READY;

	if(icMemPcOpm!=0)
	begin
		icMemPcData		= memRegOutData;
		icMemPcOK		= memRegOutOK;
		memRegInAddr	= icMemPcAddr;
		memRegInData	= UV128_XX;
		memRegInOpm		= icMemPcOpm;
	end
	else
	if(dcMemPcOpm!=0)
	begin
		dcMemPcDataI	= memRegOutData;
		dcMemPcOK		= memRegOutOK;
		memRegInAddr	= dcMemPcAddr;
		memRegInData	= dcMemPcDataO;
		memRegInOpm		= dcMemPcOpm;
	end
	else
	begin
		icMemPcOK		= UMEM_OK_READY;
		dcMemPcOK		= UMEM_OK_READY;
	end


	gprRegIdRo		= exRegOutId;
	gprRegValRo		= exRegOutVal;

	gprRegIdCo		= exRegOutIdCn;
	gprRegValCo		= exRegOutValCn;

	fprRegIdRo		= exRegOutIdFn;
	fprRegValRo		= exRegOutValFn;

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

//	if(exHold)
//		exRegNextPc		= gprOregPc;
//	else
//		exRegNextPc		= (gprOregPc+2);

//	exRegNextPc		= gprOregPc+ {61'h0, icRegStepPc, 1'b0};
//	exRegNextPc		= gprOregPc+ {61'h0, dcRegStepPc, 1'b0};

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

	gprRegExHold	= exHold;

end

reg[31:0]		regCurPc;

always @ (posedge clock)
begin
//	gprRegExHold		<= exHold;
	
	if(exHold)
	begin
//		$display("Jx2ExUnit: Hold Ex=%d Dc=%d Ic=%d",
//			exRegOutOK, dcRegOutOK, icRegOutPcOK);
	end
	else
	begin

	/* IF */
		icRegInPc		<= gprOregPc;

	/* ID1 */
		decRegPc		<= icRegInPc;
		decIstrWord		<= exBranchFlush ? 48'h300030003000 : icRegOutPcVal;
		decRegStepPc	<= exBranchFlush ? 2'b00 : icRegStepPc;

//		decIstrWord		<= icRegOutPcVal;
//		$display("ID1 %X %X", decRegPc, decIstrWord);

	/* ID2 */
		gprRegIdRm		<= decIdRegM;
		gprRegIdRn		<= decIdRegN;
		gprRegIdRi		<= decIdRegO;
		gprRegSrVal		<= gprOregSr;
		gprIdImm		<= decIdImm;
		gprIdValPc		<= decRegPc;
		gprIstrWord		<= decIstrWord;
		gprRegStepPc	<= decRegStepPc;

		gprIdUCmd		<= decIdUCmd;
		gprIdUIxt		<= decIdUIxt;

//		$display("ID2 %X %X %X-%X Rm=%X(%X) Rn=%X(%X) Imm=%X",
//			gprIdValPc, gprIstrWord,
//			gprIdUCmd, gprIdUIxt,
//			gprRegIdRm, gprRegValRm, gprRegIdRn, gprRegValRn,
//			gprIdImm);

	/* EX */
//		exOpCmd			<= exBranchFlush ? BSR_UCMD_NOP: gprIdUCmd;
		exOpCmd			<= gprIdUCmd;
		exRegIdIxt		<= gprIdUIxt;
		exRegIdRm		<= gprRegIdRm;
		exRegIdRn		<= gprRegIdRn;
		exRegIdRi		<= gprRegIdRi;
//		exRegIdCm		<= gprRegIdCm;
		exIdCurPc		<= gprIdValPc;
		exRegStepPc		<= gprRegStepPc;

		exRegValRm		<= gprRegValRm;
		exRegValRn		<= gprRegValRn;
		exRegValRi		<= gprRegValRi;
		exRegValCm		<= gprRegValCm;
		exImmValRi		<= gprIdImm;
//		exIdInGenPc		<= (gprIdValPc+2);
		exIdInGenPc		<= (gprIdValPc + {61'h0, gprRegStepPc, 1'b0} );
		exIstrWord		<= gprIstrWord;

		exRegValFRm		<= fprRegValRm;
		exRegValFRn		<= fprRegValRn;

//		exBranchFlush	<= (exCtlOutPc!=exCtlInPc);
//		exBranchFlush	<= 0;

//		$display("EX %X %X %X-%X DLR=%X Rm=%X(%X) Rn=%X(%X) Ro=%X(%X)",
//			exIdCurPc[47:0], exIstrWord,
//			exOpCmd, exRegIdIxt, exCtlInDlr,
//			exRegIdRm, exRegValRm,
//			exRegIdRn, exRegValRn,
//			exRegOutId, exRegOutVal);

//		if((exCtlOutPc!=exCtlInPc) ||
//			(exMemOpm==UMEM_OPM_CTRLF))
		if(exMemOpm==UMEM_OPM_CTRLF)
		begin
//			$display("BRA Flush %X %X", exCtlOutPc, exCtlInPc);
			exBranchFlush	<= 1;
//			regCurPc		<= exCtlOutPc;

			decIstrWord		<= 48'h300030003000;
			gprIstrWord		<= 48'h300030003000;
			exIstrWord		<= 48'h300030003000;
			gprIdUCmd		<= JX2_UCMD_NOP;
			exOpCmd			<= JX2_UCMD_NOP;
		end
		else
		begin
			exBranchFlush	<= 0;
		end

	end

	/* MA */

//	exMemDataLd		<= dcRegOutData;
//	exMemDataOK		<= dcRegOutOK;

	dcRegInAddr		<= exMemAddr;
	dcRegInOpm		<= exMemOpm;
	dcRegInData		<= exMemData;

end


endmodule
