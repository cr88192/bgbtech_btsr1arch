/*
Deal with PS2 Keyboard.
 */

`include "CoreDefs.v"

module ModPs2Kb(
	/* verilator lint_off UNUSED */
	clock,			reset,
//	ps2_clk_i,		ps2_clk_o,		ps2_clk_d,	
//	ps2_data_i,		ps2_data_o,		ps2_data_d,
	ps2_clkdat_i,	ps2_clkdat_o,	ps2_clkdat_d,
	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK,			mmioCsBase
	);

input			clock;
input			reset;

// input			ps2_clk_i;
// input			ps2_data_i;
// output			ps2_clk_o;
// output			ps2_clk_d;
// output			ps2_data_o;
// output			ps2_data_d;

input[1:0]			ps2_clkdat_i;
output[1:0]			ps2_clkdat_o;
output[1:0]			ps2_clkdat_d;


input[31:0]		mmioInData;
output[31:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;

input[11:0]		mmioCsBase;

reg[31:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[31:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

// assign			ps2_clk_o	= 1'bz;
// assign			ps2_data_o	= 1'bz;
// assign			ps2_clk_d	= 1'b0;
// assign			ps2_data_d	= 1'b0;

reg[1:0]	tClkdatO;
reg[1:0]	tClkdatD;
reg[1:0]	tClkdatO2;
reg[1:0]	tClkdatD2;

// assign		ps2_clkdat_o = 2'b00;
// assign		ps2_clkdat_d = 2'b00;

assign		ps2_clkdat_o = tClkdatO2;
assign		ps2_clkdat_d = tClkdatD2;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

reg			ps2_clk_i1a0;
reg			ps2_data_i1a0;
reg			ps2_clk_i1a1;
reg			ps2_data_i1a1;
reg			ps2_clk_i1a2;
reg			ps2_data_i1a2;

reg			ps2_clk_i2;
reg			ps2_data_i2;

reg[31:0]		tMmioInData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

wire		tMmioLowCSel;
assign		tMmioLowCSel = (tMmioAddr[27:16]==12'h000);

wire		tMmioPs2CSel;
// assign		tMmioPs2CSel = tMmioLowCSel && (tMmioAddr[15:4]==12'hE04);
assign		tMmioPs2CSel = tMmioLowCSel && (tMmioAddr[15:4]==mmioCsBase);

reg				mmioInOE;
reg				mmioInWR;

`ifndef def_true
reg[7:0]		scanBuf[15:0];
reg[3:0]		scanSpos;
reg[3:0]		scanEpos;
reg[3:0]		scanNxtSpos;
reg[3:0]		scanNxtEpos;
`endif

`ifdef def_true
reg[7:0]		scanBuf[31:0];
reg[4:0]		scanSpos;
reg[4:0]		scanEpos;
reg[4:0]		scanNxtSpos;
reg[4:0]		scanNxtEpos;
`endif

reg				scanSpAdv;
reg				scanNxtSpAdv;
reg				scanEpAdv;
reg[7:0]		ps2ScanAdv;

reg[7:0]		ps2ScanCur;

reg[11:0]		ps2Win;
reg[11:0]		ps2NxtWin;
reg				ps2WinP;
reg				ps2NxtWinP;
reg				ps2WinAdv;
reg				ps2NxtWinAdv;

reg				ps2Deb3P;
reg				ps2Deb2P;
reg				ps2DebP;
reg				ps2NxtDebP;
reg				ps2DebInhP;

reg[5:0]		ps2WinCnt;
reg[5:0]		ps2NxtWinCnt;

reg				ps2_lstClk_i;
reg				ps2_lstClk2_i;

reg[15:0]		tBitTimeout;
reg[15:0]		tNxtBitTimeout;

reg[7:0]		tScanOutDat;
reg				tScanOutDatP;
reg[3:0]		tScanOutPos;
reg[7:0]		tNxtScanOutDat;
reg				tNxtScanOutDatP;
reg[3:0]		tNxtScanOutPos;

reg[13:0]		tScanOutClk;
reg[13:0]		tNxtScanOutClk;

always @*
begin
	tMmioOutData	= UV32_XX;
	tMmioOK			= UMEM_OK_READY;

	mmioInOE			= (tMmioOpm[3]) && tMmioPs2CSel;
	mmioInWR			= (tMmioOpm[4]) && tMmioPs2CSel;

	tClkdatO		= 2'b00;
	tClkdatD		= 2'b00;
	tNxtScanOutDat	= tScanOutDat;
	tNxtScanOutDatP	= tScanOutDatP;
	tNxtScanOutPos	= tScanOutPos;
	tNxtScanOutClk	= tScanOutClk + 1;

	scanNxtSpos		= scanSpos;
	scanNxtEpos		= scanEpos;
//	scanNxtSpAdv	= 0;
	scanNxtSpAdv	= scanSpAdv;
	scanEpAdv		= 0;
	ps2ScanAdv		= 0;
	ps2NxtWinAdv	= 0;
	ps2NxtWinCnt	= ps2WinCnt;
	ps2NxtWinP		= 0;

	ps2NxtDebP		= ps2DebP;
	
	ps2DebInhP		= ps2DebP || ps2Deb2P || ps2Deb3P;
	
	if(reset)
	begin
		ps2DebInhP	= 0;
		ps2NxtDebP	= 0;
	end

`ifndef def_true
	tNxtBitTimeout	= tBitTimeout-1;
	if(tBitTimeout == 0)
	begin
		if(ps2WinCnt!=0)
		begin
			$display("PS2KB: Position Reset");
		end
	
		tNxtBitTimeout	= 0;
		ps2NxtWinCnt	= 0;
	end
