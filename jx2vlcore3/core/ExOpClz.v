`ifndef HAS_EXOPCLZ
`define HAS_EXOPCLZ

module ExOpClz(
	clock,		reset,
	idUCmd,		idUIxt,
	valRs,		valRn,
	clzValRs2
	);

input	clock;
input	reset;

input[63:0]		valRs;
output[7:0]		valRn;
input[7:0]		idUCmd;
input[7:0]		idUIxt;
// input			exHold;

output[63:0]	clzValRs2;

reg[7:0]	tValRn;
assign 	valRn = tValRn;

reg[5:0]	tClzMsk;

reg[31:0]	tValShl5;
reg[15:0]	tValShl4;
reg[ 7:0]	tValShl3;
reg[ 3:0]	tValShl2;
reg[ 1:0]	tValShl1;
reg			tValShl0;

reg[15:0]	tValTrans0;
reg[15:0]	tValTrans1;
reg[15:0]	tValTrans2;
reg[15:0]	tValTrans3;

reg[63:0]	tValRs;
reg[63:0]	tValRsT;
assign		clzValRs2 = tValRsT;

reg[4:0]	tValHiCnt0;
reg[4:0]	tValHiCnt1;
reg[4:0]	tValHiCnt2;
reg[4:0]	tValHiCnt3;

reg[5:0]	tValHiCnt0B;
reg[5:0]	tValHiCnt1B;

always @*
begin
	tClzMsk = 0;

`ifdef	def_true
	tValTrans0 = {	valRs[48], valRs[49], valRs[50], valRs[51],
					valRs[52], valRs[53], valRs[54], valRs[55],
					valRs[56], valRs[57], valRs[58], valRs[59],
					valRs[60], valRs[61], valRs[62], valRs[63] };
	tValTrans1 = {	valRs[32], valRs[33], valRs[34], valRs[35],
					valRs[36], valRs[37], valRs[38], valRs[39],
					valRs[40], valRs[41], valRs[42], valRs[43],
					valRs[44], valRs[45], valRs[46], valRs[47] };
	tValTrans2 = {	valRs[16], valRs[17], valRs[18], valRs[19],
					valRs[20], valRs[21], valRs[22], valRs[23],
					valRs[24], valRs[25], valRs[26], valRs[27],
					valRs[28], valRs[29], valRs[30], valRs[31] };
	tValTrans3 = {	valRs[ 0], valRs[ 1], valRs[ 2], valRs[ 3],
					valRs[ 4], valRs[ 5], valRs[ 6], valRs[ 7],
					valRs[ 8], valRs[ 9], valRs[10], valRs[11],
					valRs[12], valRs[13], valRs[14], valRs[15] };

	tValRsT = { tValTrans3, tValTrans2, tValTrans1, tValTrans0 };

	case({idUIxt[5], idUIxt[1:0]})
		3'b000: tValRs = { valRs[31:0], UV32_FF };
		3'b001: tValRs = { tValRsT[63:32], UV32_FF };
//		3'b001: tValRs = { tValTrans3, tValTrans2, UV32_FF };
//		3'b010: tValRs = { ~valRs[31:0], UV32_FF };
//		3'b011: tValRs = { ~tValTrans3, ~tValTrans2, UV32_FF };

		3'b100: tValRs = valRs;
		3'b101: tValRs = tValRsT;
//		3'b110: tValRs = ~valRs;
//		3'b111: tValRs = { ~tValTrans3, ~tValTrans2, ~tValTrans1, ~tValTrans0};

		default: tValRs = valRs;
	endcase
`endif

