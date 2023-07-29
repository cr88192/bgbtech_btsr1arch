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

//input[7:0]		pixCy;			//Y Component
//input[7:0]		pixCu;			//U Component
//input[7:0]		pixCv;			//V Component

`ifdef jx2_modvga_pix4x
input[31:0]		pixCy;			//Y Component
input[31:0]		pixCu;			//U Component
input[31:0]		pixCv;			//V Component
`else
input[15:0]		pixCy;			//Y Component
input[15:0]		pixCu;			//U Component
input[15:0]		pixCv;			//V Component
`endif

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

reg[12:0]	tScanPixClk_236;
reg[12:0]	tScanPixClk_240;
reg[12:0]	tScanPixClk_1472;
reg[12:0]	tScanPixClk_1588;
reg[12:0]	tScanPixClk_2880;
reg[12:0]	tScanPixClk_2944;
reg[12:0]	tScanPixClk_3176;



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

reg[15:0]	tBaseCr0;
reg[15:0]	tBaseCg0;
reg[15:0]	tBaseCb0;
reg[15:0]	tBaseCrL0;
reg[15:0]	tBaseCgL0;
reg[15:0]	tBaseCbL0;

reg[15:0]	tBaseNextCr0;
reg[15:0]	tBaseNextCg0;
reg[15:0]	tBaseNextCb0;

reg[15:0]	tBaseCrB0;
reg[15:0]	tBaseCgB0;
reg[15:0]	tBaseCbB0;

reg[15:0]	tBaseNextCrB0;
reg[15:0]	tBaseNextCgB0;
reg[15:0]	tBaseNextCbB0;


reg[15:0]	tBaseCr1;
reg[15:0]	tBaseCg1;
reg[15:0]	tBaseCb1;
reg[15:0]	tBaseCrL1;
reg[15:0]	tBaseCgL1;
reg[15:0]	tBaseCbL1;

reg[15:0]	tBaseNextCr1;
reg[15:0]	tBaseNextCg1;
reg[15:0]	tBaseNextCb1;

reg[15:0]	tBaseCrB1;
reg[15:0]	tBaseCgB1;
reg[15:0]	tBaseCbB1;

reg[15:0]	tBaseNextCrB1;
reg[15:0]	tBaseNextCgB1;
reg[15:0]	tBaseNextCbB1;

`ifdef jx2_modvga_pix4x

reg[15:0]	tBaseCr2;
reg[15:0]	tBaseCg2;
reg[15:0]	tBaseCb2;
reg[15:0]	tBaseCrL2;
reg[15:0]	tBaseCgL2;
reg[15:0]	tBaseCbL2;

reg[15:0]	tBaseNextCr2;
reg[15:0]	tBaseNextCg2;
reg[15:0]	tBaseNextCb2;

reg[15:0]	tBaseCrB2;
reg[15:0]	tBaseCgB2;
reg[15:0]	tBaseCbB2;

reg[15:0]	tBaseNextCrB2;
reg[15:0]	tBaseNextCgB2;
reg[15:0]	tBaseNextCbB2;


reg[15:0]	tBaseCr3;
reg[15:0]	tBaseCg3;
reg[15:0]	tBaseCb3;
reg[15:0]	tBaseCrL3;
reg[15:0]	tBaseCgL3;
reg[15:0]	tBaseCbL3;

reg[15:0]	tBaseNextCr3;
reg[15:0]	tBaseNextCg3;
reg[15:0]	tBaseNextCb3;

reg[15:0]	tBaseCrB3;
reg[15:0]	tBaseCgB3;
reg[15:0]	tBaseCbB3;

reg[15:0]	tBaseNextCrB3;
reg[15:0]	tBaseNextCgB3;
reg[15:0]	tBaseNextCbB3;

`endif


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

`ifdef jx2_modvga_pix4x

reg[15:0]	tScPwmCtR2;
reg[15:0]	tScPwmCtG2;
reg[15:0]	tScPwmCtB2;

