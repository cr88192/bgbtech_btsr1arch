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
VGA Module

pwmOut
/	[  13]=VSync
/	[  12]=HSync
/	[11:8]=Red
/	[ 7:4]=Green
/	[ 3:0]=Blue

pwmOut
	[   16]=VSync
	[   15]=HSync
	[14:10]=Red
	[ 9: 5]=Green
	[ 4: 0]=Blue

 */

`ifndef jx2_fbuf_nodither
`include "ModVgaDith.v"
`endif

module ModVga(
	clock,		reset,		clock_100,
	pixCy,		pixCu,		pixCv,
	ctrlRegVal,	pixAux,
	pwmOut,		pixPosX,	pixPosY,
	pixLineOdd,	timerNoise);

/* verilator lint_off UNUSED */

input clock;
input reset;
input clock_100;

input[7:0]		pixCy;			//Y Component
input[7:0]		pixCu;			//U Component
input[7:0]		pixCv;			//V Component
input[63:0]		ctrlRegVal;		//Control Register
input[15:0]		pixAux;			//Pixel Aux Bits
input			timerNoise;

// output[15:0] pwmOut;
output[16:0] pwmOut;

output[11:0] pixPosX;
output[11:0] pixPosY;
output		pixLineOdd;


reg[7:0]	cbPwmTab[31:0];

reg[16:0]	tPwmOutA;			//PWM output bits
reg[16:0]	tPwmOutB;			//PWM output bits
reg[16:0]	tPwmOutC;			//PWM output bits
reg[16:0]	tPwmOut;			//PWM output bits

//reg[16:0]	tPwmOut1;			//PWM output bits
//reg[16:0]	tPwmOut2;			//PWM output bits
//reg[16:0]	tPwmOut3;			//PWM output bits

reg[4:0]	tPwmStR;			//PWM State
reg[4:0]	tPwmNextStR;		//Next PWM State
reg[7:0]	tPwmValR;			//PWM Value
reg[7:0]	tPwmNextValR;		//Next PWM Value
reg			tPwmCarryR;

reg[4:0]	tPwmStG;			//PWM State
reg[4:0]	tPwmNextStG;		//Next PWM State
reg[7:0]	tPwmValG;			//PWM Value
reg[7:0]	tPwmNextValG;		//Next PWM Value
reg			tPwmCarryG;

reg[4:0]	tPwmStB;			//PWM State
reg[4:0]	tPwmNextStB;		//Next PWM State
reg[7:0]	tPwmValB;			//PWM Value
reg[7:0]	tPwmNextValB;		//Next PWM Value
reg			tPwmCarryB;

reg			tNextPwmEn;
reg			tPwmEn;


// reg[21:0]	tCbAcc;				//Colorburst Accumulator
// reg[21:0]	tCbNextAcc;			//Next Colorburst Accumulator

reg[7:0]	tFraPixClk;			//Fractional Pixel Clock
reg[7:0]	tFraNextPixClk;		//Next Fractional Pixel Clock
reg			tFraPixClkCarry;

reg[12:0]	tScanPixClk;		//Scan Pixel Clock
reg[12:0]	tScanNextPixClk;	//Next Scan Pixel Clock

reg[10:0]	tScanPixClkP0;		//Scan Pixel Clock (+0)
reg[10:0]	tScanPixClkP1;		//Scan Pixel Clock (+1)

reg[10:0]	tScanRowClk;		//Scan Row Clock
reg[10:0]	tScanNextRowClk;	//Next Scan Row Clock

reg[10:0]	tScanRowLim;		//Scan Row Clock
reg[10:0]	tScanNextRowLim;	//Next Scan Row Clock

reg			tHorzStrobe;		//Scan Pixel Clock
reg			tNextHorzStrobe;	//Next Scan Pixel Clock


reg[2:0]	tVSyncClk;
reg[2:0]	tVSyncNextClk;

reg[2:0]	tVEqPulseClk;
reg[2:0]	tVEqPulseNextClk;

reg[1:0]	tVFieldCnt;
reg[1:0]	tVFieldNextCnt;

reg[4:0]	tPhaseCb;
reg[4:0]	tPhaseCu;
reg[4:0]	tPhaseCv;

reg[15:0]	tModCu;
reg[15:0]	tModCv;
reg[15:0]	tModNextCu;
reg[15:0]	tModNextCv;

reg[15:0]	tBaseCy;
reg[15:0]	tBaseCu;
reg[15:0]	tBaseCv;
reg[15:0]	tBaseCuL;
reg[15:0]	tBaseCvL;

reg[15:0]	tBaseNextCy;
reg[15:0]	tBaseNextCu;
reg[15:0]	tBaseNextCv;

reg[15:0]	tBaseCr;
reg[15:0]	tBaseCg;
reg[15:0]	tBaseCb;
reg[15:0]	tBaseCrL;
reg[15:0]	tBaseCgL;
reg[15:0]	tBaseCbL;

reg[15:0]	tBaseNextCr;
reg[15:0]	tBaseNextCg;
reg[15:0]	tBaseNextCb;

reg[15:0]	tBaseCrB;
reg[15:0]	tBaseCgB;
reg[15:0]	tBaseCbB;

reg[15:0]	tBaseNextCrB;
reg[15:0]	tBaseNextCgB;
reg[15:0]	tBaseNextCbB;

reg[17:0]	tScPwmCy;
reg[15:0]	tScPwmCu;
reg[15:0]	tScPwmCv;

// reg[9:0]	tScPwmCtR;
// reg[9:0]	tScPwmCtG;
// reg[9:0]	tScPwmCtB;

reg[15:0]	tScPwmCtR;
reg[15:0]	tScPwmCtG;
reg[15:0]	tScPwmCtB;

reg[11:0]	tPixPosX;
reg[11:0]	tPixPosY;
reg[11:0]	tPixNextPosX;
reg[11:0]	tPixNextPosY;
reg			tPixLineOdd;

reg[11:0]	tPixPosXL;
reg[11:0]	tPixPosYL;

reg[7:0]	tPixCy;
reg[7:0]	tPixCu;
reg[7:0]	tPixCv;

// assign	pwmOut		= tPwmOut;
assign	pwmOut		= tPwmOutC;
// assign	pwmOut		= tPwmOutA;

assign	pixPosX		= tPixPosX;
assign	pixPosY		= tPixPosY;
assign	pixLineOdd	= tPixLineOdd;

reg			tHsync800;
reg			tVsync600;

reg			tHsync1280;
reg			tHsync1024;
reg			tVsync768;

reg[3:0]	tPixClockMode;

reg			tHsync;
reg			tVsync;
reg			tNextHsync;
reg			tNextVsync;

reg			tHsyncL;

reg[3:0]	tPwmOutAR;
reg[3:0]	tPwmOutBR;
reg			tPwmOutCarryR;

reg[3:0]	tPwmOutAG;
reg[3:0]	tPwmOutBG;
reg			tPwmOutCarryG;

reg[3:0]	tPwmOutAB;
reg[3:0]	tPwmOutBB;
reg			tPwmOutCarryB;

//reg			pwmIs4bit;

// reg[1:0]	dithPixXor;
// reg			dithRndUpR;
// reg			dithRndUpG;
// reg			dithRndUpB;

wire		dithRndUpR;
wire		dithRndUpG;
wire		dithRndUpB;

reg[3:0]	tBayerIx;
// reg[3:0]	tNxtBayerIx;

wire[3:0]	tNxtBayerIx;
assign	tNxtBayerIx =
	{ tPixPosY[1:0], tPixPosX[1:0] } ^ { tVFieldCnt, tVFieldCnt };
// assign	tNxtBayerIx = { tPixPosY[1:0], tPixPosX[1:0] };

`ifndef def_true
ModVgaDith	dithR(
	tPixPosX[1:0], tPixPosY[1:0], tVFieldCnt,
	tPwmValR[3:0], dithRndUpR);
