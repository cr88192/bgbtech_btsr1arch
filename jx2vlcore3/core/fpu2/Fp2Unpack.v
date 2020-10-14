/*
FPU2 Unpacker

Unpack element from a 128-bit input vector.

Pos:
  (1:0): Element Index
  (4:2): Element Size/Type
  (  5): Zero Extend (Int / Word)

Element Type:
  0000: Binary16
  0001: Binary32
  0010: Binary64
  0011: Binary128
  0100: Int16
  0101: Int32
  0110: Int64
  0111: Resv
  
  1100: Word16
  1101: Word32
  1110: Word64
  1111: Resv

 */

module Fp2Unpack(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regVecData,		regVecPos,
	regSign,		regExp,			regFrac
	);

input			clock;
input			reset;

input[127:0]	regVecData;
input[  5:0]	regVecPos;

output			regSign;
output[15:0]	regExp;
output[71:0]	regFrac;

reg				tRegSign;
reg[15:0]		tRegExp;
reg[71:0]		tRegFrac;
assign		regSign = tRegSign;
assign		regExp	= tRegExp;
assign		regFrac = tRegFrac;

reg[15:0]		tElem16;
reg[31:0]		tElem32;
reg[63:0]		tElem64;
reg[127:0]		tElem128;

reg				tExpIsZero;
reg				tExpIsNaN;

reg				tSign;
reg[15:0]		tExp;
reg[64:0]		tFrac;
reg				tFracLsb;

always @*
begin
	tSign		= 0;
	tExp		= 0;
	tFrac		= 0;
	tFracLsb	= 0;

	case(regVecPos[1:0])
		2'b00: begin
			tElem16=regVecData[15:0];
			tElem32=regVecData[31:0];
		end
		2'b01: begin
			tElem16=regVecData[ 31:16];
			tElem32=regVecData[ 63:32];
		end
		2'b10: begin
			tElem16=regVecData[47:32];
			tElem32=regVecData[95:64];
		end
		2'b11: begin
			tElem16=regVecData[ 63:48];
			tElem32=regVecData[127:96];
		end
	endcase	
	if(regVecPos[0])
		tElem64=regVecData[127:64];
	else
		tElem64=regVecData[ 63: 0];
	tElem128=regVecData;

	casez(regVecPos[5:2])

		4'b0000: begin
			tExpIsZero = (tElem16[14:10]==5'h00);
			tExpIsNaN  = (tElem16[14:10]==5'h1F);
			tSign	= tElem16[15];
			tExp	= {		1'b0, tElem16[14],
				((!tElem16[14] && !tExpIsZero) || tExpIsNaN) ?
					10'b1111111111 : 10'b0000000000,
				tElem16[13:10] };
			tFrac[64]		= !tExpIsZero;
			tFrac[63:54]	= tElem16[9:0];
		end

		4'b0001: begin
			tExpIsZero = (tElem32[30:23]==8'h00);
			tExpIsNaN  = (tElem32[30:23]==8'hFF);
			tSign	= tElem32[31];
			tExp	= {		1'b0, tElem32[30],
				((!tElem32[30] && !tExpIsZero) || tExpIsNaN) ?
					7'b1111111 : 7'b0000000,
				tElem32[29:23] };
			tFrac[64]		= !tExpIsZero;
			tFrac[63:41]	= tElem32[22:0];
		end

		4'b0010: begin
			tExpIsZero = (tElem64[62:52]==11'h000);
			tExpIsNaN  = (tElem64[62:52]==11'h7FF);
			tSign	= tElem64[63];
			tExp	= {		1'b0, tElem64[62],
				((!tElem64[62] && !tExpIsZero) || tExpIsNaN) ?
					4'b1111 : 4'b0000,
				tElem64[61:52] };
			tFrac[64]		= !tExpIsZero;
			tFrac[63:12]	= tElem64[51:0];
		end

		4'b0011: begin
			tSign			= tElem128[127];
			tExp			= { 1'b0, tElem128[126:112]};
			tFrac[64]		= !tExpIsZero;
			tFrac[63:12]	= tElem64[51:0];
		end

		4'b1000: begin
			tSign			= 0;
			tExp			= 16383;
			tFrac[63:48]	= tElem16[15:0];
		end
		4'b1001: begin
			tSign			= 0;
			tExp			= 16383;
			tFrac[63:32]	= tElem32[31:0];
		end
		4'b1010: begin
			tSign			= 0;
			tExp			= 16383;
			tFrac[63:0]		= tElem64[63:0];
		end

		4'b1011: begin
		end

		4'bz100: begin
			if(regVecPos[5] || !tElem16[15])
			begin
				tSign			= 0;
				tExp			= 16383 + 16;
				tFrac[63:48]	= tElem16[15:0];
			end else begin
				tSign			= 1;
				tExp			= 16383 + 16;
				tFrac[63:48]	= ~tElem16[15:0];
				tFrac[47: 0]	= UV48_FF;
				tFracLsb		= 1;
			end
		end

		4'bz101: begin
			if(regVecPos[5] || !tElem32[31])
			begin
				tSign			= 0;
				tExp			= 16383 + 32;
				tFrac[63:32]	= tElem32[31:0];
			end else begin
				tSign			= 1;
				tExp			= 16383 + 32;
				tFrac[63:32]	= ~tElem32[31:0];
				tFrac[31: 0]	= UV32_FF;
				tFracLsb		= 1;
			end
		end

		4'bz110: begin
			if(regVecPos[5] || !tElem64[63])
			begin
				tSign			= 0;
				tExp			= 16383 + 64;
				tFrac[63:0]		= tElem64[63:0];
			end else begin
				tSign			= 1;
				tExp			= 16383 + 64;
				tFrac[63: 0]	= ~tElem64[63:0];
				tFracLsb		= 1;
			end
		end

		3'bz111: begin
		end

		default: begin
		end

	endcase

	tRegSign	= tSign;
	tRegExp		= tExp;
	tRegFrac	= { 1'b0, tFrac, tFracLsb ? 7'h7F : 7'h00 };
end

endmodule
