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
Voxel:
  (31:24): Attrib / Occlusion
  (23:28): Light, Sky
  (17:12): Light Level (Block)
  (11: 8): Attrib / Ext-Type
  ( 7: 0): Block Type

Pos32:
  (31:24): ZPos, 8
  (23:12): YPos, 12
  (11: 0): XPos, 12

Pos64:
  (63:48): ZPos, 8.8
  (47:24): YPos, 16.8
  (23: 0): XPos, 16.8
 */

#include "btm_engine.h"

u64	btm_step_ydir[1024];
u64	btm_step_pdir[1024];
int	btm_step_init=0;

u32	btmra_rcptab[512];
// s32 btmra_cosang[256];
s32 btmra_sincosang[256+64];
s32 *btmra_sinang;
s32 *btmra_cosang;

float btmra_sincosang_f[256+64];
float *btmra_sinang_f;
float *btmra_cosang_f;

int BTMRA_RastInitTables()
{
	double f, g;
	int i, j, k;
	
	for(i=1; i<512; i++)
	{
		btmra_rcptab[i]=(0x7FFFFFFFU/i)*2;
	}

	for(i=0; i<(256+64); i++)
	{
		f=i*((2*M_PI)/256);
//		g=cos(f);
		g=sin(f);
		j=g*0x7FFFFFFF;
//		btmra_cosang[i]=j;
		btmra_sincosang[i]=j;
		btmra_sincosang_f[i]=g;
	}
	
	btmra_sinang=btmra_sincosang+ 0;
	btmra_cosang=btmra_sincosang+64;

	btmra_sinang_f=btmra_sincosang_f+ 0;
	btmra_cosang_f=btmra_sincosang_f+64;

	return(0);
}


u16 btm_pmort2_enc8[256];	// 8->16 bit encode
byte btm_pmort2_dec8[256];	// 8->4 bit decode

u32 btm_pmort3_enc8[256];	// 8->24 bit encode
byte btm_pmort3_dec12[4096];	// 12->4 bit decode

int BTM_RaycastInitTables()
{
	u64 v;
	double f, g, fx, fy;
	int ix, iy;
	int i, j, k;
	
	if(btm_step_init)
		return(0);
	btm_step_init=1;
	
	for(i=0; i<1024; i++)
	{
		f=i*((2*M_PI)/1024);
		fx=sin(f);
		fy=-cos(f);
		
		ix=fx*256;
		iy=fy*256;
		ix&=0xFFFFFF;
		iy&=0xFFFFFF;
		
		v=(((u64)iy)<<24)|(ix);
		btm_step_ydir[i]=v;
	}
	
	for(i=0; i<1024; i++)
	{
		f=i*((2*M_PI)/1024);
		fx=sin(f);
		fy=cos(f);
		ix=fx*256;

		iy=fy*256;
//		iy=fabs(fy)*256;
//		iy=(1.0-fabs(fx))*256;
		iy&=0x00FFFFFF;

		ix&=0x0000FFFF;
		v=(((u64)ix)<<48)|(((u64)iy)<<24)|(iy);
		btm_step_pdir[i]=v;
	}
	
	BTMRA_RastInitTables();
	
	for(i=0; i<256; i++)
	{
		j=0;
		if(i&  1)j|=0x0001;
		if(i&  2)j|=0x0004;
		if(i&  4)j|=0x0010;
		if(i&  8)j|=0x0040;
		if(i& 16)j|=0x0100;
		if(i& 32)j|=0x0400;
		if(i& 64)j|=0x1000;
		if(i&128)j|=0x4000;
		btm_pmort2_enc8[i]=j;
		
		k=0;
		if(i&0x01)k|=1;
		if(i&0x04)k|=2;
		if(i&0x10)k|=4;
		if(i&0x40)k|=8;
		btm_pmort2_dec8[i]=k;


		j=0;
		if(i&  1)j|=0x000001;
		if(i&  2)j|=0x000008;
		if(i&  4)j|=0x000040;
		if(i&  8)j|=0x000200;
		if(i& 16)j|=0x001000;
		if(i& 32)j|=0x008000;
		if(i& 64)j|=0x040000;
		if(i&128)j|=0x200000;
		btm_pmort3_enc8[i]=j;
	}

	for(i=0; i<4096; i++)
	{
		k=0;
		if(i&0x001)k|=1;
		if(i&0x008)k|=2;
		if(i&0x040)k|=4;
		if(i&0x200)k|=8;
		btm_pmort3_dec12[i]=k;
	}

	return(1);
}

int btm_stat_malloc_tot;
void * volatile btm_malloc_mutex;

int BTM_LockMalloc()
{
	if(!btm_malloc_mutex)
		btm_malloc_mutex=thMutex();
	thLockMutex(btm_malloc_mutex);
	return(0);
}

int BTM_UnlockMalloc()
{
	thUnlockMutex(btm_malloc_mutex);
	return(0);
}

char *btm_malloc_lfnix_name[256];
int btm_malloc_n_lfnix;

void *btm_malloc_chn;

int btm_malloc_lfnixforlfn(char *lfn)
{
	int i;
	
	for(i=0; i<btm_malloc_n_lfnix; i++)
		if(!strcmp(btm_malloc_lfnix_name[i], lfn))
			return(i);

	if(i>=btm_malloc_n_lfnix)
		return(-1);

	i=btm_malloc_n_lfnix++;
	btm_malloc_lfnix_name[i]=strdup(lfn);
	return(i);
}


void *btm_malloc_lln(int sz, char *lfn, int lln)
{
	void *ptr;

	BTM_LockMalloc();
	printf("malloc: %d %s:%d tot=%d\n", sz, lfn, lln, btm_stat_malloc_tot);
	btm_stat_malloc_tot+=sz;

//	ptr=malloc(sz+4*sizeof(void *));
	ptr=malloc(sz);
	
	BTM_UnlockMalloc();

	return(ptr);
}

void *btm_realloc_lln(void *ptr, int sz, char *lfn, int lln)
{
	void *ptr1;

	BTM_LockMalloc();
	printf("realloc: %d %s:%d\n", sz, lfn, lln);
	ptr1=realloc(ptr, sz);
	BTM_UnlockMalloc();

	return(ptr1);
}

void btm_free(void *ptr)
{
	free(ptr);
}

u64 BTM_RaycastStepVectorB(int yang, int pang)
{
	float sy, cy, sp, cp, la, lb;
	float fx, fy, fz, fd;
	int vx, vy, vz, sx;
	u64 ystep, pstep, step;

#if 1
	ystep=btm_step_ydir[yang&1023];
	pstep=btm_step_pdir[pang&1023];
	
	vx=(ystep>> 0);
	vy=(ystep>>24);
	vx=((int)(vx<<8))>>8;
	vy=((int)(vy<<8))>>8;

	sx=(pstep>> 0);
	sx=((int)(sx<<8))>>8;
	vz=(pstep>>48);
	
	vx=(vx*sx)>>8;
	vy=(vy*sx)>>8;
	
	vx&=0x00FFFFFF;
	vy&=0x00FFFFFF;

	step=(((u64)vz)<<48)|(((u64)vy)<<24)|(vx);
	return(step);
#endif
}

// #define BTM_RCIX_MORTON

u64 BTM_RaycastStepVector(int yang, int pang)
{
	return(BTM_RaycastStepVectorB(yang*4, pang*4));
}

u64 BTM_BlockCoordsToRcix(int cx, int cy, int cz)
{
	u64 cix, bix, hix, rix, rcix;
	int rx, ry, hx, hy, hz, bx, by, bz;

	if(cz<  0)	cz=  0;
	if(cz>127)	cz=127;

#ifdef BTM_RCIX_MORTON
	rx=(cx>>7)&511;
	ry=(cy>>7)&511;
//	rix=(ry<<9)|rx;

	rix =(btm_pmort2_enc8[rx&255]   );
	rix|=(btm_pmort2_enc8[ry&255]<<1);
	if(rx&256)rix|=0x10000;
	if(ry&256)rix|=0x20000;

	cix =(btm_pmort3_enc8[cx&127]   );
	cix|=(btm_pmort3_enc8[cy&127]<<1);
	cix|=(btm_pmort3_enc8[cz&127]<<2);

	rcix=(rix<<21)|cix;
	return(rcix);
#else

//	rx=(cx>>7)&255;
//	ry=(cy>>7)&255;
//	rix=(ry<<8)|rx;

	rx=(cx>>7)&511;
	ry=(cy>>7)&511;
	rix=(ry<<9)|rx;

//	cx&=127;
//	cy&=127;
//	cz&=127;

	hx=(cx>>4)&7;
	hy=(cy>>4)&7;
	hz=(cz>>4)&7;
	bx=cx&15;
	by=cy&15;
	bz=cz&15;

	hix=(hz<<6)|(hy<<3)|hx;
	bix=(bz<<8)|(by<<4)|bx;
	cix=(hix<<12)|bix;

//	cix=(cz<<14)|(cy<<7)|cx;
//	rcix=(rix<<24)|cix;
	rcix=(rix<<21)|cix;
	return(rcix);
#endif
}

