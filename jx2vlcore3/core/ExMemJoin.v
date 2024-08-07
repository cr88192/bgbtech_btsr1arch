module ExMemJoin(
	clock_cpu,	clock_master,
	reset,

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

input			clock_cpu;
input			clock_master;
input			reset;

`input_tile		memInData;
`output_tile	memOutData;
output[47:0]	memAddrA;
output[47:0]	memAddrB;
output[15:0]	memOpm;
input[1:0]		memOK;
input[63:0]		memBusExc;

`output_tile	mem1InData;
`input_tile		mem1OutData;
input[47:0]		mem1AddrA;
input[47:0]		mem1AddrB;
input[15:0]		mem1Opm;
output[1:0]		mem1OK;
output[63:0]	mem1BusExc;

`output_tile	mem2InData;
`input_tile		mem2OutData;
input[47:0]		mem2AddrA;
input[47:0]		mem2AddrB;
input[15:0]		mem2Opm;
output[1:0]		mem2OK;
output[63:0]	mem2BusExc;


`reg_tile		tMemInData;
reg[1:0]		tMemOK;
reg[63:0]		tMemBusExc;

`reg_tile		tMemInData2;
reg[1:0]		tMemOK2;
reg[63:0]		tMemBusExc2;

`reg_tile		tMemOutData;
reg[47:0]		tMemAddrA;
reg[47:0]		tMemAddrB;
reg[15:0]		tMemOpm;

`reg_tile		tMemOutData2;
reg[47:0]		tMemAddrA2;
reg[47:0]		tMemAddrB2;
reg[15:0]		tMemOpm2;

`ifdef jx2_mem_jnexbuf
`reg_tile		tMemOutData3;
reg[47:0]		tMemAddrA3;
reg[47:0]		tMemAddrB3;
reg[15:0]		tMemOpm3;

assign		memOutData	= tMemOutData3;
assign		memAddrA	= tMemAddrA3;
assign		memAddrB	= tMemAddrB3;
assign		memOpm		= tMemOpm3;
`else
assign		memOutData	= tMemOutData2;
assign		memAddrA	= tMemAddrA2;
assign		memAddrB	= tMemAddrB2;
assign		memOpm		= tMemOpm2;
`endif

`reg_tile		tMem1InData;
reg[1:0]		tMem1OK;
reg[63:0]		tMem1BusExc;
`reg_tile		tMem2InData;
reg[1:0]		tMem2OK;
reg[63:0]		tMem2BusExc;

`reg_tile		tMem1InData2;
reg[1:0]		tMem1OK2;
reg[63:0]		tMem1BusExc2;
`reg_tile		tMem2InData2;
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
	
//	tMem1OK			= (mem1Opm != UMEM_OPM_READY) ?
//		UMEM_OK_HOLD : UMEM_OK_READY;
//	tMem2OK			= (mem2Opm != UMEM_OPM_READY) ?
//		UMEM_OK_HOLD : UMEM_OK_READY;

	tMem1OK			= UMEM_OK_READY;
	tMem2OK			= UMEM_OK_READY;

//	tMem1OK			= tMem2Latch ? UMEM_OK_HOLD : UMEM_OK_READY;
//	tMem2OK			= tMem1Latch ? UMEM_OK_HOLD : UMEM_OK_READY;

	tMem1BusExc		= UV64_00;
	tMem2BusExc		= UV64_00;

	tMemOutData		= 0;
	tMemAddrA		= 0;
	tMemAddrB		= 0;
	tMemOpm			= 0;

	tMem1InData		= tMemInData;
	tMem2InData		= tMemInData;
//	tMem1BusExc		= UV64_00;
	tMem1BusExc		= tMemBusExc;
//	tMem2BusExc		= UV64_00;
	
	if(tMemBusExc[15])
	begin
		case(tMemBusExc[11:8])
			4'h0: 		tMem1BusExc		= tMemBusExc;
			4'h1: 		tMem1BusExc		= tMemBusExc;
			4'h2: 		tMem2BusExc		= tMemBusExc;
			4'hF: begin
				tMem1BusExc		= tMemBusExc;
		 		tMem2BusExc		= tMemBusExc;
			 end
			default: begin
			end
		endcase
	end
	
//	if(tMem1Latch || ((mem1Opm != UMEM_OPM_READY) && !tMem2Latch))

	if(tMem1Latch)
	begin
		tNextMem1Latch	=
			(mem1Opm[4:0] != UMEM_OPM_READY) ||
			(tMemOK != UMEM_OK_READY);
		tMem1OK			= tMemOK;
//		tMem1BusExc		= tMemBusExc;

		tMemOutData		= mem1OutData;
		tMemAddrA		= mem1AddrA;
		tMemAddrB		= mem1AddrB;
		tMemOpm			= mem1Opm;
	end
	else
		if(tMem2Latch)
	begin
		tNextMem2Latch	=
			(mem2Opm[4:0] != UMEM_OPM_READY) ||
			(tMemOK != UMEM_OK_READY);
		tMem2OK			= tMemOK;
//		tMem2BusExc		= tMemBusExc;

		tMemOutData		= mem2OutData;
		tMemAddrA		= mem2AddrA;
		tMemAddrB		= mem2AddrB;
		tMemOpm			= mem2Opm;
	end
	else
		if((mem1Opm[4:0] != UMEM_OPM_READY) &&
			!((mem2Opm[4:0] != UMEM_OPM_READY) && tMemStrobe))
	begin
		tNextMem1Latch	= 1;

//		tMemOutData		= mem1OutData;
//		tMemAddrA		= mem1AddrA;
//		tMemAddrB		= mem1AddrB;
//		tMemOpm			= mem1Opm;
	end
	else
		if(mem2Opm[4:0] != UMEM_OPM_READY)
	begin
		tNextMem2Latch	= 1;

//		tMemOutData		= mem2OutData;
//		tMemAddrA		= mem2AddrA;
//		tMemAddrB		= mem2AddrB;
//		tMemOpm			= mem2Opm;
	end

	if(reset)
	begin
		tNextMem1Latch	= 0;
		tNextMem2Latch	= 0;
		tMemOpm			= { 11'h0, UMEM_OPM_READY };
	end

end

`ifdef jx2_mem_jnexbuf
always @(posedge clock_master)
begin
	tMemOutData3	<= tMemOutData2;
	tMemAddrA3		<= tMemAddrA2;
	tMemAddrB3		<= tMemAddrB2;
	tMemOpm3		<= tMemOpm2;
end
`endif

always @(posedge clock_cpu)
begin
`ifdef jx2_mem_jnexbuf
	tMemInData2		<= memInData;
	tMemBusExc2		<= memBusExc;
	tMemOK2			<= memOK;
	tMemInData		<= tMemInData2;
	tMemBusExc		<= tMemBusExc2;
	tMemOK			<= tMemOK2;
`else
	tMemInData		<= memInData;
	tMemOK			<= memOK;
	tMemBusExc		<= memBusExc;
`endif

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

//	if(reset)
//	begin
//		tMem1Latch	<= 0;
//		tMem2Latch	<= 0;
//	end
//	else
//	begin
		tMem1Latch	<= tNextMem1Latch;
		tMem2Latch	<= tNextMem2Latch;

//		tMem1Latch	<= tNextMem1Latch && !(tNextMem2Latch &&  tMemStrobe);
//		tMem2Latch	<= tNextMem2Latch && !(tNextMem1Latch && !tMemStrobe);

//	end
end

endmodule
