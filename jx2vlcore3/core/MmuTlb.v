/*
TLB:
Sits between the L1 and L2 caches.

Filters output from the L1's memory interface, so that L1 operates with virtual addresses and L2 with physical addresses.

 */

`include "CoreDefs.v"

`include "MmuChkAcc.v"

module MmuTlb(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regOutAddr,
	regInAddrB,		regOutAddrB,	regInData,
	regInOpm,		regOutOpm,
	regOutExc,		regOutOK,		regOutNoRwx,
	regInMMCR,		regInKRR,		regInSR
	);

// /* verilator lint_off UNOPTFLAT */

input			clock;			//clock
input			reset;			//reset

input[31:0]		regInAddr;		//input Address (Primary)
output[31:0]	regOutAddr;		//output Address (Primary)
input[31:0]		regInAddrB;		//input Address (Secondary)
output[31:0]	regOutAddrB;	//output Address (Secondary)
input[127:0]	regInData;		//input data (LDTLB)

input[4:0]		regInOpm;		//Operation Size/Type
// output[15:0]	regOutExc;		//Raise Exception
// output[63:0]	regOutTea;		//Exception TEA
output[63:0]	regOutExc;		//Exception EXC+TEA
output[1:0]		regOutOK;		//set if operation suceeds
output[4:0]		regOutOpm;		//Operation Size/Type
output[3:0]		regOutNoRwx;	//No R/W/X

input[63:0]		regInMMCR;		//MMU Control Register
input[63:0]		regInKRR;		//Keyring Register
input[63:0]		regInSR;		//Status Register


reg[31:0]		tRegOutAddr2;
reg[31:0]		tRegOutAddrB2;
reg[15:0]		tRegOutExc2;
reg[63:0]		tRegOutTea2;
reg[1:0]		tRegOutOK2;
reg[4:0]		tRegOutOpm2;

assign		regOutAddr = tRegOutAddr2;
assign		regOutAddrB = tRegOutAddrB2;
assign		regOutOK = tRegOutOK2;
assign		regOutOpm = tRegOutOpm2;
// assign		regOutExc = tRegOutExc2;
// assign		regOutTea = tRegOutTea2;
assign		regOutExc = { tRegOutTea2[47:0], tRegOutExc2 };

// assign		regOutAddr = tRegOutAddr;
// assign		regOutOK = tRegOutOK;
// assign		regOutExc = tRegOutExc;
// assign		regOutTea = tRegOutTea;

reg[31:0]		tRegOutAddr;
reg[31:0]		tRegOutAddrB;
reg[15:0]		tRegOutExc;
reg[63:0]		tRegOutTea;
reg[1:0]		tRegOutOK;
reg[4:0]		tRegOutOpm;

reg[31:0]		tRegInAddr;		//input Address
reg[31:0]		tRegInAddrB;	//input Address

reg[63:0]	tlbBlkHiA[63:0];
reg[63:0]	tlbBlkHiB[63:0];
reg[63:0]	tlbBlkHiC[63:0];
reg[63:0]	tlbBlkHiD[63:0];

reg[63:0]	tlbBlkLoA[63:0];
reg[63:0]	tlbBlkLoB[63:0];
reg[63:0]	tlbBlkLoC[63:0];
reg[63:0]	tlbBlkLoD[63:0];

reg[63:0]	tlbFlushMask;
reg[63:0]	tlbNxtFlushMask;

reg[5:0]	tlbHixA;
reg[5:0]	tlbHixB;

reg[127:0]	tlbHdatA;
reg[127:0]	tlbHdatB;
reg[127:0]	tlbHdatC;
reg[127:0]	tlbHdatD;

reg[127:0]	tlbHdatE;
reg[127:0]	tlbHdatF;
reg[127:0]	tlbHdatG;
reg[127:0]	tlbHdatH;

reg			tlbMmuEnable;
reg			tlbMmuSkipA;
reg			tlbMmuSkipB;