u64 BTM_WorldCorgToRcix(u64 cpos)
{
#ifdef BTM_RCIX_MORTON
	int cx, cy, cz;

	cx=(cpos>> 8)&65535;
	cy=(cpos>>32)&65535;
	cz=(cpos>>56)&255;
	return(BTM_BlockCoordsToRcix(cx, cy, cz));

#else

	u64 cix, bix, hix, rix, rcix;
	int rx, ry, hx, hy, hz, bx, by, bz;
	int cx, cy, cz;

	cx=(cpos>> 8)&65535;
	cy=(cpos>>32)&65535;
	cz=(cpos>>56)&255;

	if(cz<  0)	cz=  0;
	if(cz>127)	cz=127;

	rx=(cx>>7)&511;
	ry=(cy>>7)&511;
	rix=(ry<<9)|rx;

	hx=(cx>>4)&7;
	hy=(cy>>4)&7;
	hz=(cz>>4)&7;
	bx=cx&15;
	by=cy&15;
	bz=cz&15;

	hix=(hz<<6)|(hy<<3)|hx;
	bix=(bz<<8)|(by<<4)|bx;
	cix=(hix<<12)|bix;

	rcix=(rix<<21)|cix;
	return(rcix);


//	return(BTM_BlockCoordsToRcix(cx, cy, cz));
#endif
}

int BTM_WorldCorgToRix(u64 spos)
{
	int cx, cy, cz, rx, ry, rix;

	cx=spos>> 8;
	cy=spos>>32;
//	rx=(cx>>7)&255;
//	ry=(cy>>7)&255;
//	rix=(ry<<8)|rx;

#ifdef BTM_RCIX_MORTON
	rx=(cx>>7)&511;
	ry=(cy>>7)&511;
	rix =(btm_pmort2_enc8[rx&255]   );
	rix|=(btm_pmort2_enc8[ry&255]<<1);
	if(rx&256)rix|=0x10000;
	if(ry&256)rix|=0x20000;
#else
	rx=(cx>>7)&511;
	ry=(cy>>7)&511;
	rix=(ry<<9)|rx;
#endif
	
	return(rix);
}

u64 BTM_ConvRcixToBlkPos(u64 rcix)
{
	u64 bpos;
	int bx, by, bz;
	int hx, hy, hz;
	int rx, ry, rix;
	int cx, cy, cz;
	
#ifdef BTM_RCIX_MORTON
	rix=BTM_Rcix2Rix(rcix);
	rx=	(btm_pmort2_dec8[(rix    )&255]   )|
		(btm_pmort2_dec8[(rix>> 8)&255]<<4)|
		(((rix>>16)&1)<<8);
	ry=	(btm_pmort2_dec8[(rix>> 1)&255]   )|
		(btm_pmort2_dec8[(rix>> 9)&255]<<4)|
		(((rix>>17)&1)<<8);
		
	bx=btm_pmort3_dec12[(rcix>>0)&1023];
	by=btm_pmort3_dec12[(rcix>>1)&1023];
	bz=btm_pmort3_dec12[(rcix>>2)&1023];

	hx=btm_pmort3_dec12[(rcix>>12)&255];
	hy=btm_pmort3_dec12[(rcix>>13)&255];
	hz=btm_pmort3_dec12[(rcix>>14)&255];

	cx=(rx<<7)|(hx<<4)|bx;
	cy=(ry<<7)|(hy<<4)|by;
	cz=(hz<<4)|bz;
#else
	rix=BTM_Rcix2Rix(rcix);
	bx=(rcix>> 0)&15;	by=(rcix>> 4)&15;	bz=(rcix>> 8)&15;
	hx=(rcix>>12)&7;	hy=(rcix>>15)&7;	hz=(rcix>>18)&7;
//	rx=(rix>>0)&255;	ry=(rix>>8)&255;
	rx=(rix>>0)&511;	ry=(rix>>9)&511;
	
	cx=(rx<<7)|(hx<<4)|bx;
	cy=(ry<<7)|(hy<<4)|by;
	cz=(hz<<4)|bz;
#endif

	bpos=(((u64)cz)<<32)|(((u64)cy)<<16)|(((u64)cx)<<0);
	return(bpos);
}

u64 BTM_ConvRixToBlkPos(int rix)
{
	return(BTM_ConvRcixToBlkPos(((u64)rix)<<21));
}

u64 BTM_ConvRcixToCorg(u64 rcix)
{
	u64 bpos, cpos;
	int cx, cy, cz;
	
	bpos=BTM_ConvRcixToBlkPos(rcix);
	cx=(bpos>> 0)&0xFFFF;
	cy=(bpos>>16)&0xFFFF;
	cz=(bpos>>32)&0x00FF;
	
	cpos=(((u64)cx)<<8)|(((u64)cy)<<32)|(((u64)cz)<<56);
	return(cpos);
}

u64 BTM_ConvRixToBlkPosCenter(int rix)
{
	u64 cix;
	cix=BTM_BlockCoordsToRcix(64, 64, 64);;
	return(BTM_ConvRcixToBlkPos((((u64)rix)<<21)|cix));
}

u64 BTM_BlockOffsetRcix(u64 rcix, int dx, int dy, int dz)
{
	u64 bpos, rcix1, rcix2, rcix3;
	int cx, cy, cz;

#ifndef BTM_RCIX_MORTON
	rcix1=rcix+dx;
	rcix2=rcix1+(dy<<4);
	rcix3=rcix2+(dz<<8);
	if(	((rcix1&(~  15))==(rcix&(~  15))) &&
		((rcix2&(~ 255))==(rcix&(~ 255))) &&
		((rcix3&(~4095))==(rcix&(~4095))) )
	{
		return(rcix3);
	}
#endif


	bpos=BTM_ConvRcixToBlkPos(rcix);
	cx=(bpos>>0)&65535;
	cy=(bpos>>16)&65535;
	cz=(bpos>>32)&255;
	
	rcix1=BTM_BlockCoordsToRcix(cx+dx, cy+dy, cz+dz);
	return(rcix1);
}

u64 BTM_BlockOffsetRcixPx1(u64 rcix)
{
#ifndef BTM_RCIX_MORTON
	u64 rcix1;
	rcix1=rcix+1;
	if((rcix1&(~  15))==(rcix&(~  15)))
		return(rcix1);
#endif
	return(BTM_BlockOffsetRcix(rcix, 1, 0, 0));
}

u64 BTM_BlockOffsetRcixNx1(u64 rcix)
{
#ifndef BTM_RCIX_MORTON
	u64 rcix1;
	rcix1=rcix-1;
	if((rcix1&(~  15))==(rcix&(~  15)))
		return(rcix1);
#endif
	return(BTM_BlockOffsetRcix(rcix, -1, 0, 0));
}

u64 BTM_BlockOffsetRcixPy1(u64 rcix)
{
#ifndef BTM_RCIX_MORTON
	u64 rcix1;
	rcix1=rcix+(1<<4);
	if((rcix1&(~ 255))==(rcix&(~ 255)))
		return(rcix1);
#endif
	return(BTM_BlockOffsetRcix(rcix, 0, 1, 0));
}

u64 BTM_BlockOffsetRcixNy1(u64 rcix)
{
#ifndef BTM_RCIX_MORTON
	u64 rcix1;
	rcix1=rcix-(1<<4);
	if((rcix1&(~ 255))==(rcix&(~ 255)))
		return(rcix1);
#endif
	return(BTM_BlockOffsetRcix(rcix, 0, -1, 0));
}

u64 BTM_BlockOffsetRcixPz1(u64 rcix)
{
#ifndef BTM_RCIX_MORTON
	u64 rcix1;
	rcix1=rcix+(1<<8);
	if((rcix1&(~4095))==(rcix&(~4095)))
		return(rcix1);
#endif
	return(BTM_BlockOffsetRcix(rcix, 0, 0, 1));
}

