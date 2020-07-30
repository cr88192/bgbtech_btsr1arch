module ExMemJoin(
	clock,		reset,

	memInData,	memOutData,
	memAddrA,	memAddrB,
	memOpm,		memOK,
	memBusExc,

	mem1InData,	mem1OutData,
	mem1AddrA,	mem1AddrB,
	mem1Opm,	mem1OK,
	mem1BusExc,

	mem2InData,	mem2OutData,
	mem2AddrA,	mem2AddrB,
	mem2Opm,	mem2OK,
	mem2BusExc

	);

input			clock;
input			reset;

input[127:0]	memInData;
output[127:0]	memOutData;
output[47:0]	memAddrA;
output[47:0]	memAddrB;
output[4:0]		memOpm;
input[1:0]		memOK;
input[63:0]		memBusExc;

output[127:0]	mem1InData;
input[127:0]	mem1OutData;
input[47:0]		mem1AddrA;
input[47:0]		mem1AddrB;
input[4:0]		mem1Opm;
output[1:0]		mem1OK;
output[63:0]	mem1BusExc;

output[127:0]	mem2InData;
input[127:0]	mem2OutData;
input[47:0]		mem2AddrA;
input[47:0]		mem2AddrB;
input[4:0]		mem2Opm;
output[1:0]		mem2OK;
output[63:0]	mem2BusExc;


reg[127:0]		tMemInData;
reg[1:0]		tMemOK;
reg[63:0]		tMemBusExc;

reg[127:0]		tMemOutData;
reg[47:0]		tMemAddrA;
reg[47:0]		tMemAddrB;
reg[4:0]		tMemOpm;

reg[127:0]		tMemOutData2;
reg[47:0]		tMemAddrA2;
reg[47:0]		tMemAddrB2;
reg[4:0]		tMemOpm2;

assign		memOutData	= tMemOutData2;
assign		memAddrA	= tMemAddrA2;
assign		memAddrB	= tMemAddrB2;
assign		memOpm		= tMemOpm2;

reg[127:0]		tMem1InData;
reg[1:0]		tMem1OK;
reg[63:0]		tMem1BusExc;
reg[127:0]		tMem2InData;
reg[1:0]		tMem2OK;
reg[63:0]		tMem2BusExc;

reg[127:0]		tMem1InData2;
reg[1:0]		tMem1OK2;
reg[63:0]		tMem1BusExc2;
reg[127:0]		tMem2InData2;
reg[1:0]		tMem2OK2;
reg[63:0]		tMem2BusExc2;

assign		mem1InData	= tMem1InData2;
assign		mem1OK		= tMem1OK2;
assign		mem1BusExc	= tMem1BusExc2;
assign		mem2InData	= tMem2InData2;
assign		mem2OK		= tMem2OK2;
assign		mem2BusExc	= tMem2BusExc2;

reg		tMem1Latch;
reg		tMem2Latch;
reg		tNextMem1Latch;
reg		tNextMem2Latch;

reg		tMemStrobe;
reg		tNextMemStrobe;

always @*
begin
	tNextMem1Latch = tMem1Latch;
	tNextMem2Latch = tMem2Latch;
	tNextMemStrobe = !tMemStrobe;
	
	tMem1OK			= (mem1Opm != UMEM_OPM_READY) ?
		UMEM_OK_HOLD : UMEM_OK_READY;
	tMem2OK			= (mem2Opm != UMEM_OPM_READY) ?
		UMEM_OK_HOLD : UMEM_OK_READY;
	tMem1InData		= tMemInData;
	tMem2InData		= tMemInData;
	tMem1BusExc		= UV64_00;
	tMem2BusExc		= UV64_00;
	
//	if(tMem1Latch || ((mem1Opm != UMEM_OPM_READY) && !tMem2Latch))

	if(tMem1Latch)
	begin
		tNextMem1Latch	=
			(mem1Opm != UMEM_OPM_READY) ||
			(tMemOK != UMEM_OK_READY);
		tMem1OK			= tMemOK;
		tMem1BusExc		= tMemBusExc;

		tMemOutData		= mem1OutData;
		tMemAddrA		= mem1AddrA;
		tMemAddrB		= mem1AddrB;
		tMemOpm			= mem1Opm;
	end
	else
		if(tMem2Latch)
	begin
		tNextMem2Latch	=
			(mem2Opm != UMEM_OPM_READY) ||
			(tMemOK != UMEM_OK_READY);
		tMem2OK			= tMemOK;
		tMem2BusExc		= tMemBusExc;

		tMemOutData		= mem2OutData;
		tMemAddrA		= mem2AddrA;
		tMemAddrB		= mem2AddrB;
		tMemOpm			= mem2Opm;
	end
	else
		if((mem1Opm != UMEM_OPM_READY) &&
			!((mem2Opm != UMEM_OPM_READY) && tMemStrobe))
	begin
		tNextMem1Latch	= 1;
	end
	else
		if(mem2Opm != UMEM_OPM_READY)
	begin
		tNextMem2Latch	= 1;
	end

end

always @(posedge clock)
begin

	tMemInData		<= memInData;
	tMemOK			<= memOK;
	tMemBusExc		<= memBusExc;

	tMemOutData2	<= tMemOutData;
	tMemAddrA2		<= tMemAddrA;
	tMemAddrB2		<= tMemAddrB;
	tMemOpm2		<= tMemOpm;

	tMem1InData2	<= tMem1InData;
	tMem1OK2		<= tMem1OK;
	tMem1BusExc2	<= tMem1BusExc;
	tMem2InData2	<= tMem2InData;
	tMem2OK2		<= tMem2OK;
	tMem2BusExc2	<= tMem2BusExc;

	tMemStrobe		<= tNextMemStrobe;

	if(reset)
	begin
		tMem1Latch	<= 0;
		tMem2Latch	<= 0;
	end
	else
	begin
		tMem1Latch	<= tNextMem1Latch;
		tMem2Latch	<= tNextMem2Latch;

//		tMem1Latch	<= tNextMem1Latch && !(tNextMem2Latch &&  tMemStrobe);
//		tMem2Latch	<= tNextMem2Latch && !(tNextMem1Latch && !tMemStrobe);

	end
end

endmodule
