/*
Determine the number of leading zero bits in an 88 bit value.
 */

`ifndef HAS_CSCLZ88F
`define HAS_CSCLZ88F

module ExCsClz88F(valIn, valCnt);

input [87:0]	valIn;
output[ 7:0]	valCnt;

reg[10:0]	tZeroMask;
reg[7:0]	tPickBits;
reg[3:0]	tPickCnt;
reg[2:0]	tPickCntLo;
reg			tIsZero;

reg[7:0]	tCnt;

assign		valCnt = tCnt;

always @*
begin
	tZeroMask[10] = (valIn[87:80] != 0);
	tZeroMask[ 9] = (valIn[79:72] != 0);
	tZeroMask[ 8] = (valIn[71:64] != 0);
	tZeroMask[ 7] = (valIn[63:56] != 0);
	tZeroMask[ 6] = (valIn[55:48] != 0);
	tZeroMask[ 5] = (valIn[47:40] != 0);
	tZeroMask[ 4] = (valIn[39:32] != 0);
	tZeroMask[ 3] = (valIn[31:24] != 0);
	tZeroMask[ 2] = (valIn[23:16] != 0);
	tZeroMask[ 1] = (valIn[15: 8] != 0);
	tZeroMask[ 0] = (valIn[ 7: 0] != 0);
	
	casez(tZeroMask)
		11'b1zzzzzzzzzz: begin tPickBits=valIn[87:80]; tPickCnt= 0; end
		11'b01zzzzzzzzz: begin tPickBits=valIn[79:72]; tPickCnt= 1; end
		11'b001zzzzzzzz: begin tPickBits=valIn[71:64]; tPickCnt= 2; end
		11'b0001zzzzzzz: begin tPickBits=valIn[63:56]; tPickCnt= 3; end
		11'b00001zzzzzz: begin tPickBits=valIn[55:48]; tPickCnt= 4; end
		11'b000001zzzzz: begin tPickBits=valIn[47:40]; tPickCnt= 5; end
		11'b0000001zzzz: begin tPickBits=valIn[39:32]; tPickCnt= 6; end
		11'b00000001zzz: begin tPickBits=valIn[31:24]; tPickCnt= 7; end
		11'b000000001zz: begin tPickBits=valIn[23:16]; tPickCnt= 8; end
		11'b0000000001z: begin tPickBits=valIn[15: 8]; tPickCnt= 9; end
		11'b00000000001: begin tPickBits=valIn[ 7: 0]; tPickCnt=10; end
		11'b00000000000: begin tPickBits=       8'hFF; tPickCnt=11; end
	endcase
	
	tIsZero = (tPickCnt == 11);

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