reg			tlbHitA;
reg			tlbHitB;
reg			tlbHitC;
reg			tlbHitD;

reg			tlbHitE;
reg			tlbHitF;
reg			tlbHitG;
reg			tlbHitH;

reg			tlbHitAB;
reg			tlbHitCD;
reg			tlbHitEF;
reg			tlbHitGH;
reg			tlbHit;

reg			tlbMiss;

reg			tlbMmuSkip;

reg			tlbDoLdtlb;
reg			tlbLdtlbOK;

reg			icPageEq;

reg[31:0]	tlbAddrAB;
reg[31:0]	tlbAddrCD;
reg[31:0]	tlbAddrEF;
reg[31:0]	tlbAddrGH;
reg[31:0]	tlbAccAB;
reg[31:0]	tlbAccCD;
reg[31:0]	tlbAccEF;
reg[31:0]	tlbAccGH;

reg[31:0]	tlbAddr;
reg[31:0]	tlbAcc;
reg[31:0]	tlbAddrB;
reg[31:0]	tlbAccB;
reg			tlbIs32b;

wire[15:0]	tTlbExc;

wire		regInIsLDTLB;
assign		regInIsLDTLB = (regInOpm==UMEM_OPM_LDTLB);

wire		regInIsINVTLB;
assign		regInIsINVTLB = (regInOpm==UMEM_OPM_INVTLB);

MmuChkAcc tlbChkAcc(
	clock,	reset,
	regInMMCR,
	regInKRR,
	regInSR,
	regInOpm,
	tlbAcc,
	tTlbExc,
	regOutNoRwx);

always @*
begin
	tlbDoLdtlb		= 0;
	tRegOutTea		= UV64_XX;
	tlbAcc			= UV32_XX;
	tlbNxtFlushMask	= tlbFlushMask;

	icPageEq		= (tRegInAddr[31:12] == regInAddr[31:12]);

	tlbMmuEnable	= regInMMCR[0];
	tlbIs32b		= regInSR[31];
	if(regInMMCR[2] && !regInSR[30])
		tlbIs32b		= 1;
	if(regInMMCR[3] && regInSR[30])
		tlbIs32b		= 1;
	
	if(tlbMmuEnable)
	begin
//		$display("TLB Enabled");
	end
		
	tlbHixA = regInAddr[17:12]^regInAddr[21:16];
	tlbHixB = regInAddrB[17:12]^regInAddrB[21:16];

	tlbHdatA = { tlbBlkHiA[tlbHixA], tlbBlkLoA[tlbHixA]};
	tlbHdatB = { tlbBlkHiB[tlbHixA], tlbBlkLoB[tlbHixA]};
	tlbHdatC = { tlbBlkHiC[tlbHixA], tlbBlkLoC[tlbHixA]};
	tlbHdatD = { tlbBlkHiD[tlbHixA], tlbBlkLoD[tlbHixA]};

`ifdef jx2_mem_fulldpx
	tlbHdatE = { tlbBlkHiA[tlbHixB], tlbBlkLoA[tlbHixB]};
	tlbHdatF = { tlbBlkHiB[tlbHixB], tlbBlkLoB[tlbHixB]};
	tlbHdatG = { tlbBlkHiC[tlbHixB], tlbBlkLoC[tlbHixB]};
	tlbHdatH = { tlbBlkHiD[tlbHixB], tlbBlkLoD[tlbHixB]};
`else
	tlbHdatE = 0;
	tlbHdatF = 0;
	tlbHdatG = 0;
	tlbHdatH = 0;
`endif

	if(tlbFlushMask[tlbHixA])
	begin
		tlbHdatA[0] = 0;
		tlbHdatB[0] = 0;
		tlbHdatC[0] = 0;
		tlbHdatD[0] = 0;
	end

`ifdef jx2_mem_fulldpx
	if(tlbFlushMask[tlbHixB])
	begin
		tlbHdatE[0] = 0;
		tlbHdatF[0] = 0;
		tlbHdatG[0] = 0;
		tlbHdatH[0] = 0;
	end
