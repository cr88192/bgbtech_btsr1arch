/*
Drive a pair of DBI Type-C LCD's.

Goal will be to use a similar interface to the VGA module.

Will drive the displays using alternating pixels.
* Even pixels will go to the left display.
* Odd pixels will go to the right display.

 */
 
 
module ModLcdDbi2xC(
	clock,		reset,
	pixCy,		pixCu,		pixCv,
	ctrlRegVal,	pixAux,
	pwmOut,		pixPosX,	pixPosY,
	pixLineOdd,	timerNoise);

/* verilator lint_off UNUSED */

input clock;
input reset;

input[7:0]		pixCy;			//Y/G Component
input[7:0]		pixCu;			//U/B Component
input[7:0]		pixCv;			//V/R Component
input[63:0]		ctrlRegVal;		//Control Register
input[15:0]		pixAux;			//Pixel Aux Bits
input			timerNoise;

output[15:0]	pwmOut;

output[11:0]	pixPosX;
output[11:0]	pixPosY;
output			pixLineOdd;

reg[7:0]		tPixCy;			//Y/G Component
reg[7:0]		tPixCu;			//U/B Component
reg[7:0]		tPixCv;			//V/R Component
reg[7:0]		tPixAux;		//Pixel Aux Bits
reg[7:0]		tPixAux1;		//Pixel Aux Bits
reg[7:0]		tPixAux2;		//Pixel Aux Bits
reg[7:0]		tPixAux3;		//Pixel Aux Bits

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


reg				tOutSclA;
reg				tOutSdaA;
reg				tOutResA;
reg				tOutDcA;

reg				tOutSclB;
reg				tOutSdaB;
reg				tOutResB;
reg				tOutDcB;

reg				tOutSclA2;
reg				tOutSdaA2;
reg				tOutResA2;
reg				tOutDcA2;

reg				tOutSclB2;
reg				tOutSdaB2;
reg				tOutResB2;
reg				tOutDcB2;

reg[15:0]		tPwmOut;
reg[15:0]		tPwmOut2;

assign		pwmOut = tPwmOut2;

reg[11:0]	tPixPosX;
reg[11:0]	tPixPosY;
reg[11:0]	tNxtPixPosX;
reg[11:0]	tNxtPixPosY;

assign	pixPosX = tPixPosX;
assign	pixPosY = tPixPosY;

reg[3:0]	tDataRov;
reg[3:0]	tNxtDataRov;

reg[15:0]	tDataPixTr;
reg[ 7:0]	tPixAuxTr;
reg[15:0]	tNxtDataPixTr;
reg[ 7:0]	tNxtPixAuxTr;

reg[15:0]	tDataPixL;
reg[15:0]	tDataPixR;
reg[15:0]	tNxtDataPixL;
reg[15:0]	tNxtDataPixR;

reg[7:0]	tDataCmdB;
reg[31:0]	tDataCmdW;
reg[ 3:0]	tDataCmdDcW;
reg[31:0]	tNxtDataCmdW;
reg[ 3:0]	tNxtDataCmdDcW;

