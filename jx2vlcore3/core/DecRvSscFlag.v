/*
 * RISC-V Superscalar Flags
 * (0): Can Run in Lane 1 (with another Op in 2)
 * (1): Can Run in Lane 1 (with another Op in 3)
 * (2): Can Run in Lane 2
 * (3): Can Run in Lane 3
 */

module DecRvSscFlag(istrWord, istrFlag);
input[31:0]		istrWord;
output[3:0]		istrFlag;

reg[3:0]		tIstrFlag;
assign		istrFlag = 0;

always @*
begin
	tIstrFlag	= 0;

	casez(istrWord[6:2])

		5'b00_000: begin /* LOAD, (Rm, Disp) */
			tIstrFlag	= 4'b0011;
		end

		5'b01_000: begin /* STORE, (Rm, Disp) */
			tIstrFlag	= 4'b0001;
		end

		5'b11_000: begin /* BRANCH */
			tIstrFlag	= 4'b0000;
		end


		5'b11_001: begin /* JALR */
			tIstrFlag	= 4'b0000;
		end

		5'b11_011: begin /* JAL */
			tIstrFlag	= 4'b0000;
		end


		5'b00_100: begin /* ALU OP, 3RI */
			tIstrFlag	= 4'b1111;
		end

		5'b01_100: begin /* ALU OP, 3R */
			tIstrFlag	= 4'b0000;
			if(istrWord[29:25]==6'h00)
				tIstrFlag	= 4'b1111;
		end

		5'b00_101: begin /* AUIPC */
			tIstrFlag	= 4'b0000;
		end

		5'b01_101: begin /* LUI */
			tIstrFlag	= 4'b1111;
		end


		5'b00_110: begin /* ALU OP, 3RI */
			tIstrFlag	= 4'b1111;
		end

		5'b01_110: begin /* ALU OP, 3R */
			tIstrFlag	= 4'b0000;
			if(istrWord[29:25]==6'h00)
				tIstrFlag	= 4'b1111;
		end

		default: begin
			tIstrFlag	= 4'b0000;
		end

	endcase

	if(istrWord[1:0]!=2'b11)
		tIstrFlag	= 4'b0000;

end

endmodule
