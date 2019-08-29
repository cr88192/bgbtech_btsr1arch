/*
Text Mode Memory

Screen Memory is 32kB, and is organized into 256-bit cells.

Also has a 2kB fixed font.
 */

module ModTxtMemW(clock, reset,
	pixCellIx,	cellData,
	fontGlyph,	fontData,
	ctrlRegVal,
	busAddr,
	busInData,
	busOutData,
	busOE, busWR, busOK);

/* verilator lint_off UNUSED */

input clock;
input reset;

input[13:0]		pixCellIx;
output[255:0]	cellData;

input[15:0]		fontGlyph;
output[63:0]	fontData;

output[63:0]	ctrlRegVal;

input[31:0]		busAddr;
input[31:0]		busInData;
output[31:0]	busOutData;
input			busOE;
input			busWR;
output[1:0]		busOK;

reg[1:0]	tBusOK;				//Read OK State
reg[31:0] 	tBusData;			//Output Data
wire		tBusCSel;			//Bus Chip-Select (Addr Matches)

// assign		busOK = (busOE && tBusCSel) ? tBusOK : 1'bZ;
// assign		busData = (busOE && tBusCSel) ? tBusData : 32'hZZZZ_ZZZZ;
assign		busOK = tBusOK;
assign		busOutData = tBusData;
assign		tBusCSel =
		(busAddr[27:16]==12'h00A) ||
		(busAddr[27:16]==12'h00B);

wire[31:0]	busData;
assign		busData = busInData;

reg[63:0]	tCtrlRegVal;
assign		ctrlRegVal = tCtrlRegVal;

reg[13:0]	tPixCellIx;			//base cell index
reg[13:0]	nxtPixCellIx;			//base cell index

`ifdef FBUF_EN128K
(* ram_style="block" *) reg[31:0]	scrCell1A[0:4095];
(* ram_style="block" *) reg[31:0]	scrCell1B[0:4095];
(* ram_style="block" *) reg[31:0]	scrCell1C[0:4095];
(* ram_style="block" *) reg[31:0]	scrCell1D[0:4095];
(* ram_style="block" *) reg[31:0]	scrCell1E[0:4095];
(* ram_style="block" *) reg[31:0]	scrCell1F[0:4095];
(* ram_style="block" *) reg[31:0]	scrCell1G[0:4095];
(* ram_style="block" *) reg[31:0]	scrCell1H[0:4095];
`else
`ifdef FBUF_EN64K
(* ram_style="block" *) reg[31:0]	scrCell1A[0:2047];
(* ram_style="block" *) reg[31:0]	scrCell1B[0:2047];
(* ram_style="block" *) reg[31:0]	scrCell1C[0:2047];
(* ram_style="block" *) reg[31:0]	scrCell1D[0:2047];
(* ram_style="block" *) reg[31:0]	scrCell1E[0:2047];
(* ram_style="block" *) reg[31:0]	scrCell1F[0:2047];
(* ram_style="block" *) reg[31:0]	scrCell1G[0:2047];
(* ram_style="block" *) reg[31:0]	scrCell1H[0:2047];
`else
(* ram_style="block" *) reg[31:0]	scrCell1A[0:1023];
(* ram_style="block" *) reg[31:0]	scrCell1B[0:1023];
(* ram_style="block" *) reg[31:0]	scrCell1C[0:1023];
(* ram_style="block" *) reg[31:0]	scrCell1D[0:1023];
(* ram_style="block" *) reg[31:0]	scrCell1E[0:1023];
(* ram_style="block" *) reg[31:0]	scrCell1F[0:1023];
(* ram_style="block" *) reg[31:0]	scrCell1G[0:1023];
(* ram_style="block" *) reg[31:0]	scrCell1H[0:1023];
`endif
`endif

reg[63:0]	fontMem[255:0];
reg[63:0]	fontGfx1Mem[127:0];
reg[63:0]	fontGfx2Mem[127:0];

// reg[31:0]	scrRegCtrl[7:0];	//Control Registers

reg[31:0]	scrRegCtrl0;	//Control Registers
reg[31:0]	scrRegCtrl1;	//Control Registers
reg[31:0]	scrRegCtrl2;	//Control Registers
reg[31:0]	scrRegCtrl3;	//Control Registers
reg[31:0]	scrRegCtrl4;	//Control Registers
reg[31:0]	scrRegCtrl5;	//Control Registers
reg[31:0]	scrRegCtrl6;	//Control Registers
reg[31:0]	scrRegCtrl7;	//Control Registers


reg[255:0]	tCell1;
reg[255:0]	tNextCell1;

reg[15:0]	tFontGlyph;
reg[63:0]	tFontData1;
reg[63:0]	tFontData2;
reg[63:0]	tFontDataAsc1;
reg[63:0]	tFontDataGfx1;
reg[63:0]	tFontDataGfx2;


assign cellData = tCell1;
assign fontData = tFontData1;

initial begin
	$readmemh("fontmem.txt", fontMem);
	$readmemh("gfxfont0.txt", fontGfx1Mem);
	$readmemh("gfxfont1.txt", fontGfx2Mem);
//	$readmemh("scrmem_1a.txt", scrCell1A);
//	$readmemh("scrmem_1b.txt", scrCell1B);
//	$readmemh("scrmem_1c.txt", scrCell1C);
//	$readmemh("scrmem_1d.txt", scrCell1D);
end

always @*
begin
	nxtPixCellIx	= pixCellIx;

	tCtrlRegVal = { scrRegCtrl1, scrRegCtrl0 };

	tBusOK = 0;
	tBusData = 0;
	if(busOE && tBusCSel)
	begin
//		nxtPixCellIx	= busAddr[15:2];
		nxtPixCellIx	= busAddr[16:3];

//		if(busAddr[15:8]==255)
		if(busAddr[16:8]==9'h1FF)
		begin
			tBusOK=1;
			case(busAddr[6:2])
//				0: tBusData = scrRegCtrl[0];
//				1: tBusData = scrRegCtrl[1];
//				2: tBusData = scrRegCtrl[2];
//				3: tBusData = scrRegCtrl[3];

				0: tBusData = scrRegCtrl0;
				1: tBusData = scrRegCtrl1;
				2: tBusData = scrRegCtrl2;
				3: tBusData = scrRegCtrl3;

				8: tBusData = {18'h0, pixCellIx};

			endcase
		end
		else
		begin
//			tBusOK = tPixCellIx[11:0] == busAddr[15:4];
//			case(busAddr[3:2])
//				2'b00: tBusData = tCell1[ 31: 0];
//				2'b01: tBusData = tCell1[ 63:32];
//				2'b10: tBusData = tCell1[ 95:64];
//				2'b11: tBusData = tCell1[127:96];
//			endcase

			tBusOK = (tPixCellIx[11:0] == busAddr[16:5]) ? 2'b01 : 2'b10;
			case(busAddr[4:2])
				3'b000: tBusData = tCell1[ 31:  0];
				3'b001: tBusData = tCell1[ 63: 32];
				3'b010: tBusData = tCell1[ 95: 64];
				3'b011: tBusData = tCell1[127: 96];
				3'b100: tBusData = tCell1[159:128];
				3'b101: tBusData = tCell1[191:160];
				3'b110: tBusData = tCell1[223:192];
				3'b111: tBusData = tCell1[255:224];
			endcase
		end
	end

//	tFontDataAsc1 = fontMem[tFontGlyph[7:0]];
//	tFontDataGfx1 = fontGfx1Mem[tFontGlyph[6:0]];
//	tFontDataGfx2 = fontGfx2Mem[tFontGlyph[6:0]];
	case(tFontGlyph[9:7])
	3'b000:	tFontData2 = tFontDataAsc1;
	3'b001:	tFontData2 = tFontDataAsc1;
	3'b010:	tFontData2 = tFontDataGfx2;
	3'b011:	tFontData2 = tFontDataGfx1;
//	3'b100:	tFontData2 = tFontDataAsc1;
//	3'b101:	tFontData2 = tFontDataGfx2;
	default: tFontData2 = tFontDataAsc1;
	endcase

end

always @ (posedge clock)
begin
//	tCell1			<= tNextCell1;
//	tPixCellIx		<= pixCellIx;

	tPixCellIx		<= nxtPixCellIx;

`ifdef FBUF_EN128K
	tCell1[ 31:  0]	<= scrCell1A[tPixCellIx[11:0]];
	tCell1[ 63: 32]	<= scrCell1B[tPixCellIx[11:0]];
	tCell1[ 95: 64]	<= scrCell1C[tPixCellIx[11:0]];
	tCell1[127: 96]	<= scrCell1D[tPixCellIx[11:0]];
	tCell1[159:128]	<= scrCell1E[tPixCellIx[11:0]];
	tCell1[191:160]	<= scrCell1F[tPixCellIx[11:0]];
	tCell1[223:192]	<= scrCell1G[tPixCellIx[11:0]];
	tCell1[255:224]	<= scrCell1H[tPixCellIx[11:0]];
`else
`ifdef FBUF_EN64K
	tCell1[ 31:  0]	<= scrCell1A[tPixCellIx[10:0]];
	tCell1[ 63: 32]	<= scrCell1B[tPixCellIx[10:0]];
	tCell1[ 95: 64]	<= scrCell1C[tPixCellIx[10:0]];
	tCell1[127: 96]	<= scrCell1D[tPixCellIx[10:0]];
	tCell1[159:128]	<= scrCell1E[tPixCellIx[10:0]];
	tCell1[191:160]	<= scrCell1F[tPixCellIx[10:0]];
	tCell1[223:192]	<= scrCell1G[tPixCellIx[10:0]];
	tCell1[255:224]	<= scrCell1H[tPixCellIx[10:0]];
`else
	tCell1[ 31:  0]	<= scrCell1A[tPixCellIx[9:0]];
	tCell1[ 63: 32]	<= scrCell1B[tPixCellIx[9:0]];
	tCell1[ 95: 64]	<= scrCell1C[tPixCellIx[9:0]];
	tCell1[127: 96]	<= scrCell1D[tPixCellIx[9:0]];
	tCell1[159:128]	<= scrCell1E[tPixCellIx[9:0]];
	tCell1[191:160]	<= scrCell1F[tPixCellIx[9:0]];
	tCell1[223:192]	<= scrCell1G[tPixCellIx[9:0]];
	tCell1[255:224]	<= scrCell1H[tPixCellIx[9:0]];
`endif
`endif

	tFontDataAsc1	<= fontMem[tFontGlyph[7:0]];
	tFontDataGfx1	<= fontGfx1Mem[tFontGlyph[6:0]];
	tFontDataGfx2	<= fontGfx2Mem[tFontGlyph[6:0]];
	
	tFontGlyph		<= fontGlyph;
	tFontData1		<= tFontData2;

	if(tBusCSel && busOE)
//		tPixCellIx		<= busAddr[15:2];
		tPixCellIx		<= busAddr[18:5];
	else
		tPixCellIx		<= pixCellIx;

	if(tBusCSel && busWR && !busOE)
	begin
//		$display("GfxMem A=%X V=%X", busAddr, busData);
	
		if(busAddr[16:8]==9'h1FF)
		begin
//			scrRegCtrl[busAddr[4:2]] <= busData;
			case(busAddr[4:2])
				3'b000: scrRegCtrl0	<= busData;
				3'b001: scrRegCtrl1	<= busData;
				3'b010: scrRegCtrl2	<= busData;
				3'b011: scrRegCtrl3	<= busData;
				3'b100: scrRegCtrl4	<= busData;
				3'b101: scrRegCtrl5	<= busData;
				3'b110: scrRegCtrl6	<= busData;
				3'b111: scrRegCtrl7	<= busData;
			endcase
		end
		else
		begin
//			case(busAddr[3:2])
//				2'b00: scrCell1A[busAddr[13:4]] <= busData;
//				2'b01: scrCell1B[busAddr[13:4]] <= busData;
//				2'b10: scrCell1C[busAddr[13:4]] <= busData;
//				2'b11: scrCell1D[busAddr[13:4]] <= busData;
//			endcase

`ifdef FBUF_EN128K
			case(busAddr[4:2])
				3'b000: scrCell1A[busAddr[16:5]] <= busData;
				3'b001: scrCell1B[busAddr[16:5]] <= busData;
				3'b010: scrCell1C[busAddr[16:5]] <= busData;
				3'b011: scrCell1D[busAddr[16:5]] <= busData;
				3'b100: scrCell1E[busAddr[16:5]] <= busData;
				3'b101: scrCell1F[busAddr[16:5]] <= busData;
				3'b110: scrCell1G[busAddr[16:5]] <= busData;
				3'b111: scrCell1H[busAddr[16:5]] <= busData;
			endcase
`else
`ifdef FBUF_EN64K
			case(busAddr[4:2])
				3'b000: scrCell1A[busAddr[15:5]] <= busData;
				3'b001: scrCell1B[busAddr[15:5]] <= busData;
				3'b010: scrCell1C[busAddr[15:5]] <= busData;
				3'b011: scrCell1D[busAddr[15:5]] <= busData;
				3'b100: scrCell1E[busAddr[15:5]] <= busData;
				3'b101: scrCell1F[busAddr[15:5]] <= busData;
				3'b110: scrCell1G[busAddr[15:5]] <= busData;
				3'b111: scrCell1H[busAddr[15:5]] <= busData;
			endcase
`else
			case(busAddr[4:2])
				3'b000: scrCell1A[busAddr[14:5]] <= busData;
				3'b001: scrCell1B[busAddr[14:5]] <= busData;
				3'b010: scrCell1C[busAddr[14:5]] <= busData;
				3'b011: scrCell1D[busAddr[14:5]] <= busData;
				3'b100: scrCell1E[busAddr[14:5]] <= busData;
				3'b101: scrCell1F[busAddr[14:5]] <= busData;
				3'b110: scrCell1G[busAddr[14:5]] <= busData;
				3'b111: scrCell1H[busAddr[14:5]] <= busData;
			endcase
`endif
`endif
		end
	end
end

endmodule
