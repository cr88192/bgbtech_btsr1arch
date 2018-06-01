
/*
IF, ID1, ID2, EX, [MA*, WB]

 */

`include "BsrCoreDefs.v"

`include "BsrMemTileB.v"
`include "BsrDcTileB.v"
`include "BsrIcTileB.v"

// `include "BsrRegGpr.v"
// `include "BsrDecOp.v"
// `include "BsrExOp.v"


module BsrTstMem(
	/* verilator lint_off UNUSED */
	clock,		reset,
	regInAddr,		regInOpm,
	regInData,		regOutData,
	regOutOK,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

input			clock;			//clock
input			reset;			//reset


input [31: 0]	regInAddr;		//input address
input [ 4: 0]	regInOpm;
input [31: 0]	regInData;		//input data (store)
output[31: 0]	regOutData;		//output data (load)
output[ 1: 0]	regOutOK;		//set if we have a valid value.


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


//reg[127:0]		tMemOutData;	//memory PC data
//reg[31:0]		tMemAddr;		//memory PC address
//reg[4:0]		tMemOpm;		//memory PC output-enable

//assign			memOutData	= tMemOutData;
//assign			memAddr		= tMemAddr;
//assign			memOpm		= tMemOpm;


reg[31:0]		tRegOutData;	//output PC value
reg[ 1:0]		tRegOutOK;	//set if we have a valid value.

assign	regOutData	= tRegOutData;
assign	regOutOK	= tRegOutOK;


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


/* Common */

reg		exHold;
reg		exNextPc;
reg		exBranchFlush;

reg[31:0]	exRegNextPc;

always @*
begin
	exHold			= 0;
//	if(exRegOutOK==UMEM_OK_HOLD)
//		exHold		= 1;
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

	dcRegInAddr		= regInAddr;		//input address
	dcRegInOpm		= regInOpm;
	dcRegInData		= regInData;		//input data (store)

	tRegOutData		= dcRegOutData;	//output PC value
	tRegOutOK		= dcRegOutOK;	//set if we have a valid value.

end

reg[31:0]		regCurPc;

always @ (posedge clock)
begin
	
	if(exHold)
	begin
	end

end


endmodule