`endif
	
	tlbHitA = (regInAddr [31:12] == tlbHdatA[95:76]) && tlbHdatA[0];
	tlbHitB = (regInAddr [31:12] == tlbHdatB[95:76]) && tlbHdatB[0];
	tlbHitC = (regInAddr [31:12] == tlbHdatC[95:76]) && tlbHdatC[0];
	tlbHitD = (regInAddr [31:12] == tlbHdatD[95:76]) && tlbHdatD[0];

`ifdef jx2_mem_fulldpx
	tlbHitE = (regInAddrB[31:12] == tlbHdatE[95:76]) && tlbHdatE[0];
	tlbHitF = (regInAddrB[31:12] == tlbHdatF[95:76]) && tlbHdatF[0];
	tlbHitG = (regInAddrB[31:12] == tlbHdatG[95:76]) && tlbHdatG[0];
	tlbHitH = (regInAddrB[31:12] == tlbHdatH[95:76]) && tlbHdatH[0];
`else
	tlbHitE = 1;
	tlbHitF = 1;
	tlbHitG = 1;
	tlbHitH = 1;
`endif
	
	tlbHitAB = tlbHitA || tlbHitB;
	tlbHitCD = tlbHitC || tlbHitD;
	tlbHitEF = tlbHitE || tlbHitF;
	tlbHitGH = tlbHitG || tlbHitH;
//	tlbHit = tlbHitAB || tlbHitCD;
	tlbHit = (tlbHitAB || tlbHitCD) && (tlbHitEF || tlbHitGH);

	tlbAddrAB =
		tlbHitA ? { tlbHdatA[31:12], regInAddr[11:0] } :
			{ tlbHdatB[31:12], regInAddr[11:0] };
	tlbAccAB = 
		tlbHitA ? { tlbHdatA[127:112], tlbHdatA[75:64], tlbHdatA[7:4] } :
			{ tlbHdatB[127:112], tlbHdatB[75:64], tlbHdatB[7:4] };

	tlbAddrCD =
		tlbHitC ? { tlbHdatC[31:12], regInAddr[11:0] } :
			{ tlbHdatD[31:12], regInAddr[11:0] };
	tlbAccCD = 
		tlbHitC ? { tlbHdatC[127:112], tlbHdatC[75:64], tlbHdatC[7:4] } :
			{ tlbHdatD[127:112], tlbHdatD[75:64], tlbHdatD[7:4] };

	tlbAddr = tlbHitAB ? tlbAddrAB : tlbAddrCD;
	tlbAcc = tlbHitAB ? tlbAccAB : tlbAccCD;

`ifdef jx2_mem_fulldpx
	tlbAddrEF =
		tlbHitE ? { tlbHdatE[31:12], regInAddrB[11:0] } :
			{ tlbHdatF[31:12], regInAddrB[11:0] };
	tlbAccEF = 
		tlbHitE ? { tlbHdatE[127:112], tlbHdatE[75:64], tlbHdatE[7:4] } :
			{ tlbHdatF[127:112], tlbHdatF[75:64], tlbHdatF[7:4] };

	tlbAddrGH =
		tlbHitG ? { tlbHdatG[31:12], regInAddrB[11:0] } :
			{ tlbHdatH[31:12], regInAddrB[11:0] };
	tlbAccGH = 
		tlbHitG ? { tlbHdatG[127:112], tlbHdatG[75:64], tlbHdatG[7:4] } :
			{ tlbHdatH[127:112], tlbHdatH[75:64], tlbHdatH[7:4] };

	tlbAddrB = tlbHitEF ? tlbAddrEF : tlbAddrGH;
	tlbAccB = tlbHitEF ? tlbAccEF : tlbAccGH;
`else
	tlbAddrB = regInAddrB;
	tlbAccB = 0;
