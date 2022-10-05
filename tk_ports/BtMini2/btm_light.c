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


u64 BTM_MobGetOriginPos(BTM_World *wrl, BTM_MobEntity *self);

int BTM_UpdateGetBlockLightEbl(BTM_World *wrl, u32 blk)
{
	int j;

	j=blk&255;
	if((j>0) && (j<4))
//		return((blk>>12)&63);
		return((blk>>12)&255);
		
	if(j==BTM_BLKTY_LAVA)
		return((7<<4)|12);

	if(j==BTM_BLKTY_LANTERN_WHI)
		return((0<<4)|15);

	if(j==BTM_BLKTY_LANTERN_BLUE)
		return((1<<4)|15);
	if(j==BTM_BLKTY_LANTERN_GRN)
		return((2<<4)|15);
	if(j==BTM_BLKTY_LANTERN_CYA)
		return((3<<4)|15);
	if(j==BTM_BLKTY_LANTERN_RED)
		return((4<<4)|15);
	if(j==BTM_BLKTY_LANTERN_VIO)
		return((5<<4)|15);
	if(j==BTM_BLKTY_LANTERN_YEL)
		return((6<<4)|15);

	return(0);
}

int BTM_UpdateBlockLightBlendColors(BTM_World *wrl, int ls, int le)
{
	static const u64 blendtab[16]={
#if 0
		0x09ABCDEEB000000DULL,
		0x9188F99989999999ULL,
		0xA82A79AAAAAAAAAAULL,
		0xB8A399AA3BBBBBB9ULL,
		0xCF794F779CCCCCCFULL,
		0xD999F5CC9DDDDDD5ULL,
		0xE9AA7C66AEEEEEECULL,
		0xE9AA7C679CEECCECULL,
		0xB8A399A989BB99B9ULL,
		0x09ABCDEC99BBDD0DULL,
		0x09ABCDEEBBABE0ECULL,
		0x09ABCDEEBBBB000DULL,
		0x09ABCDEC9DE0CDECULL,
		0x09ABCDEC9D00DD0DULL,
		0x09ABCDEEB0E0E0ECULL,
		0xD9A9F5CC9DCDCDCFULL
#endif

#if 1
		0x09ABCDEC99A0C00CULL,
		0x91B8D90D11B8D995ULL,
		0xAB2AE0A63B2AEAAEULL,
		0xB8A309A03BB309A0ULL,
		0xCDE04F74DDEC4F74ULL,
		0xD909F5CC9D09D5C5ULL,
		0xE0AA7C6600EAEC6CULL,
		0xCD604C670DE07CECULL,
		0x9133D90088BBD90DULL,
		0x91BBDD0D89BBDD0DULL,
		0xAB2BE0EEBBABE0EEULL,
		0x08A3C9A0BBBB0000ULL,
		0xCDE04DE7DDE0CDEFULL,
		0x09A9F5CC9D00DD0DULL,
		0x09AA7C6E00E0E0ECULL,
		0xC5E045CCDDE0FDCFULL
#endif
	};

	int lsc, lec;
	int lc, lv;

//	if((le&15)>((ls&15)+1))
//		return(le-1);
//	if((le&15)<((ls&15)+1))
//		return(ls);

	if((le&15)==0)
		return(ls);
	if((ls&15)==0)
	{
		if((le&15)>1)
			return(le-1);
		return(0);
	}

//	if((le&15)>((ls&15)+2))
	if((le&15)>((ls&15)+1))
		return(le-1);
	if((le&15)<((ls&15)+1))
		return(ls);

	lv=ls&15;
	if((le&15)>((ls&15)+1))
		lv=(le&15)-1;

	lsc=(ls>>4)&15;
	lec=(le>>4)&15;

	lc=(blendtab[lsc]>>((15-lec)*4))&15;
	lc=(lc<<4)|lv;
	return(lc);
}

