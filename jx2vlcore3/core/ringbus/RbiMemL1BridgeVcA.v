/*
 Copyright (c) 2018-2022 Brendan G Bohannon

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Bridge between the L1 and L2 Rings.

Add a small associative cache to the ring, intended to absorb conflict misses.

 */

`include "ringbus/RbiDefs.v"

module RbiMemL1BridgeVcA(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInMmcr,		regInKrr,		regInSr,

	l1mAddrIn,		l1mAddrOut,
	l1mDataIn,		l1mDataOut,
	l1mOpmIn,		l1mOpmOut,
	l1mSeqIn,		l1mSeqOut,

	l2mAddrIn,		l2mAddrOut,
	l2mDataIn,		l2mDataOut,
	l2mOpmIn,		l2mOpmOut,
	l2mSeqIn,		l2mSeqOut,

	unitNodeId,		regRngBridge
	);

input			clock;
input			reset;
input[63:0]		regInMmcr;
input[63:0]		regInKrr;
input[63:0]		regInSr;

`input_rbseq	l1mSeqIn;		//operation sequence
`output_rbseq	l1mSeqOut;		//operation sequence
`input_rbopm	l1mOpmIn;		//memory operation mode
`output_rbopm	l1mOpmOut;		//memory operation mode
`input_l1addr	l1mAddrIn;		//memory input address
`output_l1addr	l1mAddrOut;		//memory output address
`input_tile		l1mDataIn;		//memory input data
`output_tile	l1mDataOut;		//memory output data

`input_rbseq	l2mSeqIn;		//operation sequence
`output_rbseq	l2mSeqOut;		//operation sequence
`input_rbopm	l2mOpmIn;		//memory operation mode
`output_rbopm	l2mOpmOut;		//memory operation mode
`input_l2addr	l2mAddrIn;		//memory input address
`output_l2addr	l2mAddrOut;		//memory output address
`input_tile		l2mDataIn;		//memory input data
`output_tile	l2mDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?
input [  7:0]	regRngBridge;	//Random Sequence (Updates on L1 Flush)


`reg_rbseq		tL1mSeqIn;		//operation sequence
`reg_rbopm		tL1mOpmIn;		//memory operation mode
`reg_l1addr		tL1mAddrIn;		//memory input address
`reg_tile		tL1mDataIn;		//memory input data

`reg_rbseq		tL2mSeqIn;		//operation sequence
`reg_rbopm		tL2mOpmIn;		//memory operation mode
`reg_l2addr		tL2mAddrIn;		//memory input address
`reg_tile		tL2mDataIn;		//memory input data


`reg_rbseq		tL1mSeqOut;			//operation sequence
`reg_rbopm		tL1mOpmOut;			//memory operation mode
`reg_l1addr		tL1mAddrOut;		//memory output address
`reg_tile		tL1mDataOut;		//memory output data

`reg_rbseq		tL2mSeqOut;			//operation sequence
`reg_rbopm		tL2mOpmOut;			//memory operation mode
`reg_l2addr		tL2mAddrOut;		//memory output address
`reg_tile		tL2mDataOut;		//memory output data


`reg_rbseq		tL1mSeqOut2;			//operation sequence
`reg_rbopm		tL1mOpmOut2;			//memory operation mode
`reg_l1addr		tL1mAddrOut2;		//memory output address
`reg_tile		tL1mDataOut2;		//memory output data

assign		l1mSeqOut	= tL1mSeqOut2;
assign		l1mOpmOut	= tL1mOpmOut2;
assign		l1mAddrOut	= tL1mAddrOut2;
assign		l1mDataOut	= tL1mDataOut2;


`reg_rbseq		tL2mSeqOut2;			//operation sequence
`reg_rbopm		tL2mOpmOut2;			//memory operation mode
`reg_l2addr		tL2mAddrOut2;		//memory output address
`reg_tile		tL2mDataOut2;		//memory output data

assign		l2mSeqOut	= tL2mSeqOut2;
assign		l2mOpmOut	= tL2mOpmOut2;
assign		l2mAddrOut	= tL2mAddrOut2;
assign		l2mDataOut	= tL2mDataOut2;



reg		tL1mReqSent;
reg		tL2mReqSent;

wire			l1mRingIsIdle;
wire			l2mRingIsIdle;

assign		l1mRingIsIdle = (tL1mOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign		l2mRingIsIdle = (tL2mOpmIn[7:0] == JX2_RBI_OPM_IDLE);

wire			l1mRingIsReq;
wire			l2mRingIsResp;
wire			l2mRingIsRespOther;
wire			l2mRingIsMemLdResp;

wire			l1mRingIsIrq;
wire			l2mRingIsIrq;
wire			l2mRingIsIrqBc;

assign		l1mRingIsReq = tL1mOpmIn[ 7:6] == 2'b10;

assign		l2mRingIsResp =
	(tL2mOpmIn[ 7:6] == 2'b01) &&
	(tL2mSeqIn[15:10] == unitNodeId[7:2]);

assign		l2mRingIsMemLdResp =
	l2mRingIsResp && (tL2mOpmIn[ 5:4] == 2'b11);

assign		l2mRingIsRespOther =
	(tL2mOpmIn[ 7:6] == 2'b01) &&
	(tL2mSeqIn[15:10] != unitNodeId[7:2]);

assign		l1mRingIsIrq =
	(tL2mOpmIn[ 7:0] == JX2_RBI_OPM_IRQ) &&
	((tL2mDataIn[11:8] != unitNodeId[5:2]) ||
//	 (tL2mDataIn[11:8] == 4'h0) ||
	 (tL2mDataIn[11:8] == 4'hF));

assign		l2mRingIsIrq =
	(tL2mOpmIn[ 7:0] == JX2_RBI_OPM_IRQ) &&
	((tL2mDataIn[11:8] == unitNodeId[5:2]) ||
	 (tL2mDataIn[11:8] == 4'h0) ||
	 (tL2mDataIn[11:8] == 4'hF));
assign		l2mRingIsIrqBc = l2mRingIsIrq && (tL2mDataIn[11:8] == 4'hF);

`reg_rbseq		tL1mSeqReq;			//operation sequence
`reg_rbopm		tL1mOpmReq;			//memory operation mode
`reg_l1addr		tL1mAddrReq;		//memory output address
reg[127:0]		tL1mDataReq;		//memory output data