ModVgaDith	dithG(
	tPixPosX[1:0], tPixPosY[1:0], tVFieldCnt,
	tPwmValG[3:0], dithRndUpG);
ModVgaDith	dithB(
	tPixPosX[1:0], tPixPosY[1:0], tVFieldCnt,
	tPwmValB[3:0], dithRndUpB);
`endif

// `ifdef def_true
// `ifndef def_true
`ifndef jx2_fbuf_nodither
ModVgaDith	dithR(tBayerIx, tPwmValR[3:0], dithRndUpR);
ModVgaDith	dithG(tBayerIx, tPwmValG[3:0], dithRndUpG);
ModVgaDith	dithB(tBayerIx, tPwmValB[3:0], dithRndUpB);
`endif

// `ifdef def_true
// `ifndef def_true
`ifdef jx2_fbuf_nodither

assign dithRndUpR = 0;
assign dithRndUpG = 0;
assign dithRndUpB = 0;

`endif


always @*
begin
//	pwmIs4bit = 0;
	tNextPwmEn = 0;

`ifndef def_true
	{tPwmCarryR, tPwmNextStR} = {1'b0, tPwmStR} +
		{1'b0, tPwmValR[3:0], timerNoise};
	{tPwmCarryG, tPwmNextStG} = {1'b0, tPwmStG} +
		{1'b0, tPwmValG[3:0], timerNoise};
	{tPwmCarryB, tPwmNextStB} = {1'b0, tPwmStB} +
		{1'b0, tPwmValB[3:0], timerNoise};
`endif