int BTM_UpdateChunkFixVoidForRCix(BTM_World *wrl, u64 rcix)
{
	u64 rcix0, bpos;
	u32 blk, blk1;
	int i, z;

	bpos=BTM_ConvRcixToBlkPos(rcix);
	z=(bpos>>32)&255;
	
	if(z>80)
		{ blk1=BTM_BLKTY_AIR2|(15<<20); }
//	else if(z>48)
	else if(z>32)
		{ blk1=BTM_BLKTY_AIR2|(0<<20); }
	else
		{ blk1=BTM_BLKTY_STONE; }

	rcix0=rcix&(~0x00000FFF);
	
	for(i=0; i<4096; i++)
	{
		blk=BTM_GetWorldBlockCix(wrl, rcix0+i);
		
		if(!(blk&255))
		{
			BTM_SetWorldBlockCixNl(wrl, rcix0+i, blk1);
		}
	}
	return(0);
}

int BTM_UpdateBlockLightForRCixR(BTM_World *wrl, u64 rcix, int rcnt)
{
	u64 rcixa[6];
	u32 blka[6];
	u64 rcix1, rcix2;
	u32 blk, blk1, blk2;
	int lbl, lsl, ebl, esl, isair;
	int i, j, k;
	
	isair=1;
	blk=BTM_GetWorldBlockCix(wrl, rcix);
	j=blk&255;
	if(!j || (j>=4))
	{
		if(!j)
		{
			rcix1=rcix&(~0x00000FFF);
			rcix2=rcix&(~0x001FFFFF);
			blk1=BTM_GetWorldBlockCix(wrl, rcix1);
			blk2=BTM_GetWorldBlockCix(wrl, rcix2);
			
			if(!(blk2&255))
				return(0);

			if(!(blk1&255))
			{
				BTM_UpdateChunkFixVoidForRCix(wrl, rcix1);
				return(0);
			}

			return(0);
		}
	
		if(!BTM_BlockIsTransparentP(wrl, blk))
			return(0);
	
		isair=0;
//		return(0);
	}
	
	if(rcnt<=0)
		return(-1);

	rcixa[0]=BTM_BlockOffsetRcix(rcix,   1,  0,  0);
	rcixa[1]=BTM_BlockOffsetRcix(rcix,  -1,  0,  0);
	rcixa[2]=BTM_BlockOffsetRcix(rcix,   0,  1,  0);
	rcixa[3]=BTM_BlockOffsetRcix(rcix,   0, -1,  0);
	rcixa[4]=BTM_BlockOffsetRcix(rcix,   0,  0,  1);
	rcixa[5]=BTM_BlockOffsetRcix(rcix,   0,  0, -1);

	lbl=0; lsl=0;

	for(i=0; i<6; i++)
	{
		blk1=BTM_TryGetWorldBlockCix(wrl, rcixa[i]);
		
		if(rcixa[i]==rcix)
//			blk1=BTM_BLKTY_AIR2 | (15<<18);
			blk1=BTM_BLKTY_AIR2 | (15<<20);

		if(!blk1)
		{
			/* If we touch the void, abort. */
			return(0);
		}

//		if(!blk1)
//			break;

		ebl=BTM_UpdateGetBlockLightEbl(wrl, blk1);
		esl=0;

		j=blk1&255;
//		if((j>0) && (j<4))
		if(BTM_BlockIsTransparentP(wrl, blk1))
		{
//			ebl=((blk1>>12)&63);
			ebl=((blk1>>12)&255);
//			esl=((blk1>>18)&63);
			esl=((blk1>>20)&15);
		}

		lbl=BTM_UpdateBlockLightBlendColors(wrl, lbl, ebl);

//		if((ebl&15)>((lbl&15)+1))
//			lbl=ebl-1;

//		if((ebl&15)==((lbl&15)+1))
//		{
//			lbl=ebl-1;
//			lbl=BTM_UpdateBlockLightBlendColors(wrl, lbl, ebl);
//		}

//		if(i>=4)
//		if(i==4)
		if((i==4) && isair && (esl==0x0F))
		{
			if((esl&15)>(lsl&15))
				lsl=esl;
		}else
		{
			if((esl&15)>((lsl&15)+1))
				lsl=esl-1;
		}
	}

	blk2=blk;
	blk2&=~(63<<18);
	blk2&=~(63<<12);
	
//	blk2|=(lsl<<18)|(lbl<<12);
	blk2|=(lsl<<20)|(lbl<<12);
	
//	if((blk2==blk) || ((blk2|(1<<18))==blk))
	if((blk2==blk) || ((blk2|(1<<20))==blk))
		return(0);

	BTM_SetWorldBlockCixNl(wrl, rcix, blk2);

	for(i=0; i<6; i++)
	{
		BTM_UpdateBlockLightForRCixR(wrl, rcixa[i], rcnt-1);
	}

	return(0);
}

