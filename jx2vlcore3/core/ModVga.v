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

reg[16:0]	tPwmOut0;			//PWM output bits
reg[16:0]	tPwmOut1;			//PWM output bits
reg[16:0]	tPwmOut2;			//PWM output bits
reg[16:0]	tPwmOut3;			//PWM output bits

reg[7:0]	tPwmValR0;			//PWM Value
reg[7:0]	tPwmNextValR0;		//Next PWM Value
reg[7:0]	tPwmValG0;			//PWM Value
reg[7:0]	tPwmNextValG0;		//Next PWM Value
reg[7:0]	tPwmValB0;			//PWM Value
reg[7:0]	tPwmNextValB0;		//Next PWM Value

reg[7:0]	tPwmValR1;			//PWM Value
reg[7:0]	tPwmNextValR1;		//Next PWM Value
reg[7:0]	tPwmValG1;			//PWM Value
reg[7:0]	tPwmNextValG1;		//Next PWM Value
reg[7:0]	tPwmValB1;			//PWM Value
reg[7:0]	tPwmNextValB1;		//Next PWM Value

reg[7:0]	tPwmValR2;			//PWM Value
reg[7:0]	tPwmNextValR2;		//Next PWM Value
reg[7:0]	tPwmValG2;			//PWM Value
reg[7:0]	tPwmNextValG2;		//Next PWM Value
reg[7:0]	tPwmValB2;			//PWM Value
reg[7:0]	tPwmNextValB2;		//Next PWM Value

reg[7:0]	tPwmValR3;			//PWM Value
reg[7:0]	tPwmNextValR3;		//Next PWM Value
reg[7:0]	tPwmValG3;			//PWM Value
reg[7:0]	tPwmNextValG3;		//Next PWM Value
reg[7:0]	tPwmValB3;			//PWM Value
reg[7:0]	tPwmNextValB3;		//Next PWM Value

reg			tPwmValStrobe;
reg			tPwmNextValStrobe;

reg			tPwmOutStrobe;


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

reg[15:0]	tScPwmCtR0;
reg[15:0]	tScPwmCtG0;
reg[15:0]	tScPwmCtB0;

reg[15:0]	tScPwmCtR1;
reg[15:0]	tScPwmCtG1;
reg[15:0]	tScPwmCtB1;

reg[15:0]	tScPwmCtR2;
reg[15:0]	tScPwmCtG2;
reg[15:0]	tScPwmCtB2;

reg[15:0]	tScPwmCtR3;
reg[15:0]	tScPwmCtG3;
reg[15:0]	tScPwmCtB3;

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
reg			tPixCyStrobe;

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

reg			tBaseCgStrobe;
reg			tBaseNextCgStrobe;
reg			tScPwmCtStrobe;
reg			tBaseCgStrobeB;

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

`ifndef jx2_fbuf_nodither
ModVgaDith	dithR(tBayerIx, tPwmValR[3:0], dithRndUpR);
ModVgaDith	dithG(tBayerIx, tPwmValG[3:0], dithRndUpG);
ModVgaDith	dithB(tBayerIx, tPwmValB[3:0], dithRndUpB);
`endif

`ifdef jx2_fbuf_nodither
assign dithRndUpR = 0;
assign dithRndUpG = 0;
assign dithRndUpB = 0;
`endif