// `ifdef def_true
`ifndef def_true
	case(tVFieldCnt)
		2'b00: tNxtBayerIx = {
			 tPixPosY[1],  tPixPosY[0],
			 tPixPosX[1],  tPixPosX[0] };
		2'b01: tNxtBayerIx = {
			 tPixPosY[1], !tPixPosY[0],
			!tPixPosX[1],  tPixPosX[0] };
		2'b10: tNxtBayerIx = {
			!tPixPosY[1], !tPixPosY[0],
			!tPixPosX[1], !tPixPosX[0] };
		2'b11: tNxtBayerIx = {
			!tPixPosY[1],  tPixPosY[0],
			 tPixPosX[1], !tPixPosX[0] };
	endcase
//assign	tNxtBayerIx =
//	{ tPixPosY[1:0], tPixPosX[1:0] } ^ { tVFieldCnt, tVFieldCnt };
`endif

	tPwmOutAR = tPwmValR[7:4];
	tPwmOutAG = tPwmValG[7:4];
	tPwmOutAB = tPwmValB[7:4];
	{tPwmOutCarryR, tPwmOutBR} = {1'b0, tPwmOutAR} + 1;
	{tPwmOutCarryG, tPwmOutBG} = {1'b0, tPwmOutAG} + 1;
	{tPwmOutCarryB, tPwmOutBB} = {1'b0, tPwmOutAB} + 1;

`ifndef def_true
// `ifdef def_true
	tPwmOut[11:8] = (tPwmCarryR && !tPwmOutCarryR && tPwmEn) ?
		tPwmOutBR : tPwmOutAR;
	tPwmOut[ 7:4] = (tPwmCarryG && !tPwmOutCarryG && tPwmEn) ?
		tPwmOutBG : tPwmOutAG;
	tPwmOut[ 3:0] = (tPwmCarryB && !tPwmOutCarryB && tPwmEn) ?
		tPwmOutBB : tPwmOutAB;
// `else
`endif

`ifndef def_true
	tPwmOut[11:8] = tPwmOutAR;
	tPwmOut[ 7:4] = tPwmOutAG;
	tPwmOut[ 3:0] = tPwmOutAB;
`endif

// `ifdef def_true
`ifndef def_true
	tPwmOut[11:8] = (dithRndUpR && !tPwmOutCarryR && tPwmEn) ?
		tPwmOutBR : tPwmOutAR;
	tPwmOut[ 7:4] = (dithRndUpG && !tPwmOutCarryG && tPwmEn) ?
		tPwmOutBG : tPwmOutAG;
	tPwmOut[ 3:0] = (dithRndUpB && !tPwmOutCarryB && tPwmEn) ?
		tPwmOutBB : tPwmOutAB;
`endif

// `ifdef def_true
// `ifndef def_true
`ifndef jx2_fbuf_nodither
	tPwmOut[14:11] = (dithRndUpR && !tPwmOutCarryR && tPwmEn) ?
		tPwmOutBR : tPwmOutAR;
	tPwmOut[ 9: 6] = (dithRndUpG && !tPwmOutCarryG && tPwmEn) ?
		tPwmOutBG : tPwmOutAG;
	tPwmOut[ 4: 1] = (dithRndUpB && !tPwmOutCarryB && tPwmEn) ?
		tPwmOutBB : tPwmOutAB;
`endif

`ifdef jx2_fbuf_nodither
	tPwmOut[14:10] = tPwmValR[7:3];
	tPwmOut[ 9: 5] = tPwmValG[7:3];
	tPwmOut[ 4: 0] = tPwmValB[7:3];
`endif

//	tPwmOut1=tPwmOut;
//	tPwmOut2=tPwmOut;
//	tPwmOut3=tPwmOut;

	tScanNextRowLim		= tScanRowLim;

//	tPwmOut[12] = !tHsync;
//	tPwmOut[13] = !tVsync;
//	tPwmOut[14] = 0;
//	tPwmOut[15] = 0;

	tPwmOut[15] = !tHsync;
	tPwmOut[16] = !tVsync;

	tNextHsync	= 0;
	tNextVsync	= 0;

	tHsync800	= 0;
	tVsync600	= 0;

	tHsync1280	= 0;
	tHsync1024	= 0;
	tVsync768	= 0;
	
	tPixClockMode	= ctrlRegVal[23:20];
	
	if(ctrlRegVal[25:24]==2'b00)
	begin
	//	if(ctrlRegVal[8])
		if(ctrlRegVal[8] && !ctrlRegVal[9])
		begin
			tHsync1024	= 1;
	//		tVsync768	= 1;
	//		if(ctrlRegVal[3])
			if(!ctrlRegVal[3])
				tScanNextRowLim = 817;
			else
				tScanNextRowLim = 547;
		end
		else if(ctrlRegVal[3] && !ctrlRegVal[9])
		begin
			tHsync800	= 1;
			tVsync600	= 1;
			tScanNextRowLim = 625;
		end
		else
		begin
	//		tScanNextRowLim = 525;
	//		if(ctrlRegVal[9])
			if(ctrlRegVal[9] && !ctrlRegVal[3])
				tScanNextRowLim = 525;
			else
			begin
				tScanNextRowLim = 449;
				
	//			if(ctrlRegVal[8])
	//			begin
	//				tHsync1280	= 1;
	//				tScanNextRowLim = 817;
	//			end
			end
		end
	end

	if(ctrlRegVal[25:24]==2'b01)
	begin
		tHsync1280	= 1;
		tScanNextRowLim = 849;
		if(ctrlRegVal[9])
		begin
			tScanNextRowLim = 1073;
		end
	end

//	tHsync800	= 1;

`ifdef jx2_cpu_mmioclock_75
	tScanPixClkP0 = tScanPixClk[12:2];
	tScanPixClkP1 = tScanPixClk[12:2] + 1;
	{ tFraPixClkCarry, tFraNextPixClk }		= {1'b0, tFraPixClk} + 85;
	
//	if(tHsyncL^tHsync)
//	if(tHsync && !tHsyncL)
//		{ tFraPixClkCarry, tFraNextPixClk }	= 0;
	
	tScanNextPixClk	= {
		tFraPixClkCarry ? tScanPixClkP1 : tScanPixClkP0,
		tFraNextPixClk[7:6] };

	if(tHorzStrobe)
	begin
		tFraNextPixClk		= 0;
		tScanNextPixClk		= 0;
	end
`endif

//	tCbNextAcc			= tCbAcc + 150137;

`ifdef jx2_cpu_mmioclock_100
	tScanNextPixClk		= tScanPixClk + 1;
	tFraNextPixClk		= 0;
`endif

`ifdef jx2_cpu_mmioclock_50
	tScanNextPixClk		= tScanPixClk + 2;
	tFraNextPixClk		= 0;
	
	if(tPixClockMode==4'h1)
	begin
		tScanNextPixClk		= tScanPixClk + 3;
	end
	if(tPixClockMode==4'h2)
	begin
		tScanNextPixClk		= tScanPixClk + 4;
	end
	
`endif

	tScanNextRowClk		= tScanRowClk;
	tVSyncNextClk		= tVSyncClk;
	tVFieldNextCnt		= tVFieldCnt;
	tVEqPulseNextClk	= tVEqPulseClk;
	
	tPixNextPosX		= 0;
	tPixNextPosY		= 0;

	tModNextCu			= 0;
	tModNextCv			= 0;
	tBaseNextCu			= 0;
	tBaseNextCv			= 0;
	tBaseNextCy			= 0;

	tBaseNextCr			= 0;
	tBaseNextCg			= 0;
	tBaseNextCb			= 0;

	tPwmNextValR		= 76;	
	tPwmNextValG		= 76;	
	tPwmNextValB		= 76;
	
	tScPwmCy			= 0;
	tNextHorzStrobe		= 0;

	tPixLineOdd			= tVFieldCnt[0];

	if(1'b1)
	begin
//		tPixNextPosX = tScanPixClk[12:3] - 59;
//		tPixNextPosX = tScanPixClk[11:2] - 59;

		if(tHsync1280)
		begin
			tPixNextPosX = {1'b0, tScanPixClk[11:1]} - 76;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 62;
		end
		else
		if(tHsync1024)
		begin
//			tPixNextPosX =
//				{1'b0,  tScanPixClk[11:1]} - 79;
			tPixNextPosX =
				{1'b0,  tScanPixClk[12:2]} +
				{2'b00, tScanPixClk[12:3]} - 79;
//			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 30;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 62;
		end
		else
		if(tHsync800)
		begin
			tPixNextPosX =
//				tScanPixClk[13:2] +
//				{2'b00, tScanPixClk[13:4]} - 59;
				{1'b0,  tScanPixClk[12:2]} +
//				{3'b00, tScanPixClk[12:4]} - 59;
				{3'b00, tScanPixClk[12:4]} - 79;

//			tPixNextPosY = tScanNextRowClk[11:0] - 20;
//			tPixNextPosY = tScanNextRowClk[11:0] - 30;
//			tPixNextPosY = {1'b0, tScanNextRowClk[10:0]} - 30;
//			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 30;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 62;
//			tPixNextPosY = tScanNextRowClk[11:0] - 2;
//			tPixNextPosY = tScanNextRowClk[12:1] - 20;
		end
		else
		begin
//			tPixNextPosX = tScanPixClk[13:2] - 59;
//			tPixNextPosX = tScanPixClk[13:2] - 69;
//			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 69;
//			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 79;
			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 76;
//			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 72;
//			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 83;
//			tPixNextPosX = {1'b0, tScanPixClk[12:2]} +
//				{8'b00000000, tScanPixClk[12:9]} - 79;

//			tPixNextPosY = tScanNextRowClk[11:0] - 20;
//			tPixNextPosY = tScanNextRowClk[11:0] - 30;
//			tPixNextPosY = {1'b0, tScanNextRowClk[10:0]} - 30;
//			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 30;
//			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 62;
//			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 74;

//			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 47;
//			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 41;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 43;

//			tPixNextPosY = tScanNextRowClk[11:0] - 2;
//			tPixNextPosY = tScanNextRowClk[11:0] - 3;
//			tPixNextPosY = tScanNextRowClk[12:1] - 20;
		end

		if(pixAux[0])
		begin
			tBaseNextCg	= { 8'h0, tPixCy };
			tBaseNextCr = { 8'h0, tPixCv };
			tBaseNextCb = { 8'h0, tPixCu };

			tBaseNextCrB	= tBaseCrL;
			tBaseNextCgB	= tBaseCgL;
			tBaseNextCbB	= tBaseCbL;
		end
		else
		begin
			tBaseNextCy	= { 8'h0, tPixCy };
			tBaseNextCu = { 8'h0, tPixCu } - 128;
			tBaseNextCv = { 8'h0, tPixCv } - 128;

			tBaseNextCg	= tBaseCy - (tBaseCu>>1) - (tBaseCv>>1);
			tBaseNextCr = tBaseCg + (tBaseCvL<<1);
			tBaseNextCb = tBaseCg + (tBaseCuL<<1);
			
			tBaseNextCrB	= tBaseCr;
			tBaseNextCgB	= tBaseCgL;
			tBaseNextCbB	= tBaseCb;
		end

//		tScPwmCtR[15:0] = 144 * tBaseCrB + 19456;
//		tScPwmCtG[15:0] = 144 * tBaseCgB + 19456;
//		tScPwmCtB[15:0] = 144 * tBaseCbB + 19456;

//		tScPwmCtR[15:0] = (tBaseCrB << 7) + 19456;
//		tScPwmCtG[15:0] = (tBaseCgB << 7) + 19456;
//		tScPwmCtB[15:0] = (tBaseCbB << 7) + 19456;

//		tScPwmCtR[15:0] = (tBaseCrB * 160) + 20480;
//		tScPwmCtG[15:0] = (tBaseCgB * 160) + 20480;
//		tScPwmCtB[15:0] = (tBaseCbB * 160) + 20480;

		tScPwmCtR[15:0] = (tBaseCrB << 7) + (tBaseCrB << 5) + 20480;
		tScPwmCtG[15:0] = (tBaseCgB << 7) + (tBaseCgB << 5) + 20480;
		tScPwmCtB[15:0] = (tBaseCbB << 7) + (tBaseCbB << 5) + 20480;
	end

	if(tVSyncClk>0)		/* VSync */
	begin
		tScanNextRowClk = 0;
		tNextVsync		= 1;

//		if(tScanPixClk>=3176)
//		if(tScanPixClk>=1588)
//		if((tScanPixClk>=1588) && pixAux[1])
//		if((tScanPixClk>=3173) && pixAux[1])
//		if((tScanPixClk>=3175) && pixAux[1])
		if((tScanPixClk>=3176) && pixAux[1])
		begin
//			if(pixAux[1])
			tVSyncNextClk = tVSyncClk - 1;
			tScanNextPixClk = 0;
			tNextHorzStrobe	= 1;
		end
//		else if(tScanPixClk>=472)
		else if(tScanPixClk>=236)
		begin
			tPwmNextValR	= 0;
			tPwmNextValG	= 0;
			tPwmNextValB	= 0;
//			tNextVsync		= 1;
			tNextHsync		= 1;
		end
	end
	else if(tVEqPulseClk>0)		/* Equalizing Pulse */
	begin
		tScanNextRowClk = 0;
//		if(tScanPixClk>=3176)
		if(tScanPixClk>=1588)
		begin
			tVEqPulseNextClk = tVEqPulseClk - 1;
			tScanNextPixClk = 0;
			tNextHorzStrobe	= 1;
		end
//		else if(tScanPixClk>=2944)
		else if(tScanPixClk>=1472)
		begin
			tPwmNextValR	= 0;
			tPwmNextValG	= 0;
			tPwmNextValB	= 0;
			tNextHsync		= 1;
		end
	end
	else		/* VBlank or Scanline */
	begin
//		if(tScanPixClk>=6352)
//		if(tScanPixClk>=3178)
//		if((tScanPixClk>=3178) && pixAux[1])
//		if((tScanPixClk>=3173) && pixAux[1])
//		if((tScanPixClk>=3175) && pixAux[1])
		if((tScanPixClk>=3176) && pixAux[1])
//		if(tScanPixClk>=3200)
		begin
//			if(pixAux[1])
//			begin
				tScanNextRowClk = tScanRowClk + 1;
				tScanNextPixClk = 0;
				tNextHorzStrobe	= 1;
//			end
			
//			if(tScanNextRowClk>=262)
//			if(tScanNextRowClk>=524)
//			if(	((tScanNextRowClk>=524) && !tVsync600) ||
//				((tScanNextRowClk>=624) && tVsync600))
//			if(	((tScanNextRowClk>=525) && !tVsync600) ||
//				((tScanNextRowClk>=625) && tVsync600))

			if(tScanNextRowClk>=tScanRowLim)

//			if(	((tScanNextRowClk>=526) && !tVsync600) ||
//				((tScanNextRowClk>=626) && tVsync600))
			begin
				tVFieldNextCnt = tVFieldCnt + 1;
//				tVSyncNextClk = 5;
				tVSyncNextClk = 2;
				tVEqPulseNextClk = 0;
			end
		end
//		else if(tScanPixClk>=5880)
//		else if(tScanPixClk>=2940)
		else if(tScanPixClk>=2944)
		begin
			tPwmNextValR	= 0;
			tPwmNextValG	= 0;
			tPwmNextValB	= 0;
			tNextHsync		= 1;
		end
//		else if((tScanPixClk>=472) && (tScanPixClk<5592))
//		else if((tScanPixClk>=236) && (tScanPixClk<2796))
//		else if((tScanPixClk>=240) && (tScanPixClk<2816))
		else if((tScanPixClk>=240) && (tScanPixClk<2880))
//		else if(tScanPixClk>=240)
		begin
			tPwmNextValR	= tScPwmCtR[15:8];
			tPwmNextValG	= tScPwmCtG[15:8];
			tPwmNextValB	= tScPwmCtB[15:8];

			tNextPwmEn		= 1;
		end
		else
		begin
			tPwmNextValR	= 76;
			tPwmNextValG	= 76;
			tPwmNextValB	= 76;
		end
	end
end

always @ (posedge clock_100)
begin
	tPwmOutA	<= tPwmOut;
	tPwmOutB	<= tPwmOutA;
	tPwmOutC	<= tPwmOutB;
end

always @ (posedge clock)
begin
//	tPwmOutA	<= tPwmOut;
//	tPwmOutB	<= tPwmOutA;
//	tPwmOutC	<= tPwmOutB;

	tHsyncL			<= tHsync;
	tHsync			<= tNextHsync;
	tVsync			<= tNextVsync;
	tHorzStrobe		<= tNextHorzStrobe;

	tPwmStR			<= tPwmNextStR;
	tPwmStG			<= tPwmNextStG;
	tPwmStB			<= tPwmNextStB;

//	tPwmValR		<= tPwmNextValR;
//	tPwmValG		<= tPwmNextValG;
//	tPwmValB		<= tPwmNextValB;
	tPwmEn			<= tNextPwmEn;

//	tCbAcc			<= tCbNextAcc;

	tFraPixClk		<= tFraNextPixClk;
	tScanPixClk		<= tScanNextPixClk;
	tScanRowClk		<= tScanNextRowClk;
	tScanRowLim		<= tScanNextRowLim;

	tVSyncClk		<= tVSyncNextClk;
	tVEqPulseClk	<= tVEqPulseNextClk;
	tVFieldCnt		<= tVFieldNextCnt;

	tPixPosX		<= tPixNextPosX;
	tPixPosY		<= tPixNextPosY;

	tPixPosXL		<= tPixPosX;
	tPixPosYL		<= tPixPosY;

//	tPixCy			<= pixCy;
//	tPixCu			<= pixCu;
//	tPixCv			<= pixCv;

	tBaseCy			<= tBaseNextCy;
	tBaseCu			<= tBaseNextCu;
	tBaseCv			<= tBaseNextCv;
	tBaseCuL		<= tBaseCu;
	tBaseCvL		<= tBaseCv;

	tBaseCr			<= tBaseNextCr;
	tBaseCg			<= tBaseNextCg;
	tBaseCb			<= tBaseNextCb;

	tBaseCrL		<= tBaseCr;
	tBaseCgL		<= tBaseCg;
	tBaseCbL		<= tBaseCb;

	tBaseCrB		<= tBaseNextCrB;
	tBaseCgB		<= tBaseNextCgB;
	tBaseCbB		<= tBaseNextCbB;

	if(pixAux[1])
	begin
		tBayerIx		<= tNxtBayerIx;

		tPixCy			<= pixCy;
		tPixCu			<= pixCu;
		tPixCv			<= pixCv;

		tPwmValR		<= tPwmNextValR;
		tPwmValG		<= tPwmNextValG;
		tPwmValB		<= tPwmNextValB;
	end

end

endmodule