int BTM_UpdateBlockLightForRCix(BTM_World *wrl, u64 rcix)
{
//	return(BTM_UpdateBlockLightForRCixR(wrl, rcix, 64));
	return(BTM_UpdateBlockLightForRCixR(wrl, rcix, 256));
}

int BTM_UpdateRegionSetBlockLightCix(BTM_World *wrl,
	BTM_Region *rgn, u64 rcix)
{
	u64 rcix1, rcix2;
	u32 blk, blk1, blk2;
	int cix, cix1, cix2;
	int i, j, k;

	rcix1=BTM_BlockOffsetRcix(rcix,  0, 0, 1);
	rcix2=BTM_BlockOffsetRcix(rcix,  0, 0, -1);

	cix=BTM_Rcix2Cix(rcix);
	cix1=BTM_Rcix2Cix(rcix1);
	cix2=BTM_Rcix2Cix(rcix2);

	blk=BTM_GetRegionBlockCix(wrl, rgn, cix);

	if((blk&255)<4)
	{
		/* If a block were set to air, propagate sunlight downward. */
		blk1=BTM_GetRegionBlockCix(wrl, rgn, cix1);
		if(((blk1&255)==BTM_BLKTY_AIR2) || ((blk1&255)==BTM_BLKTY_AIR3))
		{
			if((blk1&255)==BTM_BLKTY_AIR3)
			{
//				blk1=BTM_BLKTY_AIR2 | (15<<18);
				blk1=BTM_BLKTY_AIR2 | (15<<20);
			}else
			{
				blk1&=~(255<<24);
				blk1&=~( 63<<12);
			}
		
//			if(((blk1>>18)&15)==15)
			if(((blk1>>20)&15)==15)
			{
				BTM_SetRegionBlockCix(wrl, rgn, cix, blk1);
				while(1)
				{
					blk2=BTM_GetRegionBlockCix(wrl, rgn, cix2);
					j=(blk2&255);
					if(!j || (j>=4))
						break;					
					BTM_SetRegionBlockCix(wrl, rgn, cix2, blk1);

					rcix1=BTM_BlockOffsetRcix(rcix2,  0, 0, -1);
					if(rcix1==rcix2)
						break;
					rcix2=rcix1;
					cix2=BTM_Rcix2Cix(rcix2);
				}
			}
		}
	}else
	{
		/* Else, propagate darkness downward. */
		blk1=BTM_BLKTY_AIR2;

		while(1)
		{
			blk2=BTM_GetRegionBlockCix(wrl, rgn, cix2);
			j=(blk2&255);
			if(!j || (j>=4))
				break;					
			BTM_SetRegionBlockCix(wrl, rgn, cix2, blk1);

			rcix1=BTM_BlockOffsetRcix(rcix2,  0, 0, -1);
			if(rcix1==rcix2)
				break;
			rcix2=rcix1;
			cix2=BTM_Rcix2Cix(rcix2);
		}
	}

	rcix1=BTM_BlockOffsetRcix(rcix,  0, 0,  1);
	rcix2=BTM_BlockOffsetRcix(rcix,  0, 0, -1);

	if(rcix1!=rcix)
		BTM_UpdateBlockLightForRCix(wrl, rcix1);
	BTM_UpdateBlockLightForRCix(wrl, rcix);
//	BTM_UpdateBlockLightForRCix(wrl, rcix2);

	if(rcix2==rcix)
		return(0);

	while(1)
	{
		cix2=BTM_Rcix2Cix(rcix2);
		blk2=BTM_GetRegionBlockCix(wrl, rgn, cix2);
		j=(blk2&255);
		if(!j || (j>=4))
			break;					
		BTM_UpdateBlockLightForRCix(wrl, rcix2);
		rcix1=BTM_BlockOffsetRcix(rcix2,  0, 0, -1);
		if(rcix1==rcix2)
			break;
		rcix2=rcix1;
	}

	return(0);
}

