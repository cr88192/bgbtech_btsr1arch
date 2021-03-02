/*
TLB:
Sits between the L1 and L2 caches.

Filters output from the L1's memory interface, so that L1 operates with virtual addresses and L2 with physical addresses.

 */

`include "RbiDefs.v"

`include "RbiMmuChkAcc.v"

module RbiMmuTlb(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regOutAddr,
	regInData,		regOutData,
	regInOpm,		regOutOpm,
	regInSeq,		regOutSeq,
	regOutExc,		regInHold,
	regInMMCR,		regInKRR,		regInSR
	);

input			clock;			//clock
input			reset;			//reset

input[47:0]		regInAddr;		//input Address
output[47:0]	regOutAddr;		//output Address
input[127:0]	regInData;		//input cache line
output[127:0]	regOutData;		//output cache line
input[15:0]		regInOpm;		//Operation Size/Type
output[15:0]	regOutOpm;		//Operation Size/Type
input[15:0]		regInSeq;		//Operation Size/Type
output[15:0]	regOutSeq;		//Operation Size/Type

output[63:0]	regOutExc;		//Exception EXC+TEA
input			regInHold;		//Hold Pipeline

input[63:0]		regInMMCR;		//MMU Control Register
input[63:0]		regInKRR;		//Keyring Register
input[63:0]		regInSR;		//Status Register



reg[47:0]		tRegOutAddr;
reg[47:0]		tRegOutAddr2;
reg[47:0]		tRegOutAddr3;
reg[127:0]		tRegOutData;		//output cache line
reg[127:0]		tRegOutData2;		//output cache line
reg[127:0]		tRegOutData3;		//output cache line
reg[15:0]		tRegOutOpm;
reg[15:0]		tRegOutOpm2;
reg[15:0]		tRegOutOpm3;
reg[15:0]		tRegOutSeq;
reg[15:0]		tRegOutSeq2;
reg[15:0]		tRegOutSeq3;

assign		regOutAddr = tRegOutAddr3;
assign		regOutData = tRegOutData3;
assign		regOutOpm = tRegOutOpm3;
assign		regOutSeq = tRegOutSeq3;

reg[15:0]		tRegOutExc;
reg[15:0]		tRegOutExc2;
reg[63:0]		tRegOutTea;
reg[63:0]		tRegOutTea2;

assign		regOutExc = { tRegOutTea2[47:0], tRegOutExc2 };


reg[47:0]		regInAddrA;		//input Address

reg[47:0]		tRegInAddr;	//input Address
reg[15:0]		tRegInOpm;		//Operation Size/Type
reg[15:0]		tRegInSeq;		//
reg[127:0]		tRegInData;		//output cache line

`ifdef jx2_expand_tlb

reg[143:0]	tlbBlkA[255:0];
reg[143:0]	tlbBlkB[255:0];
reg[143:0]	tlbBlkC[255:0];
reg[143:0]	tlbBlkD[255:0];

reg[7:0]	tlbHixA;
reg[7:0]	tlbHixB;
reg[7:0]	tlbHixSelA;
reg[7:0]	tlbHixSelB;

reg[7:0]	tlbHbIxA;
reg[7:0]	tlbHbIxB;

`else

