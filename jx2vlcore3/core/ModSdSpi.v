/*
Deal with SPI.
 */

`include "CoreDefs.v"

module ModSdSpi(
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
reg			tOutCs2;
reg			tOutCs;
reg			tInMiso;

assign	spi_sclk = tOutSclk2;
assign	spi_mosi = tOutMosi2;
assign	spi_cs = tOutCs2;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

wire		tMmioLowCSel;
assign		tMmioLowCSel = (mmioAddr[27:16]==12'h000);

wire		tMmioSelfCSel;
assign		tMmioSelfCSel = tMmioLowCSel && (mmioAddr[15:4]==mmioSelfAddr);

reg				mmioInOE;
reg				mmioInWR;

reg				mmioLatchWR;
reg				mmioNxtLatchWR;

reg[31:0]		tRegCtrl;
reg[31:0]		tNxtRegCtrl;

//reg[15:0]		tRegExch;
//reg[15:0]		tNxtRegExch;

//reg[7:0]		tRegExchI;
reg[15:0]		tRegExchI;
reg[7:0]		tRegExchO;
//reg[7:0]		tNxtRegExchI;
reg[15:0]		tNxtRegExchI;
reg[7:0]		tNxtRegExchO;
reg[7:0]		tRegValIn;

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
	mmioNxtLatchWR	= mmioInWR && mmioLatchWR;

	
	tNxtRegCtrl		= tRegCtrl;
//	tNxtDivCnt		= tDivCnt - 1;
	tNxtDivCnt		= 0;
	tNxtDivRst		= tDivRst;
	tNxtOutSclk		= tOutSclk;
	tNxtBitCnt		= tBitCnt;
	
	tDivRstH		= { 1'b0, tDivRst[13:1] };

//	tOutCs = tRegCtrl[0];
//	tNxtDivRst = { 2'b00, tRegCtrl[31:27], 7'h00 };
//	tNxtDivRst = { 2'b00, tRegCtrl[31:27], 7'h40 };
	tNxtDivRst = 1000;

	tNxtRegExchI	= tRegExchI;
	tNxtRegExchO	= tRegExchO;
	tOutMosi		= tOutMosi2;
	tOutCs			= tOutCs2;

	if(tBitCnt!=0)
//	if((tBitCnt!=0) || !tOutSclk)
	begin
		tNxtDivCnt		= tDivCnt - 1;

		if(tDivCnt==tDivRstH)
		begin
//			tNxtOutSclk = !tOutSclk;
			tNxtOutSclk = 1;
		end
		
		if(tDivCnt==0)
		begin
			if(tOutSclk)
			begin
//				tOutMosi		= tRegExch[0];
//				tOutMosi		= tRegExch[15];
				tOutMosi		= tRegExchO[7];
//				tNxtRegExch		= { spi_miso, tRegExch[15:1] };
//				tNxtRegExch		= { tRegExch[14:0], spi_miso };
//				tNxtRegExchI	= { tRegExchI[6:0], spi_miso };
//				tNxtRegExchI	= { tRegExchI[6:0], tInMiso };
				tNxtRegExchI	= { tRegExchI[14:0], tInMiso };
				tNxtRegExchO	= { tRegExchO[6:0], 1'b0 };
				tNxtDivCnt		= tDivRst;
				tNxtOutSclk		= 0;
				tNxtBitCnt		= tBitCnt - 1;
				tOutCs			= tRegCtrl[0];
			end
			else
				tNxtOutSclk		= 1;
				tNxtDivCnt		= tDivRstH;
			begin
			end
			
//			$display("SdSpi Bit=%d Mosi=%d Miso=%d ExchI=%X ExchO=%X",
//				tBitCnt, tOutMosi, spi_miso, tRegExchI, tRegExchO);
		end
	end

	if((mmioAddr[3:2]==2'b00) && mmioInOE)
	begin
		tMmioOutData	= tRegCtrl;
		tMmioOutData[1]	= (tBitCnt!=0);
		
		tMmioOK			= UMEM_OK_OK;
	end

	if((mmioAddr[3:2]==2'b00) && mmioInWR)
	begin
		if(tBitCnt==0)
		begin
			tNxtRegCtrl		= mmioInData;
			tMmioOK			= UMEM_OK_OK;
			mmioNxtLatchWR	= 1;

			if(mmioInData[1] && (tBitCnt==0))
			begin
				tNxtBitCnt			= 8;
				tNxtDivCnt			= 0;
			end
		end
		else
		begin
//			mmioNxtLatchWR	= mmioLatchWR;
			tMmioOK			= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
		end
	end

//	tRegValIn = tRegExchI[7:0];
	tRegValIn = tRegExchI[14:7];

	if((mmioAddr[3:2]==2'b01) && mmioInOE)
	begin
		if(tBitCnt==0)
		begin
//			$display("SdSpi Rd=%X", tRegExch[15:8]);
//			tMmioOutData	= { UV24_00, tRegExch[15:8] };
//			$display("SdSpi Rd=%X", tRegExchI[7:0]);
			$display("SdSpi Rd=%X", tRegValIn);
//			tMmioOutData	= { UV24_00, tRegExch[7:0] };
//			tMmioOutData	= { UV24_00, tRegExchI[7:0] };
			tMmioOutData	= { UV24_00, tRegValIn };
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
//			$display("SdSpi Wr=%X", mmioInData[7:0]);

//			tNxtRegExch[7:0]	= mmioInData[7:0];
			tNxtRegExchO[7:0]	= mmioInData[7:0];
//			tNxtRegExch[15:8]	= mmioInData[7:0];
//			tNxtBitCnt			= 8;
			tMmioOK				= UMEM_OK_OK;
			mmioNxtLatchWR		= 1;
		end
		else
		begin
//			mmioNxtLatchWR	= mmioLatchWR;
			tMmioOK			= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
		end
	end
//	else
//	begin
//		mmioNxtLatchWR		= 0;
//	end

end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOK2		<= tMmioOK;
	tOutMosi2		<= tOutMosi;
	tOutSclk2		<= tOutSclk;
	tOutCs2			<= tOutCs;
	tInMiso			<= spi_miso;

	tRegCtrl		<= tNxtRegCtrl;
	tRegExchI		<= tNxtRegExchI;
	tRegExchO		<= tNxtRegExchO;
	tDivCnt			<= tNxtDivCnt;
	tDivRst			<= tNxtDivRst;
	tOutSclk		<= tNxtOutSclk;
	tBitCnt			<= tNxtBitCnt;
	mmioLatchWR		<= mmioNxtLatchWR;
end

endmodule
