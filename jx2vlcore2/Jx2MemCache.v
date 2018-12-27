`include "Jx2CoreDefs.v"

`ifdef JX2_MEM_CACHEA256
`include "Jx2MemTileA.v"
`include "Jx2DcTileA.v"
`include "Jx2IcTileC.v"
`endif

`ifdef JX2_MEM_CACHEB128
`include "Jx2MemTileB.v"
`include "Jx2DcTileB.v"
`include "Jx2IcTileB.v"
`endif

`ifdef JX2_MEM_CACHEC256
`include "Jx2MemTileC.v"
`include "Jx2DcTileC.v"
`include "Jx2IcTileC.v"
`endif

`ifndef JX2_MEM_NOMMU
`include "Jx2MmuTlb.v"
`endif

module Jx2MemCache(
	/* verilator lint_off UNUSED */
	clock,		reset,

	dcRegInAddr,
	dcRegInData,	dcRegOutData,
	dcRegInOpm,		dcRegOutOK,

	icRegInPc,		icRegOutPcVal,
	icRegOutPcOK,	icRegStepPc,

	regOutExc,		regOutTea,
	memRegInMMCR,	memRegInKRR,
	memRegInSR,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,		
	mmioOpm,		mmioOK
	);

input			clock;			//clock
input			reset;			//reset

// /* verilator lint_off UNOPTFLAT */

input[63: 0]	dcRegInAddr;		//input address
input[ 4: 0]	dcRegInOpm;
input[63: 0]	dcRegInData;		//input data (store)
output[63: 0]	dcRegOutData;		//output data (load)
output[ 1: 0]	dcRegOutOK;			//set if we have a valid value.

input[63: 0]	icRegInPc;		//input PC address
output[47: 0]	icRegOutPcVal;	//output PC value
output[ 1: 0]	icRegOutPcOK;		//set if we have a valid value.
output[ 1: 0]	icRegStepPc;

output[15:0]	regOutExc;		//Raise Exception
output[63:0]	regOutTea;		//Exception TEA

input[63:0]		memRegInMMCR;		//MMU Control Register
input[63:0]		memRegInKRR;		//Keyring Register
input[63:0]		memRegInSR;			//Status Register


input[127:0]	memInData;	//memory PC data
output[127:0]	memOutData;	//memory PC data
output[31:0]	memAddr;	//memory PC address
output[4:0]		memOpm;		//memory PC operation mode
input[1:0]		memOK;		//memory PC OK

input[31:0]		mmioInData;		//mmio data in
output[31:0]	mmioOutData;	//mmio data out
output[31:0]	mmioAddr;		//mmio address
output[4:0]		mmioOpm;		//mmio read
input[1:0]		mmioOK;			//mmio OK


reg[15:0]	tRegOutExc2;		//Raise Exception
reg[63:0]	tRegOutTea2;		//Exception TEA
assign		regOutExc = tRegOutExc2;
assign		regOutTea = tRegOutTea2;

reg[15:0]	tRegOutExc;		//Raise Exception
reg[63:0]	tRegOutTea;		//Exception TEA

// /* verilator lint_on UNOPTFLAT */

/* Conjoined Memory Cache */

reg[47:0]		memRegInAddr;		//input PC address
reg[47:0]		memRegOutAddr;		//output PC address
`ifdef JX2_MEM_TILE128
reg[127:0]		memRegInData;		//input data (store)
wire[127:0]		memRegOutData;		//output data (load)
`else
reg[255:0]		memRegInData;		//input data (store)
wire[255:0]		memRegOutData;		//output data (load)
`endif

reg[4:0]		memRegInOpm;		//Operation Size/Type
wire[1:0]		memRegOutOK;		//set if operation suceeds

wire[15:0]		memRegOutExc;		//Raise Exception
wire[63:0]		memRegOutTea;		//Exception TEA

`ifdef JX2_MEM_CACHEB128
Jx2MemTileB	memTile(
	clock, reset,
	memRegInAddr,	memRegInData,
	memRegOutData,
	memRegOutOK,	memRegInOpm,	
	memInData,		memOutData,		memAddr,
	memOpm,			memOK,
	mmioInData,		mmioOutData,	mmioAddr,
	mmioOpm,		mmioOK
	);
`endif