`endif

//	if(ps2_lstClk2_i != ps2_lstClk_i)
//	begin
//		$display("ModPs2Kb: c=%X d=%X", ps2_clk_i2, ps2_data_i2);
//	end

	ps2NxtWin = ps2Win;
//	if(ps2_lstClk_i && !ps2_clk_i)
//	if(ps2_lstClk2_i && !ps2_lstClk_i)
	if(ps2_lstClk2_i && !ps2_lstClk_i && (tScanOutPos==0))
//	if(!ps2_lstClk_i && ps2_clk_i)
	begin
//		ps2NxtWin = { ps2_data_i, ps2Win[11:1] };
		ps2NxtWin = { ps2_data_i2, ps2Win[11:1] };
		ps2NxtWinAdv	= 1;
		
		tNxtBitTimeout = 65535;

//		if((ps2WinCnt != 0) || !ps2_data_i)
		if((ps2WinCnt != 0) || !ps2_data_i2)
			ps2NxtWinCnt	= ps2WinCnt+1;
		
		ps2NxtWinP =
			!(ps2NxtWin[2] ^ ps2NxtWin[3] ^
			  ps2NxtWin[4] ^ ps2NxtWin[5] ^
			  ps2NxtWin[6] ^ ps2NxtWin[7] ^
			  ps2NxtWin[8] ^ ps2NxtWin[9]);

//		ps2NxtWinP =
//			!(ps2NxtWin[1] ^ ps2NxtWin[2] ^
//			  ps2NxtWin[3] ^ ps2NxtWin[4] ^
//			  ps2NxtWin[5] ^ ps2NxtWin[6] ^
//			  ps2NxtWin[7] ^ ps2NxtWin[8]);

//		$display("ModPs2Kb: bit=%X win=%B", ps2_data_i, ps2NxtWin);
		$display("ModPs2Kb: bit=%X win=%B pos=%X",
			ps2_data_i2, ps2NxtWin, ps2WinCnt);

`ifndef def_true
//		if((ps2NxtWin[1]==1'b0) && (ps2NxtWin[11]==1'b1) &&
//			(ps2NxtWin[10]==ps2NxtWinP))
		if((ps2NxtWin[1]==1'b0) && (ps2NxtWin[11]==1'b1))
		begin
			$display("ModPs2Kb: See Scan=%X Cnt=%X OK=%X",
				ps2NxtWin[9:2], ps2NxtWinCnt, (ps2NxtWin[10]==ps2NxtWinP));
		end
