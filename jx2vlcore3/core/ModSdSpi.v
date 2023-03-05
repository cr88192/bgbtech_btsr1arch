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

input[63:0]		mmioInData;
output[63:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;
input[11:0]		mmioSelfAddr;

reg[63:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[63:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

reg[63:0]		tMmioInData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

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
assign		tMmioLowCSel = (tMmioAddr[27:16]==12'h000);

wire		tMmioSelfCSel;
assign		tMmioSelfCSel = tMmioLowCSel && (tMmioAddr[15:4]==mmioSelfAddr);

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
reg[3:0]		tLastBitCnt;


reg[3:0]		tByteCnt;		//Multi-Byte Transfer
reg[3:0]		tNxtByteCnt;	//Multi-Byte Transfer

reg[63:0]		tRegSendQ;
reg[63:0]		tNxtRegSendQ;

reg[63:0]		tRegRecvQ;
reg[63:0]		tNxtRegRecvQ;

reg				tRegXmitDeb;
reg				tNxtRegXmitDeb;

always @*
begin
	tMmioOutData	= UV64_00;
	tMmioOK			= UMEM_OK_READY;

	mmioInOE		= (tMmioOpm[3]) && tMmioSelfCSel;
	mmioInWR		= (tMmioOpm[4]) && tMmioSelfCSel;
	mmioNxtLatchWR	= mmioInWR && mmioLatchWR;


	tNxtByteCnt		= tByteCnt;
	tNxtRegSendQ	= tRegSendQ;
	tNxtRegRecvQ	= tRegRecvQ;

	tNxtRegXmitDeb	= tRegXmitDeb;

	tNxtRegCtrl		= tRegCtrl;
//	tNxtDivCnt		= tDivCnt - 1;
	tNxtDivCnt		= 0;
	tNxtDivRst		= tDivRst;
	tNxtOutSclk		= tOutSclk;
	tNxtBitCnt		= tBitCnt;
	
	tDivRstH		= { 1'b0, tDivRst[13:1] };

`ifdef jx2_cpu_mmioclock_50

	tNxtDivRst = { 3'b00, tRegCtrl[31:24], 3'h5 };
//	tNxtDivRst = { 3'b00, tRegCtrl[31:24], 3'h4 };
//	tNxtDivRst = { 4'h0, tRegCtrl[31:24], 2'h3 };
//	tNxtDivRst = { 4'h0, tRegCtrl[31:24], 2'h2 };

`else
//	tOutCs = tRegCtrl[0];
//	tNxtDivRst = { 2'b00, tRegCtrl[31:27], 7'h00 };
//	tNxtDivRst = { 2'b00, tRegCtrl[31:27], 7'h40 };
//	tNxtDivRst = { 5'b00, tRegCtrl[31:27], 4'h8 };
//	tNxtDivRst = { 2'b00, tRegCtrl[31:24], 4'h8 };
//	tNxtDivRst = { 2'b00, tRegCtrl[31:24], 4'hA };
//	tNxtDivRst = { 3'b00, tRegCtrl[31:24], 3'h4 };
//	tNxtDivRst = { 3'b00, tRegCtrl[31:24], 3'h6 };
//	tNxtDivRst = 1000;

	tNxtDivRst = { 2'b00, tRegCtrl[31:24], 4'hA };
//	tNxtDivRst = { 2'b00, tRegCtrl[31:24], 4'hD };
//	tNxtDivRst = { 1'b0, tRegCtrl[31:24], 5'h1A };
`endif

	tNxtRegExchI	= tRegExchI;
	tNxtRegExchO	= tRegExchO;
	tOutMosi		= tOutMosi2;
	tOutCs			= tOutCs2;

`ifdef def_true
	if(tDivCnt!=0)
	begin
		tNxtDivCnt		= tDivCnt - 1;
		if(tDivCnt==tDivRstH)
		begin
			tNxtOutSclk = 1;
//			tNxtRegExchI	= { tRegExchI[14:0], tInMiso };
//			tNxtRegExchI	= { tRegExchI[14:0], spi_miso };
		end
	end
`endif

	if(tBitCnt!=0)
//	if((tBitCnt!=0) || !tOutSclk)
	begin
`ifndef def_true
		tNxtDivCnt		= tDivCnt - 1;

		if(tDivCnt==tDivRstH)
		begin
//			tNxtOutSclk = !tOutSclk;
//			tNxtRegExchI	= { tRegExchI[14:0], tInMiso };
			tNxtOutSclk = 1;
		end
`endif

		if(tDivCnt==0)
		begin
`ifdef def_true
			tOutMosi		= tRegExchO[7];
			tNxtRegExchI	= { tRegExchI[14:0], tInMiso };
			tNxtRegExchO	= { tRegExchO[6:0], 1'b0 };
			tNxtDivCnt		= tDivRst;
			tNxtOutSclk		= 0;
			tNxtBitCnt		= tBitCnt - 1;
			tOutCs			= tRegCtrl[0];

//			if(!tOutCs)
//			begin
//				$display("SdSpi: Chip Select Not Set");
//			end
`endif

`ifndef def_true
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
			begin
				tNxtOutSclk		= 1;
				tNxtDivCnt		= tDivRstH;
			end			
`endif

//			$display("SdSpi Bit=%d Mosi=%d Miso=%d ExchI=%X ExchO=%X",
//				tBitCnt, tOutMosi, spi_miso, tRegExchI, tRegExchO);
//			$display("SdSpi Bit=%d Mosi=%d Miso=%d ExchI=%X ExchO=%X",
//				tBitCnt, tOutMosi, tInMiso, tRegExchI, tRegExchO);

		end
	end

//	tRegValIn = tRegExchI[7:0];
	tRegValIn = tRegExchI[14:7];
//	tRegValIn = tRegExchI[15:8];

//	if((tBitCnt==0) && (tLastBitCnt!=0))
	if(tBitCnt==0)
	begin
		if(tByteCnt!=0)
		begin
			tNxtByteCnt			= tByteCnt-1;
//			tNxtBitCnt			= 8;
			tNxtBitCnt			= (tByteCnt!=1) ? 8 : 0;

			tNxtRegExchO[7:0]	= tRegSendQ[7:0];
			tNxtRegSendQ		= { 8'hFF, tRegSendQ[63:8] };
			tNxtRegRecvQ		= { tRegValIn, tNxtRegRecvQ[63:8] };
			
//			$display("SdSpi: Byte: Send=%X Recv=%X",
//				tRegSendQ[7:0], tRegValIn);
			
			//9: zzxxxxxxxxxxxxxx  00
			//8: 00zzxxxxxxxxxxxx  11
			//7: 1100zzxxxxxxxxxx  22
			//6: 221100zzxxxxxxxx  33
			//5: 33221100zzxxxxxx  44
			//4: 4433221100zzxxxx  55
			//3: 554433221100zzxx  66
			//2: 66554433221100zz  77
			//1: 7766554433221100  zz
		end
	end

	if((tMmioAddr[3:2]==2'b00) && mmioInOE)
	begin
//		tMmioOutData	= tRegCtrl;
		tMmioOutData	= { UV32_00, tRegCtrl };
//		tMmioOutData[1]	= (tBitCnt!=0);
		tMmioOutData[1]	= (tBitCnt!=0) || (tByteCnt!=0);
		tNxtRegXmitDeb	= 0;
		
		tMmioOK			= UMEM_OK_OK;
	end

	if((tMmioAddr[3:2]==2'b00) && mmioInWR)
	begin
//		if(tBitCnt==0)
		if((tBitCnt==0) && (tByteCnt==0) && !tRegXmitDeb)
		begin
			tNxtRegCtrl		= tMmioInData[31:0];
//			tMmioOK			= UMEM_OK_OK;
			tMmioOK			= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
			mmioNxtLatchWR	= 1;

//			if(tMmioInData[1] && (tBitCnt==0))
			if(tMmioInData[1])		 /* XMIT 1X */
			begin
				tNxtBitCnt			= 8;
				tNxtDivCnt			= 0;
				tNxtRegXmitDeb		= 1;
			end
			else
				if(tMmioInData[5])	 /* XMIT 8X */
			begin
//				tNxtBitCnt			= 8;
				tNxtDivCnt			= 0;
//				tNxtRegExchO[7:0]	= tRegSendQ[7:0];

				tNxtByteCnt			= 9;
				tNxtRegXmitDeb		= 1;
			end
		end
		else
		begin
//			mmioNxtLatchWR	= mmioLatchWR;
//			tMmioOK			= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
			tMmioOK			= (mmioLatchWR || tRegXmitDeb) ?
				UMEM_OK_OK : UMEM_OK_HOLD;
		end
	end

	if((tMmioAddr[3:2]==2'b01) && mmioInOE)
	begin
		if(tBitCnt==0)
		begin
//			$display("SdSpi Rd=%X", tRegExch[15:8]);
//			$display("SdSpi Rd=%X", tRegExchI[7:0]);
//			$display("SdSpi Rd=%X", tRegValIn);
//			tMmioOutData	= { UV24_00, tRegValIn };
			tMmioOutData	= { UV32_00, UV24_00, tRegValIn };
			tMmioOK			= UMEM_OK_OK;
			tNxtRegXmitDeb	= 0;
		end
		else
		begin
			tMmioOK			= UMEM_OK_HOLD;
		end
	end
	
	if((tMmioAddr[3:2]==2'b01) && mmioInWR)
	begin
		if(tBitCnt==0)
		begin
//			$display("SdSpi Wr=%X", tMmioInData[7:0]);

			tNxtRegExchO[7:0]	= tMmioInData[7:0];
//			tMmioOK				= UMEM_OK_OK;
			tMmioOK				= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
			mmioNxtLatchWR		= 1;
		end
		else
		begin
			tMmioOK			= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
		end
	end
//	else
//	begin
//		mmioNxtLatchWR		= 0;
//	end

	if((tMmioAddr[3:2]==2'b10) && mmioInOE)
	begin
		tMmioOutData	= tRegRecvQ;
		tMmioOK			= UMEM_OK_OK;
		tNxtRegXmitDeb	= 0;
	end

	if((tMmioAddr[3:2]==2'b10) && mmioInWR)
	begin
		tNxtRegSendQ	= tMmioInData;
		mmioNxtLatchWR	= 1;

		tMmioOK			= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
	end

end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOK2		<= tMmioOK;

	tMmioInData		<= mmioInData;
	tMmioAddr		<= mmioAddr;
	tMmioOpm		<= mmioOpm;


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
	tLastBitCnt		<= tBitCnt;
	tByteCnt		<= tNxtByteCnt;

	mmioLatchWR		<= mmioNxtLatchWR;

	tRegSendQ		<= tNxtRegSendQ;
	tRegRecvQ		<= tNxtRegRecvQ;
	tRegXmitDeb		<= tNxtRegXmitDeb;
end

endmodule