int BTM_GetLightForRCix(BTM_World *wrl, u64 rcix)
{
	int bl, sl, j, l;
	u32 blk;

	blk=BTM_GetWorldBlockCix(wrl, rcix);
	j=blk&255;
	if(j<4)
	{
//		bl=(blk>>12)&63;
		bl=(blk>>12)&255;
//		sl=(blk>>18)&63;
		sl=(blk>>20)&15;
		sl=(sl*wrl->daylight)>>4;
		
		l=((sl&15)>(bl&15))?sl:bl;
		
		return(l);
	}

	return(0);
}

int BTM_GetLightForXYZ(BTM_World *wrl, int cx, int cy, int cz)
{
	u64 rcix;

	if(cz<0)
		return(0);
	if(cz>=128)
		return(0);

	rcix=BTM_BlockCoordsToRcix(cx, cy, cz);
	return(BTM_GetLightForRCix(wrl, rcix));
}

u32 BTM_GetColorRgbForBlockLight(int ll)
{
	u32 clr;
	int br, bg, bb, cr, cg, cb;
	int l;

	switch(ll>>4)
	{
//		case 0: br=0xBF; bg=0xBF; bb=0xBF; break;
//		case 1: br=0x3F; bg=0xBF; bb=0xFF; break;
//		case 2: br=0xFF; bg=0x3F; bb=0xBF; break;
//		case 3: br=0xFF; bg=0xBF; bb=0x3F; break;

		case 0x0: br=0xFF; bg=0xFF; bb=0xFF; break;

		case 0x1: br=0x3F; bg=0x3F; bb=0xFF; break;
		case 0x2: br=0x3F; bg=0xFF; bb=0x3F; break;
		case 0x3: br=0x3F; bg=0xFF; bb=0xFF; break;
		case 0x4: br=0xFF; bg=0x3F; bb=0x3F; break;
		case 0x5: br=0xFF; bg=0x3F; bb=0xFF; break;
		case 0x6: br=0xFF; bg=0xFF; bb=0x3F; break;

		case 0x7: br=0xFF; bg=0x7F; bb=0x3F; break;
		case 0x8: br=0x3F; bg=0x7F; bb=0xFF; break;

		case 0x9: br=0x7F; bg=0x7F; bb=0xFF; break;
		case 0xA: br=0x7F; bg=0xFF; bb=0x7F; break;
		case 0xB: br=0x7F; bg=0xFF; bb=0xFF; break;
		case 0xC: br=0xFF; bg=0x7F; bb=0x7F; break;
		case 0xD: br=0xFF; bg=0x7F; bb=0xFF; break;
		case 0xE: br=0xFF; bg=0xFF; bb=0x7F; break;

		case 0xF: br=0xFF; bg=0x3F; bb=0x7F; break;
	}
	
	l=(ll&15)+1;
	cr=(br*l)>>4;
	cg=(bg*l)>>4;
	cb=(bb*l)>>4;
	
//	clr=0xFF000000U|(cr<<16)|(cg<<8)|(cb<<0);
	clr=0xFF000000U|(cb<<16)|(cg<<8)|(cr<<0);
	return(clr);
}

