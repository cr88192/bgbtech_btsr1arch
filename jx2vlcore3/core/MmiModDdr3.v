/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
DDR2 / DDR3 Interface Module

State Machine, Transmit/Recieve Words

Assumes a 16-bit wide DDR interface, with a nominal burst length of 8.
This will move 128 bits to or from DRAM.


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
	00-0010		//Mode Register Write (Rise)
	00-0011		//Mode Register Write (Fall)

	00-1zzz		//Precharge Row

	00-1000		//Precharge, Row Activate Rise
	00-1001		//Precharge, Row Activate Fall
	00-1010		//Precharge, RAS Hold Rise
	00-1011		//Precharge, RAS Hold Fall
	00-1100		//Precharge, Precharge Rise
	00-1101		//Precharge, Precharge Fall
	00-1110		//Precharge, Wait Spin (Rise), Returns Idle
	00-1111		//Precharge, Wait Spin (Fall), Returns Idle

	01-0000		//Read, Row Activate Rise
	01-0001		//Read, Row Activate Fall
	01-0010		//Read, RAS Hold Rise
	01-0011		//Read, RAS Hold Fall
	01-0100		//Read, Column Activate Rise
	01-0101		//Read, Column Activate Fall
	01-0110		//Read, CAS/RL Hold Rise
	01-0111		//Read, CAS/RL Hold Fall

	01-1000		//Write, Row Activate Rise
	01-1001		//Write, Row Activate Fall
	01-1010		//Write, RAS Hold Rise
	01-1011		//Write, RAS Hold Fall
	01-1100		//Write, Column Activate Rise
	01-1101		//Write, Column Activate Fall
	01-1110		//Write, CAS/WL Hold Rise
	01-1111		//Write, CAS/WL Hold Fall

	10-zzzz		//Read Burst
	11-zzzz		//Write Burst

Note: ddrData_I/ddrData_O;
These exist because verilator lacks tristate IO.


DRI Init:
	0mma-aaaa-aaaa-aaaa:	Load Mode Register
		mm: MR/EMR to Load
		aa: Bits to Load into MR
	1000-0kss-dddd-dddd:	Delay
		ss: Shift Delay left 0/4/8 bits
		k: Disable CKE

*/

`include "CoreDefs.v"

module MmiModDdr3(
	clock,		clock2,
	reset,		reset2,
	memDataIn,	memDataOut,
	memAddr,	memAddrSw,
	memOpm,
	memOK,
	memOpSqI,	memOpSqO,
	
	ddrData_I,	ddrData_O,	ddrData_En,
	ddrAddr,	ddrBa,
	ddrCs,		ddrRas,		ddrCas,
	ddrWe,		ddrCke,		ddrClk,
	ddrDqsP_I,	ddrDqsN_I,
	ddrDqsP_O,	ddrDqsN_O,	ddrDqs_En,
	ddrModeIn);

input			clock;
input			clock2;
input			reset;
input			reset2;
`input_ddrtile	memDataIn;
`output_ddrtile	memDataOut;
input[31:0]		memAddr;
input[31:0]		memAddrSw;
input[4:0]		memOpm;
output[1:0]		memOK;
	
input[3:0]		memOpSqI;
output[3:0]		memOpSqO;

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

input[3:0]		ddrModeIn;

// parameter[15:0]	DDR_CAS_RL_M1	= 4;	//CAS RL Minus 1
// parameter[15:0]	DDR_CAS_RL_M1	= 3;	//CAS RL Minus 1
parameter[15:0]	DDR_CAS_RL_M1	= 3;	//CAS RL Minus 1
// parameter[15:0]	DDR_CAS_RL_M1	= 2;	//CAS RL Minus 1
// parameter[15:0]	DDR_CAS_WL_M1	= 4;	//CAS WL Minus 1
// parameter[15:0]	DDR_CAS_WL_M1	= 2;	//CAS WL Minus 1
// parameter[15:0]	DDR_CAS_WL_M1	= 1;	//CAS WL Minus 1
parameter[15:0]	DDR_CAS_WL_M1	= 0;	//CAS WL Minus 1
// parameter[15:0]	DDR_RAS_M1		= 8;	//RAS Minus 1
// parameter[15:0]	DDR_RAS_M1		= 4;	//RAS Minus 1
// parameter[15:0]	DDR_RAS_M1		= 3;	//RAS Minus 1 (~ 40ns needed)

`ifdef jx2_cpu_ddrclock_150
parameter[15:0]		DDR_RAS_M1		= 3;	//RAS Minus 1 (~ 40ns needed)
// parameter[15:0]		DDR_RAS_M1		= 4;	//RAS Minus 1 (~ 40ns needed)
`else
// parameter[15:0]		DDR_RAS_M1		= 4;	//RAS Minus 1
// parameter[15:0]		DDR_RAS_M1		= 3;	//RAS Minus 1 (~ 40ns needed)
parameter[15:0]		DDR_RAS_M1		= 2;	//RAS Minus 1 (~ 40ns needed)
`endif

// parameter[15:0]		DDR_TRP_M1		= 4;	//Precharge Time
// parameter[15:0]		DDR_TRP_M1		= 2;	//Precharge Time
parameter[15:0]		DDR_TRP_M1		= 1;	//Precharge Time
// parameter[15:0]		DDR_TRP_M1		= 0;	//Precharge Time

parameter[15:0]	DDR_RAS_INIT	= 128;	//Wait several uS

// parameter[63:0]	DDR_DRI_INIT	= 64'h0000_0122_0201_0AFF;
// parameter[63:0]	DDR_DRI_INIT	= 64'h4000_2000_0010_8200;
// parameter[63:0]	DDR_DRI_INIT	= 64'h4000_2000_0033_8200;
//parameter[127:0]	DDR_DRI_INIT	= 
//	128'h2000_2380_0033_8202_0133_2000_6000_4000_8201_8628;


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
// parameter[15:0]	DDR_DRI_INIT_5	=	16'h2004;	/* Load EMR, DLL Enable */
parameter[15:0]	DDR_DRI_INIT_5	=	16'h2005;	/* Load EMR, DLL Disable */
parameter[15:0]	DDR_DRI_INIT_4	=	16'h6000;	/* Load EMR3 */
parameter[15:0]	DDR_DRI_INIT_3	=	16'h4000;	/* Load EMR2 */
parameter[15:0]	DDR_DRI_INIT_2	=	16'h8800;	/* PRELOAD ALL */
parameter[15:0]	DDR_DRI_INIT_1	=	16'h8201;	/* Delay, NOP, 200+ cyc */
parameter[15:0]	DDR_DRI_INIT_0	=	16'h8628;	/* Delay, CKE=0, 200+ us */

// parameter[127:0]	DDR_DRI_INIT	= 


reg[15:0]		tDdrData;
reg				tDdrOut;
// assign			ddrData = tDdrOut ? tDdrData : 16'hzzzz;
// assign			ddrData = tDdrData;

reg[15:0]		tDdrData2;
reg				tDdrOut2;

reg[15:0]		tDdrData3;
reg				tDdrOut3;

wire[15:0]		ddrData;
assign			ddrData = ddrData_I;
// assign			ddrData_O = tDdrData;
// assign			ddrData_En = tDdrOut;

// assign			ddrData_O = tDdrData2;
// assign			ddrData_En = tDdrOut2;

`ifdef jx2_cpu_ddr_obufcyc
assign			ddrData_O = tDdrData2;
assign			ddrData_En = tDdrOut2;
`else
assign			ddrData_O = tDdrData3;
assign			ddrData_En = tDdrOut3;
`endif

wire[1:0]		ddrDqs;
assign			ddrDqs = { ddrDqsN_I[0], ddrDqsP_I[0] };

wire	ddr_coreclock;
wire	ddr_corereset;

`ifdef mod_ddr_fastcore
assign	ddr_coreclock = clock2;
assign	ddr_corereset = reset2;
`else
assign	ddr_coreclock = clock;
assign	ddr_corereset = reset;
`endif


reg[15:0]		ddrData2;
reg[15:0]		ddrData2A;
reg[15:0]		ddrData1H;
reg[15:0]		ddrData2H;

reg[1:0]		ddrDqs2;
reg[1:0]		ddrDqs1H;
reg[1:0]		ddrDqs2H;

// reg[9:0]		tDdrCmd;			//Command/Address pins
reg[1:0]		tDdrClk;			//clock pins
reg[1:0]		tDdrClk2;			//clock pins
reg[1:0]		tDdrClk3;			//clock pins
reg[1:0]		tDdrClk4;			//clock pins
reg[1:0]		tDdrClk5;			//clock pins
// assign			ddrCmd = tDdrCmd;
// assign			ddrClk = tDdrClk;

