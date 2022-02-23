/*
TLB:
Sits between the L1 and L2 caches.

Filters output from the L1's memory interface, so that L1 operates with virtual addresses and L2 with physical addresses.

 */

`include "ringbus/RbiDefs.v"

`include "ringbus/RbiMmuChkAcc.v"

module RbiMmuTlb(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInAddr,		regOutAddr,
	regInData,		regOutData,
	regInOpm,		regOutOpm,
	regInSeq,		regOutSeq,
	unitNodeId,		regInLdtlb,
	regOutExc,		regInHold,
	regInMMCR,		regInKRR,
	regInSR,		regInExecAcl
	);

input			clock;			//clock
input			reset;			//reset

`input_l1addr	regInAddr;		//input Address
`output_l1addr	regOutAddr;		//output Address
input[127:0]	regInData;		//input cache line
output[127:0]	regOutData;		//output cache line
input[15:0]		regInOpm;		//Operation Size/Type
output[15:0]	regOutOpm;		//Operation Size/Type
input[15:0]		regInSeq;		//Operation Size/Type
output[15:0]	regOutSeq;		//Operation Size/Type
input[7:0]		unitNodeId;		//Node ID

output[127:0]	regOutExc;		//Exception EXC+TEA
input			regInHold;		//Hold Pipeline

input[63:0]		regInMMCR;		//MMU Control Register
input[63:0]		regInKRR;		//Keyring Register
input[63:0]		regInSR;		//Status Register

input[127:0]	regInLdtlb;
input[15:0]		regInExecAcl;

reg[127:0]		tRegInLdtlbHi;
reg[127:0]		tNxtRegInLdtlbHi;


`reg_l1addr		tRegOutAddr;
`reg_l1addr		tRegOutAddr2;
`reg_l1addr		tRegOutAddr3;
reg[127:0]		tRegOutData;		//output cache line
reg[127:0]		tRegOutData2;		//output cache line
reg[127:0]		tRegOutData3;		//output cache line
reg[15:0]		tRegOutOpm;
reg[15:0]		tRegOutOpm2;
reg[15:0]		tRegOutOpm3;
reg[15:0]		tRegOutSeq;
reg[15:0]		tRegOutSeq2;
reg[15:0]		tRegOutSeq3;

// assign		regOutAddr = tRegOutAddr3;
// assign		regOutData = tRegOutData3;
// assign		regOutOpm = tRegOutOpm3;
// assign		regOutSeq = tRegOutSeq3;

assign		regOutAddr = tRegOutAddr2;
assign		regOutData = tRegOutData2;
assign		regOutOpm = tRegOutOpm2;
assign		regOutSeq = tRegOutSeq2;

reg[15:0]		tRegOutExc;
reg[15:0]		tRegOutExc2;
reg[63:0]		tRegOutTea;
reg[63:0]		tRegOutTea2;
reg[63:0]		tRegOutTeaHi;
reg[63:0]		tRegOutTeaHi2;

assign		regOutExc = { tRegOutTeaHi2[63:0], tRegOutTea2[47:0], tRegOutExc2 };


`reg_l1addr		regInAddrA;		//input Address

`reg_l1addr		tRegInAddr;	//input Address
reg[15:0]		tRegInOpm;		//Operation Size/Type
reg[15:0]		tRegInSeq;		//
reg[127:0]		tRegInData;		//output cache line
reg[127:0]		tRegInLdtlb;	//input LDTLB

`ifdef jx2_tlbsz_1024

reg[143:0]	tlbBlkA[1023:0];
reg[143:0]	tlbBlkB[1023:0];
reg[143:0]	tlbBlkC[1023:0];
reg[143:0]	tlbBlkD[1023:0];

reg[9:0]	tlbHixA;
reg[9:0]	tlbHixSelA;
reg[9:0]	tlbHixSelB;

reg[9:0]	tlbHbIxA;

`endif

`ifdef jx2_tlbsz_256

reg[143:0]	tlbBlkA[255:0];
reg[143:0]	tlbBlkB[255:0];
reg[143:0]	tlbBlkC[255:0];
reg[143:0]	tlbBlkD[255:0];

reg[7:0]	tlbHixA;
// reg[7:0]	tlbHixB;
reg[7:0]	tlbHixSelA;
reg[7:0]	tlbHixSelB;

reg[7:0]	tlbHbIxA;
// reg[7:0]	tlbHbIxB;

`endif

`ifdef jx2_tlbsz_64

