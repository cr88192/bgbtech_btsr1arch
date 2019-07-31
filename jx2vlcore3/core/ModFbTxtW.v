/*
Extended Text Mode display model.

Has 32kB display memory as 256-bit cells.

[31:30]=Cell Type Tag
  00: Cells are a text-cell, with a 10-bit font space.
    [9:8] Font
    [7:0] Glyph
    [15:10], Attr Flags (if not RGB data)
  01: Cells are a bitmap cell, 4x4x1
    [15:0], 4x4x1 Bitmap bits.
  10: Cells are a bitmap cell:
    [127:64] 8x8x1 Luma (Y)
    [ 63:48] 4x4x1 Chroma (U)
    [ 47:32] 4x4x1 Chroma (V)
    Low 28 bits: 2x YUV444
  11: 256b Color-Cell Cells
    [63:62] Sub-Tag
      00: Reserved
      01: 8x16 as 4x8 sub-cells.
      10: 4x8 as 2x4 sub-cells.
      11: 8x8 as 4x4 sub-cells.

[29:28]=Color Type Tag
  00: 2x 6-bit RGB endpoints (64 colors).
  10: 2x 9-bit RGB endpoints (512 colors).

 */

module ModFbTxtW(clock, reset,
	pixPosX,	pixPosY,
	pixCy,		pixCu,		pixCv,
	pixCellIx,	cellData,
	fontGlyph,	fontData,
	ctrlRegVal,	pixLineOdd);

/* verilator lint_off UNUSED */

input clock;
input reset;

input[9:0] pixPosX;
input[9:0] pixPosY;

output[7:0] pixCy;
output[7:0] pixCu;
output[7:0] pixCv;

output[13:0] pixCellIx;
input[255:0] cellData;

output[15:0]	fontGlyph;
input[63:0]		fontData;
input[63:0]		ctrlRegVal;
input			pixLineOdd;

reg[9:0]	tPixPosX;
reg[9:0]	tPixPosY;

reg[13:0]	tPixCellX;			//base cell X
reg[13:0]	tPixCellY;			//base cell Y

reg[13:0]	tPixCellIx;			//base cell index
reg[3:0]	tPixCellFx;			//base cell index

reg[13:0]	tPixCellNextIx;		//base cell index
reg[3:0]	tPixCellNextFx;		//pixel index (4x4)
reg[5:0]	tPixCellNextGx;		//glyph pix index (8x8)

reg[255:0]	tCellData;

reg[15:0]	tFontGlyph;
reg[63:0]	tFontData;
reg[15:0]	tFontGlyphU;
reg[15:0]	tFontGlyphV;

reg[63:0]	tPixelData31;
reg[63:0]	tPixelData32A;
reg[ 7:0]	tPixelData32B;

reg[5:0]	tClrA;
reg[5:0]	tClrB;
reg[8:0]	tClr9A;
reg[8:0]	tClr9B;

reg[11:0]	tClrRgbA;
reg[11:0]	tClrRgbB;

// reg[11:0]	tClrYuvA;
// reg[11:0]	tClrYuvB;
// reg[11:0]	tClrYuvC;

reg[15:0]	tClrYuvA;
reg[15:0]	tClrYuvB;
reg[15:0]	tClrYuvC;

reg[15:0]	tClr33A;		//A
reg[15:0]	tClr33B;		//B
reg[15:0]	tClr33C;		//(2/3)A + (1/3)B
reg[15:0]	tClr33D;		//(1/3)A + (2/3)B
reg[15:0]	tClr33E;		//(1/2)A + (1/2)B
reg[15:0]	tNextClr33C;	//(2/3)A + (1/3)B
reg[15:0]	tNextClr33D;	//(1/3)A + (2/3)B
reg[15:0]	tNextClr33E;	//(1/2)A + (1/2)B

