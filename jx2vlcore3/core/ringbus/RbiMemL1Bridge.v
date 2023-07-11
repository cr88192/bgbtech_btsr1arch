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
 */

`include "ringbus/RbiDefs.v"

module RbiMemL1Bridge(
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

	unitNodeId
	);

input			clock;
input			reset;
input[63:0]		regInMmcr;
input[63:0]		regInKrr;
input[63:0]		regInSr;

input [ 15:0]	l1mSeqIn;		//operation sequence
output[ 15:0]	l1mSeqOut;		//operation sequence
input [ 15:0]	l1mOpmIn;		//memory operation mode
output[ 15:0]	l1mOpmOut;		//memory operation mode
`input_l1addr	l1mAddrIn;		//memory input address
`output_l1addr	l1mAddrOut;		//memory output address
`input_tile		l1mDataIn;		//memory input data
`output_tile	l1mDataOut;		//memory output data

input [ 15:0]	l2mSeqIn;		//operation sequence
output[ 15:0]	l2mSeqOut;		//operation sequence
input [ 15:0]	l2mOpmIn;		//memory operation mode
output[ 15:0]	l2mOpmOut;		//memory operation mode
`input_l2addr	l2mAddrIn;		//memory input address
`output_l2addr	l2mAddrOut;		//memory output address
`input_tile		l2mDataIn;		//memory input data
`output_tile	l2mDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?



reg[ 15:0]		tL1mSeqOut;			//operation sequence
reg[ 15:0]		tL1mOpmOut;			//memory operation mode
`reg_l1addr		tL1mAddrOut;		//memory output address
`reg_tile		tL1mDataOut;		//memory output data

reg[ 15:0]		tL2mSeqOut;			//operation sequence
reg[ 15:0]		tL2mOpmOut;			//memory operation mode
`reg_l2addr		tL2mAddrOut;		//memory output address
`reg_tile		tL2mDataOut;		//memory output data

`ifdef jx2_rbi_bridge_ecyc_l1

reg[ 15:0]		tL1mSeqOut2;			//operation sequence
reg[ 15:0]		tL1mOpmOut2;			//memory operation mode
`reg_l1addr		tL1mAddrOut2;		//memory output address
`reg_tile		tL1mDataOut2;		//memory output data

assign		l1mSeqOut	= tL1mSeqOut2;
assign		l1mOpmOut	= tL1mOpmOut2;
assign		l1mAddrOut	= tL1mAddrOut2;
assign		l1mDataOut	= tL1mDataOut2;

`else

assign		l1mSeqOut	= tL1mSeqOut;
assign		l1mOpmOut	= tL1mOpmOut;
assign		l1mAddrOut	= tL1mAddrOut;
assign		l1mDataOut	= tL1mDataOut;

`endif

`ifdef jx2_rbi_bridge_ecyc_l2

reg[ 15:0]		tL2mSeqOut2;			//operation sequence
reg[ 15:0]		tL2mOpmOut2;			//memory operation mode
`reg_l2addr		tL2mAddrOut2;		//memory output address
`reg_tile		tL2mDataOut2;		//memory output data

assign		l2mSeqOut	= tL2mSeqOut2;
assign		l2mOpmOut	= tL2mOpmOut2;
assign		l2mAddrOut	= tL2mAddrOut2;
assign		l2mDataOut	= tL2mDataOut2;

`else

assign		l2mSeqOut	= tL2mSeqOut;
assign		l2mOpmOut	= tL2mOpmOut;
assign		l2mAddrOut	= tL2mAddrOut;
assign		l2mDataOut	= tL2mDataOut;

`endif

reg		tL1mReqSent;
reg		tL2mReqSent;

wire			l1mRingIsIdle;
wire			l2mRingIsIdle;

assign		l1mRingIsIdle = (l1mOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign		l2mRingIsIdle = (l2mOpmIn[7:0] == JX2_RBI_OPM_IDLE);

wire			l1mRingIsReq;
wire			l2mRingIsResp;
wire			l2mRingIsRespOther;

wire			l1mRingIsIrq;
wire			l2mRingIsIrq;
wire			l2mRingIsIrqBc;

assign		l1mRingIsReq = l1mOpmIn[ 7:6] == 2'b10;

assign		l2mRingIsResp =
	(l2mOpmIn[ 7:6] == 2'b01) &&
	(l2mSeqIn[15:10] == unitNodeId[7:2]);

assign		l2mRingIsRespOther =
	(l2mOpmIn[ 7:6] == 2'b01) &&
	(l2mSeqIn[15:10] != unitNodeId[7:2]);

assign		l1mRingIsIrq =
	(l2mOpmIn[ 7:0] == JX2_RBI_OPM_IRQ) &&
	((l2mDataIn[11:8] != unitNodeId[5:2]) ||
//	 (l2mDataIn[11:8] == 4'h0) ||
	 (l2mDataIn[11:8] == 4'hF));

assign		l2mRingIsIrq =
	(l2mOpmIn[ 7:0] == JX2_RBI_OPM_IRQ) &&
	((l2mDataIn[11:8] == unitNodeId[5:2]) ||
	 (l2mDataIn[11:8] == 4'h0) ||
	 (l2mDataIn[11:8] == 4'hF));
assign		l2mRingIsIrqBc = l2mRingIsIrq && (l2mDataIn[11:8] == 4'hF);

reg[ 15:0]		tL1mSeqReq;			//operation sequence
reg[ 15:0]		tL1mOpmReq;			//memory operation mode
`reg_l1addr		tL1mAddrReq;		//memory output address
reg[127:0]		tL1mDataReq;		//memory output data

