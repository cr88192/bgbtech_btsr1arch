/*
DDR2 / DDR3 Interface Module

State Machine, Transmit/Recieve Words

Assumes a 16-bit wide DDR interface, with a nominal burst length of 8.
This will move 128 bits to or from DRAM.

This version will attempt to run at the clock 1:1, using posedge and negedge to drive outputs. Will still use a big state-machine.

This will be given 3 clock signals:
  clock, will be the clock for the IO bus;
  clock2, will be the clock to run the RAM at;
  clock3, will be 2x clock2, used for clock-edge signaling.



CAS = Column Activate Strobe
  Divided into RL and WL.
    RL=Read Latency
    WL=Write Latency

The RL/WL values would depend both on chipset and what is written into the Mode Registers. These should match.

RAS = Row Activate Strobe
  This is how many cycles between row activation and beginning a Read/Write.


Note that for most states, the LSB will encode which clock-edge the state is active on. Where 0 is for Rising-Edge, and 1 is for Falling Edge.

States:
	00-0000		//Idle, Awaiting Request
	00-0001		//Hold, Waiting to return to Idle
	00-0010		//Mode Register Write

	00-1zzz		//Precharge Row

	00-1000		//Precharge, Row Activate
	00-1010		//Precharge, RAS Hold
	00-1100		//Precharge, Precharge
	00-1110		//Precharge, Wait Spin, Returns Idle

	01-0000		//Read, Row Activate
	01-0010		//Read, RAS Hold
	01-0100		//Read, Column Activate
	01-0110		//Read, CAS/RL Hold

	01-1000		//Write, Row Activate
	01-1010		//Write, RAS Hold
	01-1100		//Write, Column Activate
	01-1110		//Write, CAS/WL Hold

	10-zzzz		//Read Burst
	11-zzzz		//Write Burst

Note: ddrData_I/ddrData_O;
These exist because verilator lacks tristate IO.
The En signals indicate whether to enable/drive the outputs, otherwise these function as inputs.


DRI Init:
	0mma-aaaa-aaaa-aaaa:	Load Mode Register
		mm: MR/EMR to Load
		aa: Bits to Load into MR
	1000-0kss-dddd-dddd:	Delay
		ss: Shift Delay left 0/4/8 bits
		k: Disable CKE

*/

`include "CoreDefs.v"

module MmiModDdrB(
	clock,		clock2,		clock3,
//	clock,		clock2,
	reset,		reset2,
	memDataIn,	memDataOut,
	memAddr,	memOpm,
	memOK,
	
	ddrData_I,	ddrData_O,	ddrData_En,
	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP_I,	ddrDqsN_I,
	ddrDqsP_O,	ddrDqsN_O,	ddrDqs_En);

/* verilator lint_off UNUSED */

input			clock;
input			clock2;
input			clock3;
input			reset;
input			reset2;
`input_ddrtile	memDataIn;
`output_ddrtile	memDataOut;
input[31:0]		memAddr;
input[4:0]		memOpm;
output[1:0]		memOK;
	
// inout[15:0]		ddrData;		//DDR data pins
input[15:0]		ddrData_I;		//DDR data pins
output[15:0]	ddrData_O;		//DDR data pins
output			ddrData_En;		//DDR data pins

output[13:0]	ddrAddr;		//Address pins
output[2:0]		ddrBa;			//Bank Address pins

output			ddrCs;
output			ddrRas;
output			ddrCas;
output			ddrWe;
output			ddrCke;

// output[9:0]		ddrCmd;			//Command/Address pins
output[1:0]		ddrClk;			//clock pins

input[1:0]		ddrDqsP_I;
input[1:0]		ddrDqsN_I;
output[1:0]		ddrDqsP_O;
output[1:0]		ddrDqsN_O;
output			ddrDqs_En;

// parameter[15:0]	DDR_CAS_RL_M1	= 5;	//CAS RL Minus 1
parameter[15:0]	DDR_CAS_RL_M1	= 4;	//CAS RL Minus 1
// parameter[15:0]	DDR_CAS_RL_M1	= 3;	//CAS RL Minus 1
// parameter[15:0]	DDR_CAS_RL_M1	= 3;	//CAS RL Minus 1
// parameter[15:0]	DDR_CAS_RL_M1	= 2;	//CAS RL Minus 1
// parameter[15:0]	DDR_CAS_WL_M1	= 4;	//CAS WL Minus 1
// parameter[15:0]	DDR_CAS_WL_M1	= 2;	//CAS WL Minus 1
// parameter[15:0]	DDR_CAS_WL_M1	= 1;	//CAS WL Minus 1
parameter[15:0]	DDR_CAS_WL_M1	= 0;	//CAS WL Minus 1
parameter[15:0]	DDR_RAS_M1		= 8;	//RAS Minus 1
// parameter[15:0]	DDR_RAS_M1		= 3;	//RAS Minus 1 (~ 40ns needed)

parameter[15:0]	DDR_RAS_INIT	= 128;	//Wait several uS

// parameter[63:0]	DDR_DRI_INIT	= 64'h0000_0122_0201_0AFF;
// parameter[63:0]	DDR_DRI_INIT	= 64'h4000_2000_0010_8200;
// parameter[63:0]	DDR_DRI_INIT	= 64'h4000_2000_0033_8200;
//parameter[127:0]	DDR_DRI_INIT	= 
//	128'h2000_2380_0033_8202_0133_2000_6000_4000_8201_8628;


parameter[15:0]	DDR_DRI_INIT_13	=	16'h8201;	/* Delay, NOP, 200+ cyc */

`ifdef mod_ddrb_dllenable
parameter[15:0]	DDR_DRI_INIT_12	=	16'h2004;	/* Load EMR, OCD Exit */
parameter[15:0]	DDR_DRI_INIT_11	=	16'h2384;	/* Load EMR, OCD Default */
`else
parameter[15:0]	DDR_DRI_INIT_12	=	16'h2005;	/* Load EMR, OCD Exit */
parameter[15:0]	DDR_DRI_INIT_11	=	16'h2385;	/* Load EMR, OCD Default */
`endif

parameter[15:0]	DDR_DRI_INIT_10	=	16'h0233;	/* Load MR, Normal */
parameter[15:0]	DDR_DRI_INIT_9	=	16'h8900;	/* REFRESH */
parameter[15:0]	DDR_DRI_INIT_8	=	16'h8900;	/* REFRESH */
parameter[15:0]	DDR_DRI_INIT_7	=	16'h8800;	/* PRELOAD ALL */
parameter[15:0]	DDR_DRI_INIT_6	=	16'h0333;	/* Load MR, Reset DLL */
`ifdef mod_ddrb_dllenable
parameter[15:0]	DDR_DRI_INIT_5	=	16'h2004;	/* Load EMR, DLL Enable */
`else
parameter[15:0]	DDR_DRI_INIT_5	=	16'h2005;	/* Load EMR, DLL Disable */
`endif
parameter[15:0]	DDR_DRI_INIT_4	=	16'h6000;	/* Load EMR3 */
parameter[15:0]	DDR_DRI_INIT_3	=	16'h4000;	/* Load EMR2 */
parameter[15:0]	DDR_DRI_INIT_2	=	16'h8800;	/* PRELOAD ALL */
parameter[15:0]	DDR_DRI_INIT_1	=	16'h8201;	/* Delay, NOP, 200+ cyc */
parameter[15:0]	DDR_DRI_INIT_0	=	16'h8628;	/* Delay, CKE=0, 200+ us */

// parameter[127:0]	DDR_DRI_INIT	= 



wire[1:0]		ddrDqs;
assign			ddrDqs = { ddrDqsN_I[0], ddrDqsP_I[0] };

wire	ddr_coreclock;
wire	ddr_coreclock2;
wire	ddr_corereset;

`ifdef mod_ddr_fastcore
assign	ddr_coreclock = clock2;
assign	ddr_corereset = reset2;
assign	ddr_coreclock2 = clock3;
`else
assign	ddr_coreclock = clock;
assign	ddr_corereset = reset;
`endif


