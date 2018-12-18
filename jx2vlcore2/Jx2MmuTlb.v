`include "Jx2CoreDefs.v"

`include "Jx2MmuChkAcc.v"

module Jx2MmuTlb(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regOutAddr,
	regInData,		regInOpm,
	regOutExc,		regOutTea,
	regOutOK,		
	
	regInMMCR,		regInKRR,		regInSR
	);

// /* verilator lint_off UNOPTFLAT */

input			clock;			//clock
input			reset;			//reset

input[47:0]		regInAddr;		//input Address
output[47:0]	regOutAddr;		//output Address
input[127:0]	regInData;		//input data (LDTLB)

input[4:0]		regInOpm;		//Operation Size/Type
output[15:0]	regOutExc;		//Raise Exception
output[63:0]	regOutTea;		//Exception TEA
output[1:0]		regOutOK;		//set if operation suceeds

input[63:0]		regInMMCR;		//MMU Control Register
input[63:0]		regInKRR;		//Keyring Register
input[63:0]		regInSR;		//Status Register


reg[47:0]		tRegOutAddr2;
reg[15:0]		tRegOutExc2;
reg[63:0]		tRegOutTea2;
reg[1:0]		tRegOutOK2;

assign		regOutAddr = tRegOutAddr2;
assign		regOutOK = tRegOutOK2;
assign		regOutExc = tRegOutExc2;
assign		regOutTea = tRegOutTea2;

reg[47:0]		tRegOutAddr;
reg[15:0]		tRegOutExc;
reg[63:0]		tRegOutTea;
reg[1:0]		tRegOutOK;

`ifdef JX2_MMU_DO64X
reg[63:0]	tlbBlkHiA[63:0];
reg[63:0]	tlbBlkHiB[63:0];
reg[63:0]	tlbBlkHiC[63:0];
reg[63:0]	tlbBlkHiD[63:0];

reg[63:0]	tlbBlkLoA[63:0];
reg[63:0]	tlbBlkLoB[63:0];
reg[63:0]	tlbBlkLoC[63:0];
reg[63:0]	tlbBlkLoD[63:0];

reg[5:0]	tlbHixA;
reg[5:0]	tlbHixB;

`else
reg[63:0]	tlbBlkHiA[15:0];
reg[63:0]	tlbBlkHiB[15:0];
reg[63:0]	tlbBlkHiC[15:0];
reg[63:0]	tlbBlkHiD[15:0];

reg[63:0]	tlbBlkLoA[15:0];
reg[63:0]	tlbBlkLoB[15:0];
reg[63:0]	tlbBlkLoC[15:0];
reg[63:0]	tlbBlkLoD[15:0];

reg[3:0]	tlbHixA;
reg[3:0]	tlbHixB;
`endif

reg[127:0]	tlbHdatA;
reg[127:0]	tlbHdatB;
reg[127:0]	tlbHdatC;
reg[127:0]	tlbHdatD;

reg			tlbMmuEnable;
reg			tlbMmuSkipA;
reg			tlbMmuSkipB;

reg			tlbHitA;
reg			tlbHitB;
reg			tlbHitC;
reg			tlbHitD;

reg			tlbHitAB;
reg			tlbHitCD;
reg			tlbHit;

reg			tlbMiss;

reg			tlbMmuSkip;

reg			tlbDoLdtlb;
reg			tlbLdtlbOK;

reg[31:0]	tlbAddrAB;
reg[31:0]	tlbAddrCD;
reg[31:0]	tlbAccAB;
reg[31:0]	tlbAccCD;

reg[31:0]	tlbAddr;
reg[31:0]	tlbAcc;
reg			tlbIs32b;

wire[15:0]	tTlbExc;

wire		regInIsLDTLB;
assign		regInIsLDTLB = (regInOpm==UMEM_OPM_LDTLB);

// wire[15:0]	tTlbExcC;
// assign		tTlbExcC = (tTlbExcA[15]) ? tTlbExcA : tTlbExcB;

Jx2MmuChkAcc tlbChkAcc(
	clock,	reset,
	regInMMCR,
	regInKRR,
	regInSR,
	regInOpm,
	tlbAcc,
	tTlbExc);

always @*
begin
//	tMemTile		= 0;
//	tOutData		= 0;
//	tNextTile		= 0;

	tlbDoLdtlb		= 0;
	tRegOutTea		= UV64_XX;
	tlbAcc			= UV32_XX;

//	icReqLow4GB	= (regInAddr[47:32]==16'h0000);
//	icBlkBypass	= (regInAddr[47:45]==3'b101) ||
//		((regInAddr[31:29]==3'b101) && icReqLow4GB);
	
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
		
`ifdef JX2_MMU_DO64X
	tlbHixA = regInAddr[17:12]^regInAddr[21:16];
	tlbHixB = regOutAddr[17:12]^regOutAddr[21:16];
