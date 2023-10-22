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

Simplified RAM Bridge to Ringbus

Goal is to allow 64 or 128-bit aligned Load/Store ops with a "Single Tile" cache.

 */

`include "ringbus/RbiDefs.v"

`ifndef HAS_RbiMemEwDs
`define HAS_RbiMemEwDs

module RbiMemEwDs(
	/* verilator lint_off UNUSED */
	clock,			reset,

	edsDataOutLo,	edsDataOutHi,
	edsDataInLo,	edsDataInHi,
	edsAddr,		edsOpm,
	edsOK,

	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId
	);

input			clock;
input			reset;

input[63:0]		edsDataInLo;
input[63:0]		edsDataInHi;
output[63:0]	edsDataOutLo;
output[63:0]	edsDataOutHi;
input[31:0]		edsAddr;
input[4:0]		edsOpm;
output[1:0]		edsOK;


input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l2addr	memAddrIn;		//memory input address
`output_l2addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?


reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
`reg_l2addr	tMemAddrOut;		//memory output address
`reg_tile		tMemDataOut;		//memory output data

assign		memSeqOut = tMemSeqOut;
assign		memOpmOut = tMemOpmOut;
assign		memAddrOut = tMemAddrOut;
assign		memDataOut = tMemDataOut;

wire			memRingIsIdle;
wire			memRingIsResp;
assign		memRingIsIdle = (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign		memRingIsResp =
	(memOpmIn[ 7:6] == 2'b01) &&
	(memSeqIn[15:8] == unitNodeId);

wire			memRingIsRespOkLd;
wire			memRingIsRespOkSt;
wire			memRingIsRespOkLdA;
wire			memRingIsRespOkLdB;
wire			memRingIsRespOkLdC;
wire			memRingIsRespOkLdD;
wire			memRingIsRespOkStA;
wire			memRingIsRespOkStB;

assign		memRingIsRespOkLd =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b11);
assign		memRingIsRespOkSt =
	memRingIsResp &&
	(memOpmIn[ 5:4] == 2'b10);

assign		memRingIsRespOkLdA =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b00);
assign		memRingIsRespOkLdB =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b01);

assign		memRingIsRespOkLdC =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b10);
assign		memRingIsRespOkLdD =
	memRingIsRespOkLd &&
	(memSeqIn[7:6] == 2'b11);

assign		memRingIsRespOkStA =
	memRingIsRespOkSt &&
	(memSeqIn[7:6] == 2'b00);
assign		memRingIsRespOkStB =
	memRingIsRespOkSt &&
	(memSeqIn[7:6] == 2'b01);

reg		tMemRingSkipResp;
reg		tMemReqSent;

reg[63:0]		tEdsDataOutLo2;
reg[63:0]		tEdsDataOutHi2;
reg[1:0]		tEdsOK2;

assign	edsDataOutLo	= tEdsDataOutLo2;
assign	edsDataOutHi	= tEdsDataOutHi2;
assign	edsOK			= tEdsOK2;

reg[63:0]		tEdsDataOutLo;
reg[63:0]		tEdsDataOutHi;
reg[1:0]		tEdsOK;


reg[63:0]		tEdsDataInLo;
reg[63:0]		tEdsDataInHi;
reg[31:0]		tEdsAddr;
reg[4:0]		tEdsOpm;

reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
`reg_l2addr	tMemAddrReq;
`reg_tile		tMemDataReq;

reg[ 15:0]		tMemSeqReqL;
reg[ 15:0]		tMemOpmReqL;
`reg_l2addr	tMemAddrReqL;
`reg_tile		tMemDataReqL;

reg				tMemReqSt;
reg				tMemReqLd;
reg				tNxtMemReqSt;
reg				tNxtMemReqLd;

reg				tMemRespSt;
reg				tMemRespLd;
reg				tNxtMemRespSt;
reg				tNxtMemRespLd;

reg[ 5:0]		tMemSeqRov;
reg[ 5:0]		tNxtMemSeqRov;


reg[23:0]		tReqAx;

reg[127:0]		tMemBlkData;
reg[ 23:0]		tMemBlkAddr;
reg				tMemBlkDirty;

reg[127:0]		tNxtMemBlkData;
reg[ 23:0]		tNxtMemBlkAddr;
reg				tNxtMemBlkDirty;

reg[ 15:0]		tRetryCyc;
reg[ 15:0]		tNxtRetryCyc;

reg				tReqIsNz;
reg				tReqMissAddr;
reg				tReqDoMiss;

reg				tReqHold;

always @*
begin
	tMemSeqReq			= 0;
	tMemOpmReq			= 0;
	tMemAddrReq			= 0;
	tMemDataReq			= 0;

	tEdsDataOutLo		= 0;
	tEdsDataOutHi		= 0;
	tEdsOK				= 0;

	tNxtMemSeqRov		= tMemSeqRov;
	tMemRingSkipResp	= 0;
	tReqHold			= 0;
	tNxtRetryCyc		= tRetryCyc - 1;
//	tNxtRetryCyc		= 8192;

	tNxtMemRespSt		= tMemRespSt;
	tNxtMemRespLd		= tMemRespLd;

	tNxtMemBlkData		= tMemBlkData;
	tNxtMemBlkAddr		= tMemBlkAddr;
	tNxtMemBlkDirty		= tMemBlkDirty;

	tReqIsNz			= tEdsOpm[4:3] != 0;
	tReqAx				= tEdsAddr[27:4];
	
	tReqMissAddr		= tReqAx != tNxtMemBlkAddr;

	tReqDoMiss			= tReqMissAddr && tReqIsNz;

	if(tReqMissAddr)
		tReqHold = 1;

`ifndef def_true
	if(tEdsOpm[2:0]==3'b111)
	begin
		tEdsDataOutLo		= tMemBlkData[ 63: 0];
		tEdsDataOutHi		= tMemBlkData[127:64];
	end
	else
		if(tEdsOpm[2:0]==3'b011)
	begin
		if(tEdsAddr[3])
			tEdsDataOutLo = tMemBlkData[127:64];
		else
			tEdsDataOutLo = tMemBlkData[ 63: 0];
		tEdsDataOutHi	= 0;
	end
	else
	begin
		tEdsDataOutLo	= 0;

`ifndef def_true
		case(tEdsAddr[3:2])
			2'b00: tEdsDataOutLo = { 32'h00, tMemBlkData[ 31: 0]};
			2'b01: tEdsDataOutLo = { 32'h00, tMemBlkData[ 63:32]};
			2'b10: tEdsDataOutLo = { 32'h00, tMemBlkData[ 95:64]};
			2'b11: tEdsDataOutLo = { 32'h00, tMemBlkData[127:96]};
		endcase
