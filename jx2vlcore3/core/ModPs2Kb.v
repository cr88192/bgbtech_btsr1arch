/*
Deal with PS2 Keyboard.
 */

`include "CoreDefs.v"

module ModPs2Kb(
	/* verilator lint_off UNUSED */
	clock,			reset,
	ps2_clk_i,		ps2_clk_o,		ps2_clk_d,	
	ps2_data_i,		ps2_data_o,		ps2_data_d,
	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

input			clock;
input			reset;

input			ps2_clk_i;
output			ps2_clk_o;
output			ps2_clk_d;
input			ps2_data_i;
output			ps2_data_o;
output			ps2_data_d;

input[31:0]		mmioInData;
output[31:0]	mmioOutData;
input[31:0]		mmioAddr;
input[4:0]		mmioOpm;
output[1:0]		mmioOK;

reg[31:0]		tMmioOutData;
reg[1:0]		tMmioOK;

reg[31:0]		tMmioOutData2;
reg[1:0]		tMmioOK2;

assign			ps2_clk_o	= 1'bz;
assign			ps2_data_o	= 1'bz;
assign			ps2_clk_d	= 1'b0;
assign			ps2_data_d	= 1'b0;

assign		mmioOutData = tMmioOutData2;
assign		mmioOK		= tMmioOK2;

wire		tMmioLowCSel;
assign		tMmioLowCSel = (mmioAddr[27:16]==12'h000);

wire		tMmioPs2CSel;
assign		tMmioPs2CSel = tMmioLowCSel && (mmioAddr[15:4]==12'hE04);

reg				mmioInOE;
reg				mmioInWR;

reg[7:0]		scanBuf[7:0];
reg[2:0]		scanSpos;
reg[2:0]		scanEpos;
reg[2:0]		scanNxtSpos;
reg[2:0]		scanNxtEpos;
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

reg[3:0]		ps2WinCnt;
reg[3:0]		ps2NxtWinCnt;

reg				ps2_lstClk_i;
reg				ps2_lstClk2_i;

always @*
begin
	tMmioOutData	= UV32_XX;
	tMmioOK			= UMEM_OK_READY;

	mmioInOE			= (mmioOpm[3]) && tMmioPs2CSel;
	mmioInWR			= (mmioOpm[4]) && tMmioPs2CSel;
	
	scanNxtSpos		= scanSpos;
	scanNxtEpos		= scanEpos;
	scanNxtSpAdv	= 0;
	scanEpAdv		= 0;
	ps2ScanAdv		= 0;
	ps2NxtWinAdv	= 0;
	ps2NxtWinCnt	= ps2WinCnt;
	ps2NxtWinP		= 0;

	ps2NxtWin = ps2Win;
//	if(ps2_lstClk_i && !ps2_clk_i)
	if(ps2_lstClk2_i && !ps2_lstClk_i)
//	if(!ps2_lstClk_i && ps2_clk_i)
	begin
		ps2NxtWin = { ps2_data_i, ps2Win[11:1] };
		ps2NxtWinAdv	= 1;

		if((ps2WinCnt != 0) || !ps2_data_i)
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

		$display("ModPs2Kb: bit=%X win=%B", ps2_data_i, ps2NxtWin);
	end

//	if(ps2WinAdv && (ps2WinCnt==11))
//	if(ps2WinAdv && (ps2WinCnt==11))
	if(ps2WinAdv && (ps2WinCnt>=11))
//	if(ps2WinAdv && (ps2WinCnt==9))
//	if(ps2WinAdv)
	begin
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
			scanEpAdv		= 1;
			ps2NxtWinCnt	= 0;
			
			$display("ModPs2Kb: Scan=%X Cnt=%d", ps2ScanAdv, ps2WinCnt);
		end
	end
	
//	if(ps2WinCnt>11)
//		ps2NxtWinCnt = 0;

	if(scanSpAdv && !mmioInOE)
	begin
		scanNxtSpos		= scanSpos + 1;
	end
	
	if((mmioAddr[3:2]==2'b00) && mmioInOE)
	begin
//		tMmioOutData	= { UV24_00, scanBuf[scanSpos] };
		tMmioOutData	= { UV24_00, ps2ScanCur };
		scanNxtSpAdv	= 1;
		tMmioOK			= UMEM_OK_OK;
	end

	if((mmioAddr[3:2]==2'b10) && mmioInOE)
	begin
		tMmioOutData	= 0;
		tMmioOutData[0] = (scanSpos != scanEpos);
		tMmioOK			= UMEM_OK_OK;
	end
end

always @(posedge clock)
begin
	tMmioOutData2	<= tMmioOutData;
	tMmioOK2		<= tMmioOK;
	scanSpos		<= scanNxtSpos;
	scanEpos		<= scanNxtEpos;
	scanSpAdv		<= scanNxtSpAdv;
	ps2WinAdv		<= ps2NxtWinAdv;
	ps2WinP			<= ps2NxtWinP;
	ps2WinCnt		<= ps2NxtWinCnt;

	ps2_lstClk_i	<= ps2_clk_i;
	ps2_lstClk2_i	<= ps2_lstClk_i;
	ps2Win			<= ps2NxtWin;
	
	ps2ScanCur		<= scanBuf[scanSpos];
	
	if(scanEpAdv)
	begin
		scanBuf[scanEpos]	<= ps2ScanAdv;
	end
end

endmodule
