/*
VGA Module

pwmOut
	[  13]=VSync
	[  12]=HSync
	[11:8]=Red
	[ 7:4]=Green
	[ 3:0]=Blue
 */

module ModVga(
	clock,		reset,
	pixCy,		pixCu,		pixCv,
	ctrlRegVal,	pixAux,
	pwmOut,		pixPosX,	pixPosY,
	pixLineOdd);

/* verilator lint_off UNUSED */

input clock;
input reset;

input[7:0]		pixCy;			//Y Component
input[7:0]		pixCu;			//U Component
input[7:0]		pixCv;			//V Component
input[63:0]		ctrlRegVal;		//Control Register
input[15:0]		pixAux;			//Pixel Aux Bits

output[15:0] pwmOut;

output[11:0] pixPosX;
output[11:0] pixPosY;
output		pixLineOdd;


reg[7:0]	cbPwmTab[31:0];

reg[15:0]	tPwmOut;			//PWM output bits

reg[3:0]	tPwmStR;			//PWM State
reg[3:0]	tPwmNextStR;		//Next PWM State
reg[7:0]	tPwmValR;			//PWM Value
reg[7:0]	tPwmNextValR;		//Next PWM Value
reg			tPwmCarryR;

reg[3:0]	tPwmStG;			//PWM State
reg[3:0]	tPwmNextStG;		//Next PWM State
reg[7:0]	tPwmValG;			//PWM Value
reg[7:0]	tPwmNextValG;		//Next PWM Value
reg			tPwmCarryG;

reg[3:0]	tPwmStB;			//PWM State
reg[3:0]	tPwmNextStB;		//Next PWM State
reg[7:0]	tPwmValB;			//PWM Value
reg[7:0]	tPwmNextValB;		//Next PWM Value
reg			tPwmCarryB;


// reg[21:0]	tCbAcc;				//Colorburst Accumulator
// reg[21:0]	tCbNextAcc;			//Next Colorburst Accumulator

reg[12:0]	tScanPixClk;		//Scan Pixel Clock
reg[12:0]	tScanNextPixClk;	//Next Scan Pixel Clock

reg[10:0]	tScanRowClk;		//Scan Row Clock
reg[10:0]	tScanNextRowClk;	//Next Scan Row Clock

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

reg[7:0]	tPixCy;
reg[7:0]	tPixCu;
reg[7:0]	tPixCv;

assign	pwmOut		= tPwmOut;
assign	pixPosX		= tPixPosX;
assign	pixPosY		= tPixPosY;
assign	pixLineOdd	= tPixLineOdd;

reg			tHsync800;
reg			tVsync600;

reg			tHsync;
reg			tVsync;
reg			tNextHsync;
reg			tNextVsync;

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