u64 BTM_BlockOffsetRcixNz1(u64 rcix)
{
#ifndef BTM_RCIX_MORTON
	u64 rcix1;
	rcix1=rcix-(1<<8);
	if((rcix1&(~4095))==(rcix&(~4095)))
		return(rcix1);
#endif
	return(BTM_BlockOffsetRcix(rcix, 0, 0, -1));
}

u64 BTM_ConvCorgToBlkPos(u64 cpos)
{
	u64 bpos;
	int cx, cy, cz;

	cx=(cpos>> 8)&65535;
	cy=(cpos>>32)&65535;
	cz=(cpos>>56)&255;

	bpos=(((u64)cz)<<32)|(((u64)cy)<<16)|(((u64)cx)<<0);
	return(bpos);
}

int BTM_HashForBlkPos(u64 bpos)
{
	int h;
	h=((bpos*(65521ULL*65521ULL*65521ULL))>>48)&255;
	return(h);
}

volatile int btm_stat_szallocsq[24];

void *BTM_WorldAllocSq(BTM_World *wrl, int pidx)
{
	void *ptr;
	byte *pcs;
	int i, j, k, n;

	if(pidx<4)
		pidx=4;

	BTM_LockAlloc();
	ptr=wrl->mm_p2alloc[pidx];
	if(ptr)
	{
		wrl->mm_p2alloc[pidx]=*(void **)ptr;
		BTM_UnlockAlloc();
		return(ptr);
	}
	
	if(pidx>=16)
	{
		ptr=btm_malloc(1<<pidx);
		btm_stat_szallocsq[pidx]+=1<<pidx;
		BTM_UnlockAlloc();
		return(ptr);
	}

	ptr=btm_malloc(65536);
	btm_stat_szallocsq[pidx]+=65536;

	n=1<<(16-pidx);
	k=1<<pidx;
	pcs=ptr;
	for(i=0; i<n; i++)
	{
		*(void **)pcs=wrl->mm_p2alloc[pidx];
		wrl->mm_p2alloc[pidx]=pcs;
		pcs+=k;
	}
	
	ptr=wrl->mm_p2alloc[pidx];
	wrl->mm_p2alloc[pidx]=*(void **)ptr;
	BTM_UnlockAlloc();
	return(ptr);
}

int BTM_WorldFreeSq(BTM_World *wrl, void *ptr, int pidx)
{
	BTM_LockAlloc();
	*(void **)ptr=wrl->mm_p2alloc[pidx];
	wrl->mm_p2alloc[pidx]=ptr;
	BTM_UnlockAlloc();
	return(0);
}


int BTM_UpdateWorldBlockOccBlkCheck(
	BTM_World *wrl, u32 blka, u32 blkb)
{
	if(blkb&BTM_BLKDFL_NODRAW)
		return(0);

	if(blkb&BTM_BLKDFL_SEETHRU)
	{
		if(blka&BTM_BLKDFL_SEETHRU)
			return(1);
		return(0);
	}

	if(blkb&BTM_BLKDFL_NONSOLID)
	{
		if(blka&BTM_BLKDFL_NONSOLID)
			return(1);
		if(blka&BTM_BLKDFL_FLUID)
			return(1);
		return(0);
	}

	if(blkb&BTM_BLKDFL_FLUID)
	{
		if(blka&BTM_BLKDFL_FLUID)
			return(1);
		return(0);
	}
	
	if(blkb&BTM_BLKDFL_TY_MASK)
	{
		/* Partial Block */
		return(0);
	}

	return(1);
}

int BTM_UpdateWorldBlockOccCix(BTM_World *wrl, u64 rcix)
{
	BTM_Region *rgn;
//	int xmsk, ymsk, zmsk;
//	int cix1, xsh2,
	int bt, dx, dy, dz;
//	int xsh, zsh, cix
	int cix, cix1;
	int rix, rix1;
	u64 bm, rcix1;
	u32 blk, blk0, blkd, blk2, blk2d;
	int i, j, k;

	rix=BTM_Rcix2Rix(rcix);
//	cix=BTM_Rcix2Cix(rcix);

	rgn=BTM_GetRegionForRix(wrl, rix);

//	xmsk=(1<<xsh)-1;
//	ymsk=xmsk<<xsh;

//	xsh=7;
//	zsh=7;

//	xmsk=127;
//	ymsk=xmsk<< 7;
//	zmsk=xmsk<<14;
//	msk=(1<<21)-1;

//	xsh2=xsh<<1;
//	xsh2=14;

	cix=BTM_Rcix2Cix(rcix);
//	blk=rgn->vox[cix];
	blk=BTM_GetRegionBlockCix(wrl, rgn, cix);

//	blk=BTM_GetWorldBlockCix(wrl, rcix);

	bt=(blk&0xFF);
	if(bt<4)
	{
//		wrl->vox[cix]=blk;
		return(0);
	}

	blkd=btmgl_vox_atlas_side[bt];

	if(blkd&BTM_BLKDFL_NODRAW)
	{
		blk|=(0x3FU<<24);
//		rgn->vox[cix]=blk;
		BTM_SetRegionBlockCix(wrl, rgn, cix, blk);
		return(0);
	}

	if(blkd&BTM_BLKDFL_TY_MASK)
	{
		return(0);
	}

	
	blk0=blk;
	blk&=~(0x3FU<<24);

	for(i=0; i<6; i++)
	{
		dx=0; dy=0; dz=0;
		switch(i)
		{
#if 0
			case 0: dy=-1; break;
			case 1: dx= 1; break;
			case 2: dy= 1; break;
			case 3: dx=-1; break;
			case 4: dz= 1; break;
			case 5: dz=-1; break;
#endif

#if 1
			case 0: dx= 1; break;
			case 1: dx=-1; break;
			case 2: dy= 1; break;
			case 3: dy=-1; break;
			case 4: dz= 1; break;
			case 5: dz=-1; break;
#endif
		}
	
		rcix1=BTM_BlockOffsetRcix(rcix, dx, dy, dz);
		rix1=BTM_Rcix2Rix(rcix1);
		if(rix1==rix)
		{
			cix1=BTM_Rcix2Cix(rcix1);
//			bm=rgn->voxbm[cix1>>6];
			bm=(rgn->voxbm+(rgn->voxbmix[cix1>>12]<<6))[(cix1>>6)&63];

			if(bm&(1ULL<<(cix1&63)))
			{
//				blk2=rgn->vox[cix1];
				blk2=BTM_GetRegionBlockCix(wrl, rgn, cix1);
				blk2d=btmgl_vox_atlas_side[blk2&255];
				if(BTM_UpdateWorldBlockOccBlkCheck(wrl, blkd, blk2d))
					blk|=1U<<(24+i);
			}
		}
	}

	if(blk!=blk0)
	{
	//	rgn->vox[cix]=blk;
		BTM_SetRegionBlockCix(wrl, rgn, cix, blk);
		return(1);
	}
	
	return(0);
//	return(blk!=blk0);
}

int BTM_SetWorldBlockCix(BTM_World *wrl, u64 rcix, u32 blk)
{
	BTM_Region *rgn;
	int cix, rix;

	rix=BTM_Rcix2Rix(rcix);
	cix=BTM_Rcix2Cix(rcix);

	rgn=BTM_GetRegionForRix(wrl, rix);

//	rgn->vox[cix]=blk;
	BTM_SetRegionBlockCix(wrl, rgn, cix, blk);
	BTM_UpdateWorldBlockOccCix2(wrl, rcix);
	BTM_UpdateRegionSetBlockLightCix(wrl, rgn, rcix);
	return(0);
}

int BTM_SetWorldBlockCixNl(BTM_World *wrl, u64 rcix, u32 blk)
{
	BTM_Region *rgn;
	int cix, rix;

	rix=BTM_Rcix2Rix(rcix);
	cix=BTM_Rcix2Cix(rcix);

	rgn=BTM_GetRegionForRix(wrl, rix);

//	rgn->vox[cix]=blk;
	BTM_SetRegionBlockCix(wrl, rgn, cix, blk);
//	BTM_UpdateRegionSetBlockLightCix(wrl, rgn, rcix);
	return(0);
}

int BTM_SetWorldBlockXYZ(BTM_World *wrl,
	int cx, int cy, int cz, u32 blk)
{
	u64 rcix;
	int cix, rix, rx, ry;

	if(cz<0)
		return(0);
	if(cz>=128)
		return(0);

//	msk=(1<<21)-1;

//	xsh=wrl->xsh;
//	xsh2=xsh<<1;
//	cix=(((cz<<7)|cy)<<7)|cx;
	
//	rcix=(((u64)rix)<<24)|cix;
	rcix=BTM_BlockCoordsToRcix(cx, cy, cz);
	
	BTM_SetWorldBlockCix(wrl, rcix, blk);
	return(0);
}