always @*
begin
	tNextPwmEn = 0;

	tPwmOutAR = tPwmValR0[7:4];
	tPwmOutAG = tPwmValG0[7:4];
	tPwmOutAB = tPwmValB0[7:4];
	{tPwmOutCarryR, tPwmOutBR} = {1'b0, tPwmOutAR} + 1;
	{tPwmOutCarryG, tPwmOutBG} = {1'b0, tPwmOutAG} + 1;
	{tPwmOutCarryB, tPwmOutBB} = {1'b0, tPwmOutAB} + 1;

`ifndef jx2_fbuf_nodither
	tPwmOut[14:11] = (dithRndUpR && !tPwmOutCarryR && tPwmEn) ?
		tPwmOutBR : tPwmOutAR;
	tPwmOut[ 9: 6] = (dithRndUpG && !tPwmOutCarryG && tPwmEn) ?
		tPwmOutBG : tPwmOutAG;
	tPwmOut[ 4: 1] = (dithRndUpB && !tPwmOutCarryB && tPwmEn) ?
		tPwmOutBB : tPwmOutAB;
`endif

`ifdef jx2_fbuf_nodither
	tPwmOut0[14:10] = tPwmValR0[7:3];
	tPwmOut0[ 9: 5] = tPwmValG0[7:3];
	tPwmOut0[ 4: 0] = tPwmValB0[7:3];

	tPwmOut1[14:10] = tPwmValR1[7:3];
	tPwmOut1[ 9: 5] = tPwmValG1[7:3];
	tPwmOut1[ 4: 0] = tPwmValB1[7:3];

	tPwmOut2[14:10] = tPwmValR2[7:3];
	tPwmOut2[ 9: 5] = tPwmValG2[7:3];
	tPwmOut2[ 4: 0] = tPwmValB2[7:3];

	tPwmOut3[14:10] = tPwmValR3[7:3];
	tPwmOut3[ 9: 5] = tPwmValG3[7:3];
	tPwmOut3[ 4: 0] = tPwmValB3[7:3];
`endif

	tScanNextRowLim		= tScanRowLim;

	tPwmOut0[15] = !tHsync;
	tPwmOut0[16] = !tVsync;

	tPwmOut1[15] = !tHsync;
	tPwmOut1[16] = !tVsync;

	tPwmOut2[15] = !tHsync;
	tPwmOut2[16] = !tVsync;

	tPwmOut3[15] = !tHsync;
	tPwmOut3[16] = !tVsync;

	tPwmOutStrobe	= tPwmValStrobe;

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
		if(ctrlRegVal[8] && !ctrlRegVal[9])
		begin
			tHsync1024	= 1;
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
			if(ctrlRegVal[9] && !ctrlRegVal[3])
				tScanNextRowLim = 525;
			else
			begin
				tScanNextRowLim = 449;
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

`ifdef jx2_cpu_mmioclock_75
	tScanPixClkP0 = tScanPixClk[12:2];
	tScanPixClkP1 = tScanPixClk[12:2] + 1;
	{ tFraPixClkCarry, tFraNextPixClk }		= {1'b0, tFraPixClk} + 85;
	
	tScanNextPixClk	= {
		tFraPixClkCarry ? tScanPixClkP1 : tScanPixClkP0,
		tFraNextPixClk[7:6] };

	if(tHorzStrobe)
	begin
		tFraNextPixClk		= 0;
		tScanNextPixClk		= 0;
	end
`endif

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
	tBaseNextCgStrobe	= tPixCyStrobe;

	tPwmNextValR0		= 76;	
	tPwmNextValG0		= 76;	
	tPwmNextValB0		= 76;
	
	tPwmNextValR1		= 76;	
	tPwmNextValG1		= 76;	
	tPwmNextValB1		= 76;
	
	tPwmNextValR2		= 76;	
	tPwmNextValG2		= 76;	
	tPwmNextValB2		= 76;
	
	tPwmNextValR3		= 76;	
	tPwmNextValG3		= 76;	
	tPwmNextValB3		= 76;
	
	tScPwmCy			= 0;
	tNextHorzStrobe		= 0;

	tPixLineOdd			= tVFieldCnt[0];

	if(1'b1)
	begin
		if(tHsync1280)
		begin
			tPixNextPosX = {1'b0, tScanPixClk[11:1]} - 76;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 62;
		end
		else
		if(tHsync1024)
		begin
			tPixNextPosX =
				{1'b0,  tScanPixClk[12:2]} +
				{2'b00, tScanPixClk[12:3]} - 79;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 62;
		end
		else
		if(tHsync800)
		begin
			tPixNextPosX =
				{1'b0,  tScanPixClk[12:2]} +
				{3'b00, tScanPixClk[12:4]} - 79;

			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 62;
		end
		else
		begin
			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 76;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 43;
		end

		tBaseNextCg			= { 8'h0, tPixCy };
		tBaseNextCr			= { 8'h0, tPixCv };
		tBaseNextCb			= { 8'h0, tPixCu };
		tBaseNextCgStrobe	= tPixCyStrobe;

//		tBaseNextCrB	= tBaseCrL;
//		tBaseNextCgB	= tBaseCgL;
//		tBaseNextCbB	= tBaseCbL;

		tBaseNextCrB	= tBaseCr;
		tBaseNextCgB	= tBaseCg;
		tBaseNextCbB	= tBaseCb;

//		tScPwmCtR[15:0] = (tBaseCrB << 7) + (tBaseCrB << 5) + 20480;
//		tScPwmCtG[15:0] = (tBaseCgB << 7) + (tBaseCgB << 5) + 20480;
//		tScPwmCtB[15:0] = (tBaseCbB << 7) + (tBaseCbB << 5) + 20480;

		tScPwmCtR0[15:0] = (tBaseCrB << 7) + (tBaseCr << 5) + 20480;
		tScPwmCtG0[15:0] = (tBaseCgB << 7) + (tBaseCg << 5) + 20480;
		tScPwmCtB0[15:0] = (tBaseCbB << 7) + (tBaseCb << 5) + 20480;

		tScPwmCtR1[15:0] = (tBaseCrB << 7) + (tBaseCr << 5) + 20480;
		tScPwmCtG1[15:0] = (tBaseCgB << 7) + (tBaseCg << 5) + 20480;
		tScPwmCtB1[15:0] = (tBaseCbB << 7) + (tBaseCb << 5) + 20480;

		tScPwmCtR2[15:0] = (tBaseCrB << 7) + (tBaseCr << 5) + 20480;
		tScPwmCtG2[15:0] = (tBaseCgB << 7) + (tBaseCg << 5) + 20480;
		tScPwmCtB2[15:0] = (tBaseCbB << 7) + (tBaseCb << 5) + 20480;

		tScPwmCtR3[15:0] = (tBaseCrB << 7) + (tBaseCr << 5) + 20480;
		tScPwmCtG3[15:0] = (tBaseCgB << 7) + (tBaseCg << 5) + 20480;
		tScPwmCtB3[15:0] = (tBaseCbB << 7) + (tBaseCb << 5) + 20480;
		
		tScPwmCtStrobe	= tBaseCgStrobeB;
	end

	tPwmNextValStrobe	= tScPwmCtStrobe;

	if(tVSyncClk>0)		/* VSync */
	begin
		tScanNextRowClk = 0;
		tNextVsync		= 1;

		if((tScanPixClk>=3176) && pixAux[1])
		begin
			tVSyncNextClk = tVSyncClk - 1;
			tScanNextPixClk = 0;
			tNextHorzStrobe	= 1;
		end
		else if(tScanPixClk>=236)
		begin
			tNextHsync		= 1;
		end
	end
	else if(tVEqPulseClk>0)		/* Equalizing Pulse */
	begin
		tScanNextRowClk = 0;
		if(tScanPixClk>=1588)
		begin
			tVEqPulseNextClk = tVEqPulseClk - 1;
			tScanNextPixClk = 0;
			tNextHorzStrobe	= 1;
		end
		else if(tScanPixClk>=1472)
		begin
			tNextHsync		= 1;
		end
	end
	else		/* VBlank or Scanline */
	begin
		if((tScanPixClk>=3176) && pixAux[1])
		begin
			tScanNextRowClk = tScanRowClk + 1;
			tScanNextPixClk = 0;
			tNextHorzStrobe	= 1;

			if(tScanNextRowClk>=tScanRowLim)
			begin
				tVFieldNextCnt = tVFieldCnt + 1;
				tVSyncNextClk = 2;
				tVEqPulseNextClk = 0;
			end
		end
		else if(tScanPixClk>=2944)
		begin
			tNextHsync		= 1;
		end
		else if((tScanPixClk>=240) && (tScanPixClk<2880))
		begin
			tPwmNextValR0	= tScPwmCtR0[15:8];
			tPwmNextValG0	= tScPwmCtG0[15:8];
			tPwmNextValB0	= tScPwmCtB0[15:8];

			tPwmNextValR1	= tScPwmCtR1[15:8];
			tPwmNextValG1	= tScPwmCtG1[15:8];
			tPwmNextValB1	= tScPwmCtB1[15:8];

			tPwmNextValR2	= tScPwmCtR2[15:8];
			tPwmNextValG2	= tScPwmCtG2[15:8];
			tPwmNextValB2	= tScPwmCtB2[15:8];

			tPwmNextValR3	= tScPwmCtR3[15:8];
			tPwmNextValG3	= tScPwmCtG3[15:8];
			tPwmNextValB3	= tScPwmCtB3[15:8];

			tNextPwmEn		= 1;
		end
	end
	
	if(tNextHsync)
	begin
		tPwmNextValR0	= 0;
		tPwmNextValG0	= 0;
		tPwmNextValB0	= 0;

		tPwmNextValR1	= 0;
		tPwmNextValG1	= 0;
		tPwmNextValB1	= 0;

		tPwmNextValR2	= 0;
		tPwmNextValG2	= 0;
		tPwmNextValB2	= 0;

		tPwmNextValR3	= 0;
		tPwmNextValG3	= 0;
		tPwmNextValB3	= 0;
	end

end

reg[16:0]	tPwmOut0A;
reg[16:0]	tPwmOut1A;
reg[16:0]	tPwmOut2A;
reg[16:0]	tPwmOut3A;
reg			tPwmOutStrobeA;

reg[16:0]	tPwmOut0B;
reg[16:0]	tPwmOut1B;
reg[16:0]	tPwmOut2B;
reg[16:0]	tPwmOut3B;
reg			tPwmOutStrobeB;

reg[16:0]	tPwmOut;
reg[1:0]	tPwmState;
reg[1:0]	tNxtPwmState;

always @*
begin
	tNxtPwmState	= tPwmState + 1;
	if(tPwmOutStrobeA && !tPwmOutStrobeB)
		tNxtPwmState	= 0;
	
	case(tPwmState)
		2'b00: tPwmOut=tPwmOut0B;
		2'b01: tPwmOut=tPwmOut1B;
		2'b10: tPwmOut=tPwmOut2B;
		2'b11: tPwmOut=tPwmOut3B;
	endcase
end

always @ (posedge clock_100)
begin
	tPwmOut0A		<= tPwmOut0;
	tPwmOut1A		<= tPwmOut1;
	tPwmOut2A		<= tPwmOut2;
	tPwmOut3A		<= tPwmOut3;
	tPwmOutStrobeA	<= tPwmOutStrobe;

	tPwmOut0B		<= tPwmOut0A;
	tPwmOut1B		<= tPwmOut1A;
	tPwmOut2B		<= tPwmOut2A;
	tPwmOut3B		<= tPwmOut3A;
	tPwmOutStrobeB	<= tPwmOutStrobeA;

	tPwmOutA		<= tPwmOut;
	tPwmOutB		<= tPwmOutA;
	tPwmOutC		<= tPwmOutB;
	tPwmState		<= tNxtPwmState;
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

//	tPwmStR			<= tPwmNextStR;
//	tPwmStG			<= tPwmNextStG;
//	tPwmStB			<= tPwmNextStB;

	tPwmEn			<= tNextPwmEn;

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

	tBaseCy			<= tBaseNextCy;
	tBaseCu			<= tBaseNextCu;
	tBaseCv			<= tBaseNextCv;
	tBaseCuL		<= tBaseCu;
	tBaseCvL		<= tBaseCv;

	tBaseCr			<= tBaseNextCr;
	tBaseCg			<= tBaseNextCg;
	tBaseCb			<= tBaseNextCb;
	tBaseCgStrobe	<= tBaseNextCgStrobe;
	tBaseCgStrobeB	<= tBaseCgStrobe;

	tBaseCrL		<= tBaseCr;
	tBaseCgL		<= tBaseCg;
	tBaseCbL		<= tBaseCb;

	tBaseCrB		<= tBaseNextCrB;
	tBaseCgB		<= tBaseNextCgB;
	tBaseCbB		<= tBaseNextCbB;

	tPwmValStrobe	<= tPwmNextValStrobe;

	if(tPwmNextValStrobe)
	begin
		tPwmValR0		<= tPwmNextValR0;
		tPwmValG0		<= tPwmNextValG0;
		tPwmValB0		<= tPwmNextValB0;

		tPwmValR1		<= tPwmNextValR1;
		tPwmValG1		<= tPwmNextValG1;
		tPwmValB1		<= tPwmNextValB1;

		tPwmValR2		<= tPwmNextValR2;
		tPwmValG2		<= tPwmNextValG2;
		tPwmValB2		<= tPwmNextValB2;

		tPwmValR3		<= tPwmNextValR3;
		tPwmValG3		<= tPwmNextValG3;
		tPwmValB3		<= tPwmNextValB3;
	end

	tPixCyStrobe	<= pixAux[1];
	if(pixAux[1])
	begin
		tBayerIx		<= tNxtBayerIx;

		tPixCy			<= pixCy;
		tPixCu			<= pixCu;
		tPixCv			<= pixCv;
	end

end

endmodule
