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
	regInAddrB,		regOutAddrB,	regTlbData,
	regInData,		regOutData,
	regInOpm,		regOutOpm,
	regOutExc,		regOutOK,		regOutNoRwx,
	regInMMCR,		regInKRR,		regInSR
	);

// /* verilator lint_off UNOPTFLAT */

input			clock;			//clock
input			reset;			//reset

input[47:0]		regInAddr;		//input Address (Primary)
output[47:0]	regOutAddr;		//output Address (Primary)
input[47:0]		regInAddrB;		//input Address (Secondary)
output[47:0]	regOutAddrB;	//output Address (Secondary)
input[127:0]	regTlbData;		//input data (LDTLB)

`input_tile		regInData;		//input cache line
`output_tile	regOutData;		//output cache line

input[4:0]		regInOpm;		//Operation Size/Type
// output[15:0]	regOutExc;		//Raise Exception
// output[63:0]	regOutTea;		//Exception TEA
output[63:0]	regOutExc;		//Exception EXC+TEA
output[1:0]		regOutOK;		//set if operation suceeds
output[4:0]		regOutOpm;		//Operation Size/Type
output[5:0]		regOutNoRwx;	//No R/W/X

input[63:0]		regInMMCR;		//MMU Control Register
input[63:0]		regInKRR;		//Keyring Register
input[63:0]		regInSR;		//Status Register


reg[47:0]		tRegOutAddr2;
reg[47:0]		tRegOutAddrB2;
reg[15:0]		tRegOutExc2;
reg[63:0]		tRegOutTea2;
reg[1:0]		tRegOutOK2;
reg[4:0]		tRegOutOpm2;

`reg_tile		tRegOutData;		//output cache line
`reg_tile		tRegOutData2;		//output cache line

`ifndef jx2_tlb_nofwmemout
assign		regOutAddr = tRegOutAddr2;
assign		regOutAddrB = tRegOutAddrB2;
assign		regOutOK = tRegOutOK2;
assign		regOutOpm = tRegOutOpm2;
// assign		regOutExc = tRegOutExc2;
// assign		regOutTea = tRegOutTea2;
assign		regOutData = tRegOutData2;
`endif

assign		regOutExc = { tRegOutTea2[47:0], tRegOutExc2 };

`ifdef jx2_tlb_nofwmemout
assign		regOutAddr = tRegOutAddr;
assign		regOutAddrB = tRegOutAddrB;
assign		regOutOK = tRegOutOK;
assign		regOutOpm = tRegOutOpm;
// assign		regOutExc = tRegOutExc;
// assign		regOutTea = tRegOutTea;
assign		regOutData = tRegOutData;

// assign		regOutExc = { tRegOutTea[47:0], tRegOutExc };
`endif

reg[47:0]		tRegOutAddr;
reg[47:0]		tRegOutAddrB;
reg[15:0]		tRegOutExc;
reg[63:0]		tRegOutTea;
reg[1:0]		tRegOutOK;
reg[4:0]		tRegOutOpm;

reg[47:0]		regInAddrA;		//input Address

reg[47:0]		tRegInAddrA;	//input Address
reg[47:0]		tRegInAddrB;	//input Address
reg[4:0]		tRegInOpm;		//Operation Size/Type
`reg_tile		tRegInData;		//output cache line

`ifdef jx2_expand_tlb

reg[63:0]	tlbBlkHiA[255:0];
reg[63:0]	tlbBlkHiB[255:0];
reg[63:0]	tlbBlkHiC[255:0];
reg[63:0]	tlbBlkHiD[255:0];

reg[63:0]	tlbBlkLoA[255:0];
reg[63:0]	tlbBlkLoB[255:0];
reg[63:0]	tlbBlkLoC[255:0];
reg[63:0]	tlbBlkLoD[255:0];

reg[255:0]	tlbFlushMask;
reg[255:0]	tlbNxtFlushMask;

reg[7:0]	tlbHixA;
reg[7:0]	tlbHixB;
reg[7:0]	tlbHixSelA;
reg[7:0]	tlbHixSelB;

reg[7:0]	tlbHbIxA;
reg[7:0]	tlbHbIxB;

`else

(* ram_style = "distributed" *)
	reg[63:0]	tlbBlkHiA[63:0];
(* ram_style = "distributed" *)
	reg[63:0]	tlbBlkHiB[63:0];
(* ram_style = "distributed" *)
	reg[63:0]	tlbBlkHiC[63:0];
(* ram_style = "distributed" *)
	reg[63:0]	tlbBlkHiD[63:0];

(* ram_style = "distributed" *)
	reg[63:0]	tlbBlkLoA[63:0];
(* ram_style = "distributed" *)
	reg[63:0]	tlbBlkLoB[63:0];
(* ram_style = "distributed" *)
	reg[63:0]	tlbBlkLoC[63:0];
(* ram_style = "distributed" *)
	reg[63:0]	tlbBlkLoD[63:0];

reg[63:0]	tlbFlushMask;
reg[63:0]	tlbNxtFlushMask;

reg[5:0]	tlbHixA;
reg[5:0]	tlbHixB;
reg[5:0]	tlbHixSelA;
reg[5:0]	tlbHixSelB;

reg[5:0]	tlbHbIxA;
reg[5:0]	tlbHbIxB;

`endif