`reg_rbseq		tL2mSeqReq;			//operation sequence
`reg_rbopm		tL2mOpmReq;			//memory operation mode
`reg_l2addr		tL2mAddrReq;		//memory output address
reg[127:0]		tL2mDataReq;		//memory output data

wire			l1mOpmIn_IsMemStReq =
	(tL1mOpmIn[7:0]==JX2_RBI_OPM_STX) ||
	(tL1mOpmIn[7:0]==JX2_RBI_OPM_STXC);

wire			l1mOpmIn_AddrRejectROM =
	(tL1mAddrIn[31:24] == 8'h00);
//	(tL1mAddrIn[31:24] == 8'h00) &&
//	(	(tL1mAddrIn[23:16] == 8'h00) &&
//		(tL1mAddrIn[15:14] == 2'b11));

wire			l1mOpmIn_IsMemLdReq =
	(tL1mOpmIn[7:0]==JX2_RBI_OPM_PFX)	||
	(tL1mOpmIn[7:0]==JX2_RBI_OPM_SPX)	||
	(tL1mOpmIn[7:0]==JX2_RBI_OPM_LDX)	||
	(tL1mOpmIn[7:0]==JX2_RBI_OPM_LDXC)	;

wire			l2mOpmIn_IsReq =
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_PFX)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_SPX)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_LDX)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_LDSQ)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_LDSL)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_LDUL)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_STX)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_LDXC)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_STXC)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_STSQ)	||
	(tL2mOpmIn[7:0]==JX2_RBI_OPM_STSL)	;

// reg				tUpdateL2b;			//Modify Value on L2 Ring

`ifdef jx2_rbi_bridge_vca_a256		//Ringbus: Increase to 8-way

reg[127:0]		tArrDataA[255:0];
reg[127:0]		tArrDataB[255:0];
reg[127:0]		tArrDataC[255:0];
reg[127:0]		tArrDataD[255:0];

reg[47:0]		tArrAddrA[255:0];
reg[47:0]		tArrAddrB[255:0];
reg[47:0]		tArrAddrC[255:0];
reg[47:0]		tArrAddrD[255:0];

`ifdef jx2_rbi_bridge_vca_8x
reg[127:0]		tArrDataS[255:0];
reg[127:0]		tArrDataT[255:0];
reg[127:0]		tArrDataU[255:0];
reg[127:0]		tArrDataV[255:0];

reg[47:0]		tArrAddrS[255:0];
reg[47:0]		tArrAddrT[255:0];
reg[47:0]		tArrAddrU[255:0];
reg[47:0]		tArrAddrV[255:0];
`endif

reg[7:0]		tArrIx;
reg[7:0]		tBlkStIx;

`else

(* ram_style = "distributed" *)
	reg[127:0]		tArrDataA[63:0];
(* ram_style = "distributed" *)
	reg[127:0]		tArrDataB[63:0];
(* ram_style = "distributed" *)
	reg[127:0]		tArrDataC[63:0];
(* ram_style = "distributed" *)
	reg[127:0]		tArrDataD[63:0];

(* ram_style = "distributed" *)
	reg[47:0]		tArrAddrA[63:0];
(* ram_style = "distributed" *)
	reg[47:0]		tArrAddrB[63:0];
(* ram_style = "distributed" *)
	reg[47:0]		tArrAddrC[63:0];
(* ram_style = "distributed" *)
	reg[47:0]		tArrAddrD[63:0];

`ifdef jx2_rbi_bridge_vca_8x
(* ram_style = "distributed" *)
	reg[127:0]		tArrDataS[63:0];