u32 BTM_ModulateColorRgbForBlockLight(u32 rgb, int ll)
{
	u32 clr;
	int br, bg, bb, cr, cg, cb, ma;
	int l;

	if(ll==0x0F)
		return(rgb);

	if(!(ll&15))
		return(0xFF000000U);

	if(ll==0x04)
		return(0xFF000000U|((rgb>>2)&0x003F3F3FU));

	if((ll>>4)==0)
	{
		clr=0xFF000000;

		ma=((s32)(ll<<28))>>31;
		clr+=(rgb>>1)&(ma&0x007F7F7FU);

		ma=((s32)(ll<<29))>>31;
		clr+=(rgb>>2)&(ma&0x003F3F3FU);

		ma=((s32)(ll<<30))>>31;
		clr+=(rgb>>3)&(ma&0x001F1F1FU);

		ma=((s32)(ll<<31))>>31;
		clr+=(rgb>>4)&(ma&0x000F0F0FU);
		
		return(clr);
	}
	
//	cr=(rgb>>16)&255;
//	cg=(rgb>> 8)&255;
//	cb=(rgb>> 0)&255;

	cb=(rgb>>16)&255;
	cg=(rgb>> 8)&255;
	cr=(rgb>> 0)&255;

	switch(ll>>4)
	{
//		case 0: br=0xBF; bg=0xBF; bb=0xBF; break;
//		case 1: br=0x3F; bg=0xBF; bb=0xFF; break;
//		case 2: br=0xFF; bg=0x3F; bb=0xBF; break;
//		case 3: br=0xFF; bg=0xBF; bb=0x3F; break;

		case 0x0: br=0xFF; bg=0xFF; bb=0xFF; break;

		case 0x1: br=0x3F; bg=0x3F; bb=0xFF; break;
		case 0x2: br=0x3F; bg=0xFF; bb=0x3F; break;
		case 0x3: br=0x3F; bg=0xFF; bb=0xFF; break;
		case 0x4: br=0xFF; bg=0x3F; bb=0x3F; break;
		case 0x5: br=0xFF; bg=0x3F; bb=0xFF; break;
		case 0x6: br=0xFF; bg=0xFF; bb=0x3F; break;

		case 0x7: br=0xFF; bg=0x7F; bb=0x3F; break;
		case 0x8: br=0x3F; bg=0x7F; bb=0xFF; break;

		case 0x9: br=0x7F; bg=0x7F; bb=0xFF; break;
		case 0xA: br=0x7F; bg=0xFF; bb=0x7F; break;
		case 0xB: br=0x7F; bg=0xFF; bb=0xFF; break;
		case 0xC: br=0xFF; bg=0x7F; bb=0x7F; break;
		case 0xD: br=0xFF; bg=0x7F; bb=0xFF; break;
		case 0xE: br=0xFF; bg=0xFF; bb=0x7F; break;

		case 0xF: br=0xFF; bg=0x3F; bb=0x7F; break;
	}
	
	l=(ll&15)+1;
	cr=(cr*br*l)>>12;
	cg=(cg*bg*l)>>12;
	cb=(cb*bb*l)>>12;
	
//	clr=0xFF000000U|(cr<<16)|(cg<<8)|(cb<<0);
	clr=0xFF000000U|(cb<<16)|(cg<<8)|(cr<<0);
	return(clr);
}


int BTM_UpdateBlockGrassForRCix(BTM_World *wrl, u64 rcix)
{
	u64 rcixa[12];
	u32 blka[6];
	u32 blk, blk1, blk2, blk3;
	u64 rcix1, rcix2;
	int i, j, k;
	
	blk=BTM_GetWorldBlockCix(wrl, rcix);
	j=blk&255;
//	if(!j || (j>=4))
//		return(0);
	
	rcixa[ 0]=BTM_BlockOffsetRcix(rcix,   1,  0,  0);
	rcixa[ 1]=BTM_BlockOffsetRcix(rcix,  -1,  0,  0);
	rcixa[ 2]=BTM_BlockOffsetRcix(rcix,   0,  1,  0);
	rcixa[ 3]=BTM_BlockOffsetRcix(rcix,   0, -1,  0);
	rcixa[ 4]=BTM_BlockOffsetRcix(rcix,   1,  0,  1);
	rcixa[ 5]=BTM_BlockOffsetRcix(rcix,  -1,  0,  1);
	rcixa[ 6]=BTM_BlockOffsetRcix(rcix,   0,  1,  1);
	rcixa[ 7]=BTM_BlockOffsetRcix(rcix,   0, -1,  1);
	rcixa[ 8]=BTM_BlockOffsetRcix(rcix,   1,  0, -1);
	rcixa[ 9]=BTM_BlockOffsetRcix(rcix,  -1,  0, -1);
	rcixa[10]=BTM_BlockOffsetRcix(rcix,   0,  1, -1);
	rcixa[11]=BTM_BlockOffsetRcix(rcix,   0, -1, -1);

//	rcixa[4]=BTM_BlockOffsetRcix(rcix,   0,  0,  1);
//	rcixa[5]=BTM_BlockOffsetRcix(rcix,   0,  0, -1);

	for(i=0; i<12; i++)
	{
		rcix1=rcixa[i];
		rcix2=BTM_BlockOffsetRcix(rcix1, 0, 0, 1);

		blk1=BTM_TryGetWorldBlockCix(wrl, rcix1);
		blk2=BTM_TryGetWorldBlockCix(wrl, rcix2);
		
		if(!blk1 || !blk2)
			continue;

		if(((blk1&255)==BTM_BLKTY_DIRT) && ((blk2&255)<4))
		{
			blk3=(blk1&(~255))|(blk&255);
			BTM_SetWorldBlockCixNl(wrl, rcix1, blk3);
		}
	}

	return(0);
}