reg[127:0]	tlbHbDatA;
reg[127:0]	tlbHbDatB;
reg[127:0]	tlbHbDatC;
reg[127:0]	tlbHbDatD;

reg[127:0]	tlbHbDatE;
reg[127:0]	tlbHbDatF;
reg[127:0]	tlbHbDatG;
reg[127:0]	tlbHbDatH;

reg[127:0]	tlbHdatA;
reg[127:0]	tlbHdatB;
reg[127:0]	tlbHdatC;
reg[127:0]	tlbHdatD;

reg[127:0]	tlbHdatE;
reg[127:0]	tlbHdatF;
reg[127:0]	tlbHdatG;
reg[127:0]	tlbHdatH;

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

reg			tlbHitE_Hi;
reg			tlbHitF_Hi;
reg			tlbHitG_Hi;
reg			tlbHitH_Hi;
reg			tlbHitE_Mi;
reg			tlbHitF_Mi;
reg			tlbHitG_Mi;
reg			tlbHitH_Mi;
reg			tlbHitE_Lo;
reg			tlbHitF_Lo;
reg			tlbHitG_Lo;
reg			tlbHitH_Lo;

reg			tlbHitE_LoH;
reg			tlbHitF_LoH;
reg			tlbHitG_LoH;
reg			tlbHitH_LoH;
reg			tlbHitE_LoL;
reg			tlbHitF_LoL;
reg			tlbHitG_LoL;
reg			tlbHitH_LoL;

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

reg[127:0]	tlbLdHdatA;
reg[127:0]	tlbLdHdatB;
reg[127:0]	tlbLdHdatC;
reg[127:0]	tlbLdHdatD;

reg			tlbDoLdtlb;
reg			tlbLdtlbOK;

reg			icPageReady;
reg			icPageEq;

reg[47:0]	tlbAddrAB;
reg[47:0]	tlbAddrCD;
reg[47:0]	tlbAddrEF;
reg[47:0]	tlbAddrGH;
reg[31:0]	tlbAccAB;
reg[31:0]	tlbAccCD;
reg[31:0]	tlbAccEF;
reg[31:0]	tlbAccGH;

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

wire		regInIsREADY;
assign		regInIsREADY = (regInOpm==UMEM_OPM_READY);

wire		regInIsLDTLB;
assign		regInIsLDTLB = (regInOpm==UMEM_OPM_LDTLB);

wire		regInIsINVTLB;
assign		regInIsINVTLB = (regInOpm==UMEM_OPM_INVTLB);

reg		regInIsBounce;
// wire		regInIsBounce;
// assign		regInIsBounce = (regInOpm==UMEM_OPM_RD_BOUNCE);

MmuChkAcc tlbChkAcc(
	clock,	reset,
	regInMMCR,
	regInKRR,
	regInSR,
	regInOpm,
	tlbAcc,
	tlbHbIxA,	icPageReady,
	tTlbExc,
	regOutNoRwx);

always @*
begin
	tlbDoLdtlb		= 0;
//	tRegOutTea		= UV64_XX;
	tRegOutTea		= UV64_00;
//	tlbAcc			= UV32_XX;
	tlbAcc			= UV32_00;
	tlbNxtFlushMask	= tlbFlushMask;

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
//	if(regInIsLDTLB)
//		regInAddrA = regTlbData[111:64];

//	icPageEq		= (tRegInAddrA[31:12] == regInAddrA[31:12]);
//	icPageEq		= (tRegInAddrA[47:12] == regInAddrA[47:12]);
	icPageEq		= (tRegInAddrA[47:12] == regInAddrA[47:12]);
