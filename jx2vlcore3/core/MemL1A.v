/*
Contain L1 Instruction and Data Caches.

These caches provide a pipelined interface, recieving an address on one cycle, and producing output the next.

These will produce a Hold signal if the request can't be handled immediately, and will not accept new input until the condition is resolved.

FAULT Signal:
Will interpret the low 16 bits of the Data as an exception code.
If Bit(15) is Set, this raises an interrupt.
If Bit(15) is Clear, this indicates a non-critical failure.

In the case of Full-Duplex / Swap operations, a FAULT with a zero exception will indicate that the Swap operation could not be performed with this combination of locations. In this case, the L1 cache should fall back to Half-Duplex Load/Store operations.
*/

`include "MemDcA.v"

`ifdef jx2_enable_wex
`include "MemIcWxA.v"
`else
`include "MemIcA.v"
`endif

`ifdef jx2_enable_mmu
`include "MmuTlb.v"
`endif

module MemL1A(
	/* verilator lint_off UNUSED */
	clock,			reset,

	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,

	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dcOutOK,
	dcInHold,
	dcOutHold,

	regInDlr,		regInDhr,
	regInMmcr,		regInKrr,
	regInSr,
	
	regOutExc,		regTraPc,
	dcInTraPc,		deadlockLatch,

	memAddr,		memAddrB,
	memDataIn,		memDataOut,
	memOpm,			memOK
	);

input			clock;
input			reset;

input [47: 0]	icInPcAddr;		//input PC address
// output[63: 0]	icOutPcVal;		//output PC value
output[95: 0]	icOutPcVal;		//output PC value
output[ 1: 0]	icOutPcOK;		//set if we have a valid value.
output[ 3: 0]	icOutPcStep;	//PC step (Normal Op)
input			icInPcHold;
input			icInPcWxe;

input [47: 0]	dcInAddr;		//input PC address
input [4: 0]	dcInOpm;		//input PC address
output[63: 0]	dcOutVal;		//output data value
input [63: 0]	dcInVal;		//input data value
output[ 1: 0]	dcOutOK;		//set if we have a valid value.
input			dcInHold;
output			dcOutHold;		//we need to stall the pipeline

output[63: 0]	dcOutValB;		//output data value (alternate)
input [63: 0]	dcInValB;		//input data value (alternate)

input[63:0]		regInDlr;
input[63:0]		regInDhr;
input[63:0]		regInMmcr;
input[63:0]		regInKrr;
input[63:0]		regInSr;

output[63:0]	regOutExc;
output[63:0]	regTraPc;
input [63:0]	dcInTraPc;		//input PC
input			deadlockLatch;	//CPU Is Deadlocked

output[ 47:0]	memAddr;		//Memory address (Primary)
output[ 47:0]	memAddrB;		//Memory address (Secondary)
output[ 15:0]	memOpm;			//Memory Operation
input [  1:0]	memOK;			//Memory OK
`input_tile		memDataIn;		//Memory Data In
`output_tile	memDataOut;		//Memory Data Out

reg[ 47:0]		tMemAddr;		//Memory address
reg[ 47:0]		tMemAddrB;		//Memory address
reg[  4:0]		tMemOpm;		//Memory Operation

`reg_tile		tMemDataOut;	//Memory Data Out

reg[63:0]		tRegOutExc;
reg[63:0]		tRegOutExc2;

reg[63:0]		tRegTraPc;
reg[63:0]		tRegTraPc2;

wire[5:0]		tMemAccNoRwx;

`ifdef jx2_enable_mmu

wire[47:0]		tTlbAddr;
wire[47:0]		tTlbAddrB;
wire[63:0]		tTlbExc;
wire[1:0]		tTlbOK;
wire[4:0]		tTlbOpm;
`wire_tile		tTlbDataOut;	//Memory Data Out