`else
	tlbHixA = regInAddr[15:12]^regInAddr[19:16];
	tlbHixB = regOutAddr[15:12]^regOutAddr[19:16];
`endif

	tlbHdatA = { tlbBlkHiA[tlbHixA], tlbBlkLoA[tlbHixA]};
	tlbHdatB = { tlbBlkHiB[tlbHixA], tlbBlkLoB[tlbHixA]};
	tlbHdatC = { tlbBlkHiC[tlbHixA], tlbBlkLoC[tlbHixA]};
	tlbHdatD = { tlbBlkHiD[tlbHixA], tlbBlkLoD[tlbHixA]};
	
//	tlbHitA = (regInAddr [47:12] == tlbHdatA[111:76]) && tlbHdatA[0];
//	tlbHitB = (regInAddr [47:12] == tlbHdatB[111:76]) && tlbHdatB[0];
//	tlbHitC = (regInAddr [47:12] == tlbHdatC[111:76]) && tlbHdatC[0];
//	tlbHitD = (regInAddr [47:12] == tlbHdatD[111:76]) && tlbHdatD[0];

	tlbHitA = ((regInAddr [47:32] == tlbHdatA[111:96]) || tlbIs32b) &&
		(regInAddr [31:12] == tlbHdatA[95:76]) && tlbHdatA[0];
	tlbHitB = ((regInAddr [47:32] == tlbHdatB[111:96]) || tlbIs32b) &&
		(regInAddr [31:12] == tlbHdatB[95:76]) && tlbHdatB[0];
	tlbHitC = ((regInAddr [47:32] == tlbHdatC[111:96]) || tlbIs32b) &&
		(regInAddr [31:12] == tlbHdatC[95:76]) && tlbHdatC[0];
	tlbHitD = ((regInAddr [47:32] == tlbHdatD[111:96]) || tlbIs32b) &&
		(regInAddr [31:12] == tlbHdatD[95:76]) && tlbHdatD[0];
	
	tlbHitAB = tlbHitA || tlbHitB;
	tlbHitCD = tlbHitC || tlbHitD;
	tlbHit = tlbHitAB || tlbHitCD;

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

	tlbMmuSkip = 0;
	if(regInAddr[47:32]==16'h0000)
		if(regInAddr[31])
			tlbMmuSkip = 1;

	if(tlbMmuEnable && !tlbMmuSkip)
	begin
//		tlbAddr =
//			tlbHitA ? { tlbHdatA[31:12], regInAddr[11:0] } :
//			tlbHitB ? { tlbHdatB[31:12], regInAddr[11:0] } :
//			tlbHitC ? { tlbHdatC[31:12], regInAddr[11:0] } :
//			tlbHitD ? { tlbHdatD[31:12], regInAddr[11:0] } :
//			regInAddr[31:0];
//		tlbAcc = 
//			tlbHitA ? { tlbHdatA[127:112], tlbHdatA[75:64], tlbHdatA[7:4] } :
//			tlbHitB ? { tlbHdatB[127:112], tlbHdatB[75:64], tlbHdatB[7:4] } :
//			tlbHitC ? { tlbHdatC[127:112], tlbHdatC[75:64], tlbHdatC[7:4] } :
//			tlbHitD ? { tlbHdatD[127:112], tlbHdatD[75:64], tlbHdatD[7:4] } :
//			0;
//		tlbMiss = !( tlbHitA || tlbHitB || tlbHitC || tlbHitD );
		tlbMiss = ! tlbHit;
	end
	else
	begin
		tlbAddr = regInAddr[31:0];
		tlbMiss = 0;
	end


//	tRegInOpmA = regInOpm;
//	tRegInOpmB = regInOpm;
//	tRegInOpmA[4] = 0;
//	tRegInOpmB[3] = 0;
	
//	tRegOutExc = tTlbExcC;

	tRegOutExc = 0;
//	tRegOutTea[63:48] = 0;
	tRegOutTea[47:0] = regInAddr;

	if(tlbMmuEnable)
	begin
		if(!tlbMmuSkip)
		begin
			if(tTlbExc[15])
			begin
				tRegOutExc = tTlbExc;
	//			tRegOutTea[47:0] = regInAddr;
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
//		tRegOutTea[47:0] = regInAddr;
	end
	
	tRegOutAddr = { UV16_00, tlbAddr };

	if(regInIsLDTLB)
	begin
		$display("MemTile-LDTLB %X %X-%X",
			regInAddr,
			regInData[127:64],
			regInData[ 63: 0]);
		tlbDoLdtlb	= 1;
		tRegOutOK = tlbLdtlbOK ?
			UMEM_OK_OK : UMEM_OK_HOLD;
	end
	else
	begin
//		tRegOutOK = UMEM_OK_READY;
		tRegOutOK = (regInOpm[4:3]!=0) ?
			UMEM_OK_OK : UMEM_OK_READY;

//		$display("TLB Opm=%X Ok=%X", regInOpm, tRegOutOK);
	end

end

always @ (posedge clock)
begin
	tRegOutAddr2	<= tRegOutAddr;
	tRegOutExc2		<= tRegOutExc;
	tRegOutTea2		<= tRegOutTea;
	tRegOutOK2		<= tRegOutOK;

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