reg[15:0]	tScPwmCtR3;
reg[15:0]	tScPwmCtG3;
reg[15:0]	tScPwmCtB3;

`endif

reg[11:0]	tPixPosX;
reg[11:0]	tPixPosY;
reg[11:0]	tPixNextPosX;
reg[11:0]	tPixNextPosY;
reg			tPixLineOdd;

reg[11:0]	tPixPosXL;
reg[11:0]	tPixPosYL;

//reg[7:0]	tPixCy;
//reg[7:0]	tPixCu;
//reg[7:0]	tPixCv;

`ifdef jx2_modvga_pix4x
reg[31:0]	tPixCy;
reg[31:0]	tPixCu;
reg[31:0]	tPixCv;
`else
reg[15:0]	tPixCy;
reg[15:0]	tPixCu;
reg[15:0]	tPixCv;
`endif

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
reg			tHsync1440;
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

`ifdef jx2_modvga_pix4x
	tPwmOut2[14:10] = tPwmValR2[7:3];
	tPwmOut2[ 9: 5] = tPwmValG2[7:3];
	tPwmOut2[ 4: 0] = tPwmValB2[7:3];

	tPwmOut3[14:10] = tPwmValR3[7:3];
	tPwmOut3[ 9: 5] = tPwmValG3[7:3];
	tPwmOut3[ 4: 0] = tPwmValB3[7:3];
`endif

`endif

	tScanNextRowLim		= tScanRowLim;

	tPwmOut0[15] = !tHsync;
	tPwmOut0[16] = !tVsync;

	tPwmOut1[15] = !tHsync;
	tPwmOut1[16] = !tVsync;

`ifdef jx2_modvga_pix4x
	tPwmOut2[15] = !tHsync;
	tPwmOut2[16] = !tVsync;

	tPwmOut3[15] = !tHsync;
	tPwmOut3[16] = !tVsync;
`endif

	tPwmOutStrobe	= tPwmValStrobe;

	tNextHsync	= 0;
	tNextVsync	= 0;

	tHsync800	= 0;
	tVsync600	= 0;

	tHsync1280	= 0;
	tHsync1440	= 0;
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
		
		if(ctrlRegVal[3])
		begin
			tHsync1440	= 1;
			tScanNextRowLim = 932;
		end
		else
		begin
			tHsync1280	= 1;
			tScanNextRowLim = 849;
			if(ctrlRegVal[9])
			begin
				tScanNextRowLim = 1073;
			end
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
//		tScanNextPixClk		= tScanPixClk + 3;
	end
	if(tPixClockMode==4'h2)
	begin
//		tScanNextPixClk		= tScanPixClk + 4;
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

	tBaseNextCr0		= 0;
	tBaseNextCg0		= 0;
	tBaseNextCb0		= 0;

	tBaseNextCr1		= 0;
	tBaseNextCg1		= 0;
	tBaseNextCb1		= 0;

`ifdef jx2_modvga_pix4x
	tBaseNextCr2		= 0;
	tBaseNextCg2		= 0;
	tBaseNextCb2		= 0;

	tBaseNextCr3		= 0;
	tBaseNextCg3		= 0;
	tBaseNextCb3		= 0;
`endif

	tBaseNextCgStrobe	= tPixCyStrobe;

	tPwmNextValR0		= 76;	
	tPwmNextValG0		= 76;	
	tPwmNextValB0		= 76;
	
	tPwmNextValR1		= 76;	
	tPwmNextValG1		= 76;	
	tPwmNextValB1		= 76;
	
`ifdef jx2_modvga_pix4x
	tPwmNextValR2		= 76;	
	tPwmNextValG2		= 76;	
	tPwmNextValB2		= 76;
	
	tPwmNextValR3		= 76;	
	tPwmNextValG3		= 76;	
	tPwmNextValB3		= 76;