always @*
begin
	tOutSclA	= 0;
	tOutSdaA	= 0;
	tOutResA	= 0;
	tOutDcA		= 0;
	tOutSclB	= 0;
	tOutSdaB	= 0;
	tOutResB	= 0;
	tOutDcB		= 0;

	tNxtDataPixL	= tDataPixL;
	tNxtDataPixR	= tDataPixR;
	tNxtPixPosX		= tPixPosX;
	tNxtPixPosY		= tPixPosY;

	tNxtDataPixTr	= tDataPixTr;
	tNxtPixAuxTr	= tPixAuxTr;

	tNxtDataCmdW	= tDataCmdW;
	tNxtDataCmdDcW	= tDataCmdDcW;

	tPwmOut = {
		tOutSclA2,	tOutSdaA2,
		tOutResA2,	tOutDcA2,
		tOutSclB2,	tOutSdaB2,
		tOutResB2,	tOutDcB2
	};

	if(tPixAux[0])
	begin
		tBaseNextCg		= { 8'h0, tPixCy };
		tBaseNextCr		= { 8'h0, tPixCv };
		tBaseNextCb		= { 8'h0, tPixCu };

		tBaseNextCrB	= tBaseCrL;
		tBaseNextCgB	= tBaseCgL;
		tBaseNextCbB	= tBaseCbL;
	end
	else
	begin
		tBaseNextCy		= { 8'h0, tPixCy };
		tBaseNextCu		= { 8'h0, tPixCu } - 128;
		tBaseNextCv		= { 8'h0, tPixCv } - 128;

		tBaseNextCg		= tBaseCy - (tBaseCu>>1) - (tBaseCv>>1);
		tBaseNextCr		= tBaseCg + (tBaseCvL<<1);
		tBaseNextCb		= tBaseCg + (tBaseCuL<<1);
		
		tBaseNextCrB	= tBaseCr;
		tBaseNextCgB	= tBaseCgL;
		tBaseNextCbB	= tBaseCb;
	end
	
	tNxtDataPixTr	= { tBaseCrB[7:3], tBaseCgB[7:2], tBaseCbB[7:3] };
	tNxtPixAuxTr	= tPixAux3;

	
	tNxtDataRov		= tDataRov + 1;
	tDataCmdB		= tDataCmdW[7:0];

	if((tDataCmdB!=0) || tDataCmdDcW[0])
	begin
		tOutDcA			= tDataCmdDcW[0];
		tOutDcB			= tDataCmdDcW[0];
		tOutSdaA		= tDataCmdB[~tDataRov[2:0]];
		tOutSdaB		= tDataCmdB[~tDataRov[2:0]];

		if(tDataRov == 7)
		begin
			tNxtDataCmdW	= { 8'h00, tDataCmdW[31:8] };
			tNxtDataCmdDcW	= { 1'b0, tNxtDataCmdDcW[3:1] };
		end
	end
	else
	begin
		tOutDcA			= 1;
		tOutDcB			= 1;
		tOutSdaA		= tDataPixL[~tDataRov];
		tOutSdaB		= tDataPixR[~tDataRov];

		if(tDataRov == 15)
		begin
			tNxtPixPosX = tPixPosX + 1;

			if(tPixPosX == 239)
			begin
				tNxtPixPosY = tPixPosY + 1;
				tNxtPixPosX = 0;

				if(tPixPosY == 239)
				begin
					tNxtPixPosY = 0;
					tNxtPixPosX = 0;
				end
			end
		end
	end


	if(tPixAuxTr[1])
	begin
		if(tPixAuxTr[2])
			tNxtDataPixL = tDataPixTr;
		else
			tNxtDataPixR = tDataPixTr;
	end

end

always @(posedge clock)
begin
	tPwmOut2	<= tPwmOut;

	tOutSclA2	<= tOutSclA;
	tOutSdaA2	<= tOutSdaA;
	tOutResA2	<= tOutResA;
	tOutDcA2	<= tOutDcA;
	tOutSclB2	<= tOutSclB;
	tOutSdaB2	<= tOutSdaB;
	tOutResB2	<= tOutResB;
	tOutDcB2	<= tOutDcB;

	tDataRov	<= tNxtDataRov;

	tDataPixL	<= tNxtDataPixL;
	tDataPixR	<= tNxtDataPixR;

	tPixPosX	<= tNxtPixPosX;
	tPixPosY	<= tNxtPixPosY;

	tPixCy			<= pixCy;
	tPixCu			<= pixCu;
	tPixCv			<= pixCv;
	tPixAux			<= pixAux[7:0];
	tPixAux1		<= tPixAux;
	tPixAux2		<= tPixAux1;
	tPixAux3		<= tPixAux2;

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

	tDataPixTr		<= tNxtDataPixTr;
	tPixAuxTr		<= tNxtPixAuxTr;

	tDataCmdW		<= tNxtDataCmdW;
	tDataCmdDcW		<= tNxtDataCmdDcW;

end

endmodule
