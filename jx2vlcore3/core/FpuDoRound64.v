/*
Attempt at a low-latency FP64 Rounding Module.

Mode:
  00: Pass input unchanged.
  01: Round input up
  10: Zero
  11: Overflowed to Infinity.
 */

`ifndef HAS_JX2FPUDOROUND64_D
`define HAS_JX2FPUDOROUND64_D

module FpuDoRound64(valOut, valIn, mode);

input[63:0]		valIn;
output[63:0]	valOut;
input[1:0]		mode;

reg[8:0]		tValRndA0p;
reg[8:0]		tValRndA1p;
reg[8:0]		tValRndA2p;
reg[8:0]		tValRndA3p;
reg[8:0]		tValRndA4p;
reg[8:0]		tValRndA5p;
reg[8:0]		tValRndA6p;
reg[8:0]		tValRndA7p;

reg[7:0]		tValRndCp;
reg[15:0]		tValRndSm;

reg[63:0]		tValOut;
assign		valOut = tValOut;


always @*
begin
	tValRndA0p = { 1'b0, valIn[ 7: 0] } + 1;
	tValRndA1p = { 1'b0, valIn[15: 8] } + 1;
	tValRndA2p = { 1'b0, valIn[23:16] } + 1;
	tValRndA3p = { 1'b0, valIn[31:24] } + 1;
	tValRndA4p = { 1'b0, valIn[39:32] } + 1;
	tValRndA5p = { 1'b0, valIn[47:40] } + 1;
	tValRndA6p = { 1'b0, valIn[55:48] } + 1;
	tValRndA7p = { 1'b0, valIn[63:56] } + 1;
	
	tValRndCp = {
		tValRndA7p[8],	tValRndA6p[8],
		tValRndA5p[8],	tValRndA4p[8],
		tValRndA3p[8],	tValRndA2p[8],
		tValRndA1p[8],	tValRndA0p[8]	};

	casez( { tValRndCp, mode } )
		10'bzzzzzzzz00: tValRndSm=16'b0000_0000_0000_0000;
		10'bzzzzzzz001: tValRndSm=16'b0000_0000_0000_0001;
		10'bzzzzzz0101: tValRndSm=16'b0000_0000_0000_0101;
		10'bzzzzz01101: tValRndSm=16'b0000_0000_0001_0101;
		10'bzzzz011101: tValRndSm=16'b0000_0000_0101_0101;
		10'bzzz0111101: tValRndSm=16'b0000_0001_0101_0101;
		10'bzz01111101: tValRndSm=16'b0000_0101_0101_0101;
		10'bz011111101: tValRndSm=16'b0001_0101_0101_0101;
		10'b0111111101: tValRndSm=16'b0101_0101_0101_0101;
		10'b1111111101: tValRndSm=16'b1101_0101_0101_0101;
		10'bzzzzzzzz10: tValRndSm=16'b1010_1010_1010_1010;
		10'bzzzzzzzz11: tValRndSm=16'b1111_1010_1010_1010;
	endcase
	
	case(tValRndSm[1:0])
		2'b00:		tValOut[7:0]=valIn[7:0];
		2'b01:		tValOut[7:0]=tValRndA0p[7:0];
		default:	tValOut[7:0]=8'h00;
	endcase
	case(tValRndSm[3:2])
		2'b00:		tValOut[15:8]=valIn[15:8];
		2'b01:		tValOut[15:8]=tValRndA1p[7:0];
		default:	tValOut[15:8]=8'h00;
	endcase
	case(tValRndSm[5:4])
		2'b00:		tValOut[23:16]=valIn[23:16];
		2'b01:		tValOut[23:16]=tValRndA2p[7:0];
		default:	tValOut[23:16]=8'h00;
	endcase
	case(tValRndSm[7:6])
		2'b00:		tValOut[31:24]=valIn[31:24];
		2'b01:		tValOut[31:24]=tValRndA3p[7:0];
		default:	tValOut[31:24]=8'h00;
	endcase
	case(tValRndSm[9:8])
		2'b00:		tValOut[39:32]=valIn[39:32];
		2'b01:		tValOut[39:32]=tValRndA4p[7:0];
		default:	tValOut[39:32]=8'h00;
	endcase
	case(tValRndSm[11:10])
		2'b00:		tValOut[47:40]=valIn[47:40];
		2'b01:		tValOut[47:40]=tValRndA5p[7:0];
		default:	tValOut[47:40]=8'h00;
	endcase

	case(tValRndSm[13:12])
		2'b00:	tValOut[55:48]=valIn[55:48];
		2'b01:	tValOut[55:48]=tValRndA6p[7:0];
		2'b10:	tValOut[55:48]=8'h00;
		2'b11:	tValOut[55:48]=8'hF0;
	endcase

	case(tValRndSm[15:14])
		2'b00:	tValOut[63:56]=valIn[63:56];
		2'b01:	tValOut[63:56]=tValRndA7p[7:0];
		2'b10:	tValOut[63:56]=8'h00;
		2'b11:	tValOut[63:56]={ valIn[63], 7'h7F };
	endcase
end

endmodule

`endif
