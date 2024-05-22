/*
Deal with SPI.
 */

`include "CoreDefs.v"

module ModSdSpi(
	/* verilator lint_off UNUSED */
	clock,			reset,
	spi_sclk,		spi_mosi,
	spi_miso,		spi_cs,			spi_mode,
	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK,			mmioSelfAddr
	);

input			clock;
input			reset;

output			spi_sclk;
input[3:0]		spi_miso;
output[3:0]		spi_mosi;
output			spi_cs;
output[1:0]		spi_mode;

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

reg[3:0]	tOutMosi2;
reg[3:0]	tOutMosi;
reg			tOutSclk2;
reg			tOutSclk;
reg			tOutCs2;
reg			tOutCs;
// reg		tInMiso;
reg[3:0]	tInMiso;

assign	spi_sclk = tOutSclk2;
assign	spi_mosi = tOutMosi2;
// assign	spi_mosi = { 3'bzzz, tOutMosi2 };
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

reg[3:0]		tByteMode;		//SPI Mode
reg[3:0]		tNxtByteMode;	//SPI Mode

assign		spi_mode = tByteMode[1:0];


reg[5:0]		tByteCnt;		//Multi-Byte Transfer
reg[5:0]		tNxtByteCnt;	//Multi-Byte Transfer

reg[63:0]		tRegSendQ;
reg[63:0]		tNxtRegSendQ;

reg[63:0]		tRegRecvQ;
reg[63:0]		tNxtRegRecvQ;

reg[63:0]		tRegSendQB;
reg[63:0]		tNxtRegSendQB;
reg[63:0]		tRegRecvQB;
reg[63:0]		tNxtRegRecvQB;

reg[63:0]		tRegSendQC;
reg[63:0]		tNxtRegSendQC;
reg[63:0]		tRegRecvQC;
reg[63:0]		tNxtRegRecvQC;

reg[63:0]		tRegSendQD;
reg[63:0]		tNxtRegSendQD;
reg[63:0]		tRegRecvQD;
reg[63:0]		tNxtRegRecvQD;

reg				tRegXmitDeb;
reg				tNxtRegXmitDeb;

reg				tRegXmit32x;
reg				tNxtRegXmit32x;

always @*
begin
	tMmioOutData	= UV64_00;
	tMmioOK			= UMEM_OK_READY;

	mmioInOE		= (tMmioOpm[3]) && tMmioSelfCSel;
	mmioInWR		= (tMmioOpm[4]) && tMmioSelfCSel;
	mmioNxtLatchWR	= mmioInWR && mmioLatchWR;


	tNxtByteCnt		= tByteCnt;
	tNxtByteMode	= tByteMode;
	tNxtRegSendQ	= tRegSendQ;
	tNxtRegRecvQ	= tRegRecvQ;

	tNxtRegSendQB	= tRegSendQB;
	tNxtRegRecvQB	= tRegRecvQB;
	tNxtRegSendQC	= tRegSendQC;
	tNxtRegRecvQC	= tRegRecvQC;
	tNxtRegSendQD	= tRegSendQD;
	tNxtRegRecvQD	= tRegRecvQD;

	tNxtRegXmitDeb	= tRegXmitDeb;
	tNxtRegXmit32x	= tRegXmit32x;

	tNxtRegCtrl		= tRegCtrl;
//	tNxtDivCnt		= tDivCnt - 1;
	tNxtDivCnt		= 0;
	tNxtDivRst		= tDivRst;
	tNxtOutSclk		= tOutSclk;
	tNxtBitCnt		= tBitCnt;
	
	tDivRstH		= { 1'b0, tDivRst[13:1] };

`ifdef jx2_cpu_mmioclock_50

	if(tRegCtrl[31:24]==0)
	begin
		tNxtDivRst = { 3'b0, 8'h00, 3'h5 };
	end
	else
	begin
		case(tRegCtrl[23:22])
			2'b00: tNxtDivRst = { 6'b0, tRegCtrl[31:24] };
			2'b01: tNxtDivRst = { 4'b0, tRegCtrl[31:24], 2'h0 };
			2'b10: tNxtDivRst = { 2'b0, tRegCtrl[31:24], 4'h0 };
			2'b11: tNxtDivRst = {       tRegCtrl[31:24], 6'h0 };
		endcase
	end

