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
Bridge between Ring-Bus and MMIO Bus.
 */


module RbiMemL2Mmio(
	clock,			reset,
	
	memAddrIn,		memAddrOut,
	memDataIn,		memDataOut,
	memOpmIn,		memOpmOut,
	memSeqIn,		memSeqOut,
	unitNodeId,

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

input[63:0]		mmioInData;
output[63:0]	mmioOutData;
output[31:0]	mmioAddr;
output[4:0]		mmioOpm;
input[1:0]		mmioOK;
input[63:0]		mmioExcIn;


reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
`reg_l2addr		tMemAddrOut;	//memory output address
`reg_tile		tMemDataOut;	//memory output data

reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
`reg_l2addr		tMemAddrReq;
`reg_tile		tMemDataReq;
reg				tMemReqRsM;
reg				tNxtMemReqRsM;

reg[15:0]		tReqSeq;
reg[15:0]		tReqOpm;
`reg_l2addr		tReqAddr;
reg[63:0]		tReqData;
reg				tReqLive;

reg[15:0]		tNxtReqSeq;
reg[15:0]		tNxtReqOpm;
`reg_l2addr		tNxtReqAddr;
reg[63:0]		tNxtReqData;
reg				tNxtReqLive;

reg[4:0]		tReqOpm2;
reg[4:0]		tNxtReqOpm2;

assign		memSeqOut	= tMemSeqOut;
assign		memOpmOut	= tMemOpmOut;
assign		memAddrOut	= tMemAddrOut;
assign		memDataOut	= tMemDataOut;

reg[63:0]		tMmioOutData;
reg[31:0]		tMmioAddr;
reg[4:0]		tMmioOpm;

reg[63:0]		tMmioOutData2;
reg[31:0]		tMmioAddr2;
reg[4:0]		tMmioOpm2;

// assign		mmioOutData		= tMmioOutData;
// assign		mmioAddr		= tMmioAddr;
// assign		mmioOpm			= tMmioOpm;

assign		mmioOutData		= tMmioOutData2;
assign		mmioAddr		= tMmioAddr2;
assign		mmioOpm			= tMmioOpm2;

reg[63:0]		tMmioInData;
reg[1:0]		tMmioOK;

reg[63:0]		tNxtMmioOutData;
reg[31:0]		tNxtMmioAddr;
reg[4:0]		tNxtMmioOpm;

reg[63:0]		tMmioExcIn;
reg[63:0]		tMmioExcInL;


reg[63:0]		tNxtRespData;
reg[63:0]		tRespData;
reg				tNxtRespDone;
reg				tRespDone;


wire		memRingIsIdle;
wire		memRingIsResp;
wire		memRingIsLdx;
wire		memRingIsStx;
wire		memRingIsPfx;

wire		memRingIsLdQ;
wire		memRingIsStQ;
wire		memRingIsLdL;
wire		memRingIsStL;

reg[63:0]		tRingExc;
reg[63:0]		tNxtRingExc;
reg				tRingDoExc;
reg				tNxtRingDoExc;

assign	memRingIsIdle	= (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign	memRingIsLdx	= (memOpmIn[7:0] == JX2_RBI_OPM_LDX);
assign	memRingIsStx	= (memOpmIn[7:0] == JX2_RBI_OPM_STX);
assign	memRingIsPfx	= (memOpmIn[7:0] == JX2_RBI_OPM_PFX);

assign	memRingIsLdQ	= (memOpmIn[7:0] == JX2_RBI_OPM_LDSQ);
assign	memRingIsStQ	= (memOpmIn[7:0] == JX2_RBI_OPM_STSQ);
assign	memRingIsLdL	= (memOpmIn[7:0] == JX2_RBI_OPM_LDSL);
assign	memRingIsStL	= (memOpmIn[7:0] == JX2_RBI_OPM_STSL);

assign	memRingIsResp	=
	(memOpmIn[ 7:6] == 2'b01) &&
	(memSeqIn[15:8] == unitNodeId);


wire		memRingIsMmio;
assign		memRingIsMmio =
	memRingIsLdQ || memRingIsStQ ||
	memRingIsLdL || memRingIsStL ;

always @*
begin
	tMemSeqReq		= 0;
	tMemOpmReq		= 0;
	tMemAddrReq		= 0;
	tMemDataReq		= 0;
	tNxtMemReqRsM	= tMemReqRsM;

	tNxtRingDoExc	= tRingDoExc;
	tNxtRingExc		= tRingExc;

	if(tMmioExcIn[15] && !tRingDoExc)
	begin
		tNxtRingDoExc	= 1;
		tNxtRingExc		= tMmioExcIn;

		if(	(tMmioExcIn[15:12]!=4'hC) &&
			(tMmioExcIn[15:12]!=4'hE))
		begin
			$display("MMIO: Invalid IRQ %X", tMmioExcIn);
			tNxtRingExc[15:12]	= 0;
			tNxtRingDoExc		= 0;
		end
	end

	tNxtReqSeq		= tReqSeq;
	tNxtReqOpm		= tReqOpm;
	tNxtReqAddr		= tReqAddr;
	tNxtReqData		= tReqData;
	tNxtReqLive		= tReqLive;
	tNxtReqOpm2		= tReqOpm2;
	
	if(memRingIsMmio && !tReqLive)
	begin
		tNxtReqSeq		= memSeqIn;
		tNxtReqOpm		= memOpmIn;
		tNxtReqAddr		= memAddrIn;
		tNxtReqData		= memDataIn[63:0];
		tNxtReqLive		= 1;

		tNxtReqOpm2		= UMEM_OPM_READY;
		if(memRingIsLdQ)
			tNxtReqOpm2		= UMEM_OPM_RD_Q;
		if(memRingIsLdL)
			tNxtReqOpm2		= UMEM_OPM_RD_SL;
		if(memRingIsStQ)
			tNxtReqOpm2		= UMEM_OPM_WR_Q;
		if(memRingIsStL)
			tNxtReqOpm2		= UMEM_OPM_WR_SL;
	end

	tNxtMmioOutData		= tMmioOutData;
	tNxtMmioAddr		= tMmioAddr;
	tNxtMmioOpm			= tMmioOpm;

	tNxtRespData		= tRespData;
	tNxtRespDone		= tRespDone;

	if(tRespDone)
	begin
		tNxtMmioAddr	= 0;
		tNxtMmioOpm		= UMEM_OPM_READY;

		if(!tMemReqRsM)
		begin
			tMemSeqReq		= tReqSeq;
			tMemOpmReq		= { tReqOpm[15:8], JX2_RBI_OPM_OKLD };
			tMemAddrReq		= tReqAddr;
			tMemDataReq		= { UV64_00, tRespData };
			tNxtMemReqRsM	= 1;
		end
		else
			if(tMmioOK != UMEM_OK_READY)
		begin
			/* Wait it out. */
		end
		else
		begin
			tNxtMemReqRsM	= 0;
			tNxtRespDone	= 0;
			tNxtReqLive		= 0;
		end
	end
	else
	begin
		tNxtMemReqRsM	= 0;
	end

	if(tMmioOK == UMEM_OK_OK)
	begin
		tNxtMmioOpm			= UMEM_OPM_READY;

		if(!tRespDone)
		begin
			tNxtRespDone		= 1;
			tNxtRespData		= tMmioInData;
		end

	end
	else if(tMmioOK == UMEM_OK_READY)
	begin
		if(tReqLive && !tRespDone)
		begin
			if(tReqAddr[27:16]==12'h001)
			begin
				$display("MMIO: Debug Reject Range O=%X A=%X O-S=%X-%X",
					tReqAddr, tReqOpm2,
					tReqOpm, tReqSeq);
				tNxtRespDone		= 1;
				tNxtRespData		= 0;
			end
			else
			begin
				tNxtMmioOutData		= tReqData;
				tNxtMmioAddr		= tReqAddr[31:0];
				tNxtMmioOpm			= tReqOpm2;
			end
		end
	end

// `ifndef def_true
`ifdef def_true
	if(memRingIsIdle && tRingDoExc && !tNxtMemReqRsM)
	begin
		tMemSeqReq		= 0;
		tMemOpmReq		= { 8'h00, JX2_RBI_OPM_IRQ };
		tMemAddrReq		= 0;
		tMemDataReq		= { UV64_00, tRingExc };
		tNxtRingExc		= 0;
		tNxtRingDoExc	= 0;
	end
`endif

	if(reset)
	begin
		tMemSeqReq			= 0;
		tMemOpmReq			= 0;
		tMemAddrReq			= 0;
		tMemDataReq			= 0;
		tNxtMemReqRsM		= 0;

		tNxtReqSeq			= 0;
		tNxtReqOpm			= 0;
		tNxtReqAddr			= 0;
		tNxtReqData			= 0;
		tNxtReqLive			= 0;
		tNxtReqOpm2			= 0;

		tNxtMmioOutData		= 0;
		tNxtMmioAddr		= 0;
		tNxtMmioOpm			= 0;

		tNxtRespData		= 0;
		tNxtRespDone		= 0;
	end
end

always @(posedge clock)
begin

	tMmioOutData	<= tNxtMmioOutData;
	tMmioAddr		<= tNxtMmioAddr;
	tMmioOpm		<= tNxtMmioOpm;

	tMmioOutData2	<= tMmioOutData;
	tMmioAddr2		<= tMmioAddr;
	tMmioOpm2		<= tMmioOpm;

	tMmioInData		<= mmioInData;
	tMmioOK			<= mmioOK;

	tMmioExcIn		<= mmioExcIn;
	tMmioExcInL		<= tMmioExcIn;

	tRingExc		<= tNxtRingExc;
	tRingDoExc		<= tNxtRingDoExc;

	tRespData		<= tNxtRespData;
	tRespDone		<= tNxtRespDone;

	tReqSeq			<= tNxtReqSeq;
	tReqOpm			<= tNxtReqOpm;
	tReqAddr		<= tNxtReqAddr;
	tReqData		<= tNxtReqData;
	tReqLive		<= tNxtReqLive;
	tReqOpm2		<= tNxtReqOpm2;

//	if(memRingIsIdle)
	if((memRingIsMmio && !tReqLive) ||
		(memRingIsIdle && tNxtMemReqRsM && !tMemReqRsM) ||
		(memRingIsIdle && tRingDoExc && !tNxtMemReqRsM))
	begin
		tMemSeqOut  <= tMemSeqReq;
		tMemOpmOut  <= tMemOpmReq;
		tMemAddrOut <= tMemAddrReq;
		tMemDataOut <= tMemDataReq;
		tMemReqRsM	<= tNxtMemReqRsM;
	end
	else
	begin
		tMemSeqOut  <= memSeqIn;
		tMemOpmOut  <= memOpmIn;
		tMemAddrOut <= memAddrIn;
		tMemDataOut <= memDataIn;
	end
end

endmodule
