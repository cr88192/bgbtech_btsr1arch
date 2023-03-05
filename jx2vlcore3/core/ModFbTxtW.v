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
	pixCy,		pixCu,		pixCv,	pixAux,
	pixCellIx,	cellData,
	fontGlyph,	fontData,
	ctrlRegVal,	pixLineOdd,
	blinkSlow,	blinkFast);

/* verilator lint_off UNUSED */

input clock;
input reset;

input[11:0] pixPosX;
input[11:0] pixPosY;

output[7:0]		pixCy;
output[7:0]		pixCu;
output[7:0]		pixCv;
output[15:0]	pixAux;

output[13:0] pixCellIx;
input[255:0] cellData;

output[15:0]	fontGlyph;
input[63:0]		fontData;
input[63:0]		ctrlRegVal;
input			pixLineOdd;

input			blinkSlow;
input			blinkFast;

reg[11:0]	tPixPosX;
reg[11:0]	tPixPosY;

reg[11:0]	tPixPosX_Z;
reg[11:0]	tPixPosY_Z;

reg[11:0]	tPixPosX_Y;
reg[11:0]	tPixPosY_Y;

reg[11:0]	tPixPosX_X;
reg[11:0]	tPixPosY_X;

reg[13:0]	tPixCellX;			//base cell X
reg[13:0]	tPixCellY;			//base cell Y

reg[7:0]	tPixCellMaxX;			//
reg[7:0]	tPixCellMaxY;			//
reg[7:0]	tNextPixCellMaxX;		//
reg[7:0]	tNextPixCellMaxY;		//

reg[11:0]	tPixRawUsPosX;
reg[11:0]	tPixRawUsPosY;

reg[11:0]	tNxtPixRawPosX;
reg[11:0]	tNxtPixRawPosY;
reg[11:0]	tPixRawPosX;
reg[11:0]	tPixRawPosY;

reg[11:0]	tNxtPixRelPosX;
reg[11:0]	tNxtPixRelPosY;
reg[11:0]	tPixRelPosX;
reg[11:0]	tPixRelPosY;

reg[11:0]	tPixRawMaxX;
reg[11:0]	tPixRawMaxY;
reg[11:0]	tNxtPixRawMaxX;
reg[11:0]	tNxtPixRawMaxY;

reg[7:0]	tCellCursorX;		//cursor cell X
reg[7:0]	tCellCursorY;		//cursor cell Y
reg			tDoCellCursor;		//draw blinking cursor

reg[13:0]	tPixCellIx_A;		//base cell index
reg[3:0]	tPixCellFx_A;		//pixel index (4x4)
reg[5:0]	tPixCellGx_A;		//glyph pix index (8x8)

reg[13:0]	tPixCellIx_A0;		//base cell index
reg[3:0]	tPixCellFx_A0;		//pixel index (4x4)
reg[5:0]	tPixCellGx_A0;		//glyph pix index (8x8)

reg[13:0]	tPixCellIx_B;			//base cell index
reg[3:0]	tPixCellFx_B;			//base cell index
reg[5:0]	tPixCellGx_B;			//base cell index

reg[13:0]	tPixCellIx_C;			//base cell index
reg[3:0]	tPixCellFx_C;			//base cell index
reg[5:0]	tPixCellGx_C;			//base cell index

reg[5:0]	tPixCellGx_D;			//base cell index
reg[5:0]	tPixCellGx_E;			//base cell index

reg[255:0]	tCellData0;
reg[255:0]	tCellData;
reg[255:0]	tCellData_B;
reg[255:0]	tCellData_C;

reg[15:0]	tFontGlyph;
reg[15:0]	tFontGlyph_C;
reg[63:0]	tFontData_A;
reg[63:0]	tFontData_B;
reg[63:0]	tFontData_C;

reg[63:0]	tFontGlyphY_A;
reg[63:0]	tFontGlyphY_B;
reg[63:0]	tFontGlyphY_C;

reg[15:0]	tFontGlyphU_A;
reg[15:0]	tFontGlyphV_A;
reg[15:0]	tFontGlyphU_B;
reg[15:0]	tFontGlyphV_B;
reg[15:0]	tFontGlyphU_C;
reg[15:0]	tFontGlyphV_C;

reg[63:0]	tPixelData31;
reg[63:0]	tPixelData32A;
reg[ 7:0]	tPixelData32B;

`ifdef jx2_fbuf_hwsprite

reg[127:0]	tSprCtrlArr[63:0];
reg[127:0]	tSprCtrl;

reg[127:0]	tStSprCtrl;
reg[5:0]	tStSprIdx;
reg			tDoStSpr;

reg[5:0]	tNxtSprRov;
reg[5:0]	tSprRov;
reg			tReqSprAdv;
reg			tDoSprAdv;

reg[127:0]	tSpr0Ctrl;
reg[127:0]	tSpr0Data;
reg[127:0]	tSpr1Ctrl;
reg[127:0]	tSpr1Data;
reg[127:0]	tSpr2Ctrl;
reg[127:0]	tSpr2Data;
reg[127:0]	tSpr3Ctrl;
reg[127:0]	tSpr3Data;

reg[127:0]	tSpr4Ctrl;
reg[127:0]	tSpr4Data;
reg[127:0]	tNxtSpr4Data;

// wire[14:0]	tSpr0ColorA	= tSpr0Ctrl[46:32];
// wire[14:0]	tSpr0ColorB	= tSpr0Ctrl[61:47];
// wire[15:0]	tSpr0Glyph	= tSpr0Ctrl[15: 0];
wire[11:0]	tSpr0PosX	= tSpr0Ctrl[75:64];
wire[11:0]	tSpr0PosY	= tSpr0Ctrl[87:76];

// wire[14:0]	tSpr1ColorA	= tSpr1Ctrl[46:32];
// wire[14:0]	tSpr1ColorB	= tSpr1Ctrl[61:47];
// wire[15:0]	tSpr1Glyph	= tSpr1Ctrl[15: 0];
// wire[11:0]	tSpr1PosX	= tSpr1Ctrl[75:64];
// wire[11:0]	tSpr1PosY	= tSpr1Ctrl[87:76];

// wire[14:0]	tSpr2ColorA	= tSpr2Ctrl[46:32];
// wire[14:0]	tSpr2ColorB	= tSpr2Ctrl[61:47];
// wire[15:0]	tSpr2Glyph	= tSpr2Ctrl[15: 0];
// wire[11:0]	tSpr2PosX	= tSpr2Ctrl[75:64];
// wire[11:0]	tSpr2PosY	= tSpr2Ctrl[87:76];

// wire[14:0]	tSpr3ColorA	= tSpr3Ctrl[46:32];
// wire[14:0]	tSpr3ColorB	= tSpr3Ctrl[61:47];
// wire[15:0]	tSpr3Glyph	= tSpr3Ctrl[15: 0];
// wire[11:0]	tSpr3PosX	= tSpr3Ctrl[75:64];
// wire[11:0]	tSpr3PosY	= tSpr3Ctrl[87:76];

wire[15:0]	tSpr4GlyphA	= { 4'h0, tSpr4Ctrl[11: 0] };
wire[15:0]	tSpr4GlyphB	= { 4'h0, tSpr4Ctrl[23:12] };

reg			tDoSpr4Glyph;
reg			tDoSpr4GlyphB;
reg			tDoSpr4GlyphC;
reg			tDoSpr4GlyphD;
reg			tDoSpr4GlyphE;

reg			tDoSpr4GlyphBit;
reg			tDoSpr4GlyphBitL;

reg[1:0]	tSprDrawBit;
reg[1:0]	tNxtSprDrawBit;

// wire[127:0]	tSprCurCtrl		= tDoSpr4GlyphBit ? tSpr1Ctrl : tSpr0Ctrl;
// wire[127:0]	tSprCurData		= tDoSpr4GlyphBit ? tSpr1Data : tSpr0Data;

wire[127:0]	tNxtSprCurCtrl		=
	tSprDrawBit[1] ? 
		(tSprDrawBit[0] ? tSpr3Ctrl : tSpr2Ctrl) :
		(tSprDrawBit[0] ? tSpr1Ctrl : tSpr0Ctrl) ;
wire[127:0]	tNxtSprCurData		=
	tSprDrawBit[1] ? 
		(tSprDrawBit[0] ? tSpr3Data : tSpr2Data) :
		(tSprDrawBit[0] ? tSpr1Data : tSpr0Data) ;

reg[127:0]	tSprCurCtrl;
reg[127:0]	tSprCurData;
reg[11:0]	tSprCurRelX;
reg[11:0]	tSprCurRelY;

wire[63:0]	tSprCurDataSel	= tSprCurData[ 63: 0];
wire[63:0]	tSprCurDataMsk	= tSprCurData[127:64];

wire[14:0]	tSprCurColorA	= tSprCurCtrl[46:32];
wire[14:0]	tSprCurColorB	= tSprCurCtrl[61:47];
// wire[15:0]	tSprCurGlyph	= tSprCurCtrl[15: 0];
wire[11:0]	tSprCurPosX		= tSprCurCtrl[75:64];
wire[11:0]	tSprCurPosY		= tSprCurCtrl[87:76];

// wire[11:0]	tSprCurRelX		= tSprCurPosX - tPixRelPosX;
// wire[11:0]	tSprCurRelY		= tSprCurPosY - tPixRelPosY;

wire[11:0]	tNxtSprCurRelX		= tNxtSprCurCtrl[75:64] - tPixRelPosX;
wire[11:0]	tNxtSprCurRelY		= tNxtSprCurCtrl[87:76] - tPixRelPosY;

reg			tSprCurSelBit;
reg			tSprCurMskBit;
reg[15:0]	tSprCurSelClr;
reg[15:0]	tSprCurSelClrL;

`endif

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

