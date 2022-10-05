/*
Copyright (C) 2022  Brendan G Bohannon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*
( 7: 0): Side
(15: 8): Top
(23:16): Bottom, If(Side!=Top), Else Flags
(31:24): Flags

Flags:
  0x01: Not Drawn
  0x02: See-Through / Transparent
  0xC0: Solid Type
    0: Solid
    1: Non-Solid
    2: Fluid
 */

static const u32 btmgl_vox_atlas_side[256]={
0x05000000,		/* 00: null */
0x05000000,		/* 01: air (cave) */
0x05000000,		/* 02: air (open) */
0x05000000,		/* 03: air (open) */

0x00000101,		/* 04: hardstone */
0x00000202,		/* 05: dirt */
0x00000303,		/* 06: grass */
0x00000404,		/* 07: stone */

0x00001212, 0x00001313, 0x0000FFFF, 0x02000F0F,		/* 08-0B */
0x08007171, 0x08007373, 0x08009191, 0x00002323,		/* 0C-0F */
0x00002424, 0x00002525, 0x00002626, 0x00002727,		/* 10-13 */
0x00002828, 0x00002929, 0x02001F1F, 0x02000E0E,		/* 14-17 */
0x00000505, 0x00001414, 0x00001515, 0x00003C3C,		/* 18-1B */
0x00001616, 0x00000707, 0x00000606, 0x00001717,		/* 1C-1F */
0x00002020, 0x00002121, 0x0100E0E0, 0x0100E1E1,		/* 20-23 */
0x0100F1F1, 0x02001E1E, 0x02002E2E, 0x02002F2F,		/* 24-27 */
0x00001818, 0x00000808, 0x00000909, 0x00001919,		/* 28-2B */
0x00000A0A, 0x00001A1A, 0x14002A2A, 0x00000B0B,		/* 2C-2F */
0x00001B1B, 0x36003E3E, 0x00003333, 0x00003434,		/* 30-33 */
0x00003535, 0x00003636, 0x00003737, 0x00003838,		/* 34-37 */
0x00003939, 0x20000B0B, 0x00002B2B, 0x10003A3A,		/* 38-3B */
0x10003B3B, 0x0600EDED, 0x00001C1C, 0x00002C2C,		/* 3C-3F */
0x00003232, 0x00004242, 0x00003030, 0x00003434,		/* 40-43 */
0x00004444, 0x00004545, 0x00001414, 0x00001313,		/* 44-47 */
0x00005353, 0x00005454, 0x00005151, 0x00005252,		/* 48-4B */
0x00005555, 0x00005656, 0x06004F4F, 0x14003C3C,		/* 4C-4F */
0x00001010,	0x00001111,	0x46005E5E, 0x46006E6E,		/* 50-53 */
0x46005F5F, 0x46006F6F, 0x46007E7E, 0x46007F7F,		/* 54-57 */
};

