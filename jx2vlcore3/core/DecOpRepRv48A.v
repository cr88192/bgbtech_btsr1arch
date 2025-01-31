/*
Repack 48-bit RISC-V instructions to RV Jumbo 64 Encodings.

* ww-zzz-sssss-iiiiii iiiiiiii-iii-iiiii yyy1-nnnnn-0011111 
  OP Rn, Rs, Imm22s
* ww-zzz-sssss-iiiiii iiiiiiii-iii-ttttt yyy1-iiiii-0011111 
  OP Rs, Rt, Imm22s (St or Bcc)

Say: ww-yyy:
  00-000: Ld Disp22
  01-000: St Disp22
  10-000: St Disp22
  11-000: Bcc Disp22
  00-100: ALUI Imm22
  00-110: ALUWI Imm22

  11-011: JAL Disp30
  00-101: AUIPC Disp30
 */

module DecOpRepRv48A(
	/* verilator lint_off UNUSED */
	clock,
	istrWordIn,	istrWordOut,
	ifPcStepIn, ifPcStepOut,
	istrMTagOut, isRv, isFlush
	);

input			clock;
input[63:0]		istrWordIn;
output[63:0]	istrWordOut;
output[1:0]		istrMTagOut;
input[3:0]		ifPcStepIn;
output[3:0]		ifPcStepOut;
input			isRv;
input			isFlush;

reg[63:0]		tWordOut;
assign		istrWordOut = tWordOut;

reg[1:0]		tWordMTag;
assign		istrMTagOut = tWordMTag;

reg[3:0]		tPcStepOut;
assign		ifPcStepOut = tPcStepOut;

reg[31:0]		tWordRepRVJ;
reg[31:0]		tWordRepRVB;

reg[31:0]		tWordSel;
reg[1:0]		tWordSelIx;
reg				tPSelBit;

reg				tIs48;
reg				tImmSExt;

always @*
begin
	tWordOut	= istrWordIn;
	tPcStepOut	= ifPcStepIn;
	tWordMTag = 0;
	
//	tIs48 = (ifPcStepIn == 4'b0110);
	tIs48 = (istrWordIn[5:0] == 6'h1F);
	tImmSExt	= istrWordIn[37];
	
	tWordRepRVJ = {
		1'b0,
		tImmSExt,
		istrWordIn[36:27],
		tImmSExt ? 5'b11111 : 5'b00000,
		3'b100,
		tImmSExt ? 5'b11111 : 5'b00000,
		7'h3F
	};
	tWordRepRVB = {
		tImmSExt,
		istrWordIn[26:16],
		istrWordIn[42:38],
		istrWordIn[45:43],
		istrWordIn[11: 7],
		istrWordIn[47:46],
		istrWordIn[15:13],
		2'b11
	};
	
	if(isRv && tIs48)
	begin
		if((istrWordIn[6:5]==2'b00) && istrWordIn[12])
		begin
			tWordOut[31: 0] = tWordRepRVJ;
			tWordOut[63:32] = tWordRepRVB;
			tPcStepOut		= 4'b1000;
		end
	end
end

endmodule