assign	memAddr		= tTlbAddr;
assign	memAddrB	= tTlbAddrB;
assign	memOpm		= { 11'h0, tTlbOpm };
// assign	memDataOut	= tMemDataOut;
assign	memDataOut	= tTlbDataOut;

`else

assign	memAddr		= tMemAddr;
assign	memAddrB	= tMemAddrB;
// assign	memOpm		= tMemOpm;
assign	memOpm		= { 11'h0, tMemOpm };
assign	memDataOut	= tMemDataOut;

assign	tMemAccNoRwx	= 0;

`endif

assign	regOutExc	= tRegOutExc2;
assign	regTraPc	= tRegTraPc2;

reg[1:0]		tDcOutOK;
reg				tDcOutHold;
assign	dcOutOK		= tDcOutOK;
assign	dcOutHold	= tDcOutHold;

`ifdef jx2_enable_mmu

wire[127:0]		tTlbLdtlbData;
assign			tTlbLdtlbData = { regInDhr, regInDlr };

MmuTlb	tlb(
	clock,			reset,
	tMemAddr,		tTlbAddr,
	tMemAddrB,		tTlbAddrB,	tTlbLdtlbData,
	tMemDataOut,	tTlbDataOut,
	tMemOpm,		tTlbOpm,
	tTlbExc,		tTlbOK,		tMemAccNoRwx,
	regInMmcr,		regInKrr,
	regInSr);

`endif

reg			tDoneLatch;
reg			tNxtDoneLatch;
`reg_tile	tDoneData;



`reg_tile		ifMemData;
reg [  1:0]		ifMemOK;
wire[ 47:0]		ifMemAddr;
wire[  4:0]		ifMemOpm;
reg [  5:0]		ifMemNoRwx;

`ifdef jx2_enable_wex
MemIcWxA		memIc(
	clock,			reset,
	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,
	dcInOpm[4:0],	regInSr,
	ifMemData,		ifMemAddr,
	ifMemOpm,		ifMemOK,
	ifMemNoRwx

//	ifMemData2,		ifMemAddr,
//	ifMemOpm,		ifMemOK2,
//	ifMemNoRwx2
	);
`else
MemIcA		memIc(
	clock,			reset,
	icInPcAddr,		icOutPcVal,
	icOutPcOK,		icOutPcStep,
	icInPcHold,		icInPcWxe,
	dcInOpm,		regInSr,
	ifMemData,		ifMemAddr,
	ifMemOpm,		ifMemOK
	);
`endif


wire[  1:0]		dfOutOK;
wire[  1:0]		dfOutOKB;

wire[ 47:0]		dfMemAddr;
wire[ 47:0]		dfMemAddrB;
wire[  4:0]		dfMemOpm;
reg [  1:0]		dfMemOK;
reg [  5:0]		dfMemNoRwx;
reg [ 19:0]		dfMemPaBits;
`reg_tile		dfMemDataIn;
`wire_tile		dfMemDataOut;

MemDcA		memDc(
	clock,			reset,
	dcInAddr,		dcInOpm,
	dcOutVal,		dcInVal,
	dcOutValB,		dcInValB,
	dfOutOK,		dcInHold,
	dfOutOKB,		regInSr,

	dfMemAddr,		dfMemAddrB,
	dfMemDataIn,	dfMemDataOut,
	dfMemOpm,		dfMemOK,
	dfMemNoRwx,		dfMemPaBits
	);

reg		tLatchIc;
reg		tLatchDc;
reg		tNxtLatchIc;
reg		tNxtLatchDc;
reg		tIfNzOpm;
reg		tDfNzOpm;

reg		tMsgLatch;
reg		tNxtMsgLatch;
reg		tDeadlockLatch;
reg		tDeadlockLatchL;

always @*
begin
	tNxtLatchIc	= 0;
	tNxtLatchDc	= 0;
	tNxtMsgLatch	= 0;
	tRegOutExc	= UV64_00;
	tRegTraPc	= UV64_00;
	
	tNxtDoneLatch	= tDoneLatch;

//	tMemAddr	= UV32_XX;
//	tMemAddr	= UV32_00;
	tMemAddr	= UV48_00;
//	tMemAddrB	= UV32_XX;
//	tMemAddrB	= UV32_00;
	tMemAddrB	= UV48_00;
	tMemOpm		= UMEM_OPM_READY;

//	tMemDataOut	= UV128_XX;
//	tMemDataOut	= UV128_00;
	tMemDataOut	= UVTILE_00;
	
//	tDcOutOK	= dfOutOK;
	tDcOutOK	= dfOutOK[1:0];
	
	tDcOutHold	= 0;
`ifdef	jx2_mem_l1dstall
	tDcOutHold	= dfOutOKB[1];
`endif

	ifMemData	= memDataIn;
	dfMemDataIn	= memDataIn;
	
`ifdef jx2_mem_fasttdown
	if(tDoneLatch)
	begin
		ifMemData	= tDoneData;
		dfMemDataIn	= tDoneData;
	end
`endif

	tIfNzOpm	= (ifMemOpm != UMEM_OPM_READY);
	tDfNzOpm	= (dfMemOpm != UMEM_OPM_READY);

	ifMemOK	= UMEM_OK_READY;
	dfMemOK	= UMEM_OK_READY;

//	ifMemOK	= tLatchDc ? UMEM_OK_HOLD : UMEM_OK_READY;
//	dfMemOK	= tLatchIc ? UMEM_OK_HOLD : UMEM_OK_READY;
	
//	ifMemOK	= tIfNzOpm ? UMEM_OK_HOLD : UMEM_OK_READY;
//	dfMemOK	= tDfNzOpm ? UMEM_OK_HOLD : UMEM_OK_READY;
	dfMemNoRwx	= 0;
	ifMemNoRwx	= 0;
	
	if(memOK == UMEM_OK_FAULT)
	begin
		if(memDataIn[15])
		begin
			tRegOutExc = { dfMemAddr, memDataIn[15:0] };
		end
	end

	if(dfMemOpm == UMEM_OPM_FAULT)
	begin
//		if(dfMemAddr[0])
//			tRegOutExc = {UV16_00, dfMemAddr, 16'h8001 };
//		if(dfMemAddr[1])
//			tRegOutExc = {UV16_00, dfMemAddr, 16'h8002 };

//		tRegOutExc = { UV16_00, dfMemAddr, 12'h800, 
		tRegOutExc = { dfMemAddr, 12'h800, 
			(dfMemAddr[1]) ? 4'h2 : 4'h1 };
	end

	if(((tIfNzOpm && !tLatchDc) || tLatchIc) && !reset)
	begin
//		$display("L1$, Latch I$, A=%X Opm=%X, OK=%d",
//			ifMemAddr, ifMemOpm, memOK);
	
		ifMemOK		= memOK;
		ifMemNoRwx	= tMemAccNoRwx;
		tMemAddr	= ifMemAddr;
		tMemAddrB	= 0;
		tMemOpm		= ifMemOpm;
//		tMemDataOut	= UV128_XX;
//		tMemDataOut	= UV128_00;
		tMemDataOut	= UVTILE_00;
		tNxtLatchIc	= tIfNzOpm || (memOK != UMEM_OK_READY);

`ifdef jx2_enable_mmu
		ifMemNoRwx[5]	= tTlbExc[15];
`endif

`ifndef def_true
		if(tTlbExc[15])
		begin
			ifMemData = 128'h30003000_30003000_30003000_30003000;
		end
`endif

		if(tMemAccNoRwx[2])
//			tRegOutExc = {UV16_00, ifMemAddr, 16'h8003 };
			tRegOutExc = { ifMemAddr, 16'h8003 };
	end
	else
		if(((tDfNzOpm && !tLatchIc) || tLatchDc) && !reset)
	begin
//		$display("L1$, Latch D$, Opm=%X", dfMemOpm);

		dfMemOK		= memOK;
		dfMemNoRwx	= tMemAccNoRwx;
		tMemAddr	= dfMemAddr;
		tMemAddrB	= dfMemAddrB;
		tMemOpm		= dfMemOpm;
		tMemDataOut	= dfMemDataOut;
//		tNxtLatchDc	= tDfNzOpm || (memOK != UMEM_OK_READY);
		tNxtLatchDc	= tDfNzOpm || (memOK != UMEM_OK_READY) || tDoneLatch;

`ifdef jx2_mem_fasttdown
// `ifndef def_true
		if(tDoneLatch)
		begin
			dfMemOK = UMEM_OK_OK;
//			if(!tDfNzOpm)
			if(!tDfNzOpm && (memOK == UMEM_OK_READY))
				tNxtDoneLatch = 0;
		end
		else
		begin
			if(memOK == UMEM_OK_OK)
				tNxtDoneLatch = 1;
		end
`endif

		if(tMemAccNoRwx[4])
		begin
			/* Hold if mem-access completes before access checks. */
			if(memOK == UMEM_OK_OK)
				dfMemOK		= UMEM_OK_HOLD;
		end

`ifndef def_true
		if(tTlbExc[15] && dfMemOpm[4])
		begin
			/* Stores are No-Op on TLB Miss */
			tMemOpm		= UMEM_OPM_READY;
			dfMemOK		= UMEM_OK_OK;
			tNxtLatchDc	= tDfNzOpm;
		end
`endif

`ifndef def_true
		if(tTlbExc[15])
		begin
			$display("L1A: Inject Bad Marker %X", dfMemAddr);
			dfMemDataIn = 128'h55BAADAA_55BAADAA_55BAADAA_55BAADAA;
		end
`endif

		if(tMemAccNoRwx[0] && tMemAccNoRwx[1])
//			tRegOutExc = {UV16_00, dfMemAddr, 16'h8001 };
			tRegOutExc = { dfMemAddr, 16'h8001 };

`ifdef jx2_enable_mmu
		dfMemNoRwx[5]=tTlbExc[15];
`endif
	end
	else
	begin
		tNxtDoneLatch	= 0;
	end

`ifndef def_true
	if((memOK==UMEM_OK_FAULT) && (memOpm[4:3]!=2'b11) && !reset)
	begin
		if(!tMsgLatch)
		begin
//			$display("L1$ Fault, Ic=%d Dc=%d A=%X Opm=%X V=%X",
//				tLatchIc, tLatchDc, tMemAddr, tMemOpm, tMemDataOut);
		end
		tNxtMsgLatch = 1;
		
//		tRegOutExc = {UV16_00, tMemAddr, 16'h8000 };
		tRegOutExc = { tMemAddr, 16'h8000 };
	end
`endif
	
`ifdef jx2_enable_mmu
	if(tTlbExc[15] && !reset)
	begin
//		$display("L1A TLB EXC %X", tTlbExc);
		tRegOutExc = tTlbExc;
	end
`endif

//	if((tRegOutExc[15:12]==4'h8) || (tRegOutExc[15:12]==4'hA))
//	begin
//		ifMemOK	= tIfNzOpm ? UMEM_OK_OK : UMEM_OK_READY;
//		dfMemOK	= tDfNzOpm ? UMEM_OK_OK : UMEM_OK_READY;
//	end

`ifdef jx2_mem_fasttdown
	if(tDoneLatch)
	begin
		tMemOpm		= UMEM_OPM_READY;
	end
`endif

/*
	else
		if((dcInOpm == UMEM_OPM_LDTLB) && !tLatchIc && !tLatchDc)
	begin
		tDcOutOK	= memOK;
		tMemOpm		= dcInOpm;
		tMemAddr	= dcInAddr;
		tMemDataOut	= { regInDhr, regInDlr };
	end
*/

	if(tDeadlockLatchL != tDeadlockLatch)
	begin
		$display("tLatchIc=%X tLatchDc=%X", tLatchIc, tLatchDc);
		$display("ifMemOpm=%X dfMemOpm=%X", ifMemOpm, dfMemOpm);
		$display("AddrA=%X AddrB=%X memOK=%d", memAddr, memAddrB, memOK);
	end

end

always @(posedge clock)
begin	
//	ifMemData2		<= ifMemData;
//	ifMemOK2		<= ifMemOK;
//	ifMemNoRwx2		<= ifMemNoRwx;

	dfMemPaBits		<= memAddr[31:12];

	tDeadlockLatchL	<= tDeadlockLatch;
	tDeadlockLatch	<= deadlockLatch;

	if(reset)
	begin
		tRegOutExc2	<= 0;
		tLatchIc	<= 0;
		tLatchDc	<= 0;
		tMsgLatch	<= 0;
		tDoneLatch	<= 0;
	end
	else
	begin
		tRegOutExc2	<= tRegOutExc;
		tRegTraPc2	<= tRegTraPc;

		tLatchIc	<= tNxtLatchIc;
		tLatchDc	<= tNxtLatchDc;
		tMsgLatch	<= tNxtMsgLatch;
		tDoneLatch	<= tNxtDoneLatch;
		
		if(!tDoneLatch)
			tDoneData	<= memDataIn;
	end
end

endmodule
