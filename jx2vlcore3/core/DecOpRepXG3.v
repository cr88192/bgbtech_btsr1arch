/*
Repackage instructions from XG3RV to XG2.
 */

module DecOpRepXG3(
	/* verilator lint_off UNUSED */
	clock,
	istrWordIn,	istrWordOut,
	istrMTagOut, isXG3, isFlush
	);

input			clock;
input[31:0]		istrWordIn;
output[31:0]	istrWordOut;
output[1:0]		istrMTagOut;
input			isXG3;
input			isFlush;

reg[31:0]		tWordOut;
assign		istrWordOut = tWordOut;

reg[1:0]		tWordMTag;
assign		istrMTagOut = tWordMTag;

reg[31:0]		tWordRepF0;
reg[31:0]		tWordRepF1;
reg[31:0]		tWordRepF8;
reg[31:0]		tWordRepFE;

reg[31:0]		tWordSel;
reg[1:0]		tWordSelIx;
reg				tPSelBit;

reg[31:0]		tIstrWordL;

// reg				tIs1R;

always @*
begin
	tWordOut = istrWordIn;
	tWordMTag = 0;
	
	tPSelBit	= (istrWordIn[ 4: 3] == 2'b11);
	if(!istrWordIn[1])
		tPSelBit = istrWordIn[0];
	
//	tIs1R	=
//		(istrWordIn[31:28]==4'b0000) &&
//		(istrWordIn[15:12]==4'b0011) &&
//		(istrWordIn[4:2] == 3'b000) ;
	
	tWordRepF0 = {
		istrWordIn[15:12],
		istrWordIn[    5],
		istrWordIn[   10],
		istrWordIn[   20],
		istrWordIn[   26],
		istrWordIn[25:22],
		istrWordIn[31:28],

		!istrWordIn[  11],
		!istrWordIn[  21],
		!istrWordIn[  27],
		istrWordIn[    1],
		istrWordIn[    4],
//		istrWordIn[ 4: 3] == 2'b11,
		tPSelBit,
		istrWordIn[ 3: 2],
		istrWordIn[ 9: 6],
		istrWordIn[19:16]
	};

	tWordRepF1 = {
		istrWordIn[15:12],
		istrWordIn[    5],
		istrWordIn[   10],
		istrWordIn[   20],
		istrWordIn[30:27],
		istrWordIn[   26],
		istrWordIn[25:22],

		!istrWordIn[  11],
		!istrWordIn[  21],
		!istrWordIn[  31],
		istrWordIn[    1],
		istrWordIn[    4],
//		istrWordIn[ 4: 3] == 2'b11,
		tPSelBit,
		istrWordIn[ 3: 2],
		istrWordIn[ 9: 6],
		istrWordIn[19:16]
	};

	tWordRepF8 = {
		istrWordIn[31:16],

		!istrWordIn[  11],
		!istrWordIn[  15],
		!istrWordIn[   5],
		istrWordIn[    1],
		istrWordIn[    4],
//		istrWordIn[ 4: 3] == 2'b11,
		tPSelBit,
		istrWordIn[ 3: 2],

		istrWordIn[14:12],
		istrWordIn[   10],
		istrWordIn[ 9: 6]
		
	};

	tWordRepFE = {
		istrWordIn[31:16],

		!istrWordIn[   7],
		!istrWordIn[   6],
		!istrWordIn[   5],
		istrWordIn[    1],

		istrWordIn[    4],
//		istrWordIn[ 4: 3] == 2'b11,
		tPSelBit,
		istrWordIn[ 3: 2],
		istrWordIn[15: 8]
	};
	
	case(istrWordIn[4:2])
		3'b000: tWordSelIx=0;
		3'b001: tWordSelIx=1;
		3'b010: tWordSelIx=1;
		3'b011: tWordSelIx=0;
		3'b100: tWordSelIx=2;
		3'b101: tWordSelIx=0;
		3'b110: tWordSelIx=3;
		3'b111: tWordSelIx=3;
	endcase
	
	case(tWordSelIx)
		2'b00: tWordSel=tWordRepF0;
		2'b01: tWordSel=tWordRepF1;
		2'b10: tWordSel=tWordRepF8;
		2'b11: tWordSel=tWordRepFE;
	endcase
	
	if(isXG3 && (istrWordIn[ 1: 0] != 2'b11))
	begin
		tWordOut = tWordSel;
		tWordMTag = 1;
`ifndef def_true
		if(tIstrWordL != istrWordIn)
		begin
//			$display("DecOpRepXG3: %X -> %X", istrWordIn, tWordOut);
			$display("DecOpRepXG3: %b-%b-%b-%b-%b-%b -> %X-%X",
				istrWordIn[31:28],
				istrWordIn[27:22],
				istrWordIn[21:16],
				istrWordIn[15:12],
				istrWordIn[11: 6],
				istrWordIn[ 5: 0],
				tWordOut[15:0], tWordOut[31:16]);
		end
`endif
	end
	
	if(isFlush)
	begin
//		tWordOut = 32'h3030_F000;
		tWordOut = 32'h3000_F000;
//		tWordOut = 32'h3000F013;
//		tWordOut = 32'h5800F013;
		tWordMTag = 1;
	end
end

always @(clock)
begin
	tIstrWordL	<= istrWordIn;
end

endmodule