reg[ 15:0]		tL2mSeqReq;			//operation sequence
reg[ 15:0]		tL2mOpmReq;			//memory operation mode
`reg_l2addr		tL2mAddrReq;		//memory output address
reg[127:0]		tL2mDataReq;		//memory output data

`ifndef def_true

reg[ 15:0]		tTlbSeqReqIn;			//operation sequence
reg[ 15:0]		tTlbOpmReqIn;			//memory operation mode
`reg_l1addr		tTlbAddrReqIn;		//memory output address
reg[127:0]		tTlbDataReqIn;		//memory output data

reg[ 15:0]		tTlbSeqReqOut;		//operation sequence
reg[ 15:0]		tTlbOpmReqOut;		//memory operation mode
`reg_l1addr		tTlbAddrReqOut;		//memory output address
reg[127:0]		tTlbDataReqOut;		//memory output data

wire			tlbRingIsIdle;
assign		tlbRingIsIdle = (tTlbOpmReqOut[7:0] == JX2_RBI_OPM_IDLE);

`endif


wire			l2mOpmIn_IsReq =
	(l2mOpmIn[7:0]==JX2_RBI_OPM_PFX)	||
	(l2mOpmIn[7:0]==JX2_RBI_OPM_SPX)	||
	(l2mOpmIn[7:0]==JX2_RBI_OPM_LDX)	||
	(l2mOpmIn[7:0]==JX2_RBI_OPM_LDSQ)	||
	(l2mOpmIn[7:0]==JX2_RBI_OPM_LDSL)	||
	(l2mOpmIn[7:0]==JX2_RBI_OPM_LDUL)	||
	(l2mOpmIn[7:0]==JX2_RBI_OPM_STX)	||
	(l2mOpmIn[7:0]==JX2_RBI_OPM_STSQ)	||
	(l2mOpmIn[7:0]==JX2_RBI_OPM_STSL)	;

reg				tHoldL2b;			//Can't transfer L1->L2
reg				tHoldL1b;			//Can't transfer L2->L1

reg				tUpdateL2b;			//Modify Value on L2 Ring

always @*
begin

`ifdef jx2_rbi_nobridge
		tL1mSeqOut  = l2mSeqIn;
		tL1mOpmOut  = l2mOpmIn;
//		tL1mAddrOut = l2mAddrIn;
		tL1mDataOut = l2mDataIn;

		tL2mSeqOut  = l1mSeqIn;
		tL2mOpmOut  = l1mOpmIn;
//		tL2mAddrOut = l1mAddrIn;
		tL2mDataOut = l1mDataIn;

`ifdef jx2_bus_mixaddr96
		tL1mAddrOut = { UV48_00, l2mAddrIn };
		tL2mAddrOut = l1mAddrIn[47:0];
`else
		tL1mAddrOut = l2mAddrIn;
		tL2mAddrOut = l1mAddrIn;
`endif

// `ifndef def_true
`ifdef def_true
	if(l2mOpmIn_IsReq || l2mRingIsRespOther)
	begin
		if(l1mRingIsIdle)
		begin
			/* Avoid letting requests back into L1 ring. */
		
			tL1mSeqOut  = l1mSeqIn;
			tL1mOpmOut  = l1mOpmIn;
			tL1mAddrOut = l1mAddrIn;
			tL1mDataOut = l1mDataIn;

			tL2mSeqOut  = l2mSeqIn;
			tL2mOpmOut  = l2mOpmIn;
			tL2mAddrOut = l2mAddrIn;
			tL2mDataOut = l2mDataIn;
		end
	end
`endif

`ifdef def_true
// `ifndef def_true
	if(reset)
	begin
		/* Clear ring during reset */
	
		tL1mSeqOut  = 0;
		tL1mOpmOut  = 0;
//		tL1mAddrOut = 0;
//		tL1mDataOut = 0;

		tL2mSeqOut  = 0;
		tL2mOpmOut  = 0;
//		tL2mAddrOut = 0;
//		tL2mDataOut = 0;
	end
`endif

`endif


