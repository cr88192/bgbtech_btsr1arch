module Fp2Pack(
	/* verilator lint_off UNUSED */
	clock,			reset,
	regVecDataIn,	regVecDataOut,	regVecPos,
	regSign,		regExp,			regFrac
	);

input			clock;
input			reset;

input[127:0]	regVecDataIn;
output[127:0]	regVecDataOut;
input[  5:0]	regVecPos;

input			regSign;
input[15:0]		regExp;
input[71:0]		regFrac;

reg[127:0]		tVecDataOut;
reg[127:0]		tVecDataOutB;
assign		regVecDataOut = tVecDataOutB;

reg[ 15:0]		tResult16;
reg[ 31:0]		tResult32;
reg[ 63:0]		tResult64;
reg[127:0]		tResult128;
reg[ 63:0]		tResultI64;

reg[ 15:0]		tResult16B;
reg[ 31:0]		tResult32B;
reg[ 63:0]		tResult64B;
reg[127:0]		tResult128B;
reg[ 63:0]		tResultI64B;

reg[ 15:0]		tResult16B1;
reg[ 31:0]		tResult32B1;
reg[ 63:0]		tResult64B1;
reg[127:0]		tResult128B1;
reg[ 63:0]		tResultI64B1;

reg				tRound16;
reg				tRound32;
reg				tRound64;
reg				tRoundI64;
reg				tRound16B;
reg				tRound32B;
reg				tRound64B;
reg				tRoundI64B;

reg				tFixedFrac;
reg[15:0]		tFixedExp;
reg[15:0]		tDiffExp;

reg				tFixedFrac3;
reg[15:0]		tFixedExp3;
reg[15:0]		tDiffExp3;

reg				tFixedFrac4;
reg[15:0]		tFixedExp4;
reg[15:0]		tDiffExp4;

always @*
begin

//	tSgnC3 = regSign;
//	tExpC3 = regExp;
//	tFraC3 = regFrac;
	
	tFixedFrac = 0;
	tFixedExp = 0;

	casez(regVecPos[5:2])
		4'b00zz: begin
			tFixedFrac = 0;
			tFixedExp = 0;
		end
		4'b10zz: begin
			tFixedFrac = 1;
			tFixedExp = 16383;
		end

		4'bz100: begin
			tFixedFrac = 1;
			tFixedExp = 16383+16;
		end
		4'bz101: begin
			tFixedFrac = 1;
			tFixedExp = 16383+32;
		end
		4'bz110: begin
			tFixedFrac = 1;
			tFixedExp = 16383+64;
		end
		4'bz111: begin
			tFixedFrac = 1;
			tFixedExp = 16383+64;
		end
	endcase

