/*
Bridge between the L1 and L2 Rings.
 */

`include "RbiDefs.v"

module RbiMemL1Bridge(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regInMmcr,		regInKrr,		regInSr

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
input [ 47:0]	l1mAddrIn;		//memory input address
output[ 47:0]	l1mAddrOut;		//memory output address
input [127:0]	l1mDataIn;		//memory input data
output[127:0]	l1mDataOut;		//memory output data

input [ 15:0]	l2mSeqIn;		//operation sequence
output[ 15:0]	l2mSeqOut;		//operation sequence
input [ 15:0]	l2mOpmIn;		//memory operation mode
output[ 15:0]	l2mOpmOut;		//memory operation mode
input [ 31:0]	l2mAddrIn;		//memory input address
output[ 31:0]	l2mAddrOut;		//memory output address
input [127:0]	l2mDataIn;		//memory input data
output[127:0]	l2mDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?



reg[ 15:0]		tL1mSeqOut;			//operation sequence
reg[ 15:0]		tL1mOpmOut;			//memory operation mode
reg[ 47:0]		tL1mAddrOut;		//memory output address
reg[127:0]		tL1mDataOut;		//memory output data

reg[ 15:0]		tL2mSeqOut;			//operation sequence
reg[ 15:0]		tL2mOpmOut;			//memory operation mode
reg[ 31:0]		tL2mAddrOut;		//memory output address
reg[127:0]		tL2mDataOut;		//memory output data


assign		l1mSeqOut	= tL1mSeqOut;
assign		l1mOpmOut	= tL1mOpmOut;
assign		l1mAddrOut	= tL1mAddrOut;
assign		l1mDataOut	= tL1mDataOut;

assign		l2mSeqOut	= tL2mSeqOut;
assign		l2mOpmOut	= tL2mOpmOut;
assign		l2mAddrOut	= tL2mAddrOut;
assign		l2mDataOut	= tL2mDataOut;

reg		tL1mReqSent;
reg		tL2mReqSent;

wire			l1mRingIsIdle;
wire			l2mRingIsIdle;

assign		l1mRingIsIdle = (l1mOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign		l2mRingIsIdle = (l2mOpmIn[7:0] == JX2_RBI_OPM_IDLE);

wire			l1mRingIsReq;
wire			l2mRingIsResp;

assign		l1mRingIsReq = l1mOpmIn[ 7:6] == 2'b10;

assign		l2mRingIsResp =
	(l2mOpmIn[ 7:6] == 2'b01) &&
	(l2mSeqIn[15:10] == unitNodeId[7:2]);

reg[ 15:0]		tL1mSeqReq;			//operation sequence
reg[ 15:0]		tL1mOpmReq;			//memory operation mode
reg[ 47:0]		tL1mAddrReq;		//memory output address
reg[127:0]		tL1mDataReq;		//memory output data

reg[ 15:0]		tL2mSeqReq;			//operation sequence
reg[ 15:0]		tL2mOpmReq;			//memory operation mode
reg[ 31:0]		tL2mAddrReq;		//memory output address
reg[127:0]		tL2mDataReq;		//memory output data

reg[ 15:0]		tTlbSeqReqIn;			//operation sequence
reg[ 15:0]		tTlbOpmReqIn;			//memory operation mode
reg[ 47:0]		tTlbAddrReqIn;		//memory output address
reg[127:0]		tTlbDataReqIn;		//memory output data

reg[ 15:0]		tTlbSeqReqOut;		//operation sequence
reg[ 15:0]		tTlbOpmReqOut;		//memory operation mode
reg[ 47:0]		tTlbAddrReqOut;		//memory output address
reg[127:0]		tTlbDataReqOut;		//memory output data

wire			tlbRingIsIdle;
assign		tlbRingIsIdle = (tTlbOpmReqOut[7:0] == JX2_RBI_OPM_IDLE);


reg				tHoldL2b;			//Can't transfer L1->L2
reg				tHoldL1b;			//Can't transfer L2->L1

always @*
begin

	tHoldL2b		= 0;
	tHoldL1b		= 0;

	tL1mSeqReq		= UV16_00;
	tL1mOpmReq		= UV16_00;
	tL1mAddrReq		= UV48_00;
	tL1mDataReq		= UV128_XX;

	tL2mSeqReq		= UV16_00;
	tL2mOpmReq		= UV16_00;
	tL2mAddrReq		= UV48_00;
	tL2mDataReq		= UV128_XX;

	if(!l1mRingIsIdle && !l1mRingIsReq)
		tHoldL1b		= 1;

	if(!l2mRingIsIdle && !l2mRingIsResp)
		tHoldL2b		= 1;
		
	if(tlbRingIsIdle)
		tHoldL2b		= 0;

	if(l2mRingIsResp)
	begin
		tL1mSeqReq		= l2mSeqIn;
		tL1mOpmReq		= l2mOpmIn;
		tL1mAddrReq		= { JX2_RBI_ADDRHI_PHYS, l2mAddrIn };
		tL1mDataReq		= l2mDataIn;
	end

	if(l1mRingIsReq)
	begin
		tL2mSeqReq		= l1mSeqIn;
		tL2mOpmReq		= l1mOpmIn;
		tL2mAddrReq		= l1mAddrIn[31:0];
		tL2mDataReq		= l1mDataIn;
	end

end

always @(posedge clock)
begin
	if(l1mRingIsIdle || (l1mRingIsReq && !tHoldL2b))
	begin
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
		tL1ReqSent	<= 0;
	end

	if(l2mRingIsIdle || (l2mRingIsResp && !tHoldL1b))
	begin
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
		tL2ReqSent	<= 0;
	end
end

endmodule