`ifdef JX2_MEM_CACHEC256
Jx2MemTileC	memTile(
	clock, reset,
	memRegInAddr,	memRegInData,
	memRegOutAddr,	memRegOutData,
	memRegOutOK,	memRegInOpm,
	memRegOutExc,	memRegOutTea,

//	memRegInMMCR,	memRegInKRR,
//	memRegInSR,
	
	memInData,		memOutData,		memAddr,
	memOpm,			memOK,
	mmioInData,		mmioOutData,	mmioAddr,
	mmioOpm,		mmioOK
	);
`endif

`ifdef JX2_MEM_CACHEA256
Jx2MemTileA	memTile(
	clock, reset,
	memRegInAddr,	memRegInData,
	memRegOutData,
	memRegOutOK,	memRegInOpm,	
	memRegOutExc,	memRegOutTea,

	memInData,		memOutData,		memAddr,
	memOpm,			memOK,

	mmioInData,		mmioOutData,	mmioAddr,
	mmioOpm,		mmioOK
	);
`endif


/* D1 Cache */

`ifdef JX2_MEM_TILE128
reg [127:0]		dcMemPcDataI;		//memory data in
wire[127:0]		dcMemPcDataO;		//memory data out
`else
reg [255:0]		dcMemPcDataI;		//memory data in
wire[255:0]		dcMemPcDataO;		//memory data out
`endif
reg [  1:0]		dcMemPcOK;			//memory OK
wire[ 47:0]		dcMemPcAddrI;		//memory address (in)
wire[ 47:0]		dcMemPcAddrO;		//memory address (out)
wire[  4:0]		dcMemPcOpm;			//memory output-enable

`ifdef JX2_MEM_CACHEA256
Jx2DcTileA	dcTile(
	clock, reset,
	dcRegInAddr,		dcRegInOpm,
	dcRegInData,		dcRegOutData,
	dcRegOutOK,

	dcMemPcAddrI,
	dcMemPcDataI,		dcMemPcDataO,
	dcMemPcOpm,			dcMemPcOK
	);
`endif

`ifdef JX2_MEM_CACHEB128
Jx2DcTileB	dcTile(
	clock, reset,
	dcRegInAddr,		dcRegInOpm,
	dcRegInData,		dcRegOutData,
	dcRegOutOK,
	dcMemPcAddrI,
	dcMemPcDataI,		dcMemPcDataO,
	dcMemPcOpm,			dcMemPcOK
	);
`endif

`ifdef JX2_MEM_CACHEC256
Jx2DcTileC	dcTile(
	clock, reset,
	dcRegInAddr,		dcRegInOpm,
	dcRegInData,		dcRegOutData,
	dcRegOutOK,
	dcMemPcAddrI,		dcMemPcAddrO,
	dcMemPcDataI,		dcMemPcDataO,
	dcMemPcOpm,			dcMemPcOK
	);
`endif

/* I1 Cache */

`ifdef JX2_MEM_TILE128
reg [127:0]		icMemPcData;		//memory PC data
`else
reg [255:0]		icMemPcData;		//memory PC data
`endif
reg [  1:0]		icMemPcOK;		//memory PC OK
wire[ 47:0]		icMemPcAddr;		//memory PC address
wire[  4:0]		icMemPcOpm;		//memory PC output-enable

`ifdef JX2_MEM_CACHEB128
Jx2IcTileB	icTile(
	clock, reset,
	icRegInPc,
	icRegOutPcVal,	icRegOutPcOK,
	icRegStepPc,
	icMemPcData,	icMemPcAddr,
	icMemPcOpm,		icMemPcOK
	);
`endif

`ifdef JX2_MEM_CACHEC256
Jx2IcTileC	icTile(
	clock, reset,
	icRegInPc,
	icRegOutPcVal,	icRegOutPcOK,
	icRegStepPc,
	icMemPcData,	icMemPcAddr,
	icMemPcOpm,		icMemPcOK
	);
`endif

`ifdef JX2_MEM_CACHEA256
/* Case A reuses C's I$, as it is half-duplex either way. */
Jx2IcTileC	icTile(
	clock, reset,
	icRegInPc,
	icRegOutPcVal,	icRegOutPcOK,
	icRegStepPc,
	icMemPcData,	icMemPcAddr,
	icMemPcOpm,		icMemPcOK
	);
`endif