int BTM_SetWorldBlockNlXYZ(BTM_World *wrl,
	int cx, int cy, int cz, u32 blk)
{
	u64 rcix;
	int cix, rix, rx, ry;

	if(cz<0)
		return(0);
	if(cz>=128)
		return(0);

	rcix=BTM_BlockCoordsToRcix(cx, cy, cz);	
	BTM_SetWorldBlockCixNl(wrl, rcix, blk);
	return(0);
}

int BTM_UpdateWorldBlockOccCix2(BTM_World *wrl, u64 rcix)
{
	u64 rcix1, rcix2;
	int xsh, xsh2;
	
//	xsh=wrl->xsh;
//	xsh2=xsh<<1;

	BTM_UpdateWorldBlockOccCix(wrl, rcix);
//	BTM_UpdateWorldBlockOccCix(wrl, rcix+(1    ));
//	BTM_UpdateWorldBlockOccCix(wrl, rcix-(1    ));
//	BTM_UpdateWorldBlockOccCix(wrl, rcix+(1<< 7));
//	BTM_UpdateWorldBlockOccCix(wrl, rcix-(1<< 7));
//	BTM_UpdateWorldBlockOccCix(wrl, rcix+(1<<14));
//	BTM_UpdateWorldBlockOccCix(wrl, rcix-(1<<14));

	rcix1=BTM_BlockOffsetRcix(rcix,  1, 0, 0);
	rcix2=BTM_BlockOffsetRcix(rcix, -1, 0, 0);
	BTM_UpdateWorldBlockOccCix(wrl, rcix1);
	BTM_UpdateWorldBlockOccCix(wrl, rcix2);
	rcix1=BTM_BlockOffsetRcix(rcix, 0,  1, 0);
	rcix2=BTM_BlockOffsetRcix(rcix, 0, -1, 0);
	BTM_UpdateWorldBlockOccCix(wrl, rcix1);
	BTM_UpdateWorldBlockOccCix(wrl, rcix2);
	rcix1=BTM_BlockOffsetRcix(rcix, 0, 0,  1);
	rcix2=BTM_BlockOffsetRcix(rcix, 0, 0, -1);
	BTM_UpdateWorldBlockOccCix(wrl, rcix1);
	BTM_UpdateWorldBlockOccCix(wrl, rcix2);

	return(0);
}

int BTM_UpdateWorldBlockOccCix3R(BTM_World *wrl, u64 rcix, int rcnt)
{
	u64 rcix1, rcix2;
	int xsh, xsh2, rt;
	
//	xsh=wrl->xsh;
//	xsh2=xsh<<1;

	if(rcnt<=0)
		return(0);

	rt=BTM_UpdateWorldBlockOccCix(wrl, rcix);
	if(!rt)
		return(0);

	rcix1=BTM_BlockOffsetRcix(rcix,  1, 0, 0);
	rcix2=BTM_BlockOffsetRcix(rcix, -1, 0, 0);
	BTM_UpdateWorldBlockOccCix3R(wrl, rcix1, rcnt-1);
	BTM_UpdateWorldBlockOccCix3R(wrl, rcix2, rcnt-1);
	rcix1=BTM_BlockOffsetRcix(rcix, 0,  1, 0);
	rcix2=BTM_BlockOffsetRcix(rcix, 0, -1, 0);
	BTM_UpdateWorldBlockOccCix3R(wrl, rcix1, rcnt-1);
	BTM_UpdateWorldBlockOccCix3R(wrl, rcix2, rcnt-1);
	rcix1=BTM_BlockOffsetRcix(rcix, 0, 0,  1);
	rcix2=BTM_BlockOffsetRcix(rcix, 0, 0, -1);
	BTM_UpdateWorldBlockOccCix3R(wrl, rcix1, rcnt-1);
	BTM_UpdateWorldBlockOccCix3R(wrl, rcix2, rcnt-1);

	return(0);
}

u32 BTM_GetWorldBlockCix(BTM_World *wrl, u64 rcix)
{
	BTM_Region *rgn;
	u32 blk;
	int cix, rix;

	rix=BTM_Rcix2Rix(rcix);
	cix=BTM_Rcix2Cix(rcix);

	rgn=BTM_GetRegionForRix(wrl, rix);

	blk=BTM_GetRegionBlockCix(wrl, rgn, cix);
//	blk=rgn->vox[cix];
	return(blk);
}

u32 BTM_TryGetWorldBlockCix(BTM_World *wrl, u64 rcix)
{
	BTM_Region *rgn;
	u32 blk;
	int cix, rix;

	rix=BTM_Rcix2Rix(rcix);
	cix=BTM_Rcix2Cix(rcix);

	rgn=BTM_LookupRegionForRix(wrl, rix);
	if(!rgn)
		return(0);

	blk=BTM_GetRegionBlockCix(wrl, rgn, cix);
//	blk=rgn->vox[cix];
	return(blk);
}

u32 BTM_GetWorldBlockXYZ(BTM_World *wrl,
	int cx, int cy, int cz)
{
	u64 rcix;
	int cix, rix, rx, ry;

	if(cz<0)
		return(0);
	if(cz>=128)
		return(0);

//	msk=(1<<21)-1;

//	xsh=wrl->xsh;
//	xsh2=xsh<<1;
//	cix=(((cz<<7)|cy)<<7)|cx;
//	rcix=(((u64)rix)<<24)|cix;

	rcix=BTM_BlockCoordsToRcix(cx, cy, cz);
	
	return(BTM_GetWorldBlockCix(wrl, rcix));
}

u32 BTM_TryGetWorldBlockXYZ(BTM_World *wrl,
	int cx, int cy, int cz)
{
	u64 rcix;
	int cix, rix, rx, ry;

	if(cz<0)
		return(0);
	if(cz>=128)
		return(0);

	rcix=BTM_BlockCoordsToRcix(cx, cy, cz);	
	return(BTM_TryGetWorldBlockCix(wrl, rcix));
}

u32 BTM_GetWorldBlockCorg(BTM_World *wrl, u64 corg)
{
	int cx, cy, cz;
	u32 v;
	
	cx=(corg>> 8)&65535;
	cy=(corg>>32)&65535;
	cz=(corg>>56)&255;
	v=BTM_GetWorldBlockXYZ(wrl, cx, cy, cz);
	return(v);
}

u32 BTM_TryGetWorldBlockCorg(BTM_World *wrl, u64 corg)
{
	int cx, cy, cz;
	u32 v;
	
	cx=(corg>> 8)&65535;
	cy=(corg>>32)&65535;
	cz=(corg>>56)&255;
	v=BTM_TryGetWorldBlockXYZ(wrl, cx, cy, cz);
	return(v);
}

int BTM_UpdateWorldBlockOcc(BTM_World *wrl)
{
	int xsh, zsh, msk;
	int i;

//	xsh=wrl->xsh;
//	zsh=wrl->zsh;
//	msk=(1<<(xsh+xsh+zsh))-1;
//	for(i=0; i<(msk+1); i++)
//		BTM_UpdateWorldBlockOccCix(wrl, i);
	return(0);
}

BTM_World *BTM_AllocWorld(int xsh, int zsh)
{
	BTM_World *tmp;
	int x, y, z, ix, xs, zs;
	int i, j;
	
	tmp=btm_malloc(sizeof(BTM_World));
	memset(tmp, 0, sizeof(BTM_World));

//	tmp->xsh=xsh;
//	tmp->zsh=zsh;
//	tmp->vox=malloc((1<<(xsh+xsh+zsh))*sizeof(u32));
//	memset(tmp->vox, 0, (1<<(xsh+xsh+zsh))*sizeof(u32));

//	tmp->voxbm=malloc((1<<(xsh+xsh+zsh-6))*8);
//	memset(tmp->voxbm, 0, (1<<(xsh+xsh+zsh-6))*8);
	
#if 0
	xs=1<<xsh;
	zs=1<<zsh;
	for(x=0; x<xs; x++)
		for(y=0; y<xs; y++)
	{
//		z=0;
//		ix=(((z<<xsh)|y)<<xsh)|x;
//		BTM_SetWorldBlockCix(tmp, ix, 0x1);

		z=zs-1;
		ix=(((z<<xsh)|y)<<xsh)|x;
		BTM_SetWorldBlockCix(tmp, ix, 0x22);
	}
#endif
	
	tmp->magic1=BTM_MAGIC1;
	tmp->magic2=BTM_MAGIC1;
	tmp->magic3=BTM_MAGIC1;
	tmp->magic4=BTM_MAGIC1;
	tmp->magic5=BTM_MAGIC1;
//	tmp->magic1=BTM_MAGIC1;
	
	return(tmp);
}