(* ram_style = "distributed" *)
	reg[127:0]		tArrDataT[63:0];
(* ram_style = "distributed" *)
	reg[127:0]		tArrDataU[63:0];
(* ram_style = "distributed" *)
	reg[127:0]		tArrDataV[63:0];

(* ram_style = "distributed" *)
	reg[47:0]		tArrAddrS[63:0];
(* ram_style = "distributed" *)
	reg[47:0]		tArrAddrT[63:0];
(* ram_style = "distributed" *)
	reg[47:0]		tArrAddrU[63:0];
(* ram_style = "distributed" *)
	reg[47:0]		tArrAddrV[63:0];
`endif

// reg[4:0]		tArrIx;
// reg[4:0]		tBlkStIx;

reg[5:0]		tArrIx;
reg[5:0]		tBlkStIx;

`endif


reg[3:0]		tNxtArrChk;
reg[3:0]		tArrChk;

reg				tDoBlkSt;

reg[127:0]		tBlkDataA;
reg[127:0]		tBlkDataB;
reg[127:0]		tBlkDataC;
reg[127:0]		tBlkDataD;

reg[47:0]		tBlkAddrA;
reg[47:0]		tBlkAddrB;
reg[47:0]		tBlkAddrC;
reg[47:0]		tBlkAddrD;
reg				tBlkHitA;
reg				tBlkHitB;
reg				tBlkHitC;
reg				tBlkHitD;

reg[127:0]		tBlkDataH;
reg[47:0]		tBlkAddrH;
reg				tBlkHitH;

reg[2:0]		tBlkHitId;

reg[127:0]		tStBlkDataA;
reg[127:0]		tStBlkDataB;
reg[127:0]		tStBlkDataC;
reg[127:0]		tStBlkDataD;

reg[47:0]		tStBlkAddrA;
reg[47:0]		tStBlkAddrB;
reg[47:0]		tStBlkAddrC;
reg[47:0]		tStBlkAddrD;

`ifdef jx2_rbi_bridge_vca_8x

reg[127:0]		tBlkDataS;
reg[127:0]		tBlkDataT;
reg[127:0]		tBlkDataU;
reg[127:0]		tBlkDataV;

reg[47:0]		tBlkAddrS;
reg[47:0]		tBlkAddrT;
reg[47:0]		tBlkAddrU;
reg[47:0]		tBlkAddrV;
reg				tBlkHitS;
reg				tBlkHitT;
reg				tBlkHitU;
reg				tBlkHitV;

reg[127:0]		tStBlkDataS;
reg[127:0]		tStBlkDataT;
reg[127:0]		tStBlkDataU;
reg[127:0]		tStBlkDataV;

reg[47:0]		tStBlkAddrS;
reg[47:0]		tStBlkAddrT;
reg[47:0]		tStBlkAddrU;
reg[47:0]		tStBlkAddrV;

`endif

reg			tStBlkAddrB_Adv;
reg			tStBlkAddrC_Adv;
reg			tStBlkAddrD_Adv;
reg			tStBlkAddrS_Adv;
reg			tStBlkAddrT_Adv;
reg			tStBlkAddrU_Adv;
reg			tStBlkAddrV_Adv;

reg			tStBlkAddrA_Flu;
reg			tStBlkAddrB_Flu;
reg			tStBlkAddrC_Flu;
reg			tStBlkAddrD_Flu;
reg			tStBlkAddrS_Flu;
reg			tStBlkAddrT_Flu;
reg			tStBlkAddrU_Flu;
reg			tStBlkAddrV_Flu;

always @*
begin
`ifdef jx2_rbi_bridge_vca_a256
	tArrIx		= tL1mAddrIn[11:4];
`else
//		tArrIx		= tL1mAddrIn[8:4];
	tArrIx		= tL1mAddrIn[9:4];
`endif

	if(	l2mRingIsMemLdResp &&
		!l1mOpmIn_IsMemStReq &&
		!l1mOpmIn_IsMemLdReq )
	begin
`ifdef jx2_rbi_bridge_vca_a256
		tArrIx	= tL2mAddrIn[11:4];