reg		memIsReady;
reg		memNextIsReady;
reg		memIcLatch;
reg		memNextIcLatch;
reg		memDcLatch;
reg		memNextDcLatch;

`ifndef JX2_MEM_NOMMU

reg[127:0]		mmuInData;		//input data (LDTLB)

wire			mmuIsHold;
assign		mmuIsHold = (mmuOutOKA == UMEM_OK_HOLD);

wire			mmuIsOK;
assign		mmuIsOK = (mmuOutOKA == UMEM_OK_OK);

/* MMU Lane A */

reg[47:0]		mmuInAddrA;		//input Address
wire[47:0]		mmuOutAddrA;	//output Address

reg[4:0]		mmuInOpmA;		//Operation Size/Type
wire[15:0]		mmuOutExcA;		//Raise Exception
wire[63:0]		mmuOutTeaA;		//Exception TEA
wire[1:0]		mmuOutOKA;		//set if operation suceeds

Jx2MmuTlb	mmuTlbA(
	clock,			reset,
	mmuInAddrA,		mmuOutAddrA,
	mmuInData,		mmuInOpmA,
	mmuOutExcA,		mmuOutTeaA,
	mmuOutOKA,
	memRegInMMCR,	memRegInKRR,
	memRegInSR
	);

`ifndef JX2_MEM_HALFDPX

/* MMU Lane B */

reg[47:0]		mmuInAddrB;		//input Address
wire[47:0]		mmuOutAddrB;	//output Address

reg[4:0]		mmuInOpmB;		//Operation Size/Type
wire[15:0]		mmuOutExcB;		//Raise Exception
wire[63:0]		mmuOutTeaB;		//Exception TEA
wire[1:0]		mmuOutOKB;		//set if operation suceeds

Jx2MmuTlb	mmuTlbB(
	clock,			reset,
	mmuInAddrB,		mmuOutAddrB,
	mmuInData,		mmuInOpmB,
	mmuOutExcB,		mmuOutTeaB,
	mmuOutOKB,
	memRegInMMCR,	memRegInKRR,
	memRegInSR
	);

`endif

`endif

reg		regSticky1;
reg		regNextSticky1;

always @*
begin
	icMemPcOK		= UMEM_OK_HOLD;
	dcMemPcOK		= UMEM_OK_HOLD;
	icMemPcData		= memRegOutData;
	dcMemPcDataI	= memRegOutData;

`ifdef JX2_MEM_NOMMU

`ifdef JX2_MEM_HALFDPX
//	memRegInAddr	= 0;
`else
//	memRegInAddr	= 0;
//	memRegOutAddr	= 0;
`endif

	memRegInOpm		= UMEM_OPM_READY;

`ifdef JX2_MEM_TILE128
	memRegInData	= UV128_XX;
`else
	memRegInData	= UV256_XX;
`endif
`endif

`ifndef JX2_MEM_NOMMU
	mmuInData		= dcMemPcDataO[127:0];
`endif

//	memRegInAddr	= 0;
//	memRegOutAddr	= 0;
//	memRegInOpm		= UMEM_OPM_READY;

//	memIsReady		= (memRegOutOK==UMEM_OK_READY);
	memNextIsReady	= (memRegOutOK==UMEM_OK_READY);
	memNextIcLatch	= 0;
	memNextDcLatch	= 0;
	
	tRegOutExc	= 0;
	tRegOutTea	= 0;
	regNextSticky1	= 0;
	
	if(memRegOutExc[15])
	begin
		tRegOutExc = memRegOutExc;
		tRegOutTea = memRegOutTea;
	end

`ifndef JX2_MEM_NOMMU

`ifdef JX2_MEM_HALFDPX
	memRegInAddr	= 0;
`else
	memRegInAddr	= 0;
	memRegOutAddr	= 0;
`endif

`ifdef JX2_MEM_TILE128
	memRegInData	= UV128_XX;
`else
	memRegInData	= UV256_XX;
`endif

	memRegInOpm		= UMEM_OPM_READY;

	mmuInAddrA	= UV48_XX;
	mmuInOpmA	= 0;

	if(mmuOutExcA[15])
	begin
		tRegOutExc = mmuOutExcA;
		tRegOutTea = mmuOutTeaA;
	end

`ifndef JX2_MEM_HALFDPX
	mmuInAddrB	= UV48_XX;
	mmuInOpmB	= 0;

	if(mmuOutExcB[15])
	begin
		tRegOutExc = mmuOutExcB;
		tRegOutTea = mmuOutTeaB;
	end
