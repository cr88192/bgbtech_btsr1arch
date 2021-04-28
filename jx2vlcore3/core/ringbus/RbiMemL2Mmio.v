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
	mmioOK
	);

input			clock;
input			reset;

input [ 15:0]	memSeqIn;		//operation sequence
output[ 15:0]	memSeqOut;		//operation sequence
input [ 15:0]	memOpmIn;		//memory operation mode
output[ 15:0]	memOpmOut;		//memory operation mode
input [ 47:0]	memAddrIn;		//memory input address
output[ 47:0]	memAddrOut;		//memory output address
input [127:0]	memDataIn;		//memory input data
output[127:0]	memDataOut;		//memory output data

input [  7:0]	unitNodeId;		//Who Are We?

input[63:0]		mmioInData;
output[63:0]	mmioOutData;
output[31:0]	mmioAddr;
output[4:0]		mmioOpm;
input[1:0]		mmioOK;


reg[ 15:0]		tMemSeqOut;		//operation sequence
reg[ 15:0]		tMemOpmOut;		//memory operation mode
reg[ 47:0]		tMemAddrOut;	//memory output address
reg[127:0]		tMemDataOut;	//memory output data

reg[ 15:0]		tMemSeqReq;
reg[ 15:0]		tMemOpmReq;
reg[ 47:0]		tMemAddrReq;
reg[127:0]		tMemDataReq;
reg				tMemReqRsM;
reg				tNxtMemReqRsM;

reg[15:0]		tReqSeq;
reg[15:0]		tReqOpm;
reg[47:0]		tReqAddr;
reg[63:0]		tReqData;
reg				tReqLive;

reg[15:0]		tNxtReqSeq;
reg[15:0]		tNxtReqOpm;
reg[47:0]		tNxtReqAddr;
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

assign		mmioOutData		= tMmioOutData;
assign		mmioAddr		= tMmioAddr;
assign		mmioOpm			= tMmioOpm;

reg[63:0]		tNxtMmioOutData;
reg[31:0]		tNxtMmioAddr;
reg[4:0]		tNxtMmioOpm;

reg[63:0]		tNxtRespData;
reg[63:0]		tRespData;
reg				tNxtRespDone;
reg				tRespDone;


wire		memRingIsIdle;
wire		memRingIsResp;
wire		memRingIsLdx;
wire		memRingIsStx;

wire		memRingIsLdQ;
wire		memRingIsStQ;
wire		memRingIsLdL;
wire		memRingIsStL;

assign	memRingIsIdle	= (memOpmIn[7:0] == JX2_RBI_OPM_IDLE);
assign	memRingIsLdx	= (memOpmIn[7:0] == JX2_RBI_OPM_LDX);
assign	memRingIsStx	= (memOpmIn[7:0] == JX2_RBI_OPM_STX);

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
		if(!tMemReqRsM)
		begin
			tMemSeqReq		= tReqSeq;
			tMemOpmReq		= { tReqOpm[15:8], JX2_RBI_OPM_OKLD };
			tMemAddrReq		= tReqAddr;
			tMemDataReq		= { UV64_00, tRespData };
			tNxtMemReqRsM	= 1;
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

	if(mmioOK == UMEM_OK_OK)
	begin
		tNxtMmioOpm			= UMEM_OPM_READY;
		tNxtRespDone		= 1;
		tNxtRespData		= mmioInData;
	end
	else if(mmioOK == UMEM_OK_READY)
	begin
		if(tReqLive && !tRespDone)
		begin
			tNxtMmioOutData		= tReqData;
			tNxtMmioAddr		= tReqAddr[31:0];
			tNxtMmioOpm			= tReqOpm2;
		end
	end
end

always @(posedge clock)
begin

	tMmioOutData	<= tNxtMmioOutData;
	tMmioAddr		<= tNxtMmioAddr;
	tMmioOpm		<= tNxtMmioOpm;

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
		(memRingIsIdle && tNxtMemReqRsM && !tMemReqRsM))
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