int BTM_CheckWorldMagic(BTM_World *wrl)
{
	if(	(wrl->magic1!=BTM_MAGIC1) ||
		(wrl->magic2!=BTM_MAGIC1) ||
		(wrl->magic3!=BTM_MAGIC1) ||
		(wrl->magic4!=BTM_MAGIC1) ||
		(wrl->magic5!=BTM_MAGIC1)	)
	{
		debug_break
	}
	return(0);
}

int BTM_GetWorldTimeHhMm(BTM_World *wrl)
{
	int hh, mm, hm, tt;

	tt=wrl->daytimer;
	hh=tt/60000;
	mm=(tt-(hh*60000))/1000;
	
	hh+=6;
	if(hh>=24)
		hh-=24;
	
	hm=hh*100+mm;
	return(hm);
}

int BTM_SetWorldTimeHhMm(BTM_World *wrl, int hhmm)
{
	int hh, mm, hm, tt;

	hm=hhmm;

	hh=hm/100;
	mm=hm%100;

	hh=hh-6;
	if(hh<0)
		hh+=24;
	
	tt=((hh*60+mm)*1000);
	wrl->daytimer=tt;
	return(0);
}

#if 0
static const int  btmgl_cube_tris[6*6]=
{
	0, 1, 5,  0, 5, 4,	//-Y
	1, 3, 7,  1, 7, 5,	//+X
	3, 2, 6,  3, 6, 7,	//+Y
	2, 0, 4,  2, 4, 6,	//-X
	4, 5, 7,  4, 7, 6,	//+Z
	1, 0, 2,  1, 2, 3	//-Z
};

static const int  btmgl_cube_quads[6*4]=
{
	0, 1, 5, 4,	//-Y
	1, 3, 7, 5,	//+X
	3, 2, 6, 7,	//+Y
	2, 0, 4, 6,	//-X
	4, 5, 7, 6,	//+Z
	1, 0, 2, 3	//-Z
};
#endif

#if 1
static const int  btmgl_cube_tris[6*6]=
{
	1, 3, 7,  1, 7, 5,	//+X
	2, 0, 4,  2, 4, 6,	//-X
	3, 2, 6,  3, 6, 7,	//+Y
	0, 1, 5,  0, 5, 4,	//-Y
	4, 5, 7,  4, 7, 6,	//+Z
	1, 0, 2,  1, 2, 3	//-Z
};

static const int  btmgl_cube_quads[6*4]=
{
	1, 3, 7, 5,	//+X
	2, 0, 4, 6,	//-X
	3, 2, 6, 7,	//+Y
	0, 1, 5, 4,	//-Y
	4, 5, 7, 6,	//+Z
	1, 0, 2, 3	//-Z
};

static const int  btmgl_cross_quads[6*4]=
{
	1, 2, 6, 5, //Cross X/Y 1
	3, 0, 4, 7, //Cross X/Y 2

	3, 2, 6, 7,	//+Y
	0, 1, 5, 4,	//-Y
	4, 5, 7, 6,	//+Z
	1, 0, 2, 3	//-Z
};
#endif


void *btmgl_array_mutex;
void *btmgl_world_mutex;

float *btmgl_blkemit_xyz;
float *btmgl_blkemit_st;
u32 *btmgl_blkemit_rgb;
int btmgl_blkemit_npts;
int btmgl_blkemit_mpts;

float *btmgl_blkemit2_xyz;
float *btmgl_blkemit2_st;
u32 *btmgl_blkemit2_rgb;
int btmgl_blkemit2_npts;

int BTMGL_EmitLockArrays()
{
#ifdef BTM_RAYTHREAD
	if(!btmgl_array_mutex)
		btmgl_array_mutex=thMutex();
	thLockMutex(btmgl_array_mutex);
#endif
	return(0);
}

int BTMGL_EmitUnlockArrays()
{
#ifdef BTM_RAYTHREAD
	thUnlockMutex(btmgl_array_mutex);
#endif
	return(0);
}

int BTMGL_LockWorld()
{
#ifdef BTM_RAYTHREAD
	if(!btmgl_world_mutex)
		btmgl_world_mutex=thMutex();
	thLockMutex(btmgl_world_mutex);
#endif
	return(0);
}

int BTMGL_UnlockWorld()
{
#ifdef BTM_RAYTHREAD
	thUnlockMutex(btmgl_world_mutex);
#endif
	return(0);
}


int BTMGL_EmitBlockVertex(float *xyz, float *st, u32 rgb)
{
	int ix;
	int i, j, k;
	
	if(!btmgl_blkemit_xyz)
	{
#ifdef BTM_RAYTHREAD
		if(!btmgl_array_mutex)
		{
			btmgl_array_mutex=thMutex();
			btmgl_world_mutex=thMutex();
		}
#endif
	
		BTMGL_EmitLockArrays();

//		k=(btm_drawdist*btm_drawdist)*12;
//		k=(btm_drawdist*btm_drawdist)*14;
//		k=(btm_drawdist*btm_drawdist)*16;
		k=(btm_drawdist*btm_drawdist)*18;
		
		ix=4096;
		while(ix<k)
			ix=ix<<1;

//		ix=4096;
//		ix=1<<19;
//		ix=1<<18;
		btmgl_blkemit_xyz=btm_malloc(ix*4*sizeof(float));
		btmgl_blkemit_st=btm_malloc(ix*2*sizeof(float));
		btmgl_blkemit_rgb=btm_malloc(ix*1*sizeof(u32));

#ifdef BTM_RAYTHREAD
		btmgl_blkemit2_xyz=btm_malloc(ix*4*sizeof(float));
		btmgl_blkemit2_st=btm_malloc(ix*2*sizeof(float));
		btmgl_blkemit2_rgb=btm_malloc(ix*1*sizeof(u32));
#else
		btmgl_blkemit2_xyz=btmgl_blkemit_xyz;
		btmgl_blkemit2_st=btmgl_blkemit_st;
		btmgl_blkemit2_rgb=btmgl_blkemit_rgb;
#endif

		btmgl_blkemit_mpts=ix;

		BTMGL_EmitUnlockArrays();
	}
	
	if((btmgl_blkemit_npts+1)>=btmgl_blkemit_mpts)
	{
		BTMGL_EmitLockArrays();
	
		ix=btmgl_blkemit_mpts+(btmgl_blkemit_mpts>>1);

		btmgl_blkemit_xyz=btm_realloc(btmgl_blkemit_xyz,
			ix*4*sizeof(float));
		btmgl_blkemit_st=btm_realloc(btmgl_blkemit_st,
			ix*2*sizeof(float));
		btmgl_blkemit_rgb=btm_realloc(btmgl_blkemit_rgb,
			ix*1*sizeof(u32));

#ifdef BTM_RAYTHREAD
		btmgl_blkemit2_xyz=btm_realloc(btmgl_blkemit2_xyz,
			ix*4*sizeof(float));
		btmgl_blkemit2_st=btm_realloc(btmgl_blkemit2_st,
			ix*2*sizeof(float));
		btmgl_blkemit2_rgb=btm_realloc(btmgl_blkemit2_rgb,
			ix*1*sizeof(u32));
#else
		btmgl_blkemit2_xyz=btmgl_blkemit_xyz;
		btmgl_blkemit2_st=btmgl_blkemit_st;
		btmgl_blkemit2_rgb=rgb;
#endif

		btmgl_blkemit_mpts=ix;

		BTMGL_EmitUnlockArrays();	
	}
	
	ix=btmgl_blkemit_npts++;

	memcpy(btmgl_blkemit_xyz+ix*4, xyz, 3*sizeof(float));
	memcpy(btmgl_blkemit_st+ix*2, st, 2*sizeof(float));
	btmgl_blkemit_rgb[ix]=rgb;
	return(0);
}