`endif

`endif

//	if((icMemPcOpm!=0) && (memIsReady||memIcLatch) && !memDcLatch)
	if(	((icMemPcOpm!=0) || memIcLatch) &&
		(memIsReady || memIcLatch) &&
		!memDcLatch)
	begin
//		memNextIcLatch	= 1;
		memNextIcLatch	= (icMemPcOpm!=0);
		icMemPcData		= memRegOutData;
		icMemPcOK		= memRegOutOK;

`ifdef JX2_MEM_NOMMU
		memRegInAddr	= icMemPcAddr;
		memRegOutAddr	= icMemPcAddr;
		memRegInOpm		= icMemPcOpm;
`else
		mmuInAddrA		= icMemPcAddr;
		mmuInOpmA		= icMemPcOpm;

		memRegInAddr	= mmuOutAddrA;
		memRegOutAddr	= mmuOutAddrA;
		memRegInOpm		= icMemPcOpm;
//		memRegInOpm		= UMEM_OPM_READY;

		if(!mmuIsOK)
//		if(1'b1)
		begin
//			$display("TLB Hold AI=%X Opm=%X", mmuInAddrA, mmuInOpmA);
//			$display("TLB Hold AO=%X Ok=%X", mmuOutAddrA, mmuOutOKA);
//			$display("TLB Hold Opm=%X Ok=%X", icMemPcOpm, mmuOutOKA);
			memRegInOpm		= UMEM_OPM_READY;
//			memRegInOpm		= icMemPcOpm;
		end

//		$display("TLB Hold AI=%X Opm=%X", mmuInAddrA, mmuInOpmA);
//		$display("TLB Hold AO=%X Ok=%X", mmuOutAddrA, mmuOutOKA);
//		$display("Mem Opm=%X Ok=%X", memRegInOpm, memRegOutOK);
		
//		if(mmuIsHold)
//			icMemPcOK		= UMEM_OK_HOLD;
`endif

`ifndef JX2_MEM_QUIET
		$display("Jx2MemCache: IC Addr=%X Opm=%X",
			icMemPcAddr, icMemPcOpm);

		if(dcMemPcOpm!=0)
		begin
			$display("Jx2MemCache: DC(Wait) AI=%X AO=%X Opm=%X",
				dcMemPcAddrI, dcMemPcAddrO,
				dcMemPcOpm);
		end

`ifdef JX2_MEMDBG_PRINTDATA
`ifndef JX2_MEM_TILE128
		$display("Jx2MemCache: icBlkData(H)=%X_%X_%X_%X",
			memRegOutData[255:224], memRegOutData[223:192],
			memRegOutData[191:160], memRegOutData[159:128]);
`endif
		$display("Jx2MemCache: icBlkData(L)=%X_%X_%X_%X",
			memRegOutData[127: 96], memRegOutData[ 95: 64],
			memRegOutData[ 63: 32], memRegOutData[ 31:  0]);
`endif

`endif
	end
	else
		if((dcMemPcOpm!=0) && (memIsReady||memDcLatch))
//		if(((dcMemPcOpm!=0)||memDcLatch) && (memIsReady||memDcLatch))
	begin
//		memNextDcLatch	= 1;
		memNextDcLatch	= (dcMemPcOpm!=0);
		dcMemPcDataI	= memRegOutData;
		dcMemPcOK		= memRegOutOK;

`ifdef JX2_MEM_NOMMU
		memRegInData	= dcMemPcDataO;
		memRegInOpm		= dcMemPcOpm;

`ifdef JX2_MEM_HALFDPX
		memRegInAddr	= dcMemPcAddrI;
`else
		memRegOutAddr	= dcMemPcAddrI;
		memRegInAddr	= dcMemPcAddrO;
`endif

`else

//		if(mmuIsHold)
//			dcMemPcOK	= UMEM_OK_HOLD;

		memRegInData	= dcMemPcDataO;
		memRegInOpm		= dcMemPcOpm;

		if(!mmuIsOK)
			memRegInOpm		= UMEM_OPM_READY;

