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

`include "ringbus/RbiDefs.v"

`ifdef jx2_mem_useddrwa
`include "ringbus/RbiMemL2DcWa.v"
`else
`include "ringbus/RbiMemL2Dc.v"
`endif

`include "ringbus/RbiMemL2Rom.v"
`include "ringbus/RbiMemL2Mmio.v"

module RbiMemL2A(
	clock,
	reset,
	
	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId,		timers,

	ddrMemAddr,		ddrMemAddrB,
	ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK,
	ddrOpSqI,		ddrOpSqO,

	mmioAddr,		mmioOpm,
	mmioInData,		mmioOutData,
	mmioOK,			mmioExcIn
	);

input			clock;
input			reset;

input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
`input_l2addr	memAddrIn;		//memory input address
`output_l2addr	memAddrOut;		//memory output address
`input_tile		memDataIn;		//memory input data
`output_tile	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?
input [  7:0]	timers;			//Timer Signals

`input_ddrtile	ddrMemDataIn;
`output_ddrtile	ddrMemDataOut;

output[31:0]	ddrMemAddr;
output[31:0]	ddrMemAddrB;
output[4:0]		ddrMemOpm;
input[1:0]		ddrMemOK;

input[3:0]		ddrOpSqI;
output[3:0]		ddrOpSqO;


input[63:0]		mmioInData;
output[63:0]	mmioOutData;
output[31:0]	mmioAddr;
output[4:0]		mmioOpm;
input[1:0]		mmioOK;
input[63:0]		mmioExcIn;


wire[ 15:0]		l2mSeqIn;
wire[ 15:0]		l2mSeqOut;
wire[ 15:0]		l2mOpmIn;
wire[ 15:0]		l2mOpmOut;
`wire_l2addr	l2mAddrIn;
`wire_l2addr	l2mAddrOut;
`wire_tile		l2mDataIn;
`wire_tile		l2mDataOut;
wire[  7:0]		l2mNodeId;
wire			l2mDeadlockStrobe;