// assign			ddrClk = tDdrClk2;
// assign			ddrClk = tDdrClk3;
assign			ddrClk = tDdrClk5;

reg[1:0]	tDdrDqs;
reg			tDdrDqs_En;

reg[1:0]	tDdrDqs2;
reg			tDdrDqs2_En;
reg[1:0]	tDdrDqs3;
reg[1:0]	tDdrDqs3P;
reg[1:0]	tDdrDqs3N;
reg			tDdrDqs3_En;

reg[1:0]	tDdrDqs4;
reg[1:0]	tDdrDqs4P;
reg[1:0]	tDdrDqs4N;
reg			tDdrDqs4_En;

reg[1:0]	tDdrDqs5;
reg[1:0]	tDdrDqs5P;
reg[1:0]	tDdrDqs5N;
reg			tDdrDqs5_En;

wire[1:0]	tDdrDqs2P;
wire[1:0]	tDdrDqs2N;
assign		tDdrDqs2P	= tDdrDqs2[0] ? 2'b11 : 2'b00;
assign		tDdrDqs2N	= tDdrDqs2[1] ? 2'b11 : 2'b00;

// assign		ddrDqsP_O	= tDdrDqs2P;
// assign		ddrDqsN_O	= tDdrDqs2N;
// assign		ddrDqs_En	= tDdrDqs2_En;

// assign		ddrDqsP_O	= tDdrDqs2[0] ? 2'b11 : 2'b00;
// assign		ddrDqsN_O	= tDdrDqs2[1] ? 2'b11 : 2'b00;
// assign		ddrDqs_En	= tDdrDqs2_En;

// assign		ddrDqsP_O	= tDdrDqs3[0] ? 2'b11 : 2'b00;
// assign		ddrDqsN_O	= tDdrDqs3[1] ? 2'b11 : 2'b00;

// assign		ddrDqsP_O	= tDdrDqs3P;
// assign		ddrDqsN_O	= tDdrDqs3N;
// assign		ddrDqs_En	= tDdrDqs3_En;

// assign		ddrDqsP_O	= tDdrDqs4P;
// assign		ddrDqsN_O	= tDdrDqs4N;
// assign		ddrDqs_En	= tDdrDqs4_En;

`ifdef jx2_cpu_ddr_obufcyc
assign		ddrDqsP_O	= tDdrDqs4P;
assign		ddrDqsN_O	= tDdrDqs4N;
assign		ddrDqs_En	= tDdrDqs4_En;
`else
assign		ddrDqsP_O	= tDdrDqs5P;
assign		ddrDqsN_O	= tDdrDqs5N;
assign		ddrDqs_En	= tDdrDqs5_En;
`endif

reg[13:0]		tDdrAddr;		//Address pins
reg[2:0]		tDdrBa;			//Bank Address pins
reg				tDdrCs;
reg				tDdrRas;
reg				tDdrCas;
reg				tDdrWe;
reg				tDdrCke;

`ifndef def_true
assign		ddrAddr	= tDdrAddr;
assign		ddrBa	= tDdrBa;
assign		ddrCs	= tDdrCs;
assign		ddrRas	= tDdrRas;
assign		ddrCas	= tDdrCas;
assign		ddrWe	= tDdrWe;
assign		ddrCke	= tDdrCke;
`endif

reg[13:0]		tDdrLastAddr;	//Address pins
reg[2:0]		tDdrLastBa;		//Bank Address pins
reg				tDdrLastCs;
reg				tDdrLastRas;
reg				tDdrLastCas;
reg				tDdrLastWe;
reg				tDdrLastCke;

`ifdef def_true
assign		ddrAddr	= tDdrLastAddr;
assign		ddrBa	= tDdrLastBa;
assign		ddrCs	= tDdrLastCs;
assign		ddrRas	= tDdrLastRas;
assign		ddrCas	= tDdrLastCas;
assign		ddrWe	= tDdrLastWe;
assign		ddrCke	= tDdrLastCke;
`endif

reg[127:0]		tMemDataOut;
reg[127:0]		tMemDataOutL;
reg[1:0]		tMemOK;
reg[1:0]		tMemOKL;
reg[1:0]		tMemOKL2;
reg[3:0]		tMemOkSq;
reg[3:0]		tMemOkSqL;

`ifdef jx2_mem_ddr32B
`reg_ddrtile	tMemDataInA;
`reg_ddrtile	tMemDataOutA;
`reg_ddrtile	tNxtMemDataOutA;
reg[1:0]		tMemOKA;
reg[1:0]		tNxtMemOKA;
reg[2:0]		tMemStateA;
reg[2:0]		tNxtMemStateA;
reg[31:0]		tMemAddrA;
reg[31:0]		tMemAddrSwA;
reg[4:0]		tMemOpmA;

reg[127:0]		tNxtMemDataIn;
reg[31:0]		tNxtMemAddr;
reg[31:0]		tNxtMemAddrSw;
reg[4:0]		tNxtMemOpm;
`endif

reg[63:0]		tMemDataChk2A0;
reg[31:0]		tMemDataChk2A1;
reg[19:0]		tMemDataChk2A;

`reg_ddrtile	tMemDataOut2;
reg[19:0]		tMemDataChk2;
reg[11:0]		tMemOK2;

`ifdef mod_ddr_extrabuf
reg[63:0]		tMemDataChk6A0;
reg[31:0]		tMemDataChk6A1;
reg[19:0]		tMemDataChk6A;
`endif

`ifdef mod_ddr_basicbuf
reg[63:0]		tMemDataChk4A0;
reg[31:0]		tMemDataChk4A1;
reg[19:0]		tMemDataChk4A;

reg[63:0]		tMemDataChk3A0;
reg[31:0]		tMemDataChk3A1;
reg[19:0]		tMemDataChk3A;
`endif


`ifdef mod_ddr_extrabuf
`reg_ddrtile	tMemDataOut3;
reg[19:0]		tMemDataChk3;
reg[11:0]		tMemOK3;
`reg_ddrtile	tMemDataOut4;
reg[19:0]		tMemDataChk4;
reg[11:0]		tMemOK4;
`reg_ddrtile	tMemDataOut5;
reg[19:0]		tMemDataChk5;
reg[11:0]		tMemOK5;
`reg_ddrtile	tMemDataOut6;
reg[19:0]		tMemDataChk6;
reg[11:0]		tMemOK6;
reg[11:0]		tMemOK7;

`reg_ddrtile	tMemDataOutO;
reg[1:0]		tMemOKO;
`reg_ddrtile	tMemDataOutOL;
reg[1:0]		tMemOKOL;
reg[3:0]		tMemOpSqO;
reg[3:0]		tMemOpSqOL;

// assign			memOK		= tMemOK6;
// assign			memOK		= tMemOK7;
// assign			memDataOut	= tMemDataOut6;
assign			memOK		= tMemOKO;
assign			memDataOut	= tMemDataOutO;
assign			memOpSqO	= tMemOpSqO;
`endif

`ifdef mod_ddr_basicbuf
`reg_ddrtile	tMemDataOut3;
reg[19:0]		tMemDataChk3;
reg[11:0]		tMemOK3;
`reg_ddrtile	tMemDataOut4;
reg[19:0]		tMemDataChk4;
reg[11:0]		tMemOK4;
// assign			memOK		= tMemOK4;
// assign			memDataOut	= tMemDataOut4;

`reg_ddrtile	tMemDataOutO;
reg[1:0]		tMemOKO;
`reg_ddrtile	tMemDataOutOL;
reg[1:0]		tMemOKOL;
reg[3:0]		tMemOpSqO;
reg[3:0]		tMemOpSqOL;

assign			memOK		= tMemOKO;
assign			memDataOut	= tMemDataOutO;
assign			memOpSqO	= tMemOpSqO;

`endif

`ifdef mod_ddr_fastbuf
assign			memOK		= tMemOK2[1:0];
assign			memDataOut	= tMemDataOut2;
`endif

reg[127:0]		tMemDataIn;
reg[31:0]		tMemAddr;
reg[31:0]		tMemAddrSw;
reg[4:0]		tMemOpm;


reg[63:0]		tMemChkInB0;
reg[31:0]		tMemChkInB1;
reg[19:0]		tMemChkInB2;
reg[15:0]		tMemChkInB2A;

`ifdef mod_ddr_basicbuf
reg[63:0]		tMemChkInC0;
reg[31:0]		tMemChkInC1;
reg[19:0]		tMemChkInC2;
reg[15:0]		tMemChkInC2A;
`endif

`ifdef mod_ddr_extrabuf
reg[63:0]		tMemChkInE0;
reg[31:0]		tMemChkInE1;
reg[19:0]		tMemChkInE2;
reg[15:0]		tMemChkInE2A;
`endif

`reg_ddrtile	tMemDataInB;
reg[31:0]		tMemAddrB;
reg[31:0]		tMemAddrSwB;
reg[19:0]		tMemChkInB;
reg[17:0]		tMemOpmB;