// reg[15:0]		ddrData2;
// reg[15:0]		ddrData2A;
// reg[15:0]		ddrData1H;
// reg[15:0]		ddrData2H;

// reg[1:0]		ddrDqs2;
// reg[1:0]		ddrDqs1H;
// reg[1:0]		ddrDqs2H;

// reg[1:0]		tDdrClk;			//clock pins
// reg[1:0]		tDdrClk2;			//clock pins


// reg[1:0]	tDdrDqs;
reg			tDdrDqs_En;
reg			tDdrLastDqs_En;

// reg[1:0]	tDdrDqs2;
// reg			tDdrDqs2_En;


reg[13:0]		tDdrLastAddrP;		//Address pins
reg[2:0]		tDdrLastBaP;		//Bank Address pins
reg				tDdrLastCsP;
reg				tDdrLastRasP;
reg				tDdrLastCasP;
reg				tDdrLastWeP;
reg				tDdrLastCkeP;
reg[15:0]		tDdrLastDataOutP;
reg				tDdrLastOutP;
reg[1:0]		tDdrLastDqsP;

reg[13:0]		tDdrLastAddrN;		//Address pins
reg[2:0]		tDdrLastBaN;		//Bank Address pins
reg				tDdrLastCsN;
reg				tDdrLastRasN;
reg				tDdrLastCasN;
reg				tDdrLastWeN;
reg				tDdrLastCkeN;
reg[15:0]		tDdrLastDataOutN;
reg				tDdrLastOutN;
reg[1:0]		tDdrLastDqsN;

/* Outputs, Posedge */
reg[13:0]		tDdrAddrP;		//Address pins
reg[2:0]		tDdrBaP;		//Bank Address pins
reg				tDdrCsP;
reg				tDdrRasP;
reg				tDdrCasP;
reg				tDdrWeP;
reg				tDdrCkeP;
reg[15:0]		tDdrDataOutP;
reg				tDdrOutP;
reg[1:0]		tDdrDqsP;
reg				tDdrDqsP_En;

reg[15:0]		tDdrDataInP;
reg[1:0]		tDdrDqsInP;
reg[15:0]		tDdrDataInP1;
reg[1:0]		tDdrDqsInP1;

/* Outputs, Negedge */
reg[13:0]		tDdrAddrN;		//Address pins
reg[2:0]		tDdrBaN;		//Bank Address pins
reg				tDdrCsN;
reg				tDdrRasN;
reg				tDdrCasN;
reg				tDdrWeN;
reg				tDdrCkeN;
reg[15:0]		tDdrDataOutN;
reg				tDdrOutN;
reg[1:0]		tDdrDqsN;
reg				tDdrDqsN_En;

reg[15:0]		tDdrDataInN;
reg[1:0]		tDdrDqsInN;
reg[15:0]		tDdrDataInN1;
reg[1:0]		tDdrDqsInN1;

reg[15:0]		tDdrDataInP2;
reg[1:0]		tDdrDqsInP2;
reg[15:0]		tDdrDataInP3;
reg[1:0]		tDdrDqsInP3;

reg[15:0]		tDdrDataInN2;
reg[1:0]		tDdrDqsInN2;
reg[15:0]		tDdrDataInN3;
reg[1:0]		tDdrDqsInN3;


/* verilator lint_off MULTIDRIVEN */

/* Outputs, Temp (Pos/Neg) */
reg[13:0]		tDdrAddrT;		//Address pins
reg[2:0]		tDdrBaT;			//Bank Address pins
reg				tDdrCsT;
reg				tDdrRasT;
reg				tDdrCasT;
reg				tDdrWeT;
reg				tDdrCkeT;

assign		ddrAddr	= tDdrAddrT;
assign		ddrBa	= tDdrBaT;
assign		ddrCs	= tDdrCsT;
assign		ddrRas	= tDdrRasT;
assign		ddrCas	= tDdrCasT;
assign		ddrWe	= tDdrWeT;
assign		ddrCke	= tDdrCkeT;

reg[1:0]		tDdrClkT;			//clock pins
assign			ddrClk = tDdrClkT;

reg[1:0]		tDdrDqsT;
reg				tDdrDqsT_En;
assign			ddrDqsP_O	= tDdrDqsT[0] ? 2'b11 : 2'b00;
assign			ddrDqsN_O	= tDdrDqsT[1] ? 2'b11 : 2'b00;
assign			ddrDqs_En	= tDdrDqsT_En;

reg[15:0]		tDdrDataT;
reg				tDdrOutT;

wire[15:0]		ddrData;
assign			ddrData_O = tDdrDataT;
assign			ddrData = ddrData_I;
assign			ddrData_En = tDdrOutT;

`ifndef def_true

always @(posedge ddr_coreclock)
begin
	tDdrAddrT		<= tDdrAddrP;
	tDdrBaT			<= tDdrBaP;
	tDdrCsT			<= tDdrCsP;
	tDdrRasT		<= tDdrRasP;
	tDdrCasT		<= tDdrCasP;
	tDdrWeT			<= tDdrWeP;
	tDdrCkeT		<= tDdrCkeP;
	tDdrDataT		<= tDdrDataOutP;
	tDdrOutT		<= tDdrOutP;
	tDdrDqsT		<= tDdrDqsP;
	tDdrDqsT_En		<= tDdrDqs_En;
	tDdrClkT		<= 2'b01;

	tDdrDataInP		<= ddrData;
	tDdrDqsInP		<= ddrDqs;
	tDdrDataInP1	<= tDdrDataInN;
	tDdrDqsInP1		<= tDdrDqsInN;
end

always @(negedge ddr_coreclock)
begin
	tDdrAddrT		<= tDdrAddrN;
	tDdrBaT			<= tDdrBaN;
	tDdrCsT			<= tDdrCsN;
	tDdrRasT		<= tDdrRasN;
	tDdrCasT		<= tDdrCasN;
	tDdrWeT			<= tDdrWeN;
	tDdrCkeT		<= tDdrCkeN;
	tDdrDataT		<= tDdrDataOutN;
	tDdrOutT		<= tDdrOutN;
	tDdrDqsT		<= tDdrDqsN;
	tDdrDqsT_En		<= tDdrDqs_En;
	tDdrClkT		<= 2'b10;
	tDdrDataInN		<= ddrData;
	tDdrDqsInN		<= ddrDqs;
	tDdrDataInN1	<= tDdrDataInP;
	tDdrDqsInN1		<= tDdrDqsInP;
end
`endif