reg[15:0]	tClr32CV;		//Center
reg[15:0]	tNextClr32CV;	//Center
reg[15:0]	tClr32A;		//A
reg[15:0]	tClr32B;		//B
reg[15:0]	tClr32C;		//(2/3)A + (1/3)B
reg[15:0]	tClr32D;		//(1/3)A + (2/3)B
reg[15:0]	tClr32E;		//(1/2)A + (1/2)B
reg[15:0]	tNextClr32A;		//A
reg[15:0]	tNextClr32B;		//B
reg[15:0]	tNextClr32C;	//(2/3)A + (1/3)B
reg[15:0]	tNextClr32D;	//(1/3)A + (2/3)B
reg[15:0]	tNextClr32E;	//(1/2)A + (1/2)B

reg[14:0]	tClr32YD;		//Y Delta Pair
reg[6:0]	tClr32Dy;		//Y Delta
reg[6:0]	tNextClr32Dy;		//Y Delta
reg[1:0]	tClrIx32;


reg[31:0]	tCellData33;
reg[31:0]	tNextCellData33;
reg[1:0]	tClrIx33;

reg[7:0]	tPixCy;
reg[7:0]	tPixCu;
reg[7:0]	tPixCv;

reg[11:0]	cbClrTab[63:0];

reg		useCol80;
reg		useHalfCell;

reg		tCellIsOdd;
reg		tNextCellIsOdd;

assign	pixCellIx = tPixCellIx;
assign	fontGlyph = tFontGlyph;

assign	pixCy = tPixCy;
assign	pixCu = tPixCu;
assign	pixCv = tPixCv;

initial
begin
	cbClrTab[ 0]=12'h088;	cbClrTab[ 1]=12'h1A8;
	cbClrTab[ 2]=12'h2D8;	cbClrTab[ 3]=12'h3F8;
	cbClrTab[ 4]=12'h255;	cbClrTab[ 5]=12'h385;
	cbClrTab[ 6]=12'h5A5;	cbClrTab[ 7]=12'h6D5;
	cbClrTab[ 8]=12'h522;	cbClrTab[ 9]=12'h652;
	cbClrTab[10]=12'h782;	cbClrTab[11]=12'h9A2;
	cbClrTab[12]=12'h700;	cbClrTab[13]=12'h920;
	cbClrTab[14]=12'hA50;	cbClrTab[15]=12'hB80;
	cbClrTab[16]=12'h18A;	cbClrTab[17]=12'h2AA;
	cbClrTab[18]=12'h3DA;	cbClrTab[19]=12'h5FA;
	cbClrTab[20]=12'h358;	cbClrTab[21]=12'h588;
	cbClrTab[22]=12'h6A8;	cbClrTab[23]=12'h7D8;
	cbClrTab[24]=12'h625;	cbClrTab[25]=12'h755;
	cbClrTab[26]=12'h985;	cbClrTab[27]=12'hAA5;
	cbClrTab[28]=12'h902;	cbClrTab[29]=12'hA22;
	cbClrTab[30]=12'hB52;	cbClrTab[31]=12'hD82;
	cbClrTab[32]=12'h28D;	cbClrTab[33]=12'h3AD;
	cbClrTab[34]=12'h5DD;	cbClrTab[35]=12'h6FD;
	cbClrTab[36]=12'h55A;	cbClrTab[37]=12'h68A;
	cbClrTab[38]=12'h7AA;	cbClrTab[39]=12'h9DA;
	cbClrTab[40]=12'h728;	cbClrTab[41]=12'h958;
	cbClrTab[42]=12'hA88;	cbClrTab[43]=12'hBA8;
	cbClrTab[44]=12'hA05;	cbClrTab[45]=12'hB25;
	cbClrTab[46]=12'hD55;	cbClrTab[47]=12'hE85;
	cbClrTab[48]=12'h38F;	cbClrTab[49]=12'h5AF;
	cbClrTab[50]=12'h6DF;	cbClrTab[51]=12'h7FF;
	cbClrTab[52]=12'h65D;	cbClrTab[53]=12'h78D;
	cbClrTab[54]=12'h9AD;	cbClrTab[55]=12'hADD;
	cbClrTab[56]=12'h92A;	cbClrTab[57]=12'hA5A;
	cbClrTab[58]=12'hB8A;	cbClrTab[59]=12'hDAA;
	cbClrTab[60]=12'hB08;	cbClrTab[61]=12'hD28;
	cbClrTab[62]=12'hE58;	cbClrTab[63]=12'hF88;