reg[15:0]	tClrYuvA_B;
reg[15:0]	tClrYuvB_B;
reg[15:0]	tClrYuvC_B;

reg[15:0]	tClrYuvA_C;
reg[15:0]	tClrYuvB_C;
reg[15:0]	tClrYuvC_C;

reg[15:0]	tClrYuvC_D;

reg[15:0]	tClr33A;		//A
reg[15:0]	tClr33B;		//B
reg[15:0]	tClr33C;		//(2/3)A + (1/3)B
reg[15:0]	tClr33D;		//(1/3)A + (2/3)B
reg[15:0]	tClr33E;		//(1/2)A + (1/2)B
reg[15:0]	tNextClr33A;//A
reg[15:0]	tNextClr33B;		//B
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
reg[15:0]	tNextClr32A;	//A
reg[15:0]	tNextClr32B;	//B
reg[15:0]	tNextClr32C;	//(2/3)A + (1/3)B
reg[15:0]	tNextClr32D;	//(1/3)A + (2/3)B
reg[15:0]	tNextClr32E;	//(1/2)A + (1/2)B

reg[14:0]	tClr32YD;		//Y Delta Pair
reg[6:0]	tClr32Dy;		//Y Delta
reg[6:0]	tNextClr32Dy;		//Y Delta
reg[1:0]	tClrIx32;
reg[1:0]	tClrNxtIx32;


reg[3:0]	tScrMode;
reg[3:0]	tScrClrsMod;
reg[3:0]	tScrClrsModB;
reg[3:0]	tScrClrsModC;

reg[31:0]	tCellData33;
reg[31:0]	tNextCellData33;
reg[1:0]	tClrIx33;
reg[1:0]	tClrNxtIx33;

reg[3:0]	tPixClrBmRgbi;
reg[3:0]	tPixClrBmRgbiB;
reg[11:0]	tPixClrBmRgbiYV12;

reg[1:0]	tPixClrBm2i;
reg[11:0]	tPixClrBm2iYV12;
reg[15:0]	tPixClrBm2iYV16_D;
reg[15:0]	tPixClrBm2iYV16_C;

reg			tPixClrBm1i;

reg[15:0]	tPixClrBmYv16_D;
reg[15:0]	tPixClrBmYv16_C;
reg[15:0]	tPixClrBmRgbiYV16_D;
reg[15:0]	tPixClrBmRgbiYV16_C;

reg[7:0]	tPixClrBmYv8_D;
reg[7:0]	tPixClrBmYv8_C;
reg[15:0]	tPixClrBmPalYV16_C;
reg[15:0]	tPixClrBmPalYV16_D;

reg[3:0]	tPixClrBmYv4_D;
reg[3:0]	tPixClrBmYv4_C;
reg[3:0]	tPixClrBmYv4Rgbi;
reg[11:0]	tPixClrBmYv4RgbiYV12;

reg			tPixRgb565;
reg			tBlinkStrobeA;
reg			tBlinkStrobeB;


reg[7:0]	tPixCy;
reg[7:0]	tPixCu;
reg[7:0]	tPixCv;
reg[15:0]	tPixAux;

reg[7:0]	tPixCy2;
reg[7:0]	tPixCu2;
reg[7:0]	tPixCv2;
reg[15:0]	tPixAux2;

reg[11:0]	cbClrTab[63:0];

reg[11:0]	cbClrTabRgbi[15:0];

reg[63:0]	tCtrlRegVal;

reg		useCol80;
reg		useRow50;
reg		useHalfCell;
reg		useQtrCell;
reg		useHorz800;
reg		useColPow2;
reg		useVert480;

reg		tCellIsOdd;
reg		tNextCellIsOdd;
reg		tCellIsOddB;
reg		tNextCellIsOddB;

reg		tCellLimitX;
reg		tCellLimitY;

reg		tPixRawLimitX;
reg		tPixRawLimitY;

assign	pixCellIx = tPixCellIx_B;
// assign	pixCellIx = tPixCellIx_C;
assign	fontGlyph = tFontGlyph;

assign	pixCy	= tPixCy2;
assign	pixCu	= tPixCu2;
assign	pixCv	= tPixCv2;
assign	pixAux	= tPixAux2;

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
	
	cbClrTabRgbi[ 0]=12'h000;	cbClrTabRgbi[ 1]=12'h28D;
	cbClrTabRgbi[ 2]=12'h522;	cbClrTabRgbi[ 3]=12'h728;
	cbClrTabRgbi[ 4]=12'h2D8;	cbClrTabRgbi[ 5]=12'h5DD;
	cbClrTabRgbi[ 6]=12'h782;	cbClrTabRgbi[ 7]=12'hA88;
	cbClrTabRgbi[ 8]=12'h588;	cbClrTabRgbi[ 9]=12'h78D;
	cbClrTabRgbi[10]=12'hA22;	cbClrTabRgbi[11]=12'hD28;
	cbClrTabRgbi[12]=12'h7D8;	cbClrTabRgbi[13]=12'hADD;
	cbClrTabRgbi[14]=12'hD82;	cbClrTabRgbi[15]=12'hF88;
end

// always @ (clock)
always @*
begin
	tPixCellX	= 0;
	tPixCellY	= 0;
	useHalfCell	= 0;
	useQtrCell	= 0;
	useHorz800	= 0;
	
//	useCol80 = 1;
//	useCol80 = 0;

	useCol80	= tCtrlRegVal[0];
	useHalfCell	= tCtrlRegVal[1];
	useRow50	= tCtrlRegVal[2];
//	useHorz800	= tCtrlRegVal[3];
	useHorz800	= tCtrlRegVal[3] && !tCtrlRegVal[9];
	
	tScrMode	= tCtrlRegVal[7:4];
	useColPow2	= tCtrlRegVal[8];
	useVert480	= tCtrlRegVal[9];
	useQtrCell	= tCtrlRegVal[10];

	tScrClrsMod		= tCtrlRegVal[15:12];

	tCellCursorX	= tCtrlRegVal[39:32];
//	tCellCursorY	= tCtrlRegVal[47:40] + 2;
	tCellCursorY	= tCtrlRegVal[47:40];

	tClrYuvC		= 0;
	tNextCellIsOdd	= 0;
	tNextCellIsOddB	= 0;
	tPixAux			= 0;

//	tPixRgb565		= 0;
	tPixRgb565		= tScrMode[3];

	tCellLimitX		= 0;
	tCellLimitY		= 0;
	tNextPixCellMaxX	= 80;