`endif

	tlbMmuSkip = 0;
	if(regInAddr[31])
		tlbMmuSkip = 1;

	if(tlbMmuEnable && !tlbMmuSkip)
	begin
		tlbMiss = ! tlbHit;
	end
	else
	begin
		tlbAddr		= regInAddr [31:0];
		tlbAddrB	= regInAddrB[31:0];
		tlbMiss		= 0;
	end

	tRegOutExc = 0;
//	tRegOutTea[31:0] = regInAddr;
	tRegOutTea[31:0] = (tlbHitAB || tlbHitCD) ? regInAddrB : regInAddr;

	if(tlbMmuEnable)
	begin
		if(!tlbMmuSkip)
		begin
			if(tTlbExc[15])
			begin
				tRegOutExc = tTlbExc;
			end
		end
		else
		begin
			if(!regInSR[30])
			begin
				if(regInOpm[4])
					tRegOutExc	= 16'h8002;
				if(regInOpm[3])
					tRegOutExc	= 16'h8001;
			end
		end
	end
		
	

	if(tlbMiss)
	begin
		tRegOutExc = 16'hA001;
	end
	
	tRegOutAddr  = tlbAddr;
	tRegOutAddrB = tlbAddrB;
	tRegOutOpm   = regInOpm;

	if(regInAddr[31:0]!=tlbAddr[31:0])
	begin
		$display("TLB(A) %X -> %X", regInAddr, tlbAddr);
	end

	if(regInAddrB[31:0]!=tlbAddrB[31:0])
	begin
		$display("TLB(B) %X -> %X", regInAddrB, tlbAddrB);
	end
	
	if(regInIsINVTLB || reset)
	begin
		tlbNxtFlushMask = UV64_FF;
	end

	if(regInIsLDTLB)
	begin
		$display("MemTile-LDTLB %X %X-%X",
			regInAddr,
			regInData[127:64],
			regInData[ 63: 0]);
		tlbDoLdtlb	= 1;
		tlbNxtFlushMask[tlbHixA] = 0;
		tRegOutOK = tlbLdtlbOK ?
			UMEM_OK_OK : UMEM_OK_HOLD;
	end
	else
	begin
		tRegOutOK = (regInOpm[4:3]!=0) && icPageEq ?
			UMEM_OK_OK : UMEM_OK_READY;

//		$display("TLB Opm=%X Ok=%X", regInOpm, tRegOutOK);
	end

end

always @ (posedge clock)
begin
	tRegOutAddr2	<= tRegOutAddr;
	tRegOutAddrB2	<= tRegOutAddrB;
	tRegOutExc2		<= tRegOutExc;
	tRegOutTea2		<= tRegOutTea;
	tRegOutOK2		<= tRegOutOK;
	tRegOutOpm2		<= tRegOutOpm;

	tRegInAddr		<= regInAddr;
	tRegInAddrB		<= regInAddrB;
	tlbFlushMask	<= tlbNxtFlushMask;

	if(tlbDoLdtlb && !tlbLdtlbOK)
	begin
		tlbBlkHiA[tlbHixA]	<= regInData[127:64];
		tlbBlkLoA[tlbHixA]	<= regInData[ 63: 0];
		tlbBlkHiB[tlbHixA]	<= tlbHdatA[127:64];
		tlbBlkLoB[tlbHixA]	<= tlbHdatA[ 63: 0];
		tlbBlkHiC[tlbHixA]	<= tlbHdatB[127:64];
		tlbBlkLoC[tlbHixA]	<= tlbHdatB[ 63: 0];
		tlbBlkHiD[tlbHixA]	<= tlbHdatC[127:64];
		tlbBlkLoD[tlbHixA]	<= tlbHdatC[ 63: 0];
		
		tlbLdtlbOK	<=	1;
	end
	else
	begin
		tlbLdtlbOK	<= 0;
	end
end

// /* verilator lint_on UNOPTFLAT */


endmodule