`else
//		tArrIx	= tL2mAddrIn[8:4];
		tArrIx	= tL2mAddrIn[9:4];
`endif
	end

	tBlkStIx	= tArrIx;
	tDoBlkSt	= 0;
	
	tNxtArrChk		= regRngBridge[7:4] ^ regRngBridge[3:0] ^ 4'h5;

	tBlkDataA	= tArrDataA[tArrIx];
	tBlkDataB	= tArrDataB[tArrIx];
	tBlkDataC	= tArrDataC[tArrIx];
	tBlkDataD	= tArrDataD[tArrIx];

	tBlkAddrA	= tArrAddrA[tArrIx];
	tBlkAddrB	= tArrAddrB[tArrIx];
	tBlkAddrC	= tArrAddrC[tArrIx];
	tBlkAddrD	= tArrAddrD[tArrIx];

`ifdef jx2_rbi_bridge_vca_8x
	tBlkDataS	= tArrDataS[tArrIx];
	tBlkDataT	= tArrDataT[tArrIx];
	tBlkDataU	= tArrDataU[tArrIx];
	tBlkDataV	= tArrDataV[tArrIx];

	tBlkAddrS	= tArrAddrS[tArrIx];
	tBlkAddrT	= tArrAddrT[tArrIx];
	tBlkAddrU	= tArrAddrU[tArrIx];
	tBlkAddrV	= tArrAddrV[tArrIx];
`endif

//		tBlkHitA	= tBlkAddrA[43:4] == tL1mAddrIn[43:4];
//		tBlkHitB	= tBlkAddrB[43:4] == tL1mAddrIn[43:4];
//		tBlkHitC	= tBlkAddrC[43:4] == tL1mAddrIn[43:4];
//		tBlkHitD	= tBlkAddrD[43:4] == tL1mAddrIn[43:4];

	tBlkHitA	= tBlkAddrA[31:4] == tL1mAddrIn[31:4];
	tBlkHitB	= tBlkAddrB[31:4] == tL1mAddrIn[31:4];
	tBlkHitC	= tBlkAddrC[31:4] == tL1mAddrIn[31:4];
	tBlkHitD	= tBlkAddrD[31:4] == tL1mAddrIn[31:4];

`ifdef jx2_rbi_bridge_vca_8x
//		tBlkHitS	= tBlkAddrS[43:4] == tL1mAddrIn[43:4];
//		tBlkHitT	= tBlkAddrT[43:4] == tL1mAddrIn[43:4];
//		tBlkHitU	= tBlkAddrU[43:4] == tL1mAddrIn[43:4];
//		tBlkHitV	= tBlkAddrV[43:4] == tL1mAddrIn[43:4];

	tBlkHitS	= tBlkAddrS[31:4] == tL1mAddrIn[31:4];
	tBlkHitT	= tBlkAddrT[31:4] == tL1mAddrIn[31:4];
	tBlkHitU	= tBlkAddrU[31:4] == tL1mAddrIn[31:4];
	tBlkHitV	= tBlkAddrV[31:4] == tL1mAddrIn[31:4];
`endif


`ifdef jx2_rbi_bridge_vca_8x
	casez({tBlkHitA, tBlkHitB, tBlkHitC, tBlkHitD,
		tBlkHitS, tBlkHitT, tBlkHitU, tBlkHitV})
		8'b1zzzzzzz: begin tBlkHitId = 0; tBlkHitH	= 1; end
		8'b01zzzzzz: begin tBlkHitId = 1; tBlkHitH	= 1; end
		8'b001zzzzz: begin tBlkHitId = 2; tBlkHitH	= 1; end
		8'b0001zzzz: begin tBlkHitId = 3; tBlkHitH	= 1; end
		8'b00001zzz: begin tBlkHitId = 4; tBlkHitH	= 1; end
		8'b000001zz: begin tBlkHitId = 5; tBlkHitH	= 1; end
		8'b0000001z: begin tBlkHitId = 6; tBlkHitH	= 1; end
		8'b00000001: begin tBlkHitId = 7; tBlkHitH	= 1; end
		8'b00000000: begin tBlkHitId = 0; tBlkHitH	= 0; end
	endcase
	
	case(tBlkHitId[2:0])
		3'h0: begin
			tBlkDataH	= tBlkDataA;
			tBlkAddrH	= tBlkAddrA;
		end
		3'h1: begin
			tBlkDataH	= tBlkDataB;
			tBlkAddrH	= tBlkAddrB;
		end
		3'h2: begin
			tBlkDataH	= tBlkDataC;
			tBlkAddrH	= tBlkAddrC;
		end
		3'h3: begin
			tBlkDataH	= tBlkDataD;
			tBlkAddrH	= tBlkAddrD;
		end
		3'h4: begin
			tBlkDataH	= tBlkDataS;
			tBlkAddrH	= tBlkAddrS;
		end
		3'h5: begin
			tBlkDataH	= tBlkDataT;
			tBlkAddrH	= tBlkAddrT;
		end
		3'h6: begin
			tBlkDataH	= tBlkDataU;
			tBlkAddrH	= tBlkAddrU;
		end
		3'h7: begin
			tBlkDataH	= tBlkDataV;
			tBlkAddrH	= tBlkAddrV;
		end
	endcase
`else
	casez({tBlkHitA, tBlkHitB, tBlkHitC, tBlkHitD})
		4'b1zzz: begin tBlkHitId = 0; tBlkHitH	= 1; end
		4'b01zz: begin tBlkHitId = 1; tBlkHitH	= 1; end
		4'b001z: begin tBlkHitId = 2; tBlkHitH	= 1; end
		4'b0001: begin tBlkHitId = 3; tBlkHitH	= 1; end
		4'b0000: begin tBlkHitId = 0; tBlkHitH	= 0; end
	endcase
	
	case(tBlkHitId[1:0])
		2'h0: begin
			tBlkDataH	= tBlkDataA;
			tBlkAddrH	= tBlkAddrA;
		end
		2'h1: begin
			tBlkDataH	= tBlkDataB;
			tBlkAddrH	= tBlkAddrB;
		end
		2'h2: begin
			tBlkDataH	= tBlkDataC;
			tBlkAddrH	= tBlkAddrC;
		end
		2'h3: begin
			tBlkDataH	= tBlkDataD;
			tBlkAddrH	= tBlkAddrD;
		end
	endcase
