module ExCpuId(
	/* verilator lint_off UNUSED */
	clock,	reset,
	timers,
	index,
	resLo,	resHi
	);

input			clock;
input			reset;
input[11:0]		timers;

input[4:0]		index;
output[63:0]	resLo;
output[63:0]	resHi;

parameter		isAltCore = 0;


reg[63:0]		tResLoA;
reg[63:0]		tResHiA;
reg[63:0]		tResLo;
reg[63:0]		tResHi;

assign		resLo = tResLo;
assign		resHi = tResHi;

reg[31:0]	tRngA;
reg[31:0]	tRngB;
reg[31:0]	tNxtRngA;
reg[31:0]	tNxtRngB;
reg			tRngBitA;
reg			tRngBitB;
reg			tRngBitAL;
reg			tRngBitBL;
reg			tRngNoiseA1;
reg			tRngNoiseA2;
reg			tRngNoiseA;


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

//	arrCpuIdLo[1][0] = isAltCore;
//	arrCpuIdLo[1][3:0] = isAltCore;

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
	tResLoA = arrCpuIdLo[index[2:0]];
	tResHiA = UV64_00;
//	tResHiA = arrCpuIdHi[index[2:0]];
	
	tRngBitA	=
		tRngA[1] ^ tRngA[3] ^
		tRngA[5] ^ tRngA[7] ^
		tRngNoiseA ^ tRngBitBL ^ 1;
	tRngBitB	=
		tRngB[1] ^ tRngB[3] ^
		tRngB[5] ^ tRngB[7] ^
		tRngNoiseA ^ tRngBitAL ^ 1;

	tNxtRngA	= { tRngBitA, tRngA[31:1] };
	tNxtRngB	= { tRngBitB, tRngB[31:1] };

	tResHi = tResHiA;


`ifdef def_true
	casez(index[4:0])
		5'b0_0000: begin
			tResLo = tResLoA;
			tResHi = tResHiA;
		end
		5'b0_0001: begin
			tResLo = tResLoA;
			tResHi = tResHiA;
			tResLo[3:0] = timers[11:8];
		end
		5'b0_001z: begin
			tResLo = tResLoA;
			tResHi = tResHiA;
		end
		5'b0_01zz: begin
			tResLo = tResLoA;
			tResHi = tResHiA;
		end
		5'b0_1zzz: begin
			tResLo = tResLoA;
			tResHi = tResHiA;
		end

		5'b1_0zzz: begin
			tResLo = UV64_00;
			tResHi = UV64_00;
		end

		5'b1_10zz: begin
			tResLo = UV64_00;
			tResHi = UV64_00;
		end
		5'b1_110z: begin
			tResLo = UV64_00;
			tResHi = UV64_00;
		end

		5'b1_1110: begin
			tResLo = UV64_00;
			tResHi = UV64_00;
		end
		5'b1_1111: begin
			tResLo = {
				tRngA[31:28], tRngB[ 3: 0],
				tRngA[27:24], tRngB[ 7: 4],
				tRngA[23:20], tRngB[11: 8],
				tRngA[19:16], tRngB[15:12],
				tRngA[15:12], tRngB[19:16],
				tRngA[11: 8], tRngB[23:20],
				tRngA[ 7: 4], tRngB[27:24],
				tRngA[ 3: 0], tRngB[31:28]
			};
		end
//		default: begin
//			tResLo = UV64_00;
//		end
	endcase
`endif
end

always @(posedge clock)
begin
	tRngA			<= tNxtRngA;
	tRngB			<= tNxtRngB;
	tRngBitAL		<= tRngBitA;
	tRngBitBL		<= tRngBitB;

	tRngNoiseA1		<= timers[0];
	tRngNoiseA2		<= tRngNoiseA1;
	tRngNoiseA		<= tRngNoiseA2 ^ tRngNoiseA1;
end

endmodule