int BTMGL_EmitBlockFaces(int cx, int cy, int cz, int fm, u32 blk,
	u64 lbl, u64 lsl)
{
	float xyz[8*4];
	float sta[4*4*2];
	const int *tri;
	float *st;
	float mx, my, mz, nx, ny, nz;
	int i0, i1, i2, i3;
	u32 rgb, rgb1, binf;
	int bt, tx, ty, bl, sl, ma;
	int i, j, k, l;
	
	if(!(fm&0x3F))
		return(0);
	
	bt=blk&255;
	if(bt<4)
		return(0);
	if(bt==BTM_BLKTY_SKY1)
		return(0);
	
	if(!(fm&0x80))
	{
		k=(blk>>24)&0x3F;
		fm&=~k;
		if(!(fm&0x3F))
			return(0);
	}
	
	binf=btmgl_vox_atlas_side[bt];
	
	mx=cx;	nx=cx+1;
	my=cy;	ny=cy+1;
	mz=cz;	nz=cz+1;
	
	if(fm&0x80)
	{
		nz=cz+0.5;
	}
	
	for(i=0; i<8; i++)
	{
		j=i*4;
		xyz[j+0]=(i&1)?nx:mx;
		xyz[j+1]=(i&2)?ny:my;
		xyz[j+2]=(i&4)?nz:mz;
	}

//	rgb=0xFFFFFFFFU;

	
	tx=(binf>> 0)&15;	ty=15-((binf>>4)&15);
	sta[0]=(tx+0)*(1.0/16)+(1.0/2048);	sta[1]=1.0-(1.0/2048)-(ty+0)*(1.0/16);
	sta[2]=(tx+1)*(1.0/16)-(1.0/2048);	sta[3]=1.0-(1.0/2048)-(ty+0)*(1.0/16);
	sta[4]=(tx+0)*(1.0/16)+(1.0/2048);	sta[5]=1.0+(1.0/2048)-(ty+1)*(1.0/16);
	sta[6]=(tx+1)*(1.0/16)-(1.0/2048);	sta[7]=1.0+(1.0/2048)-(ty+1)*(1.0/16);

	if(((binf>>8)&0xFF) == (binf&0xFF))
	{
		memcpy(sta+ 8, sta+0, 8*sizeof(float));
		memcpy(sta+16, sta+0, 8*sizeof(float));
	}else
	{
		tx=(binf>> 8)&15;	ty=15-((binf>>12)&15);
		sta[ 8]=(tx+0)*(1.0/16)+(1.0/2048);
		sta[ 9]=(ty+0)*(1.0/16)+(1.0/2048);
		sta[10]=(tx+1)*(1.0/16)-(1.0/2048);
		sta[11]=(ty+0)*(1.0/16)+(1.0/2048);
		sta[12]=(tx+0)*(1.0/16)+(1.0/2048);
		sta[13]=(ty+1)*(1.0/16)-(1.0/2048);
		sta[14]=(tx+1)*(1.0/16)-(1.0/2048);
		sta[15]=(ty+1)*(1.0/16)-(1.0/2048);

		tx=(binf>>16)&15;	ty=15-((binf>>20)&15);
		sta[16]=(tx+0)*(1.0/16)+(1.0/2048);
		sta[17]=(ty+0)*(1.0/16)+(1.0/2048);
		sta[18]=(tx+1)*(1.0/16)-(1.0/2048);
		sta[19]=(ty+0)*(1.0/16)+(1.0/2048);
		sta[20]=(tx+0)*(1.0/16)+(1.0/2048);
		sta[21]=(ty+1)*(1.0/16)-(1.0/2048);
		sta[22]=(tx+1)*(1.0/16)-(1.0/2048);
		sta[23]=(ty+1)*(1.0/16)-(1.0/2048);

		for(i=4; i<12; i++)
		{
			sta[i*2+1]=1.0-sta[i*2+1];
		}
	}

	if(binf&BTM_BLKDFL_FLUID)
	{
		st=sta;
		
		for(i=0; i<3; i++)
		{
			i0=((btmgl_time_ms>>5)+(cx<<4)-(cy<<4))&255;
			i1=(i0+16)&255;
			i2=(i0-16)&255;
			i3=(i2+16)&255;
			st[0]+=btmra_sinang_f[i0]*(0.125/16);
			st[1]+=btmra_cosang_f[i0]*(0.125/16);
			st[2]+=btmra_sinang_f[i1]*(0.125/16);
			st[3]+=btmra_cosang_f[i1]*(0.125/16);
			st[4]+=btmra_sinang_f[i2]*(0.125/16);
			st[5]+=btmra_cosang_f[i2]*(0.125/16);
			st[6]+=btmra_sinang_f[i3]*(0.125/16);
			st[7]+=btmra_cosang_f[i3]*(0.125/16);
			st+=8;
		}
	}

#if 1
	for(i=0; i<6; i++)
	{
		if(!(fm&(1<<i)))
			continue;

		bl=(lbl>>(i*8))&255;
		sl=(lsl>>(i*8))&255;
		
		l=((bl&15)>(sl&15))?bl:sl;

		if(i==4)
		{
			rgb=0xFFFFFFFFU;
			st=sta+8;
		}else if(i==5)
		{
			rgb=0xFF9F9F9FU;
			st=sta+16;
		}else
		{
			rgb=0xFFBFBFBFU;
			st=sta;
		}
		
		if(fm&64)
		{
			rgb&=0xFF7FFF7FU;
		}

		if(l!=0x0F)
		{
			rgb=BTM_ModulateColorRgbForBlockLight(rgb, l);
		}

		tri=btmgl_cube_quads+i*4;
		BTMGL_EmitBlockVertex(xyz+tri[0]*4, st+0*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[1]*4, st+1*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[2]*4, st+3*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[3]*4, st+2*2, rgb);
	}
#endif

	return(0);
}

int BTMGL_EmitBlockFacesSlab(
	int cx, int cy, int cz,
	int fm, u32 blk,
	u64 lbl, u64 lsl)
{
	BTMGL_EmitBlockFaces(cx, cy, cz, 0x80|fm, blk, lbl, lsl);
	return(0);
}