//	tNxtDivRst = { 3'b00, tRegCtrl[31:24], 3'h5 };

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

//	tNxtDivRst = { 2'b00, tRegCtrl[31:24], 4'hA };
//	tNxtDivRst = { 2'b00, tRegCtrl[31:24], 4'hD };
//	tNxtDivRst = { 1'b0, tRegCtrl[31:24], 5'h1A };

	if(tRegCtrl[31:24]==0)
	begin
		tNxtDivRst = { 2'b0, 8'h00, 3'hA };
	end
	else
	begin
		case(tRegCtrl[23:22])
			2'b00: tNxtDivRst = { 5'b0, tRegCtrl[31:24], 1'b0 };
			2'b01: tNxtDivRst = { 3'b0, tRegCtrl[31:24], 3'h0 };
			2'b10: tNxtDivRst = { 1'b0, tRegCtrl[31:24], 5'h0 };
			2'b11: tNxtDivRst = {       tRegCtrl[30:24], 7'h0 };
		endcase
	end

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
			tNxtOutSclk = 1 ^ tRegCtrl[13];
//			tNxtRegExchI	= { tRegExchI[14:0], tInMiso };
//			tNxtRegExchI	= { tRegExchI[14:0], spi_miso };

			if((tBitCnt!=0) && ((tByteMode[2:1]==2'b11) || tRegCtrl[12]))
			begin
				/* QSPI DDR */
				tOutMosi		= tRegExchO[7:4];
				tNxtRegExchI	= { tRegExchI[11:0], tInMiso[3:0] };
				tNxtRegExchO	= { tRegExchO[3:0], 4'b0 };
				tNxtBitCnt		= tBitCnt - 4;
			end
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
			tNxtOutSclk = 1 ^ tRegCtrl[13];
		end
`endif

		if(tDivCnt==0)
		begin
			if((tByteMode[2:1]==2'b11) || !tRegCtrl[12])
			begin
`ifdef def_true
	 //			tOutMosi		= tRegExchO[7];
				tOutMosi		= { 3'b0, tRegExchO[7] };
	//			tNxtRegExchI	= { tRegExchI[14:0], tInMiso };
				tNxtRegExchI	= { tRegExchI[14:0], tInMiso[0] };
				tNxtRegExchO	= { tRegExchO[6:0], 1'b0 };
				tNxtDivCnt		= tDivRst;
				tNxtOutSclk		= 0 ^ tRegCtrl[13];
				tNxtBitCnt		= tBitCnt - 1;
				tOutCs			= tRegCtrl[0];

				if(tByteMode[1])
				begin
					/* QSPI */
					tOutMosi		= tRegExchO[7:4];
					tNxtRegExchI	= { tRegExchI[11:0], tInMiso[3:0] };
					tNxtRegExchO	= { tRegExchO[3:0], 4'b0 };
					tNxtBitCnt		= tBitCnt - 4;
				end

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
					tNxtOutSclk		= 0 ^ tRegCtrl[13];
					tNxtBitCnt		= tBitCnt - 1;
					tOutCs			= tRegCtrl[0];
				end
				else
				begin
					tNxtOutSclk		= 1 ^ tRegCtrl[13];
					tNxtDivCnt		= tDivRstH;
				end			
`endif

	//			$display("SdSpi Bit=%d Mosi=%d Miso=%d ExchI=%X ExchO=%X",
	//				tBitCnt, tOutMosi, spi_miso, tRegExchI, tRegExchO);
	//			$display("SdSpi Bit=%d Mosi=%d Miso=%d ExchI=%X ExchO=%X",
	//				tBitCnt, tOutMosi, tInMiso, tRegExchI, tRegExchO);
			end

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
//			tNxtRegSendQ		= { 8'hFF, tRegSendQ[63:8] };
			tNxtRegRecvQ		= { tRegValIn, tNxtRegRecvQ[63:8] };

			tNxtRegSendQ		= { tRegSendQB[7:0], tRegSendQ [63:8] };
			tNxtRegSendQB		= { tRegSendQC[7:0], tRegSendQB[63:8] };
			tNxtRegSendQC		= { tRegSendQD[7:0], tRegSendQC[63:8] };
			tNxtRegSendQD		= {           8'hFF, tRegSendQD[63:8] };

			tNxtRegRecvQB		= { tRegRecvQC[7:0], tRegRecvQB[63:8] };
			tNxtRegRecvQC		= { tRegRecvQD[7:0], tRegRecvQC[63:8] };
			tNxtRegRecvQD		= {       tRegValIn, tRegRecvQD[63:8] };

			if(tRegXmit32x)
			begin
				tNxtRegRecvQ	= { tRegRecvQB[7:0], tRegRecvQ [63:8] };
			end
			
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

			tNxtByteMode	= tNxtRegCtrl[11:8];

//			if(tMmioInData[1] && (tBitCnt==0))
			if(tMmioInData[1])		 /* XMIT 1X */
			begin
				tNxtBitCnt			= 8;
				tNxtDivCnt			= 0;
				tNxtRegXmitDeb		= 1;
				tNxtRegXmit32x		= 0;
			end
			else
				if(tMmioInData[5])	 /* XMIT 8X */
			begin
//				tNxtBitCnt			= 8;
				tNxtDivCnt			= 0;
//				tNxtRegExchO[7:0]	= tRegSendQ[7:0];

				tNxtByteCnt			= 9;
				tNxtRegXmitDeb		= 1;
				tNxtRegXmit32x		= 0;
			end
			else
				if(tMmioInData[6])	 /* XMIT 32X */
			begin
				tNxtDivCnt			= 0;

				tNxtByteCnt			= 33;
				tNxtRegXmitDeb		= 1;
				tNxtRegXmit32x		= 1;
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
		case(tMmioAddr[1:0])
			2'b00:	tMmioOutData	= tRegRecvQ;
			2'b01:	tMmioOutData	= tRegRecvQB;
			2'b10:	tMmioOutData	= tRegRecvQC;
			2'b11:	tMmioOutData	= tRegRecvQD;
		endcase
	
//		tMmioOutData	= tRegRecvQ;
		tMmioOK			= UMEM_OK_OK;
		tNxtRegXmitDeb	= 0;
	end

	if((tMmioAddr[3:2]==2'b10) && mmioInWR)
	begin
//		tNxtRegSendQ	= tMmioInData;
		mmioNxtLatchWR	= 1;

		case(tMmioAddr[1:0])
			2'b00:	tNxtRegSendQ	= tMmioInData;
			2'b01:	tNxtRegSendQB	= tMmioInData;
			2'b10:	tNxtRegSendQC	= tMmioInData;
			2'b11:	tNxtRegSendQD	= tMmioInData;
		endcase

		tMmioOK			= mmioLatchWR ? UMEM_OK_OK : UMEM_OK_HOLD;
	end

	if(reset)
	begin
		tNxtRegXmitDeb	= 0;
		mmioNxtLatchWR	= 0;
		tNxtByteCnt			= 0;
		tNxtBitCnt			= 0;

		tMmioOK			= UMEM_OK_READY;
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
//	tInMiso			<= spi_miso[0];

	tRegCtrl		<= tNxtRegCtrl;
	tRegExchI		<= tNxtRegExchI;
	tRegExchO		<= tNxtRegExchO;
	tDivCnt			<= tNxtDivCnt;
	tDivRst			<= tNxtDivRst;
	tOutSclk		<= tNxtOutSclk;

	tBitCnt			<= tNxtBitCnt;
	tLastBitCnt		<= tBitCnt;
	tByteCnt		<= tNxtByteCnt;
	tByteMode		<= tNxtByteMode;

	mmioLatchWR		<= mmioNxtLatchWR;

	tRegSendQ		<= tNxtRegSendQ;
	tRegRecvQ		<= tNxtRegRecvQ;

	tRegSendQB		<= tNxtRegSendQB;
	tRegRecvQB		<= tNxtRegRecvQB;
	tRegSendQC		<= tNxtRegSendQC;
	tRegRecvQC		<= tNxtRegRecvQC;
	tRegSendQD		<= tNxtRegSendQD;
	tRegRecvQD		<= tNxtRegRecvQD;

	tRegXmitDeb		<= tNxtRegXmitDeb;
	tRegXmit32x		<= tNxtRegXmit32x;
end

endmodule
