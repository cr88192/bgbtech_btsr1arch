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

`ifndef HAS_EXCPUID
`define HAS_EXCPUID

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
reg			tRngBitC;
reg			tRngBitD;
reg			tRngBitAL;
reg			tRngBitBL;
reg			tRngBitCL;
reg			tRngBitDL;
reg			tRngNoiseA1;
reg			tRngNoiseA2;
reg			tRngNoiseA3;
reg			tRngNoiseA4;
reg			tRngNoiseA;


reg[63:0]		arrCpuIdLo[7:0];
// reg[63:0]		arrCpuIdHi[7:0];

reg[63:0]		valCpuIdLo0;
reg[63:0]		valCpuIdLo1;
reg[63:0]		valCpuIdLo2;
reg[63:0]		valCpuIdLo3;

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

`ifdef jx2_enable_wex
	arrCpuIdLo[1][8] = 1;
`endif
`ifdef jx2_enable_wex3w
	arrCpuIdLo[1][9] = 1;
`endif
`ifdef jx2_enable_wexjumbo
	arrCpuIdLo[1][10] = 1;
`endif
`ifdef jx2_enable_mmu
	arrCpuIdLo[1][11] = 1;
`endif
`ifdef jx2_enable_vaddr48
	arrCpuIdLo[1][12] = 1;
`endif
`ifdef jx2_enable_gsv
	arrCpuIdLo[1][13] = 1;
`endif
`ifdef jx2_enable_pmort
	arrCpuIdLo[1][14] = 1;
`endif
`ifdef jx2_enable_fpu
	arrCpuIdLo[1][15] = 1;
`endif

`ifdef jx2_enable_fpu_w
	arrCpuIdLo[1][16] = 1;
`endif
`ifdef jx2_fpu_longdbl
	arrCpuIdLo[1][17] = 1;
`endif
`ifdef jx2_fpu_fmac
	arrCpuIdLo[1][18] = 1;
`endif
`ifdef jx2_alu_wx
	arrCpuIdLo[1][19] = 1;
`endif

`endif

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
	valCpuIdLo0=64'h2020324632584A42;  //"BJX2F0  ", Arch, Profile, SubVer
	valCpuIdLo1=0;
	valCpuIdLo2=0;
	valCpuIdLo3=0;

`ifdef jx2_enable_ops16

	valCpuIdLo0[39:32]=8'h45;	//'E' Profile

`ifdef jx2_enable_fpu

	valCpuIdLo0[39:32]=8'h46;	//'F' Profile

`ifdef jx2_enable_mmu

	valCpuIdLo0[39:32]=8'h43;	//'C' Profile

`ifdef jx2_enable_wex3w
`ifdef jx2_enable_vaddr48

	valCpuIdLo0[39:32]=8'h41;	//'A' Profile

`ifdef jx2_enable_vaddr96
	valCpuIdLo0[39:32]=8'h47;	//'G' Profile
`endif	//addr96

`endif	//addr48
`endif	//wex3w

`endif	//mmu

`endif	//fpu

`else	//ops16

	valCpuIdLo0[39:32]=8'h44;	//'D' Profile

`ifdef jx2_enable_fpu
	valCpuIdLo0[39:32]=8'h42;	//'B' Profile
`endif

`endif	//ops16

	valCpuIdLo1[3:0] = timers[11:8];

`ifdef jx2_enable_wex
	valCpuIdLo1[8] = 1;
`endif
`ifdef jx2_enable_wex3w
	valCpuIdLo1[9] = 1;
`endif
`ifdef jx2_enable_wexjumbo
	valCpuIdLo1[10] = 1;
`endif
`ifdef jx2_enable_mmu
	valCpuIdLo1[11] = 1;
`endif
`ifdef jx2_enable_vaddr48
	valCpuIdLo1[12] = 1;
`endif
`ifdef jx2_enable_gsv
	valCpuIdLo1[13] = 1;
`endif
`ifdef jx2_enable_pmort
	valCpuIdLo1[14] = 1;
`endif
`ifdef jx2_enable_fpu
	valCpuIdLo1[15] = 1;
`endif

