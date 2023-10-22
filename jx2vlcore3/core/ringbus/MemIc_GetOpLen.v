module MemIc_GetOpLen(istrBits, opLen, isXG2, isRV);
input[15:0]		istrBits;
output[3:0]		opLen;
input			isXG2;
input			isRV;

reg[3:0]		tOpLen;
assign		opLen = tOpLen;

reg[3:0]		opLenA0;

always @*
begin
`ifndef def_true
	casez(istrBits[12:9])
		4'b1111:	opLenA0=4'b1110;		//FE/FF
		4'b1110:	opLenA0=4'b0110;		//FC/FD
		4'b110z:	opLenA0=4'b0010;		//F8..FB
		4'b101z:	opLenA0=4'b0110;		//F4..F7
		4'b100z:	opLenA0=4'b0010;		//F0..F3
		4'b0111:	opLenA0=4'b0110;		//FE/FF
		4'b0110:	opLenA0=4'b0010;		//FC/FD
		4'b0101:	opLenA0=4'b0110;		//EA/EB
		4'b0100:	opLenA0=4'b0010;		//E8/E9
		4'b00zz:	opLenA0=4'b0010;		//E0..E7
	endcase

	casez({isXG2, istrBits[15:11]})
		6'b1zzzzz: begin end
		6'b0111zz: begin end
		6'b001110:	opLenA0 = 4'b0010;
		6'b001111:	opLenA0 = 4'b0110;
		6'b010010:	opLenA0 = 4'b0010;
		6'b010011:	opLenA0 = 4'b0110;
		default:	opLenA0 = 4'b0001;
	endcase
`endif

`ifdef def_true

	casez({isXG2, istrBits[15:9]})
		8'b1_zzz1_111:	opLenA0 = 4'b1110;		//FE/FF
		8'b1_zzz1_110:	opLenA0 = 4'b0110;		//FC/FD
		8'b1_zzz1_10z:	opLenA0 = 4'b0010;		//F8..FB
		8'b1_zzz1_01z:	opLenA0 = 4'b0110;		//F4..F7
		8'b1_zzz1_00z:	opLenA0 = 4'b0010;		//F0..F3
		8'b1_zzz0_111:	opLenA0 = 4'b0110;		//FE/FF
		8'b1_zzz0_110:	opLenA0 = 4'b0010;		//FC/FD
		8'b1_zzz0_101:	opLenA0 = 4'b0110;		//EA/EB
		8'b1_zzz0_100:	opLenA0 = 4'b0010;		//E8/E9
		8'b1_zzz0_0zz:	opLenA0 = 4'b0010;		//E0..E7

		8'b0_1111_111:	opLenA0 = 4'b1110;		//FE/FF
		8'b0_1111_110:	opLenA0 = 4'b0110;		//FC/FD
		8'b0_1111_10z:	opLenA0 = 4'b0010;		//F8..FB
		8'b0_1111_01z:	opLenA0 = 4'b0110;		//F4..F7
		8'b0_1111_00z:	opLenA0 = 4'b0010;		//F0..F3
		8'b0_1110_111:	opLenA0 = 4'b0110;		//FE/FF
		8'b0_1110_110:	opLenA0 = 4'b0010;		//FC/FD
		8'b0_1110_101:	opLenA0 = 4'b0110;		//EA/EB
		8'b0_1110_100:	opLenA0 = 4'b0010;		//E8/E9
		8'b0_1110_0zz:	opLenA0 = 4'b0010;		//E0..E7

		8'b0_1001_1zz:	opLenA0 = 4'b0110;
		8'b0_1001_0zz:	opLenA0 = 4'b0010;

		8'b0_0111_1zz:	opLenA0 = 4'b0110;
		8'b0_0111_0zz:	opLenA0 = 4'b0010;

		default:		opLenA0 = 4'b0001;
	endcase
`endif

`ifdef jx2_enable_riscv
	if(isRV)
	begin
		opLenA0=4'b0010;
		if(istrBits[ 1: 0]!=2'b11)
			opLenA0=4'b0001;
	end
`endif

	tOpLen = opLenA0;
end

endmodule
