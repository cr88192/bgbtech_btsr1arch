/*
Deal with L2 Cache sending requests to the right place.
*/

`include "CoreDefs.v"
`include "MemL2Dc.v"
`include "MemL2Rom.v"

module MemL2A(
	clock,
	reset,
	
	memAddr,	memAddrB,
	memDataIn,	memDataOut,
	memOpm,		memOK,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK,

	mmioAddr,		mmioOpm,
	mmioInData,		mmioOutData,
	mmioOK
	);

input			clock;
input			reset;
	
input[47:0]		memAddr;
input[47:0]		memAddrB;
input[127:0]	memDataIn;
output[127:0]	memDataOut;
input[4:0]		memOpm;
output[1:0]		memOK;

input[127:0]	ddrMemDataIn;
output[127:0]	ddrMemDataOut;
output[31:0]	ddrMemAddr;
output[4:0]		ddrMemOpm;
input[1:0]		ddrMemOK;

// input[31:0]		mmioInData;
// output[31:0]	mmioOutData;
input[63:0]		mmioInData;
output[63:0]	mmioOutData;
output[31:0]	mmioAddr;
output[4:0]		mmioOpm;
input[1:0]		mmioOK;



reg[127:0]		tMemDataOut;
reg[1:0]		tMemOK;
// reg[127:0]		tDdrMemDataOut;
// reg[31:0]		tDdrMemAddr;
// reg[4:0]		tDdrMemOpm;
// reg[31:0]		tMmioOutData;
reg[63:0]		tMmioOutData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

assign	memDataOut		= tMemDataOut;
assign	memOK			= tMemOK;
// assign	ddrMemDataOut	= tDdrMemDataOut;
// assign	ddrMemAddr		= tDdrMemAddr;
// assign	ddrMemOpm		= tDdrMemOpm;
assign	mmioOutData		= tMmioOutData;
assign	mmioAddr		= tMmioAddr;
assign	mmioOpm			= tMmioOpm;

wire	reqIsMmio;
// assign	reqIsMmio	= (memOpm[2:0] != 3'b111);
assign	reqIsMmio	= (memOpm[4:3] != 2'b00) && (memOpm[2:0] != 3'b111);

reg		reqMmioLatch;
reg		reqLo64Latch;
reg		reqRamLatch;
reg		reqIsLatch;

wire	reqIsCcmd;
assign	reqIsCcmd	= (memOpm[4:3] == 2'b00) && (memOpm[2:0] != 3'b000);

wire[4:0]		l2MemOpm;
assign			l2MemOpm = reqIsMmio ? UMEM_OPM_READY : memOpm;

wire[127:0]		l2MemDataOut;
wire[1:0]		l2MemOK;

MemL2Dc		l2dc(
	clock,		reset,

//	memAddr,	memAddrB,
	memAddr[31:0],	memAddrB[31:0],
	memDataIn,	l2MemDataOut,
	l2MemOpm,	l2MemOK,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK
	);

wire	tAddrIsLo64k;
assign	tAddrIsLo64k	= (memAddr[31:16] == UV16_00);

wire	tAddrIsRam;
assign	tAddrIsRam	= (memAddr[31:24] != UV8_00) && (!memAddr[31]);

wire[4:0]		l2rMemOpm;
assign			l2rMemOpm = tAddrIsLo64k ? memOpm : UMEM_OPM_READY ;

wire[127:0]		l2rMemDataOut;
wire[1:0]		l2rMemOK;

MemL2Rom	l2rom(
	clock,		reset,

//	memAddr,	l2rMemOpm,
	memAddr[31:0],	l2rMemOpm,
	memDataIn,	l2rMemDataOut,
	l2rMemOK
	);

reg[1:0]	tCcmdOK;
reg[31:0]	tCcmdData;
reg			reqCcmdLatch;

reg[4:0]	tCcmdOpmA;
reg[4:0]	tCcmdOpmB;
reg[4:0]	tCcmdOpmC;

always @*
begin
	reqIsLatch = reqMmioLatch || reqLo64Latch || reqRamLatch || reqCcmdLatch;

	tCcmdOK		= UMEM_OK_READY;
//	tCcmdData	= UV32_XX;
	tCcmdData	= UV32_00;
	
	case(memOpm)
		UMEM_OPM_LDTLB, UMEM_OPM_INVTLB: begin
			/* These simply get OK here; MMU has already seen it. */
//			tCcmdOK		= UMEM_OK_OK;
			tCcmdOK		= (memOpm==tCcmdOpmC)?UMEM_OK_OK:UMEM_OK_HOLD;
			tCcmdData	= UV32_00;
		end
		UMEM_OPM_FLUSHIS, UMEM_OPM_FLUSHDS: begin
			/* These simply get OK here. */
//			tCcmdOK		= UMEM_OK_OK;
			tCcmdOK		= (memOpm==tCcmdOpmC)?UMEM_OK_OK:UMEM_OK_HOLD;
			tCcmdData	= UV32_00;
		end
		default: begin
		end
	endcase
end

always @(posedge clock)
begin
	tCcmdOpmA	<= memOpm;
	tCcmdOpmB	<= tCcmdOpmA;
	tCcmdOpmC	<= tCcmdOpmB;

	if(reset)
	begin
		reqCcmdLatch	<= 0;
		reqMmioLatch	<= 0;
		reqLo64Latch	<= 0;
		reqRamLatch		<= 0;
	end
	else
		if((reqIsCcmd && !reqIsLatch) || reqCcmdLatch)
	begin
//		tMemDataOut		<= { UV96_XX, tCcmdData };
		tMemDataOut		<= { UV96_00, tCcmdData };
		tMemOK			<= tCcmdOK;
		reqCcmdLatch	<= reqIsCcmd || (tCcmdOK != UMEM_OK_READY);
	end
	else
//		if(reqIsMmio)
		if((reqIsMmio && !reqIsLatch) || reqMmioLatch)
	begin
//		tMemDataOut		<= { UV96_XX, mmioInData };
		tMemDataOut		<= { UV64_00, mmioInData };
		tMemOK			<= mmioOK;
		reqMmioLatch	<= (reqIsMmio && (memOpm[4:3]!=0)) ||
			(mmioOK != UMEM_OK_READY);
	end
	else
		if((tAddrIsLo64k && !reqIsLatch) || reqLo64Latch)
	begin
		if((tAddrIsLo64k && (memOpm[4:3]!=0)) || reqLo64Latch)
		begin
//			$display("Lo64K A=%X Opm=%X OK=%X D=%X",
//				memAddr, memOpm, l2rMemOK, l2rMemDataOut);
		end

		tMemDataOut		<= l2rMemDataOut;
		tMemOK			<= l2rMemOK;
//		reqLo64Latch	<= tAddrIsLo64k || (l2rMemOK != UMEM_OK_READY);
		reqLo64Latch	<= (tAddrIsLo64k && (memOpm[4:3]!=0)) ||
			(l2rMemOK != UMEM_OK_READY);
	end
	else
		if((tAddrIsRam && !reqIsLatch) || reqRamLatch)
	begin
		tMemDataOut		<= l2MemDataOut;
		tMemOK			<= l2MemOK;
		reqRamLatch		<= (tAddrIsRam && (memOpm[4:3]!=0)) ||
			(l2MemOK != UMEM_OK_READY);
	end
	else
	begin
		tMemDataOut		<= UV128_00;
		tMemOK			<= UMEM_OK_READY;
	end

//	tDdrMemDataOut;
//	tDdrMemAddr;

//	tMmioOutData		<= memDataIn[31:0];
	tMmioOutData		<= memDataIn[63:0];
//	tMmioAddr			<= memAddr;
	tMmioAddr			<= memAddr[31:0];
	tMmioOpm			<= reqIsMmio ? memOpm : UMEM_OPM_READY;

`ifndef def_true
// `ifdef def_true
	if(memOpm!=0)
	begin
		$display("L2 A=%X Opm=%X OK=%X  Do=%X Di=%X",
			memAddr, memOpm, tMemOK,
			tMemDataOut, memDataIn);
		if(l2rMemOpm!=0)
			$display("L2r Opm=%X", l2rMemOpm);
		if(l2MemOpm!=0)
			$display("L2 Opm=%X", l2MemOpm);
		if(tMmioOpm!=0)
			$display("L2 MMIO Opm=%X", tMmioOpm);
	end
`endif

end

endmodule