reg	tAccCkEdge1;
reg	tAccCkEdge1L;

`ifndef def_true
reg	tAccCkEdge;

always @(posedge ddr_coreclock2)
// always @(negedge ddr_coreclock2)
begin
//	tAccCkEdge		<= !tAccCkEdge;
	tAccCkEdge1L	<= tAccCkEdge1;
	if(tAccCkEdge1L != tAccCkEdge1)
		tAccCkEdge		<= 0;
//		tAccCkEdge		<= 1;
	else
		tAccCkEdge		<= !tAccCkEdge;

	tDdrAddrT		<= tDdrAddrP;
	tDdrBaT			<= tDdrBaP;
	tDdrCsT			<= tDdrCsP;
	tDdrRasT		<= tDdrRasP;
	tDdrCasT		<= tDdrCasP;
	tDdrWeT			<= tDdrWeP;
	tDdrCkeT		<= tDdrCkeP;

//	tDdrAddrT		<= tAccCkEdge ? tDdrAddrN		: tDdrAddrP;
//	tDdrBaT			<= tAccCkEdge ? tDdrBaN			: tDdrBaP;
//	tDdrCsT			<= tAccCkEdge ? tDdrCsN			: tDdrCsP;
//	tDdrRasT		<= tAccCkEdge ? tDdrRasN		: tDdrRasP;
//	tDdrCasT		<= tAccCkEdge ? tDdrCasN		: tDdrCasP;
//	tDdrWeT			<= tAccCkEdge ? tDdrWeN			: tDdrWeP;
//	tDdrCkeT		<= tAccCkEdge ? tDdrCkeN		: tDdrCkeP;
//	tDdrDataT		<= tAccCkEdge ? tDdrDataOutN	: tDdrDataOutP;
//	tDdrOutT		<= tAccCkEdge ? tDdrOutN		: tDdrOutP;
//	tDdrDqsT		<= tAccCkEdge ? tDdrDqsN		: tDdrDqsP;
//	tDdrClkT		<= tAccCkEdge ? 2'b10			: 2'b01;

	tDdrDqsT_En		<= tDdrDqs_En;

	if(!tAccCkEdge)
	begin
		$display("Rise: tDdrDqsP=%X Clk=%X DqsEn=%X",
			tDdrDqsP, 2'b01, tDdrDqs_En);

		tDdrDataT		<= tDdrDataOutP;
		tDdrOutT		<= tDdrOutP;
		tDdrDqsT		<= tDdrDqsP;
		tDdrClkT		<= 2'b01;

		tDdrDataInN		<= ddrData;
		tDdrDqsInN		<= ddrDqs;
		tDdrDataInN1	<= tDdrDataInP;
		tDdrDqsInN1		<= tDdrDqsInP;
	end
	else
	begin
		$display("Fall: tDdrDqsP=%X Clk=%X DqsEn=%X",
			tDdrDqsN, 2'b10, tDdrDqs_En);

		tDdrDataT		<= tDdrDataOutN;
		tDdrOutT		<= tDdrOutN;
		tDdrDqsT		<= tDdrDqsN;
		tDdrClkT		<= 2'b10;

		tDdrDataInP		<= ddrData;
		tDdrDqsInP		<= ddrDqs;
		tDdrDataInP1	<= tDdrDataInN;
		tDdrDqsInP1		<= tDdrDqsInN;
	end
end
`endif

`ifdef def_true
reg	tAccCkEdge;

always @(posedge ddr_coreclock2)
// always @(negedge ddr_coreclock2)
// always @(posedge ddr_coreclock or negedge ddr_coreclock)
begin
//	tAccCkEdge		<= !tAccCkEdge;
	tAccCkEdge1L	<= tAccCkEdge1;
	if(tAccCkEdge1L != tAccCkEdge1)
//		tAccCkEdge		<= 0;
		tAccCkEdge		<= 1;
	else
		tAccCkEdge		<= !tAccCkEdge;

	tDdrAddrT		<= tDdrLastAddrP;
	tDdrBaT			<= tDdrLastBaP;
	tDdrCsT			<= tDdrLastCsP;
	tDdrRasT		<= tDdrLastRasP;
	tDdrCasT		<= tDdrLastCasP;
	tDdrWeT			<= tDdrLastWeP;
	tDdrCkeT		<= tDdrLastCkeP;

	tDdrDqsT_En		<= tDdrLastDqs_En;

	if(!tAccCkEdge)
	begin
//		$display("Rise: DqsP=%X Clk=%X DqsEn=%X",
//			tDdrDqsP, 2'b01, tDdrDqs_En);

		tDdrDataT		<= tDdrLastDataOutP;
		tDdrOutT		<= tDdrLastOutP;
		tDdrDqsT		<= tDdrLastDqsP;
		tDdrClkT		<= 2'b01;

		tDdrDataInN		<= ddrData;
		tDdrDqsInN		<= ddrDqs;
		tDdrDataInN1	<= tDdrDataInP;
		tDdrDqsInN1		<= tDdrDqsInP;
	end
	else
	begin
//		$display("Fall: DqsN=%X Clk=%X DqsEn=%X",
//			tDdrDqsN, 2'b10, tDdrDqs_En);

		tDdrDataT		<= tDdrLastDataOutN;
		tDdrOutT		<= tDdrLastOutN;
		tDdrDqsT		<= tDdrLastDqsN;
		tDdrClkT		<= 2'b10;

		tDdrDataInP		<= ddrData;
		tDdrDqsInP		<= ddrDqs;
		tDdrDataInP1	<= tDdrDataInN;
		tDdrDqsInP1		<= tDdrDqsInN;
	end
end
`endif

// always @(posedge ddr_coreclock)
always @(negedge ddr_coreclock)
begin
	tDdrDataInP2	<= tDdrDataInP;
	tDdrDqsInP2		<= tDdrDqsInP;
	tDdrDataInN2	<= tDdrDataInN;
	tDdrDqsInN2		<= tDdrDqsInN;

	tDdrDataInP3	<= tDdrDataInP1;
	tDdrDqsInP3		<= tDdrDqsInP1;
	tDdrDataInN3	<= tDdrDataInN1;
	tDdrDqsInN3		<= tDdrDqsInN1;
end

/* verilator lint_on MULTIDRIVEN */


reg[127:0]		tMemDataOut;
reg[127:0]		tMemDataOutL;
reg[1:0]		tMemOK;
reg[1:0]		tMemOKL;

`ifdef jx2_mem_ddr32B
`reg_ddrtile	tMemDataInA;
`reg_ddrtile	tMemDataOutA;
`reg_ddrtile	tNxtMemDataOutA;
reg[1:0]		tMemOKA;
reg[1:0]		tNxtMemOKA;
reg[2:0]		tMemStateA;
reg[2:0]		tNxtMemStateA;
reg[31:0]		tMemAddrA;
reg[4:0]		tMemOpmA;