`reg_ddrtile	tMemDataInC;
reg[31:0]		tMemAddrC;
reg[31:0]		tMemAddrSwC;
reg[19:0]		tMemChkInC;
reg[17:0]		tMemOpmC;

`reg_ddrtile	tMemDataInD;
reg[31:0]		tMemAddrD;
reg[31:0]		tMemAddrSwD;
reg[19:0]		tMemChkInD;
reg[17:0]		tMemOpmD;

`reg_ddrtile	tMemDataInE;
reg[31:0]		tMemAddrE;
reg[31:0]		tMemAddrSwE;
reg[19:0]		tMemChkInE;
reg[17:0]		tMemOpmE;

`reg_ddrtile	tMemDataInF;
reg[31:0]		tMemAddrF;
reg[31:0]		tMemAddrSwF;
reg[19:0]		tMemChkInF;
reg[17:0]		tMemOpmF;

reg[3:0]		tMemOpSq;
reg[3:0]		tReqOpSq;
reg[3:0]		tNxtReqOpSq;


reg[5:0]		accState;
reg[5:0]		accNextState;

reg				accCkLo;
reg				accNextCkLo;

reg[127:0]		accReadBlk;
reg[127:0]		accNextReadBlk;

reg[127:0]		accStoreFifo;
reg[127:0]		accNextStoreFifo;

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

wire[15:0]		ddrData2p;
assign		ddrData2p = tPhaseHc ? ddrData2H : ddrData2;

reg[1:0]		tBurstCnt;
reg[1:0]		tNxtBurstCnt;

reg				tDoSwap;		//Do Swap
reg				tNxtDoSwap;
reg				tSwapStLd;		//Swap: Store Then Load
reg				tNxtSwapStLd;
reg				tSwapLoadDn;	//Swap is Done
reg				tNxtSwapLoadDn;


always @*
begin
	accNextState	= accState;
	accNextCkLo		= !accCkLo;

	accNextReadBlk	= accReadBlk;
//	accNextStoreFifo	= accStoreFifo;
	tDdrOut			= 0;
//	tDdrData		= 0;
	tDdrData		= 16'hzzzz;
	
//	tMemDataOut		= UV128_XX;
//	tMemDataOut		= UV128_00;
//	tMemOK			= UMEM_OK_READY;
//	tMemOkSq		= 0;

	tMemDataOut		= accReadBlk;
	tMemOK			= tMemOKL;
	tMemOkSq		= tMemOkSqL;

	tNxtReqOpSq		= tReqOpSq;

	tNxtBurstCnt	= tBurstCnt;
	tNxtDoSwap		= tDoSwap;
	tNxtSwapLoadDn	= tSwapLoadDn;
	tNxtSwapStLd	= tSwapStLd;

//	tDdrCmd			= 10'b0000000111;
	tDdrClk			= accNextCkLo ? 2'b10 : 2'b01;
//	tDdrClk			= accNextCkLo ? 2'b01 : 2'b10;
//	tDdrClk			= (!accState[0]) ? 2'b10 : 2'b01;

	tDdrDqs			= tDdrClk;
	tDdrDqs_En		= 0;
	tNxtPhaseHc		= tPhaseHc;

	accNxtSkipRowClose	= accSkipRowClose;
	accNxtSkipRowOpen	= accSkipRowOpen;

	if(accState[0])
	begin
		tDdrAddr	= tDdrLastAddr;
		tDdrBa		= tDdrLastBa;
		tDdrCke		= tDdrLastCke;
		tDdrCs		= tDdrLastCs;
		tDdrRas		= tDdrLastRas;
		tDdrCas		= tDdrLastCas;
		tDdrWe		= tDdrLastWe;
	end
	else
	begin
		tDdrAddr	= 0;
		tDdrBa		= 0;
		tDdrCke		= 1;
//		tDdrCs		= 1;
		tDdrCs		= 0;
		tDdrRas		= 1;
		tDdrCas		= 1;
		tDdrWe		= 1;
	end

	accNextCkCas	= accCkCas;

	accNextAddr		= accAddr;
	accNextRowAddr	= accRowAddr;
	accNextColAddr	= accColAddr;
	accNextBaAddr	= accBaAddr;

	dreNextRowAddr	= dreRowAddr;
	dreNextBaAddr	= dreBaAddr;
	dreNextCount	= dreCount;
	
//	driNextModeOut	= driModeOut;

//	tMemOK			= UMEM_OK_READY;

//	driStillInit	= (driModeOut[15:0]!=0);
//	dreIsZero		= (dreCount == 0);
	dreNextIsZero	= (dreCount == 0);

//	driStillInit	= (driInitState != 5'h1F);
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

//	driStillInit	= (driInitCmd != 0);
	driNextStillInit	= (driInitCmd != 0);

// reg[4:0]		driInitState;
// reg[4:0]		driNextInitState;
// reg[15:0]		driInitCmd;

//	if(!dreIsZero && accCkLo)
	if(!dreNextIsZero && accCkLo)
	begin
		dreNextCount = dreCount - 1;
	end

	if(driStillInit)
	begin
//		$display("DRI Mode %X", driModeOut);
	end

// `ifndef def_true
`ifdef def_true
	/* If doing something Load/Store requests can wait. */
	if(accState!=0)
	begin
//		if(tMemOpm[3])
//			tMemOK		= UMEM_OK_HOLD;
//		if(tMemOpm[4])
//			tMemOK		= UMEM_OK_HOLD;
		if(tMemOpm[4:3]!=0)
			tMemOK		= UMEM_OK_HOLD;
	end
`endif

	tDdrData 				= accStoreFifo[15:0];