int BTM_UpdateBlockPlantForRCix(BTM_World *wrl, u64 rcix)
{
	u32 blka[6];
	u32 blk, blk1, blk2, blk3;
	u64 rcix1, rcix2;
	int i, j, k;
	
	blk=BTM_GetWorldBlockCix(wrl, rcix);
	j=blk&255;
	k=(blk>>8)&15;
	
	if((k || (j!=BTM_BLKTY_GRASSCLUMP)) && (k<15))
	{
		blk1=blk+0x100;
		BTM_SetWorldBlockCixNl(wrl, rcix, blk1);
	}

	return(0);
}

int BTM_BlockTickBlockForRCix(BTM_World *wrl, u64 rcix)
{
	u32 blk, blk1, blk2;
	int lbl, lsl, ebl, esl;
	int i, j, k;
	
	blk=BTM_GetWorldBlockCix(wrl, rcix);
	j=blk&255;
	if(j && (j<4))
	{
		BTM_UpdateBlockLightForRCix(wrl, rcix);
		return(0);
	}
	
	if(BTM_BlockIsTransparentP(wrl, blk))
		BTM_UpdateBlockLightForRCix(wrl, rcix);
	
	if(	(j==BTM_BLKTY_GRASS) ||
		(j==BTM_BLKTY_MYCELIUM) ||
		(j==BTM_BLKTY_REDGRASS))
	{
		BTM_UpdateBlockGrassForRCix(wrl, rcix);
	}

	if(	(j==BTM_BLKTY_GRASSCLUMP) ||
		(j==BTM_BLKTY_WHEAT) ||
		(j==BTM_BLKTY_CARROTS) ||
		(j==BTM_BLKTY_BEETS) ||
		(j==BTM_BLKTY_POTATOES))
	{
		BTM_UpdateBlockPlantForRCix(wrl, rcix);
	}
	
//	if((j>=4) && !((blk>>24)&63))
	if(j>=4)
	{
//		BTM_UpdateWorldBlockOccCix2(wrl, rcix);
		BTM_UpdateWorldBlockOccCix3R(wrl, rcix, 32);
	}

	return(0);
}