end

// always @ (clock)
always @*
begin
	tPixCellX	= 0;
	tPixCellY	= 0;
	useHalfCell	= 0;
	
//	useCol80 = 1;
//	useCol80 = 0;

	useCol80 = ctrlRegVal[0];
	useHalfCell = ctrlRegVal[1];

	tClrYuvC = 0;
	tNextCellIsOdd	= 0;

	if(useCol80 && !useHalfCell)
	begin
		tPixCellX[6:0] = tPixPosX[9:3];
		tPixCellY[6:0] = tPixPosY[9:3];
		tPixCellNextIx = tPixCellY*80 + tPixCellX - 160;
		tNextCellIsOdd = 0;
	end
	else
	begin
		tPixCellX[5:0] = tPixPosX[9:4];
		tPixCellY[6:0] = tPixPosY[9:3];
		tPixCellNextIx = tPixCellY*40 + tPixCellX - 80;
		tNextCellIsOdd = tPixPosX[3];
	end
	
	if(useCol80)
	begin
//		tPixCellX[6:0] = tPixPosX[9:3];
//		tPixCellY[6:0] = tPixPosY[9:3];
	//	tPixCellNextIx = tPixCellY*80 + tPixCellX;
//		tPixCellNextIx = tPixCellY*80 + tPixCellX - 160;

		tPixCellNextFx[1:0] = 2'h2 - tPixPosX[2:1];
		tPixCellNextFx[3:2] = 2'h2 - tPixPosY[2:1];
		
		tPixCellNextGx[2:0]	= 3'h7 - tPixPosX[2:0];
		tPixCellNextGx[5:3]	= 3'h7 - tPixPosY[2:0];
	end
	else
	begin
//		tPixCellX[5:0] = tPixPosX[9:4];
//		tPixCellY[6:0] = tPixPosY[9:3];
//		tPixCellNextIx = tPixCellY*40 + tPixCellX - 80;

		tPixCellNextFx[1:0] = 2'h3 - tPixPosX[3:2];
		tPixCellNextFx[3:2] = 2'h3 - tPixPosY[2:1];
		
		tPixCellNextGx[2:0]	= 3'h7 - tPixPosX[3:1];
		tPixCellNextGx[5:3]	= 3'h7 - tPixPosY[2:0];
	end
	
	tCellData = cellData;
	if(useHalfCell)
	begin
		tCellData = tCellIsOdd ?
			{ UV128_XX, cellData[255:128] } :
			{ UV128_XX, cellData[127:  0] };
	end
	
//	tCellData = 32'h003F_0041;
//	tCellData = 32'h000C_0041;
//	tCellData[7:0]=tPixCellNextIx[7:0];