//	accNextStoreFifo		= accReadBlk;
	accNextStoreFifo		= accStoreFifo;
	if(accState[5:3]==3'b110)
	begin
		accNextStoreFifo	= { 16'h00, accStoreFifo[127:16] };
		tDdrOut				= 1;
		tDdrDqs_En			= 1;
	end

//	$display("ModDdr: State %X", accState);

	if(accState!=0)
	begin
//		$display("ModDdr: State %X", accState);
	end

	case(accState)
	6'b000000: begin	/* Waiting for request */
//		$display("ModDdr: State 0");

		tMemOK			= UMEM_OK_READY;

		accNextState	= 6'b000000;
		tNxtSwapLoadDn	= 0;
		tNxtDoSwap		= 0;
		tNxtSwapStLd	= 0;

		accNextStoreFifo	= tMemDataIn;
		accNextReadBlk		= tMemDataIn;

`ifdef jx2_mem_ddrswap
		if((tMemOpm[4:3] == 2'b11) && !driStillInit && !dreIsZero)
		begin
//			$display("ModDdr: Swap Req, Ar=%X Aw=%X", tMemAddr, tMemAddrSw);
			tNxtDoSwap		= 1;
//			accNextStoreFifo	= tMemDataIn;

//			tNxtSwapStLd	= 1;
//			tNxtSwapStLd	= (tMemAddrSw[21:6] == tMemAddr[21:6]);
			tNxtSwapStLd	= (tMemAddrSw[27:4] == tMemAddr[27:4]);

			tMemOK		= UMEM_OK_HOLD;
			if(accCkLo)
			begin
				if(tNxtSwapStLd)
				begin
					accNextState	= 6'b011000;
					accNextAddr		= {4'h0, tMemAddrSw[27:4], 4'h0};
					tNxtDoSwap		= 0;
				end
				else
				begin
					accNextState = 6'b010000;
					accNextAddr	= {4'h0, tMemAddr[27:4], 4'h0};
				end

				tNxtReqOpSq	= tMemOpSq;
				accNextRowAddr = accNextAddr[28:13];
				accNextBaAddr = accNextAddr[12:10];
				accNextColAddr = {6'b0, accNextAddr[ 9:0]};
			end
		end
		else
`else
		if((tMemOpm[4:3]==2'b11) &&
			!driStillInit && !dreIsZero)	/* Swap Request */
		begin
			tMemDataOut		= UV128_00;
			tMemOK			= UMEM_OK_FAULT;
		end
		else
`endif
			if(tMemOpm[3] && !driStillInit && !dreIsZero)	/* Load Request */
		begin
//			$display("ModDdr: Read Req, A=%X", tMemAddr);

			tMemOK		= UMEM_OK_HOLD;
			if(accCkLo)
			begin
				accNextState = 6'b010000;

				if(accSkipRowOpen)
					accNextState = 6'b010100;

				tNxtReqOpSq	= tMemOpSq;

				accNextAddr	= {4'h0, tMemAddr[27:4], 4'h0};

//				accNextRowAddr = accNextAddr[24:9];
//				accNextColAddr = {7'b0, accNextAddr[ 8:0]};

				accNextRowAddr = accNextAddr[28:13];
				accNextBaAddr = accNextAddr[12:10];
				accNextColAddr = {6'b0, accNextAddr[ 9:0]};

//				accNextRowAddr = tMemAddr[28:13];
//				accNextBaAddr = tMemAddr[12:10];
//				accNextColAddr = {6'b0, tMemAddr[ 9:0]};

//				$display("Read Row=%X Bank=%X Col=%X",
//					accNextRowAddr,
//					accNextBaAddr,
//					accNextColAddr);
			end
		end
		else
			if(tMemOpm[4] && !driStillInit && !dreIsZero)	/* Store Request */
		begin
//			$display("ModDdr: Store Req, A=%X", tMemAddr);

			accNextReadBlk	= tMemDataIn;
//			accNextStoreFifo	= tMemDataIn;
			tMemOK			= UMEM_OK_HOLD;
			tNxtReqOpSq	= tMemOpSq;
			if(accCkLo)
			begin
				if(tMemOpm==UMEM_OPM_WR_SW)
				begin
					accNextState	= 6'b000010;
					accNextRowAddr	= tMemDataIn[15:0];
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
//					accNextReadBlk = tMemDataIn;
//					accNextStoreFifo = tMemDataIn;
				end
			end
		end
		else
		begin
			if(accCkLo)
			begin
//				$display("ModDdr: IdleClk");

//				if(driModeOut[15:0]!=0)
//				if(driInitCmd!=0)
				if(driStillInit)
				begin
					accNextState	= 6'b000010;
//					accNextRowAddr	= driModeOut[15:0];
//					driNextModeOut	= { 16'h0000, driModeOut[63:16] };
//					driNextModeOut	= { 16'h0000, driModeOut[127:16] };

					accNextRowAddr		= driInitCmd;
					driNextInitState	= driInitState + 1;
				end
				else
//				if(dreCount == 0)
				if(dreIsZero)
				begin
					accNextState	= 6'b001000;

					accNextRowAddr	= dreRowAddr;
//					accNextBaAddr = 0;
					accNextBaAddr = dreBaAddr;
					accNextColAddr = 0;

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

//					dreNextRowAddr	= { 3'b000, dreRowAddr[12:0] + 13'h1 };
//					dreNextCount = 8192;
//					dreNextCount = 4096;
//					dreNextCount = 2048;
					dreNextCount = 1024;
				end
				else
				begin
//					dreNextCount = dreCount - 1;
					tDdrCs	= 1;
				end
			end
		end

	end

	6'b000001: begin	/* Access Complete, Hold */
		tDdrCs	= 1;

`ifdef jx2_mem_ddrswap
		if(tDoSwap && !tSwapLoadDn)
		begin
			tMemOK			= UMEM_OK_HOLD;
			accNextCkCas	= DDR_TRP_M1;	/* RAS Latency (-1) */

			if(accCkLo)
				accNextState	= 6'b000100;
			else
				accNextState	= 6'b000101;
		end
		else
`endif

//		if(tMemOpm[4] || tMemOpm[3])
		if(tMemOpm[4:3] != 0)
		begin
//			$display("DDR ReadBlk %X", accReadBlk);
			/* Hang out here until released. */
			tMemDataOut		= accReadBlk;
			tMemOK			= UMEM_OK_OK;
			tMemOkSq		= tReqOpSq;
			accNextState	= 6'b000001;

//			$display("DDR: Sequence, %X %X", tReqOpSq, tMemOpSq);
			
			if(tReqOpSq != tMemOpSq)
			begin
//				$display("DDR: Sequence Trigger, %X %X", tReqOpSq, tMemOpSq);
				/* Sequence Changed, Return to Idle */
				accNextState	= 6'b000000;
			end
		end
		else
		begin
			/* Return to waiting state. */
			accNextState	= 6'b000000;
		end
	end

	

	6'b000010: begin
		/* Mode Register Write (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10  R9  R8  Hi  Lo */
//		tDdrCmd			= {accRowAddr[13:8], 4'b0000};

		tDdrAddr		= {1'b0, accRowAddr[12: 0] };
		tDdrBa			= {1'b0, accRowAddr[14:13] };
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 0;	tDdrWe	= 0;

		accNextState	= 6'b000011;

		if(accRowAddr[15:12]==4'h8)
		begin
			case(accRowAddr[11:8])
				4'h6: begin		/* NOP Delay with CKE Disabled */
					tDdrCke	= 0;
					tDdrCs	= 0;	tDdrRas	= 1;
					tDdrCas	= 1;	tDdrWe	= 1;
				end

				4'h8: begin		/* PRECHARGE */
					tDdrCs	= 0;	tDdrRas	= 0;
					tDdrCas	= 1;	tDdrWe	= 0;
				end

				4'h9: begin		/* REFRESH */
					tDdrCs	= 0;	tDdrRas	= 0;
					tDdrCas	= 0;	tDdrWe	= 1;
				end

				default: begin	/* NOP Delay */
					tDdrCs	= 0;	tDdrRas	= 1;
					tDdrCas	= 1;	tDdrWe	= 1;
				end
			endcase

//			tDdrCs	= 0;	tDdrRas	= 1;
//			tDdrCas	= 1;	tDdrWe	= 0;
			
		end

	end
	6'b000011: begin
		/* Mode Register Write (Fall) */
		/* R14 R13  R7  R6  R5  R4  R3  R2  R1  R0 */
//		tDdrCmd			= {accRowAddr[7:0], accRowAddr[15:14]};
//		accNextState	= 6'b000001;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
//		if(accRowAddr[15:8]==8'h0A)
		if(accRowAddr[15:12]==4'h8)
		begin
//			accNextCkCas	= DDR_RAS_INIT;
			case(accRowAddr[11:8])
				4'h0:	accNextCkCas	= { 8'h0, accRowAddr[7:0]       };
				4'h1:	accNextCkCas	= { 4'h0, accRowAddr[7:0], 4'h0 };
				4'h2: 	accNextCkCas	= {       accRowAddr[7:0], 8'h0 };
				4'h6: 
				begin
					tDdrCke			= 0;
					accNextCkCas	= {       accRowAddr[7:0], 8'h0 };
				end

//				4'h8: 
//				begin
//				end

//				default:	accNextCkCas	= DDR_RAS_INIT;

				default: 
				begin
				end

			endcase
		end
		accNextState	= 6'b001110;
	end
	
	6'b000100: begin	/* Swap Landing Zone, Rise */
		accNextState	= 6'b000101;

		if(accCkCas==0)
		begin
			if(!tSwapStLd)
			begin
				tMemOK		= UMEM_OK_OK;
			end
		end
	end

	6'b000101: begin	/* Swap Landing Zone, Fall */
		tNxtSwapLoadDn	= 1;
		
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b000100;
		end
		else
		begin
			if(tSwapStLd)
			begin
	//			$display("Swap, StB Load");
	//			tMemOK			= UMEM_OK_HOLD;
	//			tMemOkSq		= 0;
				accNextState	= 6'b010000;
				accNextAddr	= {4'h0, tMemAddr[27:4], 4'h0};
			end
			else
			begin
	//			$display("Swap, StB Store");
				accNextState	= 6'b011000;
				accNextAddr	= {4'h0, tMemAddrSw[27:4], 4'h0};
				tMemOK			= UMEM_OK_OK;
			end
			
			accNextRowAddr = accNextAddr[28:13];
			accNextBaAddr = accNextAddr[12:10];
			accNextColAddr = {6'b0, accNextAddr[ 9:0]};
		end
	end
	


	/* Precharge Start */
	
	6'b001000: begin
		/* Row Activate (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10 R9 R8 Hi Lo */
//		tDdrCmd			= {accBaAddr[2:0], accRowAddr[12:8], 2'b10};

		tDdrAddr		= accRowAddr[13:0];
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 1;	tDdrWe	= 1;

		accNextState	= 6'b001001;
	end
	6'b001001: begin
		/* Row Activate (Fall) */
//		tDdrCmd			= {accRowAddr[14:13], accRowAddr[7: 0]};
		accNextState	= 6'b001010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b001010: begin
		/* RAS Hold (Rise) */
		accNextState	= 6'b001011;
	end
	6'b001011: begin
		/* RAS Hold (Fall) */
		accNextState	= 6'b001100;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b001010;
		end
	end

	6'b001100: begin
		/* Precharge (Rise) */
		/*  -  -  -  -  -  1  1  0  1  1 */
		
//		tDdrCmd			= {accBaAddr[2:0], accColAddr[2:1], 5'b11011};

		tDdrAddr		= accColAddr[13:0];
		tDdrAddr[10]	= 1;
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 1;	tDdrWe	= 0;

		accNextState	= 6'b001101;
	end
	6'b001101: begin
		/* Precharge (Fall) */
		/*  -  -  -  -  -  -  -  -  -  - */
//		tDdrCmd			= {accColAddr[11:3], 1'b1};
		accNextState	= 6'b001110;
		accNextCkCas	= DDR_CAS_RL_M1;	/* CAS Latency (-1) */
	end

	6'b001110: begin
		/* CAS Hold (Rise) */
		accNextState	= 6'b001111;
		tDdrCke			= tDdrLastCke;
	end
	6'b001111: begin
		/* CAS Hold (Fall) */
		accNextState	= 6'b000000;
		tDdrCke			= tDdrLastCke;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b001110;
//			$display("DDR: CAS Hold %d", accNextCkCas);
		end
	end


	/* Read Start */
	
	6'b010000: begin
		/* Row Activate (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10  R9  R8  Hi  Lo */
//		tDdrCmd			= {accBaAddr[2:0], accRowAddr[12:8], 2'b10};

		tDdrAddr		= accRowAddr[13:0];
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 1;	tDdrWe	= 1;

		accNextState	= 6'b010001;
	end
	6'b010001: begin
		/* Row Activate (Fall) */
		/* R14 R13  R7  R6  R5  R4  R3  R2  R1  R0 */
//		tDdrCmd			= {accRowAddr[14:13], accRowAddr[7: 0]};
		accNextState	= 6'b010010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b010010: begin
		/* RAS Hold (Rise) */
		accNextState	= 6'b010011;
	end
	6'b010011: begin
		/* RAS Hold (Fall) */
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

//		tDdrCmd			= {accBaAddr[2:0], accColAddr[2:1], 5'b00101};

		tDdrAddr		= accColAddr[13:0];
		tDdrAddr[10]	= 1;
		tDdrAddr[12]	= 1;
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 1;
		tDdrCas	= 0;	tDdrWe	= 1;

		if(accSkipRowClose)
			tDdrAddr[10]	= 0;

		accNextState	= 6'b010101;
	end

	6'b010101: begin
		/* Column Read (Fall) */
		/* C11 C10 C9 C8 C7 C6 C5 C4 C3 AP */

//		$display("DDR Read ColB %X", accColAddr);

//		tDdrCmd			= {accColAddr[11:3], 1'b1};
		accNextState	= 6'b010110;
//		accNextState	= 6'b010111;
		accNextCkCas	= DDR_CAS_RL_M1;
	end

	6'b010110: begin
		/* Read CAS Hold (Rise) */
		accNextState	= 6'b010111;
	end
	6'b010111: begin
		/* Read CAS Hold (Fall) */
		accNextState	= 6'b100000;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b010110;
		end
	end
	


	/* Write Start */
	
	6'b011000: begin
		/* Row Activate (Rise) */
		/* BA2 BA1 BA0 R12 R11 R10 R9 R8 Hi Lo */
//		tDdrCmd			= {accBaAddr[2:0], accRowAddr[12:8], 2'b10};

		tDdrAddr		= accRowAddr[13:0];
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 0;
		tDdrCas	= 1;	tDdrWe	= 1;

		accNextState	= 6'b011001;
	end
	6'b011001: begin
		/* Row Activate (Fall) */
//		tDdrCmd			= {accRowAddr[14:13], accRowAddr[7: 0]};
		accNextState	= 6'b011010;
		accNextCkCas	= DDR_RAS_M1;	/* RAS Latency (-1) */
	end

	6'b011010: begin
		/* RAS Hold (Rise) */
		accNextState	= 6'b011011;
	end
	6'b011011: begin
		/* RAS Hold (Fall) */
		accNextState	= 6'b011100;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b011010;
		end
	end

	6'b011100: begin
		/* Column Write (Rise) */
		/* BA2 BA1 BA0 C2 C1 r r 1 0 1 */
		
//		tDdrCmd			= {accBaAddr[2:0], accColAddr[2:1], 5'b00101};

		tDdrAddr		= accColAddr[13:0];
		tDdrAddr[10]	= 1;
		tDdrAddr[12]	= 1;
		tDdrBa			= accBaAddr[2:0];
		tDdrCke	= 1;
		tDdrCs	= 0;	tDdrRas	= 1;
		tDdrCas	= 0;	tDdrWe	= 0;

		if(accSkipRowClose)
			tDdrAddr[10]	= 0;

		accNextState	= 6'b011101;
	end
	6'b011101: begin
		/* Column Write (Fall) */
		/* C11 C10 C9 C8 C7 C6 C5 C4 C3 AP */
//		tDdrCmd			= {accColAddr[11:3], 1'b1};
		accNextState	= 6'b011110;
//		accNextState	= 6'b110000;
		accNextCkCas	= DDR_CAS_WL_M1;	/* CAS Latency (-1) */
	end

	6'b011110: begin
		/* Write CAS Hold (Rise) */
		accNextState	= 6'b011111;
		tDdrDqs			= 2'b10;
		tDdrDqs_En		= 1;
	end
	6'b011111: begin
		/* Write CAS Hold (Fall) */
		accNextState	= 6'b110000;
//		tDdrDqs			= 2'b10;
		tDdrDqs_En		= 1;
		if(accCkCas!=0)
		begin
			accNextCkCas	= accCkCas - 1;
			accNextState	= 6'b011110;
			tDdrDqs			= 2'b10;
		end
	end
	

	
	/* Read Burst States */
	
	6'b100000: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word0 %X (Ck=%d)", ddrData2, clock);
`endif
		accNextState			= 6'b100001;
//		tDdrDqs_En		= 1;

		/* Attempt to align with DQS */
		if(ddrDqs2H==1)
		begin
			accNextReadBlk[15:0]	= ddrData2H;
			tNxtPhaseHc = 1;
		end
		else
			if(ddrDqs2==1)
		begin
			accNextReadBlk[15:0]	= ddrData2;
			tNxtPhaseHc = 0;
		end
		else
		begin
				accNextState			= 6'b100000;
		end

`ifndef def_true
		/* If needed, attempt to re-align with DQS */
		if(ddrDqs2!=1)
		begin
			if(ddrDqs2H!=1)
			begin
				accNextState			= 6'b100000;
			end
			else
			begin
				tNxtPhaseHc = 1;
				accNextReadBlk[15:0]	= ddrData2H;
			end
		end
`endif
	end
	6'b100001: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word1 %X (Ck=%d)", ddrData2p, clock);
`endif
		accNextState			= 6'b100010;
		accNextReadBlk[31:16]	= ddrData2p;
//		tDdrDqs_En		= 1;
	end
	6'b100010: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word2 %X", ddrData2p);
`endif
		accNextState			= 6'b100011;
		accNextReadBlk[47:32]	= ddrData2p;
//		tDdrDqs_En		= 1;
	end
	6'b100011: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word3 %X", ddrData2p);
`endif
		accNextState			= 6'b100100;
		accNextReadBlk[63:48]	= ddrData2p;
//		tDdrDqs_En		= 1;
`ifdef jx2_ddr_bl64b
		accNextState			= 6'b000001;
`endif
	end
	6'b100100: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word4 %X", ddrData2p);
`endif
		accNextState			= 6'b100101;
		accNextReadBlk[79:64]	= ddrData2p;
//		tDdrDqs_En		= 1;
	end
	6'b100101: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word5 %X", ddrData2p);
`endif
		accNextState			= 6'b100110;
		accNextReadBlk[95:80]	= ddrData2p;
//		tDdrDqs_En		= 1;
	end
	6'b100110: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word6 %X", ddrData2p);
`endif
		accNextState			= 6'b100111;
		accNextReadBlk[111:96]	= ddrData2p;
//		tDdrDqs_En		= 1;
	end
	6'b100111: begin
`ifdef mod_ddr_dbgprn
		$display("DDR Read Word7 %X", ddrData2p);
`endif
		accNextState			= 6'b000001;
		accNextReadBlk[127:112]	= ddrData2p;
//		tDdrDqs_En		= 1;
	end


	/* Write Burst States */
	
	6'b110000: begin
		accNextState			= 6'b110001;
//		tDdrData 				= accReadBlk[15:0];
//		tDdrData 				= accStoreFifo[15:0];
//		accNextStoreFifo			= { 16'h00, accStoreFifo[127:16] };
//		tDdrOut					= 1;
//		tDdrDqs_En				= 1;
`ifdef mod_ddr_dbgprn
		$display("DDR Write Word0 %X (Ck=%d)", tDdrData, clock);
`endif
	end
	6'b110001: begin
		accNextState			= 6'b110010;
//		tDdrData 				= accReadBlk[31:16];
//		tDdrData 				= accStoreFifo[15:0];
//		accNextStoreFifo			= { 16'h00, accStoreFifo[127:16] };
//		tDdrOut					= 1;
//		tDdrDqs_En				= 1;
`ifdef mod_ddr_dbgprn
		$display("DDR Write Word1 %X (Ck=%d)", tDdrData, clock);
`endif
	end
	6'b110010: begin
		accNextState			= 6'b110011;
//		tDdrData 				= accReadBlk[47:32];
//		tDdrData 				= accStoreFifo[15:0];
//		accNextStoreFifo			= { 16'h00, accStoreFifo[127:16] };
//		tDdrOut					= 1;
//		tDdrDqs_En				= 1;
`ifdef mod_ddr_dbgprn
		$display("DDR Write Word2 %X (Ck=%d)", tDdrData, clock);
`endif
	end
	6'b110011: begin
		accNextState			= 6'b110100;
//		tDdrData 				= accReadBlk[63:48];
//		tDdrData 				= accStoreFifo[15:0];
//		accNextStoreFifo			= { 16'h00, accStoreFifo[127:16] };
//		tDdrOut					= 1;
//		tDdrDqs_En				= 1;
`ifdef mod_ddr_dbgprn
		$display("DDR Write Word3 %X (Ck=%d)", tDdrData, clock);
`endif

`ifdef jx2_ddr_bl64b
		accNextState			= 6'b000001;
`endif
	end
	6'b110100: begin
		accNextState			= 6'b110101;
//		tDdrData 				= accReadBlk[79:64];
//		tDdrData 				= accStoreFifo[15:0];
//		accNextStoreFifo			= { 16'h00, accStoreFifo[127:16] };
//		tDdrOut					= 1;
//		tDdrDqs_En				= 1;
`ifdef mod_ddr_dbgprn
		$display("DDR Write Word4 %X (Ck=%d)", tDdrData, clock);
`endif
	end
	6'b110101: begin
		accNextState			= 6'b110110;
//		tDdrData 				= accReadBlk[95:80];
//		tDdrData 				= accStoreFifo[15:0];
//		accNextStoreFifo			= { 16'h00, accStoreFifo[127:16] };
//		tDdrOut					= 1;
//		tDdrDqs_En				= 1;
`ifdef mod_ddr_dbgprn
		$display("DDR Write Word5 %X (Ck=%d)", tDdrData, clock);
`endif
	end
	6'b110110: begin
		accNextState			= 6'b110111;
//		tDdrData 				= accReadBlk[111:96];
//		tDdrData 				= accStoreFifo[15:0];
//		accNextStoreFifo			= { 16'h00, accStoreFifo[127:16] };
//		tDdrOut					= 1;
//		tDdrDqs_En				= 1;
`ifdef mod_ddr_dbgprn
		$display("DDR Write Word6 %X (Ck=%d)", tDdrData, clock);
`endif
	end
	6'b110111: begin
		accNextState			= 6'b000001;
//		tDdrData 				= accReadBlk[127:112];
//		tDdrData 				= accStoreFifo[15:0];
//		accNextStoreFifo			= { 16'h00, accStoreFifo[127:16] };
//		tDdrOut					= 1;
//		tDdrDqs_En				= 1;
`ifdef mod_ddr_dbgprn
		$display("DDR Write Word7 %X (Ck=%d)", tDdrData, clock);
`endif
	end

	default: begin
		accNextState			= 6'b000000;
	end

	endcase
	
	if(reset)
	begin
		tMemOK			= UMEM_OK_READY;
		accNextState	= 6'b000000;
	end

	
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
	tNxtMemAddrSw	= tMemAddrSw;
	tNxtMemOpm		= tMemOpm;

`ifndef def_true
// `ifdef def_true
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
						tNxtMemDataOutA[127:0]		= UV128_00;
						tNxtMemStateA	= 3'b110;
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
					tNxtMemAddrSw	= tMemAddrSwA;
					tNxtMemOKA		= UMEM_OK_HOLD;
				end

				3'b010, 3'b011: begin
					accNxtSkipRowClose	= 0;
//					accNxtSkipRowOpen	= 1;
					tNxtMemDataIn	= tMemDataInA[255:128];
					tNxtMemOpm		= tMemOpmA;
					tNxtMemAddr		= tMemAddrA;
					tNxtMemAddrSw	= tMemAddrSwA;
					tNxtMemAddr[4]		= 1;
					tNxtMemAddrSw[4]	= 1;
					tNxtMemOKA		= UMEM_OK_HOLD;
					tNxtMemStateA	= 3'b011;
				end

				3'b100: begin
					accNxtSkipRowClose	= 0;
					accNxtSkipRowOpen	= 0;

					tNxtMemOpm		= UMEM_OPM_READY;
					tNxtMemOKA		= UMEM_OK_OK;
					tNxtMemStateA	= 3'b111;
				end

				3'b110: begin
					accNxtSkipRowClose	= 0;
					accNxtSkipRowOpen	= 0;

					tNxtMemOpm		= UMEM_OPM_READY;
					tNxtMemOKA		= UMEM_OK_FAULT;
					tNxtMemStateA	= 3'b111;
				end

				3'b111: begin
					accNxtSkipRowClose	= 0;
					accNxtSkipRowOpen	= 0;
//					if(tMemOpmA == UMEM_OPM_READY)
					if(tMemOpmA[4:3]==2'b00)
						tNxtMemStateA	= 3'b000;
				end
				
				default: begin
					tNxtMemOpm		= UMEM_OPM_READY;
					tNxtMemOKA		= UMEM_OK_FAULT;
//					if(tMemOpmA == UMEM_OPM_READY)
//					if(tMemOpmA[4:3]==2'b00)
//						tNxtMemStateA	= 3'b000;
					tNxtMemStateA	= 3'b111;
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
//					if(tMemOpmA[3])
//						$display("Data A Addr=%X Data=%X",
//							tNxtMemAddr, tMemDataOutL);
					tNxtMemDataOutA[127:0]		= tMemDataOutL;
					tNxtMemStateA				= 3'b010;
				end
				3'b011: begin
//					if(tMemOpmA[3])
//						$display("Data B Addr=%X Data=%X",
//							tNxtMemAddr, tMemDataOutL);
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
			tNxtMemStateA				= 3'b110;
		end
	endcase
	
	if(reset)
	begin
		accNxtSkipRowClose	= 0;
		accNxtSkipRowOpen	= 0;
		tNxtMemOpm = 0;
		tNxtMemStateA	= 0;

//		tNxtMemDataOutA[255:0]		= UV256_00;
		tNxtMemDataOutA[15: 0]		= 16'h1234;			/* Magic */
		tNxtMemOKA					= UMEM_OK_READY;
	end
	
`endif
	
	if(tReset)
	begin
		accNextState		= 0;
		driNextInitState	= 0;
	end
end

reg[7:0]	regInitSanity;

reg			tCheckD;
reg			tCheckOK4;


initial begin
	regInitSanity = 0;
end

// always @(negedge clock)
// always @(negedge clock2)
always @(negedge ddr_coreclock)
begin
`ifndef def_true
	tDdrClk3		<= tDdrClk2;
	tDdrDqs3		<= tDdrDqs2;
	tDdrDqs3_En		<= tDdrDqs2_En;
	tDdrDqs3P		<= tDdrDqs2P;
	tDdrDqs3N		<= tDdrDqs2N;
`endif

`ifdef def_true
	tDdrClk5		<= tDdrClk4;
	tDdrDqs5		<= tDdrDqs4;
	tDdrDqs5_En		<= tDdrDqs4_En;
	tDdrDqs5P		<= tDdrDqs4P;
	tDdrDqs5N		<= tDdrDqs4N;
`endif

//	ddrData2		<= ddrData;
	ddrData1H		<= ddrData;
	ddrDqs1H		<= ddrDqs;
end

always @(posedge ddr_coreclock)
begin
`ifdef def_true
	tDdrClk3		<= tDdrClk2;
	tDdrDqs3		<= tDdrDqs2;
	tDdrDqs3_En		<= tDdrDqs2_En;
	tDdrDqs3P		<= tDdrDqs2P;
	tDdrDqs3N		<= tDdrDqs2N;

	tDdrClk4		<= tDdrClk3;
	tDdrDqs4		<= tDdrDqs3;
	tDdrDqs4_En		<= tDdrDqs3_En;
	tDdrDqs4P		<= tDdrDqs3P;
	tDdrDqs4N		<= tDdrDqs3N;
`endif

	tDdrData2		<= tDdrData;
	tDdrOut2		<= tDdrOut;

	tDdrData3		<= tDdrData2;
	tDdrOut3		<= tDdrOut2;
end


always @*
begin
	tMemChkInB2A = memAddr[31:16] ^ memAddr[15:0];
	tMemChkInB0 = memDataIn[127:64] ^ memDataIn[63:0];
	tMemChkInB1 = tMemChkInB0[63:32] ^ tMemChkInB0[31:0];
	tMemChkInB2 =	{
		memOpSqI[1:0] ^ memOpSqI[3:2],
		memOpm[4:3] ^ memOpm[1:0],
		tMemChkInB0[31:16] ^ tMemChkInB0[15:0] ^ tMemChkInB2A };
end

always @*
begin
`ifdef mod_ddr_extrabuf
	tMemDataChk6A0	= tMemDataOut6 [127:64] ^ tMemDataOut6   [63:0];
	tMemDataChk6A1	= tMemDataChk6A0[63:32] ^ tMemDataChk6A0[31:0];
	tMemDataChk6A	= 
		{ tMemOK6[5:4], tMemOK6[1:0],
		tMemDataChk6A1[31:16] ^ tMemDataChk6A1[15:0] };
`endif

`ifdef mod_ddr_basicbuf
	tMemDataChk4A0	= tMemDataOut4 [127:64] ^ tMemDataOut4   [63:0];
	tMemDataChk4A1	= tMemDataChk4A0[63:32] ^ tMemDataChk4A0[31:0];
	tMemDataChk4A	= 
		{ tMemOK4[5:4], tMemOK4[1:0],
			tMemDataChk4A1[31:16] ^ tMemDataChk4A1[15:0] };

`ifdef def_true
	tMemDataChk3A0	= tMemDataOut3 [127:64] ^ tMemDataOut3   [63:0];
	tMemDataChk3A1	= tMemDataChk3A0[63:32] ^ tMemDataChk3A0[31:0];
	tMemDataChk3A	= 
		{ tMemOK3[5:4], tMemOK3[1:0],
			tMemDataChk3A1[31:16] ^ tMemDataChk3A1[15:0] };
`endif

`endif
end

`ifdef mod_ddr_extrabuf
always @(posedge clock)
begin
	tMemOK3			<= tMemOK2;
	tMemDataOut3	<= tMemDataOut2;
	tMemDataChk3	<= tMemDataChk2;

	tMemOK4			<= tMemOK3;
	tMemDataOut4	<= tMemDataOut3;
	tMemDataChk4	<= tMemDataChk3;

	tMemOK5			<= tMemOK4;
	tMemDataOut5	<= tMemDataOut4;
	tMemDataChk5	<= tMemDataChk4;

	tCheckOK4		<= (tMemOK5 == tMemOK4);

	tMemOK6			<= tMemOK5;
	tMemDataOut6	<= tMemDataOut5;
	tMemDataChk6	<= tMemDataChk5;

	tMemOK7			<= tMemOK6;

	tMemDataInB		<= memDataIn;
	tMemAddrB		<= memAddr;
	tMemAddrSwB		<= memAddrSw;
//	tMemOpmB		<= memOpm;
//	tMemOpmB		<= { ~memOpm, memOpm };
	tMemOpmB		<= { ~memOpSqI, memOpSqI, ~memOpm, memOpm };
	tMemChkInB		<= tMemChkInB2;

//	tResetB			<= reset;

//	if((tMemOK7[1:0] == (~tMemOK7[3:2])) &&
	if(	(tMemOK6[1:0] == (~tMemOK6[ 3:2])) &&
		(tMemOK6[7:4] == (~tMemOK6[11:8])) &&
		(tMemDataChk6A == tMemDataChk6))
//	if(1'b1)
	begin
		tMemDataOutO	<= tMemDataOut6;
//		tMemOKO			<= tMemOK7[1:0];
		tMemOKO			<= tMemOK6[1:0];
		tMemOpSqO		<= tMemOK6[7:4];
	end
	else
	begin
//		if(tMemOK7[1:0] != (~tMemOK7[3:2]))
		if(tMemOK6[1:0] != (~tMemOK6[3:2]))
			$display("DDR: OK Check Fail");
		if(tMemDataChk6A != tMemDataChk6)
			$display("DDR: Data Check Fail %X %X",
				tMemDataChk6A, tMemDataChk6);
	
		tMemDataOutO	<= tMemDataOutOL;
		tMemOKO			<= tMemOKOL;
		tMemOpSqO		<= tMemOpSqOL;
	end
	tMemDataOutOL	<= tMemDataOutO;
	tMemOKOL		<= tMemOKO;
	tMemOpSqOL		<= tMemOpSqO;

end
`endif

reg tOpmOkSticky;

`ifdef mod_ddr_basicbuf
always @(posedge clock)
begin
	tMemOK3			<= tMemOK2;
	tMemDataOut3	<= tMemDataOut2;
	tMemDataChk3	<= tMemDataChk2;

	tMemOK4			<= tMemOK3;
	tMemDataOut4	<= tMemDataOut3;
	tMemDataChk4	<= tMemDataChk3;

`ifdef def_true
// `ifndef def_true
	if(	(tMemOK3[1:0] == (~tMemOK3[3:2])) &&
		(tMemOK3[7:4] == (~tMemOK3[11:8])) &&
		(tMemDataChk3A == tMemDataChk3))
	begin
		tMemDataOutO	<= tMemDataOut3;
		tMemOKO			<= tMemOK3[1:0];
		tMemOpSqO		<= tMemOK3[7:4];

	end
	else
	begin
		tMemDataOutO	<= tMemDataOutOL;
		tMemOKO			<= tMemOKOL;
		tMemOpSqO		<= tMemOpSqOL;
	end
`endif

`ifndef def_true
// `ifdef def_true
	if(	(tMemOK4[1:0] == (~tMemOK4[3:2])) &&
		(tMemOK4[7:4] == (~tMemOK4[11:8])) &&
		(tMemDataChk4A == tMemDataChk4))
	begin
// `ifdef def_true
`ifndef def_true
		if(tMemOpmB[4:0] == UMEM_OPM_READY)
		begin
			if(tMemOK4[1:0] != UMEM_OK_READY)
				tOpmOkSticky <= 1;
		end

		if(tMemOK4[1:0] == UMEM_OK_READY)
			tOpmOkSticky <= 0;

		if(tOpmOkSticky)
			tMemOKO			<= UMEM_OK_READY;
		else
			tMemOKO			<= tMemOK4[1:0];
`endif
	
		tMemDataOutO	<= tMemDataOut4;
		tMemOKO			<= tMemOK4[1:0];
		tMemOpSqO		<= tMemOK4[7:4];

	end
	else
	begin
		tMemDataOutO	<= tMemDataOutOL;
		tMemOKO			<= tMemOKOL;
		tMemOpSqO		<= tMemOpSqOL;
	end
`endif

	tMemDataOutOL	<= tMemDataOutO;
	tMemOKOL		<= tMemOKO;
	tMemOpSqOL		<= tMemOpSqO;

	tMemDataInB		<= memDataIn;
	tMemChkInB		<= tMemChkInB2;
	tMemAddrB		<= memAddr;
	tMemAddrSwB		<= memAddrSw;
//	tMemOpmB		<= memOpm;
//	tMemOpmB		<= { ~memOpm, memOpm };
	tMemOpmB		<= { ~memOpSqI, memOpSqI, ~memOpm, memOpm };
end
`endif


always @*
begin
`ifdef mod_ddr_basicbuf
	tMemChkInC2A = tMemAddrC[31:16] ^ tMemAddrC[15:0];
	tMemChkInC0 = tMemDataInC[127:64] ^ tMemDataInC[63:0];
	tMemChkInC1 = tMemChkInC0[63:32] ^ tMemChkInC0[31:0];
	tMemChkInC2 =	{
		tMemOpmC[11:10] ^ tMemOpmC[13:12],
		tMemOpmC[4:3] ^ tMemOpmC[1:0],
		tMemChkInC0[31:16] ^ tMemChkInC0[15:0] ^ tMemChkInC2A };
`endif

`ifdef mod_ddr_extrabuf
	tMemChkInE2A = tMemAddrE[31:16] ^ tMemAddrE[15:0];
	tMemChkInE0 = tMemDataInE[127:64] ^ tMemDataInE[63:0];
	tMemChkInE1 = tMemChkInE0[63:32] ^ tMemChkInE0[31:0];
	tMemChkInE2 =		{
		tMemOpmE[11:10] ^ tMemOpmE[13:12],
		tMemOpmE[4:3] ^ tMemOpmE[1:0],
		tMemChkInE0[31:16] ^ tMemChkInE0[15:0] ^ tMemChkInE2A };
`endif
end

always @*
begin
	tMemDataChk2A0	= tMemDataOut  [127:64] ^ tMemDataOut   [63:0];
	tMemDataChk2A1	= tMemDataChk2A0[63:32] ^ tMemDataChk2A0[31:0];
	tMemDataChk2A	= { tMemOkSq[1:0], tMemOK,
		tMemDataChk2A1[31:16] ^ tMemDataChk2A1[15:0] };
end

// always @(posedge clock)
// always @(posedge clock2)
always @(posedge ddr_coreclock)
begin
`ifdef jx2_mem_ddr32B
//	tMemOK2			<= tMemOKA;
//	tMemOK2			<= { ~tMemOKA, tMemOKA };
	tMemOK2			<= { ~tMemOkSq, tMemOkSq, ~tMemOK, tMemOK };
	tMemDataOut2	<= tMemDataOutA;
//	tMemDataChk2	<= tMemDataOutA;
	tMemDataChk2	<= tMemDataChk2A;
`else
//	tMemOK2			<= tMemOK;
//	tMemOK2			<= { ~tMemOK, tMemOK };
	tMemOK2			<= { ~tMemOkSq, tMemOkSq, ~tMemOK, tMemOK };
	tMemDataOut2	<= tMemDataOut;
	tMemDataChk2	<= tMemDataChk2A;
`endif

	tMemOKL			<= tMemOK;
	tMemOkSqL		<= tMemOkSq;
	tMemDataOutL	<= tMemDataOut;

`ifdef mod_ddr_extrabuf
//	tResetC			<= tResetB;
	tResetC			<= ddr_corereset;
	tResetD			<= tResetC;
	tReset			<= tResetD;

	tMemDataInC		<= tMemDataInB;
	tMemChkInC		<= tMemChkInB;
	tMemAddrC		<= tMemAddrB;
	tMemAddrSwC		<= tMemAddrSwB;
	tMemOpmC		<= tMemOpmB;

	tMemDataInD		<= tMemDataInC;
	tMemChkInD		<= tMemChkInC;
	tMemAddrD		<= tMemAddrC;
	tMemAddrSwD		<= tMemAddrSwC;
	tMemOpmD		<= tMemOpmC;

	tMemDataInE		<= tMemDataInD;
	tMemChkInE		<= tMemChkInD;
	tMemAddrE		<= tMemAddrD;
	tMemAddrSwE		<= tMemAddrSwD;
	tMemOpmE		<= tMemOpmD;

	tMemOpmF		<= tMemOpmE;

//	tCheckD			<= (tMemOpmE == tMemOpmD);

`ifdef jx2_mem_ddr32B
//	if(tMemChkInE == tMemChkInE2)
	if(	(tMemChkInE == tMemChkInE2) &&
		(tMemOpmF[4:0] == (~tMemOpmF[9:5])))
	begin
		tMemDataInA		<= tMemDataInE;
		tMemAddrA		<= tMemAddrE;
		tMemAddrSwA		<= tMemAddrSwE;
//		tMemOpmA		<= tMemOpmE;
		tMemOpmA		<= tMemOpmF[4:0];
	end
`else
//	if((tMemChkInE == tMemChkInE2) &&
//		(tMemOpmF[4:0] == (~tMemOpmF[9:5])))
//	if((tMemChkInE == tMemChkInE2) &&
//		(tMemOpmE[4:0] == (~tMemOpmE[9:5])))
	if((tMemChkInE == tMemChkInE2) &&
		(tMemOpmE[ 4: 0] == (~tMemOpmE[ 9: 5])) &&
		(tMemOpmE[13:10] == (~tMemOpmE[17:14])))
	begin
		tMemDataIn		<= tMemDataInE;
		tMemAddr		<= tMemAddrE;
		tMemAddrSw		<= tMemAddrSwE;
		tMemOpm			<= tMemOpmE[4:0];
		tMemOpSq		<= tMemOpmE[13:10];
//		tMemOpm			<= tMemOpmF[4:0];
	end
`endif

`endif

`ifdef mod_ddr_basicbuf
	tResetC			<= ddr_corereset;
	tResetD			<= tResetC;
	tReset			<= tResetD;

	tMemDataInC		<= tMemDataInB;
	tMemChkInC		<= tMemChkInB;
	tMemAddrC		<= tMemAddrB;
	tMemAddrSwC		<= tMemAddrSwB;
	tMemOpmC		<= tMemOpmB;

	tMemOpmD		<= tMemOpmC;

`ifdef jx2_mem_ddr32B
	tMemDataInA		<= tMemDataInC;
	tMemAddrA		<= tMemAddrC;
	tMemAddrSwA		<= tMemAddrSwC;
//	tMemOpmA		<= tMemOpmC;
	tMemOpmA		<= tMemOpmC[4:0];
//	tMemOpmA		<= tMemOpmD;
	tMemOpSq		<= tMemOpmC[13:10];
`else
//	if((tMemChkInC == tMemChkInC2) &&
//		(tMemOpmD[4:0] == (~tMemOpmD[9:5])))
//	if((tMemChkInC == tMemChkInC2) &&
//		(tMemOpmC[4:0] == (~tMemOpmC[9:5])))
	if(	(tMemChkInC == tMemChkInC2) &&
		(tMemOpmC[ 4: 0] == (~tMemOpmC[ 9: 5])) &&
		(tMemOpmC[13:10] == (~tMemOpmC[17:14])))
	begin
		tMemDataIn		<= tMemDataInC;
		tMemAddr		<= tMemAddrC;
		tMemAddrSw		<= tMemAddrSwC;
		tMemOpm			<= tMemOpmC[4:0];
		tMemOpSq		<= tMemOpmC[13:10];
//		tMemOpm			<= tMemOpmD[4:0];

	end
`endif

`endif

`ifdef mod_ddr_fastbuf

`ifdef jx2_mem_ddr32B
	tMemDataInA		<= memDataIn;
	tMemAddrA		<= memAddr;
	tMemAddrSwA		<= memAddrSw;
	tMemOpmA		<= memOpm;
`else
	tMemDataIn		<= memDataIn;
	tMemAddr		<= memAddr;
	tMemAddrSw		<= memAddrSw;
	tMemOpm			<= memOpm;
	tMemOpSq		<= memOpSqI;
`endif

//	tReset			<= reset;
	tReset			<= ddr_corereset;
`endif

	ddrData2		<= ddrData;
	ddrDqs2			<= ddrDqs;
	ddrData2H		<= ddrData1H;
	ddrDqs2H		<= ddrDqs1H;
	tPhaseHc		<= tNxtPhaseHc;

	accSkipRowClose	<= accNxtSkipRowClose;
	accSkipRowOpen	<= accNxtSkipRowOpen;

`ifdef jx2_mem_ddr32B
	tMemDataOutL	<= tMemDataOut;
	tMemOKL			<= tMemOK;
	tMemOkSqL		<= tMemOkSq;

	tMemDataOutA	<= tNxtMemDataOutA;
	tMemOKA			<= tNxtMemOKA;
	tMemStateA		<= tNxtMemStateA;

	tMemDataIn		<= tNxtMemDataIn;
	tMemAddr		<= tNxtMemAddr;
	tMemAddrSw		<= tNxtMemAddrSw;
	tMemOpm			<= tNxtMemOpm;
`endif

//	ddrData2A		<= ddrData;
//	ddrData2		<= ddrData2A;

	tReqOpSq		<= tNxtReqOpSq;

	accState		<= accNextState;
	accCkLo			<= accNextCkLo;
	accReadBlk		<= accNextReadBlk;
	accStoreFifo		<= accNextStoreFifo;
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

	tDdrClk2		<= tDdrClk;
	tDdrDqs2		<= tDdrDqs;
	tDdrDqs2_En		<= tDdrDqs_En;


	tDdrLastAddr	<= tDdrAddr;	//Address pins
	tDdrLastBa		<= tDdrBa;		//Bank Address pins
	tDdrLastCs		<= tDdrCs;
	tDdrLastRas		<= tDdrRas;
	tDdrLastCas		<= tDdrCas;
	tDdrLastWe		<= tDdrWe;
	tDdrLastCke		<= tDdrCke;

	tBurstCnt		<= tNxtBurstCnt;
	tDoSwap			<= tNxtDoSwap;
	tSwapLoadDn		<= tNxtSwapLoadDn;
	tSwapStLd		<= tNxtSwapStLd;

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