`ifdef	def_true
// `ifndef	def_true
	casez(tValRs[63:48])
		16'b1zzz_zzzz_zzzz_zzzz: tValHiCnt0=0;
		16'b01zz_zzzz_zzzz_zzzz: tValHiCnt0=1;
		16'b001z_zzzz_zzzz_zzzz: tValHiCnt0=2;
		16'b0001_zzzz_zzzz_zzzz: tValHiCnt0=3;
		16'b0000_1zzz_zzzz_zzzz: tValHiCnt0=4;
		16'b0000_01zz_zzzz_zzzz: tValHiCnt0=5;
		16'b0000_001z_zzzz_zzzz: tValHiCnt0=6;
		16'b0000_0001_zzzz_zzzz: tValHiCnt0=7;
		16'b0000_0000_1zzz_zzzz: tValHiCnt0=8;
		16'b0000_0000_01zz_zzzz: tValHiCnt0=9;
		16'b0000_0000_001z_zzzz: tValHiCnt0=10;
		16'b0000_0000_0001_zzzz: tValHiCnt0=11;
		16'b0000_0000_0000_1zzz: tValHiCnt0=12;
		16'b0000_0000_0000_01zz: tValHiCnt0=13;
		16'b0000_0000_0000_001z: tValHiCnt0=14;
		16'b0000_0000_0000_0001: tValHiCnt0=15;
		16'b0000_0000_0000_0000: tValHiCnt0=16;
	endcase

	casez(tValRs[47:32])
		16'b1zzz_zzzz_zzzz_zzzz: tValHiCnt1=0;
		16'b01zz_zzzz_zzzz_zzzz: tValHiCnt1=1;
		16'b001z_zzzz_zzzz_zzzz: tValHiCnt1=2;
		16'b0001_zzzz_zzzz_zzzz: tValHiCnt1=3;
		16'b0000_1zzz_zzzz_zzzz: tValHiCnt1=4;
		16'b0000_01zz_zzzz_zzzz: tValHiCnt1=5;
		16'b0000_001z_zzzz_zzzz: tValHiCnt1=6;
		16'b0000_0001_zzzz_zzzz: tValHiCnt1=7;
		16'b0000_0000_1zzz_zzzz: tValHiCnt1=8;
		16'b0000_0000_01zz_zzzz: tValHiCnt1=9;
		16'b0000_0000_001z_zzzz: tValHiCnt1=10;
		16'b0000_0000_0001_zzzz: tValHiCnt1=11;
		16'b0000_0000_0000_1zzz: tValHiCnt1=12;
		16'b0000_0000_0000_01zz: tValHiCnt1=13;
		16'b0000_0000_0000_001z: tValHiCnt1=14;
		16'b0000_0000_0000_0001: tValHiCnt1=15;
		16'b0000_0000_0000_0000: tValHiCnt1=16;
	endcase

	casez(tValRs[31:16])
		16'b1zzz_zzzz_zzzz_zzzz: tValHiCnt2=0;
		16'b01zz_zzzz_zzzz_zzzz: tValHiCnt2=1;
		16'b001z_zzzz_zzzz_zzzz: tValHiCnt2=2;
		16'b0001_zzzz_zzzz_zzzz: tValHiCnt2=3;
		16'b0000_1zzz_zzzz_zzzz: tValHiCnt2=4;
		16'b0000_01zz_zzzz_zzzz: tValHiCnt2=5;
		16'b0000_001z_zzzz_zzzz: tValHiCnt2=6;
		16'b0000_0001_zzzz_zzzz: tValHiCnt2=7;
		16'b0000_0000_1zzz_zzzz: tValHiCnt2=8;
		16'b0000_0000_01zz_zzzz: tValHiCnt2=9;
		16'b0000_0000_001z_zzzz: tValHiCnt2=10;
		16'b0000_0000_0001_zzzz: tValHiCnt2=11;
		16'b0000_0000_0000_1zzz: tValHiCnt2=12;
		16'b0000_0000_0000_01zz: tValHiCnt2=13;
		16'b0000_0000_0000_001z: tValHiCnt2=14;
		16'b0000_0000_0000_0001: tValHiCnt2=15;
		16'b0000_0000_0000_0000: tValHiCnt2=16;
	endcase

	casez(tValRs[15:0])
		16'b1zzz_zzzz_zzzz_zzzz: tValHiCnt3=0;
		16'b01zz_zzzz_zzzz_zzzz: tValHiCnt3=1;
		16'b001z_zzzz_zzzz_zzzz: tValHiCnt3=2;
		16'b0001_zzzz_zzzz_zzzz: tValHiCnt3=3;
		16'b0000_1zzz_zzzz_zzzz: tValHiCnt3=4;
		16'b0000_01zz_zzzz_zzzz: tValHiCnt3=5;
		16'b0000_001z_zzzz_zzzz: tValHiCnt3=6;
		16'b0000_0001_zzzz_zzzz: tValHiCnt3=7;
		16'b0000_0000_1zzz_zzzz: tValHiCnt3=8;
		16'b0000_0000_01zz_zzzz: tValHiCnt3=9;
		16'b0000_0000_001z_zzzz: tValHiCnt3=10;
		16'b0000_0000_0001_zzzz: tValHiCnt3=11;
		16'b0000_0000_0000_1zzz: tValHiCnt3=12;
		16'b0000_0000_0000_01zz: tValHiCnt3=13;
		16'b0000_0000_0000_001z: tValHiCnt3=14;
		16'b0000_0000_0000_0001: tValHiCnt3=15;
		16'b0000_0000_0000_0000: tValHiCnt3=16;
	endcase

	casez( { tValHiCnt0[4], tValHiCnt1[4], tValHiCnt2[4], tValHiCnt3[4] })
		4'b0zzz: tValRn = { 4'b0000, tValHiCnt0[3:0] };
		4'b10zz: tValRn = { 4'b0001, tValHiCnt1[3:0] };
		4'b110z: tValRn = { 4'b0010, tValHiCnt2[3:0] };
		4'b1110: tValRn = { 4'b0011, tValHiCnt3[3:0] };
		4'b1111: tValRn = 64;
	endcase

