/*
Deal with SPI.
 */

`include "CoreDefs.v"

module ModPs2Kb(
	/* verilator lint_off UNUSED */
	clock,			reset,
	spi_sclk,		spi_mosi,
	spi_miso,		spi_cs,
	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK,			mmioSelfAddr
	);

input			clock;
input			reset;

output			spi_sclk;
input			spi_miso;
output			spi_mosi;
output			spi_cs;

input[31:0]		mmioInData;
output[31:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;
input[11:0]		mmioSelfAddr;

reg[31:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[31:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

reg			tOutMosi2;
reg			tOutMosi;
reg			tOutSclk2;
reg			tOutSclk;

assign	spi_sclk = tOutSclk2;
assign	spi_mosi = tOutMosi2;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

wire		tMmioLowCSel;
assign		tMmioLowCSel = (mmioAddr[27:16]==12'h000);

wire		tMmioSelfCSel;
assign		tMmioSelfCSel = tMmioLowCSel && (mmioAddr[15:12]==mmioSelfAddr);

reg				mmioInOE;
reg				mmioInWR;

reg				mmioLatchWR;
reg				mmioNxtLatchWR;

reg[31:0]		tRegCtrl;
reg[31:0]		tNxtRegCtrl;

reg[15:0]		tRegExch;
reg[15:0]		tNxtRegExch;

reg[13:0]		tDivCnt;
reg[13:0]		tNxtDivCnt;
reg[13:0]		tDivRst;
reg[13:0]		tNxtDivRst;
reg[13:0]		tDivRstH;
reg				tNxtOutSclk;

reg[3:0]		tBitCnt;
reg[3:0]		tNxtBitCnt;

always @*
begin
	tMmioOutData	= UV32_XX;
	tMmioOK			= UMEM_OK_READY;

	mmioInOE		= (mmioOpm[3]) && tMmioSelfCSel;
	mmioInWR		= (mmioOpm[4]) && tMmioSelfCSel;
	
	tNxtRegCtrl		= tRegCtrl;
//	tNxtDivCnt		= tDivCnt - 1;
	tNxtDivCnt		= 0;
	tNxtDivRst		= tDivRst;
	tNxtOutSclk		= tOutSclk;
	tNxtBitCnt		= tBitCnt;
	
	tDivRstH		= { 1'b0, tDivRst[13, 1] };

	if(tBitCnt!=0)
	begin
		tNxtDivCnt		= tDivCnt - 1;

		if(tDivCnt==tDivRstH)
		begin
			tNxtOutSclk = !tOutSclk;
		end
		
		if(tDivCnt==0)
		begin
			tOutMosi	= tRegExch[0];
			tNxtRegExch	= { spi_miso, tRegExch[15:1] };
			tNxtDivCnt	= tDivRst;
			tNxtOutSclk	= 0;
			tNxtBitCnt	= tBitCnt - 1;
		end
	end

	if((mmioAddr[3:2]==2'b00) && mmioInOE)
	begin
		tMmioOutData	= tRegCtrl;
		tMmioOK			= UMEM_OK_OK;
	end

	if((mmioAddr[3:2]==2'b00) && mmioInWR)
	begin
		tNxtRegCtrl		= mmioInData;
		tMmioOK			= UMEM_OK_OK;
	end

	if((mmioAddr[3:2]==2'b01) && mmioInOE)
	begin
		if(tBitCnt==0)
		begin
			tMmioOutData	= { UV24_00, tRegExch[15:8] };
			tMmioOK			= UMEM_OK_OK;
		end
		else
		begin
			tMmioOK			= UMEM_OK_HOLD;
		end
	end

	if((mmioAddr[3:2]==2'b01) && mmioInWR)
	begin
		if(tBitCnt==0)
		begin
			tRegExch[7:0]	= mmioInData[7:0];
			tNxtBitCnt		= 8;
			tMmioOK			= UMEM_OK_OK;
			mmioNxtLatchWR	= 1;
		end
		else
		begin
			mmioNxtLatchWR	= mmioLatchWR;
			tMmioOK			= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
		end
	end
	else
	begin
		mmioNxtLatchWR		= 0;
	end

end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOK2		<= tMmioOK;
	tOutMosi2		<= tOutMosi;
	tOutSclk2		<= tOutSclk;

	tRegCtrl		<= tNxtRegCtrl;
	tRegExch		<= tNxtRegExch;
	tDivCnt			<= tNxtDivCnt;
	tDivRst			<= tNxtDivRst;
	tOutSclk		<= tNxtOutSclk;
	tBitCnt			<= tNxtBitCnt;
	mmioLatchWR		<= mmioNxtLatchWR;
end

endmodule