//	icPageEq		= 	(tRegInAddrA[47:16] == regInAddrA[47:16]) &&
//						((tRegInAddrA[15:12] == regInAddrA[15:12]) || tlbMmuPg64K);

	tAddrIsLo4G		= (tRegInAddrA[47:32] == 16'h0000);
	tAddrIsHi4G		= (tRegInAddrA[47:32] == 16'hFFFF);
	tAddrIsMMIO		= (tRegInAddrA[31:28] == 4'hF) &&
		(tAddrIsLo4G || tAddrIsHi4G || tlbIs32b);
	tAddrIsPhys		= tAddrIsHi4G && !tRegInAddrA[31];

	regInIsBounce	= (tRegInOpm == UMEM_OPM_RD_BOUNCE);

//	tAddrIsLo4G		= (regInAddr[47:32] == 16'h0000);
//	tAddrIsHi4G		= (regInAddr[47:32] == 16'hFFFF);
//	tAddrIsMMIO		= (regInAddr[31:28] == 4'hF) &&
//		(tAddrIsLo4G || tAddrIsHi4G);
	
//	if(regInSR[29])
	if(regInSR[29] && regInSR[28])
	begin
//		$display("TLB Disable ISR");
		tlbMmuEnable = 0;
	end
	
	if(tlbMmuEnable)
	begin
//		$display("TLB Enabled");
	end

`ifdef jx2_expand_tlb

`ifdef def_true
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
			tlbHixSelA={regInAddrA[15:12], regInAddrA[27:24]};
			tlbHixSelB={regInAddrB[15:12], regInAddrB[27:24]};
		end
		2'b01: begin
			tlbHixSelA=regInAddrA[31:24];
			tlbHixSelB=regInAddrB[31:24];
		end
		2'b10: begin
			tlbHixSelA={regInAddrA[15:14], regInAddrA[29:24]};
			tlbHixSelB={regInAddrB[15:14], regInAddrB[29:24]};
		end
		2'b11: begin
			tlbHixSelA=regInAddrA[31:24];
			tlbHixSelB=regInAddrB[31:24];
		end
	endcase
	tlbHixA = tlbHixSelA ^ regInAddrA[23:16];
	tlbHixB = tlbHixSelB ^ regInAddrB[23:16];
`endif

`ifndef def_true
	tlbHixA = (tlbMmuPg64K?
//		regInAddrA[31:24]:{regInAddrA[27:24],regInAddrA[15:12]})^
		regInAddrA[31:24]:regInAddrA[19:12])^
//		regInAddrA[27:20]:regInAddrA[19:12])^
		regInAddrA[23:16];
	tlbHixB = (tlbMmuPg64K?
//		regInAddrB[31:24]:{regInAddrB[27:24],regInAddrB[15:12]})^
		regInAddrB[31:24]:regInAddrB[19:12])^
//		regInAddrB[27:20]:regInAddrB[19:12])^
		regInAddrB[23:16];
`endif

`else

`ifdef def_true
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
			tlbHixSelA={regInAddrA[15:12], regInAddrA[23:22]};
			tlbHixSelB={regInAddrB[15:12], regInAddrB[23:22]};
		end
		2'b01: begin
			tlbHixSelA=regInAddrA[27:22];
			tlbHixSelB=regInAddrB[27:22];
		end
		2'b10: begin
			tlbHixSelA={regInAddrA[15:14], regInAddrA[25:22]};
			tlbHixSelB={regInAddrB[15:14], regInAddrB[25:22]};
		end
		2'b11: begin
			tlbHixSelA=regInAddrA[27:22];
			tlbHixSelB=regInAddrB[27:22];
		end
	endcase

`ifdef jx2_expand_tlb
	tlbHixA = tlbHixSelA ^ regInAddrA[23:16];
	tlbHixB = tlbHixSelB ^ regInAddrB[23:16];
`else
	tlbHixA = tlbHixSelA[5:0] ^ regInAddrA[21:16];
	tlbHixB = tlbHixSelB[5:0] ^ regInAddrB[21:16];
`endif

`endif

`ifndef def_true
//	tlbHixA = regInAddr[17:12]^regInAddr[21:16];
//	tlbHixB = regInAddrB[17:12]^regInAddrB[21:16];

//	tlbHixA = (tlbMmuPg64K?0:regInAddr[17:12])^regInAddr[21:16];
//	tlbHixB = (tlbMmuPg64K?0:regInAddrB[17:12])^regInAddrB[21:16];

	tlbHixA = (tlbMmuPg64K?
		regInAddrA[27:22]:regInAddrA[17:12])^
		regInAddrA[21:16];
	tlbHixB = (tlbMmuPg64K?
		regInAddrB[27:22]:regInAddrB[17:12])^
		regInAddrB[21:16];
`endif

`ifndef def_true
	if(tlbMmuPg64K)
	begin
		tlbHixA = regInAddrA[27:22]^regInAddrA[21:16];
		tlbHixB = regInAddrB[27:22]^regInAddrB[21:16];
	end
	else
	begin
		tlbHixA = regInAddrA[17:12]^regInAddrA[21:16];
		tlbHixB = regInAddrB[17:12]^regInAddrB[21:16];
	end
`endif

`endif


	icPageReady		= (tlbHbIxA == tlbHixA);
	if(!icPageEq)
		icPageReady = 0;

//	tlbHdatA = { tlbBlkHiA[tlbHixA], tlbBlkLoA[tlbHixA]};
//	tlbHdatB = { tlbBlkHiB[tlbHixA], tlbBlkLoB[tlbHixA]};
//	tlbHdatC = { tlbBlkHiC[tlbHixA], tlbBlkLoC[tlbHixA]};
//	tlbHdatD = { tlbBlkHiD[tlbHixA], tlbBlkLoD[tlbHixA]};

	tlbHdatA = tlbHbDatA;
	tlbHdatB = tlbHbDatB;
	tlbHdatC = tlbHbDatC;
	tlbHdatD = tlbHbDatD;

	tlbLdHdatA = tlbHdatA;
	tlbLdHdatB = tlbHdatB;
	tlbLdHdatC = tlbHdatC;
	tlbLdHdatD = tlbHdatD;

// `ifdef jx2_mem_fulldpx
`ifndef def_true

//	tlbHdatE = { tlbBlkHiA[tlbHixB], tlbBlkLoA[tlbHixB]};
//	tlbHdatF = { tlbBlkHiB[tlbHixB], tlbBlkLoB[tlbHixB]};
//	tlbHdatG = { tlbBlkHiC[tlbHixB], tlbBlkLoC[tlbHixB]};
//	tlbHdatH = { tlbBlkHiD[tlbHixB], tlbBlkLoD[tlbHixB]};

	tlbHdatE = tlbHbDatE;
	tlbHdatF = tlbHbDatF;
	tlbHdatG = tlbHbDatG;
	tlbHdatH = tlbHbDatH;
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

// `ifdef jx2_mem_fulldpx
`ifndef def_true
	if(tlbFlushMask[tlbHixB])
	begin
		tlbHdatE[0] = 0;
		tlbHdatF[0] = 0;
		tlbHdatG[0] = 0;
		tlbHdatH[0] = 0;
	end
`endif

`ifndef def_true
	tlbHitA_Hi = (regInAddr [47:32] == tlbHdatA[111:96]);
	tlbHitB_Hi = (regInAddr [47:32] == tlbHdatB[111:96]);
	tlbHitC_Hi = (regInAddr [47:32] == tlbHdatC[111:96]);
	tlbHitD_Hi = (regInAddr [47:32] == tlbHdatD[111:96]);
	tlbHitA_Mi = (regInAddr [31:16] == tlbHdatA[95:80]);
	tlbHitB_Mi = (regInAddr [31:16] == tlbHdatB[95:80]);
	tlbHitC_Mi = (regInAddr [31:16] == tlbHdatC[95:80]);
	tlbHitD_Mi = (regInAddr [31:16] == tlbHdatD[95:80]);
	tlbHitA_Lo = (regInAddr [15:12] == tlbHdatA[79:76]);
	tlbHitB_Lo = (regInAddr [15:12] == tlbHdatB[79:76]);
	tlbHitC_Lo = (regInAddr [15:12] == tlbHdatC[79:76]);
	tlbHitD_Lo = (regInAddr [15:12] == tlbHdatD[79:76]);

// `ifdef jx2_mem_fulldpx
`ifndef def_true
	tlbHitE_Hi = (regInAddrB[47:32] == tlbHdatE[111:96]);
	tlbHitF_Hi = (regInAddrB[47:32] == tlbHdatF[111:96]);
	tlbHitG_Hi = (regInAddrB[47:32] == tlbHdatG[111:96]);
	tlbHitH_Hi = (regInAddrB[47:32] == tlbHdatH[111:96]);
	tlbHitE_Mi = (regInAddrB[31:16] == tlbHdatE[95:80]);
	tlbHitF_Mi = (regInAddrB[31:16] == tlbHdatF[95:80]);
	tlbHitG_Mi = (regInAddrB[31:16] == tlbHdatG[95:80]);
	tlbHitH_Mi = (regInAddrB[31:16] == tlbHdatH[95:80]);
	tlbHitE_Lo = (regInAddrB[15:12] == tlbHdatE[79:76]);
	tlbHitF_Lo = (regInAddrB[15:12] == tlbHdatF[79:76]);
	tlbHitG_Lo = (regInAddrB[15:12] == tlbHdatG[79:76]);
	tlbHitH_Lo = (regInAddrB[15:12] == tlbHdatH[79:76]);
`endif
`endif

`ifdef def_true
	tlbHitA_Hi = (tRegInAddrA[47:32] == tlbHdatA[111:96]);
	tlbHitB_Hi = (tRegInAddrA[47:32] == tlbHdatB[111:96]);
	tlbHitC_Hi = (tRegInAddrA[47:32] == tlbHdatC[111:96]);
	tlbHitD_Hi = (tRegInAddrA[47:32] == tlbHdatD[111:96]);
	tlbHitA_Mi = (tRegInAddrA[31:16] == tlbHdatA[95:80]);
	tlbHitB_Mi = (tRegInAddrA[31:16] == tlbHdatB[95:80]);
	tlbHitC_Mi = (tRegInAddrA[31:16] == tlbHdatC[95:80]);
	tlbHitD_Mi = (tRegInAddrA[31:16] == tlbHdatD[95:80]);

//	tlbHitA_Lo = (tRegInAddrA[15:12] == tlbHdatA[79:76]);
//	tlbHitB_Lo = (tRegInAddrA[15:12] == tlbHdatB[79:76]);
//	tlbHitC_Lo = (tRegInAddrA[15:12] == tlbHdatC[79:76]);
//	tlbHitD_Lo = (tRegInAddrA[15:12] == tlbHdatD[79:76]);

	tlbHitA_LoH = (tRegInAddrA[15:14] == tlbHdatA[79:78]);
	tlbHitB_LoH = (tRegInAddrA[15:14] == tlbHdatB[79:78]);
	tlbHitC_LoH = (tRegInAddrA[15:14] == tlbHdatC[79:78]);
	tlbHitD_LoH = (tRegInAddrA[15:14] == tlbHdatD[79:78]);
	tlbHitA_LoL = (tRegInAddrA[13:12] == tlbHdatA[77:76]);
	tlbHitB_LoL = (tRegInAddrA[13:12] == tlbHdatB[77:76]);
	tlbHitC_LoL = (tRegInAddrA[13:12] == tlbHdatC[77:76]);
	tlbHitD_LoL = (tRegInAddrA[13:12] == tlbHdatD[77:76]);
	tlbHitA_Lo = tlbHitA_LoH && (tlbHitA_LoL || tlbMmuPg16K);
	tlbHitB_Lo = tlbHitB_LoH && (tlbHitB_LoL || tlbMmuPg16K);
	tlbHitC_Lo = tlbHitC_LoH && (tlbHitC_LoL || tlbMmuPg16K);
	tlbHitD_Lo = tlbHitD_LoH && (tlbHitD_LoL || tlbMmuPg16K);

// `ifdef jx2_mem_fulldpx
`ifndef def_true
	tlbHitE_Hi = (tRegInAddrB[47:32] == tlbHdatE[111:96]);
	tlbHitF_Hi = (tRegInAddrB[47:32] == tlbHdatF[111:96]);
	tlbHitG_Hi = (tRegInAddrB[47:32] == tlbHdatG[111:96]);
	tlbHitH_Hi = (tRegInAddrB[47:32] == tlbHdatH[111:96]);
	tlbHitE_Mi = (tRegInAddrB[31:16] == tlbHdatE[95:80]);
	tlbHitF_Mi = (tRegInAddrB[31:16] == tlbHdatF[95:80]);
	tlbHitG_Mi = (tRegInAddrB[31:16] == tlbHdatG[95:80]);
	tlbHitH_Mi = (tRegInAddrB[31:16] == tlbHdatH[95:80]);
//	tlbHitE_Lo = (tRegInAddrB[15:12] == tlbHdatE[79:76]);
//	tlbHitF_Lo = (tRegInAddrB[15:12] == tlbHdatF[79:76]);
//	tlbHitG_Lo = (tRegInAddrB[15:12] == tlbHdatG[79:76]);
//	tlbHitH_Lo = (tRegInAddrB[15:12] == tlbHdatH[79:76]);

	tlbHitE_LoH = (tRegInAddrB[15:14] == tlbHdatE[79:78]);
	tlbHitF_LoH = (tRegInAddrB[15:14] == tlbHdatF[79:78]);
	tlbHitG_LoH = (tRegInAddrB[15:14] == tlbHdatG[79:78]);
	tlbHitH_LoH = (tRegInAddrB[15:14] == tlbHdatH[79:78]);
	tlbHitE_LoL = (tRegInAddrB[13:12] == tlbHdatE[77:76]);
	tlbHitF_LoL = (tRegInAddrB[13:12] == tlbHdatF[77:76]);
	tlbHitG_LoL = (tRegInAddrB[13:12] == tlbHdatG[77:76]);
	tlbHitH_LoL = (tRegInAddrB[13:12] == tlbHdatH[77:76]);
	tlbHitE_Lo = tlbHitE_LoH && (tlbHitE_LoL || tlbMmuPg16K);
	tlbHitF_Lo = tlbHitF_LoH && (tlbHitF_LoL || tlbMmuPg16K);
	tlbHitG_Lo = tlbHitG_LoH && (tlbHitG_LoL || tlbMmuPg16K);
	tlbHitH_Lo = tlbHitH_LoH && (tlbHitH_LoL || tlbMmuPg16K);

`endif
`endif

	if(tlbIs32b)
	begin
		tlbHitA_Hi = 1;
		tlbHitB_Hi = 1;
		tlbHitC_Hi = 1;
		tlbHitD_Hi = 1;
// `ifdef jx2_mem_fulldpx
`ifndef def_true
		tlbHitE_Hi = 1;
		tlbHitF_Hi = 1;
		tlbHitG_Hi = 1;
		tlbHitH_Hi = 1;
`endif
	end

	if(tlbMmuPg64K)
	begin
		tlbHitA_Lo = 1;
		tlbHitB_Lo = 1;
		tlbHitC_Lo = 1;
		tlbHitD_Lo = 1;
// `ifdef jx2_mem_fulldpx
`ifndef def_true
		tlbHitE_Lo = 1;
		tlbHitF_Lo = 1;
		tlbHitG_Lo = 1;
		tlbHitH_Lo = 1;
`endif
	end
	
	tlbHitA = tlbHitA_Hi && tlbHitA_Mi && tlbHitA_Lo && tlbHdatA[0];
	tlbHitB = tlbHitB_Hi && tlbHitB_Mi && tlbHitB_Lo && tlbHdatB[0];
	tlbHitC = tlbHitC_Hi && tlbHitC_Mi && tlbHitC_Lo && tlbHdatC[0];
	tlbHitD = tlbHitD_Hi && tlbHitD_Mi && tlbHitD_Lo && tlbHdatD[0];

//`ifdef jx2_mem_fulldpx
`ifndef def_true
	tlbHitE = tlbHitE_Hi && tlbHitE_Mi && tlbHitE_Lo && tlbHdatE[0];
	tlbHitF = tlbHitF_Hi && tlbHitF_Mi && tlbHitF_Lo && tlbHdatF[0];
	tlbHitG = tlbHitG_Hi && tlbHitG_Mi && tlbHitG_Lo && tlbHdatG[0];
	tlbHitH = tlbHitH_Hi && tlbHitH_Mi && tlbHitH_Lo && tlbHdatH[0];
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

	tlbAddrAB[47:12] = tlbHitA ? tlbHdatA[47:12] : tlbHdatB[47:12];
	tlbAddrCD[47:12] = tlbHitC ? tlbHdatC[47:12] : tlbHdatD[47:12];
//	tlbAddrAB[11:0] = regInAddr[11:0];
//	tlbAddrCD[11:0] = regInAddr[11:0];

	tlbAddr[47:12] = tlbHitAB ? tlbAddrAB[47:12] : tlbAddrCD[47:12];
//	tlbAddr[11: 0] = regInAddr[11: 0];
	tlbAddr[11: 0] = tRegInAddrA[11: 0];
//	if(tlbMmuPg64K)
//		tlbAddr[15:12] = regInAddr[15:12];
//		tlbAddr[15:12] = tRegInAddrA[15:12];

	if(!tlbMmuPg4K)
		tlbAddr[13:12] = tRegInAddrA[13:12];
	if(tlbMmuPg64K)
		tlbAddr[15:14] = tRegInAddrA[15:14];

// `ifdef jx2_mem_fulldpx
`ifndef def_true
	tlbAddrEF[47:12] = tlbHitE ? tlbHdatE[47:12] : tlbHdatF[47:12];
	tlbAddrGH[47:12] = tlbHitG ? tlbHdatG[47:12] : tlbHdatH[47:12];
	tlbAddrB[47:12] = tlbHitEF ? tlbAddrEF[47:12] : tlbAddrGH[47:12];
//	tlbAddrB[11: 0] = regInAddrB[11: 0];
	tlbAddrB[11: 0] = tRegInAddrB[11: 0];
//	if(tlbMmuPg64K)
//		tlbAddrB[15:12] = regInAddrB[15:12];
//		tlbAddrB[15:12] = tRegInAddrB[15:12];

	if(!tlbMmuPg4K)
		tlbAddrB[13:12] = tRegInAddrB[13:12];
	if(tlbMmuPg64K)
		tlbAddrB[15:14] = tRegInAddrB[15:14];

`else
//	tlbAddrB = regInAddrB;
	tlbAddrB = tRegInAddrB;
`endif

	tlbAccAB = 
		tlbHitA ? { tlbHdatA[127:112], tlbHdatA[75:64], tlbHdatA[7:4] } :
			{ tlbHdatB[127:112], tlbHdatB[75:64], tlbHdatB[7:4] };
	tlbAccCD = 
		tlbHitC ? { tlbHdatC[127:112], tlbHdatC[75:64], tlbHdatC[7:4] } :
			{ tlbHdatD[127:112], tlbHdatD[75:64], tlbHdatD[7:4] };
	tlbAcc = tlbHitAB ? tlbAccAB : tlbAccCD;

// `ifdef jx2_mem_fulldpx
`ifndef def_true
	tlbAccEF = 
		tlbHitE ? { tlbHdatE[127:112], tlbHdatE[75:64], tlbHdatE[7:4] } :
			{ tlbHdatF[127:112], tlbHdatF[75:64], tlbHdatF[7:4] };
	tlbAccGH = 
		tlbHitG ? { tlbHdatG[127:112], tlbHdatG[75:64], tlbHdatG[7:4] } :
			{ tlbHdatH[127:112], tlbHdatH[75:64], tlbHdatH[7:4] };
	tlbAccB = tlbHitEF ? tlbAccEF : tlbAccGH;
`else
	tlbAccB = 0;
`endif

	tlbMiss = 0;

	tlbMmuSkip = 0;
//	if(regInAddr[31])
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

			$display("Miss A=%X B=%X SR=%X_%X",
				tRegInAddrA,	tRegInAddrB,
				regInSR[63:32], regInSR[31:0]);

//			tlbAddr		= regInAddrA[47:0];
//			tlbAddrB	= regInAddrB[47:0];
//			tlbAddr		= tRegInAddrA[47:0];
//			tlbAddrB	= tRegInAddrB[47:0];
//			tlbAddr		= 0;
//			tlbAddrB	= 0;

			tlbAddr		= 48'h010000;
//			tlbAddrB	= 48'h010000;
			tlbAddrB	= tRegInAddrB[47:0];

//			if(tRegInOpm[4])
//			begin
//				tlbAddr		= 0;
//				tlbAddrB	= 0;
//				tlbAddr		= 48'h010000;
//				tlbAddrB	= 48'h010000;
//			end
		end
	end
	else
	begin
//		tlbAddr		= regInAddrA[47:0];
//		tlbAddrB	= regInAddrB[47:0];
		tlbAddr		= tRegInAddrA[47:0];
		tlbAddrB	= tRegInAddrB[47:0];
		
		if(tAddrIsPhys && tAddrIsHi4G)
			tlbAddr[47:32]	= 0;

		tlbMiss		= 0;
	end

	tRegOutExc = 0;
//	tRegOutTea[31:0] = regInAddr;
//	tRegOutTea[47:0] = (tlbHitAB || tlbHitCD) ? regInAddrB : regInAddr;
//	tRegOutTea[47:0] = regInAddr;
//	tRegOutTea[47:0] = tRegInAddrA;

// `ifdef jx2_mem_fulldpx
`ifndef def_true
	tRegOutTea[47:0] = (tlbHitAB || tlbHitCD) ? tRegInAddrB : tRegInAddrA;
`else
	tRegOutTea[47:0] = tRegInAddrA;
`endif

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
`ifndef def_true
			if(!regInSR[30])
			begin
//				if(regInOpm[4])
				if(tRegInOpm[4])
					tRegOutExc	= 16'h8002;
//				if(regInOpm[3])
				if(tRegInOpm[3])
					tRegOutExc	= 16'h8001;
			end
`endif
		end
	end
		
	

//	if(tlbMmuEnable && icPageEq &&
	if(tlbMmuEnable && icPageReady &&
//		(regInOpm[4:3]!=0) && !tlbMmuSkip)
		(tRegInOpm[4:3]!=0) && !tlbMmuSkip)
	begin
		if(tlbMiss)
		begin
//			$display("TLB Miss %X", regInAddr);
			tRegOutExc = 16'hA001;
		end
		else
		begin
	//		$display("TLB Hit %X", regInAddr);
		end
	end
	
	tRegOutAddr  = tlbAddr;
	tRegOutAddrB = tlbAddrB;
//	tRegOutOpm   = regInOpm;
	tRegOutOpm   = tRegInOpm;
//	tRegOutOpm   = (!tRegOutExc[15] && icPageReady) ?
//		regInOpm : (tRegOutExc[15] ? UMEM_OPM_LDTLB : UMEM_OPM_READY);
//	tRegOutOpm   =
//		(	(!tRegOutExc[15] && icPageReady) ||
//			 !regInOpm  [ 4] || !tlbMmuEnable	) ?
//		regInOpm : (tRegOutExc[15] ? UMEM_OPM_LDTLB : UMEM_OPM_READY);

//	if(regInAddr[47:0]!=tlbAddr[47:0])
//	if(tRegInAddrA[47:0]!=tlbAddr[47:0])
	if((tRegInAddrA[47:0]!=tlbAddr[47:0]) && !tAddrIsPhys)
	begin
		$display("TLB(A) %X -> %X", tRegInAddrA, tlbAddr);
	end

//`ifdef jx2_mem_fulldpx
`ifndef def_true
//	if(regInAddrB[47:0]!=tlbAddrB[47:0])
//	if(tRegInAddrB[47:0]!=tlbAddrB[47:0])
	if((tRegInAddrB[47:0]!=tlbAddrB[47:0]) && (tRegInOpm[4:3]==3))
	begin
		$display("TLB(B) %X -> %X", tRegInAddrB, tlbAddrB);
	end
`endif

	if(regInIsBounce && !tAddrIsMMIO)
	begin
//		$display("TLB: Bounce %X", tRegInAddrA);
		tRegOutOpm   = UMEM_OPM_LDTLB;
//		tRegOutOpm   = UMEM_OPM_RD_TILE;
	end
	
	if(regInIsINVTLB || reset)
	begin
`ifdef jx2_expand_tlb
		tlbNxtFlushMask = UV256_FF;
`else
		tlbNxtFlushMask = UV64_FF;
`endif
	end

	if(reset)
	begin
		tRegOutExc = 0;
		tRegOutOpm   = UMEM_OPM_READY;
`ifdef jx2_mem_line32B
//		tRegOutOpm   = UMEM_OPM_INVTLB;	/* Signal to simulation. */
		tRegOutOpm   = UMEM_OPM_WR_UB;	/* Signal to simulation. */
`endif
	end

	if(regInIsLDTLB)
	begin
		$display("MemTile-LDTLB %X %X-%X",
			regInAddr,
			regTlbData[127:64],
			regTlbData[ 63: 0]);
//		tlbDoLdtlb	= 1;
		tlbDoLdtlb	= icPageReady;
		tlbNxtFlushMask[tlbHixA] = 0;
		tRegOutOK = tlbLdtlbOK ?
			UMEM_OK_OK : UMEM_OK_HOLD;

		tlbLdHdatA = regTlbData[127:0];
		tlbLdHdatB = tlbHdatA;
		tlbLdHdatC = tlbHdatB;
		tlbLdHdatD = tlbHdatC;

	end
	else
	begin
//		tRegOutOK = (regInOpm[4:3]!=0) && icPageEq ?
//		tRegOutOK = (regInOpm[4:3]!=0) && icPageReady ?
		tRegOutOK = (tRegInOpm[4:3]!=0) && icPageReady ?
			UMEM_OK_OK : UMEM_OK_READY;

//		$display("TLB Opm=%X Ok=%X", regInOpm, tRegOutOK);

`ifdef def_true
// `ifndef def_true
//		if(tlbMmuEnable && icPageEq &&
		if(tlbMmuEnable && icPageReady &&
			!tlbMiss &&
//			(regInOpm[4:3]!=0) && !tlbMmuSkip)
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
	tRegOutAddr2	<= tRegOutAddr;
	tRegOutAddrB2	<= tRegOutAddrB;
	tRegOutExc2		<= tRegOutExc;
	tRegOutTea2		<= tRegOutTea;
	tRegOutOK2		<= tRegOutOK;
//	tRegOutOpm2		<= tRegOutOpm;
`ifdef jx2_mem_fasttdown
	tRegOutOpm2		<= (regInIsREADY && !reset) ? UMEM_OPM_READY : tRegOutOpm;
`else
	tRegOutOpm2		<= tRegOutOpm;
`endif
	tRegOutData2	<= tRegOutData;

//	tRegInAddr		<= regInAddr;
	tRegInAddrA		<= regInAddrA;
	tRegInAddrB		<= regInAddrB;
	tRegInOpm		<= regInOpm;
	tRegInData		<= regInData;
	tlbFlushMask	<= tlbNxtFlushMask;

	tlbHbIxA		<= tlbHixA;
	tlbHbIxB		<= tlbHixB;

	tlbHbDatA		<= { tlbBlkHiA[tlbHixA], tlbBlkLoA[tlbHixA]};
	tlbHbDatB		<= { tlbBlkHiB[tlbHixA], tlbBlkLoB[tlbHixA]};
	tlbHbDatC		<= { tlbBlkHiC[tlbHixA], tlbBlkLoC[tlbHixA]};
	tlbHbDatD		<= { tlbBlkHiD[tlbHixA], tlbBlkLoD[tlbHixA]};

//`ifdef jx2_mem_fulldpx
`ifndef def_true
	tlbHbDatE		<= { tlbBlkHiA[tlbHixB], tlbBlkLoA[tlbHixB]};
	tlbHbDatF		<= { tlbBlkHiB[tlbHixB], tlbBlkLoB[tlbHixB]};
	tlbHbDatG		<= { tlbBlkHiC[tlbHixB], tlbBlkLoC[tlbHixB]};
	tlbHbDatH		<= { tlbBlkHiD[tlbHixB], tlbBlkLoD[tlbHixB]};
`endif

	if(tlbDoLdtlb && !tlbLdtlbOK)
	begin
//		$display("Do LdTlb %X-%X Ix=%X",
//			regTlbData[127:64], regTlbData[63:0], tlbHixA);

`ifndef def_true
		tlbBlkHiA[tlbHixA]	<= regTlbData[127:64];
		tlbBlkLoA[tlbHixA]	<= regTlbData[ 63: 0];
		tlbBlkHiB[tlbHixA]	<= tlbHdatA[127:64];
		tlbBlkLoB[tlbHixA]	<= tlbHdatA[ 63: 0];
		tlbBlkHiC[tlbHixA]	<= tlbHdatB[127:64];
		tlbBlkLoC[tlbHixA]	<= tlbHdatB[ 63: 0];
		tlbBlkHiD[tlbHixA]	<= tlbHdatC[127:64];
		tlbBlkLoD[tlbHixA]	<= tlbHdatC[ 63: 0];
`endif

`ifdef def_true
		tlbBlkHiA[tlbHixA]	<= tlbLdHdatA[127:64];
		tlbBlkLoA[tlbHixA]	<= tlbLdHdatA[ 63: 0];
		tlbBlkHiB[tlbHixA]	<= tlbLdHdatB[127:64];
		tlbBlkLoB[tlbHixA]	<= tlbLdHdatB[ 63: 0];
		tlbBlkHiC[tlbHixA]	<= tlbLdHdatC[127:64];
		tlbBlkLoC[tlbHixA]	<= tlbLdHdatC[ 63: 0];
		tlbBlkHiD[tlbHixA]	<= tlbLdHdatD[127:64];
		tlbBlkLoD[tlbHixA]	<= tlbLdHdatD[ 63: 0];
`endif

		tlbLdtlbOK	<=	1;
	end
	else
	begin
		tlbLdtlbOK	<= 0;
	end
end

// /* verilator lint_on UNOPTFLAT */


endmodule