reg[127:0]		tNxtMemDataIn;
reg[31:0]		tNxtMemAddr;
reg[4:0]		tNxtMemOpm;
`endif

`reg_ddrtile	tMemDataOut2;
reg[1:0]		tMemOK2;

`ifdef mod_ddr_extrabuf
`reg_ddrtile	tMemDataOut3;
reg[1:0]		tMemOK3;
`reg_ddrtile	tMemDataOut4;
reg[1:0]		tMemOK4;
`reg_ddrtile	tMemDataOut5;
reg[1:0]		tMemOK5;
`reg_ddrtile	tMemDataOut6;
reg[1:0]		tMemOK6;
assign			memOK		= tMemOK6;
assign			memDataOut	= tMemDataOut6;
`endif

`ifdef mod_ddr_basicbuf
`reg_ddrtile	tMemDataOut3;
reg[1:0]		tMemOK3;
`reg_ddrtile	tMemDataOut4;
reg[1:0]		tMemOK4;
assign			memOK		= tMemOK4;
assign			memDataOut	= tMemDataOut4;
`endif

`ifdef mod_ddr_fastbuf
assign			memOK		= tMemOK2;
assign			memDataOut	= tMemDataOut2;
`endif

reg[127:0]		tMemDataIn;
reg[31:0]		tMemAddr;
reg[4:0]		tMemOpm;


`reg_ddrtile	tMemDataInB;
reg[31:0]		tMemAddrB;
reg[4:0]		tMemOpmB;

`reg_ddrtile	tMemDataInC;
reg[31:0]		tMemAddrC;
reg[4:0]		tMemOpmC;

`reg_ddrtile	tMemDataInD;
reg[31:0]		tMemAddrD;
reg[4:0]		tMemOpmD;

`reg_ddrtile	tMemDataInE;
reg[31:0]		tMemAddrE;
reg[4:0]		tMemOpmE;