`endif


// `ifdef	def_true
`ifndef	def_true
	casez(tValRs[63:32])
		32'b1zzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 0;
		32'b01zz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 1;
		32'b001z_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 2;
		32'b0001_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 3;
		32'b0000_1zzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 4;
		32'b0000_01zz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 5;
		32'b0000_001z_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 6;
		32'b0000_0001_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 7;
		32'b0000_0000_1zzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 8;
		32'b0000_0000_01zz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B= 9;
		32'b0000_0000_001z_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B=10;
		32'b0000_0000_0001_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B=11;
		32'b0000_0000_0000_1zzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B=12;
		32'b0000_0000_0000_01zz_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B=13;
		32'b0000_0000_0000_001z_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B=14;
		32'b0000_0000_0000_0001_zzzz_zzzz_zzzz_zzzz: tValHiCnt0B=15;
		32'b0000_0000_0000_0000_1zzz_zzzz_zzzz_zzzz: tValHiCnt0B=16;
		32'b0000_0000_0000_0000_01zz_zzzz_zzzz_zzzz: tValHiCnt0B=17;
		32'b0000_0000_0000_0000_001z_zzzz_zzzz_zzzz: tValHiCnt0B=18;
		32'b0000_0000_0000_0000_0001_zzzz_zzzz_zzzz: tValHiCnt0B=19;
		32'b0000_0000_0000_0000_0000_1zzz_zzzz_zzzz: tValHiCnt0B=20;
		32'b0000_0000_0000_0000_0000_01zz_zzzz_zzzz: tValHiCnt0B=21;
		32'b0000_0000_0000_0000_0000_001z_zzzz_zzzz: tValHiCnt0B=22;
		32'b0000_0000_0000_0000_0000_0001_zzzz_zzzz: tValHiCnt0B=23;
		32'b0000_0000_0000_0000_0000_0000_1zzz_zzzz: tValHiCnt0B=24;
		32'b0000_0000_0000_0000_0000_0000_01zz_zzzz: tValHiCnt0B=25;
		32'b0000_0000_0000_0000_0000_0000_001z_zzzz: tValHiCnt0B=26;
		32'b0000_0000_0000_0000_0000_0000_0001_zzzz: tValHiCnt0B=27;
		32'b0000_0000_0000_0000_0000_0000_0000_1zzz: tValHiCnt0B=28;
		32'b0000_0000_0000_0000_0000_0000_0000_01zz: tValHiCnt0B=29;
		32'b0000_0000_0000_0000_0000_0000_0000_001z: tValHiCnt0B=30;
		32'b0000_0000_0000_0000_0000_0000_0000_0001: tValHiCnt0B=31;
		32'b0000_0000_0000_0000_0000_0000_0000_0000: tValHiCnt0B=32;
	endcase

	casez(tValRs[31: 0])
		32'b1zzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 0;
		32'b01zz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 1;
		32'b001z_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 2;
		32'b0001_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 3;
		32'b0000_1zzz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 4;
		32'b0000_01zz_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 5;
		32'b0000_001z_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 6;
		32'b0000_0001_zzzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 7;
		32'b0000_0000_1zzz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 8;
		32'b0000_0000_01zz_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B= 9;
		32'b0000_0000_001z_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B=10;
		32'b0000_0000_0001_zzzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B=11;
		32'b0000_0000_0000_1zzz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B=12;
		32'b0000_0000_0000_01zz_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B=13;
		32'b0000_0000_0000_001z_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B=14;
		32'b0000_0000_0000_0001_zzzz_zzzz_zzzz_zzzz: tValHiCnt1B=15;
		32'b0000_0000_0000_0000_1zzz_zzzz_zzzz_zzzz: tValHiCnt1B=16;
		32'b0000_0000_0000_0000_01zz_zzzz_zzzz_zzzz: tValHiCnt1B=17;
		32'b0000_0000_0000_0000_001z_zzzz_zzzz_zzzz: tValHiCnt1B=18;
		32'b0000_0000_0000_0000_0001_zzzz_zzzz_zzzz: tValHiCnt1B=19;
		32'b0000_0000_0000_0000_0000_1zzz_zzzz_zzzz: tValHiCnt1B=20;
		32'b0000_0000_0000_0000_0000_01zz_zzzz_zzzz: tValHiCnt1B=21;
		32'b0000_0000_0000_0000_0000_001z_zzzz_zzzz: tValHiCnt1B=22;
		32'b0000_0000_0000_0000_0000_0001_zzzz_zzzz: tValHiCnt1B=23;
		32'b0000_0000_0000_0000_0000_0000_1zzz_zzzz: tValHiCnt1B=24;
		32'b0000_0000_0000_0000_0000_0000_01zz_zzzz: tValHiCnt1B=25;
		32'b0000_0000_0000_0000_0000_0000_001z_zzzz: tValHiCnt1B=26;
		32'b0000_0000_0000_0000_0000_0000_0001_zzzz: tValHiCnt1B=27;
		32'b0000_0000_0000_0000_0000_0000_0000_1zzz: tValHiCnt1B=28;
		32'b0000_0000_0000_0000_0000_0000_0000_01zz: tValHiCnt1B=29;
		32'b0000_0000_0000_0000_0000_0000_0000_001z: tValHiCnt1B=30;
		32'b0000_0000_0000_0000_0000_0000_0000_0001: tValHiCnt1B=31;
		32'b0000_0000_0000_0000_0000_0000_0000_0000: tValHiCnt1B=32;
	endcase

	casez( { tValHiCnt0B[5], tValHiCnt1B[5] })
		2'b0z: tValRn = { 3'b000, tValHiCnt0B[4:0] };
		2'b10: tValRn = { 3'b001, tValHiCnt1B[4:0] };
		2'b11: tValRn = 64;
	endcase

`endif