//	tCellData[17] = !tPixCellY[0];
//	tCellData[19] = !tPixCellY[1];
//	tCellData[21] = !tPixCellY[2];
//	tCellData[23] = tPixCellY[0];
//	tCellData[25] = tPixCellY[1];
//	tCellData[27] = tPixCellY[2];

	if(tPixCellIx >= 2000)
		tCellData = 0;
	if(!useCol80 && (tPixCellIx>=1000))
		tCellData = 0;

	tFontGlyph = tCellData[15:0];
	tClrA = tCellData[21:16];
	tClrB = tCellData[27:22];
	
	tClr9A = tCellData[18:10];
	tClr9B = tCellData[27:19];
	
	tClrRgbA = 0;
	tClrRgbB = 0;
	
	case(tCellData[29:28])
		2'b00: begin
			tClrRgbA[11:10]=tClrA[5:4];
			tClrRgbA[ 9: 8]=tClrA[5:4];
			tClrRgbA[ 7: 6]=tClrA[3:2];
			tClrRgbA[ 5: 4]=tClrA[3:2];
			tClrRgbA[ 3: 2]=tClrA[1:0];
			tClrRgbA[ 1: 0]=tClrA[1:0];

			tClrRgbB[11:10]=tClrB[5:4];
			tClrRgbB[ 9: 8]=tClrB[5:4];
			tClrRgbB[ 7: 6]=tClrB[3:2];
			tClrRgbB[ 5: 4]=tClrB[3:2];
			tClrRgbB[ 3: 2]=tClrB[1:0];
			tClrRgbB[ 1: 0]=tClrB[1:0];
		end

		2'b10: begin
			tClrRgbA[11: 9]=tClr9A[8:6];
			tClrRgbA[ 7: 5]=tClr9A[5:3];
			tClrRgbA[ 3: 1]=tClr9A[2:0];

			tClrRgbB[11: 9]=tClr9B[8:6];
			tClrRgbB[ 7: 5]=tClr9B[5:3];
			tClrRgbB[ 3: 1]=tClr9B[2:0];
		end
		
		default: begin end
	endcase
	