//	tNextPixCellMaxY	= 27;
	tNextPixCellMaxY	= 25;

	tNxtPixRawMaxX		= 640;
	tNxtPixRawMaxY		= 400;

	tNxtPixRawPosX		= tPixRawUsPosX;
	tNxtPixRawPosY		= tPixRawUsPosY;
	tNxtPixRelPosX		= tPixPosX_Z;
	tNxtPixRelPosY		= tPixPosY_Z;

	/* Z Stage */
	if(useColPow2)
	begin
		if(useHorz800)
		begin
			if(useRow50)
			begin
				tNextPixCellMaxY	= 96;
			end
			else
			begin
				tNextPixCellMaxY	= 48;
			end
		end
		else
		begin
			if(useRow50)
			begin
				tNextPixCellMaxY	= 64;
			end
			else
			begin
				tNextPixCellMaxY	= 32;
			end
		end

	end
	else
		if(useRow50)
	begin
		tNxtPixRawMaxY		= 400;
		tNextPixCellMaxY	= 50;
		if(useVert480)
		begin
			tNxtPixRawMaxY		= 480;
			tNextPixCellMaxY	= 60;
		end
	end
	else
	begin
		tNxtPixRawMaxY		= 200;
		tNextPixCellMaxY	= 25;
		if(useVert480)
		begin
			tNxtPixRawMaxY		= 240;
			tNextPixCellMaxY	= 30;
		end
	end

	if(useRow50)
	begin
		tNxtPixRawPosY	= tPixRawUsPosY[11:0];
		tNxtPixRelPosY	= tPixPosY_Z[11:0];
		tPixCellY[6:0]	= tPixPosY_Z[9:3];
	end
		else
	begin
		tNxtPixRawPosY	= { 1'b0, tPixRawUsPosY[11:1] };
		tNxtPixRelPosY	= { 1'b0, tPixPosY_Z[11:1] };
		tPixCellY[6:0]	= { tPixPosY_Z[9], tPixPosY_Z[9:4] };
	end

	if(useColPow2)
	begin
		if(useCol80 && !useHalfCell)
		begin
			tNxtPixRawPosX		= tPixRawUsPosX[11:0];
			tNxtPixRelPosX		= tPixPosX_Z[11:0];
			tPixCellX[6:0]		= tPixPosX_Z[9:3];
			tNextPixCellMaxX	= 128;
		end
		else
		begin
			tNxtPixRawPosX		= { 1'b0, tPixRawUsPosX[11:1] };
			tNxtPixRelPosX		= { 1'b0, tPixPosX_Z[11:1] };
			tPixCellX[5:0]		= tPixPosX_Z[9:4];
			tNextPixCellMaxX	= 64;
		end

	end
	else
		if(useCol80)
	begin
		tNxtPixRawPosX	= tPixRawUsPosX[11:0];
		tNxtPixRelPosX	= tPixPosX_Z[11:0];

		tPixCellX[6:0] = tPixPosX_Z[9:3];
		if(useHorz800)
		begin
			tNxtPixRawMaxX		= 800;
			tNextPixCellMaxX	= 100;
		end
		else
		begin
			tNxtPixRawMaxX		= 640;
			tNextPixCellMaxX	= 80;
		end
	end
	else
	begin
		tNxtPixRawPosX	= { 1'b0, tPixRawUsPosX[11:1] };
		tNxtPixRelPosX	= { 1'b0, tPixPosX_Z[11:1] };

		tPixCellX[5:0] = tPixPosX_Z[9:4];
		if(useHorz800)
		begin
			tNxtPixRawMaxX		= 400;
			tNextPixCellMaxX	= 50;
		end
		else
		begin
			tNxtPixRawMaxX		= 320;
			tNextPixCellMaxX	= 40;
		end
	end

	if(useQtrCell)
	begin
		tPixCellIx_A	=
			(tPixCellY*({ 6'h0, tPixCellMaxX }>>2)) +
			(tPixCellX>>2);
		tNextCellIsOdd = tPixCellX[0];
		tNextCellIsOddB = tPixCellX[1];
	end
	else
		if(useHalfCell)
	begin
		tPixCellIx_A	=
			(tPixCellY*({6'h0, tPixCellMaxX}>>1)) +
			(tPixCellX>>1);
		tNextCellIsOdd = tPixCellX[0];
	end
	else
	begin
//		tPixCellIx_A	=
//			(tPixCellY*tPixCellMaxX) +
//			tPixCellX - {6'h0, (tPixCellMaxX<<1)};
		tPixCellIx_A	=
			(tPixCellY*tPixCellMaxX) +
			tPixCellX;
		tNextCellIsOdd = 0;
	end

//	tCellLimitY		= (tPixCellY[5:0] >= tPixCellMaxY) || tPixCellY[6];
	tCellLimitY		= (tPixCellY[7:0] >= tPixCellMaxY) || tPixCellY[8];
	tCellLimitX		= tPixCellX[7:0] >= tPixCellMaxX;
	
	tPixRawLimitX	= tPixRawPosX > tPixRawMaxX;
	tPixRawLimitY	= tPixRawPosY > tPixRawMaxY;
	
	tDoCellCursor =
		(tPixCellX[7:0] == tCellCursorX) &&
		(tPixCellY[7:0] == tCellCursorY);
//		(tPixCellY[7:0] == tCellCursorY) &&
//		((tCellCursorX != 0) || (tCellCursorX != 0));
	
	
	/* Stage A-0 */
	
	if(useRow50)
	begin
//		tPixCellFx_A0[3:2] = 2'h3 - tPixPosY[1:0];
//		tPixCellGx_A0[5:3]	= 3'h7 - { tPixPosY[1:0], pixLineOdd };
		tPixCellFx_A0[3:2] = 2'h3 - tPixPosY[2:1];
		tPixCellGx_A0[5:3]	= 3'h7 - tPixPosY[2:0];
	end
	else
	begin
//		tPixCellFx_A0[3:2] = 2'h3 - tPixPosY[2:1];
//		tPixCellGx_A0[5:3]	= 3'h7 - tPixPosY[2:0];
		tPixCellFx_A0[3:2] = 2'h3 - tPixPosY[3:2];
		tPixCellGx_A0[5:3]	= 3'h7 - tPixPosY[3:1];
	end
	
	if(useCol80)
	begin
		tPixCellFx_A0[1:0] = 2'h3 - tPixPosX[2:1];
		tPixCellGx_A0[2:0]	= 3'h7 - tPixPosX[2:0];
	end
	else
	begin
		tPixCellFx_A0[1:0] = 2'h3 - tPixPosX[3:2];		
		tPixCellGx_A0[2:0]	= 3'h7 - tPixPosX[3:1];
	end
	
//	tCellData = cellData;
	if(useQtrCell)
	begin
		case({tCellIsOddB, tCellIsOdd})
			2'b00: tCellData0 = { UV192_00, cellData[ 63:  0] };
			2'b01: tCellData0 = { UV192_00, cellData[127: 64] };
			2'b10: tCellData0 = { UV192_00, cellData[191:128] };
			2'b11: tCellData0 = { UV192_00, cellData[255:192] };
		endcase
	end
	else
		if(useHalfCell)
	begin
		tCellData0 = tCellIsOdd ?
			{ UV128_00, cellData[255:128] } :
			{ UV128_00, cellData[127:  0] };
	end
	else
	begin
		tCellData0 = cellData;
	end

`ifdef jx2_fbuf_hwsprite

	tStSprCtrl = 0;
	tStSprIdx = 0;
	tDoStSpr = 0;

	if(	tCellLimitY &&
		(cellData[31:30]==2'b00) &&
		(cellData[63:62]==2'b10))
	begin
		tStSprCtrl = cellData[127:0];
		tStSprIdx = cellData[93:88];
		tDoStSpr = 1;
	end

	tNxtSprRov	= tSprRov;
	tDoSprAdv	= 0;
	tReqSprAdv	= 0;
	
	if(tPixRawLimitY)
	begin
		if(tSprRov != 4)
			tReqSprAdv	= 1;
	end
	else
	begin
		if(tPixRelPosY >= (tSpr0PosY+8))
			tReqSprAdv	= 1;

		if((tSpr0Ctrl[63:62]!=2'b10) || (tSpr0Ctrl[31:30]!=2'b00))
			tReqSprAdv	= 1;
	end
		
	if(tReqSprAdv)
	begin
		if(	tDoSpr4GlyphB && tDoSpr4GlyphC &&
			tDoSpr4GlyphD && tDoSpr4GlyphE)
		begin
			tNxtSprRov	= tSprRov + 1;
			tDoSprAdv	= 1;
		end
		else
			if((tSpr4Ctrl[63:62]!=2'b10) || (tSpr4Ctrl[31:30]!=2'b00))
		begin
			tNxtSprRov	= tSprRov + 1;
			tDoSprAdv	= 1;
		end
	end

`endif

//	if(tPixCellIx_B >= 2000)
//		tCellData = 0;
//	if(!useCol80 && (tPixCellIx_B>=1000))
//		tCellData = 0;

	if(tCellLimitX || tCellLimitY)
		tCellData0 = 0;

`ifndef JX2_FBUF_CELLDE1
	tCellData = tCellData0;
	tPixCellFx_A = tPixCellFx_A0;
	tPixCellGx_A = tPixCellGx_A0;
`endif

	/* Stage A-1 */

	tFontGlyph = tCellData[15:0];
	tClrA = tCellData[21:16];
	tClrB = tCellData[27:22];
	
	tClr9A = tCellData[18:10];
	tClr9B = tCellData[27:19];
	
	tClrRgbA = 0;
	tClrRgbB = 0;
	tBlinkStrobeA	= 0;

//	if((tScrMode[3:0]==4'h8) && (tCellData[31]==1'b0))
//		tPixRgb565 = 0;

	if(tCellData[31:30]==2'b00)
	begin
		case(tCellData[15:14])
			2'b00: 	tBlinkStrobeA	= 0;
			2'b01: 	tBlinkStrobeA	= blinkFast;
			2'b10: 	tBlinkStrobeA	= blinkSlow;
			2'b11: 	tBlinkStrobeA	= 0;
		endcase
	end

	if(tCellData[31:30]==2'b10)
	begin
		tBlinkStrobeA	= blinkSlow;
	end
	
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

	if(tPixRgb565)
	begin
		tClrYuvA =
			{ 1'b0,
				tClrRgbA[11: 8], 1'b0,
				tClrRgbA[ 7: 4], 1'b0,
				tClrRgbA[ 3: 0], 1'b0 };
		tClrYuvB =
			{ 1'b0,
				tClrRgbB[11: 8], 1'b0,
				tClrRgbB[ 7: 4], 1'b0,
				tClrRgbB[ 3: 0], 1'b0 };
	end
	else
	begin
		tClrYuvA[15:10]=
			{1'b0, tClrRgbA[ 7: 3]}+
			{2'b0, tClrRgbA[11: 8]}+
			{2'b0, tClrRgbA[ 3: 0]};
		tClrYuvB[15:10]=
			{1'b0, tClrRgbB[ 7: 3]}+
			{2'b0, tClrRgbB[11: 8]}+
			{2'b0, tClrRgbB[ 3: 0]};
	//	tClrYuvA[9:5]=5'h10+(tClrRgbA[ 3: 0]-tClrRgbA[ 7: 4]);
	//	tClrYuvB[9:5]=5'h10+(tClrRgbB[ 3: 0]-tClrRgbB[ 7: 4]);
	//	tClrYuvA[4:0]=5'h10+(tClrRgbA[11: 8]-tClrRgbA[ 7: 4]);
	//	tClrYuvB[4:0]=5'h10+(tClrRgbB[11: 8]-tClrRgbB[ 7: 4]);
		tClrYuvA[9:5]=5'h10+(tClrRgbA[11: 8]-tClrRgbA[ 7: 4]);
		tClrYuvB[9:5]=5'h10+(tClrRgbB[11: 8]-tClrRgbB[ 7: 4]);
		tClrYuvA[4:0]=5'h10+(tClrRgbA[ 3: 0]-tClrRgbA[ 7: 4]);
		tClrYuvB[4:0]=5'h10+(tClrRgbB[ 3: 0]-tClrRgbB[ 7: 4]);
	end

	tFontData_A = fontData;

`ifdef jx2_fbuf_hwsprite
	tNxtSpr4Data	= tSpr4Data;

	if(tDoSpr4GlyphC)
	begin
		if(tDoSpr4GlyphBitL)
			tNxtSpr4Data[ 63: 0]	= fontData;
		else
			tNxtSpr4Data[127:64]	= fontData;
	end
`endif

	tFontGlyphY_A = tCellData[127:64];
	tFontGlyphU_A = tCellData[63:48];
	tFontGlyphV_A = tCellData[47:32];

	if(tCellData[31:30]==2'b10)
	begin
//		tClrYuvB[11:8]=tCellData[27:24];
//		tClrYuvA[11:8]=tCellData[21:18];
//		tClrYuvB[ 7:4]=tCellData[15:12];
//		tClrYuvA[ 7:4]=tCellData[11: 8];
//		tClrYuvB[ 3:0]=tCellData[ 7: 4];
//		tClrYuvA[ 3:0]=tCellData[ 3: 0];

//		tClrYuvB[11:8]=tCellData[29:26];
//		tClrYuvB[ 7:4]=tCellData[24:21];
//		tClrYuvB[ 3:0]=tCellData[19:16];
//		tClrYuvA[11:8]=tCellData[14:11];
//		tClrYuvA[ 7:4]=tCellData[ 9: 6];
//		tClrYuvA[ 3:0]=tCellData[ 4: 1];

		tClrYuvB={ 1'b0, tCellData[29:15] };
		tClrYuvA={ 1'b0, tCellData[14: 0] };
	end


//	case( { tPixCellFx_A[3], tPixCellFx_A[1] } )
	case( { ~tPixCellFx_A[3], ~tPixCellFx_A[1] } )
		2'b00: begin
//		2'b10: begin
			tNextClr33B		= { tCellData[125:111], tCellData[114] };
			tNextClr33A		= { tCellData[110: 96], tCellData[ 99] };
			tNextCellData33	= tCellData[255:224];
			tClr32YD	= tCellData[125:111];
		end
		2'b01: begin
//		2'b11: begin
			tNextClr33B		= { tCellData[ 93: 79], tCellData[82] };
			tNextClr33A		= { tCellData[ 78: 64], tCellData[67] };
			tNextCellData33	= tCellData[223:192];
			tClr32YD	= tCellData[110:96];
		end
		2'b10: begin
//		2'b00: begin
			tNextClr33B		= { tCellData[ 61: 47], tCellData[50] };
			tNextClr33A		= { tCellData[ 46: 32], tCellData[35] };
			tNextCellData33	= tCellData[191:160];
			tClr32YD	= tCellData[93:79];
		end
		2'b11: begin
//		2'b01: begin
			tNextClr33B		= { tCellData[ 29: 15], tCellData[18] };
			tNextClr33A		= { tCellData[ 14:  0], tCellData[ 3] };
			tNextCellData33	= tCellData[159:128];
			tClr32YD	= tCellData[78:64];
		end
	endcase
	
	if(tPixRgb565)
	begin
		tNextClr33A={
			tNextClr33A[15:11],
			tNextClr33A[10:6], tNextClr33A[10],
			tNextClr33A[5:1] };
		tNextClr33B={
			tNextClr33B[15:11],
			tNextClr33B[10:6], tNextClr33B[10],
			tNextClr33B[5:1] };
	end

	tNextClr32CV	= tPixCellFx_A[2] ? tClr33A : tClr33B;
	tNextClr32Dy	= tPixCellFx_A[2] ? tClr32YD[6:0] : tClr32YD[13:7];

//	if(tPixRgb565)
	if(1'b0)
	begin
		tNextClr32A = {
			tClr32CV[15:11] + { 1'b0, tClr32Dy[6:3] },
			tClr32CV[10: 5] + { 1'b0, tClr32Dy[6:2] },
			tClr32CV[ 4: 0] + { 1'b0, tClr32Dy[6:3] } };
		tNextClr32B = {
			tClr32CV[15:11] - { 1'b0, tClr32Dy[6:3] },
			tClr32CV[10: 5] - { 1'b0, tClr32Dy[6:2] },
			tClr32CV[ 4: 0] - { 1'b0, tClr32Dy[6:3] } };

`ifndef def_true
		tNextClr32C = {
			{ 1'b0, tClr32A[15:12] } + { 1'b0, tClr32E[15:12] },
			{ 1'b0, tClr32A[10: 6] } + { 1'b0, tClr32E[10: 6] } ,
			{ 1'b0, tClr32A[ 4: 1] } + { 1'b0, tClr32E[ 4: 1] } };
		tNextClr32D = {
			{ 1'b0, tClr32E[15:12] } + { 1'b0, tClr32B[15:12] },
			{ 1'b0, tClr32E[10: 6] } + { 1'b0, tClr32B[10: 6] } ,
			{ 1'b0, tClr32E[ 4: 1] } + { 1'b0, tClr32B[ 4: 1] } };
		tNextClr32E = {
			{ 1'b0, tClr32A[15:12] } + { 1'b0, tClr32B[15:12] } ,
			{ 1'b0, tClr32A[10: 6] } + { 1'b0, tClr32B[10: 6] } ,
			{ 1'b0, tClr32A[ 4: 1] } + { 1'b0, tClr32B[ 4: 1] } };
`endif
	end
	else
	begin
		tNextClr32A = {
			tClr32CV[15:10] + { 1'b0, tClr32Dy[6:2] },
			tClr32CV[9:0] };
		tNextClr32B = {
			tClr32CV[15:10] - { 1'b0, tClr32Dy[6:2] },
			tClr32CV[9:0] };

`ifndef def_true
		tNextClr32C = {
			{ 1'b0, tClr32A[15:11] } + { 1'b0, tClr32E[15:11] },
			{ 1'b0, tClr32A[ 9: 6] } + { 1'b0, tClr32E[ 9: 6] } ,
			{ 1'b0, tClr32A[ 4: 1] } + { 1'b0, tClr32E[ 4: 1] } };
		tNextClr32D = {
			{ 1'b0, tClr32E[15:11] } + { 1'b0, tClr32B[15:11] },
			{ 1'b0, tClr32E[ 9: 6] } + { 1'b0, tClr32B[ 9: 6] } ,
			{ 1'b0, tClr32E[ 4: 1] } + { 1'b0, tClr32B[ 4: 1] } };
		tNextClr32E = {
			{ 1'b0, tClr32A[15:11] } + { 1'b0, tClr32B[15:11] } ,
			{ 1'b0, tClr32A[ 9: 6] } + { 1'b0, tClr32B[ 9: 6] } ,
			{ 1'b0, tClr32A[ 4: 1] } + { 1'b0, tClr32B[ 4: 1] } };
`endif
	end

	if(tCellData[63:62]==2'b10)
	begin
		tNextClr33A = tNextClr32A;
		tNextClr33B = tNextClr32B;
	end

//	if(tPixRgb565)
	if(1'b0)
	begin
		tNextClr33C = {
			{ 1'b0, tClr33A[15:12] } + { 1'b0, tClr33E[15:12] },
			{ 1'b0, tClr33A[10: 6] } + { 1'b0, tClr33E[10: 6] } ,
			{ 1'b0, tClr33A[ 4: 1] } + { 1'b0, tClr33E[ 4: 1] } };
		tNextClr33D = {
			{ 1'b0, tClr33E[15:12] } + { 1'b0, tClr33B[15:12] },
			{ 1'b0, tClr33E[10: 6] } + { 1'b0, tClr33B[10: 6] } ,
			{ 1'b0, tClr33E[ 4: 1] } + { 1'b0, tClr33B[ 4: 1] } };
		tNextClr33E = {
			{ 1'b0, tClr33A[15:12] } + { 1'b0, tClr33B[15:12] } ,
			{ 1'b0, tClr33A[10: 6] } + { 1'b0, tClr33B[10: 6] } ,
			{ 1'b0, tClr33A[ 4: 1] } + { 1'b0, tClr33B[ 4: 1] } };
	end
	else
	begin
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
	end

	tNextClr32C = tNextClr33C;
	tNextClr32D = tNextClr33D;
	tNextClr32E = tNextClr33E;


	/* Stage B */

//	case( { tPixCellGx_B[4:3], tPixCellGx_B[1:0] } )
	case( { ~tPixCellGx_B[4:3], ~tPixCellGx_B[1:0] } )
		4'h0: tClrNxtIx33=tCellData33[31:30];
		4'h1: tClrNxtIx33=tCellData33[29:28];
		4'h2: tClrNxtIx33=tCellData33[27:26];
		4'h3: tClrNxtIx33=tCellData33[25:24];
		4'h4: tClrNxtIx33=tCellData33[23:22];
		4'h5: tClrNxtIx33=tCellData33[21:20];
		4'h6: tClrNxtIx33=tCellData33[19:18];
		4'h7: tClrNxtIx33=tCellData33[17:16];
		4'h8: tClrNxtIx33=tCellData33[15:14];
		4'h9: tClrNxtIx33=tCellData33[13:12];
		4'hA: tClrNxtIx33=tCellData33[11:10];
		4'hB: tClrNxtIx33=tCellData33[ 9: 8];
		4'hC: tClrNxtIx33=tCellData33[ 7: 6];
		4'hD: tClrNxtIx33=tCellData33[ 5: 4];
		4'hE: tClrNxtIx33=tCellData33[ 3: 2];
		4'hF: tClrNxtIx33=tCellData33[ 1: 0];
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

	case( tPixCellGx_B[5:3])
		3'h0: tPixelData32B=tPixelData32A[63:56];
		3'h1: tPixelData32B=tPixelData32A[55:48];
		3'h2: tPixelData32B=tPixelData32A[47:40];
		3'h3: tPixelData32B=tPixelData32A[39:32];
		3'h4: tPixelData32B=tPixelData32A[31:24];
		3'h5: tPixelData32B=tPixelData32A[23:16];
		3'h6: tPixelData32B=tPixelData32A[15: 8];
		3'h7: tPixelData32B=tPixelData32A[ 7: 0];
	endcase
	case(tPixCellGx_B[2:1])
		2'h3: tClrNxtIx32=tPixelData32B[7:6];
		2'h2: tClrNxtIx32=tPixelData32B[5:4];
		2'h1: tClrNxtIx32=tPixelData32B[3:2];
		2'h0: tClrNxtIx32=tPixelData32B[1:0];
	endcase


	/* Stage C */
	
	tClrYuvC_C = UV16_00;

	if(tCellData_C[31:30]==2'b11)
	begin
//		if(tCellData_C[29:28]==2'b01)
		if(tCellData_C[63:62]==2'b01)
		begin
			tClrYuvC_C = (pixLineOdd) ?
				((tClrIx33[0]) ? tClr33A : tClr33B) :
				((tClrIx33[1]) ? tClr33A : tClr33B) ;
		end
		else
			if(tCellData_C[63:62]==2'b10)
		begin
			case(tClrIx32)
				2'b00: tClrYuvC_C=tClr32B;
				2'b01: tClrYuvC_C=tClr32D;
				2'b10: tClrYuvC_C=tClr32C;
				2'b11: tClrYuvC_C=tClr32A;
			endcase
		end
		else
			if(tCellData_C[63:62]==2'b11)
		begin
			case(tClrIx33)
				2'b00: tClrYuvC_C=tClr33B;
				2'b01: tClrYuvC_C=tClr33D;
				2'b10: tClrYuvC_C=tClr33C;
				2'b11: tClrYuvC_C=tClr33A;
			endcase
		end
	end
	else
	if(tCellData_C[31:30]==2'b10)
	begin
		begin
//			tClrYuvC_C[15:10] = (tFontGlyphY_C[tPixCellGx_C]) ?
//				tClrYuvA_C[15:10] : tClrYuvB_C[15:10];
//			tClrYuvC_C[9:5] = (tFontGlyphU_C[tPixCellFx_C]) ?
//				tClrYuvA_C[9:5] : tClrYuvB_C[9:5];
//			tClrYuvC_C[4:0] = (tFontGlyphV_C[tPixCellFx_C]) ?
//				tClrYuvA_C[4:0] : tClrYuvB_C[4:0];

			tClrYuvC_C = (tFontGlyphY_C[tPixCellGx_C]) ?
				tClrYuvA_C : tClrYuvB_C;
		end

	end
	else
	begin
		if(tCellData_C[31:30]==2'b00)
			tClrYuvC_C = (tFontData_C[tPixCellGx_C]) ?
				tClrYuvA_C : tClrYuvB_C;
		else if(tCellData_C[31:30]==2'b01)
			tClrYuvC_C = (tFontGlyph_C[tPixCellFx_C]) ?
				tClrYuvA_C : tClrYuvB_C;
		
		if(tDoCellCursor && blinkSlow)
		begin
//			if(tPixCellFx_C[3:2]==2'b00)
			if(tPixCellGx_C[5:3]==3'b001)
				tClrYuvC_C = tClrYuvA_C;
		end
	end

`ifdef JX2_FBUF_ENBM

	case(tPixCellFx_C)
		4'hF: tPixClrBmYv16_C = tCellData_C[ 15:  0];
		4'hE: tPixClrBmYv16_C = tCellData_C[ 31: 16];
		4'hD: tPixClrBmYv16_C = tCellData_C[ 47: 32];
		4'hC: tPixClrBmYv16_C = tCellData_C[ 63: 48];
		4'hB: tPixClrBmYv16_C = tCellData_C[ 79: 64];
		4'hA: tPixClrBmYv16_C = tCellData_C[ 95: 80];
		4'h9: tPixClrBmYv16_C = tCellData_C[111: 96];
		4'h8: tPixClrBmYv16_C = tCellData_C[127:112];
		4'h7: tPixClrBmYv16_C = tCellData_C[143:128];
		4'h6: tPixClrBmYv16_C = tCellData_C[159:144];
		4'h5: tPixClrBmYv16_C = tCellData_C[175:160];
		4'h4: tPixClrBmYv16_C = tCellData_C[191:176];
		4'h3: tPixClrBmYv16_C = tCellData_C[207:192];
		4'h2: tPixClrBmYv16_C = tCellData_C[223:208];
		4'h1: tPixClrBmYv16_C = tCellData_C[239:224];
		4'h0: tPixClrBmYv16_C = tCellData_C[255:240];
	endcase

	case(tPixCellFx_C)
		4'hF: tPixClrBmYv8_C = tCellData_C[  7:  0];
		4'hE: tPixClrBmYv8_C = tCellData_C[ 15:  8];
		4'hD: tPixClrBmYv8_C = tCellData_C[ 23: 16];
		4'hC: tPixClrBmYv8_C = tCellData_C[ 31: 24];
		4'hB: tPixClrBmYv8_C = tCellData_C[ 39: 32];
		4'hA: tPixClrBmYv8_C = tCellData_C[ 47: 40];
		4'h9: tPixClrBmYv8_C = tCellData_C[ 55: 48];
		4'h8: tPixClrBmYv8_C = tCellData_C[ 63: 56];
		4'h7: tPixClrBmYv8_C = tCellData_C[ 71: 64];
		4'h6: tPixClrBmYv8_C = tCellData_C[ 79: 72];
		4'h5: tPixClrBmYv8_C = tCellData_C[ 87: 80];
		4'h4: tPixClrBmYv8_C = tCellData_C[ 95: 88];
		4'h3: tPixClrBmYv8_C = tCellData_C[103: 96];
		4'h2: tPixClrBmYv8_C = tCellData_C[111:104];
		4'h1: tPixClrBmYv8_C = tCellData_C[119:112];
		4'h0: tPixClrBmYv8_C = tCellData_C[127:120];
	endcase

	case(tPixCellFx_C)
		4'hF: tPixClrBmYv4_C = tCellData_C[ 3: 0];
		4'hE: tPixClrBmYv4_C = tCellData_C[ 7: 4];
		4'hD: tPixClrBmYv4_C = tCellData_C[11: 8];
		4'hC: tPixClrBmYv4_C = tCellData_C[15:12];
		4'hB: tPixClrBmYv4_C = tCellData_C[19:16];
		4'hA: tPixClrBmYv4_C = tCellData_C[23:20];
		4'h9: tPixClrBmYv4_C = tCellData_C[27:24];
		4'h8: tPixClrBmYv4_C = tCellData_C[31:28];
		4'h7: tPixClrBmYv4_C = tCellData_C[35:32];
		4'h6: tPixClrBmYv4_C = tCellData_C[39:36];
		4'h5: tPixClrBmYv4_C = tCellData_C[43:40];
		4'h4: tPixClrBmYv4_C = tCellData_C[47:44];
		4'h3: tPixClrBmYv4_C = tCellData_C[51:48];
		4'h2: tPixClrBmYv4_C = tCellData_C[55:52];
		4'h1: tPixClrBmYv4_C = tCellData_C[59:56];
		4'h0: tPixClrBmYv4_C = tCellData_C[63:60];
	endcase

	case( { tPixCellGx_C[3], tPixCellGx_C[0] } )
//		2'b00: tPixClrBmRgbi = tPixClrBmYv16_C[ 3: 0];
//		2'b01: tPixClrBmRgbi = tPixClrBmYv16_C[ 7: 4];
//		2'b10: tPixClrBmRgbi = tPixClrBmYv16_C[11: 8];
//		2'b11: tPixClrBmRgbi = tPixClrBmYv16_C[15:12];

		2'b11: tPixClrBmRgbi = tPixClrBmYv16_C[ 3: 0];
		2'b10: tPixClrBmRgbi = tPixClrBmYv16_C[ 7: 4];
		2'b01: tPixClrBmRgbi = tPixClrBmYv16_C[11: 8];
		2'b00: tPixClrBmRgbi = tPixClrBmYv16_C[15:12];
	endcase

	case( { tPixCellGx_C[3], tPixCellGx_C[0] } )
//		2'b00: tPixClrBm2i = tPixClrBmYv8_C[1:0];
//		2'b01: tPixClrBm2i = tPixClrBmYv8_C[3:2];
//		2'b10: tPixClrBm2i = tPixClrBmYv8_C[5:4];
//		2'b11: tPixClrBm2i = tPixClrBmYv8_C[7:6];

		2'b11: tPixClrBm2i = tPixClrBmYv8_C[1:0];
		2'b10: tPixClrBm2i = tPixClrBmYv8_C[3:2];
		2'b01: tPixClrBm2i = tPixClrBmYv8_C[5:4];
		2'b00: tPixClrBm2i = tPixClrBmYv8_C[7:6];
	endcase

	case( { tPixCellGx_C[3], tPixCellGx_C[0] } )
		2'b11: tPixClrBm1i = tPixClrBmYv4_C[0];
		2'b10: tPixClrBm1i = tPixClrBmYv4_C[1];
		2'b01: tPixClrBm1i = tPixClrBmYv4_C[2];
		2'b00: tPixClrBm1i = tPixClrBmYv4_C[3];
	endcase

	tScrClrsModB = tScrClrsMod;
	tScrClrsModC = tScrClrsMod;

	case(tScrClrsMod)
		4'h0:		tScrClrsModC = 4'hF;
		4'h8:		tScrClrsModC = 4'h2;
		4'h9:		tScrClrsModC = 4'h6;
		4'hA:		tScrClrsModC = 4'h3;
		4'hB:		tScrClrsModC = 4'h5;
		default:	tScrClrsModC = tScrClrsMod;
	endcase

	if(tScrClrsMod[3:2]==2'b11)
	begin
		case( { tScrClrsMod[1:0], tPixCellGx_C[3] ^ tPixCellGx_C[0] } )
			3'b000: tScrClrsModB = 4'hB;
			3'b001: tScrClrsModB = 4'h6;
			3'b010: tScrClrsModB = 4'h7;
			3'b011: tScrClrsModB = 4'h6;
			3'b100: tScrClrsModB = 4'h5;
			3'b101: tScrClrsModB = 4'h4;
			3'b110: tScrClrsModB = 4'h3;
			3'b111: tScrClrsModB = 4'h2;
		endcase

		case( { tScrClrsMod[1:0], tPixCellGx_C[3], tPixCellGx_C[0] } )
			4'b0000: tScrClrsModC = 4'h2;
			4'b0001: tScrClrsModC = 4'h1;
			4'b0010: tScrClrsModC = 4'h4;
			4'b0011: tScrClrsModC = 4'h2;

			4'b0100: tScrClrsModC = 4'h3;
			4'b0101: tScrClrsModC = 4'h1;
			4'b0110: tScrClrsModC = 4'h4;
			4'b0111: tScrClrsModC = 4'h6;

			4'b1000: tScrClrsModC = 4'h2;
			4'b1001: tScrClrsModC = 4'h3;
			4'b1010: tScrClrsModC = 4'h5;
			4'b1011: tScrClrsModC = 4'h6;

			4'b1100: tScrClrsModC = 4'h7;
			4'b1101: tScrClrsModC = 4'h3;
			4'b1110: tScrClrsModC = 4'h5;
			4'b1111: tScrClrsModC = 4'h6;
		endcase
	end

	if(useQtrCell)
	begin
		case(tScrMode)
			4'h1: begin
				tPixClrBmRgbi = tPixClrBmYv4_C;
			end
			4'h2: begin
				tPixClrBmRgbi = tPixClrBm1i ? tScrClrsModC : 4'h0;
			end
			default: begin
			end
		endcase
	end

//	case(tPixClrBmRgbi)
	case(tPixClrBmRgbiB)
		4'h0:	tPixClrBmRgbiYV12 = 12'h000;
		4'h1:	tPixClrBmRgbiYV12 = 12'h00A;
		4'h2:	tPixClrBmRgbiYV12 = 12'h0A0;
		4'h3:	tPixClrBmRgbiYV12 = 12'h0AA;
		4'h4:	tPixClrBmRgbiYV12 = 12'hA00;
		4'h5:	tPixClrBmRgbiYV12 = 12'hA0A;
		4'h6:	tPixClrBmRgbiYV12 = 12'hAA0;
		4'h7:	tPixClrBmRgbiYV12 = 12'hAAA;
		4'h8:	tPixClrBmRgbiYV12 = 12'h555;
		4'h9:	tPixClrBmRgbiYV12 = 12'h55F;
		4'hA:	tPixClrBmRgbiYV12 = 12'h5F5;
		4'hB:	tPixClrBmRgbiYV12 = 12'h5FF;
		4'hC:	tPixClrBmRgbiYV12 = 12'hF55;
		4'hD:	tPixClrBmRgbiYV12 = 12'hF5F;
		4'hE:	tPixClrBmRgbiYV12 = 12'hFF5;
		4'hF:	tPixClrBmRgbiYV12 = 12'hFFF;
	endcase

	casez( { tScrClrsModB, tPixClrBm2i } )
		//Palette 0
		6'b000000:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b000001:	tPixClrBm2iYV12 = 12'h555;	//Dark Gray
		6'b000010:	tPixClrBm2iYV12 = 12'hAAA;	//Light Gray
		6'b000011:	tPixClrBm2iYV12 = 12'hFFF;	//White
		//Palette 1
		6'b000100:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b000101:	tPixClrBm2iYV12 = 12'h0F0;	//Green
		6'b000110:	tPixClrBm2iYV12 = 12'hF00;	//Red
		6'b000111:	tPixClrBm2iYV12 = 12'hFF0;	//Yellow
		//Palette 2
		6'b001000:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b001001:	tPixClrBm2iYV12 = 12'h0F0;	//Green
		6'b001010:	tPixClrBm2iYV12 = 12'hF00;	//Red
		6'b001011:	tPixClrBm2iYV12 = 12'h00F;	//Blue
		//Palette 3
		6'b001100:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b001101:	tPixClrBm2iYV12 = 12'h0FF;	//Cyan
		6'b001110:	tPixClrBm2iYV12 = 12'hF0F;	//Magenta
		6'b001111:	tPixClrBm2iYV12 = 12'hFF0;	//Yellow
		//Palette 4
		6'b010000:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b010001:	tPixClrBm2iYV12 = 12'h0FF;	//Cyan
		6'b010010:	tPixClrBm2iYV12 = 12'hFF0;	//Yellow
		6'b010011:	tPixClrBm2iYV12 = 12'hFFF;	//White
		//Palette 5
		6'b010100:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b010101:	tPixClrBm2iYV12 = 12'hF0F;	//Magenta
		6'b010110:	tPixClrBm2iYV12 = 12'hFF0;	//Yellow
		6'b010111:	tPixClrBm2iYV12 = 12'hFFF;	//White
		//Palette 6
		6'b011000:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b011001:	tPixClrBm2iYV12 = 12'h0F0;	//Green
		6'b011010:	tPixClrBm2iYV12 = 12'hF00;	//Red
		6'b011011:	tPixClrBm2iYV12 = 12'hFFF;	//White
		//Palette 7
		6'b011100:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b011101:	tPixClrBm2iYV12 = 12'h0FF;	//Cyan
		6'b011110:	tPixClrBm2iYV12 = 12'hF0F;	//Magenta
		6'b011111:	tPixClrBm2iYV12 = 12'hFFF;	//White
		//Palette 8
		6'b100000:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b100001:	tPixClrBm2iYV12 = 12'h050;	//Dark Olive
		6'b100010:	tPixClrBm2iYV12 = 12'h5A0;	//Medium Olive
		6'b100011:	tPixClrBm2iYV12 = 12'hAF0;	//Olive
		//Palette 9
		6'b100100:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b100101:	tPixClrBm2iYV12 = 12'h500;	//Dark Sepia
		6'b100110:	tPixClrBm2iYV12 = 12'hA50;	//Light Sepia
		6'b100111:	tPixClrBm2iYV12 = 12'hFA0;	//Sepia
		//Palette 10
		6'b101000:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b101001:	tPixClrBm2iYV12 = 12'h050;	//Dark Azure
		6'b101010:	tPixClrBm2iYV12 = 12'h0A5;	//Light Azure
		6'b101011:	tPixClrBm2iYV12 = 12'h0FA;	//Azure
		//Palette 11
		6'b101100:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b101101:	tPixClrBm2iYV12 = 12'h0F0;	//Green
		6'b101110:	tPixClrBm2iYV12 = 12'h00F;	//Blue
		6'b101111:	tPixClrBm2iYV12 = 12'hFFF;	//White
		//Palette 12-15
		6'b11zz00:	tPixClrBm2iYV12 = 12'h000;	//Black
		6'b11zz01:	tPixClrBm2iYV12 = 12'h555;	//Dark Gray
		6'b11zz10:	tPixClrBm2iYV12 = 12'hAAA;	//Light Gray
		6'b11zz11:	tPixClrBm2iYV12 = 12'hFFF;	//White
	endcase
	
//	tPixClrBmRgbiYV16_C = {
//		tPixClrBmRgbiYV12[11:8], tPixClrBmRgbiYV12[ 11],
//		tPixClrBmRgbiYV12[ 7:4], tPixClrBmRgbiYV12[7:6],
//		tPixClrBmRgbiYV12[ 3:0], tPixClrBmRgbiYV12[  3]
//	};
//	tPixClrBm2iYV16_C = {
//		tPixClrBm2iYV12[11:8], tPixClrBm2iYV12[ 11],
//		tPixClrBm2iYV12[ 7:4], tPixClrBm2iYV12[7:6],
//		tPixClrBm2iYV12[ 3:0], tPixClrBm2iYV12[  3]
//	};

	tPixClrBmRgbiYV16_C = {
		1'b0,
		tPixClrBmRgbiYV12[11:8], tPixClrBmRgbiYV12[11],
		tPixClrBmRgbiYV12[ 7:4], tPixClrBmRgbiYV12[ 7],
		tPixClrBmRgbiYV12[ 3:0], tPixClrBmRgbiYV12[ 3]
	};
	tPixClrBm2iYV16_C = {
		1'b0,
		tPixClrBm2iYV12[11:8], tPixClrBm2iYV12[11],
		tPixClrBm2iYV12[ 7:4], tPixClrBm2iYV12[ 7],
		tPixClrBm2iYV12[ 3:0], tPixClrBm2iYV12[ 3]
	};

`ifndef def_true
	case(tPixClrBmYv8_C[1:0])
		2'b00: tPixClrBmPalYV16_C=tFontData_C[15: 0];
		2'b01: tPixClrBmPalYV16_C=tFontData_C[31:16];
		2'b10: tPixClrBmPalYV16_C=tFontData_C[47:32];
		2'b11: tPixClrBmPalYV16_C=tFontData_C[63:48];
	endcase
`endif

`ifdef def_true
	if(tPixClrBmYv8_C[7])
	begin
		tPixClrBmPalYV16_C={
			1'b0,
			tPixClrBmYv8_C[6] ?
				{ tPixClrBmYv8_C[3:0], tPixClrBmYv8_C[3] } : 5'h00,
			tPixClrBmYv8_C[5] ?
				{ tPixClrBmYv8_C[3:0], tPixClrBmYv8_C[3] } : 5'h00,
			tPixClrBmYv8_C[4] ?
				{ tPixClrBmYv8_C[3:0], tPixClrBmYv8_C[3] } : 5'h00
			};
	end
	else
	begin
		tPixClrBmPalYV16_C={
			1'b0,
			tPixClrBmYv8_C[6:5], tPixClrBmYv8_C[6:5], tPixClrBmYv8_C[6],
			tPixClrBmYv8_C[4:2], tPixClrBmYv8_C[4:3],
			tPixClrBmYv8_C[1:0], tPixClrBmYv8_C[1:0], tPixClrBmYv8_C[1]
			};
	end
`endif

	if((tScrMode!=0) && useHalfCell)
	begin
		tFontGlyph = { 10'b0000001111, tPixClrBmYv8_C[7:2] };
	end

`ifdef jx2_fbuf_hwsprite

	tDoSpr4Glyph		= 0;
	tDoSpr4GlyphBit		= !tDoSpr4GlyphBitL;

	if(tPixRawLimitX)
	begin
		tFontGlyph		= tDoSpr4GlyphBitL ? tSpr4GlyphB : tSpr4GlyphA;

//		tFontGlyph		= tSpr4Glyph;
//		tFontGlyph[0]	= tSpr4Glyph[0] ^ tDoSpr4GlyphBitL;
		tDoSpr4Glyph	= 1;
	end
	
	if(tDoSprAdv)
		tDoSpr4Glyph	= 0;

`endif


	/* Stage D */

	case(tScrMode)
		4'h0: begin
			tClrYuvC	= tClrYuvC_D;
		end

		4'h1: begin
//			tClrYuvC	= tPixClrBmYv16_D;
			tClrYuvC	= useHalfCell ?
				tPixClrBmPalYV16_D : tPixClrBmYv16_D;
			if(useQtrCell)
				tClrYuvC	= tPixClrBmRgbiYV16_D;
			tPixRgb565	= 0;
		end
		4'h2: begin
			tClrYuvC	= useHalfCell ?
				tPixClrBm2iYV16_D : tPixClrBmRgbiYV16_D;
			if(useQtrCell)
				tClrYuvC	= tPixClrBmRgbiYV16_D;
			tPixRgb565	= 1;
		end
		4'h8: begin
			tClrYuvC	= tClrYuvC_D;
			tPixRgb565	= 1;
		end
		4'h9: begin
//			tClrYuvC	= tPixClrBmYv16_D;
			tClrYuvC	= useHalfCell ? tPixClrBmPalYV16_D : tPixClrBmYv16_D;
			tPixRgb565	= 1;
		end
		4'hA: begin
			tClrYuvC	= useHalfCell ?
				tPixClrBm2iYV16_D : tPixClrBmRgbiYV16_D;
			tPixRgb565	= 1;
		end
		
		default: begin
		end
	endcase
`endif

	if(tPixRgb565)
	begin
//		tPixCy		= { tClrYuvC[10: 5], tClrYuvC[10: 9] };
		tPixCy		= { tClrYuvC[ 9: 4], tClrYuvC[ 9: 8] };
		tPixCu		= { tClrYuvC[ 4: 0], tClrYuvC[ 4: 2] };
//		tPixCv		= { tClrYuvC[15:11], tClrYuvC[15:13] };
		tPixCv		= { tClrYuvC[14:10], tClrYuvC[14:12] };
		tPixAux[0]	= 1;

//		tPixCu=0;
	end
	else
	begin
		tPixCy		= { tClrYuvC[15:10], tClrYuvC[15:14] };
//		tPixCu		= { tClrYuvC[ 9: 5], tClrYuvC[ 9: 7] };
//		tPixCv		= { tClrYuvC[ 4: 0], tClrYuvC[ 4: 2] };
		tPixCv		= { tClrYuvC[ 9: 5], tClrYuvC[ 9: 7] };
		tPixCu		= { tClrYuvC[ 4: 0], tClrYuvC[ 4: 2] };
		tPixAux[0]	= 0;
		
//		tPixCu=128;
	end

`ifdef jx2_fbuf_hwsprite
	tNxtSprDrawBit	= tSprDrawBit + 1;

	tSprCurSelBit	= tSprCurDataSel[ { tSprCurRelY[2:0], tSprCurRelY[2:0] } ];
	tSprCurMskBit	= tSprCurDataMsk[ { tSprCurRelY[2:0], tSprCurRelY[2:0] } ];

	tSprCurSelClr	= (tSprDrawBit != 0) ? tSprCurSelClrL : 0;
	
	if((tSprCurRelX[11:3]==0) && (tSprCurRelY[11:3]==0) && tSprCurMskBit)
	begin
		tSprCurSelClr	= {
			1'b1,
			tSprCurSelBit ?
				tSprCurColorA :
				tSprCurColorB
			};
	end

	if(tSprCurSelClrL[15])
	begin
		tPixCy		= { tSprCurSelClrL[ 9: 4], tSprCurSelClrL[ 9: 8] };
		tPixCu		= { tSprCurSelClrL[ 4: 0], tSprCurSelClrL[ 4: 2] };
		tPixCv		= { tSprCurSelClrL[14:10], tSprCurSelClrL[14:12] };
		tPixAux[0]	= 1;
	end
`endif

//	tPixAux[1]	= (tPixCellGx_C != tPixCellGx_B);
	tPixAux[1]	= (tPixCellGx_D != tPixCellGx_C);
//	tPixAux[1]	= (tPixCellGx_E != tPixCellGx_D);

	tPixAux[2]	= tPixCellGx_D[0];
	
	if(tPixPosX[11] || tPixPosY[11])
	begin
		if(tPixRgb565)
		begin
			tPixCy=0;
			tPixCu=0;
			tPixCv=0;
		end
		else
		begin
			tPixCy=0;
			tPixCu=128;
			tPixCv=128;
		end
	end

`ifndef def_true
// `ifdef def_true
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
	tPixCy2			<= tPixCy;
	tPixCu2			<= tPixCu;
	tPixCv2			<= tPixCv;
	tPixAux2		<= tPixAux;

//	tPixPosX_Z		<= pixPosX;
//	tPixPosY_Z		<= pixPosY;

	tPixRawUsPosX	<= pixPosX;
	tPixRawUsPosY	<= pixPosY;
	tPixRawPosX		<= tNxtPixRawPosX;
	tPixRawPosY		<= tNxtPixRawPosY;
	tPixRelPosX		<= tNxtPixRelPosX;
	tPixRelPosY		<= tNxtPixRelPosY;

	tPixRawMaxX		<= tNxtPixRawMaxX;
	tPixRawMaxY		<= tNxtPixRawMaxY;


	tPixPosX_Z		<= pixPosX + {
		tCtrlRegVal[55] ? 4'hF : 4'h0,
		tCtrlRegVal[55:48] };
	tPixPosY_Z		<= pixPosY + {
		tCtrlRegVal[63] ? 4'hF : 4'h0,
		tCtrlRegVal[63:56] };

	tCtrlRegVal		<= ctrlRegVal;

//	tPixPosX		<= tPixPosX_Z;
//	tPixPosY		<= tPixPosY_Z;

//	tPixPosX_Y		<= tPixPosX_Z;
//	tPixPosY_Y		<= tPixPosY_Z;
//	tPixPosX		<= tPixPosX_Y;
//	tPixPosY		<= tPixPosY_Y;

	tPixPosX_Y		<= tPixPosX_Z;
	tPixPosY_Y		<= tPixPosY_Z;
	tPixPosX_X		<= tPixPosX_Y;
	tPixPosY_X		<= tPixPosY_Y;
	tPixPosX		<= tPixPosX_X;
	tPixPosY		<= tPixPosY_X;

	tPixCellMaxX	<= tNextPixCellMaxX;
	tPixCellMaxY	<= tNextPixCellMaxY;

	tPixCellIx_B		<= tPixCellIx_A;
	tPixCellFx_B		<= tPixCellFx_A;
	tPixCellGx_B		<= tPixCellGx_A;

	tPixCellIx_C		<= tPixCellIx_B;
	tPixCellFx_C		<= tPixCellFx_B;
	tPixCellGx_C		<= tPixCellGx_B;

	tPixCellGx_D		<= tPixCellGx_C;
	tPixCellGx_E		<= tPixCellGx_D;

	tCellIsOdd			<= tNextCellIsOdd;
	tCellIsOddB			<= tNextCellIsOddB;
	tBlinkStrobeB		<= tBlinkStrobeA;

`ifdef JX2_FBUF_CELLDE1
	tCellData		<= tCellData0;
	tPixCellFx_A	<= tPixCellFx_A0;
	tPixCellGx_A	<= tPixCellGx_A0;
`endif

	tCellData_B		<= tCellData;
	tCellData_C		<= tCellData_B;

//	tFontData_B		<= tFontData_A;
//	tFontData_C		<= (tCellData_B[31:30]==2'b10) ? tFontData_B : fontData;
//	tFontData_C		<= fontData;
	tFontData_C		<= tBlinkStrobeB ? UV64_00 : fontData;
	tFontGlyph_C	<= tFontGlyph;

	tFontGlyphY_B	<= tFontGlyphY_A;
	tFontGlyphU_B	<= tFontGlyphU_A;
	tFontGlyphV_B	<= tFontGlyphV_A;

	tFontGlyphY_C	<= tFontGlyphY_B;
	tFontGlyphU_C	<= tFontGlyphU_B;
	tFontGlyphV_C	<= tFontGlyphV_B;

	tClrIx33		<= tClrNxtIx33;
	tClrIx32		<= tClrNxtIx32;

	tClr33A			<= tNextClr33A;		//A
	tClr33B			<= tNextClr33B;		//B
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
	
	tPixClrBmYv16_D		<= tPixClrBmYv16_C;
	tPixClrBmYv8_D		<= tPixClrBmYv8_C;
	tPixClrBmRgbiYV16_D	<= tPixClrBmRgbiYV16_C;
	tPixClrBm2iYV16_D	<= tPixClrBm2iYV16_C;
	tPixClrBmPalYV16_D	<= tPixClrBmPalYV16_C;

	tPixClrBmRgbiB		<= tPixClrBmRgbi;

	tClrYuvA_B			<= tClrYuvA;
	tClrYuvB_B			<= tClrYuvB;
	tClrYuvA_C			<= tClrYuvA_B;
	tClrYuvB_C			<= tClrYuvB_B;
	tClrYuvC_D			<= tClrYuvC_C;

`ifdef jx2_fbuf_hwsprite
	if(tDoStSpr)
	begin
		tSprCtrlArr[tStSprIdx]	<= tStSprCtrl;
	end

	tSprRov		<= tNxtSprRov;
	tSprCtrl	<= tSprCtrlArr[tSprRov];

	if(tDoSprAdv)
	begin
		tSpr0Ctrl	<= tSpr1Ctrl;
		tSpr0Data	<= tSpr1Data;
		tSpr1Ctrl	<= tSpr2Ctrl;
		tSpr1Data	<= tSpr2Data;
		tSpr2Ctrl	<= tSpr3Ctrl;
		tSpr2Data	<= tSpr3Data;
		tSpr3Ctrl	<= tSpr4Ctrl;
		tSpr3Data	<= tSpr4Data;

		tSpr4Ctrl	<= tSprCtrl;
//		tSpr4Data	<= tSpr1Data;
		tSprRov		<= tNxtSprRov;
	end

	tDoSpr4GlyphB		<= tDoSpr4Glyph;
	tDoSpr4GlyphC		<= tDoSpr4GlyphB;
	tDoSpr4GlyphD		<= tDoSpr4GlyphC;
	tDoSpr4GlyphE		<= tDoSpr4GlyphD;
	tDoSpr4GlyphBitL	<= tDoSpr4GlyphBit;

	tSpr4Data		<= tNxtSpr4Data;

	tSprCurSelClrL	<= tSprCurSelClr;
	tSprDrawBit		<= tNxtSprDrawBit;

	tSprCurCtrl		<= tNxtSprCurCtrl;
	tSprCurData		<= tNxtSprCurData;
	tSprCurRelX		<= tNxtSprCurRelX;
	tSprCurRelY		<= tNxtSprCurRelY;

`endif

end

endmodule
