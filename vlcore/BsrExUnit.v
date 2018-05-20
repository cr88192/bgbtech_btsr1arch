
/*
IF, ID1, ID2, EX, [MA*, WB]

 */

`include "BsrCoreDefs.v"

`include "BsrMemTileB.v"
`include "BsrDcTileB.v"
`include "BsrIcTileB.v"

`include "BsrRegGpr.v"
`include "BsrDecOp.v"
`include "BsrExOp.v"


module BsrExUnit(
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

reg[19:0]		memRegInAddr;		//input PC address
reg[127:0]		memRegInData;		//input data (store)
reg[4:0]		memRegInOpm;		//Operation Size/Type
wire[127:0]		memRegOutData;		//output data (load)
wire[1:0]		memRegOutOK;		//set if operation suceeds

BsrMemTileB	memTile(
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

reg [31: 0]		dcRegInAddr;		//input address
reg [ 4: 0]		dcRegInOpm;
reg [31: 0]		dcRegInData;		//input data (store)
wire[31: 0]		dcRegOutData;		//output data (load)
wire[ 1: 0]		dcRegOutOK;			//set if we have a valid value.

reg [127:0]		dcMemPcDataI;		//memory data in
wire[127:0]		dcMemPcDataO;		//memory data out
reg [  1:0]		dcMemPcOK;			//memory OK
wire[ 19:0]		dcMemPcAddr;		//memory address
wire[  4:0]		dcMemPcOpm;			//memory output-enable

BsrDcTileB	dcTile(
	clock, reset,
	dcRegInAddr,		dcRegInOpm,
	dcRegInData,		dcRegOutData,
	dcRegOutOK,
	dcMemPcAddr,		dcMemPcOpm,
	dcMemPcDataI,		dcMemPcDataO,
	dcMemPcOK
	);


/* I1 Cache */

reg [31: 0]		icRegInPc;		//input PC address
wire[15: 0]		icRegOutPcVal;	//output PC value
wire[ 1: 0]		icRegOutPcOK;		//set if we have a valid value.

reg [127:0]		icMemPcData;		//memory PC data
reg [  1:0]		icMemPcOK;		//memory PC OK
wire[ 19:0]		icMemPcAddr;		//memory PC address
wire[  4:0]		icMemPcOpm;		//memory PC output-enable

BsrIcTileB	icTile(
	clock, reset,
	icRegInPc,
	icRegOutPcVal,	icRegOutPcOK,
	icMemPcData,	icMemPcAddr,
	icMemPcOpm,		icMemPcOK
	);


/* Decode */

reg [31: 0]		decRegPc;		//input PC address
reg [15:0]		decIstrWord;

wire[ 5:0]		decIdRegN;
wire[ 5:0]		decIdRegM;
wire[31:0]		decIdImm;
wire[ 7:0]		decIdUCmd;
wire[ 7:0]		decIdUIxt;

BsrDecOp	decOp(
	clock,			reset,
	decIstrWord,
	decIdRegN,		decIdRegM,
	decIdImm,		decIdUCmd,
	decIdUIxt
	);


/* Register File */
reg[ 7:0]		gprIdUCmd;
reg[ 7:0]		gprIdUIxt;

reg[5:0]		gprRegIdRm;
reg[5:0]		gprRegIdRn;
wire[31:0]		gprRegValRm;
wire[31:0]		gprRegValRn;
reg[5:0]		gprRegIdRo;
reg[31:0]		gprRegValRo;
reg[31:0]		gprRegSrVal;
reg				gprRegExHold;

reg[31:0]		gprIdImm;
reg[31:0]		gprIdValPc;
reg [15:0]		gprIstrWord;

reg[31:0]		gprExNextSp2;		//
reg[31:0]		gprExNextPc2;		//PC in
reg[31:0]		gprExNextPr2;		//PR in
reg[31:0]		gprExNextSr2;		//SR in
reg[31:0]		gprExNextVbr2;		//VBR
reg[31:0]		gprExNextDlr2;		//DLR
reg[31:0]		gprExNextDhr2;		//DHR
reg[31:0]		gprExNextGbr2;		//GBR
reg[31:0]		gprExNextTbr2;		//TBR

wire[31:0]		gprOregSp;			//SP (R15)
wire[31:0]		gprOregPc;			//PC
wire[31:0]		gprOregPr;			//PR
wire[31:0]		gprOregSr;			//SR
wire[31:0]		gprOregVbr;			//VBR
wire[31:0]		gprOregDlr;			//DLR
wire[31:0]		gprOregDhr;			//DHR
wire[31:0]		gprOregGbr;			//GBR
wire[31:0]		gprOregTbr;			//TBR

BsrRegGpr	gprFile(
	clock,			reset,

	gprRegIdRm,		gprRegValRm,
	gprRegIdRn,		gprRegValRn,
	gprRegIdRo,		gprRegValRo,

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

/* Execute Unit */

reg[7:0]		exOpCmd;		//command opcode
reg[7:0]		exRegIdIxt;		//ALU Index / Opcode Extension

reg[5:0]		exRegIdRm;
reg[5:0]		exRegIdRn;
reg [15:0]		exIstrWord;

reg[31:0]		exRegValRm;		//Rm input value
reg[31:0]		exRegValRn;		//Rn input value
reg[31:0]		exImmValRi;		//immediate/disp value
reg[31:0]		exIdInGenPc;	//PC to next instruction

wire[31:0]		exRegOutVal;	//Rn output value
wire[5:0]		exRegOutId;		//Rn, value to write
wire[1:0]		exRegOutOK;		//execute status

wire[31:0]		exMemAddr;		//memory address
wire[31:0]		exMemData;		//memory data (store)
wire[4:0]		exMemOpm;		//mem op mode
wire[7:0]		exMemOpCmd2;	//mem EX chain
reg [31:0]		exMemDataLd;	//memory data (load)
reg [1:0]		exMemDataOK;	//memory status (load)

reg [31:0]		exCtlInSp;		//SP in
wire[31:0]		exCtlOutSp;		//SP out
reg [31:0]		exCtlInPc;		//PC in
wire[31:0]		exCtlOutPc;		//PC out
reg [31:0]		exCtlInPr;		//PR in
wire[31:0]		exCtlOutPr;		//PR out
reg [31:0]		exCtlInSr;		//SR in
wire[31:0]		exCtlOutSr;		//SR out
reg [31:0]		exCtlInVbr;
wire[31:0]		exCtlOutVbr;
reg [31:0]		exCtlInDlr;		//MACL in
wire[31:0]		exCtlOutDlr;	//MACL out
reg [31:0]		exCtlInDhr;		//MACH in
wire[31:0]		exCtlOutDhr;	//MACH out
reg [31:0]		exCtlInGbr;
wire[31:0]		exCtlOutGbr;
reg [31:0]		exCtlInTbr;
wire[31:0]		exCtlOutTbr;


BsrExOp		exOp(
	clock,			reset,

	exOpCmd,		exRegIdIxt,
	exRegIdRm,		exRegValRm,
	exRegIdRn,		exRegValRn,
	exImmValRi,		exIdInGenPc,
	exRegOutId,		exRegOutVal,
	exRegOutOK,

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


/* Common */

reg		exHold;
reg		exNextPc;
reg		exBranchFlush;

reg[31:0]	exRegNextPc;

always @*
begin
	exHold			= 0;
	if(exRegOutOK==UMEM_OK_HOLD)
		exHold		= 1;
	if(dcRegOutOK==UMEM_OK_HOLD)
		exHold		= 1;
	if(icRegOutPcOK==UMEM_OK_HOLD)
		exHold		= 1;

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
		exRegNextPc		= (gprOregPc+2);

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
		$display("BsrExUnit: Hold %d %d %d",
			exRegOutOK, dcRegOutOK, icRegOutPcOK);
	end
	else
	begin

	/* IF */
		icRegInPc		<= gprOregPc;

	/* ID1 */
		decRegPc		<= icRegInPc;
		decIstrWord		<= exBranchFlush ? 16'h3000 : icRegOutPcVal;
//		decIstrWord		<= icRegOutPcVal;
		$display("ID1 %X %X", decRegPc, decIstrWord);

	/* ID2 */
		gprRegIdRm		<= decIdRegM;
		gprRegIdRn		<= decIdRegN;
		gprRegSrVal		<= gprOregSr;
		gprIdImm		<= decIdImm;
		gprIdValPc		<= decRegPc;
		gprIstrWord		<= decIstrWord;

		gprIdUCmd		<= decIdUCmd;
		gprIdUIxt		<= decIdUIxt;

		$display("ID2 %X %X %X-%X Rm=%X(%X) Rn=%X(%X) Imm=%X",
			gprIdValPc, gprIstrWord,
			gprIdUCmd, gprIdUIxt,
			gprRegIdRm, gprRegValRm, gprRegIdRn, gprRegValRn,
			gprIdImm);

	/* EX */
//		exOpCmd			<= exBranchFlush ? BSR_UCMD_NOP: gprIdUCmd;
		exOpCmd			<= gprIdUCmd;
		exRegIdIxt		<= gprIdUIxt;
		exRegIdRm		<= gprRegIdRm;
		exRegIdRn		<= gprRegIdRn;

		exRegValRm		<= gprRegValRm;
		exRegValRn		<= gprRegValRn;
		exImmValRi		<= gprIdImm;
		exIdInGenPc		<= gprIdValPc;
		exIstrWord		<= gprIstrWord;

//		exBranchFlush	<= (exCtlOutPc!=exCtlInPc);
//		exBranchFlush	<= 0;

		$display("EX %X %X %X-%X DLR=%X->%X",
			exIdInGenPc, exIstrWord,
			exOpCmd, exRegIdIxt, gprOregDlr, exCtlInDlr);

		if(exCtlOutPc!=exCtlInPc)
		begin
			$display("BRA Flush %X %X", exCtlOutPc, exCtlInPc);
			exBranchFlush	<= 1;
//			regCurPc		<= exCtlOutPc;

			decIstrWord		<= 16'h3000;
			gprIstrWord		<= 16'h3000;
			exIstrWord		<= 16'h3000;
			gprIdUCmd		<= BSR_UCMD_NOP;
			exOpCmd			<= BSR_UCMD_NOP;
		end
		else
		begin
			exBranchFlush	<= 0;
		end

		/* MA */

		exMemDataLd		<= dcRegOutData;
		exMemDataOK		<= dcRegOutOK;

		dcRegInAddr		<= exMemAddr;
		dcRegInOpm		<= exMemOpm;
		dcRegInData		<= exMemData;

	end

end


endmodule
