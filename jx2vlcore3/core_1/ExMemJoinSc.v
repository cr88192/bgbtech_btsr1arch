module ExMemJoinSc(
	clock_cpu,	clock_master,
	reset,

	memInData,	memOutData,
	memAddrA,	memAddrB,
	memOpm,		memOK,
	memBusExc,

	mem1InData,	mem1OutData,
	mem1AddrA,	mem1AddrB,
	mem1Opm,	mem1OK,
	mem1BusExc
	);

input			clock_cpu;
input			clock_master;
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


reg[127:0]		tMemInData;
reg[1:0]		tMemOK;
reg[63:0]		tMemBusExc;

reg[127:0]		tMemInData2;
reg[1:0]		tMemOK2;
reg[63:0]		tMemBusExc2;

reg[127:0]		tMemOutData;
reg[47:0]		tMemAddrA;
reg[47:0]		tMemAddrB;
reg[4:0]		tMemOpm;

reg[127:0]		tMemOutData2;
reg[47:0]		tMemAddrA2;
reg[47:0]		tMemAddrB2;
reg[4:0]		tMemOpm2;

`ifdef jx2_mem_l2exbuf
reg[127:0]		tMemOutData3;
reg[47:0]		tMemAddrA3;
reg[47:0]		tMemAddrB3;
reg[4:0]		tMemOpm3;

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

reg[127:0]		tMem1InData;
reg[1:0]		tMem1OK;
reg[63:0]		tMem1BusExc;

reg[127:0]		tMem1InData2;
reg[1:0]		tMem1OK2;
reg[63:0]		tMem1BusExc2;

assign		mem1InData	= tMem1InData2;
assign		mem1OK		= tMem1OK2;
assign		mem1BusExc	= tMem1BusExc2;

always @*
begin
	tMem1InData		= tMemInData;
	tMem1BusExc		= tMemBusExc;
	tMem1OK			= tMemOK;

	tMemOutData		= mem1OutData;
	tMemAddrA		= mem1AddrA;
	tMemAddrB		= mem1AddrB;
	tMemOpm			= mem1Opm;
end

`ifdef jx2_mem_l2exbuf
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
`ifdef jx2_mem_l2exbuf
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
end

endmodule
