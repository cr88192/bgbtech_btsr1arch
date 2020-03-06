module ExCpuId(
	/* verilator lint_off UNUSED */
	clock,	reset,
	index,
	resLo,	resHi
	);

input			clock;
input			reset;
input[4:0]		index;
output[63:0]	resLo;
output[63:0]	resHi;

parameter		isAltCore = 0;


reg[63:0]		tResLo;
reg[63:0]		tResHi;

assign		resLo = tResLo;
assign		resHi = tResHi;


reg[63:0]		arrCpuIdLo[7:0];
// reg[63:0]		arrCpuIdHi[7:0];

initial
begin
	arrCpuIdLo[0]=64'h2020324632584A42;  //"BJX2F0  ", Arch, Profile, SubVer
	arrCpuIdLo[1]=UV64_00;
	arrCpuIdLo[2]=UV64_00;
	arrCpuIdLo[3]=UV64_00;

	arrCpuIdLo[4]=UV64_XX;
	arrCpuIdLo[5]=UV64_XX;
	arrCpuIdLo[6]=UV64_XX;
	arrCpuIdLo[7]=UV64_XX;

	arrCpuIdLo[1][0] = isAltCore;

`ifndef def_true
	arrCpuIdHi[0]=UV64_00;
	arrCpuIdHi[1]=UV64_00;
	arrCpuIdHi[2]=UV64_00;
	arrCpuIdHi[3]=UV64_00;

	arrCpuIdHi[4]=UV64_XX;
	arrCpuIdHi[5]=UV64_XX;
	arrCpuIdHi[6]=UV64_XX;
	arrCpuIdHi[7]=UV64_XX;
`endif
end

always @*
begin
	tResLo = arrCpuIdLo[index[2:0]];
//	tResHi = arrCpuIdHi[index[2:0]];
	tResHi = UV64_00;
end

endmodule