`ifdef jx2_mem_useddrwa
RbiMemL2DcWa		l2dc(
`else
RbiMemL2Dc		l2dc(
`endif
	clock,			reset,
	
	l2mAddrIn,		l2mAddrOut,
	l2mDataIn,		l2mDataOut,
	l2mOpmIn,		l2mOpmOut,
	l2mSeqIn,		l2mSeqOut,
	l2mNodeId,

	ddrMemAddr,		ddrMemAddrB,
	ddrMemOpm,
	ddrMemDataIn,	ddrMemDataOut,
	ddrMemOK,
	ddrOpSqI,		ddrOpSqO,
	
	l2mDeadlockStrobe
	);

wire[ 15:0]		l2rSeqIn;
wire[ 15:0]		l2rSeqOut;
wire[ 15:0]		l2rOpmIn;
wire[ 15:0]		l2rOpmOut;
`wire_l2addr	l2rAddrIn;
`wire_l2addr	l2rAddrOut;
`wire_tile		l2rDataIn;
`wire_tile		l2rDataOut;
wire[  7:0]		l2rNodeId;

RbiMemL2Rom		l2rom(
	clock,			reset,
	l2rAddrIn,		l2rAddrOut,
	l2rDataIn,		l2rDataOut,
	l2rOpmIn,		l2rOpmOut,
	l2rSeqIn,		l2rSeqOut,
	l2rNodeId
	);

wire[ 15:0]		l2bSeqIn;
wire[ 15:0]		l2bSeqOut;
wire[ 15:0]		l2bOpmIn;
wire[ 15:0]		l2bOpmOut;
`wire_l2addr	l2bAddrIn;
`wire_l2addr	l2bAddrOut;
`wire_tile		l2bDataIn;
`wire_tile		l2bDataOut;
wire[  7:0]		l2bNodeId;

RbiMemL2Mmio	l2mmio(
	clock,			reset,
	l2bAddrIn,		l2bAddrOut,
	l2bDataIn,		l2bDataOut,
	l2bOpmIn,		l2bOpmOut,
	l2bSeqIn,		l2bSeqOut,
	l2bNodeId,

	mmioAddr,		mmioOpm,
	mmioInData,		mmioOutData,
	mmioOK,			mmioExcIn
	);

assign			l2mNodeId = 8'h82;
assign			l2rNodeId = 8'h84;
assign			l2bNodeId = 8'h86;
assign			l2mDeadlockStrobe = 0;

reg[ 15:0]		tL2mSeqIn;
reg[ 15:0]		tL2mOpmIn;
`reg_l2addr		tL2mAddrIn;
`reg_tile		tL2mDataIn;

assign		l2mSeqIn	= tL2mSeqIn;
assign		l2mOpmIn	= tL2mOpmIn;
assign		l2mAddrIn	= tL2mAddrIn;
assign		l2mDataIn	= tL2mDataIn;

// assign		l2mSeqIn	= memSeqIn;
// assign		l2mOpmIn	= memOpmIn;
// assign		l2mAddrIn	= memAddrIn;
// assign		l2mDataIn	= memDataIn;

// assign		l2rSeqIn	= l2mSeqOut;
// assign		l2rOpmIn	= l2mOpmOut;
// assign		l2rAddrIn	= l2mAddrOut;
// assign		l2rDataIn	= l2mDataOut;

reg[ 15:0]		tL2mSeqOut;
reg[ 15:0]		tL2mOpmOut;
`reg_l2addr		tL2mAddrOut;
`reg_tile		tL2mDataOut;

assign		l2rSeqIn	= tL2mSeqOut;
assign		l2rOpmIn	= tL2mOpmOut;
assign		l2rAddrIn	= tL2mAddrOut;
assign		l2rDataIn	= tL2mDataOut;

assign		l2bSeqIn	= l2rSeqOut;
assign		l2bOpmIn	= l2rOpmOut;
assign		l2bAddrIn	= l2rAddrOut;
assign		l2bDataIn	= l2rDataOut;

// assign		memSeqOut	= l2bSeqOut;
// assign		memOpmOut	= l2bOpmOut;
// assign		memAddrOut	= l2bAddrOut;
// assign		memDataOut	= l2bDataOut;

reg[ 15:0]		tL2bSeqOut;
reg[ 15:0]		tL2bOpmOut;
`reg_l2addr		tL2bAddrOut;
`reg_tile		tL2bDataOut;

assign		memSeqOut	= tL2bSeqOut;
assign		memOpmOut	= tL2bOpmOut;
assign		memAddrOut	= tL2bAddrOut;
assign		memDataOut	= tL2bDataOut;


always @*
begin
	tL2mSeqIn	= memSeqIn;
	tL2mOpmIn	= memOpmIn;
	tL2mAddrIn	= memAddrIn;
	tL2mDataIn	= memDataIn;
	
	tL2mOpmIn[15] = timers[0];

	if(memOpmIn[11])
		$display("L2A: TLB Missed A O=%X A=%X", memOpmIn, memAddrIn);

	if(memOpmOut[11])
		$display("L2A: TLB Missed B O=%X A=%X", memOpmOut, memAddrOut);

	tL2mSeqOut		= l2mSeqOut;
	tL2mOpmOut		= l2mOpmOut;
	tL2mAddrOut		= l2mAddrOut;
	tL2mDataOut		= l2mDataOut;

	tL2bSeqOut		= l2bSeqOut;
	tL2bOpmOut		= l2bOpmOut;
	tL2bAddrOut		= l2bAddrOut;
	tL2bDataOut		= l2bDataOut;

// `ifndef def_true
`ifdef def_true
	if(	(memOpmIn[7:0] == 8'h00) &&
		((l2mOpmOut[7:0] == JX2_RBI_OPM_LDX) ||
		(l2mOpmOut[7:0] == JX2_RBI_OPM_STX) ||
		(l2mOpmOut[7:0] == JX2_RBI_OPM_PFX) ||
		(l2mOpmOut[7:0] == JX2_RBI_OPM_SPX)) &&
		(l2mAddrOut[29:24] != 6'h00) &&
		(l2mAddrOut[31:30]==2'h0))
	begin
		tL2mSeqIn		= l2mSeqOut;
		tL2mOpmIn		= l2mOpmOut;
		tL2mAddrIn		= l2mAddrOut;
		tL2mDataIn		= l2mDataOut;

		tL2mSeqOut		= memSeqIn;
		tL2mOpmOut		= memOpmIn;
		tL2mAddrOut		= memAddrIn;
		tL2mDataOut		= memDataIn;
	end
`endif

// `ifndef def_true
`ifdef def_true
	if(	(l2bOpmOut[7:0] == 8'h00) &&
//		(l2mOpmOut[7:0] != 8'h00) && 
		(l2mOpmOut[7:6] == 2'b01))
	begin
		/* Shortcut */
	
		tL2mSeqOut		= l2bSeqOut;
		tL2mOpmOut		= l2bOpmOut;
		tL2mAddrOut		= l2bAddrOut;
		tL2mDataOut		= l2bDataOut;

		tL2bSeqOut		= l2mSeqOut;
		tL2bOpmOut		= l2mOpmOut;
		tL2bAddrOut		= l2mAddrOut;
		tL2bDataOut		= l2mDataOut;
	end
`endif

`ifndef def_true
	if(memSeqIn[7:0]!=0)
	begin
		$display("L2A-0: S=%X O=%X A=%X D=%X",
			memSeqIn, memOpmIn, memAddrIn, memDataIn);
	end
`endif

`ifndef def_true
// `ifdef def_true
	if((memOpmOut[7:0]!=0) && (memOpmOut[ 7:6] != 2'b01))
	begin
		$display("L2A-0: S=%X O=%X A=%X D=%X",
			memSeqOut, memOpmOut, memAddrOut, memDataOut);
	end
`endif

end

endmodule