`ifdef JX2_MEM_HALFDPX
		mmuInAddrA		= dcMemPcAddrI;
		memRegInAddr	= mmuOutAddrA;
		mmuInOpmA		= dcMemPcOpm;
`else
		mmuInAddrA		= dcMemPcAddrI;
		mmuInAddrB		= dcMemPcAddrO;
		memRegOutAddr	= mmuOutAddrA;
		memRegInAddr	= mmuOutAddrB;
		mmuInOpmA		= dcMemPcOpm;
		mmuInOpmB		= dcMemPcOpm;
		mmuInOpmA[4]	= 0;
		mmuInOpmB[3]	= 0;
`endif

`endif
		
`ifndef JX2_MEM_QUIET
		if(!regSticky1)
		begin
			regNextSticky1 = 1;

//			$display("Jx2MemCache: DC AI=%X AO=%X Opm=%X",
//				dcMemPcAddrI, dcMemPcAddrO,
//				dcMemPcOpm);
		
`ifdef JX2_MEMDBG_PRINTDATA
`ifndef JX2_MEM_TILE128
			$display("Jx2MemCache: dcBlkData(H)=%X_%X_%X_%X",
				memRegInData[255:224], memRegInData[223:192],
				memRegInData[191:160], memRegInData[159:128]);
`endif
			$display("Jx2MemCache: dcBlkData(L)=%X_%X_%X_%X",
				memRegInData[127: 96], memRegInData[ 95: 64],
				memRegInData[ 63: 32], memRegInData[ 31:  0]);

`ifndef JX2_MEM_TILE128
			$display("Jx2MemCache: memRegOutData(H)=%X_%X_%X_%X",
				memRegOutData[255:224], memRegOutData[223:192],
				memRegOutData[191:160], memRegOutData[159:128]);
`endif
			$display("Jx2MemCache: memRegOutData(L)=%X_%X_%X_%X",
				memRegOutData[127: 96], memRegOutData[ 95: 64],
				memRegOutData[ 63: 32], memRegOutData[ 31:  0]);
`endif
		end
`endif

	end
	else
	begin
`ifndef JX2_MEM_QUIET
		if((icMemPcOpm!=0) || (dcMemPcOpm!=0))
		begin
			$display("MemCache: Latch State IcL=%X DcL=%X IcOpm=%X DcOpm=%X",
				memIcLatch, memDcLatch, icMemPcOpm, dcMemPcOpm);
//			$display("  IcOK=%d DcOK=%d", memRegOutOK, memIsReady);
			$display("  memOK=%d isReady=%d", memRegOutOK, memIsReady);
		end
`endif

		if(icMemPcOpm==0)
			icMemPcOK		= UMEM_OK_READY;
		if(dcMemPcOpm==0)
			dcMemPcOK		= UMEM_OK_READY;
	end
end

always @(posedge clock)
begin
	memIsReady		<= memNextIsReady;
	memIcLatch		<= memNextIcLatch;
	memDcLatch		<= memNextDcLatch;

	regSticky1		<= regNextSticky1;

	tRegOutExc2		<= tRegOutExc;		//Raise Exception
	tRegOutTea2		<= tRegOutTea;		//Exception TEA
	
// `ifndef JX2_MEM_NOMMU
`ifndef def_true
	if(memNextIcLatch)
	begin
		memRegInAddr	<= mmuOutAddrA;
		memRegOutAddr	<= mmuOutAddrA;
		memRegInOpm		<= icMemPcOpm;

		if(!mmuIsOK)
			memRegInOpm		<= UMEM_OPM_READY;

	end
	else
		if(memNextDcLatch)
	begin
		memRegInData	<= dcMemPcDataO;
		memRegInOpm		<= dcMemPcOpm;

		if(!mmuIsOK)
			memRegInOpm		<= UMEM_OPM_READY;

`ifdef JX2_MEM_HALFDPX
		memRegInAddr	<= mmuOutAddrA;
`else
		memRegOutAddr	<= mmuOutAddrA;
		memRegInAddr	<= mmuOutAddrB;
`endif
	end
	else
	begin
		memRegInAddr	<= 0;
		memRegOutAddr	<= 0;
		memRegInOpm		<= UMEM_OPM_READY;
	end
`endif

end

endmodule