`endif


`ifndef def_true

`ifdef jx2_rbi_bridge_vca_8x
	tStBlkDataV	= tBlkDataU;
	tStBlkDataU	= tBlkDataT;
	tStBlkDataT	= tBlkDataS;
	tStBlkDataS	= tBlkDataD;

	tStBlkAddrV	= tBlkAddrU;
	tStBlkAddrU	= tBlkAddrT;
	tStBlkAddrT	= tBlkAddrS;
	tStBlkAddrS	= tBlkAddrD;
`endif

	tStBlkDataD	= tBlkDataC;
	tStBlkDataC	= tBlkDataB;
	tStBlkDataB	= tBlkDataA;
	tStBlkDataA	= tL1mDataIn;

	tStBlkAddrD	= tBlkAddrC;
	tStBlkAddrC	= tBlkAddrB;
	tStBlkAddrB	= tBlkAddrA;
	tStBlkAddrA	= tL1mAddrIn;
`endif

//	tStBlkDataA	= UV128_XX;
//	tStBlkAddrA	= { UV8_FF, UV40_XX };

	tStBlkDataA	= tBlkDataH;
	tStBlkAddrA	= tBlkAddrH;

	tStBlkAddrB_Adv = 1;
	tStBlkAddrC_Adv = 1;
	tStBlkAddrD_Adv = 1;
	tStBlkAddrS_Adv = 1;
	tStBlkAddrT_Adv = 1;
	tStBlkAddrU_Adv = 1;
	tStBlkAddrV_Adv = 1;

	tStBlkAddrA_Flu = 0;
	tStBlkAddrB_Flu = 0;
	tStBlkAddrC_Flu = 0;
	tStBlkAddrD_Flu = 0;
	tStBlkAddrS_Flu = 0;
	tStBlkAddrT_Flu = 0;
	tStBlkAddrU_Flu = 0;
	tStBlkAddrV_Flu = 0;

	if(l1mOpmIn_IsMemStReq &&
		(tL1mAddrIn[47:44]==4'hC) &&
		(tL1mAddrIn[43:32]==0) &&
		!l1mOpmIn_AddrRejectROM &&
		!tL1mOpmIn[11])
	begin
		tStBlkDataA	= tL1mDataIn;
		tStBlkAddrA	= tL1mAddrIn;
		tStBlkAddrA[3:0] = tArrChk;
		tDoBlkSt	= 1;
	end

// `ifndef def_true
`ifdef def_true
	if(
		l2mRingIsMemLdResp &&
		(tL2mAddrIn[47:44]==4'hC) &&
		(tL2mAddrIn[43:32]==0) &&
		(tArrIx[5:0] == tL2mAddrIn[9:4]) &&
		!tL2mOpmIn[11] )
//		!tL2mOpmIn[11] &&
//		!tL2mOpmIn[3])
	begin
		tStBlkDataA	= tL2mDataIn;
		tStBlkAddrA	= tL2mAddrIn;
		tStBlkAddrA[3:0] = tArrChk;
		tDoBlkSt	= 1;
	end
`endif

//	if(tStBlkAddrA[31:24]==0)
//		tDoBlkSt	= 0;
//	if(tStBlkAddrA[47:44]!=4'hC)
//		tDoBlkSt	= 0;