int BTMGL_EmitScaledBlockFaces(
	float cxm, float cym, float czm,
	float cxn, float cyn, float czn,
	int fm, u32 blk,
	u64 lbl, u64 lsl)
{
	float xyz[8*4];
	float sta[4*4*2];
	const int *tri;
	float *st;
	float mx, my, mz, nx, ny, nz;
	int i0, i1, i2, i3;
	u32 rgb, rgb1, binf;
	int bt, tx, ty, bl, sl, ma;
	int i, j, k, l;
	
	if(!(fm&0x3F))
		return(0);
	
	bt=blk&255;
	binf=btmgl_vox_atlas_side[bt];
	
	mx=cxm;	nx=cxn;
	my=cym;	ny=cyn;
	mz=czm;	nz=czn;
	
	for(i=0; i<8; i++)
	{
		j=i*4;
		xyz[j+0]=(i&1)?nx:mx;
		xyz[j+1]=(i&2)?ny:my;
		xyz[j+2]=(i&4)?nz:mz;
	}

	tx=(binf>> 0)&15;	ty=15-((binf>>4)&15);
	sta[0]=(tx+0)*(1.0/16)+(1.0/512);	sta[1]=1.0-(1.0/512)-(ty+0)*(1.0/16);
	sta[2]=(tx+1)*(1.0/16)-(1.0/512);	sta[3]=1.0-(1.0/512)-(ty+0)*(1.0/16);
	sta[4]=(tx+0)*(1.0/16)+(1.0/512);	sta[5]=1.0+(1.0/512)-(ty+1)*(1.0/16);
	sta[6]=(tx+1)*(1.0/16)-(1.0/512);	sta[7]=1.0+(1.0/512)-(ty+1)*(1.0/16);

	if(((binf>>8)&0xFF) == (binf&0xFF))
	{
		memcpy(sta+ 8, sta+0, 8*sizeof(float));
		memcpy(sta+16, sta+0, 8*sizeof(float));
	}else
	{
		tx=(binf>> 8)&15;	ty=15-((binf>>12)&15);
		sta[ 8]=(tx+0)*(1.0/16)+(1.0/256);	sta[ 9]=(ty+0)*(1.0/16)+(1.0/256);
		sta[10]=(tx+1)*(1.0/16)-(1.0/256);	sta[11]=(ty+0)*(1.0/16)+(1.0/256);
		sta[12]=(tx+0)*(1.0/16)+(1.0/256);	sta[13]=(ty+1)*(1.0/16)-(1.0/256);
		sta[14]=(tx+1)*(1.0/16)-(1.0/256);	sta[15]=(ty+1)*(1.0/16)-(1.0/256);

		tx=(binf>>16)&15;	ty=15-((binf>>20)&15);
		sta[16]=(tx+0)*(1.0/16)+(1.0/256);	sta[17]=(ty+0)*(1.0/16)+(1.0/256);
		sta[18]=(tx+1)*(1.0/16)-(1.0/256);	sta[19]=(ty+0)*(1.0/16)+(1.0/256);
		sta[20]=(tx+0)*(1.0/16)+(1.0/256);	sta[21]=(ty+1)*(1.0/16)-(1.0/256);
		sta[22]=(tx+1)*(1.0/16)-(1.0/256);	sta[23]=(ty+1)*(1.0/16)-(1.0/256);

		for(i=4; i<12; i++)
		{
			sta[i*2+1]=1.0-sta[i*2+1];
		}
	}

#if 0
	if(binf&BTM_BLKDFL_FLUID)
	{
		st=sta;
		
		for(i=0; i<3; i++)
		{
			i0=((btmgl_time_ms>>5)+(cxm<<4)-(cym<<4))&255;
			i1=(i0+16)&255;
			i2=(i0-16)&255;
			i3=(i2+16)&255;
			st[0]+=btmra_sinang_f[i0]*(0.125/16);
			st[1]+=btmra_cosang_f[i0]*(0.125/16);
			st[2]+=btmra_sinang_f[i1]*(0.125/16);
			st[3]+=btmra_cosang_f[i1]*(0.125/16);
			st[4]+=btmra_sinang_f[i2]*(0.125/16);
			st[5]+=btmra_cosang_f[i2]*(0.125/16);
			st[6]+=btmra_sinang_f[i3]*(0.125/16);
			st[7]+=btmra_cosang_f[i3]*(0.125/16);
			st+=8;
		}
	}
#endif

#if 1
	for(i=0; i<6; i++)
	{
		if(!(fm&(1<<i)))
			continue;

		bl=(lbl>>(i*8))&255;
		sl=(lsl>>(i*8))&255;
		
		l=((bl&15)>(sl&15))?bl:sl;

		if(i==4)
		{
			rgb=0xFFFFFFFFU;
			st=sta+8;
		}else if(i==5)
		{
			rgb=0xFF9F9F9FU;
			st=sta+16;
		}else
		{
			rgb=0xFFBFBFBFU;
			st=sta;
		}
		
		if(fm&64)
		{
			rgb&=0xFF7FFF7FU;
		}

		if(l!=0x0F)
		{
			rgb=BTM_ModulateColorRgbForBlockLight(rgb, l);
		}

		if(fm&0x100)
		{
			tri=btmgl_cross_quads+i*4;
		}else
		{
			tri=btmgl_cube_quads+i*4;
		}
		BTMGL_EmitBlockVertex(xyz+tri[0]*4, st+0*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[1]*4, st+1*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[2]*4, st+3*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[3]*4, st+2*2, rgb);
	}
#endif

	return(0);
}

int BTMGL_EmitBlockFacesStair(
	int cx, int cy, int cz,
	int fm, u32 blk,
	u64 lbl, u64 lsl)
{
	BTMGL_EmitScaledBlockFaces(
		cx+0, cy+0, cz+0,
		cx+1, cy+1, cz+0.33,
		fm, blk, lbl, lsl);

	switch((blk>>8)&3)
	{
	case 0:
		BTMGL_EmitScaledBlockFaces(
			cx+0.00, cy+0.00, cz+0.00,
			cx+1.00, cy+0.33, cz+1.00,
			fm, blk, lbl, lsl);
		BTMGL_EmitScaledBlockFaces(
			cx+0.00, cy+0.33, cz+0.00,
			cx+1.00, cy+0.66, cz+0.66,
			fm, blk, lbl, lsl);
		break;
	case 2:
		BTMGL_EmitScaledBlockFaces(
			cx+0.00, cy+0.66, cz+0.00,
			cx+1.00, cy+1.00, cz+1.00,
			fm, blk, lbl, lsl);
		BTMGL_EmitScaledBlockFaces(
			cx+0.00, cy+0.33, cz+0.00,
			cx+1.00, cy+0.66, cz+0.66,
			fm, blk, lbl, lsl);
		break;

	case 3:
		BTMGL_EmitScaledBlockFaces(
			cx+0.00, cy+0.00, cz+0.00,
			cx+0.33, cy+1.00, cz+1.00,
			fm, blk, lbl, lsl);
		BTMGL_EmitScaledBlockFaces(
			cx+0.33, cy+0.00, cz+0.00,
			cx+0.66, cy+1.00, cz+0.66,
			fm, blk, lbl, lsl);
		break;
	case 1:
		BTMGL_EmitScaledBlockFaces(
			cx+0.66, cy+0.00, cz+0.00,
			cx+1.00, cy+1.00, cz+1.00,
			fm, blk, lbl, lsl);
		BTMGL_EmitScaledBlockFaces(
			cx+0.33, cy+0.00, cz+0.00,
			cx+0.66, cy+1.00, cz+0.66,
			fm, blk, lbl, lsl);
		break;
	}

//	BTMGL_EmitBlockFaces(cx, cy, cz, 0x80|fm, blk, lbl, lsl);
	return(0);
}

int BTMGL_EmitBlockFacesPlant(
	int cx, int cy, int cz,
	int fm, u32 blk,
	u64 lbl, u64 lsl)
{
	u32 blkd;
	float zofsb, zofst;

	blkd=btmgl_vox_atlas_side[blk&255];

	zofsb=0;
	zofst=0;
	
	if((blkd&BTM_BLKDFL_TY_MASK)==BTM_BLKDFL_TY_CROP)
	{
		zofsb=(-0.5)+((((blk>>8)&15)+1)/16.0)*0.5;
		zofst=(-0.9)+((((blk>>8)&15)+1)/16.0)*0.9;
	}

	BTMGL_EmitScaledBlockFaces(
		cx+0, cy+0, cz+0+zofsb,
		cx+1, cy+1, cz+1+zofst,
		0x103, blk, lbl, lsl);
	return(0);
}


