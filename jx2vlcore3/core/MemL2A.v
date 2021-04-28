/*
Deal with L2 Cache sending requests to the right place.

* 00000000 .. 00007FFF: Boot ROM
* 00008000 .. 0000BFFF: Reserved / More ROM
* 0000C000 .. 0000DFFF: Boot SRAM
* 0000E000 .. 0000FFFF: Reserved / More SRAM
* 00010000 .. 0001FFFF: NULL Page
* 00020000 .. 000FFFFF: Reserved
* 00100000 .. 00FFFFFF: Reserved
* 01000000 .. 7FFFFFFF: DRAM
* 80000000 .. EFFFFFFF: Reserved
* F0000000 .. FFFFFFFF: MMIO (No MMU, Bypass)
** F0000000 .. FEFFFFFF: MMIO / Chipset
** FF000000 .. FFFFFFFF: Memory Mapped Registers

Loads from the NULL page will always return zeroes, and writes to this page will be ignored.

*/

`include "CoreDefs.v"
`include "MemL2Rom.v"

`ifndef jx2_mem_l2skip
`include "MemL2Dc.v"
`endif

module MemL2A(
	clock,
	reset,
	
	memAddr,		memAddrB,
	memDataIn,		memDataOut,
	memOpm,			memOK,
	memBounceIrq,

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
`input_tile		memDataIn;
`output_tile	memDataOut;
input[15:0]		memOpm;
output[1:0]		memOK;
output[63:0]	memBounceIrq;

`input_ddrtile	ddrMemDataIn;
`output_ddrtile	ddrMemDataOut;

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


reg[47:0]		tMemAddr;
reg[47:0]		tMemAddr2;
reg[47:0]		tMemAddrL;

reg[47:0]		tMemAddrB;
reg[47:0]		tMemAddrB2;
reg[47:0]		tMemAddrBL;

`reg_tile		tMemDataIn;
`reg_tile		tMemDataIn2;
`reg_tile		tMemDataInL;

reg[4:0]		tMemOpm;
reg[4:0]		tMemOpm2;
reg[4:0]		tMemOpmL;

`reg_tile		tMemDataOut;
`reg_tile		tMemDataOut2;
reg[1:0]		tMemOK;
reg[1:0]		tMemOK2;

reg[63:0]		tMmioOutData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

`ifdef jx2_l2a_nofw_memout
assign	memDataOut		= tMemDataOut;
assign	memOK			= tMemOK;
`else
assign	memDataOut		= tMemDataOut2;
assign	memOK			= tMemOK2;
`endif

assign	mmioOutData		= tMmioOutData;
assign	mmioAddr		= tMmioAddr;
assign	mmioOpm			= tMmioOpm;

reg[63:0]	tMemBounceIrq;
reg[63:0]	tMemBounceIrqB;
assign	memBounceIrq = tMemBounceIrqB;


wire	reqIsSwap;
assign	reqIsSwap	= (tMemOpm[4:3] == 2'b11);

wire	reqIsMmio;
// assign	reqIsMmio	= (memOpm[2:0] != 3'b111);
// assign	reqIsMmio	= (memOpm[4:3] != 2'b00) && (memOpm[2:0] != 3'b111);
assign	reqIsMmio	=
	(tMemOpm[4:3] != 2'b00) &&
	(tMemOpm[2:0] != 3'b111) && tMemOpm[1];

reg		reqMmioLatch;
reg		reqLo64Latch;
reg		reqRamLatch;
reg		reqIsLatch;

wire	tAddrIsRam;

reg			tDoneLatch;
`reg_tile	tDoneData;