`ifndef jx2_rbi_nobridge

	tHoldL2b		= 0;
	tHoldL1b		= 0;
	tUpdateL2b		= 0;

	tL1mSeqReq		= UV16_00;
	tL1mOpmReq		= UV16_00;
	tL1mAddrReq		= UVB1AT_00;
	tL1mDataReq		= UV128_XX;

	tL2mSeqReq		= UV16_00;
	tL2mOpmReq		= UV16_00;
	tL2mAddrReq		= UVB2AT_00;
	tL2mDataReq		= UV128_XX;

	if(!l1mRingIsIdle && !l1mRingIsReq)
		tHoldL1b		= 1;

	if(!l2mRingIsIdle && !l2mRingIsResp)
		tHoldL2b		= 1;
		
//	if(tlbRingIsIdle)
//		tHoldL2b		= 0;

	if(l2mRingIsIrqBc && (l2mSeqIn[15:10] != unitNodeId[7:2]) && !reset)
	begin
		tL2mSeqReq		= l2mSeqIn;
		tL2mOpmReq		= l2mOpmIn;
		tL2mAddrReq		= l2mAddrIn;
		tL2mDataReq		= l2mDataIn;
		tUpdateL2b		= 1;

		if(l2mSeqIn[15:10] == 6'h00)
			tL2mSeqReq[15:10]	= unitNodeId[7:2];
	end

	if(tUpdateL2b)
	begin
//		tHoldL1b = 1;
		tHoldL2b = 1;
	end

	if(l2mRingIsResp && !tUpdateL2b && !reset)
	begin
		tL1mSeqReq		= l2mSeqIn;
		tL1mOpmReq		= l2mOpmIn;
//		tL1mAddrReq		= { JX2_RBI_ADDRHI_PHYS, l2mAddrIn };
`ifdef jx2_bus_mixaddr96
		tL1mAddrReq		= { UV48_00, l2mAddrIn };
`else
		tL1mAddrReq		= l2mAddrIn;
`endif
		tL1mDataReq		= l2mDataIn;
	end

	if(l1mRingIsReq && !tUpdateL2b && !reset)
	begin
		tL2mSeqReq		= l1mSeqIn;
		tL2mOpmReq		= l1mOpmIn;
//		tL2mAddrReq		= l1mAddrIn[31:0];
`ifdef jx2_bus_mixaddr96
		tL2mAddrReq		= l1mAddrIn[47:0];
`else
		tL2mAddrReq		= l1mAddrIn;
`endif
		tL2mDataReq		= l1mDataIn;
	end

`endif

end



always @(posedge clock)
begin
`ifdef jx2_rbi_bridge_ecyc_l1
	tL1mSeqOut2		<= tL1mSeqOut;
	tL1mOpmOut2		<= tL1mOpmOut;
	tL1mAddrOut2	<= tL1mAddrOut;
	tL1mDataOut2	<= tL1mDataOut;
`endif

`ifdef jx2_rbi_bridge_ecyc_l2
	tL2mSeqOut2		<= tL2mSeqOut;
	tL2mOpmOut2		<= tL2mOpmOut;
	tL2mAddrOut2	<= tL2mAddrOut;
	tL2mDataOut2	<= tL2mDataOut;
`endif
end


`ifndef jx2_rbi_nobridge

always @(posedge clock)
begin
	if(l1mRingIsIdle || ((l1mRingIsReq || l1mRingIsIrq) && !tHoldL2b) || reset)
	begin
`ifndef def_true
		if(tL1mOpmReq!=0)
		begin
			$display("L2->L1 O=%X S=%X A=%X D=%X",
				tL1mOpmReq,
				tL1mSeqReq,
				tL1mAddrReq,
				tL1mDataReq);
		end
`endif

		tL1mSeqOut  <= tL1mSeqReq;
		tL1mOpmOut  <= tL1mOpmReq;
		tL1mAddrOut <= tL1mAddrReq;
		tL1mDataOut <= tL1mDataReq;
		tL1mReqSent	<= 1;
	end
	else
	begin
		/* Forward whatever is on the bus */
		tL1mSeqOut  <= l1mSeqIn;
		tL1mOpmOut  <= l1mOpmIn;
		tL1mAddrOut <= l1mAddrIn;
		tL1mDataOut <= l1mDataIn;
		tL1mReqSent	<= 0;
	end

	if(l2mRingIsIdle || tUpdateL2b ||
		((l2mRingIsResp || l2mRingIsIrq) && !tHoldL1b) || reset)
	begin
`ifndef def_true
		if(tL2mOpmReq!=0)
		begin
			$display("L1->L2 O=%X S=%X A=%X D=%X",
				tL2mOpmReq,
				tL2mSeqReq,
				tL2mAddrReq,
				tL2mDataReq);
		end
`endif

		tL2mSeqOut  <= tL2mSeqReq;
		tL2mOpmOut  <= tL2mOpmReq;
		tL2mAddrOut <= tL2mAddrReq;
		tL2mDataOut <= tL2mDataReq;
		tL2mReqSent	<= 1;
	end
	else
	begin
		/* Forward whatever is on the bus */
		tL2mSeqOut  <= l2mSeqIn;
		tL2mOpmOut  <= l2mOpmIn;
		tL2mAddrOut <= l2mAddrIn;
		tL2mDataOut <= l2mDataIn;
		tL2mReqSent	<= 0;
	end
end

`endif

endmodule