//	tClrYuvA[11:8]=
//		{1'b0, tClrRgbA[ 7: 5]}+
//		{2'b0, tClrRgbA[11:10]}+
//		{2'b0, tClrRgbA[ 3: 2]};
//	tClrYuvB[11:8]=
//		{1'b0, tClrRgbB[ 7: 5]}+
//		{2'b0, tClrRgbB[11:10]}+
//		{2'b0, tClrRgbB[ 3: 2]};
//	tClrYuvA[7:4]=4'h8+(tClrRgbA[ 3: 1]-tClrRgbA[ 7: 5]);
//	tClrYuvB[7:4]=4'h8+(tClrRgbB[ 3: 1]-tClrRgbB[ 7: 5]);
//	tClrYuvA[3:0]=4'h8+(tClrRgbA[11: 9]-tClrRgbA[ 7: 5]);
//	tClrYuvB[3:0]=4'h8+(tClrRgbB[11: 9]-tClrRgbB[ 7: 5]);

	tClrYuvA[15:10]=
		{1'b0, tClrRgbA[ 7: 3]}+
		{2'b0, tClrRgbA[11: 8]}+
		{2'b0, tClrRgbA[ 3: 0]};
	tClrYuvB[15:10]=
		{1'b0, tClrRgbB[ 7: 3]}+
		{2'b0, tClrRgbB[11: 8]}+
		{2'b0, tClrRgbB[ 3: 0]};
	tClrYuvA[9:5]=5'h10+(tClrRgbA[ 3: 0]-tClrRgbA[ 7: 4]);
	tClrYuvB[9:5]=5'h10+(tClrRgbB[ 3: 0]-tClrRgbB[ 7: 4]);
	tClrYuvA[4:0]=5'h10+(tClrRgbA[11: 8]-tClrRgbA[ 7: 4]);
	tClrYuvB[4:0]=5'h10+(tClrRgbB[11: 8]-tClrRgbB[ 7: 4]);
	
//	tClrYuvA = cbClrTab[tClrA];
//	tClrYuvB = cbClrTab[tClrB];
	tFontData = fontData;

	case( { tPixCellNextFx[3], tPixCellNextFx[1] } )
		2'b00: begin
			tClr33B		= { tCellData[125:111], tCellData[114] };
			tClr33A		= { tCellData[110: 96], tCellData[ 99] };
			tNextCellData33	= tCellData[255:224];
			tClr32YD	= tCellData[125:111];
		end
		2'b01: begin
			tClr33B		= { tCellData[ 93: 79], tCellData[82] };
			tClr33A		= { tCellData[ 78: 64], tCellData[67] };
			tNextCellData33	= tCellData[223:192];
			tClr32YD	= tCellData[110:96];
		end
		2'b10: begin
			tClr33B		= { tCellData[ 61: 47], tCellData[50] };
			tClr33A		= { tCellData[ 46: 32], tCellData[35] };
			tNextCellData33	= tCellData[191:160];
			tClr32YD	= tCellData[93:79];
		end
		2'b11: begin
			tClr33B		= { tCellData[ 29: 15], tCellData[18] };
			tClr33A		= { tCellData[ 14:  0], tCellData[ 3] };
			tNextCellData33	= tCellData[159:128];
			tClr32YD	= tCellData[78:64];
		end
	endcase

	tNextClr32CV	= tPixCellNextFx[2] ? tClr33A : tClr33B;
	tNextClr32Dy	= tPixCellNextFx[2] ? tClr32YD[6:0] : tClr32YD[13:7];

	tNextClr32A = {
		tClr32CV[15:10] + { 1'b0, tClr32Dy[6:2] },
		tClr32CV[9:0] };
	tNextClr32B = {
		tClr32CV[15:10] - { 1'b0, tClr32Dy[6:2] },
		tClr32CV[9:0] };

	tNextClr33C = {
		{ 1'b0, tClr33A[15:11] } + { 1'b0, tClr33E[15:11] },
		{ 1'b0, tClr33A[ 9: 6] } + { 1'b0, tClr33E[ 9: 6] } ,
		{ 1'b0, tClr33A[ 4: 1] } + { 1'b0, tClr33E[ 4: 1] } };
	tNextClr33D = {
		{ 1'b0, tClr33E[15:11] } + { 1'b0, tClr33B[15:11] },
		{ 1'b0, tClr33E[ 9: 6] } + { 1'b0, tClr33B[ 9: 6] } ,
		{ 1'b0, tClr33E[ 4: 1] } + { 1'b0, tClr33B[ 4: 1] } };
	tNextClr33E = {
		{ 1'b0, tClr33A[15:11] } + { 1'b0, tClr33B[15:11] } ,
		{ 1'b0, tClr33A[ 9: 6] } + { 1'b0, tClr33B[ 9: 6] } ,
		{ 1'b0, tClr33A[ 4: 1] } + { 1'b0, tClr33B[ 4: 1] } };

	tNextClr32C = {
		{ 1'b0, tClr33A[15:11] } + { 1'b0, tClr33E[15:11] },
		{ 1'b0, tClr33A[ 9: 6] } + { 1'b0, tClr33E[ 9: 6] } ,
		{ 1'b0, tClr33A[ 4: 1] } + { 1'b0, tClr33E[ 4: 1] } };
	tNextClr32D = {
		{ 1'b0, tClr33E[15:11] } + { 1'b0, tClr33B[15:11] },
		{ 1'b0, tClr33E[ 9: 6] } + { 1'b0, tClr33B[ 9: 6] } ,
		{ 1'b0, tClr33E[ 4: 1] } + { 1'b0, tClr33B[ 4: 1] } };
	tNextClr32E = {
		{ 1'b0, tClr33A[15:11] } + { 1'b0, tClr33B[15:11] } ,
		{ 1'b0, tClr33A[ 9: 6] } + { 1'b0, tClr33B[ 9: 6] } ,
		{ 1'b0, tClr33A[ 4: 1] } + { 1'b0, tClr33B[ 4: 1] } };

	case( { tPixCellNextGx[4:3], tPixCellNextGx[1:0] } )
		4'h0: tClrIx33=tCellData33[31:30];
		4'h1: tClrIx33=tCellData33[29:28];
		4'h2: tClrIx33=tCellData33[27:26];
		4'h3: tClrIx33=tCellData33[25:24];
		4'h4: tClrIx33=tCellData33[23:22];
		4'h5: tClrIx33=tCellData33[21:20];
		4'h6: tClrIx33=tCellData33[19:18];
		4'h7: tClrIx33=tCellData33[17:16];
		4'h8: tClrIx33=tCellData33[15:14];
		4'h9: tClrIx33=tCellData33[13:12];
		4'hA: tClrIx33=tCellData33[11:10];
		4'hB: tClrIx33=tCellData33[ 9: 8];
		4'hC: tClrIx33=tCellData33[ 7: 6];
		4'hD: tClrIx33=tCellData33[ 5: 4];
		4'hE: tClrIx33=tCellData33[ 3: 2];
		4'hF: tClrIx33=tCellData33[ 1: 0];
	endcase

`ifndef def_true
	tPixelData31 = !pixLineOdd ?
		{	tCellData[255:248],	tCellData[239:232],
			tCellData[223:216],	tCellData[207:200],
			tCellData[191:184],	tCellData[175:168],
			tCellData[159:152],	tCellData[143:136] } :
		{	tCellData[247:240],	tCellData[231:224],
			tCellData[215:208],	tCellData[199:192],
			tCellData[183:176],	tCellData[167:160],
			tCellData[151:144],	tCellData[135:128] };
`endif

	tPixelData32A = tCellData[255:192];

	case( tPixCellNextGx[5:3])
		3'h0: tPixelData32B=tPixelData32A[63:56];
		3'h1: tPixelData32B=tPixelData32A[55:48];
		3'h2: tPixelData32B=tPixelData32A[47:40];
		3'h3: tPixelData32B=tPixelData32A[39:32];
		3'h4: tPixelData32B=tPixelData32A[31:24];
		3'h5: tPixelData32B=tPixelData32A[23:16];
		3'h6: tPixelData32B=tPixelData32A[15: 8];
		3'h7: tPixelData32B=tPixelData32A[ 7: 0];
	endcase
	case(tPixCellNextGx[2:1])
		2'h3: tClrIx32=tPixelData32B[7:6];
		2'h2: tClrIx32=tPixelData32B[5:4];
		2'h1: tClrIx32=tPixelData32B[3:2];
		2'h0: tClrIx32=tPixelData32B[1:0];
	endcase


	if(tCellData[31:30]==2'b11)
	begin
		if(tCellData[29:28]==2'b01)
		begin
			tClrYuvC = (pixLineOdd) ?
				((tClrIx33[0]) ? tClr33A : tClr33B) :
				((tClrIx33[1]) ? tClr33A : tClr33B) ;
			
//			tClrYuvC = (tPixelData31[tPixCellNextGx]) ?
//				tClr33A : tClr33B;
		end
		else
			if(tCellData[63:62]==2'b10)
		begin
			case(tClrIx32)
				2'b00: tClrYuvC=tClr32B;
				2'b01: tClrYuvC=tClr32D;
				2'b10: tClrYuvC=tClr32C;
				2'b11: tClrYuvC=tClr32A;
			endcase
		end
		else
			if(tCellData[63:62]==2'b11)
		begin
			case(tClrIx33)
				2'b00: tClrYuvC=tClr33B;
				2'b01: tClrYuvC=tClr33D;
				2'b10: tClrYuvC=tClr33C;
				2'b11: tClrYuvC=tClr33A;
			endcase
		end
	end
	else
	if(tCellData[31:30]==2'b10)
	begin
		begin
			tFontData = tCellData[127:64];
			tClrYuvB[11:8]=tCellData[27:24];
			tClrYuvA[11:8]=tCellData[21:18];
			tClrYuvB[ 7:4]=tCellData[15:12];
			tClrYuvA[ 7:4]=tCellData[11: 8];
			tClrYuvB[ 3:0]=tCellData[ 7: 4];
			tClrYuvA[ 3:0]=tCellData[ 3: 0];
			
			tFontGlyphU = tCellData[63:48];
			tFontGlyphV = tCellData[47:32];

	//		tClrYuvC = (tFontData[tPixCellNextGx]) ? tClrYuvA : tClrYuvB;
//			tClrYuvC[11:8] = (tFontData[tPixCellNextGx]) ?
//				tClrYuvA[11:8] : tClrYuvB[11:8];

//			tClrYuvC[7:4] = (tFontGlyphU[tPixCellNextFx]) ?
//				tClrYuvA[7:4] : tClrYuvB[7:4];
//			tClrYuvC[3:0] = (tFontGlyphV[tPixCellNextFx]) ?
//				tClrYuvA[3:0] : tClrYuvB[3:0];


			tClrYuvC[15:10] = (tFontData[tPixCellNextGx]) ?
				tClrYuvA[15:10] : tClrYuvB[15:10];
			tClrYuvC[9:5] = (tFontGlyphU[tPixCellNextFx]) ?
				tClrYuvA[9:5] : tClrYuvB[9:5];
			tClrYuvC[4:0] = (tFontGlyphV[tPixCellNextFx]) ?
				tClrYuvA[4:0] : tClrYuvB[4:0];
		end

	end
	else
	begin
//		tFontData = 64'h0010_3844_7C44_4400;
//		tClrYuvA = 12'hF88;

		if(cellData[31:30]==2'b00)
			tClrYuvC = (tFontData[tPixCellNextGx]) ? tClrYuvA : tClrYuvB;
		else if(cellData[31:30]==2'b01)
			tClrYuvC = (tFontGlyph[tPixCellNextFx]) ? tClrYuvA : tClrYuvB;
	end

	
//	tPixCy[7:4] = tClrYuvC[11:8];	tPixCy[3:0] = tClrYuvC[11:8];
//	tPixCu[7:4] = tClrYuvC[7:4];	tPixCu[3:0] = tClrYuvC[7:4];
//	tPixCv[7:4] = tClrYuvC[3:0];	tPixCv[3:0] = tClrYuvC[3:0];

	tPixCy = { tClrYuvC[15:10], tClrYuvC[15:14] };
	tPixCu = { tClrYuvC[ 9: 5], tClrYuvC[ 9: 7] };
	tPixCv = { tClrYuvC[ 4: 0], tClrYuvC[ 4: 2] };

`ifndef def_true
//	tPixCy=128;
	tPixCy	=	(tPixCellX[0]^tPixCellY[0]) ? 255 : 0;
//	tPixCy	=	(pixPosX[3]^pixPosY[3]) ? 255 : 0;
	tPixCu	=	!(tPixCellX[1]^tPixCellY[1]) ? 255 : 0;
	tPixCv	=	(tPixCellX[1]^tPixCellY[1]) ? 255 : 0;

//	tPixCu=128;
//	tPixCv=128;

//	tPixCu=255;
//	tPixCv=0;
	
//	tPixCy=128;
//	tPixCu=255;
//	tPixCv=255;
`endif

end


always @ (posedge clock)
begin

	tPixPosX		<= pixPosX;
	tPixPosY		<= pixPosY;

	tPixCellIx		<= tPixCellNextIx;
	tPixCellFx		<= tPixCellNextFx;

	tCellIsOdd		<= tNextCellIsOdd;

	tClr33C			<= tNextClr33C;
	tClr33D			<= tNextClr33D;
	tClr33E			<= tNextClr33E;
	tCellData33		<= tNextCellData33;

	tClr32A			<= tNextClr32A;		//A
	tClr32B			<= tNextClr32B;		//B
	tClr32C			<= tNextClr32C;
	tClr32D			<= tNextClr32D;
	tClr32E			<= tNextClr32E;
	tClr32CV		<= tNextClr32CV;	//Center
	tClr32Dy		<= tNextClr32Dy;	//Y Delta

end

endmodule