always @*
begin
//	pwmIs4bit = 0;

	{tPwmCarryR, tPwmNextStR} = {1'b0, tPwmStR} + {1'b0, tPwmValR[3:0]};
	{tPwmCarryG, tPwmNextStG} = {1'b0, tPwmStG} + {1'b0, tPwmValG[3:0]};
	{tPwmCarryB, tPwmNextStB} = {1'b0, tPwmStB} + {1'b0, tPwmValB[3:0]};

	tPwmOutAR = tPwmValR[7:4];
	tPwmOutAG = tPwmValG[7:4];
	tPwmOutAB = tPwmValB[7:4];
	{tPwmOutCarryR, tPwmOutBR} = {1'b0, tPwmOutAR} + 1;
	{tPwmOutCarryG, tPwmOutBG} = {1'b0, tPwmOutAG} + 1;
	{tPwmOutCarryB, tPwmOutBB} = {1'b0, tPwmOutAB} + 1;
	tPwmOut[11:8] = (tPwmCarryR && !tPwmOutCarryR) ? tPwmOutBR : tPwmOutAR;
	tPwmOut[ 7:4] = (tPwmCarryG && !tPwmOutCarryG) ? tPwmOutBG : tPwmOutAG;
	tPwmOut[ 3:0] = (tPwmCarryB && !tPwmOutCarryB) ? tPwmOutBB : tPwmOutAB;

//	tPwmOut[11:8] = tPwmOutAR;
//	tPwmOut[ 7:4] = tPwmOutAG;
//	tPwmOut[ 3:0] = tPwmOutAB;

	tPwmOut[12] = !tHsync;
	tPwmOut[13] = !tVsync;
	tPwmOut[14] = 0;
	tPwmOut[15] = 0;

	tNextHsync	= 0;
	tNextVsync	= 0;

	tHsync800	= 0;
	tVsync600	= 0;
	
	if(ctrlRegVal[3])
	begin
		tHsync800	= 1;
		tVsync600	= 1;
	end

//	tCbNextAcc			= tCbAcc + 150137;
	tScanNextPixClk		= tScanPixClk + 1;
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

	tPixLineOdd			= tVFieldCnt[0];

	if(1'b1)
	begin
//		tPixNextPosX = tScanPixClk[12:3] - 59;
//		tPixNextPosX = tScanPixClk[11:2] - 59;
		if(tHsync800)
		begin
			tPixNextPosX =
//				tScanPixClk[13:2] +
//				{2'b00, tScanPixClk[13:4]} - 59;
				{1'b0,  tScanPixClk[12:2]} +
				{3'b00, tScanPixClk[12:4]} - 59;

//			tPixNextPosY = tScanNextRowClk[11:0] - 20;
//			tPixNextPosY = tScanNextRowClk[11:0] - 30;
			tPixNextPosY = {1'b0, tScanNextRowClk[10:0]} - 30;
//			tPixNextPosY = tScanNextRowClk[11:0] - 2;
//			tPixNextPosY = tScanNextRowClk[12:1] - 20;
		end
		else
		begin
//			tPixNextPosX = tScanPixClk[13:2] - 59;
//			tPixNextPosX = tScanPixClk[13:2] - 69;
//			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 69;
			tPixNextPosX = {1'b0, tScanPixClk[12:2]} - 79;

//			tPixNextPosY = tScanNextRowClk[11:0] - 20;
//			tPixNextPosY = tScanNextRowClk[11:0] - 30;
			tPixNextPosY = {1'b0, tScanNextRowClk[10:0]} - 30;
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
			tBaseNextCr = tBaseCg + (tBaseCv<<1);
			tBaseNextCb = tBaseCg + (tBaseCu<<1);
			
			tBaseNextCrB	= tBaseCr;
			tBaseNextCgB	= tBaseCgL;
			tBaseNextCbB	= tBaseCb;
		end

		tScPwmCtR[15:0] = 144 * tBaseCrB + 19456;
		tScPwmCtG[15:0] = 144 * tBaseCgB + 19456;
		tScPwmCtB[15:0] = 144 * tBaseCbB + 19456;
	end

	if(tVSyncClk>0)		/* VSync */
	begin
		tScanNextRowClk = 0;
		tNextVsync		= 1;

//		if(tScanPixClk>=3176)
		if(tScanPixClk>=1588)
		begin
			tVSyncNextClk = tVSyncClk - 1;
			tScanNextPixClk = 0;
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
		if(tScanPixClk>=3178)
		begin
			tScanNextRowClk = tScanRowClk + 1;
			tScanNextPixClk = 0;
			
//			if(tScanNextRowClk>=262)
//			if(tScanNextRowClk>=524)
			if(	((tScanNextRowClk>=524) && !tVsync600) ||
				((tScanNextRowClk>=624) && tVsync600))
			begin
				tVFieldNextCnt = tVFieldCnt + 1;
//				tVSyncNextClk = 5;
				tVSyncNextClk = 2;
				tVEqPulseNextClk = 0;
			end
		end
//		else if(tScanPixClk>=5880)
		else if(tScanPixClk>=2940)
		begin
			tPwmNextValR	= 0;
			tPwmNextValG	= 0;
			tPwmNextValB	= 0;
			tNextHsync		= 1;
		end
//		else if((tScanPixClk>=472) && (tScanPixClk<5592))
		else if((tScanPixClk>=236) && (tScanPixClk<2796))
		begin

			tPwmNextValR = tScPwmCtR[15:8];
			tPwmNextValG = tScPwmCtG[15:8];
			tPwmNextValB = tScPwmCtB[15:8];

		end
	end
end

always @ (posedge clock)
begin
	tHsync			<= tNextHsync;
	tVsync			<= tNextVsync;

	tPwmStR			<= tPwmNextStR;
	tPwmStG			<= tPwmNextStG;
	tPwmStB			<= tPwmNextStB;

	tPwmValR		<= tPwmNextValR;
	tPwmValG		<= tPwmNextValG;
	tPwmValB		<= tPwmNextValB;

//	tCbAcc			<= tCbNextAcc;

	tScanPixClk		<= tScanNextPixClk;
	tScanRowClk		<= tScanNextRowClk;

	tVSyncClk		<= tVSyncNextClk;
	tVEqPulseClk	<= tVEqPulseNextClk;
	tVFieldCnt		<= tVFieldNextCnt;

	tPixPosX		<= tPixNextPosX;
	tPixPosY		<= tPixNextPosY;

	tPixCy			<= pixCy;
	tPixCu			<= pixCu;
	tPixCv			<= pixCv;

	tBaseCy			<= tBaseNextCy;
	tBaseCu			<= tBaseNextCu;
	tBaseCv			<= tBaseNextCv;

	tBaseCr			<= tBaseNextCr;
	tBaseCg			<= tBaseNextCg;
	tBaseCb			<= tBaseNextCb;

	tBaseCrL		<= tBaseCr;
	tBaseCgL		<= tBaseCg;
	tBaseCbL		<= tBaseCb;

	tBaseCrB		<= tBaseNextCrB;
	tBaseCgB		<= tBaseNextCgB;
	tBaseCbB		<= tBaseNextCbB;

end

endmodule