(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkA[63:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkB[63:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkC[63:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkD[63:0];

reg[5:0]	tlbHixA;
reg[5:0]	tlbHixB;
reg[5:0]	tlbHixSelA;
reg[5:0]	tlbHixSelB;

reg[5:0]	tlbHbIxA;
reg[5:0]	tlbHbIxB;

`endif

reg[143:0]	tlbHbDatA;
reg[143:0]	tlbHbDatB;
reg[143:0]	tlbHbDatC;
reg[143:0]	tlbHbDatD;

reg[143:0]	tlbHdatA;
reg[143:0]	tlbHdatB;
reg[143:0]	tlbHdatC;
reg[143:0]	tlbHdatD;

reg			tlbMmuEnable;
reg			tlbMmuAddr48;
reg			tlbMmuPg64K;
reg			tlbMmuPg16K;
reg			tlbMmuPg4K;

reg			tlbMmuSkipA;
reg			tlbMmuSkipB;

reg			tlbHitA_Hi;
reg			tlbHitB_Hi;
reg			tlbHitC_Hi;
reg			tlbHitD_Hi;
reg			tlbHitA_Mi;
reg			tlbHitB_Mi;
reg			tlbHitC_Mi;
reg			tlbHitD_Mi;
reg			tlbHitA_Lo;
reg			tlbHitB_Lo;
reg			tlbHitC_Lo;
reg			tlbHitD_Lo;

reg			tlbHitA_LoH;
reg			tlbHitB_LoH;
reg			tlbHitC_LoH;
reg			tlbHitD_LoH;
reg			tlbHitA_LoL;
reg			tlbHitB_LoL;
reg			tlbHitC_LoL;
reg			tlbHitD_LoL;

reg			tlbHitA;
reg			tlbHitB;
reg			tlbHitC;
reg			tlbHitD;

reg			tlbHitAB;
reg			tlbHitCD;
reg			tlbHit;

reg			tlbMiss;

reg			tlbMmuSkip;

reg[143:0]	tlbLdHdatA;
reg[143:0]	tlbLdHdatB;
reg[143:0]	tlbLdHdatC;
reg[143:0]	tlbLdHdatD;

reg			tlbDoLdtlb;
reg			tlbLdtlbOK;

reg			icPageReady;
reg			icPageEq;

reg[47:0]	tlbAddrAB;
reg[47:0]	tlbAddrCD;
reg[31:0]	tlbAccAB;
reg[31:0]	tlbAccCD;

reg[47:0]	tlbAddr;
reg[31:0]	tlbAcc;
reg[47:0]	tlbAddrB;
reg[31:0]	tlbAccB;
reg			tlbIs32b;
reg			tlbIs48b;

reg			tAddrIsMMIO;
reg			tAddrIsLo4G;
reg			tAddrIsHi4G;
reg			tAddrIsPhys;

wire[15:0]	tTlbExc;

reg[7:0]	tTlbRov;
reg[7:0]	tNxtTlbRov;

wire		regInIsREADY;
assign		regInIsREADY = (tRegInOpm[7:0]==JX2_RBI_OPM_IDLE);

wire		regInIsLDTLB;
assign		regInIsLDTLB = (tRegInOpm[7:0]==JX2_RBI_OPM_LDTLB);

wire		regInIsINVTLB;
assign		regInIsINVTLB = (tRegInOpm[7:0]==JX2_RBI_OPM_INVTLB);

reg		regInIsBounce;

wire[5:0]	tChkAccNoRwx;	//No R/W/X

RbiMmuChkAcc tlbChkAcc(
	clock,	reset,
	regInHold,
	regInMMCR,
	regInKRR,
	regInSR,
	regInOpm[7:0],
	tlbAcc,
	tTlbExc,
	tChkAccNoRwx);

always @*
begin
	tlbDoLdtlb		= 0;
	tRegOutTea		= UV64_00;
	tlbAcc			= UV32_00;
	tNxtTlbRov		= tTlbRov;

	tlbMmuEnable	= regInMMCR[0];
	tlbMmuPg64K		= regInMMCR[4];
	tlbMmuPg16K		= regInMMCR[5];
	tlbMmuPg4K		= !(tlbMmuPg16K || tlbMmuPg64K);

	tlbIs48b		= regInSR[31];
	if(regInMMCR[2] && !regInSR[30])
		tlbIs48b	= 1;
	if(regInMMCR[3] && regInSR[30])
		tlbIs48b	= 1;
	tlbIs32b		= !tlbIs48b;

	tRegOutData		= tRegInData;

	regInAddrA		= regInAddr[47:0];

	if(regInOpm[7:0]==JX2_RBI_OPM_LDTLB)
	begin
		regInAddrA		= regInData[111:64];
	end

`ifdef jx2_expand_tlb

	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
			tlbHixSelA={regInAddrA[15:12], regInAddrA[27:24]};
		end
		2'b01: begin
			tlbHixSelA=regInAddrA[31:24];
		end
		2'b10: begin
			tlbHixSelA={regInAddrA[15:14], regInAddrA[29:24]};
		end
		2'b11: begin
			tlbHixSelA=regInAddrA[31:24];
		end
	endcase
	tlbHixA = tlbHixSelA ^ regInAddrA[23:16];

`else
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
			tlbHixSelA={regInAddrA[15:12], regInAddrA[23:22]};
		end
		2'b01: begin
			tlbHixSelA=regInAddrA[27:22];
		end
		2'b10: begin
			tlbHixSelA={regInAddrA[15:14], regInAddrA[25:22]};
		end
		2'b11: begin
			tlbHixSelA=regInAddrA[27:22];
		end
	endcase
	tlbHixA = tlbHixSelA[5:0] ^ regInAddrA[21:16];
`endif



//	icPageEq		= (tRegInAddr[47:12] == regInAddrA[47:12]);

	tAddrIsLo4G		= (tRegInAddr[47:32] == 16'h0000);
	tAddrIsHi4G		= (tRegInAddr[47:32] == 16'hFFFF);
	tAddrIsMMIO		= (tRegInAddr[31:28] == 4'hF) &&
		(tAddrIsLo4G || tAddrIsHi4G || tlbIs32b);
	tAddrIsPhys		= tAddrIsHi4G && !tRegInAddr[31];

//	regInIsBounce	= (tRegInOpm == UMEM_OPM_RD_BOUNCE);

	if(regInSR[29] && regInSR[28])
	begin
//		$display("TLB Disable ISR");
		tlbMmuEnable = 0;
	end
	
	if(tlbMmuEnable)
	begin
//		$display("TLB Enabled");
	end



//	icPageReady		= (tlbHbIxA == tlbHixA);
//	if(!icPageEq)
//		icPageReady = 0;

	tlbHdatA = tlbHbDatA;
	tlbHdatB = tlbHbDatB;
	tlbHdatC = tlbHbDatC;
	tlbHdatD = tlbHbDatD;

	tlbLdHdatA = tlbHdatA;
	tlbLdHdatB = tlbHdatB;
	tlbLdHdatC = tlbHdatC;
	tlbLdHdatD = tlbHdatD;

//	if(tlbFlushMask[tlbHixA])
//	begin
//		tlbHdatA[0] = 0;
//		tlbHdatB[0] = 0;
//		tlbHdatC[0] = 0;
//		tlbHdatD[0] = 0;
//	end

	if(tlbHbDatA[135:128] != tTlbRov)
		tlbHdatA[0] = 0;
	if(tlbHbDatB[135:128] != tTlbRov)
		tlbHdatB[0] = 0;
	if(tlbHbDatC[135:128] != tTlbRov)
		tlbHdatC[0] = 0;
	if(tlbHbDatD[135:128] != tTlbRov)
		tlbHdatD[0] = 0;


`ifdef def_true
	tlbHitA_Hi = (tRegInAddr[47:32] == tlbHdatA[111:96]);
	tlbHitB_Hi = (tRegInAddr[47:32] == tlbHdatB[111:96]);
	tlbHitC_Hi = (tRegInAddr[47:32] == tlbHdatC[111:96]);
	tlbHitD_Hi = (tRegInAddr[47:32] == tlbHdatD[111:96]);
	tlbHitA_Mi = (tRegInAddr[31:16] == tlbHdatA[95:80]);
	tlbHitB_Mi = (tRegInAddr[31:16] == tlbHdatB[95:80]);
	tlbHitC_Mi = (tRegInAddr[31:16] == tlbHdatC[95:80]);
	tlbHitD_Mi = (tRegInAddr[31:16] == tlbHdatD[95:80]);

	tlbHitA_LoH = (tRegInAddr[15:14] == tlbHdatA[79:78]);
	tlbHitB_LoH = (tRegInAddr[15:14] == tlbHdatB[79:78]);
	tlbHitC_LoH = (tRegInAddr[15:14] == tlbHdatC[79:78]);
	tlbHitD_LoH = (tRegInAddr[15:14] == tlbHdatD[79:78]);
	tlbHitA_LoL = (tRegInAddr[13:12] == tlbHdatA[77:76]);
	tlbHitB_LoL = (tRegInAddr[13:12] == tlbHdatB[77:76]);
	tlbHitC_LoL = (tRegInAddr[13:12] == tlbHdatC[77:76]);
	tlbHitD_LoL = (tRegInAddr[13:12] == tlbHdatD[77:76]);
	tlbHitA_Lo = tlbHitA_LoH && (tlbHitA_LoL || tlbMmuPg16K);
	tlbHitB_Lo = tlbHitB_LoH && (tlbHitB_LoL || tlbMmuPg16K);
	tlbHitC_Lo = tlbHitC_LoH && (tlbHitC_LoL || tlbMmuPg16K);
	tlbHitD_Lo = tlbHitD_LoH && (tlbHitD_LoL || tlbMmuPg16K);
`endif

	if(tlbIs32b)
	begin
		tlbHitA_Hi = 1;
		tlbHitB_Hi = 1;
		tlbHitC_Hi = 1;
		tlbHitD_Hi = 1;
	end

	if(tlbMmuPg64K)
	begin
		tlbHitA_Lo = 1;
		tlbHitB_Lo = 1;
		tlbHitC_Lo = 1;
		tlbHitD_Lo = 1;
	end
	
	tlbHitA = tlbHitA_Hi && tlbHitA_Mi && tlbHitA_Lo && tlbHdatA[0];
	tlbHitB = tlbHitB_Hi && tlbHitB_Mi && tlbHitB_Lo && tlbHdatB[0];
	tlbHitC = tlbHitC_Hi && tlbHitC_Mi && tlbHitC_Lo && tlbHdatC[0];
	tlbHitD = tlbHitD_Hi && tlbHitD_Mi && tlbHitD_Lo && tlbHdatD[0];

	tlbHitAB = tlbHitA || tlbHitB;
	tlbHitCD = tlbHitC || tlbHitD;
	tlbHit = tlbHitAB || tlbHitCD;

	tlbAddrAB[47:12] = tlbHitA ? tlbHdatA[47:12] : tlbHdatB[47:12];
	tlbAddrCD[47:12] = tlbHitC ? tlbHdatC[47:12] : tlbHdatD[47:12];

	tlbAddr[47:12] = tlbHitAB ? tlbAddrAB[47:12] : tlbAddrCD[47:12];
	tlbAddr[11: 0] = tRegInAddr[11: 0];

	if(!tlbMmuPg4K)
		tlbAddr[13:12] = tRegInAddr[13:12];
	if(tlbMmuPg64K)
		tlbAddr[15:14] = tRegInAddr[15:14];

	tlbAccAB = 
		tlbHitA ? { tlbHdatA[127:112], tlbHdatA[75:64], tlbHdatA[7:4] } :
			{ tlbHdatB[127:112], tlbHdatB[75:64], tlbHdatB[7:4] };
	tlbAccCD = 
		tlbHitC ? { tlbHdatC[127:112], tlbHdatC[75:64], tlbHdatC[7:4] } :
			{ tlbHdatD[127:112], tlbHdatD[75:64], tlbHdatD[7:4] };
	tlbAcc = tlbHitAB ? tlbAccAB : tlbAccCD;

	tlbMiss = 0;

	tlbMmuSkip = 0;
	if(tAddrIsMMIO)
		tlbMmuSkip = 1;
	
	if(tAddrIsPhys)
		tlbMmuSkip = 1;

	if(tlbMmuEnable && !tlbMmuSkip && (tRegInOpm[4:3]!=0))
	begin
		tlbMiss = ! tlbHit;
		
		if(tlbMiss)
		begin
			if(regInSR[29] && regInSR[28])
			begin
				$display("Miss while in ISR");
			end

//			$display("Miss A=%X B=%X SR=%X_%X",
//				tRegInAddr,	tRegInAddrB,
//				regInSR[63:32], regInSR[31:0]);

			tlbAddr		= 48'h010000;
		end
	end
	else
	begin
		tlbAddr		= tRegInAddr[47:0];
		if(tAddrIsPhys && tAddrIsHi4G)
			tlbAddr[47:32]	= 0;
		tlbMiss		= 0;
	end

	tRegOutExc = 0;

	tRegOutTea[47:0] = tRegInAddr;

	if(tlbMmuEnable)
	begin
		if(!tlbMmuSkip)
		begin
			if(tTlbExc[15])
			begin
				tRegOutExc = tTlbExc;
			end
		end
	end
	
	tRegOutAddr  = tlbAddr;
	tRegOutOpm   = tRegInOpm;

	if(tlbMmuEnable &&
		(tRegInOpm[5:4]!=0) && !tlbMmuSkip)
	begin
		if(tlbMiss)
		begin
//			$display("TLB Miss %X", regInAddr);
			tRegOutExc = 16'hA001;
			tRegOutOpm[11:8] = 4'hF;
		end
		else
		begin
	//		$display("TLB Hit %X", regInAddr);
		end
	end


	if((tRegInAddr[47:0]!=tlbAddr[47:0]) && !tAddrIsPhys)
	begin
		$display("TLB(A) %X -> %X", tRegInAddr, tlbAddr);
	end

	if(regInIsBounce && !tAddrIsMMIO)
	begin
//		tRegOutOpm   = UMEM_OPM_LDTLB;
	end
	
	if(regInIsINVTLB || reset)
	begin
		tNxtTlbRov		= tTlbRov + 1;
	end

	if(reset)
	begin
		tRegOutExc = 0;
		tRegOutOpm   = UV16_00;
	end

	if(regInIsLDTLB)
	begin
		$display("MemTile-LDTLB %X %X-%X",
			tRegInAddr,
			tRegInData[127:64],
			tRegInData[ 63: 0]);
		tlbDoLdtlb	= icPageReady;

		tlbLdHdatA = { 8'h00, tTlbRov, tRegInData[127:0] };
		tlbLdHdatB = tlbHdatA;
		tlbLdHdatC = tlbHdatB;
		tlbLdHdatD = tlbHdatC;

	end
	else
	begin

//		$display("TLB Opm=%X Ok=%X", regInOpm, tRegOutOK);

`ifdef def_true
		if(tlbMmuEnable && icPageReady &&
			!tlbMiss &&
			(tRegInOpm[4:3]!=0) && !tlbMmuSkip)
		begin
			/* Special: Shuffle TLBE's based on TLB Hits.
			 * This should slightly improve hit rate.
			 */
			if(tlbHitA)
			begin
				/* No Op */
			end
			else if(tlbHitB)
			begin
				tlbLdHdatA	= tlbHdatB;
				tlbLdHdatB	= tlbHdatA;
				tlbLdHdatC	= tlbHdatC;
				tlbLdHdatD	= tlbHdatD;
				tlbDoLdtlb	= 1;
			end
			else if(tlbHitC)
			begin
				tlbLdHdatA	= tlbHdatC;
				tlbLdHdatB	= tlbHdatA;
				tlbLdHdatC	= tlbHdatB;
				tlbLdHdatD	= tlbHdatD;
				tlbDoLdtlb	= 1;
			end
			else if(tlbHitD)
			begin
				tlbLdHdatA	= tlbHdatD;
				tlbLdHdatB	= tlbHdatA;
				tlbLdHdatC	= tlbHdatB;
				tlbLdHdatD	= tlbHdatC;
				tlbDoLdtlb	= 1;
			end
		end
`endif

	end

end

always @ (posedge clock)
begin
	if(!regInHold)
	begin
		tRegOutAddr2	<= tRegOutAddr;
		tRegOutData2	<= tRegOutData;
	//	tRegOutOpm2		<= tRegOutOpm;
		tRegOutOpm2		<= { 2'b00, tChkAccNoRwx, tRegOutOpm[7:0] };
		tRegOutSeq2		<= tRegOutSeq;

		tRegOutAddr3	<= tRegOutAddr2;
		tRegOutData3	<= tRegOutData2;
		tRegOutOpm3		<= tRegOutOpm2;
		tRegOutSeq3		<= tRegOutSeq2;

		tRegOutExc2		<= tRegOutExc;
		tRegOutTea2		<= tRegOutTea;

		tRegInAddr		<= regInAddr;
		tRegInData		<= regInData;
		tRegInOpm		<= regInOpm;
		tRegInSeq		<= regInSeq;

		tTlbRov			<= tNxtTlbRov;

		tlbHbIxA		<= tlbHixA;
		tlbHbIxB		<= tlbHixB;
	end

	tlbHbDatA		<= tlbBlkA[tlbHixA];
	tlbHbDatB		<= tlbBlkB[tlbHixA];
	tlbHbDatC		<= tlbBlkC[tlbHixA];
	tlbHbDatD		<= tlbBlkD[tlbHixA];

	if(tlbDoLdtlb && !tlbLdtlbOK)
	begin
//		$display("Do LdTlb %X-%X Ix=%X",
//			regTlbData[127:64], regTlbData[63:0], tlbHixA);

		tlbBlkA[tlbHbIxA]	<= tlbLdHdatA;
		tlbBlkB[tlbHbIxA]	<= tlbLdHdatB;
		tlbBlkC[tlbHbIxA]	<= tlbLdHdatC;
		tlbBlkD[tlbHbIxA]	<= tlbLdHdatD;

		tlbLdtlbOK	<=	1;
	end
	else
	begin
		tlbLdtlbOK	<= 0;
	end
end

endmodule
