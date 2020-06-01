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
	busOE, busWR, busQW, busOK);

/* verilator lint_off UNUSED */

input clock;
input reset;

input[13:0]		pixCellIx;
output[255:0]	cellData;

input[15:0]		fontGlyph;
output[63:0]	fontData;

output[63:0]	ctrlRegVal;

input[31:0]		busAddr;
// input[31:0]		busInData;
// output[31:0]	busOutData;
input[63:0]		busInData;
output[63:0]	busOutData;
input			busOE;
input			busWR;
input			busQW;
output[1:0]		busOK;

reg[1:0]	tBusOK;				//Read OK State
// reg[31:0] 	tBusData;			//Output Data
reg[63:0] 	tBusData;			//Output Data
wire		tBusCSel;			//Bus Chip-Select (Addr Matches)

reg[1:0]	tBusOK2;			//Read OK State
reg[63:0] 	tBusData2;			//Output Data

// assign		busOK = (busOE && tBusCSel) ? tBusOK : 1'bZ;
// assign		busData = (busOE && tBusCSel) ? tBusData : 32'hZZZZ_ZZZZ;
assign		busOK = tBusOK2;
assign		busOutData = tBusData2;
assign		tBusCSel =
		(busAddr[27:16]==12'h00A) ||
		(busAddr[27:16]==12'h00B);

// wire[31:0]	busData;
wire[63:0]	busData;
assign		busData = busInData;

wire[31:0]	busDataInA;
wire[31:0]	busDataInB;
assign	busDataInA = busInData[31:0];
assign	busDataInB = busQW ? busInData[63:32] : busInData[31:0];

wire		busDataAdA;
wire		busDataAdB;
assign	busDataAdA = busQW || !busAddr[2];
assign	busDataAdB = busQW ||  busAddr[2];

wire		busDataStA;
wire		busDataStB;
wire		busDataStC;
wire		busDataStD;
wire		busDataStE;
wire		busDataStF;
wire		busDataStG;
wire		busDataStH;
assign	busDataStA = busDataAdA && (busAddr[4:3]==0);
assign	busDataStB = busDataAdB && (busAddr[4:3]==0);
assign	busDataStC = busDataAdA && (busAddr[4:3]==1);
assign	busDataStD = busDataAdB && (busAddr[4:3]==1);
assign	busDataStE = busDataAdA && (busAddr[4:3]==2);
assign	busDataStF = busDataAdB && (busAddr[4:3]==2);
assign	busDataStG = busDataAdA && (busAddr[4:3]==3);
assign	busDataStH = busDataAdB && (busAddr[4:3]==3);

reg[63:0]	tCtrlRegVal;
assign		ctrlRegVal = tCtrlRegVal;

reg[13:0]	tPixCellIx;			//base cell index
reg[13:0]	tPixCellIx2;		//base cell index (last cycle)
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

reg[15:0]	fontRamA[511:0];
reg[15:0]	fontRamB[511:0];
reg[15:0]	fontRamC[511:0];
reg[15:0]	fontRamD[511:0];

// reg[31:0]	scrRegCtrl[7:0];	//Control Registers

reg[31:0]	scrRegCtrl0;	//Control Registers
reg[31:0]	scrRegCtrl1;	//Control Registers
reg[31:0]	scrRegCtrl2;	//Control Registers
reg[31:0]	scrRegCtrl3;	//Control Registers
reg[31:0]	scrRegCtrl4;	//Control Registers
reg[31:0]	scrRegCtrl5;	//Control Registers
reg[31:0]	scrRegCtrl6;	//Control Registers
reg[31:0]	scrRegCtrl7;	//Control Registers

reg[31:0]	scrRegCtrl9;	//CR9, Secondary Boot Vector


reg[255:0]	tCell1;
reg[255:0]	tNextCell1;

reg[15:0]	tFontGlyph;
reg[63:0]	tFontData1;
reg[63:0]	tFontData2;
reg[63:0]	tFontDataAsc1;
reg[63:0]	tFontDataGfx1;
reg[63:0]	tFontDataGfx2;
reg[63:0]	tFontDataRam;

`ifdef FBUF_EN128K
reg[11:0]	tBusCellIx;			//base cell index
`else
`ifdef FBUF_EN64K
reg[10:0]	tBusCellIx;			//base cell index
`else
reg[9:0]	tBusCellIx;			//base cell index
`endif
`endif


wire[255:0]		tCellData;
reg[255:0]		tCellDataL;

assign		tCellData = (pixCellIx==tPixCellIx) ? tCell1 : tCellDataL;

// assign cellData = tCell1;
// assign cellData = tCellData;
assign cellData = tCellDataL;
assign fontData = tFontData1;
// assign fontData = tFontData2;

initial begin
	$readmemh("fontmem.txt", fontMem);
	$readmemh("gfxfont0.txt", fontGfx1Mem);
	$readmemh("gfxfont1.txt", fontGfx2Mem);

	$readmemh("scrmem_1a.txt", scrCell1A);
	$readmemh("scrmem_1b.txt", scrCell1B);
	$readmemh("scrmem_1c.txt", scrCell1C);
	$readmemh("scrmem_1d.txt", scrCell1D);
end

always @*
begin
	nxtPixCellIx	= pixCellIx;

	tCtrlRegVal = { scrRegCtrl1, scrRegCtrl0 };

//	tBusCellIx		= busAddr[16:5];

`ifdef FBUF_EN128K
	tBusCellIx		= busAddr[16:5];
`else
`ifdef FBUF_EN64K
	tBusCellIx		= busAddr[15:5];
`else
	tBusCellIx		= busAddr[14:5];
`endif
`endif

	tBusOK = 0;
//	tBusData = 0;
	tBusData = tBusData2;

	if(busOE && tBusCSel)
	begin
//		nxtPixCellIx	= busAddr[15:2];
		nxtPixCellIx	= busAddr[16:3];

//		if(busAddr[15:8]==255)
		if(busAddr[16:8]==9'h1FF)
		begin
			tBusOK=1;
			case(busAddr[7:2])
//				0: tBusData = scrRegCtrl[0];
//				1: tBusData = scrRegCtrl[1];
//				2: tBusData = scrRegCtrl[2];
//				3: tBusData = scrRegCtrl[3];

				0: tBusData = { UV32_XX, scrRegCtrl0 };
				1: tBusData = { UV32_XX, scrRegCtrl1 };
				2: tBusData = { UV32_XX, scrRegCtrl2 };
				3: tBusData = { UV32_XX, scrRegCtrl3 };
				4: tBusData = { UV32_XX, scrRegCtrl4 };
				5: tBusData = { UV32_XX, scrRegCtrl5 };
				6: tBusData = { UV32_XX, scrRegCtrl6 };
				7: tBusData = { UV32_XX, scrRegCtrl7 };
//				8: tBusData = {18'h0, pixCellIx};
				8: tBusData = { UV32_XX, 18'h0, pixCellIx};
				
				9: tBusData = { UV32_00, scrRegCtrl9 };

				default: tBusData = UV64_00;

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

//			tBusOK = (tPixCellIx[11:0] == busAddr[16:5]) ? 2'b01 : 2'b10;
			tBusOK = (tPixCellIx2[11:0] == busAddr[16:5]) ? 2'b01 : 2'b10;
			if(busQW)
			begin
				case(busAddr[4:3])
					2'b00: tBusData = tCell1[ 63:  0];
					2'b01: tBusData = tCell1[127: 64];
					2'b10: tBusData = tCell1[191:128];
					2'b11: tBusData = tCell1[255:192];
				endcase
			end
			else
			begin
				case(busAddr[4:2])
					3'b000: tBusData = { UV32_XX, tCell1[ 31:  0] };
					3'b001: tBusData = { UV32_XX, tCell1[ 63: 32] };
					3'b010: tBusData = { UV32_XX, tCell1[ 95: 64] };
					3'b011: tBusData = { UV32_XX, tCell1[127: 96] };
					3'b100: tBusData = { UV32_XX, tCell1[159:128] };
					3'b101: tBusData = { UV32_XX, tCell1[191:160] };
					3'b110: tBusData = { UV32_XX, tCell1[223:192] };
					3'b111: tBusData = { UV32_XX, tCell1[255:224] };
				endcase
			end
		end
	end

	if(busWR && tBusCSel)
	begin
		tBusOK=1;
	end

//	tFontDataAsc1 = fontMem[tFontGlyph[7:0]];
//	tFontDataGfx1 = fontGfx1Mem[tFontGlyph[6:0]];
//	tFontDataGfx2 = fontGfx2Mem[tFontGlyph[6:0]];
	case(tFontGlyph[9:7])
	3'b000:	tFontData2 = tFontDataAsc1;
	3'b001:	tFontData2 = tFontDataAsc1;
	3'b010:	tFontData2 = tFontDataGfx2;
	3'b011:	tFontData2 = tFontDataGfx1;
	3'b100:	tFontData2 = tFontDataRam;
	3'b101:	tFontData2 = tFontDataRam;
	3'b110:	tFontData2 = tFontDataRam;
	3'b111:	tFontData2 = tFontDataRam;
	endcase

end

always @ (posedge clock)
begin
 	tBusData2		<= tBusData;
	tBusOK2			<= tBusOK;

//	tCell1			<= tNextCell1;
//	tPixCellIx		<= pixCellIx;

	tPixCellIx		<= nxtPixCellIx;
	tCellDataL		<= tCellData;
	tPixCellIx2		<= tPixCellIx;

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
	tFontDataRam	<= {
		fontRamD[tFontGlyph[8:0]],
		fontRamC[tFontGlyph[8:0]],
		fontRamB[tFontGlyph[8:0]],
		fontRamA[tFontGlyph[8:0]] };
	
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
			case(busAddr[7:2])
				6'b000000: scrRegCtrl0	<= busData[31:0];
				6'b000001: scrRegCtrl1	<= busData[31:0];
				6'b000010: scrRegCtrl2	<= busData[31:0];
				6'b000011: scrRegCtrl3	<= busData[31:0];
				6'b000100: scrRegCtrl4	<= busData[31:0];
				6'b000101: scrRegCtrl5	<= busData[31:0];
				6'b000110: scrRegCtrl6	<= busData[31:0];
				6'b000111: scrRegCtrl7	<= busData[31:0];
				6'b001000: begin
				end
				6'b001001: scrRegCtrl9	<= busData[31:0];

				6'b001100: fontRamA[busData[24:16]]	<= busData[15:0];
				6'b001101: fontRamB[busData[24:16]]	<= busData[15:0];
				6'b001110: fontRamC[busData[24:16]]	<= busData[15:0];
				6'b001111: fontRamD[busData[24:16]]	<= busData[15:0];
				default: begin
				end
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

`ifdef def_true
			if(busDataStA)
				scrCell1A[tBusCellIx] <= busDataInA;
			if(busDataStB)
				scrCell1B[tBusCellIx] <= busDataInB;
			if(busDataStC)
				scrCell1C[tBusCellIx] <= busDataInA;
			if(busDataStD)
				scrCell1D[tBusCellIx] <= busDataInB;
			if(busDataStE)
				scrCell1E[tBusCellIx] <= busDataInA;
			if(busDataStF)
				scrCell1F[tBusCellIx] <= busDataInB;
			if(busDataStG)
				scrCell1G[tBusCellIx] <= busDataInA;
			if(busDataStH)
				scrCell1H[tBusCellIx] <= busDataInB;
`endif

`ifndef def_true
			case(busAddr[4:3])
				2'b00: begin
					if(busDataAdA)
						scrCell1A[tBusCellIx] <= busDataInA;
					if(busDataAdB)
						scrCell1B[tBusCellIx] <= busDataInB;
				end
				2'b01: begin
					if(busDataAdA)
						scrCell1C[tBusCellIx] <= busDataInA;
					if(busDataAdB)
						scrCell1D[tBusCellIx] <= busDataInB;
				end
				2'b10: begin
					if(busDataAdA)
						scrCell1E[tBusCellIx] <= busDataInA;
					if(busDataAdB)
						scrCell1F[tBusCellIx] <= busDataInB;
				end
				2'b11: begin
					if(busDataAdA)
						scrCell1G[tBusCellIx] <= busDataInA;
					if(busDataAdB)
						scrCell1H[tBusCellIx] <= busDataInB;
				end
			endcase
`endif

`ifndef def_true

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

`endif
		end
	end
end

endmodule