wire	reqIsCcmd;
// assign	reqIsCcmd	= (memOpm[4:3] == 2'b00) && (memOpm[2:0] != 3'b000);
// assign	reqIsCcmd	= (tMemOpm[4:3] == 2'b00) && (tMemOpm[2:0] != 3'b000);
assign	reqIsCcmd	=
	((tMemOpm[4:3] == 2'b00) && (tMemOpm[2:0] != 3'b000)) ||
	((tMemOpm[4:3] != 2'b00) && !tMemOpm[1]);

wire	reqIsMem;
assign	reqIsMem	= (tMemOpm[4:3] != 2'b00) && (tMemOpm[2:0] == 3'b111);

wire[4:0]		l2MemOpm;
// assign			l2MemOpm = reqIsMmio ? UMEM_OPM_READY : memOpm;
// assign			l2MemOpm = reqIsMmio ? UMEM_OPM_READY : tMemOpm;
assign			l2MemOpm = (reqIsMmio || tDoneLatch) ? UMEM_OPM_READY : tMemOpm;


`wire_tile		l2MemDataOut;
`wire_tile		l2rMemDataOut;

wire[1:0]		l2MemOK;
reg[1:0]		l2MemOK2;
reg[1:0]		l2MemOK3;
reg[1:0]		l2MemOK4;

wire[1:0]		l2rMemOK;
reg[1:0]		l2rMemOK2;
reg[1:0]		l2rMemOK3;
reg[1:0]		l2rMemOK4;


`ifdef jx2_mem_l2skip

assign 	ddrMemAddr = tMemAddr[31:0];
assign 	ddrMemDataOut = tMemDataIn;
// assign 	ddrMemOpm = tAddrIsRam ? l2MemOpm : UMEM_OPM_READY;
assign 	ddrMemOpm = (tAddrIsRam && !tDoneLatch) ? l2MemOpm : UMEM_OPM_READY;

assign 	l2MemDataOut = ddrMemDataIn;
assign	l2MemOK	= ddrMemOK;

`else

MemL2Dc		l2dc(
	clock,		reset,

//	memAddr,	memAddrB,
//	memAddr[31:0],	memAddrB[31:0],
//	memDataIn,

	tMemAddr[31:0],	tMemAddrB[31:0],
	tMemDataIn,
	
	l2MemDataOut,
	l2MemOpm,	l2MemOK,

	ddrMemAddr,		ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK,
	
	tHoldStrobe
	);

`endif


wire	tAddrIsLo64k;
// assign	tAddrIsLo64k	= (memAddr[31:16] == UV16_00);
assign	tAddrIsLo64k	= (tMemAddr[31:16] == UV16_00);

wire	tAddrIsZe64k;
assign	tAddrIsZe64k	= (tMemAddr[31:16] == 16'h0001);

wire	tAddrIsZe1M;
assign	tAddrIsZe1M	= (tMemAddr[31:20] == 12'h001);

reg			tAddrIsLo4G;
reg			tAddrIsHi4G;

// wire	tAddrIsRam;
// assign	tAddrIsRam	= (memAddr[31:24] != UV8_00) && (!memAddr[31]);
assign	tAddrIsRam	= (tMemAddr[31:24] != UV8_00) && (!tMemAddr[31]);

wire[4:0]		l2rMemOpm;
// assign			l2rMemOpm = tAddrIsLo64k ? memOpm : UMEM_OPM_READY ;
assign			l2rMemOpm = tAddrIsLo64k ? tMemOpm : UMEM_OPM_READY ;

MemL2Rom	l2rom(
	clock,		reset,

//	memAddr,	l2rMemOpm,
//	memAddr[31:0],
	tMemAddr[31:0],
	l2rMemOpm,
//	memDataIn,
	tMemDataIn,
	l2rMemDataOut,
	l2rMemOK
	);

reg[63:0]	tMmioInData;
reg[1:0]	tMmioOK;

reg[1:0]	tCcmdOK;
reg[31:0]	tCcmdData;
reg			reqCcmdLatch;

reg[4:0]	tCcmdOpmA;
reg[4:0]	tCcmdOpmB;
reg[4:0]	tCcmdOpmC;

reg[15:0]	tHoldCyc;
reg[15:0]	tNxtHoldCyc;
reg			tHoldLatch;
reg			tNxtHoldLatch;
reg			tHoldLatchL;
reg			tHoldStrobe;

always @*
begin
`ifdef jx2_l2a_nofw_memin
	tMemAddr		= memAddr;
	tMemAddrB		= memAddrB;
	tMemDataIn		= memDataIn;
	tMemOpm			= memOpm;
`endif

	reqIsLatch = reqMmioLatch || reqLo64Latch || reqRamLatch || reqCcmdLatch;

	tAddrIsLo4G		= (tMemAddr[47:32] == 16'h0000);
	tAddrIsHi4G		= (tMemAddr[47:32] == 16'hFFFF);
	
	if(!tAddrIsLo4G && !tAddrIsHi4G)
	begin
		$display("L2A: Address out of range, A=%X", tMemAddr);
	end

	tCcmdOK		= UMEM_OK_READY;
//	tCcmdData	= UV32_XX;
	tCcmdData	= UV32_00;
	tNxtHoldCyc		= 0;
	tNxtHoldLatch	= 0;
	tMemBounceIrq	= 0;

//	case(memOpm)
	case(tMemOpm)
		UMEM_OPM_READY: begin
			tCcmdOK		= UMEM_OK_READY;
		end
		UMEM_OPM_LDTLB, UMEM_OPM_INVTLB: begin
			/* These simply get OK here; MMU has already seen it. */
			tCcmdOK		= UMEM_OK_OK;
//			tCcmdOK		= (memOpm==tCcmdOpmC)?UMEM_OK_OK:UMEM_OK_HOLD;
//			tCcmdOK		= (tMemOpm==tCcmdOpmC)?UMEM_OK_OK:UMEM_OK_HOLD;
//			tCcmdOK		= (tMemOpm==tCcmdOpmA)?UMEM_OK_OK:UMEM_OK_HOLD;
			tCcmdData	= UV32_00;
		end
		UMEM_OPM_FLUSHIS, UMEM_OPM_FLUSHDS: begin
			/* These simply get OK here. */
//			tCcmdOK		= UMEM_OK_OK;
//			tCcmdOK		= (memOpm==tCcmdOpmC)?UMEM_OK_OK:UMEM_OK_HOLD;
			tCcmdOK		= (tMemOpm==tCcmdOpmC)?UMEM_OK_OK:UMEM_OK_HOLD;
			tCcmdData	= UV32_00;
		end
		UMEM_OPM_TRAP: begin
//			tCcmdOK		= (memOpm==tCcmdOpmC)?UMEM_OK_OK:UMEM_OK_HOLD;
			tCcmdOK		= UMEM_OK_OK;
			if(!tMemBounceIrqB[15])
//				tMemBounceIrq = memDataIn[63:0];
				tMemBounceIrq = tMemDataIn[63:0];
		end

		UMEM_OPM_WR_UB: begin
			/* Ignore, Semaphore Signal */
			tCcmdOK		= UMEM_OK_READY;
		end
		
		default: begin
//			tCcmdOK		= UMEM_OK_OK;
		end
	endcase
	
	if(tMemOK[1])
	begin
		tNxtHoldCyc		= tHoldCyc + 1;
		tNxtHoldLatch	= tHoldLatch;
		if(tHoldCyc>65280)
		begin
			tNxtHoldCyc		= 65535;
			tNxtHoldLatch	= 1;
			
			if(!tHoldLatch)
			begin
				$display("MemL2A: Deadlock Detect");

				$display("Latch=%d MMIO=%d Low64k=%d L2/RAM=%d CCmd=%d",
					reqIsLatch, reqMmioLatch, reqLo64Latch,
					reqRamLatch, reqCcmdLatch);

				$display(
					"CPU Addr=%X AddrB=%X DataIn=%X DataOut=%X OPM=%X OK=%X",
					memAddr,	memAddrB,	memDataIn,	memDataOut,
					memOpm,		memOK		);

				$display("DDR Addr=%X DataIn=%X DataOut=%X OPM=%X OK=%X",
					ddrMemAddr,	ddrMemDataIn,	ddrMemDataOut,
					ddrMemOpm,	ddrMemOK		);

				$display("MMIO Addr=%X DataIn=%X DataOut=%X OPM=%X OK=%X",
					mmioAddr,	mmioInData,	mmioOutData,
					mmioOpm,	mmioOK		);
			end
		end
	end
end

always @(posedge clock)
begin
//	tCcmdOpmA		<= memOpm;
	tCcmdOpmA		<= tMemOpm;
	tCcmdOpmB		<= tCcmdOpmA;
	tCcmdOpmC		<= tCcmdOpmB;
	tHoldCyc		<= tNxtHoldCyc;
	tHoldLatchL		<= tHoldLatch;
	tHoldLatch		<= tNxtHoldLatch;
	tHoldStrobe		<= tHoldLatch!=tHoldLatchL;

`ifndef def_true	
	if((memOpm[2:0]==3'b111) && (memOpm[4:3]!=2'b00))
	begin
		$display("L2A Cyc: memOpm=%X l2MemOpm=%X l2rMemOpm=%X memOK=%X l2MemOK=%X l2rMemOK=%X",
		memOpm, l2MemOpm, l2rMemOpm, memOK, l2MemOK, l2rMemOK);
	end
`endif

	tMemBounceIrqB	<= tMemBounceIrq;

`ifndef jx2_l2a_nofw_memin
	tMemAddr		<= memAddr;
	tMemAddrB		<= memAddrB;
	tMemDataIn		<= memDataIn;
//	tMemOpm			<= memOpm;
	tMemOpm			<= memOpm[4:0];
`endif

	tMemAddrL		<= tMemAddr;
	tMemAddrBL		<= tMemAddrB;
	tMemDataInL		<= tMemDataIn;
	tMemOpmL		<= tMemOpm;
	
`ifdef def_true

	if((tMemAddr!=tMemAddrL) && (tMemOpm!=tMemOpmL))
	begin
//		$display("L2A: Both Change A=%X O=%X D=%X",
//			tMemAddr, tMemOpm, tMemDataIn[63:0]);
	end
	else
	begin

//		if(tMemOpm!=0)
		if((tMemOpm!=0) && (tMemOpmL!=0))
		begin
			if(tMemOpm!=tMemOpmL)
			begin
				$display("L2A: OPM Change A=%X O=%X->%X",
					tMemAddr, tMemOpmL, tMemOpm);
			end

			if(	tMemDataIn != tMemDataInL)
			begin
				$display("L2A: Data Change A=%X O=%X D=%X->%X",
					tMemAddr, tMemOpm, tMemDataInL[63:0], tMemDataIn[63:0]);
			end
		
			if(tMemAddr!=tMemAddrL)
			begin
				$display("L2A: Addr Change A=%X->%X O=%X",
					tMemAddrL, tMemAddr, tMemOpm);
			end

			if(tMemAddrB!=tMemAddrBL)
			begin
				$display("L2A: AddrB Change A=%X->%X O=%X",
					tMemAddrBL, tMemAddrB, tMemOpm);
			end
			
			if((tMemOK!=tMemOK2) && (tMemOK!=UMEM_OK_READY) &&
				(tMemOK2!=UMEM_OK_READY) && (tMemOK2!=UMEM_OK_HOLD))
			begin
				$display("L2A: OK Change OK=%X->%X A=%X O=%X",
					tMemOK2, tMemOK, tMemAddr, tMemOpm);

				$display("    L2 OK %X %X %X %X",
					l2MemOK, l2MemOK2, l2MemOK3, l2MemOK4);
				
//				$display("    L2R OK %X %X %X %X",
//					l2rMemOK, l2rMemOK2, l2rMemOK3, l2rMemOK4);
			end
			
		end
	end
`endif

	tMmioInData		<= mmioInData;
	tMmioOK			<= mmioOK;

	tMemDataOut2	<= tMemDataOut;
	tMemOK2			<= tMemOK;

	if(reset)
	begin
		reqCcmdLatch	<= 0;
		reqMmioLatch	<= 0;
		reqLo64Latch	<= 0;
		reqRamLatch		<= 0;
		tDoneLatch		<= 0;

		tMemOK			<= UMEM_OK_READY;
	end
	else
		if((reqIsCcmd && !reqIsLatch) || reqCcmdLatch)
	begin
`ifdef jx2_mem_line32B
		tMemDataOut		<= { UV224_00, tCcmdData };
`else
//		tMemDataOut		<= { UV96_XX, tCcmdData };
		tMemDataOut		<= { UV96_00, tCcmdData };
`endif
		tMemOK			<= tCcmdOK;
		reqCcmdLatch	<= reqIsCcmd || (tCcmdOK != UMEM_OK_READY);
	end
	else
//		if(reqIsMmio)
		if((reqIsMmio && !reqIsLatch) || reqMmioLatch)
	begin
//		tMemDataOut		<= { UV96_XX, mmioInData };
//		tMemDataOut		<= { UV64_00, mmioInData };
//		tMemOK			<= mmioOK;
//		reqMmioLatch	<= (reqIsMmio && (memOpm[4:3]!=0)) ||
//		reqMmioLatch	<= (reqIsMmio && (tMemOpm[4:3]!=0)) ||
//			(mmioOK != UMEM_OK_READY);

`ifdef jx2_mem_line32B
		tMemDataOut		<= { UV192_00, tMmioInData };
		tDoneData		<= { UV192_00, tMmioInData };
`else
		tMemDataOut		<= { UV64_00, tMmioInData };
		tDoneData		<= { UV64_00, tMmioInData };
`endif
		tMemOK			<= tMmioOK;
		reqMmioLatch	<= (reqIsMmio && (tMemOpm[4:3]!=0)) ||
			(tMmioOK != UMEM_OK_READY);

`ifdef jx2_mem_fasttdown
		if(tDoneLatch)
		begin
			tMemDataOut		<= tDoneData;
			tMemOK			<= UMEM_OK_OK;
		end
		else
		begin
//			tDoneData		<= l2MemDataOut;
			tDoneLatch		<= (tMmioOK == UMEM_OK_OK);
		end
`endif

	end
	else
		if((tAddrIsLo64k && !reqIsLatch) || reqLo64Latch)
	begin
//		if((tAddrIsLo64k && (memOpm[4:3]!=0)) || reqLo64Latch)
//		if((tAddrIsLo64k && (tMemOpm[4:3]!=0)) || reqLo64Latch)
		if((tAddrIsLo64k && reqIsMem) || reqLo64Latch)
		begin
//			$display("Lo64K A=%X Opm=%X OK=%X D=%X",
//				memAddr, memOpm, l2rMemOK, l2rMemDataOut);
		end

		tMemDataOut		<= l2rMemDataOut;
		tMemOK			<= l2rMemOK;
//		reqLo64Latch	<= tAddrIsLo64k || (l2rMemOK != UMEM_OK_READY);
//		reqLo64Latch	<= (tAddrIsLo64k && (memOpm[4:3]!=0)) ||
		reqLo64Latch	<= (tAddrIsLo64k && (tMemOpm[4:3]!=0)) ||
			(l2rMemOK != UMEM_OK_READY);
	end
	else
//		if((tAddrIsRam && !reqIsLatch) || reqRamLatch)
//		if((tAddrIsRam && (tMemOpm[4:3]!=0) && !reqIsLatch) || reqRamLatch)
		if((tAddrIsRam && reqIsMem && !reqIsLatch) || reqRamLatch)
	begin
		tMemDataOut		<= l2MemDataOut;
		tMemOK			<= l2MemOK;
//		reqRamLatch		<= (tAddrIsRam && (memOpm[4:3]!=0)) ||
		reqRamLatch		<= (tAddrIsRam && (tMemOpm[4:3]!=0)) ||
			(l2MemOK != UMEM_OK_READY);

`ifdef jx2_mem_fasttdown
		if(tDoneLatch)
		begin
			tMemDataOut		<= tDoneData;
			tMemOK			<= UMEM_OK_OK;
//			tDoneLatch		<=
//				(tMemOpm[4:3] != 0) ||
//				(l2MemOK != UMEM_OK_READY);
		end
		else
		begin
			tDoneData		<= l2MemDataOut;
			tDoneLatch		<= (l2MemOK == UMEM_OK_OK);
		end
`endif
	end
	else
//		if(tAddrIsZe64k && reqIsMem)
		if((tAddrIsZe64k || tAddrIsZe1M) && reqIsMem)
	begin
		tMemDataOut		<= UVTILE_00;
		tMemOK			<= UMEM_OK_OK;
		if(reqIsSwap)
			tMemOK		<= UMEM_OK_FAULT;
//		tMemOK			<= (tMemOpm[4:3]!=0) ?
//			UMEM_OK_OK : UMEM_OK_READY;
	end
	else
	begin
`ifndef def_true
		if(reqIsMem)
		begin
			tMemDataOut			<= UVTILE_00;
			tMemDataOut[15:0]	<= 16'h8000;
			tMemOK				<= UMEM_OK_FAULT;
		end
		else
		begin
			tMemDataOut		<= UVTILE_00;
			tMemOK			<= UMEM_OK_READY;
		end
`endif

		tMemDataOut		<= UVTILE_00;
		tMemOK			<= UMEM_OK_READY;
		tDoneLatch		<= 0;
	end

//	tDdrMemDataOut;
//	tDdrMemAddr;

//	tMmioOutData		<= memDataIn[31:0];
//	tMmioOutData		<= memDataIn[63:0];
	tMmioOutData		<= tMemDataIn[63:0];
//	tMmioAddr			<= memAddr;
//	tMmioAddr			<= memAddr[31:0];
	tMmioAddr			<= tMemAddr[31:0];
//	tMmioOpm			<= reqIsMmio ? memOpm : UMEM_OPM_READY;
//	tMmioOpm			<= reqIsMmio ? tMemOpm : UMEM_OPM_READY;
`ifdef jx2_mem_fasttdown
	tMmioOpm			<=
		(reqIsMmio && !tDoneLatch) ?
			tMemOpm : UMEM_OPM_READY;
`else
	tMmioOpm			<= reqIsMmio ? tMemOpm : UMEM_OPM_READY;
`endif

	if(reset)
	begin
		tMmioAddr			<= 0;
		tMmioOpm			<= UMEM_OPM_READY;
	end

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

	l2MemOK2	<= l2MemOK;
	l2MemOK3	<= l2MemOK2;
	l2MemOK4	<= l2MemOK3;

	l2rMemOK2	<= l2rMemOK;
	l2rMemOK3	<= l2rMemOK2;
	l2rMemOK4	<= l2rMemOK3;

end

endmodule