(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkA[63:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkB[63:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkC[63:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkD[63:0];

reg[5:0]	tlbHixA;
// reg[5:0]	tlbHixB;
reg[5:0]	tlbHixSelA;
reg[5:0]	tlbHixSelB;

reg[5:0]	tlbHbIxA;
// reg[5:0]	tlbHbIxB;

`endif

`ifdef jx2_tlbsz_32
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkA[31:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkB[31:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkC[31:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkD[31:0];

reg[4:0]	tlbHixA;
reg[4:0]	tlbHixSelA;
reg[4:0]	tlbHixSelB;
reg[4:0]	tlbHbIxA;
`endif

`ifdef jx2_tlbsz_16
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkA[15:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkB[15:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkC[15:0];
(* ram_style = "distributed" *)
	reg[143:0]	tlbBlkD[15:0];

reg[3:0]	tlbHixA;
reg[3:0]	tlbHixSelA;
reg[3:0]	tlbHixSelB;
reg[3:0]	tlbHbIxA;
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

reg			tlbHitB_HiX;
reg			tlbHitD_HiX;
reg			tlbHitB_MiX;
reg			tlbHitD_MiX;
reg			tlbHitB_LoX;
reg			tlbHitD_LoX;
reg			tlbHitB_X;
reg			tlbHitD_X;

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

reg			tlbHitAX;
reg			tlbHitCX;

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

`reg_l1addr	tlbAddrAB;
`reg_l1addr	tlbAddrCD;
reg[35:0]		tlbAccAB;
reg[35:0]		tlbAccCD;

`reg_l1addr	tlbAddr;
reg[35:0]		tlbAcc;
`reg_l1addr	tlbAddrB;
reg[35:0]		tlbAccB;
reg				tlbIs32b;
reg				tlbIs48b;

reg[47:0]	aclEntryA;
reg[47:0]	aclEntryB;
reg[47:0]	aclEntryC;
reg[47:0]	aclEntryD;

reg[47:0]	nxtAclEntryA;
reg[47:0]	nxtAclEntryB;
reg[47:0]	nxtAclEntryC;
reg[47:0]	nxtAclEntryD;

reg			tAddrIsMMIO;
reg			tAddrIsHiMMIO;
reg			tAddrIsLo4G;
reg			tAddrIsHi4G;
reg			tAddrIsPhys;
reg			tAddrIsPhysV;

wire[15:0]	tTlbExc;

reg[7:0]	tTlbRov;
reg[7:0]	tNxtTlbRov;

wire		tRegInIsREADY;
assign		tRegInIsREADY = (tRegInOpm[7:0]==JX2_RBI_OPM_IDLE);

wire		tRegInIsLDTLB;
assign		tRegInIsLDTLB = (tRegInOpm[7:0]==JX2_RBI_OPM_LDTLB);

wire		tRegInIsINVTLB;
assign		tRegInIsINVTLB = (tRegInOpm[7:0]==JX2_RBI_OPM_INVTLB);

wire		tRegInIsLDACL;
assign		tRegInIsLDACL = (tRegInOpm[7:0]==JX2_RBI_OPM_LDACL);

// wire		tRegInIsINVACL;
// assign		tRegInIsINVACL = (tRegInOpm[7:0]==JX2_RBI_OPM_INVACL);

wire		tRegInIsIRQ;
assign		tRegInIsIRQ = (tRegInOpm[7:0]==JX2_RBI_OPM_IRQ);

wire		tRegInIsCCmd;
assign		tRegInIsCCmd = (tRegInOpm[7:4]==4'h8);

wire		tRegInIsLDX;
wire		tRegInIsSTX;
assign		tRegInIsLDX = (tRegInOpm[7:4]==4'h9);
assign		tRegInIsSTX = (tRegInOpm[7:4]==4'hA);


wire		regInIsREADY;
assign		regInIsREADY = (regInOpm[7:0]==JX2_RBI_OPM_IDLE);

wire		regInIsPhysAddr;
wire		regInIsPhysAddrC;
wire		regInIsPhysAddrV;
assign		regInIsPhysAddrC = (regInAddrA[47:44]==4'hC);
assign		regInIsPhysAddrV = (regInAddrA[47:44]==4'hD);
assign		regInIsPhysAddr = regInIsPhysAddrC || regInIsPhysAddrV;

wire		regInIsLDX;
wire		regInIsSTX;
wire		regInIsPFX;
wire		regInIsNzX;
assign		regInIsLDX = (regInOpm[7:0]==JX2_RBI_OPM_LDX);
assign		regInIsSTX = (regInOpm[7:0]==JX2_RBI_OPM_STX);
assign		regInIsPFX =
	(regInOpm[7:0]==JX2_RBI_OPM_PFX) ||
	(regInOpm[7:0]==JX2_RBI_OPM_SPX);
assign		regInIsNzX = regInIsLDX || regInIsSTX || regInIsPFX;

wire		regInIsLDIO;
wire		regInIsSTIO;
wire		regInIsMMIO;
wire		regInIsReqIO;
assign		regInIsLDIO = (regInOpm[7:4]==4'h9);
assign		regInIsSTIO = (regInOpm[7:4]==4'hA);
assign		regInIsMMIO = (regInIsLDIO || regInIsSTIO) && !regInIsNzX;
assign		regInIsReqIO = regInIsLDIO || regInIsSTIO;

wire[63:0]	tRegInKRR;
assign	tRegInKRR = ((regInExecAcl!=0) && (regInKRR[15:0]!=0)) ?
	{ regInKRR[47:0], regInExecAcl } : regInKRR;

reg		tRegInIsBounce;

wire[5:0]	tChkAccNoRwx;	//No R/W/X

RbiMmuChkAcc tlbChkAcc(
	clock,	reset,
	regInHold,
	regInMMCR,
//	regInKRR,
	tRegInKRR,
	regInSR,
	regInOpm[7:0],
	tlbAcc,
	aclEntryA,	aclEntryB,
	aclEntryC,	aclEntryD,
	tTlbExc,
	tChkAccNoRwx);

always @*
begin
	tlbDoLdtlb		= 0;
	tRegOutTea		= UV64_00;
	tRegOutTeaHi	= UV64_00;
	tlbAcc			= UV36_00;
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

//	regInAddrA		= regInAddr[47:0];
	regInAddrA		= regInAddr;

	if(regInOpm[7:0]==JX2_RBI_OPM_LDTLB)
	begin
`ifdef jx2_enable_l1addr96
		regInAddrA		= { UV48_00, regInLdtlb[111:64] };
`else
//		regInAddrA		= regInData[111:64];
		regInAddrA		= regInLdtlb[111:64];
`endif
	end

`ifdef jx2_tlbsz_1024
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
//			tlbHixSelA={regInAddrA[15:12], regInAddrA[31:26]};
			tlbHixSelA={
				regInAddrA[15:14],	regInAddrA[25:24],
				regInAddrA[27:26],	regInAddrA[13:12],
				regInAddrA[31:30]};
		end
		2'b01: begin
//			tlbHixSelA=regInAddrA[31:22];
			tlbHixSelA={
				regInAddrA[23:22],	regInAddrA[25:24],
				regInAddrA[27:26],	regInAddrA[29:28],
				regInAddrA[31:30]};
		end
		2'b10: begin
//			tlbHixSelA={regInAddrA[15:14], regInAddrA[31:24]};
			tlbHixSelA={
				regInAddrA[15:14],	regInAddrA[25:24],
				regInAddrA[27:26],	regInAddrA[29:28],
				regInAddrA[31:30]};
		end
		2'b11: begin
//			tlbHixSelA=regInAddrA[31:22];
			tlbHixSelA={
				regInAddrA[23:22],	regInAddrA[25:24],
				regInAddrA[27:26],	regInAddrA[29:28],
				regInAddrA[31:30]};
		end
	endcase

//	tlbHixSelB = {
//		regInAddrA[20], regInAddrA[21],
//		regInAddrA[22], regInAddrA[23],
//		regInAddrA[24], regInAddrA[25],
//		regInAddrA[28], regInAddrA[27],
//		regInAddrA[28], regInAddrA[29]
//		};
	
//	tlbHixA = tlbHixSelA ^ regInAddrA[25:16];
	tlbHixA = tlbHixSelA + regInAddrA[25:16];
`endif

// `ifdef jx2_tlbsz_256
`ifndef def_true
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
//			tlbHixSelA={regInAddrA[15:12], regInAddrA[27:24]};
			tlbHixSelA={
				regInAddrA[15:14],	regInAddrA[25:24],
				regInAddrA[27:26],	regInAddrA[13:12] };
		end
		2'b01: begin
//			tlbHixSelA=regInAddrA[31:24];
			tlbHixSelA={
				regInAddrA[23:22],	regInAddrA[25:24],
				regInAddrA[27:26],	regInAddrA[29:28] };
		end
		2'b10: begin
//			tlbHixSelA={regInAddrA[15:14], regInAddrA[29:24]};
			tlbHixSelA={
				regInAddrA[15:14],	regInAddrA[25:24],
				regInAddrA[27:26],	regInAddrA[29:28] };
		end
		2'b11: begin
//			tlbHixSelA=regInAddrA[31:24];
			tlbHixSelA={
				regInAddrA[23:22],	regInAddrA[25:24],
				regInAddrA[27:26],	regInAddrA[29:28] };
		end
	endcase
//	tlbHixA = tlbHixSelA ^ regInAddrA[23:16];
	tlbHixA = tlbHixSelA + regInAddrA[23:16];
`endif

`ifdef jx2_tlbsz_256
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
			tlbHixA={regInAddrA[15:12], regInAddrA[19:16]};
		end
		2'b01: begin
			tlbHixA=regInAddrA[23:16];
		end
		2'b10: begin
			tlbHixA={regInAddrA[15:14], regInAddrA[21:16]};
		end
		2'b11: begin
			tlbHixA=regInAddrA[23:16];
		end
	endcase
`endif

`ifdef jx2_tlbsz_64
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

`ifdef jx2_tlbsz_32
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
			tlbHixSelA={regInAddrA[15:12], regInAddrA[21]};
		end
		2'b01: begin
			tlbHixSelA=regInAddrA[25:21];
		end
		2'b10: begin
			tlbHixSelA={regInAddrA[15:14], regInAddrA[23:21]};
		end
		2'b11: begin
			tlbHixSelA=regInAddrA[25:21];
		end
	endcase
	tlbHixA = tlbHixSelA[4:0] ^ regInAddrA[20:16];
`endif

// `ifdef jx2_tlbsz_16
`ifndef def_true
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
			tlbHixSelA=regInAddrA[15:12];
		end
		2'b01: begin
			tlbHixSelA=regInAddrA[23:20];
		end
		2'b10: begin
			tlbHixSelA={regInAddrA[15:14], regInAddrA[21:20]};
		end
		2'b11: begin
			tlbHixSelA=regInAddrA[23:20];
		end
	endcase
//	tlbHixA = tlbHixSelA[3:0] ^ regInAddrA[19:16];
	tlbHixA = tlbHixSelA[3:0] + regInAddrA[19:16];
`endif

`ifdef jx2_tlbsz_16
// `ifndef def_true
	case({tlbMmuPg16K, tlbMmuPg64K})
		2'b00: begin
			tlbHixA=regInAddrA[15:12];
		end
		2'b01: begin
			tlbHixA=regInAddrA[19:16];
		end
		2'b10: begin
			tlbHixA=regInAddrA[17:14];
		end
		2'b11: begin
			tlbHixA=regInAddrA[19:16];
		end
	endcase
`endif

	nxtAclEntryA	= aclEntryA;
	nxtAclEntryB	= aclEntryB;
	nxtAclEntryC	= aclEntryC;
	nxtAclEntryD	= aclEntryD;

//	icPageEq		= (tRegInAddr[47:12] == regInAddrA[47:12]);

	tAddrIsLo4G		= (tRegInAddr[47:32] == 16'h0000);
	tAddrIsHi4G		= (tRegInAddr[47:32] == 16'hFFFF);
//	tAddrIsHiMMIO	= (tRegInAddr[47:32] == 16'hF000);
	tAddrIsHiMMIO	= (tRegInAddr[47:44] == 4'hF) && tlbIs48b;
	tAddrIsMMIO		= ((tRegInAddr[31:28] == 4'hF) &&
		((tAddrIsLo4G && tlbIs32b) || tAddrIsHi4G || tlbIs32b)) ||
		tAddrIsHiMMIO;
	tAddrIsPhys		= (tAddrIsHi4G && !tRegInAddr[31]) ||
		(tRegInAddr[47:44] == 4'hC) ||
		(tRegInAddr[47:44] == 4'hD);
	tAddrIsPhysV	= 
		(tRegInAddr[47:44] == 4'hD);

//	tRegInIsBounce	= (tRegInOpm == UMEM_OPM_RD_BOUNCE);
	tRegInIsBounce	= 0;

	if(regInSR[29] && regInSR[28])
	begin
//		$display("TLB Disable ISR");
		tlbMmuEnable = 0;
	end
	
	if(tlbMmuEnable)
	begin
//		$display("TLB Enabled");
	end


	icPageReady		= 1;

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

`ifdef jx2_enable_l1addr96
	tlbHitB_HiX = (tRegInAddr[95:80] == tlbHdatB[111:96]);
	tlbHitD_HiX = (tRegInAddr[95:80] == tlbHdatD[111:96]);
	tlbHitB_MiX = (tRegInAddr[79:64] == tlbHdatB[95:80]);
	tlbHitD_MiX = (tRegInAddr[79:64] == tlbHdatD[95:80]);
	tlbHitB_LoX = (tRegInAddr[63:48] == tlbHdatB[79:64]);
	tlbHitD_LoX = (tRegInAddr[63:48] == tlbHdatD[79:64]);

	tlbHitB_X = (tlbHitB_HiX && tlbHitB_MiX && tlbHitB_LoX) || 
		(tlbHdatB[1:0]!=2'b10) ;
	tlbHitD_X = (tlbHitD_HiX && tlbHitD_MiX && tlbHitD_LoX) || 
		(tlbHdatD[1:0]!=2'b10) ;
`else
	tlbHitB_HiX = 1;
	tlbHitD_HiX = 1;
	tlbHitB_MiX = 1;
	tlbHitD_MiX = 1;
	tlbHitB_LoX = 1;
	tlbHitD_LoX = 1;
	tlbHitB_X	= 1;
	tlbHitD_X	= 1;
`endif

	if(tlbIs32b)
	begin
		tlbHitA_Hi = 1;
		tlbHitB_Hi = 1;
		tlbHitC_Hi = 1;
		tlbHitD_Hi = 1;
	end

`ifndef def_true

`ifdef jx2_tlb_xtlbe
	if(tlbHdatB[1:0]!=2'b10)
	begin
		tlbHitB_HiX = 1;
		tlbHitB_MiX = 1;
		tlbHitB_LoX = 1;
	end
	if(tlbHdatD[1:0]!=2'b10)
	begin
		tlbHitD_HiX = 1;
		tlbHitD_MiX = 1;
		tlbHitD_LoX = 1;
	end
`else
	tlbHitB_HiX = 1;
	tlbHitD_HiX = 1;
	tlbHitB_MiX = 1;
	tlbHitD_MiX = 1;
	tlbHitB_LoX = 1;
	tlbHitD_LoX = 1;
`endif
`endif

	if(tlbMmuPg64K)
	begin
		tlbHitA_Lo = 1;
		tlbHitB_Lo = 1;
		tlbHitC_Lo = 1;
		tlbHitD_Lo = 1;
	end
	
`ifdef jx2_tlb_xtlbe
	tlbHitA =
		tlbHitA_Hi && tlbHitA_Mi && tlbHitA_Lo &&
		tlbHitB_X && (tlbHdatA[1:0] == 2'b01);
	tlbHitB =
		tlbHitB_Hi && tlbHitB_Mi && tlbHitB_Lo &&
		(tlbHdatB[1:0] == 2'b01);
	tlbHitC =
		tlbHitC_Hi && tlbHitC_Mi && tlbHitC_Lo &&
		tlbHitD_X && (tlbHdatC[1:0] == 2'b01);
	tlbHitD =
		tlbHitD_Hi && tlbHitD_Mi && tlbHitD_Lo &&
		(tlbHdatD[1:0] == 2'b01);

	tlbHitAX = tlbHitA && (tlbHdatB[1:0] == 2'b10);
	tlbHitCX = tlbHitC && (tlbHdatD[1:0] == 2'b10);
`else
	tlbHitA = tlbHitA_Hi && tlbHitA_Mi && tlbHitA_Lo && tlbHdatA[0];
	tlbHitB = tlbHitB_Hi && tlbHitB_Mi && tlbHitB_Lo && tlbHdatB[0];
	tlbHitC = tlbHitC_Hi && tlbHitC_Mi && tlbHitC_Lo && tlbHdatC[0];
	tlbHitD = tlbHitD_Hi && tlbHitD_Mi && tlbHitD_Lo && tlbHdatD[0];
	tlbHitAX = 0;
	tlbHitCX = 0;
`endif

	tlbHitAB = tlbHitA || tlbHitB;
	tlbHitCD = tlbHitC || tlbHitD;
	tlbHit = tlbHitAB || tlbHitCD;

	tlbAddrAB[47:12] = tlbHitA ? tlbHdatA[47:12] : tlbHdatB[47:12];
	tlbAddrCD[47:12] = tlbHitC ? tlbHdatC[47:12] : tlbHdatD[47:12];
	
`ifdef jx2_enable_l1addr96
	tlbAddrAB[95:48] = tlbHitAX ? tlbHdatB[63:16] : UV48_00;
	tlbAddrCD[95:48] = tlbHitCX ? tlbHdatD[63:16] : UV48_00;
	tlbAddr[95:12] = tlbHitAB ? tlbAddrAB[95:12] : tlbAddrCD[95:12];
`else
	tlbAddr[47:12] = tlbHitAB ? tlbAddrAB[47:12] : tlbAddrCD[47:12];
`endif

//	tlbAddr[47:12] = tlbHitAB ? tlbAddrAB[47:12] : tlbAddrCD[47:12];
	tlbAddr[11: 0] = tRegInAddr[11: 0];

	if(!tlbMmuPg4K)
		tlbAddr[13:12] = tRegInAddr[13:12];
	if(tlbMmuPg64K)
		tlbAddr[15:14] = tRegInAddr[15:14];

	tlbAccAB = 	tlbHitA ?
		{ tlbHdatA[3:0], tlbHdatA[127:112], tlbHdatA[75:64], tlbHdatA[7:4] } :
		{ tlbHdatB[3:0], tlbHdatB[127:112], tlbHdatB[75:64], tlbHdatB[7:4] } ;
	tlbAccCD = 	tlbHitC ?
		{ tlbHdatC[3:0], tlbHdatC[127:112], tlbHdatC[75:64], tlbHdatC[7:4] } :
		{ tlbHdatD[3:0], tlbHdatD[127:112], tlbHdatD[75:64], tlbHdatD[7:4] };
	tlbAcc = tlbHitAB ? tlbAccAB : tlbAccCD;
	
	if(tlbAddr[47:44] == 4'h0)
		tlbAddr[47:44] = 4'hC;

	tlbMiss = 0;

	tlbMmuSkip = 0;
	if(tAddrIsMMIO)
		tlbMmuSkip = 1;
	
	if(tAddrIsPhys)
		tlbMmuSkip = 1;
		
	if(tRegInIsCCmd)
		tlbMmuSkip = 1;

	if(tRegInIsIRQ)
		tlbMmuSkip = 1;

	if(!(tRegInIsLDX || tRegInIsSTX))
		tlbMmuSkip = 1;

	tRegOutOpm   = tRegInOpm;
	tRegOutSeq   = tRegInSeq;

	if(tAddrIsPhysV)
		tRegOutOpm[11]	= 1;

	if((tRegInAddr[47] && tRegInIsLDX) || tAddrIsMMIO)
	begin
		if(tlbMmuEnable && !regInSR[30])
		begin
			tRegOutOpm[11:8]	= 4'h7;
		end
	end

//	if(tlbMmuEnable && !tlbMmuSkip && (tRegInOpm[4:3]!=0))
	if(tlbMmuEnable && !tlbMmuSkip)
	begin
		tlbMiss = ! tlbHit;
		
		if(tRegInIsLDTLB)
			tlbMiss = 0;
		
		if(tlbMiss)
		begin
			if(regInSR[29] && regInSR[28])
			begin
				$display("Miss while in ISR");
			end

//			$display("Miss A=%X B=%X SR=%X_%X",
//				tRegInAddr,	tRegInAddrB,
//				regInSR[63:32], regInSR[31:0]);

//			tlbAddr		= 48'h010000;
`ifdef jx2_enable_l1addr96
			tlbAddr		= { 48'h00, 36'h010, tRegInAddr[11:0] };
`else
			tlbAddr		= { 36'h010, tRegInAddr[11:0] };
`endif
			tRegOutOpm[11:8] = 4'hF;
		end
	end
	else
	begin
//		tlbAddr		= tRegInAddr[47:0];
		tlbAddr		= tRegInAddr;
		if(tAddrIsPhys)
			tlbAddr[47:44]	= 4'hC;
		if(tAddrIsPhys && tAddrIsHi4G)
			tlbAddr[43:32]	= 0;
		tlbMiss		= 0;
	end

	tRegOutExc = 0;

	tRegOutTea[47:0]	= tRegInAddr[47:0];
`ifdef jx2_tlb_xtlbe
	tRegOutTeaHi[47:0]	= tRegInAddr[95:48];
`else
	tRegOutTeaHi[47:0]	= 0;
`endif

	if(tlbMmuEnable)
	begin
		if(!tlbMmuSkip)
		begin
			if(tTlbExc[15])
			begin
				tRegOutExc = tTlbExc;

				if(tTlbExc[3:0]==4'h2)
				begin
					tRegOutTea[15: 0]	= tlbAcc  [31:16];
					tRegOutTea[31:16]	= tRegInKRR[15: 0];
				end
			end
			else
			begin
				if(tlbAcc[6:4]==3'b110)
				begin
					tRegOutExc[15:12]	= 4'h7;
					tRegOutTeaHi[63:48]	= tlbAcc[31:16];
				end
			end

		end
	end
	
	tRegOutAddr  = tlbAddr;
//	tRegOutOpm   = tRegInOpm;
//	tRegOutSeq   = tRegInSeq;

`ifndef def_true
//	if(tRegInOpm!=0)
	if(tRegInOpm[7:0]==JX2_RBI_OPM_LDX)
	begin
		$display("TLB-1, %X %X %X", tRegInOpm, tRegInSeq, tlbAddr);
	end
`endif

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


//	if((tRegInAddr[47:0]!=tlbAddr[47:0]) && !tAddrIsPhys)
	if((tRegInAddr[43:0]!=tlbAddr[43:0]) && !tAddrIsPhys)
	begin
//		$display("TLB(A) %X -> %X", tRegInAddr, tlbAddr);
	end

//	if(tRegInIsBounce && !tAddrIsMMIO)
//	begin
//		tRegOutOpm   = UMEM_OPM_LDTLB;
//	end
	
	if(tRegInIsINVTLB || reset)
	begin
		tNxtTlbRov		= tTlbRov + 1;

		nxtAclEntryA	= 0;
		nxtAclEntryB	= 0;
		nxtAclEntryC	= 0;
		nxtAclEntryD	= 0;
	end

	if(tRegInIsIRQ)
	begin
		if(	(unitNodeId[5:2]==regInData[11:8]) ||
			(regInData[11:8]==4'h0) ||
			(regInData[11:8]==4'hF))
		begin
			tRegOutTea  [47:0] = regInData[ 63:16];
			tRegOutTeaHi[63:0] = regInData[127:64];
			tRegOutExc	= regInData[15:0];
			if(regInData[11:8]!=4'hF)
				tRegOutOpm	= UV16_00;
		end
	end

	if(reset)
	begin
		tRegOutExc = 0;
		tRegOutOpm   = UV16_00;
	end

`ifndef def_true
	if(tRegInIsINVACL || reset)
	begin
		nxtAclEntryA	= 0;
		nxtAclEntryB	= 0;
		nxtAclEntryC	= 0;
		nxtAclEntryD	= 0;
	end
`endif

	if(tRegInIsLDACL)
	begin
		nxtAclEntryA	= regInLdtlb[47:0];
		nxtAclEntryB	= aclEntryA;
		nxtAclEntryC	= aclEntryB;
		nxtAclEntryD	= aclEntryC;
	end

	tNxtRegInLdtlbHi	= tRegInLdtlbHi;

	if(tRegInIsLDTLB)
	begin
`ifdef jx2_tlb_xtlbe
		if(tRegInLdtlb[1:0]==2'b10)
		begin
			$display("MemTile-LDTLB Hi %X %X-%X",
				tRegInAddr,
				tRegInLdtlb[127:64],
				regInLdtlb[ 63: 0]);

			tNxtRegInLdtlbHi = tRegInLdtlb;
		end
		else if(tRegInLdtlbHi[1:0]==2'b10)
		begin
			$display("MemTile-LDTLB HiLo %X %X-%X",
				tRegInAddr,
				tRegInLdtlb[127:64],
				regInLdtlb[ 63: 0]);
			tlbDoLdtlb	= icPageReady;
			tNxtRegInLdtlbHi[1:0] = 0;

			tlbLdHdatA = { 8'h00, tTlbRov, tRegInLdtlb[127:0] };
			tlbLdHdatB = { 8'h00, tTlbRov, tRegInLdtlbHi[127:0] };
			tlbLdHdatC = tlbHdatA;
			tlbLdHdatD = tlbHdatB;
		end
		else
		begin
			$display("MemTile-LDTLB Base %X %X-%X",
				tRegInAddr,
				tRegInLdtlb[127:64],
				regInLdtlb[ 63: 0]);
			tlbDoLdtlb	= icPageReady;
			tNxtRegInLdtlbHi[1:0] = 0;

			tlbLdHdatA = { 8'h00, tTlbRov, tRegInLdtlb[127:0] };
			tlbLdHdatB = tlbHdatC;
			tlbLdHdatC = tlbHdatA;
			tlbLdHdatD = tlbHdatB;
			
			if(tlbHdatD[1:0]==2'b10)
				tlbLdHdatB[1:0]=0;
		end
`else
		$display("MemTile-LDTLB %X %X-%X",
			tRegInAddr,
			tRegInLdtlb[127:64],
			regInLdtlb[ 63: 0]);
		tlbDoLdtlb	= icPageReady;
		tNxtRegInLdtlbHi[1:0] = 0;

		tlbLdHdatA = { 8'h00, tTlbRov, tRegInLdtlb[127:0] };
		tlbLdHdatB = tlbHdatA;
		tlbLdHdatC = tlbHdatB;
		tlbLdHdatD = tlbHdatC;
`endif
	end
	else
	begin

//		$display("TLB Opm=%X Ok=%X", regInOpm, tRegOutOK);

`ifdef def_true
		if(tlbMmuEnable && icPageReady &&
			!tlbMiss && !tlbMmuSkip &&
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
// `ifdef def_true
`ifndef def_true
		tRegInAddr		<= regInAddr;
		tRegInData		<= regInData;
		tRegInLdtlb		<= regInLdtlb;
		tRegInLdtlbHi	<= tNxtRegInLdtlbHi;
		if(!tlbMmuEnable)
		begin
			tRegOutAddr2	<= regInAddr;
			tRegOutData2	<= regInData;
			tRegOutOpm2		<= regInOpm;
			tRegOutSeq2		<= regInSeq;

			if(regInIsPhysAddrV)
				tRegOutOpm2[11]	<= 1;

//			tRegInOpm		<= regInOpm;
//			tRegInSeq		<= regInSeq;			
			tRegInOpm		<= regInIsReqIO ? 0 : regInOpm;
			tRegInSeq		<= regInIsReqIO ? 0 : regInSeq;
		end
		else if(
			((regInIsPhysAddr && regInIsNzX) || regInIsMMIO) &&
			(tRegOutOpm[7:0]==8'h00))
		begin
			/* Physical Addresses and MMIO may skip TLB */
			tRegOutAddr2	<= regInAddr;
			tRegOutData2	<= regInData;
			tRegOutOpm2		<= regInOpm;
			tRegOutSeq2		<= regInSeq;
			
			if(regInIsPhysAddrV)
				tRegOutOpm2[11]	<= 1;

			tRegInOpm		<= 0;
			tRegInSeq		<= 0;
		end
		else
		begin
			tRegOutAddr2	<= tRegOutAddr;
			tRegOutData2	<= tRegOutData;
		//	tRegOutOpm2		<= tRegOutOpm;
//			tRegOutOpm2		<= { 2'b00, tChkAccNoRwx, tRegOutOpm[7:0] };
			tRegOutOpm2		<= { 2'b00,
				tChkAccNoRwx | tRegOutOpm[13:8],
				tRegOutOpm[7:0] };
			tRegOutSeq2		<= tRegOutSeq;

			tRegInOpm		<= regInOpm;
			tRegInSeq		<= regInSeq;
		end
`else
		tRegOutAddr2	<= tRegOutAddr;
		tRegOutData2	<= tRegOutData;
	//	tRegOutOpm2		<= tRegOutOpm;
		tRegOutOpm2		<= { 2'b00,
			tChkAccNoRwx | tRegOutOpm[13:8],
//			tChkAccNoRwx,
			tRegOutOpm[7:0] };
		tRegOutSeq2		<= tRegOutSeq;

		tRegInAddr		<= regInAddr;
		tRegInData		<= regInData;
		tRegInLdtlb		<= regInLdtlb;
		tRegInOpm		<= regInOpm;
		tRegInSeq		<= regInSeq;

		tRegInLdtlbHi	<= tNxtRegInLdtlbHi;
`endif

//		tRegOutAddr3	<= tRegOutAddr2;
//		tRegOutData3	<= tRegOutData2;
//		tRegOutOpm3		<= tRegOutOpm2;
//		tRegOutSeq3		<= tRegOutSeq2;

		tRegOutExc2		<= tRegOutExc;
		tRegOutTea2		<= tRegOutTea;
		tRegOutTeaHi2	<= tRegOutTeaHi;

//		tRegInAddr		<= regInAddr;
//		tRegInData		<= regInData;
//		tRegInOpm		<= regInOpm;
//		tRegInSeq		<= regInSeq;
		
`ifndef def_true
		if(regInOpm[7:0]==JX2_RBI_OPM_LDX)
		begin
			$display("TLB-0 %X %X %X", regInOpm, regInSeq, regInAddr);
		end
`endif

		tTlbRov			<= tNxtTlbRov;

		tlbHbIxA		<= tlbHixA;
//		tlbHbIxB		<= tlbHixB;
	end

	aclEntryA		<= nxtAclEntryA;
	aclEntryB		<= nxtAclEntryB;
	aclEntryC		<= nxtAclEntryC;
	aclEntryD		<= nxtAclEntryD;

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