`ifdef jx2_enable_fpu_w
	valCpuIdLo1[16] = 1;
`endif
`ifdef jx2_fpu_longdbl
	valCpuIdLo1[17] = 1;
`endif
`ifdef jx2_fpu_fmac
	valCpuIdLo1[18] = 1;
`endif
`ifdef jx2_alu_wx
	valCpuIdLo1[19] = 1;
`endif

`ifdef jx2_alu_xgpr
	valCpuIdLo1[20] = 1;
`endif
`ifdef jx2_enable_pred_s
	valCpuIdLo1[21] = 1;
`endif
`ifdef jx2_fpu_lane2
	valCpuIdLo1[22] = 1;
`endif
`ifdef jx2_enable_convfp16
	valCpuIdLo1[23] = 1;
`endif

`ifdef jx2_enable_riscv
	valCpuIdLo1[24] = 1;
`endif

`ifdef jx2_enable_cmptag
	valCpuIdLo1[25] = 1;
`endif

`ifdef jx2_enable_vaddr96
	valCpuIdLo1[26] = 1;
`endif
`ifdef jx2_enable_vaddr96qadd
	valCpuIdLo1[27] = 1;
`endif

`ifdef jx2_alu_dmac
	valCpuIdLo1[28] = 1;
`endif
`ifdef jx2_agu_ridisp
	valCpuIdLo1[29] = 1;
`endif

`ifdef jx2_alu_slomuldiv
	valCpuIdLo1[30] = 1;
`endif

`ifdef jx2_fpu_enable_fdiv
	valCpuIdLo1[31] = 1;
`endif

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

	tRngBitC	=
		tRngA[17] ^ tRngA[19] ^
		tRngA[21] ^ tRngA[23] ^
		tRngNoiseA ^ tRngBitDL ^ 1;
	tRngBitD	=
		tRngB[17] ^ tRngB[19] ^
		tRngB[21] ^ tRngB[23] ^
		tRngNoiseA ^ tRngBitCL ^ 1;

//	tNxtRngA	= { tRngBitA, tRngA[31:1] };
//	tNxtRngB	= { tRngBitB, tRngB[31:1] };

	tNxtRngA	= { tRngA[30:16], tRngA[15]^tRngBitC, tRngA[14:0], tRngBitA };
//	tNxtRngB	= { tRngB[30:16], tRngB[15]^tRngBitD, tRngB[14:0], tRngBitB };
	tNxtRngB	= { tRngBitB, tRngB[31:17], tRngB[16]^tRngBitD, tRngB[15:1] };

	tResHi = tResHiA;


`ifdef def_true
	casez(index[4:0])
		5'b0_0000: begin
//			tResLo = tResLoA;
			tResLo = valCpuIdLo0;
			tResHi = tResHiA;
		end
		5'b0_0001: begin
//			tResLo = tResLoA;
			tResHi = tResHiA;
			tResLo = valCpuIdLo1;
//			tResLo[3:0] = timers[11:8];
		end
		5'b0_0010: begin
//			tResLo = tResLoA;
			tResHi = tResHiA;
			tResLo = valCpuIdLo2;
		end
		5'b0_0011: begin
//			tResLo = tResLoA;
			tResHi = tResHiA;
			tResLo = valCpuIdLo3;
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
	tRngBitCL		<= tRngBitC;
	tRngBitDL		<= tRngBitD;

	tRngNoiseA1		<= timers[0];
	tRngNoiseA2		<= tRngNoiseA1;
	tRngNoiseA3		<= tRngNoiseA2;
	tRngNoiseA4		<= tRngNoiseA3;
	tRngNoiseA		<= tRngNoiseA4 ^ tRngNoiseA3;
end

endmodule

`endif