//	if(tStBlkAddrA[43:32]!=0)
//		tDoBlkSt	= 0;
//	if(tStBlkAddrA[31:27]!=0)
//		tDoBlkSt	= 0;

	if((tL1mAddrIn[47:44]==4'hD) || tL1mOpmIn[11])
	begin
		/* If flushing a line, flush all the ways. */

		tStBlkAddrB_Adv = 0;
		tStBlkAddrC_Adv = 0;
		tStBlkAddrD_Adv = 0;
		tStBlkAddrS_Adv = 0;
		tStBlkAddrT_Adv = 0;
		tStBlkAddrU_Adv = 0;
		tStBlkAddrV_Adv = 0;

//		tStBlkAddrA_Flu = 1;
//		tStBlkAddrB_Flu = 1;
//		tStBlkAddrC_Flu = 1;
//		tStBlkAddrD_Flu = 1;
//		tStBlkAddrS_Flu = 1;
//		tStBlkAddrT_Flu = 1;
//		tStBlkAddrU_Flu = 1;
//		tStBlkAddrV_Flu = 1;

		tStBlkAddrA_Flu = tBlkHitA;
		tStBlkAddrB_Flu = tBlkHitB;
		tStBlkAddrC_Flu = tBlkHitC;
		tStBlkAddrD_Flu = tBlkHitD;

`ifdef jx2_rbi_bridge_vca_8x
		tStBlkAddrS_Flu = tBlkHitS;
		tStBlkAddrT_Flu = tBlkHitT;
		tStBlkAddrU_Flu = tBlkHitU;
		tStBlkAddrV_Flu = tBlkHitV;
`endif

		tDoBlkSt	= 1;
	end


`ifdef jx2_rbi_bridge_vca_mtf
	/* If a line had hit in the cache, move it to the front. */

//	if(!tDoBlkSt)
//	begin
//		tStBlkDataA	= tBlkDataH;
//		tStBlkAddrA	= tBlkAddrH;
//	end

	case(tBlkHitId[2:0])
		3'h0: begin
		end

		3'h1: begin
			tStBlkAddrB_Adv = 1;
			tStBlkAddrC_Adv = 0;
			tStBlkAddrD_Adv = 0;
			tStBlkAddrS_Adv = 0;
			tStBlkAddrT_Adv = 0;
			tStBlkAddrU_Adv = 0;
			tStBlkAddrV_Adv = 0;

			tDoBlkSt	= 1;
		end
		3'h2: begin
			tStBlkAddrB_Adv = 1;
			tStBlkAddrC_Adv = 1;
			tStBlkAddrD_Adv = 0;
			tStBlkAddrS_Adv = 0;
			tStBlkAddrT_Adv = 0;
			tStBlkAddrU_Adv = 0;
			tStBlkAddrV_Adv = 0;

			tDoBlkSt	= 1;
		end
		3'h3: begin
			tStBlkAddrB_Adv = 1;
			tStBlkAddrC_Adv = 1;
			tStBlkAddrD_Adv = 1;
			tStBlkAddrS_Adv = 0;
			tStBlkAddrT_Adv = 0;
			tStBlkAddrU_Adv = 0;
			tStBlkAddrV_Adv = 0;

			tDoBlkSt	= 1;
		end
`ifdef jx2_rbi_bridge_vca_8x
		3'h4: begin
			tStBlkAddrB_Adv = 1;
			tStBlkAddrC_Adv = 1;
			tStBlkAddrD_Adv = 1;
			tStBlkAddrS_Adv = 1;
			tStBlkAddrT_Adv = 0;
			tStBlkAddrU_Adv = 0;
			tStBlkAddrV_Adv = 0;

			tDoBlkSt	= 1;
		end
		3'h5: begin
			tStBlkAddrB_Adv = 1;
			tStBlkAddrC_Adv = 1;
			tStBlkAddrD_Adv = 1;
			tStBlkAddrS_Adv = 1;
			tStBlkAddrT_Adv = 1;
			tStBlkAddrU_Adv = 0;
			tStBlkAddrV_Adv = 0;

			tDoBlkSt	= 1;
		end
		3'h6: begin
			tStBlkAddrB_Adv = 1;
			tStBlkAddrC_Adv = 1;
			tStBlkAddrD_Adv = 1;
			tStBlkAddrS_Adv = 1;
			tStBlkAddrT_Adv = 1;
			tStBlkAddrU_Adv = 1;
			tStBlkAddrV_Adv = 0;

			tDoBlkSt	= 1;
		end
		3'h7: begin
			tStBlkAddrB_Adv = 1;
			tStBlkAddrC_Adv = 1;
			tStBlkAddrD_Adv = 1;
			tStBlkAddrS_Adv = 1;
			tStBlkAddrT_Adv = 1;
			tStBlkAddrU_Adv = 1;
			tStBlkAddrV_Adv = 1;

			tDoBlkSt	= 1;
		end
`else
		default: begin
		end
`endif
	endcase
`endif

	tStBlkDataD	= tStBlkAddrD_Adv ? tBlkDataC : tBlkDataD;
	tStBlkDataC	= tStBlkAddrC_Adv ? tBlkDataB : tBlkDataC;
	tStBlkDataB	= tStBlkAddrB_Adv ? tBlkDataA : tBlkDataB;

	tStBlkAddrD	= tStBlkAddrD_Adv ? tBlkAddrC : tBlkAddrD;
	tStBlkAddrC	= tStBlkAddrC_Adv ? tBlkAddrB : tBlkAddrC;
	tStBlkAddrB	= tStBlkAddrB_Adv ? tBlkAddrA : tBlkAddrB;
	
	if(tStBlkAddrD_Adv ? tStBlkAddrC_Flu : tStBlkAddrD_Flu)
		tStBlkAddrD[47:44] = 4'hF;
	if(tStBlkAddrC_Adv ? tStBlkAddrB_Flu : tStBlkAddrC_Flu)
		tStBlkAddrC[47:44] = 4'hF;
	if(tStBlkAddrB_Adv ? tStBlkAddrA_Flu : tStBlkAddrB_Flu)
		tStBlkAddrB[47:44] = 4'hF;
	if(tStBlkAddrA_Flu)
		tStBlkAddrA[47:44] = 4'hF;

`ifdef jx2_rbi_bridge_vca_8x
	tStBlkDataV	= tStBlkAddrV_Adv ? tBlkDataU : tBlkDataV;
	tStBlkDataU	= tStBlkAddrU_Adv ? tBlkDataT : tBlkDataU;
	tStBlkDataT	= tStBlkAddrT_Adv ? tBlkDataS : tBlkDataT;
	tStBlkDataS	= tStBlkAddrS_Adv ? tBlkDataD : tBlkDataS;

	tStBlkAddrV	= tStBlkAddrV_Adv ? tBlkAddrU : tBlkAddrV;
	tStBlkAddrU	= tStBlkAddrU_Adv ? tBlkAddrT : tBlkAddrU;
	tStBlkAddrT	= tStBlkAddrT_Adv ? tBlkAddrS : tBlkAddrT;
	tStBlkAddrS	= tStBlkAddrS_Adv ? tBlkAddrD : tBlkAddrS;
	
	if(tStBlkAddrV_Adv ? tStBlkAddrU_Flu : tStBlkAddrV_Flu)
		tStBlkAddrV[47:44] = 4'hF;
	if(tStBlkAddrU_Adv ? tStBlkAddrT_Flu : tStBlkAddrU_Flu)
		tStBlkAddrU[47:44] = 4'hF;
	if(tStBlkAddrT_Adv ? tStBlkAddrS_Flu : tStBlkAddrT_Flu)
		tStBlkAddrT[47:44] = 4'hF;
	if(tStBlkAddrS_Adv ? tStBlkAddrD_Flu : tStBlkAddrS_Flu)
		tStBlkAddrS[47:44] = 4'hF;
`endif

`ifdef def_true
	tStBlkAddrA[43:32] = 0;
	tStBlkAddrB[43:32] = 0;
	tStBlkAddrC[43:32] = 0;
	tStBlkAddrD[43:32] = 0;

`ifdef jx2_rbi_bridge_vca_8x
	tStBlkAddrS[43:32] = 0;
	tStBlkAddrT[43:32] = 0;
	tStBlkAddrU[43:32] = 0;
	tStBlkAddrV[43:32] = 0;
`endif

`endif

	if(tBlkAddrH[3:0]!=tArrChk)
		tBlkHitH	= 0;

	if(tBlkAddrH[47:44]!=4'hC)
		tBlkHitH	= 0;

	/* Reject addresses outside normal physical space. */
	if(tL1mAddrIn[47:44]!=4'hC)
		tBlkHitH	= 0;

	if(tL1mAddrIn[43:32]!=0)
		tBlkHitH	= 0;

//		tBlkHitH	= 0;


	tL1mSeqOut  = tL2mSeqIn;
	tL1mOpmOut  = tL2mOpmIn;
//	tL1mAddrOut = tL2mAddrIn;
	tL1mDataOut = tL2mDataIn;

	tL2mSeqOut  = tL1mSeqIn;
	tL2mOpmOut  = tL1mOpmIn;
//	tL2mAddrOut = tL1mAddrIn;
	tL2mDataOut = tL1mDataIn;

`ifdef jx2_bus_mixaddr96
	tL1mAddrOut = { UV48_00, tL2mAddrIn };
	tL2mAddrOut = tL1mAddrIn[47:0];
`else
	tL1mAddrOut = tL2mAddrIn;
	tL2mAddrOut = tL1mAddrIn;
`endif

`ifdef def_true
	if((l1mOpmIn_IsMemLdReq || l1mOpmIn_IsMemStReq) &&
		!tL1mAddrIn[47])
	begin
		$display("L1 Bridge: Leaked Virtual Request O=%X S=%X A=%X",
			tL1mOpmIn, tL1mSeqIn, tL1mAddrIn);

		tL1mSeqOut  = tL1mSeqIn;
		tL1mOpmOut  = tL1mOpmIn;
		tL1mAddrOut = tL1mAddrIn;
		tL1mDataOut = tL1mDataIn;

		tL2mSeqOut  = tL2mSeqIn;
		tL2mOpmOut  = tL2mOpmIn;
		tL2mAddrOut = tL2mAddrIn;
		tL2mDataOut = tL2mDataIn;
	end
`endif

// `ifndef def_true
`ifdef def_true
	if(l2mOpmIn_IsReq || l2mRingIsRespOther)
	begin
		if(l1mRingIsIdle)
		begin
			/* Avoid letting requests back into L1 ring. */
		
			tL1mSeqOut  = tL1mSeqIn;
			tL1mOpmOut  = tL1mOpmIn;
			tL1mAddrOut = tL1mAddrIn;
			tL1mDataOut = tL1mDataIn;


			tL2mSeqOut  = tL2mSeqIn;
			tL2mOpmOut  = tL2mOpmIn;
			tL2mAddrOut = tL2mAddrIn;
			tL2mDataOut = tL2mDataIn;
		end
	end
`endif

// `ifndef def_true
`ifdef def_true
	if(l1mOpmIn_IsMemLdReq && tBlkHitH)
	begin
		tL1mSeqOut  = tL1mSeqIn;
		tL1mOpmOut  = {
			tL1mOpmIn[15:8],
//			JX2_RBI_OPM_OKLD[7:0] };
			JX2_RBI_OPM_OKLD[7:4],
			tL1mOpmIn[11:8]};
		tL1mAddrOut = tL1mAddrIn;
		tL1mDataOut = tBlkDataH;

		tL2mSeqOut  = tL2mSeqIn;
		tL2mOpmOut  = tL2mOpmIn;
		tL2mAddrOut = tL2mAddrIn;
		tL2mDataOut = tL2mDataIn;
	end
`endif

`ifdef def_true
// `ifndef def_true
	if(reset)
	begin
		/* Clear ring during reset */
	
		tL1mSeqOut  = 0;
		tL1mOpmOut  = 0;

		tL2mSeqOut  = 0;
		tL2mOpmOut  = 0;
	end
`endif

end


always @(posedge clock)
begin
	tL1mSeqIn		<= l1mSeqIn;
	tL1mOpmIn		<= l1mOpmIn;
	tL1mAddrIn		<= l1mAddrIn;
	tL1mDataIn		<= l1mDataIn;

	tL2mSeqIn		<= l2mSeqIn;
	tL2mOpmIn		<= l2mOpmIn;
	tL2mAddrIn		<= l2mAddrIn;
	tL2mDataIn		<= l2mDataIn;

	tL1mSeqOut2		<= tL1mSeqOut;
	tL1mOpmOut2		<= tL1mOpmOut;
	tL1mAddrOut2	<= tL1mAddrOut;
	tL1mDataOut2	<= tL1mDataOut;

	tL2mSeqOut2		<= tL2mSeqOut;
	tL2mOpmOut2		<= tL2mOpmOut;
	tL2mAddrOut2	<= tL2mAddrOut;
	tL2mDataOut2	<= tL2mDataOut;
	
	tArrChk			<= tNxtArrChk;
	
	if(tDoBlkSt)
	begin
		tArrDataA[tBlkStIx]		<= tStBlkDataA;
		tArrDataB[tBlkStIx]		<= tStBlkDataB;
		tArrDataC[tBlkStIx]		<= tStBlkDataC;
		tArrDataD[tBlkStIx]		<= tStBlkDataD;

		tArrAddrA[tBlkStIx]		<= tStBlkAddrA;
		tArrAddrB[tBlkStIx]		<= tStBlkAddrB;
		tArrAddrC[tBlkStIx]		<= tStBlkAddrC;
		tArrAddrD[tBlkStIx]		<= tStBlkAddrD;

`ifdef jx2_rbi_bridge_vca_8x
		tArrDataS[tBlkStIx]		<= tStBlkDataS;
		tArrDataT[tBlkStIx]		<= tStBlkDataT;
		tArrDataU[tBlkStIx]		<= tStBlkDataU;
		tArrDataV[tBlkStIx]		<= tStBlkDataV;

		tArrAddrS[tBlkStIx]		<= tStBlkAddrS;
		tArrAddrT[tBlkStIx]		<= tStBlkAddrT;
		tArrAddrU[tBlkStIx]		<= tStBlkAddrU;
		tArrAddrV[tBlkStIx]		<= tStBlkAddrV;
`endif
	end
end


endmodule
