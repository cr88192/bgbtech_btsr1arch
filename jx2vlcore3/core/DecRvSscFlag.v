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
assign		istrFlag = tIstrFlag;

always @*
begin
	tIstrFlag	= 0;

	casez(istrWord[6:2])

		5'b00_000: begin /* LOAD, (Rm, Disp) */
			tIstrFlag	= 4'b0011;
			if(istrWord[14:12]==3'b111)
				tIstrFlag	= 4'b0000;
		end

		5'b01_000: begin /* STORE, (Rm, Disp) */
			tIstrFlag	= 4'b0001;
			if(istrWord[14:12]==3'b111)
				tIstrFlag	= 4'b0000;
		end

		5'b10_000: begin /* FMADD */
			tIstrFlag	= 4'b0001;
//			if(istrWord[14:12]==3'b111)
//				tIstrFlag	= 4'b0000;
		end

		5'b11_000: begin /* BRANCH */
			tIstrFlag	= 4'b0000;
//			if(istrWord[14:12]==3'b111)
//				tIstrFlag	= 4'b0000;
		end

		5'b00_001: begin /* FP-LOAD, (Rm, Disp) */
			tIstrFlag	= 4'b0011;
		end

		5'b01_001: begin /* FP-STORE, (Rm, Disp) */
			tIstrFlag	= 4'b0001;
		end

		5'b10_001: begin /* FMSUB */
			tIstrFlag	= 4'b0001;
		end

		5'b11_001: begin /* JALR */
			tIstrFlag	= 4'b0000;
		end

		5'b10_010: begin /* FNMSUB */
			tIstrFlag	= 4'b0001;
		end

		5'b00_011: begin /* FENCE */
			tIstrFlag	= 4'b0000;
		end
		5'b01_011: begin /* ATOMIC */
			tIstrFlag	= 4'b0000;
		end

		5'b10_011: begin /* FNMADD */
			tIstrFlag	= 4'b0001;
		end

		5'b11_011: begin /* JAL */
			tIstrFlag	= 4'b0000;
		end


		5'b00_100: begin /* ALU OP, 3RI */
			tIstrFlag	= 4'b1111;
			if(istrWord[14:13]==2'b01)
				tIstrFlag	= 4'b0011;
		end

		5'b01_100: begin /* ALU OP, 3R */
			tIstrFlag	= 4'b0000;
			if(istrWord[29:25]==5'h00)
			begin
				tIstrFlag	= 4'b1111;
				if(istrWord[14:13]==2'b01)
					tIstrFlag	= 4'b0011;
			end
		end

		5'b10_100: begin /* FPU OP, 3R */
			tIstrFlag	= 4'b0011;
		end

		5'b11_100: begin /* SYSTEM */
			tIstrFlag	= 4'b0000;
		end

		5'b00_101: begin /* AUIPC */
			tIstrFlag	= 4'b0000;
		end

		5'b01_101: begin /* LUI */
//			tIstrFlag	= 4'b1111;
			tIstrFlag	= 4'b0011;
		end


		5'b00_110: begin /* ALU OP, 3RI */
			tIstrFlag	= 4'b1111;
			if(istrWord[14:13]==2'b01)
				tIstrFlag	= 4'b0011;
		end

		5'b01_110: begin /* ALU OP, 3R */
			tIstrFlag	= 4'b0000;
			if(istrWord[29:25]==5'h00)
			begin
				tIstrFlag	= 4'b1111;
				if(istrWord[14:13]==2'b01)
					tIstrFlag	= 4'b0011;
			end
		end

		default: begin
			tIstrFlag	= 4'b0000;
		end

	endcase

	if(istrWord[1:0]!=2'b11)
		tIstrFlag	= 4'b0000;

end

endmodule