`reg_ddrtile	tMemDataInF;
reg[31:0]		tMemAddrF;
reg[4:0]		tMemOpmF;


reg[5:0]		accState;
reg[5:0]		accNextState;

reg[127:0]		accReadBlk;
reg[127:0]		accNextReadBlk;

reg[31:0]		accAddr;
reg[31:0]		accNextAddr;

reg[15:0]		accRowAddr;
reg[15:0]		accColAddr;
reg[ 2:0]		accBaAddr;
reg[15:0]		accNextRowAddr;
reg[15:0]		accNextColAddr;
reg[ 2:0]		accNextBaAddr;

// reg[7:0]		accCkCas;		//Clock-CAS / Wait Time
// reg[7:0]		accNextCkCas;

reg[15:0]		accCkCas;		//Clock-CAS / Wait Time
reg[15:0]		accNextCkCas;

reg				accSkipRowClose;
reg				accNxtSkipRowClose;
reg				accSkipRowOpen;
reg				accNxtSkipRowOpen;


reg[15:0]		dreRowAddr;			//DRAM Refresh, Current Row
reg[15:0]		dreNextRowAddr;		//DRAM Refresh, Next Row
reg[2:0]		dreBaAddr;			//DRAM Refresh, Current Bank
reg[2:0]		dreNextBaAddr;		//DRAM Refresh, Next Bank
reg[15:0]		dreCount;			//DRAM Refresh, Cycle Count
reg[15:0]		dreNextCount;		//DRAM Refresh, Next Cycle Count
reg				dreIsZero;
reg				dreNextIsZero;

// reg[63:0]		driModeOut;			//DRAM Init, Mode Out
// reg[63:0]		driNextModeOut;		//DRAM Init, Next Out
// reg[127:0]		driModeOut;			//DRAM Init, Mode Out
// reg[127:0]		driNextModeOut;		//DRAM Init, Next Out

reg[4:0]		driInitState;
reg[4:0]		driNextInitState;
reg[15:0]		driInitCmd;

reg				driStillInit;
reg				driNextStillInit;

reg				tPhaseHc;
reg				tNxtPhaseHc;

reg				tReset;
reg				tResetB;
reg				tResetC;
reg				tResetD;

wire[15:0]		tDdrData2pP;
wire[15:0]		tDdrData2pN;
// assign		tDdrData2pP = tPhaseHc ? tDdrDataInP1 : tDdrDataInP;
// assign		tDdrData2pN = tPhaseHc ? tDdrDataInN1 : tDdrDataInN;

assign		tDdrData2pP = tPhaseHc ? tDdrDataInP3 : tDdrDataInP2;
assign		tDdrData2pN = tPhaseHc ? tDdrDataInN3 : tDdrDataInN2;


always @*
begin
	accNextState	= accState;

	accNextReadBlk	= accReadBlk;
	tDdrOutP		= 0;
	tDdrOutN		= 0;
	tDdrDataOutP	= 0;
	tDdrDataOutN	= 0;
//	tDdrData		= 16'hzzzz;
	
//	tMemDataOut		= UV128_XX;
	tMemDataOut		= UV128_00;
	tMemOK			= UMEM_OK_READY;

	tDdrDqs_En		= 0;
	tNxtPhaseHc		= tPhaseHc;

	accNxtSkipRowClose	= accSkipRowClose;
	accNxtSkipRowOpen	= accSkipRowOpen;

	tDdrAddrP	= 0;
	tDdrBaP		= 0;
	tDdrCkeP	= 1;
	tDdrCsP		= 0;
	tDdrRasP	= 1;
	tDdrCasP	= 1;
	tDdrWeP		= 1;
	tDdrDqsP	= 2'b01;

	tDdrAddrN	= 0;
	tDdrBaN		= 0;
	tDdrCkeN	= 1;
	tDdrCsN		= 0;
	tDdrRasN	= 1;
	tDdrCasN	= 1;
	tDdrWeN		= 1;
	tDdrDqsN	= 2'b10;

	accNextCkCas	= accCkCas;

	accNextAddr		= accAddr;
	accNextRowAddr	= accRowAddr;
	accNextColAddr	= accColAddr;
	accNextBaAddr	= accBaAddr;

	dreNextRowAddr	= dreRowAddr;
	dreNextBaAddr	= dreBaAddr;
	dreNextCount	= dreCount;
	
	dreNextIsZero	= (dreCount == 0);

	driNextInitState	= driInitState;

	case(driInitState)
		5'h00:		driInitCmd=DDR_DRI_INIT_0;
		5'h01:		driInitCmd=DDR_DRI_INIT_1;
		5'h02:		driInitCmd=DDR_DRI_INIT_2;
		5'h03:		driInitCmd=DDR_DRI_INIT_3;
		5'h04:		driInitCmd=DDR_DRI_INIT_4;
		5'h05:		driInitCmd=DDR_DRI_INIT_5;
		5'h06:		driInitCmd=DDR_DRI_INIT_6;
		5'h07:		driInitCmd=DDR_DRI_INIT_7;
		5'h08:		driInitCmd=DDR_DRI_INIT_8;
		5'h09:		driInitCmd=DDR_DRI_INIT_9;
		5'h0A:		driInitCmd=DDR_DRI_INIT_10;
		5'h0B:		driInitCmd=DDR_DRI_INIT_11;
		5'h0C:		driInitCmd=DDR_DRI_INIT_12;
		5'h0D:		driInitCmd=DDR_DRI_INIT_13;
		default:	driInitCmd=0;
	endcase

	driNextStillInit	= (driInitCmd != 0);

	if(!dreNextIsZero)
	begin
		dreNextCount = dreCount - 1;
	end

	if(driStillInit)
	begin
//		$display("DRI Mode %X", driModeOut);
	end

	/* If doing something Load/Store requests can wait. */
	if(accState!=0)
	begin
		if(tMemOpm[4:3]!=0)
			tMemOK		= UMEM_OK_HOLD;
	end

//	$display("ModDdr: State %X", accState);

	if(accState!=0)
	begin
//		$display("ModDdr: State %X", accState);
	end

	case(accState)
	6'b000000: begin	/* Waiting for request */
//		$display("ModDdr: State 0");

		accNextState = 6'b000000;

		if(tMemOpm[4:3]==2'b11)	/* Swap Request */
		begin
			tMemDataOut		= UV128_00;
			tMemOK			= UMEM_OK_FAULT;
		end
		else
			if(tMemOpm[3] && !driStillInit && !dreIsZero)	/* Load Request */
		begin
//			$display("ModDdr: Read Req, A=%X", tMemAddr);

			tMemOK		= UMEM_OK_HOLD;

			accNextState = 6'b010000;
			if(accSkipRowOpen)
				accNextState = 6'b010100;
			
			accNextAddr	= {4'h0, tMemAddr[27:4], 4'h0};

			accNextRowAddr = accNextAddr[28:13];
			accNextBaAddr = accNextAddr[12:10];
			accNextColAddr = {6'b0, accNextAddr[ 9:0]};

		end
		else
			if(tMemOpm[4] && !driStillInit && !dreIsZero)	/* Store Request */
		begin
//			$display("ModDdr: Store Req, A=%X", tMemAddr);

			accNextReadBlk	= tMemDataIn;
			tMemOK			= UMEM_OK_HOLD;

			if(tMemOpm==UMEM_OPM_WR_SW)
			begin
				accNextState = 6'b000010;
				accNextRowAddr = tMemDataIn[15:0];
			end
			else
			begin
				accNextState = 6'b011000;
				if(accSkipRowOpen)
					accNextState = 6'b011100;

				accNextAddr	= {4'h0, tMemAddr[27:4], 4'h0};
				accNextRowAddr = accNextAddr[28:13];
				accNextBaAddr = accNextAddr[12:10];
				accNextColAddr = {6'b0, accNextAddr[ 9:0]};
				accNextReadBlk = tMemDataIn;
			end
		end
		else
		begin
			if(driStillInit)
			begin
				accNextState	= 6'b000010;
				accNextRowAddr		= driInitCmd;
				driNextInitState	= driInitState + 1;
			end
			else
			if(dreIsZero)
			begin
				accNextState	= 6'b001000;

				accNextRowAddr	= dreRowAddr;
				accNextBaAddr	= dreBaAddr;
				accNextColAddr	= 0;

				if(dreBaAddr==7)
				begin
					dreNextRowAddr	= { 3'b000, dreRowAddr[12:0] + 13'h1 };
					dreNextBaAddr	= 0;
				end
				else
				begin
					dreNextRowAddr	= dreRowAddr;
					dreNextBaAddr	= dreBaAddr + 1;
				end

				dreNextCount = 4096;
			end
			else
			begin
				tDdrCsP	= 1;
				tDdrCsN	= 1;
			end
		end

	end

	6'b000001: begin	/* Access Complete, Hold */
		tDdrCsP	= 1;
		tDdrCsN	= 1;
		if(tMemOpm[4:3] != 0)
		begin
//			$display("DDR ReadBlk %X", accReadBlk);
			/* Hang out here until released. */
			tMemDataOut		= accReadBlk;
			tMemOK			= UMEM_OK_OK;
			accNextState	= 6'b000001;
		end
		else
		begin
			/* Return to waiting state. */
			accNextState	= 6'b000000;
		end
	end
	

	6'b000010: begin
		/* Mode Register Write */

		tDdrAddrP		= {1'b0, accRowAddr[12: 0] };
		tDdrBaP			= {1'b0, accRowAddr[14:13] };
		tDdrCkeP		= 1;
		tDdrCsP			= 0;	tDdrRasP	= 0;
		tDdrCasP		= 0;	tDdrWeP	= 0;

		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
		accNextState	= 6'b001110;

		if(accRowAddr[15:12]==4'h8)
		begin
			case(accRowAddr[11:8])
				4'h0: begin
					tDdrCsP		= 0;	tDdrRasP	= 1;
					tDdrCasP	= 1;	tDdrWeP		= 1;
					accNextCkCas	= { 8'h0, accRowAddr[7:0]       };
				end
				4'h1: begin
					tDdrCsP		= 0;	tDdrRasP	= 1;
					tDdrCasP	= 1;	tDdrWeP		= 1;
					accNextCkCas	= { 4'h0, accRowAddr[7:0], 4'h0 };
				end
				4'h2: begin
					tDdrCsP		= 0;	tDdrRasP	= 1;
					tDdrCasP	= 1;	tDdrWeP		= 1;
				 	accNextCkCas	= {       accRowAddr[7:0], 8'h0 };
				end

				4'h6: begin
					/* NOP Delay with CKE Disabled */
					tDdrCkeP	= 0;
					tDdrCsP		= 0;	tDdrRasP	= 1;
					tDdrCasP	= 1;	tDdrWeP		= 1;
					accNextCkCas	= {       accRowAddr[7:0], 8'h0 };
				end

				4'h8: begin		/* PRECHARGE */
					tDdrCsP		= 0;	tDdrRasP	= 0;
					tDdrCasP	= 1;	tDdrWeP		= 0;
				end

				4'h9: begin		/* REFRESH */
					tDdrCsP		= 0;	tDdrRasP	= 0;
					tDdrCasP	= 0;	tDdrWeP		= 1;
				end

				default: begin	/* NOP Delay */
					tDdrCsP		= 0;	tDdrRasP	= 1;
					tDdrCasP	= 1;	tDdrWeP		= 1;
				end
			endcase
		end

		tDdrAddrN	= tDdrAddrP;
		tDdrBaN		= tDdrBaP;
		tDdrCkeN	= tDdrCkeP;
		tDdrCsN		= tDdrCsP;
		tDdrRasN	= tDdrRasP;
		tDdrCasN	= tDdrCasP;
		tDdrWeN		= tDdrWeP;
	end

	/* Precharge Start */
	
	6'b001000: begin
		/* Row Activate */

		tDdrAddrP	= accRowAddr[13:0];
		tDdrBaP		= accBaAddr[2:0];
		tDdrCkeP	= 1;
		tDdrCsP		= 0;	tDdrRasP	= 0;
		tDdrCasP	= 1;	tDdrWeP		= 1;

		tDdrAddrN	= tDdrAddrP;
		tDdrBaN		= tDdrBaP;
		tDdrCkeN	= tDdrCkeP;
		tDdrCsN		= tDdrCsP;
		tDdrRasN	= tDdrRasP;
		tDdrCasN	= tDdrCasP;
		tDdrWeN		= tDdrWeP;

		accNextState	= 6'b001010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b001010: begin
		/* RAS Hold */
		accNextState	= 6'b001100;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b001010;
		end
	end

	6'b001100: begin
		/* Precharge */
		
		tDdrAddrP		= accColAddr[13:0];
		tDdrAddrP[10]	= 1;
		tDdrBaP			= accBaAddr[2:0];
		tDdrCkeP		= 1;
		tDdrCsP			= 0;	tDdrRasP	= 0;
		tDdrCasP		= 1;	tDdrWeP	= 0;

		tDdrAddrN	= tDdrAddrP;
		tDdrBaN		= tDdrBaP;
		tDdrCkeN	= tDdrCkeP;
		tDdrCsN		= tDdrCsP;
		tDdrRasN	= tDdrRasP;
		tDdrCasN	= tDdrCasP;
		tDdrWeN		= tDdrWeP;

		accNextState	= 6'b001110;
		accNextCkCas	= DDR_CAS_RL_M1;	/* CAS Latency (-1) */
	end

	6'b001110: begin
		/* CAS Hold */
		accNextState	= 6'b000000;
		tDdrCkeP		= tDdrLastCkeP;
		tDdrCkeN		= tDdrLastCkeP;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b001110;
		end
	end

	/* Read Start */
	
	6'b010000: begin
		/* Row Activate */

		tDdrAddrP	= accRowAddr[13:0];
		tDdrBaP		= accBaAddr[2:0];
		tDdrCkeP	= 1;
		tDdrCsP		= 0;	tDdrRasP	= 0;
		tDdrCasP	= 1;	tDdrWeP	= 1;

		tDdrAddrN	= tDdrAddrP;
		tDdrBaN		= tDdrBaP;
		tDdrCkeN	= tDdrCkeP;
		tDdrCsN		= tDdrCsP;
		tDdrRasN	= tDdrRasP;
		tDdrCasN	= tDdrCasP;
		tDdrWeN		= tDdrWeP;

		accNextState	= 6'b010010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b010010: begin
		/* RAS Hold */
		accNextState	= 6'b010100;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b010010;
		end
	end

	6'b010100: begin
		/* Column Read (Rise) */
		/* BA2 BA1 BA0 C2 C1 r r Hi Lo Hi */

//		$display("DDR Read ColA %X", accColAddr);

		tDdrAddrP		= accColAddr[13:0];
		tDdrAddrP[10]	= 1;
		tDdrAddrP[12]	= 1;
		tDdrBaP			= accBaAddr[2:0];
		tDdrCkeP		= 1;
		tDdrCsP			= 0;	tDdrRasP	= 1;
		tDdrCasP		= 0;	tDdrWeP	= 1;

		if(accSkipRowClose)
			tDdrAddrP[10]	= 0;

		tDdrAddrN	= tDdrAddrP;
		tDdrBaN		= tDdrBaP;
		tDdrCkeN	= tDdrCkeP;
		tDdrCsN		= tDdrCsP;
		tDdrRasN	= tDdrRasP;
		tDdrCasN	= tDdrCasP;
		tDdrWeN		= tDdrWeP;

		accNextState	= 6'b010110;
		accNextCkCas	= DDR_CAS_RL_M1;
	end

	6'b010110: begin
		/* Read CAS Hold */
		accNextState	= 6'b100000;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b010110;
		end
	end

	/* Write Start */
	
	6'b011000: begin
		/* Row Activate */

		tDdrAddrP	= accRowAddr[13:0];
		tDdrBaP		= accBaAddr[2:0];
		tDdrCkeP	= 1;
		tDdrCsP		= 0;	tDdrRasP	= 0;
		tDdrCasP	= 1;	tDdrWeP	= 1;

		tDdrAddrN	= tDdrAddrP;
		tDdrBaN		= tDdrBaP;
		tDdrCkeN	= tDdrCkeP;
		tDdrCsN		= tDdrCsP;
		tDdrRasN	= tDdrRasP;
		tDdrCasN	= tDdrCasP;
		tDdrWeN		= tDdrWeP;

		accNextState	= 6'b011010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b011010: begin
		/* RAS Hold */
		accNextState	= 6'b011100;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b011010;
		end
	end

	6'b011100: begin
		/* Column Write */

//		$display("Col Write, Data=%X", accReadBlk);

		tDdrAddrP		= accColAddr[13:0];
		tDdrAddrP[10]	= 1;
		tDdrAddrP[12]	= 1;
		tDdrBaP			= accBaAddr[2:0];
		tDdrCkeP		= 1;
		tDdrCsP			= 0;	tDdrRasP	= 1;
		tDdrCasP		= 0;	tDdrWeP		= 0;

		if(accSkipRowClose)
			tDdrAddrP[10]	= 0;

		tDdrAddrN	= tDdrAddrP;
		tDdrBaN		= tDdrBaP;
		tDdrCkeN	= tDdrCkeP;
		tDdrCsN		= tDdrCsP;
		tDdrRasN	= tDdrRasP;
		tDdrCasN	= tDdrCasP;
		tDdrWeN		= tDdrWeP;

		accNextState	= 6'b011110;
		accNextCkCas	= DDR_CAS_WL_M1;	/* CAS Latency (-1) */
	end

	6'b011110: begin
		/* Write CAS Hold */
		tDdrDqsP		= 2'b10;
		tDdrDqsN		= 2'b10;
		tDdrDqs_En		= 1;
		accNextState	= 6'b110000;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b011110;
		end
	end
	
	/* Read Burst States */
	
	6'b100000: begin
		accNextState			= 6'b100010;
		
		/* Attempt to align with DQS */
//		if(tDdrDqsInP1==2'b01)
		if(tDdrDqsInP3==2'b01)
		begin
//			accNextReadBlk[15:0]	= tDdrDataInP1;
//			accNextReadBlk[31:16]	= tDdrDataInN1;
			accNextReadBlk[15:0]	= tDdrDataInP3;
			accNextReadBlk[31:16]	= tDdrDataInN3;
			tNxtPhaseHc = 1;
		end
		else
//			if(tDdrDqsInP==2'b01)
			if(tDdrDqsInP2==2'b01)
		begin
//			accNextReadBlk[15:0]	= tDdrDataInP;
//			accNextReadBlk[31:16]	= tDdrDataInN;
			accNextReadBlk[15:0]	= tDdrDataInP2;
			accNextReadBlk[31:16]	= tDdrDataInN2;
			tNxtPhaseHc = 0;
		end
		else
		begin
				accNextState			= 6'b100000;
		end
	end
	6'b100010: begin
		accNextState			= 6'b100100;
		accNextReadBlk[47:32]	= tDdrData2pP;
		accNextReadBlk[63:48]	= tDdrData2pN;
	end
	6'b100100: begin
		accNextState			= 6'b100110;
		accNextReadBlk[79:64]	= tDdrData2pP;
		accNextReadBlk[95:80]	= tDdrData2pN;
	end
	6'b100110: begin
		accNextState			= 6'b000001;
		accNextReadBlk[111:96]	= tDdrData2pP;
		accNextReadBlk[127:112]	= tDdrData2pN;
	end


	/* Write Burst States */
	
	6'b110000: begin
		accNextState			= 6'b110010;
		tDdrDataOutP 			= accReadBlk[15:0];
		tDdrDataOutN 			= accReadBlk[31:16];
		tDdrOutP				= 1;
		tDdrOutN				= 1;
		tDdrDqs_En				= 1;
	end
	6'b110010: begin
		accNextState			= 6'b110100;
		tDdrDataOutP 			= accReadBlk[47:32];
		tDdrDataOutN 			= accReadBlk[63:48];
		tDdrOutP				= 1;
		tDdrOutN				= 1;
		tDdrDqs_En				= 1;
	end
	6'b110100: begin
		accNextState			= 6'b110110;
		tDdrDataOutP 			= accReadBlk[79:64];
		tDdrDataOutN 			= accReadBlk[95:80];
		tDdrOutP				= 1;
		tDdrOutN				= 1;
		tDdrDqs_En				= 1;
	end
	6'b110110: begin
		accNextState			= 6'b000001;
		tDdrDataOutP 			= accReadBlk[111:96];
		tDdrDataOutN 			= accReadBlk[127:112];
		tDdrOutP				= 1;
		tDdrOutN				= 1;
		tDdrDqs_En				= 1;
	end

	default: begin
		accNextState			= 6'b000000;
	end

	endcase
	
	
`ifdef jx2_mem_ddr32B
	/*
	 * Deal with 256 bit cache-lines via another state machine.
	 * Breaks the request into several sub-requests.
	 */

	tNxtMemDataOutA	= tMemDataOutA;
	tNxtMemOKA		= tMemOKA;
	tNxtMemStateA	= tMemStateA;