`ifndef def_true
// `ifdef def_true
//	if(!idUIxt[0])
	if(1'b0)
	begin
		tValShl5	= valRs[31:0];
		tClzMsk[5]	= 0;
	end else
//		if(valRs[63:32]==0)
//		if(valRs[63:31]==0)
//		if(tValRs[63:31]==0)
		if(tValRs[63:32]==0)
	begin
//		tValShl5	= valRs[31:0];
		tValShl5	= tValRs[31:0];
		tClzMsk[5]	= 1;
	end else begin
//		tValShl5	= valRs[63:32];
		tValShl5	= tValRs[63:32];
	end

	if(tValShl5[31:16]==0)
//	if(tValShl5[31:15]==0)
	begin
		tValShl4	= tValShl5[15:0];
		tClzMsk[4]	= 1;
	end else begin
		tValShl4	= tValShl5[31:16];
	end
	
	if(tValShl4[15:8]==0)
//	if(tValShl4[15:7]==0)
	begin
		tValShl3	= tValShl4[7:0];
		tClzMsk[3]	= 1;
	end else begin
		tValShl3	= tValShl4[15:8];
	end
	
	if(tValShl3[7:4]==0)
//	if(tValShl3[7:3]==0)
	begin
		tValShl2	= tValShl3[3:0];
		tClzMsk[2]	= 1;
	end else begin
		tValShl2	= tValShl3[7:4];
	end
	
	if(tValShl2[3:2]==0)
//	if(tValShl2[3:1]==0)
	begin
		tValShl1	= tValShl2[1:0];
		tClzMsk[1]	= 1;
	end else begin
		tValShl1	= tValShl2[3:2];
	end

	if(tValShl1[1]==0)
//	if(tValShl1[1:0]==0)
	begin
		tValShl0	= tValShl1[0];
		tClzMsk[0]	= 1;
	end else begin
		tValShl0	= tValShl1[1];
//		tValShl0	= tValShl1[0];
	end

	tValRn = { 2'b00, tClzMsk } + { UV7_00, (tValShl0==0) };
`endif

end

endmodule

`endif