`endif
	end

//	if(ps2WinAdv && (ps2WinCnt==11))
//	if(ps2WinAdv && (ps2WinCnt==11))
	if(ps2WinAdv && (ps2WinCnt>=11))
//	if(ps2WinAdv && (ps2WinCnt==9))
//	if(ps2WinAdv)
	begin
`ifndef def_true
		if((ps2Win[1]==1'b0) && (ps2Win[11]==1'b1) &&
			(ps2Win[10]!=ps2WinP))
		begin
			$display("ModPs2Kb: Bad Parity, Scan=%X Cnt=%d",
				ps2ScanAdv, ps2WinCnt);
		end
`endif

		if((ps2Win[1]==1'b0) && (ps2Win[11]==1'b1) &&
			(ps2Win[10]==ps2WinP))
//		if((ps2Win[1:0]==2'b01) && (ps2Win[11]==1'b1) &&
//			(ps2Win[10]==ps2WinP))
//		if((ps2Win[0]==1'b0) &&
//			(ps2Win[9]==ps2WinP))
//		if((ps2Win[1:0]==2'b01) && (ps2Win[11]==1'b1))
//		if((ps2Win[2:1]==2'b01))
		begin
//			ps2ScanAdv		= ps2Win[10:3];
			ps2ScanAdv		= ps2Win[9:2];
//			ps2ScanAdv		= ps2Win[8:1];
			scanNxtEpos		= scanEpos + 1;
//			scanEpAdv		= 1;
			scanEpAdv		= (scanNxtEpos != scanSpos);
			ps2NxtWinCnt	= 0;
			
			$display("ModPs2Kb: Scan=%X Cnt=%d", ps2ScanAdv, ps2WinCnt);
		end
	end
	
//	if(ps2WinCnt>11)
//		ps2NxtWinCnt = 0;

//	if(scanSpAdv && !mmioInOE)
	if(scanSpAdv && !mmioInOE && !ps2DebP)
	begin
		scanNxtSpos		= scanSpos + 1;
		scanNxtSpAdv	= 0;
	end
	
	if((tMmioAddr[3:2]==2'b00) && mmioInOE)
	begin
//		tMmioOutData	= { UV24_00, scanBuf[scanSpos] };
		tMmioOutData	= { UV24_00, ps2ScanCur };
//		scanNxtSpAdv	= 1;
		scanNxtSpAdv	= (scanSpos != scanEpos);
		tMmioOK			= UMEM_OK_OK;
		ps2NxtDebP		= 1;
	end

	if((tMmioAddr[3:2]==2'b10) && mmioInOE)
	begin
		tMmioOutData	= 0;
		tMmioOutData[0] = (scanSpos != scanEpos);
//		tMmioOutData[0] = (scanSpos != scanEpos) && !ps2DebP;

		tMmioOutData[1] = (tScanOutPos != 0);

//		tMmioOK			= UMEM_OK_OK;
		tMmioOK			= ps2DebInhP ? UMEM_OK_HOLD : UMEM_OK_OK;
		ps2NxtDebP		= 0;
	end


	if((tMmioAddr[3:2]==2'b01) && mmioInWR)
	begin
		tNxtScanOutDat	= tMmioInData[7:0];
		tNxtScanOutDatP	=
			tNxtScanOutDat[7] ^ tNxtScanOutDat[6] ^
			tNxtScanOutDat[5] ^ tNxtScanOutDat[4] ^
			tNxtScanOutDat[3] ^ tNxtScanOutDat[2] ^
			tNxtScanOutDat[1] ^ tNxtScanOutDat[0] ^
			1;
		tNxtScanOutPos	= 4'hD;
		tMmioOK			= UMEM_OK_OK;
		
		tNxtScanOutClk	= 0;
	end

	if((tMmioAddr[3:2]==2'b10) && mmioInWR)
	begin
		/* Ignore writes to status register for now. */
		tMmioOK			= UMEM_OK_OK;
	end

	if(tScanOutPos!=0)
	begin
		if(tScanOutPos == 4'hD)
		begin
			tClkdatO[1] = 0;
			tClkdatD[1] = 1;

			if(tScanOutClk[13])
			begin
				tNxtScanOutPos = tScanOutPos - 1;
			end
		end
		else
		begin
			if(ps2_lstClk2_i && !ps2_lstClk_i)
			begin
				tNxtScanOutPos = tScanOutPos - 1;
			end
		end
		
		tClkdatD[0] = 1;
		case(tScanOutPos)
			4'hF: tClkdatO[1] = 0;
			4'hE: tClkdatO[1] = 0;
			4'hD: tClkdatO[1] = 0;
			4'hC: tClkdatO[1] = 0;
			4'hB: tClkdatO[1] = tScanOutDat[0];
			4'hA: tClkdatO[1] = tScanOutDat[1];
			4'h9: tClkdatO[1] = tScanOutDat[2];
			4'h8: tClkdatO[1] = tScanOutDat[3];
			4'h7: tClkdatO[1] = tScanOutDat[4];
			4'h6: tClkdatO[1] = tScanOutDat[5];
			4'h5: tClkdatO[1] = tScanOutDat[6];
			4'h4: tClkdatO[1] = tScanOutDat[7];
			4'h3: tClkdatO[1] = tScanOutDatP;
			4'h2: tClkdatO[1] = 1;

			4'h1: tClkdatD[0] = 0;
			4'h0: tClkdatD[0] = 0;
		endcase
	end
	
//	if(reset)
//	begin
//		ps2NxtWinCnt	= 0;
//		scanNxtSpos		= 0;
//		scanNxtEpos		= 0;
//	end
end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOK2		<= tMmioOK;

	tMmioInData		<= mmioInData;
	tMmioAddr		<= mmioAddr;
	tMmioOpm		<= mmioOpm;

	tBitTimeout		<= tNxtBitTimeout;

	scanSpos		<= scanNxtSpos;
	scanEpos		<= scanNxtEpos;
	scanSpAdv		<= scanNxtSpAdv;
	ps2WinAdv		<= ps2NxtWinAdv;
	ps2WinP			<= ps2NxtWinP;
	ps2WinCnt		<= ps2NxtWinCnt;

	ps2Deb3P		<= ps2Deb2P;
	ps2Deb2P		<= ps2DebP;
	ps2DebP			<= ps2NxtDebP;

	tClkdatO2		<= tClkdatO;
	tClkdatD2		<= tClkdatD;

	tScanOutDat		<= tNxtScanOutDat;
	tScanOutDatP	<= tNxtScanOutDatP;
	tScanOutPos		<= tNxtScanOutPos;
	tScanOutClk		<= tNxtScanOutClk;

//	ps2_clk_i1a0	<= ps2_clk_i;
	ps2_clk_i1a0	<= ps2_clkdat_i[1];
	ps2_clk_i1a1	<= ps2_clk_i1a0;
	ps2_clk_i1a2	<= ps2_clk_i1a1;

//	ps2_data_i1a0	<= ps2_data_i;
	ps2_data_i1a0	<= ps2_clkdat_i[0];
	ps2_data_i1a1	<= ps2_data_i1a0;
	ps2_data_i1a2	<= ps2_data_i1a1;

//	ps2_clk_i2		<= ps2_clk_i1a1;
//	ps2_data_i2		<= ps2_data_i1a1;

//	ps2_clk_i2		<= ps2_clk_i1a2;
//	ps2_data_i2		<= ps2_data_i1a0;

//	ps2_clk_i2		<= ps2_clk_i1a0;
//	ps2_data_i2		<= ps2_data_i1a2;

	ps2_clk_i2		<= ps2_clk_i1a0;
	ps2_data_i2		<= ps2_data_i1a0;

//	ps2_clk_i2		<= ps2_clk_i;
//	ps2_data_i2		<= ps2_data_i;

	ps2_lstClk_i	<= ps2_clk_i2;
	ps2_lstClk2_i	<= ps2_lstClk_i;
	ps2Win			<= ps2NxtWin;
	
	ps2ScanCur		<= scanBuf[scanSpos];
	
	if(scanEpAdv)
	begin
		scanBuf[scanEpos]	<= ps2ScanAdv;
	end
end

endmodule