static const byte btmgl_vox_hrsc[256]={
0x00, 0x00, 0x00, 0x00, 0x7F, 0x10, 0x10, 0x7F,
0x7F, 0x7F, 0x00, 0x5F, 0x17, 0x17, 0x17, 0x5F,
0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x07, 0x07,
0x7F, 0x7F, 0x7F, 0x10, 0x10, 0x10, 0x10, 0x10,
0x10, 0x10, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07,
0x7F, 0x30, 0x10, 0x10, 0x7F, 0x17, 0x10, 0x7F,
0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x3F, 0x7F, 0x5F, 0x1F, 0x00, 0x30, 0x30,
0x00, 0x7F, 0x10, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
0x7F, 0x7F, 0x7F, 0x7F, 0x10, 0x10, 0x10, 0x10,
0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


int BTM_BlockIsFluidP(BTM_World *wrl, u32 blk)
{
	u32 blkd;
	
	blkd=btmgl_vox_atlas_side[blk&255];
	if(blkd&BTM_BLKDFL_FLUID)
		return(1);
	return(0);
}

int BTM_BlockIsAirP(BTM_World *wrl, u32 blk)
{
	int j;
	
	j=blk&255;
	return((j>0)&(j<4));
//	u32 blkd;
//	blkd=btmgl_vox_atlas_side[blk&255];
//	if(blkd&BTM_BLKDFL_FLUID)
//		return(1);
//	return(0);
}

int BTM_BlockIsTransparentP(BTM_World *wrl, u32 blk)
{
	u32 blkd;
	int j;
	
	j=blk&255;
	if((j>0)&&(j<4))
		return(1);
	
	blkd=btmgl_vox_atlas_side[j];
	if(blkd&(
			BTM_BLKDFL_FLUID|BTM_BLKDFL_NODRAW|
			BTM_BLKDFL_SEETHRU|BTM_BLKDFL_TY_MASK))
		return(1);
	return(0);
}


int BTM_BlockIsOrientedP(BTM_World *wrl, u32 blk)
{
	u32 blkd;
	int j;

	j=blk&255;
	if((j>0)&&(j<4))
		return(1);
	
	blkd=btmgl_vox_atlas_side[j];

	if((blkd&BTM_BLKDFL_TY_MASK)==BTM_BLKDFL_TY_STAIR)
		return(1);

	return(0);
}

char *btm_blkmininames[256]={
"Void",		"AirCave",	"Air",		"AirSky",		//00..03
"HrdStn",	"Dirt",		"Grass",	"Stone",		//04..07
"BrkRed",	"DirtRed",	"NoFace",	"Glass",		//08..0B
"Lava",		"Water",	"Slime",	"LantRed",		//0C..0F
"LantBlu",	"LantYel",	"LantGrn",	"LantCyn",		//10..13
"LantVio",	"LantWht",	"Grate",	"Bars",			//14..17
"Stone2",	"Stone3",	"BgBrkStn",	"Hay",			//18..1B
"TriDirt0",	"TriDirt1",	"TriDirt2",	"TriDirt3",		//1C..1F
"FarmWet",	"FarmDry",	"Sky1",		"Sky2",			//20..23
"Sky3",		"Fence1",	"Fence2",	"Fence3",		//24..27
"Bricks",	"Log",		"Leaves",	"FrLeaves",		//28..2B
"SandStn",	"Sand",		"Snow",		"Cobble",		//2C..2F
"CobleGrn",	"Plant",	"SomaRed",	"SomaBlu",		//30..33
"SomaYel",	"SomaGrn",	"SomaCyn",	"SomaVio",		//34..37
"SomaWht",	"Stairs",	"GngRock",	"StnSlab",		//38..3B
"WoodSlab",	"Trigger",	"Planks",	"Planks2",		//3C..3F
"SomaBlk",	"BrkBlk",	"Cloud",	"BrkBlu",		//40..43
"Stn2Blu",	"BrickGrn",	"CoblBlu",	"CoblRed",		//44..47
"Stn3Blu",	"BgBrkBlu",	"HexTile",	"Brk2Red",		//48..4B
"GrsFgBlu",	"GrsFgRed",	"Ladder",	"HayNest",		//4C..4F
"BgBrkBrn",	"BgBrkGry", "Wheat",    "Carrots",		//50..53
"Beets",    "Potatoes", "PumpVine", "MelnVine",		//54..57
NULL
};

char *BTM_BlockMiniDesc(BTM_World *wrl, u32 blk)
{
	char *s;
	
	s=btm_blkmininames[blk&255];
	if(!s)
		s="?";
	return(s);
}

u32 BTM_BlockForName(BTM_World *wrl, char *name)
{
	char *s;
	int i;
	
	if(!name)
		return(0);
	
	for(i=0; i<256; i++)
	{
		s=btm_blkmininames[i];
		if(!s)
			continue;
		if(!stricmp(s, name))
			return(i);
	}
	return(0);
}