//	if(tMemOpmA[4:3]!=0)
//		tMemOK		= UMEM_OK_HOLD;

//	if(tMemOK == UMEM_OK_HOLD)
//		tNxtMemOKA = UMEM_OK_HOLD;

	tNxtMemDataIn	= tMemDataIn;
	tNxtMemAddr		= tMemAddr;
	tNxtMemOpm		= tMemOpm;

`ifndef def_true
	if(tMemOpmA!=0)
	begin
		$display("DDR: State=%X OPMA=%X OKA=%X",
			tNxtMemStateA, tMemOpmA, tMemOKA);
		$display("DDR: OPMi=%X OKi=%X",
			tMemOpm, tMemOK);
	end
`endif

	case(tMemOKL)
		UMEM_OK_READY: begin
			case(tMemStateA)
				3'b000: begin
					tNxtMemOKA = UMEM_OK_READY;
					if(tMemOpmA[4:3]==2'b11)
					begin
						tNxtMemStateA	= 3'b101;
					end
					else if(tMemOpmA[4:3]!=2'b00)
					begin
						tNxtMemStateA	= 3'b001;

//						tNxtMemDataIn	= tMemDataInA[127:0];
//						tNxtMemOpm		= tMemOpmA;
//						tNxtMemAddr		= tMemAddrA;
					end
				end
				
				3'b001: begin
//					accNxtSkipRowClose	= 1;
					accNxtSkipRowOpen	= 0;
					tNxtMemDataIn	= tMemDataInA[127:0];
					tNxtMemOpm		= tMemOpmA;
					tNxtMemAddr		= tMemAddrA;
					tNxtMemOKA		= UMEM_OK_HOLD;
				end

				3'b010, 3'b011: begin
					accNxtSkipRowClose	= 0;
//					accNxtSkipRowOpen	= 1;
					tNxtMemDataIn	= tMemDataInA[255:128];
					tNxtMemOpm		= tMemOpmA;
					tNxtMemAddr		= tMemAddrA;
					tNxtMemAddr[4]	= 1;
					tNxtMemOKA		= UMEM_OK_HOLD;
					tNxtMemStateA	= 3'b011;
				end

				3'b100: begin
					accNxtSkipRowClose	= 0;
					accNxtSkipRowOpen	= 0;

					tNxtMemOpm		= UMEM_OPM_READY;
					tNxtMemOKA		= UMEM_OK_OK;
//					if(tMemOpmA == UMEM_OPM_READY)
					if(tMemOpmA[4:3]==2'b00)
						tNxtMemStateA	= 3'b000;
				end
				3'b101: begin
					accNxtSkipRowClose	= 0;
					accNxtSkipRowOpen	= 0;

					tNxtMemOpm		= UMEM_OPM_READY;
					tNxtMemOKA		= UMEM_OK_FAULT;
//					if(tMemOpmA == UMEM_OPM_READY)
					if(tMemOpmA[4:3]==2'b00)
						tNxtMemStateA	= 3'b000;
				end
				
				default: begin
					tNxtMemOpm		= UMEM_OPM_READY;
					tNxtMemOKA		= UMEM_OK_FAULT;
//					if(tMemOpmA == UMEM_OPM_READY)
					if(tMemOpmA[4:3]==2'b00)
						tNxtMemStateA	= 3'b000;
				end
			
			endcase
			
		end
		UMEM_OK_OK: begin
			case(tMemStateA)
				3'b000: begin
					/* Wait for READY */
				end
				3'b010: begin
					/* Wait for READY */
				end
				3'b100: begin
					/* Wait for READY */
				end

				3'b001: begin
					tNxtMemDataOutA[127:0]		= tMemDataOutL;
					tNxtMemStateA				= 3'b010;
				end
				3'b011: begin
					tNxtMemDataOutA[255:128]	= tMemDataOutL;
					tNxtMemStateA				= 3'b100;
				end
				
				default: begin
					/* Wait for READY */
				end
			endcase
			
			tNxtMemOpm = UMEM_OPM_READY;
		end
		UMEM_OK_HOLD: begin
			/* Wait for state to update. */
		end
		UMEM_OK_FAULT: begin
			/* Faulted, send back result. */
			tNxtMemDataOutA[127:0]		= tMemDataOutL;
			tNxtMemStateA				= 3'b101;
		end
	endcase
	
	if(reset)
	begin
		accNxtSkipRowClose	= 0;
		accNxtSkipRowOpen	= 0;
		tNxtMemOpm = 0;
	end
	
`endif
	
	if(tReset)
	begin
		accNextState		= 0;
		driNextInitState	= 0;
		tNxtMemOpm = 0;
		tNxtMemStateA	= 0;

//		tNxtMemDataOutA[255:0]		= UV256_00;
		tNxtMemDataOutA[15: 0]		= 16'h1234;			/* Magic */
		tNxtMemOKA					= UMEM_OK_READY;
	end
end

reg[7:0]	regInitSanity;

reg			tCheckD;
reg			tCheckOK4;


initial begin
	regInitSanity = 0;
end

`ifndef def_true
// always @(negedge clock)
// always @(negedge clock2)
always @(negedge ddr_coreclock)
begin
	tDdrClk3		<= tDdrClk2;
	tDdrDqs3		<= tDdrDqs2;
	tDdrDqs3_En		<= tDdrDqs2_En;
//	ddrData2		<= ddrData;
//	ddrData1H		<= ddrData;
//	ddrDqs1H		<= ddrDqs;
end
`endif

`ifdef mod_ddr_extrabuf
always @(posedge clock)
begin
	tMemOK3			<= tMemOK2;
	tMemDataOut3	<= tMemDataOut2;

	tMemOK4			<= tMemOK3;
	tMemDataOut4	<= tMemDataOut3;

	tMemOK5			<= tMemOK4;
	tMemDataOut5	<= tMemDataOut4;

	tCheckOK4		<= (tMemOK5 == tMemOK4);

	tMemOK6			<= tMemOK5;
	tMemDataOut6	<= tMemDataOut5;

	tMemDataInB		<= memDataIn;
	tMemAddrB		<= memAddr;
	tMemOpmB		<= memOpm;

//	tResetB			<= reset;
end
`endif


`ifdef mod_ddr_basicbuf
always @(posedge clock)
begin
	tMemOK3			<= tMemOK2;
	tMemDataOut3	<= tMemDataOut2;

	tMemOK4			<= tMemOK3;
	tMemDataOut4	<= tMemDataOut3;

	tMemDataInB		<= memDataIn;
	tMemAddrB		<= memAddr;
	tMemOpmB		<= memOpm;
end
`endif


// always @(posedge clock)
// always @(posedge clock2)
always @(posedge ddr_coreclock)
begin
`ifdef jx2_mem_ddr32B
	tMemOK2			<= tMemOKA;
	tMemDataOut2	<= tMemDataOutA;
`else
	tMemOK2			<= tMemOK;
	tMemDataOut2	<= tMemDataOut;
`endif

`ifdef mod_ddr_extrabuf
//	tResetC			<= tResetB;
	tResetC			<= ddr_corereset;
	tResetD			<= tResetC;
	tReset			<= tResetD;

	tMemDataInC		<= tMemDataInB;
	tMemAddrC		<= tMemAddrB;
	tMemOpmC		<= tMemOpmB;

	tMemDataInD		<= tMemDataInC;
	tMemAddrD		<= tMemAddrC;
	tMemOpmD		<= tMemOpmC;

	tMemDataInE		<= tMemDataInD;
	tMemAddrE		<= tMemAddrD;
	tMemOpmE		<= tMemOpmD;

//	tCheckD			<= (tMemOpmE == tMemOpmD);

`ifdef jx2_mem_ddr32B
	tMemDataInA		<= tMemDataInE;
	tMemAddrA		<= tMemAddrE;
	tMemOpmA		<= tMemOpmE;
`else
	tMemDataIn		<= tMemDataInE;
	tMemAddr		<= tMemAddrE;
	tMemOpm			<= tMemOpmE;
`endif

`endif

`ifdef mod_ddr_basicbuf
	tResetC			<= ddr_corereset;
	tResetD			<= tResetC;
	tReset			<= tResetD;

	tMemDataInC		<= tMemDataInB;
	tMemAddrC		<= tMemAddrB;
	tMemOpmC		<= tMemOpmB;

`ifdef jx2_mem_ddr32B
	tMemDataInA		<= tMemDataInC;
	tMemAddrA		<= tMemAddrC;
	tMemOpmA		<= tMemOpmC;
`else
	tMemDataIn		<= tMemDataInC;
	tMemAddr		<= tMemAddrC;
	tMemOpm			<= tMemOpmC;
`endif

`endif

`ifdef mod_ddr_fastbuf

`ifdef jx2_mem_ddr32B
	tMemDataInA		<= memDataIn;
	tMemAddrA		<= memAddr;
	tMemOpmA		<= memOpm;
`else
	tMemDataIn		<= memDataIn;
	tMemAddr		<= memAddr;
	tMemOpm			<= memOpm;
`endif

//	tReset			<= reset;
	tReset			<= ddr_corereset;
`endif

//	ddrData2		<= ddrData;
//	ddrDqs2			<= ddrDqs;
//	ddrData2H		<= ddrData1H;
//	ddrDqs2H		<= ddrDqs1H;
	tPhaseHc		<= tNxtPhaseHc;

	accSkipRowClose	<= accNxtSkipRowClose;
	accSkipRowOpen	<= accNxtSkipRowOpen;

`ifdef jx2_mem_ddr32B
	tMemDataOutL	<= tMemDataOut;
	tMemOKL			<= tMemOK;

	tMemDataOutA	<= tNxtMemDataOutA;
	tMemOKA			<= tNxtMemOKA;
	tMemStateA		<= tNxtMemStateA;

	tMemDataIn		<= tNxtMemDataIn;
	tMemAddr		<= tNxtMemAddr;
	tMemOpm			<= tNxtMemOpm;
`endif

//	ddrData2A		<= ddrData;
//	ddrData2		<= ddrData2A;

	accState		<= accNextState;
	accReadBlk		<= accNextReadBlk;
	accCkCas		<= accNextCkCas;

	accAddr			<= accNextAddr;
	accRowAddr		<= accNextRowAddr;
	accColAddr		<= accNextColAddr;
	accBaAddr		<= accNextBaAddr;

	dreRowAddr		<= dreNextRowAddr;
	dreBaAddr		<= dreNextBaAddr;
	dreCount		<= dreNextCount;

	driStillInit	<= driNextStillInit;
	dreIsZero		<= dreNextIsZero;

//	tDdrClk2		<= tDdrClk;
//	tDdrDqs2		<= tDdrDqs;
//	tDdrDqs2_En		<= tDdrDqs_En;
	tAccCkEdge1		<= !tAccCkEdge1;

	tDdrLastDqs_En		<= tDdrDqs_En;

	tDdrLastAddrP		<= tDdrAddrP;	//Address pins
	tDdrLastBaP			<= tDdrBaP;		//Bank Address pins
	tDdrLastCsP			<= tDdrCsP;
	tDdrLastRasP		<= tDdrRasP;
	tDdrLastCasP		<= tDdrCasP;
	tDdrLastWeP			<= tDdrWeP;
	tDdrLastCkeP		<= tDdrCkeP;
	tDdrLastDataOutP	<= tDdrDataOutP;
	tDdrLastOutP		<= tDdrOutP;
	tDdrLastDqsP		<= tDdrDqsP;

	tDdrLastAddrN		<= tDdrAddrN;	//Address pins
	tDdrLastBaN			<= tDdrBaN;		//Bank Address pins
	tDdrLastCsN			<= tDdrCsN;
	tDdrLastRasN		<= tDdrRasN;
	tDdrLastCasN		<= tDdrCasN;
	tDdrLastWeN			<= tDdrWeN;
	tDdrLastCkeN		<= tDdrCkeN;
	tDdrLastDataOutN	<= tDdrDataOutN;
	tDdrLastOutN		<= tDdrOutN;
	tDdrLastDqsN		<= tDdrDqsN;

	if(tReset || (regInitSanity!=8'h55))
	begin
//		driModeOut	<= DDR_DRI_INIT;
		driInitState	<= 0;
		accState <= 0;
	end
	else
	begin
//		driModeOut	<= driNextModeOut;
		driInitState	<= driNextInitState;
	end

	if(tReset)
	begin
		regInitSanity	<= 0;
	end
	else
	begin
		regInitSanity	<= 8'h55;
	end

end

endmodule