int BTMGL_DrawSceneBlocks(BTM_World *wrl)
{
	static int lastframe=-1;
	BTM_Region *rgn;
	u32 *dbd;
	u64 *dbp;
	int *dbl;
//	u32 *vox;

	u64 cpos, lbl, lsl;
	u32 blk, blk1, blkd;
	u64 rcix, rcix1;

	int pos, cix, cix1, rix, rix1;
	int rgix, cx, cy, cz, vx, vy, vz;
	int px, py, pz, pd;
	int cxm, czm, xsh, zsh, xzshr;
	int cxfull, cxhalf, cxlqtr, cxhqtr;
	int ra, sina, cosa;
	int np, fm;
	int i, j, k;
	
//	xsh=wrl->xsh;
//	zsh=wrl->zsh;
//	cxm=(1<<xsh)-1;
//	czm=(1<<zsh)-1;
//	vox=wrl->vox;
	
//	xzshr=xsh+xsh;

	if(wrl->frame==lastframe)
		return(0);
	lastframe=wrl->frame;

	BTMGL_LockWorld();

	cxfull=1<<(16-0);
	cxhalf=1<<(16-1);
	cxlqtr=1<<(16-2);
	cxhqtr=cxhalf+cxlqtr;
	
	ra=wrl->cam_yaw&255;
	cosa=btmra_cosang[ra];
	sina=btmra_sinang[ra];

	vx=(wrl->cam_org>> 0)&0xFFFFFF;
	vy=(wrl->cam_org>>24)&0xFFFFFF;
	vz=(wrl->cam_org>>48)&0x00FFFF;
//	vx=(vx<< 8)>> 8;
//	vy=(vy<< 8)>> 8;
//	vz=(vz<<16)>>16;
	
	vx>>=8;
	vy>>=8;
	vz>>=8;

	btmgl_blkemit_npts = 0;
	
	rgn=NULL; rgix=-1;
//	vox=NULL;

	np=wrl->scr_npts;
	for(i=0; i<np; i++)
	{
		rcix=wrl->scr_pts_list[i];
		rix=BTM_Rcix2Rix(rcix);
		cix=BTM_Rcix2Cix(rcix);
		
		cpos=BTM_ConvRcixToBlkPos(rcix);
		cx=(cpos>> 0)&65535;
		cy=(cpos>>16)&65535;
		cz=(cpos>>32)&255;
		
//		cx=(cix>> 0)&127;
//		cy=(cix>> 7)&127;
//		cz=(cix>>14)&127;	
//		cx+=((rix>>0)&255)<<7;
//		cy+=((rix>>8)&255)<<7;

		if(rix!=rgix)
		{
//			rgn=BTM_GetRegionForRix(wrl, rix);
			rgn=BTM_LookupRegionForRix(wrl, rix);
			if(!rgn)
				continue;
			rgix=rix;
//			vox=rgn->vox;
		}

#if 1
		if(vx>cxhalf)
		{	if(cx<cxlqtr)
				cx+=cxfull;		}
		else
		{	if(cx>cxhqtr)
				cx-=cxfull;		}
		if(vy>cxhalf)
		{	if(cy<cxlqtr)
				cy+=cxfull;		}
		else
		{	if(cy>cxhqtr)
				cy-=cxfull;		}
#endif

//		blk=vox[cix];
		blk=BTM_GetRegionBlockCix(wrl, rgn, cix);
		fm=0x3F;
//		lsl=0xFFFFFFFFFFFFULL;
		blkd=btmgl_vox_atlas_side[blk&255];

//		if(!(blkd&(BTM_BLKDFL_SEETHRU|BTM_BLKDFL_FLUID)))
//		if(!(blkd&BTM_BLKDFL_FLUID))
		if(!(blkd&BTM_BLKDFL_FLUID) && !(blkd&BTM_BLKDFL_TY_MASK))
		{
			if(vy>cy)
	//			fm&=~1;
				fm&=~8;
			else
				fm&=~4;

			if(vx>cx)
	//			fm&=~8;
				fm&=~2;
			else
	//			fm&=~2;
				fm&=~1;

			if(vz>=cz)
				fm&=~32;
			else
				fm&=~16;
		}

		if(rcix==wrl->scr_lhit)
			fm|=64;

		lbl=0;
		lsl=0;
		
		for(j=0; j<6; j++)
		{
			if(!(fm&(1<<j)))
				continue;

			px=0; py=0; pz=0;
			switch(j)
			{
				case 0: px= 1; break;
				case 1: px=-1; break;
				case 2: py= 1; break;
				case 3: py=-1; break;
				case 4: pz= 1; break;
				case 5: pz=-1; break;
			}
			rcix1=BTM_BlockOffsetRcix(rcix, px, py, pz);
			rix1=BTM_Rcix2Cix(rcix1);

//			if(j==4)cix1=cix+(1<<14);
//			else if(j==5)cix1=cix-(1<<14);
//			else if(j==0)cix1=cix+1;
//			else if(j==1)cix1=cix-1;
//			else if(j==2)cix1=cix+(1<<7);
//			else if(j==3)cix1=cix-(1<<7);

			if(rix1==rix)
			{
				cix1=BTM_Rcix2Cix(rcix1);
				blk1=BTM_GetRegionBlockCix(wrl, rgn, cix1);
			}else
			{
//				blk1=BTM_GetWorldBlockCix(wrl, rcix1);
				blk1=BTM_TryGetWorldBlockCix(wrl, rcix1);
			}

//			lbl|=((blk1>>12)&63LL)<<(j*8);
			lbl|=((blk1>>12)&255LL)<<(j*8);
//			lsl|=((blk1>>18)&63LL)<<(j*8);

//			k=(blk1>>18)&63;
			k=(blk1>>20)&15;
			k=(k*wrl->daylight)>>4;
			lsl|=((u64)k)<<(j*8);

		}
		
		if((blkd&BTM_BLKDFL_TY_MASK)==BTM_BLKDFL_TY_FULL)
		{
			BTMGL_EmitBlockFaces(cx, cy, cz, fm, blk, lbl, lsl);
		}else if((blkd&BTM_BLKDFL_TY_MASK)==BTM_BLKDFL_TY_SLAB)
		{
			BTMGL_EmitBlockFacesSlab(cx, cy, cz, fm, blk, lbl, lsl);
		}else if((blkd&BTM_BLKDFL_TY_MASK)==BTM_BLKDFL_TY_STAIR)
		{
			BTMGL_EmitBlockFacesStair(cx, cy, cz, fm, blk, lbl, lsl);
		}else if((blkd&BTM_BLKDFL_TY_MASK)==BTM_BLKDFL_TY_PLANT)
		{
			BTMGL_EmitBlockFacesPlant(cx, cy, cz, fm, blk, lbl, lsl);
		}else if((blkd&BTM_BLKDFL_TY_MASK)==BTM_BLKDFL_TY_CROP)
		{
			BTMGL_EmitBlockFacesPlant(cx, cy, cz, fm, blk, lbl, lsl);
		}
	}

	BTMGL_UnlockWorld();

	BTMGL_EmitLockArrays();
	
#ifdef BTM_RAYTHREAD
	memcpy(btmgl_blkemit2_xyz, btmgl_blkemit_xyz, btmgl_blkemit_npts*4*4);
	memcpy(btmgl_blkemit2_st, btmgl_blkemit_st, btmgl_blkemit_npts*2*4);
	memcpy(btmgl_blkemit2_rgb, btmgl_blkemit_rgb, btmgl_blkemit_npts*1*4);
#endif

	btmgl_blkemit2_npts=btmgl_blkemit_npts;

	BTMGL_EmitUnlockArrays();

	return(0);
}

int BTMGL_DrawSceneArrays(BTM_World *wrl)
{
	if(btmgl_blkemit2_npts<=0)
		return(0);

	BTMGL_EmitLockArrays();

#ifdef BTM_TARGET_SMALL
	pglDisable(GL_ALPHA_TEST);
	pglEnable(GL_TEXTURE_2D);
#endif

//	pglBindTexture(TKRA_TEXTURE_2D, 2);
	pglBindTexture(GL_TEXTURE_2D, 2);

	pglEnableClientState(GL_VERTEX_ARRAY);
	pglEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	pglEnableClientState(GL_NORMAL_ARRAY);
	pglEnableClientState(GL_COLOR_ARRAY);

	pglVertexPointer(3, TKRA_GL_FLOAT, 4*4, btmgl_blkemit2_xyz+0);
	pglTexCoordPointer(2, TKRA_GL_FLOAT, 2*4, btmgl_blkemit2_st+0);
	pglColorPointer(4, TKRA_GL_UNSIGNED_BYTE, 4, btmgl_blkemit2_rgb+0);
//	pglDrawArrays(TKRA_GL_TRIANGLES, 0, btmgl_blkemit_npts);
	pglDrawArrays(TKRA_GL_QUADS, 0, btmgl_blkemit2_npts);

	pglDisableClientState(GL_VERTEX_ARRAY);
	pglDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	pglDisableClientState(GL_NORMAL_ARRAY);
	pglDisableClientState(GL_COLOR_ARRAY);

	BTMGL_EmitUnlockArrays();

	return(0);
}

int btmgl_filter_min;
int btmgl_filter_max;

void BTMGL_UploadCompressed (
	byte *data, byte mipmap, byte alpha)
{
	BTMGL_DDS_HEADER *tdds;
	byte *cs, *css;
	int xs, ys;
	int fmin, fmax;
	int xshl, xshl1, isz, mip, txc;
	int i, j, k;
	
	if(!memcmp(data, "DDS ", 4))
//	if(1)
	{
		tdds = (void *)(data+4);
		css=data+4+(tdds->dwSize);
//		xshl=data[4];

		xs=tdds->dwWidth;
		i=xs; xshl=0;
		while(i>1)
			{ i>>=1; xshl++; }
	}else
	{
		printf("BTMGL_UploadCompressed: Not DDS\n");
		return;
	}

//	txc=GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	txc=TKRA_GL_CMPR_RGBA_S3TC_DXT1;
	if(!(alpha&1))
//		txc=GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		txc=TKRA_GL_CMPR_RGB_S3TC_DXT1;

	fmin=btmgl_filter_min;
	fmax=btmgl_filter_max;
	
	if(alpha&2)
	{
		fmax=GL_NEAREST;
	}
	

	cs=css; mip=0;
	while(xshl>=0)
	{
		if(mip && !mipmap)
			break;
	
		xshl1=xshl-2;
		if(xshl1<0)
			xshl1=0;
	
		isz=1<<(xshl1+xshl1+3);
		pglCompressedTexImage2D(
			GL_TEXTURE_2D, mip, 
			txc,
			1<<xshl, 1<<xshl, 0, isz, cs);
		cs+=isz;
		xshl--; mip++;
	}

	if (mipmap)
	{
		pglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, fmin);
		pglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, fmax);
	}
	else
	{
		pglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, fmax);
		pglTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, fmax);
	}
}

