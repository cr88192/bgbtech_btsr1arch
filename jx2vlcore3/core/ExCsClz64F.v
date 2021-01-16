/*
Determine the number of leading zero bits in an 64 bit value.
 */

`ifndef HAS_CSCLZ64F
`define HAS_CSCLZ64F

module ExCsClz64F(valIn, valCnt);

input [63:0]	valIn;
output[ 7:0]	valCnt;

reg[7:0]	tZeroMask;
reg[7:0]	tPickBits;
reg[3:0]	tPickCnt;
reg[2:0]	tPickCntLo;
reg			tIsZero;

reg[7:0]	tCnt;

assign		valCnt = tCnt;

always @*
begin
	tZeroMask[ 7] = (valIn[63:56] != 0);
	tZeroMask[ 6] = (valIn[55:48] != 0);
	tZeroMask[ 5] = (valIn[47:40] != 0);
	tZeroMask[ 4] = (valIn[39:32] != 0);
	tZeroMask[ 3] = (valIn[31:24] != 0);
	tZeroMask[ 2] = (valIn[23:16] != 0);
	tZeroMask[ 1] = (valIn[15: 8] != 0);
	tZeroMask[ 0] = (valIn[ 7: 0] != 0);
	
	casez(tZeroMask)
		8'b1zzzzzzz: begin tPickBits=valIn[63:56]; tPickCnt=0; end
		8'b01zzzzzz: begin tPickBits=valIn[55:48]; tPickCnt=1; end
		8'b001zzzzz: begin tPickBits=valIn[47:40]; tPickCnt=2; end
		8'b0001zzzz: begin tPickBits=valIn[39:32]; tPickCnt=3; end
		8'b00001zzz: begin tPickBits=valIn[31:24]; tPickCnt=4; end
		8'b000001zz: begin tPickBits=valIn[23:16]; tPickCnt=5; end
		8'b0000001z: begin tPickBits=valIn[15: 8]; tPickCnt=6; end
		8'b00000001: begin tPickBits=valIn[ 7: 0]; tPickCnt=7; end
		8'b00000000: begin tPickBits=       8'hFF; tPickCnt=8; end
	endcase
	
	tIsZero = (tPickCnt == 8);

	casez(tPickBits)
		8'b1zzzzzzz: tPickCntLo=0;
		8'b01zzzzzz: tPickCntLo=1;
		8'b001zzzzz: tPickCntLo=2;
		8'b0001zzzz: tPickCntLo=3;
		8'b00001zzz: tPickCntLo=4;
		8'b000001zz: tPickCntLo=5;
		8'b0000001z: tPickCntLo=6;
		8'b00000001: tPickCntLo=7;
		8'b00000000: tPickCntLo=0;
	endcase
	
	tCnt = { tIsZero, tPickCnt, tPickCntLo };
end

endmodule

`endif
