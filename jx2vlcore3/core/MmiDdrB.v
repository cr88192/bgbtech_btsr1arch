/*
DDR2 / DDR3 Interface Module

Work on design for a faster module.

Run RAM at a native speed of 200MHz using a FIFO buffer.

Commands and data are organized into rising and falling edge signals.

Addr(15:14): Bank
Addr(13: 0): Row/Collumn Address

Cmd(   15): Busy		(1: Command still in progress, 0: Command Ended)
Cmd(14:12): Resv
Cmd(11:10): DQS_I	(Data Clock, Captured Input)
Cmd(    9): Resv
Cmd(    8): Data_En	(Data Ouput Enable, 1=Output, 0=Input)
Cmd( 7: 6): DQS_O	(Data Clock, Output)
Cmd(    5): DQS_En	(Data Clock Output Enable, 1=Output, 0=Input)
Cmd(    4): CKE		(Clock Enable)
Cmd(    3): CS		(Chip Select)
Cmd(    2): RAS		(Row Address Strobe)
Cmd(    1): CAS		(Col Address Strobe)
Cmd(    0): WE		(Write Enable)

*/

`include "CoreDefs.v"

module MmiDdrB(
	clock_100,	clock_200,	reset,
	memDataIn,	memDataOut,
	memAddr,	memOpm,
	memOK,
	
	ddrData_I,	ddrData_O,	ddrData_En,
	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP_I,	ddrDqsN_I,
	ddrDqsP_O,	ddrDqsN_O,	ddrDqs_En);

input			clock;
input			clock200;
input			reset;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
input[31:0]		memAddr;
input[4:0]		memOpm;
output[1:0]		memOK;
	
// inout[15:0]		ddrData;		//DDR data pins
input[15:0]		ddrData_I;		//DDR data pins
output[15:0]	ddrData_O;		//DDR data pins
output			ddrData_En;		//DDR data pins

output[13:0]	ddrAddr;		//Address pins
output[2:0]		ddrBa;			//Bank Address pins

output			ddrCs;			//Chip Select
output			ddrRas;			//Rowq Address Strobe
output			ddrCas;			//Collumn Address Strobe
output			ddrWe;			//Write Enable
output			ddrCke;			//Clock Enable

// output[9:0]		ddrCmd;			//Command/Address pins
output[1:0]		ddrClk;			//clock pins

input[1:0]		ddrDqsP_I;
input[1:0]		ddrDqsN_I;
output[1:0]		ddrDqsP_O;
output[1:0]		ddrDqsN_O;
output			ddrDqs_En;

parameter[15:0]	DDR_CAS_RL_M1	= 3;	//CAS RL Minus 1
parameter[15:0]	DDR_CAS_WL_M1	= 1;	//CAS WL Minus 1
parameter[15:0]	DDR_RAS_M1		= 8;	//RAS Minus 1

parameter[15:0]	DDR_RAS_INIT	= 128;	//Wait several uS


parameter[15:0]	DDR_DRI_INIT_13	=	16'h8201;	/* Delay, NOP, 200+ cyc */
// parameter[15:0]	DDR_DRI_INIT_12	=	16'h2004;	/* Load EMR, OCD Exit */
parameter[15:0]	DDR_DRI_INIT_12	=	16'h2005;	/* Load EMR, OCD Exit */
// parameter[15:0]	DDR_DRI_INIT_11	=	16'h2384;	/* Load EMR, OCD Default */
parameter[15:0]	DDR_DRI_INIT_11	=	16'h2385;	/* Load EMR, OCD Default */
parameter[15:0]	DDR_DRI_INIT_10	=	16'h0233;	/* Load MR, Normal */
parameter[15:0]	DDR_DRI_INIT_9	=	16'h8900;	/* REFRESH */
parameter[15:0]	DDR_DRI_INIT_8	=	16'h8900;	/* REFRESH */
parameter[15:0]	DDR_DRI_INIT_7	=	16'h8800;	/* PRELOAD ALL */
parameter[15:0]	DDR_DRI_INIT_6	=	16'h0333;	/* Load MR, Reset DLL */
// parameter[15:0]	DDR_DRI_INIT_5	=	16'h2004;	/* Load EMR */
parameter[15:0]	DDR_DRI_INIT_5	=	16'h2005;	/* Load EMR, DLL Disable */
parameter[15:0]	DDR_DRI_INIT_4	=	16'h6000;	/* Load EMR3 */
parameter[15:0]	DDR_DRI_INIT_3	=	16'h4000;	/* Load EMR2 */
parameter[15:0]	DDR_DRI_INIT_2	=	16'h8800;	/* PRELOAD ALL */
parameter[15:0]	DDR_DRI_INIT_1	=	16'h8201;	/* Delay, NOP, 200+ cyc */
parameter[15:0]	DDR_DRI_INIT_0	=	16'h8628;	/* Delay, CKE=0, 200+ us */


parameter[15:0]	DDR_CMD_IDLE	= 16'h0017;

reg[15:0]		tDdrData;
reg				tDdrOut;

wire[15:0]		ddrData;
assign			ddrData_O = tDdrData;
assign			ddrData = ddrData_I;
assign			ddrData_En = tDdrOut;

wire[1:0]		ddrDqs;
assign			ddrDqs = { ddrDqsN_I[0], ddrDqsP_I[0] };


reg[15:0]		ddrData2;
reg[15:0]		ddrData2A;
reg[15:0]		ddrData1H;
reg[15:0]		ddrData2H;

reg[1:0]		ddrDqs2;
reg[1:0]		ddrDqs1H;
reg[1:0]		ddrDqs2H;

reg[1:0]		tDdrClk;			//clock pins
assign			ddrClk = tDdrClk;

reg[1:0]	tDdrDqs;
reg			tDdrDqs_En;

assign		ddrDqsP_O	= tDdrDqs[0] ? 2'b11 : 2'b00;
assign		ddrDqsN_O	= tDdrDqs[1] ? 2'b11 : 2'b00;
assign		ddrDqs_En	= tDdrDqs_En;

reg[13:0]		tDdrAddr;		//Address pins
reg[2:0]		tDdrBa;			//Bank Address pins
reg				tDdrCs;
reg				tDdrRas;
reg				tDdrCas;
reg				tDdrWe;
reg				tDdrCke;

assign		ddrAddr	= tDdrAddr;
assign		ddrBa	= tDdrBa;
assign		ddrCs	= tDdrCs;
assign		ddrRas	= tDdrRas;
assign		ddrCas	= tDdrCas;
assign		ddrWe	= tDdrWe;
assign		ddrCke	= tDdrCke;


reg[1:0]		tMemOK;
assign			memOK = tMemOK2;

reg[127:0]		tMemDataOut;
assign			memDataOut = tMemDataOut2;

reg[127:0]		tMemDataIn;
reg[31:0]		tMemAddr;
reg[4:0]		tMemOpm;


reg[127:0]		tReqDdrFifoCmdP;
reg[127:0]		tReqDdrFifoAddrP;
reg[127:0]		tReqDdrFifoDataOP;
reg[127:0]		tReqDdrFifoDataIP;
reg[127:0]		tReqDdrFifoCmdN;
reg[127:0]		tReqDdrFifoAddrN;
reg[127:0]		tReqDdrFifoDataON;
reg[127:0]		tReqDdrFifoDataIN;
reg				tDdrFifoDoReq;

reg[127:0]		tResDdrFifoCmdP;
reg[127:0]		tResDdrFifoDataP;
reg[127:0]		tResDdrFifoCmdN;
reg[127:0]		tResDdrFifoDataN;



reg[127:0]		tDdrFifoCmdP;
reg[127:0]		tDdrFifoAddrP;
reg[127:0]		tDdrFifoDataOP;
reg[127:0]		tDdrFifoDataIP;
reg[127:0]		tDdrFifoCmdN;
reg[127:0]		tDdrFifoAddrN;
reg[127:0]		tDdrFifoDataON;
reg[127:0]		tDdrFifoDataIN;

reg[127:0]		tNxtDdrFifoCmdP;
reg[127:0]		tNxtDdrFifoAddrP;
reg[127:0]		tNxtDdrFifoDataOP;
reg[127:0]		tNxtDdrFifoDataIP;
reg[127:0]		tNxtDdrFifoCmdN;
reg[127:0]		tNxtDdrFifoAddrN;
reg[127:0]		tNxtDdrFifoDataON;
reg[127:0]		tNxtDdrFifoDataIN;

reg				tDdrFifoBusyP;
reg				tDdrFifoBusyN;
reg				tNxtDdrFifoBusyP;
reg				tNxtDdrFifoBusyN;

reg[15:0]		tCurDdrFifoCmdP;
reg[15:0]		tCurDdrFifoAddrP;
reg[15:0]		tCurDdrFifoDataOP;
reg[15:0]		tCurDdrFifoCmdN;
reg[15:0]		tCurDdrFifoAddrN;
reg[15:0]		tCurDdrFifoDataON;

reg[15:0]		tFifoCmdTP;
reg[15:0]		tFifoCmdTN;

always @*
begin
	tCurDdrFifoCmdP		= tDdrFifoCmdP[15:0];
	tCurDdrFifoAddrP	= tDdrFifoAddrP[15:0];
	tCurDdrFifoDataOP	= tDdrFifoDataOP[15:0];
	tNxtDdrFifoBusyP	= tCurDdrFifoCmdP[15];

	tFifoCmdTP			= DDR_CMD_IDLE;
	tFifoCmdTP[11:10]	= ddrDqs;
	tNxtDdrFifoCmdP		= { tFifoCmdT, tDdrFifoCmdP  [127:16] };
	tNxtDdrFifoAddrP	= {  16'h0000, tDdrFifoAddrP [127:16] };
	tNxtDdrFifoDataOP   = {  16'h0000, tDdrFifoDataOP[127:16] };
	tNxtDdrFifoDataIP   = { ddrData_I, tDdrFifoDataIP[127:16] };

	if(tDdrFifoDoReq && !tDdrFifoBusyP)
	begin
		tNxtDdrFifoBusyP	= 1;
		tNxtDdrFifoCmdP		= tReqDdrFifoCmdP;
		tNxtDdrFifoAddrP	= tRegDdrFifoAddrP;
		tNxtDdrFifoDataOP   = tReqDdrFifoDataOP;
		tNxtDdrFifoDataIP   = tReqDdrFifoDataIP;
	end
end

always @*
begin
	tCurDdrFifoCmdN		= tDdrFifoCmdN[15:0];
	tCurDdrFifoAddrN	= tDdrFifoAddrN[15:0];
	tCurDdrFifoDataON	= tDdrFifoDataON[15:0];
	tNxtDdrFifoBusyN	= tCurDdrFifoCmdN[15];

	tFifoCmdTN			= DDR_CMD_IDLE;
	tFifoCmdTN[11:10]	= ddrDqs;
	tNxtDdrFifoCmdN		= { tFifoCmdT, tDdrFifoCmdN  [127:16] };
	tNxtDdrFifoAddrN	= {  16'h0000, tDdrFifoAddrN [127:16] };
	tNxtDdrFifoDataON   = {  16'h0000, tDdrFifoDataON[127:16] };
	tNxtDdrFifoDataIN   = { ddrData_I, tDdrFifoDataIN[127:16] };

	if(tDdrFifoDoReq && !tDdrFifoBusyN)
	begin
		tNxtDdrFifoBusyN	= 1;
		tNxtDdrFifoCmdN		= tReqDdrFifoCmdN;
		tNxtDdrFifoAddrN	= tRegDdrFifoAddrN;
		tNxtDdrFifoDataON   = tReqDdrFifoDataON;
		tNxtDdrFifoDataIN   = tReqDdrFifoDataIN;
	end
end

always @(posedge clock_200)
begin
	tDdrFifoBusyP		<= tNxtDdrFifoBusyP;
	tDdrFifoCmdP		<= tNxtDdrFifoCmdP;
	tDdrFifoAddrP		<= tNxtDdrFifoAddrP;
	tDdrFifoDataOP		<= tNxtDdrFifoDataOP;
	tDdrFifoDataIP		<= tNxtDdrFifoDataIP;

	tDdrClk				<= 2'b10;
	tDdrOut				<= tCurDdrFifoCmdP[8];
	tDdrDqs				<= tCurDdrFifoCmdP[7:6];
	tDdrDqs_En			<= tCurDdrFifoCmdP[5];
	tDdrCke				<= tCurDdrFifoCmdP[4];
	tDdrCs				<= tCurDdrFifoCmdP[3];
	tDdrRas				<= tCurDdrFifoCmdP[2];
	tDdrCas				<= tCurDdrFifoCmdP[1];
	tDdrWe				<= tCurDdrFifoCmdP[0];
	tDdrBa				<= tCurDdrFifoAddrP[15:14];
	tDdrAddr			<= tCurDdrFifoAddrP[13:0];
	tDdrData			<= tCurDdrFifoDataOP;

	if(tDdrFifoBusyP && !tNxtDdrFifoBusyP)
	begin
		tResDdrFifoCmdP		<= tDdrFifoCmdP;
		tResDdrFifoDataP	<= tDdrFifoDataIP;
	end
end

always @(negedge clock_200)
begin
	tDdrFifoBusyN		<= tNxtDdrFifoBusyN;
	tDdrFifoCmdN		<= tNxtDdrFifoCmdN;
	tDdrFifoAddrN		<= tNxtDdrFifoAddrN;
	tDdrFifoDataON		<= tNxtDdrFifoDataON;
	tDdrFifoDataIN		<= tNxtDdrFifoDataIN;

	tDdrClk				<= 2'b01;
	tDdrOut				<= tCurDdrFifoCmdN[8];
	tDdrDqs				<= tCurDdrFifoCmdN[7:6];
	tDdrDqs_En			<= tCurDdrFifoCmdN[5];
	tDdrCke				<= tCurDdrFifoCmdN[4];
	tDdrCs				<= tCurDdrFifoCmdN[3];
	tDdrRas				<= tCurDdrFifoCmdN[2];
	tDdrCas				<= tCurDdrFifoCmdN[1];
	tDdrWe				<= tCurDdrFifoCmdN[0];
	tDdrBa				<= tCurDdrFifoAddrN[15:14];
	tDdrAddr			<= tCurDdrFifoAddrN[13:0];
	tDdrData			<= tCurDdrFifoDataON;

	if(tDdrFifoBusyN && !tNxtDdrFifoBusyN)
	begin
		tResDdrFifoCmdN		<= tDdrFifoCmdN;
		tResDdrFifoDataN	<= tDdrFifoDataIN;
	end
end 

endmodule