int BTM_BlockTickRegion(BTM_World *wrl, BTM_Region *rgn)
{
	BTM_Region *rgn1;
	BTM_MobEntity *ent, *ent1, *elst, *enxt;
	int npmobs;

	u64 rcix, cpos, bpos;
	u32 blk;
	int cix, cnt;
	int ecx, ecy, erx, ery, erix;
	int i, j, k, l, h;
//	int cx, cy, cz;

	npmobs=BTM_CountRegionEntityClass(wrl, rgn, 1);

//	cnt=8;
	cnt=255;
#ifdef BTM_TARGET_SMALL
	cnt=15;
#endif

	while(cnt)
	{
//		cix=(rand()*rand()*rand())&((1<<21)-1);

		cix=rand();
		cix=(cix*127)+rand();
		cix=(cix*127)+rand();
		cix=(cix*127)+rand();
		cix=cix&((1<<21)-1);

		rcix=(((u64)(rgn->rgnix))<<21)|cix;
		BTM_BlockTickBlockForRCix(wrl, rcix);
		
		if(npmobs<24)
		{
			blk=BTM_GetWorldBlockCix(wrl, rcix);
			j=blk&255;
			
			if(j==BTM_BLKTY_GRASS)
			{
				cpos=BTM_ConvRcixToCorg(rcix);
				cpos+=2LL<<56;
				BTMGL_SpawnWorldMobClass(wrl, cpos, 1);
			}
		}
		
		cnt--;
	}
	
	ent=rgn->live_entity;
	while(ent)
	{
		ent1=rgn->live_entity;
		while(ent1)
		{
			BTM_MobCheckHandleCollide(wrl, ent, ent1);
			ent1=ent1->next;
		}

//		if(ent->Tick)
//			ent->Tick(wrl, ent);
		ent=ent->next;
	}
	
	ent=rgn->live_entity;
	while(ent)
	{
		if(ent->Tick)
			ent->Tick(wrl, ent);
		ent=ent->next;
	}
	
	elst=NULL;
	ent=rgn->live_entity;
	while(ent)
	{
		enxt=ent->next;
	
		ecx=ent->org_x>>8;
		ecy=ent->org_y>>8;
		erx=(ecx>>7)&511;
		ery=(ecy>>7)&511;
		erix=(ery<<9)|erx;

		if(erix!=rgn->rgnix)
		{
			rgn1=BTM_GetRegionForRix(wrl, erix);
			ent->next=rgn1->live_entity;
			rgn1->live_entity=ent;
			
			rgn->dirty|=32;
			rgn1->dirty|=32;
		}else
		{
			ent->next=elst;
			elst=ent;
		}

		ent=enxt;
	}
	rgn->live_entity=elst;
	
	for(i=0; i<256; i++)
		rgn->live_entity_hash[i]=NULL;
	
	ent=rgn->live_entity;
	while(ent)
	{
		cpos=BTM_MobGetOriginPos(wrl, ent);
//		bpos=BTM_MobGetOriginBlockPos(ent);
		bpos=BTM_ConvCorgToBlkPos(cpos);
		ent->spos=cpos;
		ent->bpos=bpos;
		
//		h=((bpos*(65521ULL*65521ULL*65521ULL))>>48)&255;
		h=BTM_HashForBlkPos(bpos);
		ent->nxt_bpos=rgn->live_entity_hash[h];
		rgn->live_entity_hash[h]=ent;
		ent->chn_bpos=NULL;
		
		ent=ent->next;
	}
	

	return(0);
}

int BTM_BlockTickWorldI(BTM_World *wrl)
{
	BTM_Region *rgn, *rnxt;
	u64 rpos;
	int cx, cy, vx, vy, dx, dy, d;

	vx=(wrl->cam_org>> 8)&0xFFFF;
	vy=(wrl->cam_org>>32)&0xFFFF;
	
	rgn=wrl->region;
	while(rgn)
	{
		if((rgn->dirty&4) && !(rgn->dirty&2))
		{
			rgn=rgn->next;
			continue;
		}
	
//		rpos=BTM_ConvRixToBlkPos(rgn->rgnix);
		rpos=BTM_ConvRixToBlkPosCenter(rgn->rgnix);
		cx=(rpos>> 0)&65535;
		cy=(rpos>>16)&65535;

		dx=(s16)(vx-cx);
		dy=(s16)(vy-cy);
		dx=dx^(dx>>31);
		dy=dy^(dy>>31);
//		d=dx+dy;
		if(dx>dy)
			d=dx+(dy>>1);
		else
			d=dy+(dx>>1);
		
//		if(d>=256)
//		if(d>=192)
		if(d>=128)
		{
			rgn=rgn->next;
			continue;
		}

		rnxt=rgn->next;
		BTM_BlockTickRegion(wrl, rgn);
		rgn=rnxt;
	}

	return(0);
}

int BTM_BlockTickWorld(BTM_World *wrl, int dt)
{
	static int accdt;
	
	accdt+=dt;
	while(accdt>=100)
	{
		BTM_BlockTickWorldI(wrl);
		accdt-=100;
	}

	return(0);
}

float BTM_V3F_Normalize(float *fvi, float *fvo)
{
	float f;
	f=sqrt(fvi[0]*fvi[0]+fvi[1]*fvi[1]+fvi[2]*fvi[2]);
	fvo[0]=fvi[0]/f;
	fvo[1]=fvi[1]/f;
	fvo[2]=fvi[2]/f;
	return(f);
}
