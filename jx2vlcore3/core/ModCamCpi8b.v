/*
Camera module input for 8-bit CPI.

Assume a 640x480 module.
Builds a 4-bit 320x200 monochrome framebuffer.
 */

`include "CoreDefs.v"

module ModCamCpi8b(
	/* verilator lint_off UNUSED */
	clock,			reset,

	in_data,		in_clk,
	in_hsync,		in_vsync,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK,
	mmioSelfAddr,	mmioFbufAddr
	);

input			clock;
input			reset;

input[7:0]		in_data;
input			in_clk;
input			in_hsync;
input			in_vsync;

input[63:0]		mmioInData;
output[63:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;
input[11:0]		mmioSelfAddr;
input[11:0]		mmioFbufAddr;

reg[63:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[63:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

reg[63:0]		tMmioInData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

reg[7:0]		tInData;
reg				tInClk;
reg				tInHSync;
reg				tInVSync;
reg				tInClkL;
reg				tInHSyncL;
reg				tInVSyncL;

reg				tInRowStrobe;
reg				tNxtInRowStrobe;
reg				tInRowMask;
reg				tNxtInRowMask;

reg[8:0]		tInRowCount;
reg[8:0]		tNxtInRowSCount;

reg[19:0]		tInCount;
reg[19:0]		tNxtInCount;

reg[15:0]		tInCountG;
reg[15:0]		tNxtInCountG;

reg[63:0]		tInDataWord;
reg[63:0]		tNxtInDataWord;

reg[63:0]		tInDataWordG;
reg[63:0]		tNxtInDataWordG;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

wire		tMmioLowCSel;
assign		tMmioLowCSel = (tMmioAddr[27:16]==12'h000);

wire		tMmioSelfCSel;
assign		tMmioSelfCSel = tMmioLowCSel && (tMmioAddr[15:4]==mmioSelfAddr);

wire		tMmioFbufCSel;
assign		tMmioFbufCSel = (tMmioAddr[27:16]==mmioFbufAddr);

// reg[7:0]		tOutPwm;
// reg[7:0]		tOutPwm2;
// assign		out_pwm4x = tOutPwm2;

reg				mmioInOE;
reg				mmioInWR;

reg				mmioInOE_Fb;

reg				mmioLatchWR;
reg				mmioNxtLatchWR;

reg[63:0]		tFbufArr[4095:0];
reg[63:0]		tFbufData;
reg[11:0]		tFbufIdx;
reg[11:0]		tFbufIdxL;

reg[63:0]		tFbufStData;
reg[11:0]		tFbufStIdx;
reg				tFbufDoSt;

always @*
begin
	tMmioOutData	= UV64_00;
	tMmioOK			= UMEM_OK_READY;

	mmioInOE		= (tMmioOpm[3]) && tMmioSelfCSel;
	mmioInWR		= (tMmioOpm[4]) && tMmioSelfCSel;

	mmioInOE_Fb		= (tMmioOpm[3]) && tMmioFbufCSel;

	tFbufIdx		= tMmioAddr[14:3];

	if(mmioInOE_Fb)
	begin
		tMmioOutData	= tFbufData;
		tMmioOK			= (tFbufIdxL==tFbufIdx) ? UMEM_OK_OK : UMEM_OK_HOLD;
	end

	tNxtInCount			= tInCount;
	tNxtInCountG		= tInCountG;
	tNxtInDataWord		= tInDataWord;
	tNxtInDataWordG		= tInDataWordG;
	tNxtInRowStrobe		= tInRowStrobe;
	tNxtInRowMask		= tInRowMask;
	tNxtInRowCount		= tInRowCount;

	tFbufStData			= UV64_XX;
	tFbufStIdx			= tInCountG[15:4];
	tFbufDoSt			= 0;

	if(tInClk && !tInClkL)
	begin
		tNxtInDataWord		= { tInDataWord[55:0], tInData };
		tNxtInCount			= tInCount + 1;
		
		if((tInCount[1:0]==0) && !tInRowStrobe && tInRowMask)
		begin
			tNxtInDataWordG	= { tInDataWordG[59:0], tNxtInDataWord[10:7] };
			tNxtInCountG	= tInCountG + 1;

			tFbufStData			= tNxtInDataWordG;
			
			if(tNxtInCountG[3:0]==0)
			begin
				tFbufStIdx			= tInCountG[15:4];
				tFbufDoSt			= 1;
			end
		end
	end

	if(!tInHSync && tInHSyncL)
	begin
		tNxtInRowStrobe		= !tInRowStrobe;
		tNxtInRowCount		= tInRowCount + 1;
		tNxtInRowMask		=
			(tInRowCount >= 40) &&
			(tInRowCount <= 240) ;
	end

	if(!tInVSync && tInVSyncL)
	begin
		tNxtInCoun		= 0;
		tNxtInCountG	= 0;
		tNxtInRowStrobe	= 0;
		tNxtInRowCount	= 0;
	end

end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOK2		<= tMmioOK;

	tMmioInData		<= mmioInData;
	tMmioAddr		<= mmioAddr;
	tMmioOpm		<= mmioOpm;
	
	
	tFbufData		<= tFbufArr[tFbufIdx];
	tFbufIdxL		<= tFbufIdx;

	if(tFbufDoSt)
	begin
		tFbufArr[tFbufStIdx]	<= tFbufStData;
	end

	tInData			<= in_data;
	tInClk			<= in_clk;
	tInHSync		<= in_hsync;
	tInVSync		<= in_vsync;
	tInClkL			<= tInClk;
	tInHSyncL		<= tInHSync;
	tInVSyncL		<= tInVSync;

	tInDataWord		<= tNxtInDataWord;
	tInDataWordG	<= tNxtInDataWordG;

	tInCount		<= tNxtInCount;
	tInCountG		<= tNxtInCountG;

	tInRowStrobe	<= tNxtInRowStrobe;
	tInRowMask		<= tNxtInRowMask;
	tInRowCount		<= tNxtInRowCount;

end

endmodule