`endif

		tEdsDataOutHi	= 0;
	end
`endif

`ifdef def_true
	tEdsDataOutLo		= tMemBlkData[ 63: 0];
	tEdsDataOutHi		= tMemBlkData[127:64];

	if(tEdsOpm[2:0]!=3'b111)
	begin
		if(tEdsAddr[3])
			tEdsDataOutLo = tEdsDataOutHi;
		tEdsDataOutHi	= 0;
	end

	if(tEdsOpm[2:0]==3'b010)
	begin
		if(tEdsAddr[2])
			tEdsDataOutLo[31:0] = tEdsDataOutLo[63:32];
		tEdsDataOutLo[63:32] = 0;
	end
`endif
	
	if(tEdsOpm[3])
	begin
//		$display("EWDS: Load A=%X D=%X BLK=%X",
//			tEdsAddr, tEdsDataOutLo, tMemBlkData);
	end
	
	if(tEdsOpm[4] && !tReqMissAddr)
	begin
		if(tEdsOpm[2:0]==3'b111)
		begin
			tNxtMemBlkData = { tEdsDataInHi, tEdsDataInLo };
		end
		else if(tEdsOpm[2:0]==3'b011)
		begin
			if(tEdsAddr[3])
				tNxtMemBlkData[127:64] = tEdsDataInLo;
			else
				tNxtMemBlkData[ 63: 0] = tEdsDataInLo;
		end
		else
		begin
`ifndef def_true
			if(tEdsAddr[3:2]==2'b00)
				tNxtMemBlkData[ 31: 0] = tEdsDataInLo[31:0];
			if(tEdsAddr[3:2]==2'b01)
				tNxtMemBlkData[ 63:32] = tEdsDataInLo[31:0];
			if(tEdsAddr[3:2]==2'b10)
				tNxtMemBlkData[ 95:64] = tEdsDataInLo[31:0];
			if(tEdsAddr[3:2]==2'b11)
				tNxtMemBlkData[127:96] = tEdsDataInLo[31:0];\
`endif
		end
		tNxtMemBlkDirty		= 1;
	end

	if(memRingIsRespOkLdA)
	begin
//		$display("EWDS: Ld Resp A=%X D=%X",
//			memAddrIn, memDataIn);
		tNxtMemBlkData		= memDataIn;
		tNxtMemBlkAddr		= memAddrIn[27:4];
		tNxtMemBlkDirty		= 0;
		tNxtMemRespLd		= 1;
	end

	if(memRingIsRespOkStA)
	begin
//		tNxtMemBlkDirty		= 0;
		tNxtMemRespSt		= 1;
	end


	if(tReqMissAddr)
	begin
		tNxtMemReqSt	= tMemReqSt;
		tNxtMemReqLd	= tMemReqLd;
	end
	else
	begin
		tNxtMemReqSt	= 0;
		tNxtMemReqLd	= 0;
	end

	if(tMemReqSt && !tMemRespSt)
		tReqHold = 1;
	if(tMemReqLd && !tMemRespLd)
		tReqHold = 1;

	if(!tReqIsNz)
		tReqHold = 0;

	if(tReqDoMiss && tMemBlkDirty && !tMemReqSt)
	begin
//		$display("EWDS: Send STA Req, A=%X", tNxtMemBlkAddr[27:4]);
		tNxtMemSeqRov	= tMemSeqRov + 1;
		tMemSeqReq		= { unitNodeId, 2'b00, tMemSeqRov };
		tMemDataReq		= tMemBlkData;
		tMemOpmReq		= { UV8_00, JX2_RBI_OPM_STX };
//		tMemAddrReq		= { 20'hC0002, tNxtMemBlkAddr, 4'h0 };
		tMemAddrReq		= { 20'hC0000, tNxtMemBlkAddr, 4'h0 };
		tNxtMemReqSt	= 1;
//		tNxtMemReqIxStA	= tReqIxA;

	end
	else
		if(tReqDoMiss && !tMemReqLd)
	begin
//		$display("EWDS: Send LDA Req A=%X", tReqAx);
		tNxtMemSeqRov	= tMemSeqRov + 1;
		tMemSeqReq		= { unitNodeId, 2'b00, tMemSeqRov };
		tMemOpmReq		= { UV8_00, JX2_RBI_OPM_LDX };
//		tMemAddrReq		= { 20'hC0002, tReqAx, 4'h0 };
		tMemAddrReq		= { 20'hC0000, tReqAx, 4'h0 };
		tNxtMemReqLd	= 1;
	end
	
	if(tRetryCyc[15:8]==0)
	begin
		if(tMemReqSt && !tMemRespSt)
		begin
			tNxtMemReqSt	= 0;
		end
		else if(tMemReqLd && !tMemRespLd)
		begin
			tNxtMemReqLd	= 0;
		end
		else
		begin
			tNxtRetryCyc	= 8192;
		end
	end
	
	if(!tReqHold)
	begin
		tNxtMemReqSt	= 0;
		tNxtMemReqLd	= 0;
		tNxtMemRespSt	= 0;
		tNxtMemRespLd	= 0;
		tNxtRetryCyc	= 8192;
	end

	if(tReqIsNz)
	begin
		tEdsOK			= tReqHold ? UMEM_OK_HOLD : UMEM_OK_OK;
	end
end

always @(posedge clock)
begin
	tEdsDataInLo	<= edsDataInLo;
	tEdsDataInHi	<= edsDataInHi;
	tEdsAddr		<= edsAddr;
	tEdsOpm			<= edsOpm;

	tEdsDataOutLo2	<= tEdsDataOutLo;
	tEdsDataOutHi2	<= tEdsDataOutHi;
	tEdsOK2			<= tEdsOK;

	tMemBlkData		<= tNxtMemBlkData;
	tMemBlkAddr		<= tNxtMemBlkAddr;
	tMemBlkDirty	<= tNxtMemBlkDirty;

	tMemSeqRov		<= tNxtMemSeqRov;

	tMemRespSt		<= tNxtMemRespSt;
	tMemRespLd		<= tNxtMemRespLd;
	tRetryCyc		<= tNxtRetryCyc;

	if(reset)
	begin
		tMemSeqOut  <= 0;
		tMemOpmOut  <= 0;
		tMemAddrOut <= 0;
		tMemDataOut <= 0;

		tMemReqSent	<= 0;
	end
	else
		if((memRingIsIdle || memRingIsResp) && !tMemRingSkipResp)
	begin
		tMemSeqOut  <= tMemSeqReq;
		tMemOpmOut  <= tMemOpmReq;
		tMemAddrOut <= tMemAddrReq;
		tMemDataOut <= tMemDataReq;
		tMemReqSent	<= 1;

		tMemReqSt		<= tNxtMemReqSt;
		tMemReqLd		<= tNxtMemReqLd;
	end
	else
	begin
		/* Forward whatever is on the bus */
		tMemSeqOut  <= memSeqIn;
		tMemOpmOut  <= memOpmIn;
		tMemAddrOut <= memAddrIn;
		tMemDataOut <= memDataIn;
		tMemReqSent	<= 0;

		tMemReqSt		<= tMemReqSt;
		tMemReqLd		<= tMemReqLd;
	end

end


endmodule

`endif