`endif
	
	tScPwmCy			= 0;
	tNextHorzStrobe		= 0;

	tPixLineOdd			= tVFieldCnt[0];

	if(1'b1)
	begin
//		if(tHsync1280)
		if(tHsync1280 || tHsync1440 ||
			tHsync800 || tHsync1024)
		begin
			tPixNextPosX = {1'b0, tScanPixClk[11:1]} - 76;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 62;
		end

`ifndef def_true
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
`endif

		else
		begin
			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 76;
			tPixNextPosY = {1'b0, tScanRowClk[10:0]} - 43;
		end

		tBaseNextCg0			= { 8'h0, tPixCy[7:0] };
		tBaseNextCr0			= { 8'h0, tPixCv[7:0] };
		tBaseNextCb0			= { 8'h0, tPixCu[7:0] };

		tBaseNextCg1			= { 8'h0, tPixCy[15:8] };
		tBaseNextCr1			= { 8'h0, tPixCv[15:8] };
		tBaseNextCb1			= { 8'h0, tPixCu[15:8] };

`ifdef jx2_modvga_pix4x
		tBaseNextCg2			= { 8'h0, tPixCy[23:16] };
		tBaseNextCr2			= { 8'h0, tPixCv[23:16] };
		tBaseNextCb2			= { 8'h0, tPixCu[23:16] };

		tBaseNextCg3			= { 8'h0, tPixCy[31:24] };
		tBaseNextCr3			= { 8'h0, tPixCv[31:24] };
		tBaseNextCb3			= { 8'h0, tPixCu[31:24] };
`endif

		tBaseNextCgStrobe	= tPixCyStrobe;

//		tBaseNextCrB	= tBaseCrL;
//		tBaseNextCgB	= tBaseCgL;
//		tBaseNextCbB	= tBaseCbL;

		tBaseNextCrB0	= tBaseCr0;
		tBaseNextCgB0	= tBaseCg0;
		tBaseNextCbB0	= tBaseCb0;

		tBaseNextCrB1	= tBaseCr1;
		tBaseNextCgB1	= tBaseCg1;
		tBaseNextCbB1	= tBaseCb1;

`ifdef jx2_modvga_pix4x
		tBaseNextCrB2	= tBaseCr2;
		tBaseNextCgB2	= tBaseCg2;
		tBaseNextCbB2	= tBaseCb2;

		tBaseNextCrB3	= tBaseCr3;
		tBaseNextCgB3	= tBaseCg3;
		tBaseNextCbB3	= tBaseCb3;
`endif

//		tScPwmCtR[15:0] = (tBaseCrB << 7) + (tBaseCrB << 5) + 20480;
//		tScPwmCtG[15:0] = (tBaseCgB << 7) + (tBaseCgB << 5) + 20480;
//		tScPwmCtB[15:0] = (tBaseCbB << 7) + (tBaseCbB << 5) + 20480;

		tScPwmCtR0[15:0] = (tBaseCrB0 << 7) + (tBaseCrB0 << 5) + 20480;
		tScPwmCtG0[15:0] = (tBaseCgB0 << 7) + (tBaseCgB0 << 5) + 20480;
		tScPwmCtB0[15:0] = (tBaseCbB0 << 7) + (tBaseCbB0 << 5) + 20480;

		tScPwmCtR1[15:0] = (tBaseCrB1 << 7) + (tBaseCrB1 << 5) + 20480;
		tScPwmCtG1[15:0] = (tBaseCgB1 << 7) + (tBaseCgB1 << 5) + 20480;
		tScPwmCtB1[15:0] = (tBaseCbB1 << 7) + (tBaseCbB1 << 5) + 20480;

`ifdef jx2_modvga_pix4x
		tScPwmCtR2[15:0] = (tBaseCrB2 << 7) + (tBaseCrB2 << 5) + 20480;
		tScPwmCtG2[15:0] = (tBaseCgB2 << 7) + (tBaseCgB2 << 5) + 20480;
		tScPwmCtB2[15:0] = (tBaseCbB2 << 7) + (tBaseCbB2 << 5) + 20480;

		tScPwmCtR3[15:0] = (tBaseCrB3 << 7) + (tBaseCrB3 << 5) + 20480;
		tScPwmCtG3[15:0] = (tBaseCgB3 << 7) + (tBaseCgB3 << 5) + 20480;
		tScPwmCtB3[15:0] = (tBaseCbB3 << 7) + (tBaseCbB3 << 5) + 20480;
`endif
		
		tScPwmCtStrobe	= tBaseCgStrobeB;
	end

	tPwmNextValStrobe	= tScPwmCtStrobe;

	tScanPixClk_236 = 236;
	tScanPixClk_240 = 240;
	tScanPixClk_1472=1472;
	tScanPixClk_1588=1588;
	tScanPixClk_2880=2880;
	tScanPixClk_2944=2944;
	tScanPixClk_3176=3176;

	if(tHsync1024)
	begin
//		tScanPixClk_2880=4416;
//		tScanPixClk_2944=4480;
//		tScanPixClk_3176=4712;

		tScanPixClk_2880=2368;
		tScanPixClk_2944=2432;
		tScanPixClk_3176=2664;
	end

	if(tHsync800)
	begin
//		tScanPixClk_2880=3520;
//		tScanPixClk_2944=3584;
//		tScanPixClk_3176=3816;

		tScanPixClk_2880=1920;
		tScanPixClk_2944=1984;
		tScanPixClk_3176=2216;
	end

	if(tHsync1440)
	begin
//		tScanPixClk_2880=3200;
//		tScanPixClk_2944=3264;
//		tScanPixClk_3176=3469;

		tScanPixClk_2880=1760;
		tScanPixClk_2944=1824;
		tScanPixClk_3176=2056;
	end

	if(tVSyncClk>0)		/* VSync */
	begin
		tScanNextRowClk = 0;
		tNextVsync		= 1;

		if((tScanPixClk>=tScanPixClk_3176) && pixAux[1])
		begin
			tVSyncNextClk = tVSyncClk - 1;
			tScanNextPixClk = 0;
			tNextHorzStrobe	= 1;
		end
		else if(tScanPixClk>=tScanPixClk_236)
		begin
			tNextHsync		= 1;
		end
	end
`ifndef def_true
	else if(tVEqPulseClk>0)		/* Equalizing Pulse */
	begin
		tScanNextRowClk = 0;
		if(tScanPixClk>=tScanPixClk_1588)
		begin
			tVEqPulseNextClk = tVEqPulseClk - 1;
			tScanNextPixClk = 0;
			tNextHorzStrobe	= 1;
		end
		else if(tScanPixClk>=tScanPixClk_1472)
		begin
			tNextHsync		= 1;
		end
	end
`endif
	else		/* VBlank or Scanline */
	begin
		if((tScanPixClk>=tScanPixClk_3176) && pixAux[1])
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
		else if(tScanPixClk>=tScanPixClk_2944)
		begin
			tNextHsync		= 1;
		end
		else if(
			(tScanPixClk>=tScanPixClk_240) &&
			(tScanPixClk< tScanPixClk_2880))
		begin
			tPwmNextValR0	= tScPwmCtR0[15:8];
			tPwmNextValG0	= tScPwmCtG0[15:8];
			tPwmNextValB0	= tScPwmCtB0[15:8];

			tPwmNextValR1	= tScPwmCtR1[15:8];
			tPwmNextValG1	= tScPwmCtG1[15:8];
			tPwmNextValB1	= tScPwmCtB1[15:8];

`ifdef jx2_modvga_pix4x
			tPwmNextValR2	= tScPwmCtR2[15:8];
			tPwmNextValG2	= tScPwmCtG2[15:8];
			tPwmNextValB2	= tScPwmCtB2[15:8];

			tPwmNextValR3	= tScPwmCtR3[15:8];
			tPwmNextValG3	= tScPwmCtG3[15:8];
			tPwmNextValB3	= tScPwmCtB3[15:8];
`endif

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
	
`ifdef jx2_modvga_pix4x
	case(tPwmState)
		2'b00: tPwmOut=tPwmOut0B;
		2'b01: tPwmOut=tPwmOut1B;
		2'b10: tPwmOut=tPwmOut2B;
		2'b11: tPwmOut=tPwmOut3B;
	endcase
`else
	tPwmOut=tPwmState[0] ? tPwmOut1B : tPwmOut0B;
`endif
end

always @ (posedge clock_100)
begin
	tPwmOut0A		<= tPwmOut0;
	tPwmOut1A		<= tPwmOut1;
`ifdef jx2_modvga_pix4x
	tPwmOut2A		<= tPwmOut2;
	tPwmOut3A		<= tPwmOut3;
`endif
	tPwmOutStrobeA	<= tPwmOutStrobe;

	tPwmOut0B		<= tPwmOut0A;
	tPwmOut1B		<= tPwmOut1A;
`ifdef jx2_modvga_pix4x
	tPwmOut2B		<= tPwmOut2A;
	tPwmOut3B		<= tPwmOut3A;
`endif
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

	tBaseCr0		<= tBaseNextCr0;
	tBaseCg0		<= tBaseNextCg0;
	tBaseCb0		<= tBaseNextCb0;
	tBaseCr1		<= tBaseNextCr1;
	tBaseCg1		<= tBaseNextCg1;
	tBaseCb1		<= tBaseNextCb1;

`ifdef jx2_modvga_pix4x
	tBaseCr2		<= tBaseNextCr2;
	tBaseCg2		<= tBaseNextCg2;
	tBaseCb2		<= tBaseNextCb2;
	tBaseCr3		<= tBaseNextCr3;
	tBaseCg3		<= tBaseNextCg3;
	tBaseCb3		<= tBaseNextCb3;
`endif

	tBaseCgStrobe	<= tBaseNextCgStrobe;
	tBaseCgStrobeB	<= tBaseCgStrobe;

//	tBaseCrL		<= tBaseCr;
//	tBaseCgL		<= tBaseCg;
//	tBaseCbL		<= tBaseCb;

	tBaseCrB0		<= tBaseNextCrB0;
	tBaseCgB0		<= tBaseNextCgB0;
	tBaseCbB0		<= tBaseNextCbB0;
	tBaseCrB1		<= tBaseNextCrB1;
	tBaseCgB1		<= tBaseNextCgB1;
	tBaseCbB1		<= tBaseNextCbB1;

`ifdef jx2_modvga_pix4x
	tBaseCrB2		<= tBaseNextCrB2;
	tBaseCgB2		<= tBaseNextCgB2;
	tBaseCbB2		<= tBaseNextCbB2;
	tBaseCrB3		<= tBaseNextCrB3;
	tBaseCgB3		<= tBaseNextCgB3;
	tBaseCbB3		<= tBaseNextCbB3;
`endif

	tPwmValStrobe	<= tPwmNextValStrobe;

	if(tPwmNextValStrobe)
	begin
		tPwmValR0		<= tPwmNextValR0;
		tPwmValG0		<= tPwmNextValG0;
		tPwmValB0		<= tPwmNextValB0;

		tPwmValR1		<= tPwmNextValR1;
		tPwmValG1		<= tPwmNextValG1;
		tPwmValB1		<= tPwmNextValB1;

`ifdef jx2_modvga_pix4x
		tPwmValR2		<= tPwmNextValR2;
		tPwmValG2		<= tPwmNextValG2;
		tPwmValB2		<= tPwmNextValB2;

		tPwmValR3		<= tPwmNextValR3;
		tPwmValG3		<= tPwmNextValG3;
		tPwmValB3		<= tPwmNextValB3;
`endif

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