//	tDiffExp = tFixedExp - regExp;
	tDiffExp = tFixedExp + (~regExp);

	/* Stage 3 */
	tSgnC3B	= tSgnC3;
	if(tFixedFrac3)
	begin
		tExpC3B	= tFixedExp3;
		tFraC3B	= 0;
		if(tDiffExp3[14:7]==8'h00)
		begin
			case(tDiffExp3[6:3])
				4'h0:		tFraC3B	= tFraC3    ;
				4'h1:		tFraC3B	= tFraC3>> 8;
				4'h2:		tFraC3B	= tFraC3>>16;
				4'h3:		tFraC3B	= tFraC3>>24;
				4'h4:		tFraC3B	= tFraC3>>32;
				4'h5:		tFraC3B	= tFraC3>>40;
				4'h6:		tFraC3B	= tFraC3>>48;
				4'h7:		tFraC3B	= tFraC3>>56;
				4'h8:		tFraC3B	= tFraC3>>64;
				default:	tFraC3B	= 0;
			end
		end
		else
		if(tDiffExp3[14:7]==8'hFF)
		begin
			case(tDiffExp3[6:3])
				4'hF:		tFraC3B	= tFraC3    ;
				4'hE:		tFraC3B	= tFraC3<< 8;
				4'hD:		tFraC3B	= tFraC3<<16;
				4'hC:		tFraC3B	= tFraC3<<24;
				4'hB:		tFraC3B	= tFraC3<<32;
				4'hA:		tFraC3B	= tFraC3<<40;
				4'h9:		tFraC3B	= tFraC3<<48;
				4'h8:		tFraC3B	= tFraC3<<56;
				4'h7:		tFraC3B	= tFraC3<<64;
				default:	tFraC3B	= 0;
			end
		end
	end
	else
	if(tFraC3[71])
	begin
		tExpC3B	= {1'b0, tExpC3} + 1;
		tFraC3B	= {1'b0, tFraC3[71:1] };
	end
	else
	if(tFraC3[70:62]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} ;
		tFraC3B	= tFraC3 ;
	end
	else if(tFraC3[61:54]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 8;
		tFraC3B	= tFraC3 << 8;
	end
	else if(tFraC3[53:46]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 16;
		tFraC3B	= tFraC3 << 16;
	end
	else if(tFraC3[45:38]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 24;
		tFraC3B	= tFraC3 << 24;
	end
	else if(tFraC3[37:30]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 32;
		tFraC3B	= tFraC3 << 32 ;
	end
	else if(tFraC3[29:22]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 40;
		tFraC3B	= tFraC3 << 40 ;
	end
	else if(tFraC3[21:14]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 48;
		tFraC3B	= tFraC3 << 48 ;
	end
	else if(tFraC3[13:6]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 56;
		tFraC3B	= tFraC3 << 56 ;
	end
	else if(tFraC3[5:0]!=0)
	begin
		tExpC3B	= {1'b0, tExpC3} - 64;
		tFraC3B	= tFraC3 << 64 ;
	end
	else
	begin
		tSgnC3B	= 0;
		tExpC3B	= 0;
		tFraC3B	= 0;
	end
	
	/* Stage 4 */
	if(tFixedFrac4)
	begin
		if(tDiffExp4[14]==1'b0)
		begin
			case(tDiffExp4[2:0])
				3'h0:		tFraC4B	= tFraC4   ;
				3'h1:		tFraC4B	= tFraC4>>1;
				3'h2:		tFraC4B	= tFraC4>>2;
				3'h3:		tFraC4B	= tFraC4>>3;
				3'h4:		tFraC4B	= tFraC4>>4;
				3'h5:		tFraC4B	= tFraC4>>5;
				3'h6:		tFraC4B	= tFraC4>>6;
				3'h7:		tFraC4B	= tFraC4>>7;
			end
		end
		else
		if(tDiffExp4[14]==1'b1)
		begin
			case(tDiffExp4[2:0])
				3'h7:		tFraC4B	= tFraC4   ;
				3'h6:		tFraC4B	= tFraC4<<1;
				3'h5:		tFraC4B	= tFraC4<<2;
				3'h4:		tFraC4B	= tFraC4<<3;
				3'h3:		tFraC4B	= tFraC4<<4;
				3'h2:		tFraC4B	= tFraC4<<5;
				3'h1:		tFraC4B	= tFraC4<<6;
				3'h0:		tFraC4B	= tFraC4<<7;
			end
		end
	end
	else
	begin
		casez(tFraC4[70:63])
			8'b1zzzzzzz: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4;
				tFraC4B	= tFraC4;
			end
			8'b01zzzzzz: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4 - 1;
				tFraC4B	= tFraC4 << 1;
			end
			8'b001zzzzz: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4 - 2;
				tFraC4B	= tFraC4 << 2;
			end
			8'b0001zzzz: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4 - 3;
				tFraC4B	= tFraC4 << 3;
			end
			8'b00001zzz: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4 - 4;
				tFraC4B	= tFraC4 << 4;
			end
			8'b000001zz: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4 - 5;
				tFraC4B	= tFraC4 << 5;
			end
			8'b0000001z: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4 - 6;
				tFraC4B	= tFraC4 << 6;
			end
			8'b00000001: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4 - 7;
				tFraC4B	= tFraC4 << 7;
			end
			8'b00000000: begin
				tSgnC4B	= tSgnC4;
				tExpC4B	= tExpC4 - 8;
				tFraC4B	= tFraC4 << 8;
			end
		endcase
	end
	
	if(tExpC4B[15])
	begin
		tSgnC4B	= 0;
		tExpC4B	= 0;
		tFraC4B	= 0;
	end

	tResult16	=
		{ tSgnC4B, tExpC4B[14], tExpC4B[ 3:0], tFraC4B[69:60]        };
	tResult32	=
		{ tSgnC4B, tExpC4B[14], tExpC4B[ 6:0], tFraC4B[69:47]        };
	tResult64	=
		{ tSgnC4B, tExpC4B[14], tExpC4B[ 9:0], tFraC4B[69:18]        };
	tResult128	=
		{ tSgnC4B, tExpC4B[14], tExpC4B[13:0], tFraC4B[69: 0], 42'h0 };
	tResultI64	= tFraC4B[69: 6];
	tRound16	= tFraC4B[59];
	tRound32	= tFraC4B[46];
	tRound64	= tFraC4B[17];
	tRoundI64	= tFraC4B[ 5];


	tVecDataOut		= regVecDataIn;

	tResult16B1		= tResult16B + tRound16B;
	tResult32B1		= tResult32B + tRound32B;
	tResult64B1		= tResult64B + tRound64B;
	tResultI64B1	= tResultI64B + tRoundI64B;
	tResult128B1	= tResult128;

	casez(regVecPos[5:2])

		4'b0000: begin
			case(regVecPos[1:0])
				2'b00: tVecDataOut[15: 0] = tResult16B1;
				2'b01: tVecDataOut[31:16] = tResult16B1;
				2'b10: tVecDataOut[47:32] = tResult16B1;
				2'b11: tVecDataOut[63:48] = tResult16B1;
			endcase
		end

		4'b0001: begin
			case(regVecPos[1:0])
				2'b00: tVecDataOut[ 31: 0] = tResult32B1;
				2'b01: tVecDataOut[ 63:32] = tResult32B1;
				2'b10: tVecDataOut[ 95:64] = tResult32B1;
				2'b11: tVecDataOut[127:96] = tResult32B1;
			endcase
		end

		4'b0010: begin
			case(regVecPos[1:0])
				2'b00: tVecDataOut[ 63: 0] = tResult64B1;
				2'b01: tVecDataOut[127:64] = tResult64B1;
				2'b10: begin end
				2'b11: begin end
			endcase
		end

		4'b0010: begin
			case(regVecPos[1:0])
				2'b00: tVecDataOut[127: 0] = tResult128B1;
				2'b01: begin end
				2'b10: begin end
				2'b11: begin end
			endcase
		end

	endcase
	
end

always @(posedge clock)
begin
	tVecDataOutB	<= tVecDataOut;

	tSgnC3			<= regSign;
	tExpC3			<= regExp;
	tFraC3			<= regFrac;

	tFixedFrac3		<= tFixedFrac;
	tFixedExp3		<= tFixedExp;
	tDiffExp3		<= tDiffExp;
	tFixedFrac4		<= tFixedFrac3;
	tFixedExp4		<= tFixedExp3;
	tDiffExp4		<= tDiffExp3;

	tResult16B		<= tResult16;
	tResult32B		<= tResult32;
	tResult64B		<= tResult64;
	tResultI64B		<= tResultI64;
	tResult128B		<= tResult128;
	tRound16B		<= tRound16;
	tRound32B		<= tRound32;
	tRound64B		<= tRound64;
	tRoundI64B		<= tRoundI64;

end

endmodule
