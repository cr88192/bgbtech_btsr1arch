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
Redesigned Raycaster
 */

/*
Raycast diverging line.
When the coordinates fall into different blocks, split the rays.
 */

u64 BTM_RaycastStepVector(int yang, int pang);
u64 BTM_RaycastStepVectorB(int yang, int pang);
u64 BTM_WorldCorgToRcix(u64 cpos);
u32 BTM_GetWorldBlockCorg(BTM_World *wrl, u64 corg);
u64 BTM_ConvRcixToBlkPos(u64 rcix);

u64 BTM_BlockOffsetRcix(u64 rcix, int dx, int dy, int dz);

u64 BTM_BlockOffsetRcixPx1(u64 rcix);
u64 BTM_BlockOffsetRcixNx1(u64 rcix);
u64 BTM_BlockOffsetRcixPy1(u64 rcix);
u64 BTM_BlockOffsetRcixNy1(u64 rcix);
u64 BTM_BlockOffsetRcixPz1(u64 rcix);
u64 BTM_BlockOffsetRcixNz1(u64 rcix);

BTM_Region *BTM_GetRegionForRix(BTM_World *wrl, int rix);
BTM_Region *BTM_LookupRegionForRix(BTM_World *wrl, int rix);

u32 BTM_GetRegionBlockCix(BTM_World *wrl, BTM_Region *rgn, int cix);
int BTM_SetRegionBlockCix(BTM_World *wrl,
	BTM_Region *rgn, int cix, u32 blk);

byte *BTM_RegionDoLoadBufferIx(BTM_World *wrl, BTM_Region *rgn,
	int ch, int ldfl, int *rsz);
int BTM_RegionDoStoreBufferIx(BTM_World *wrl,
	BTM_Region *rgn, int ch, byte *ibuf, int isz);

int BTM_SpawnRegionEntities(BTM_World *wrl,
	BTM_Region *rgn, BCCX_Node *ents);
BCCX_Node *BTM_FlattenRegionLiveEntities(
	BTM_World *wrl, BTM_Region *rgn);

BCCX_Node *BTM_FlattenWorldGlobalState(BTM_World *wrl);

u32 BTM_ModulateColorRgbForBlockLight(u32 rgb, int ll);

// #define	BTM_Rcix2Rix(rcix)	((rcix>>21)&65535);
#define	BTM_Rcix2Rix(rcix)	((rcix>>21)&262143);
#define	BTM_Rcix2Cix(rcix)	((rcix)&((1<<21)-1));

s32 *btmra_sinang;
s32 *btmra_cosang;

float *btmra_sinang_f;
float *btmra_cosang_f;

int btm_drawdist;
int cam_mvflags;

int btmgl_time_ms;

int btm_drawstat_dist[1024];
int btm_drawstat_dist_hrsc[1024];

u64 btm_raycast_avgvec(u64 vec0, u64 vec1)
{
	u64 vec2;
	int cx0, cx1, cy0, cy1, cz0, cz1;
	int cx2, cy2, cz2;
	
	cx0=(vec0>> 0)&0x00FFFFFF;
	cx1=(vec1>> 0)&0x00FFFFFF;
	cy0=(vec0>>24)&0x00FFFFFF;
	cy1=(vec1>>24)&0x00FFFFFF;
	cz0=(vec0>>48)&0x0000FFFF;
	cz1=(vec1>>48)&0x0000FFFF;
	
	cx0=((int)(cx0<<8))>>8;
	cx1=((int)(cx1<<8))>>8;
	cy0=((int)(cy0<<8))>>8;
	cy1=((int)(cy1<<8))>>8;
	cz0=((int)(cz0<<16))>>16;
	cz1=((int)(cz1<<16))>>16;
	
	cx2=(cx0+cx1)>>1;
	cy2=(cy0+cy1)>>1;
	cz2=(cz0+cz1)>>1;
	
	vec2=
		((cx2&0x00FFFFFFULL)<< 0) |
		((cy2&0x00FFFFFFULL)<<24) |
		((cz2&0x0000FFFFULL)<<48) ;
	return(vec2);
}

int BTM_RaycastTryAddHitCix1(BTM_World *wrl, u64 rcix, int nrcnt)
{
	BTM_Region *rgn;
	u64 blk;
	u64 *lst;
	int *chn;
	int *hash;
	byte *rcnt;
	int msk, bt, fm, rix, cix;

	int i, j, k, n, h;

	rix=BTM_Rcix2Rix(rcix);
	cix=BTM_Rcix2Cix(rcix);

	rgn=BTM_GetRegionForRix(wrl, rix);

//	blk=rgn->vox[cix];
	blk=BTM_GetRegionBlockCix(wrl, rgn, cix);
	bt=blk&0xFF;
	fm=(blk>>24)&63;
//	if(bt<2)
	if(bt<4)
		return(0);
	if(fm==63)
		return(0);

//	h=((rcix*(251*65521))>>24)&63;
	h=((rcix*(251*65521))>>24)&(BTM_RAYCAST_HASHSZ-1);

	if(nrcnt>30)
	{
		if(wrl->scr_cxpred[h]==rcix)
			return(0);
		wrl->scr_cxpred[h]=rcix;
	}

	lst=wrl->scr_pts_list;
	chn=wrl->scr_pts_chn;
	hash=wrl->scr_pts_hash;
	rcnt=wrl->scr_pts_rcnt;

	i=hash[h];
	while(i>=0)
	{
		if(lst[i]==rcix)
		{
			rcnt[i]=nrcnt;
			return(0);
		}
		i=chn[i];
	}
	
	if(wrl->scr_npts>=(BTM_RAYCAST_MAXHITS-1))
		return(0);
	
	i=wrl->scr_npts++;
	lst[i]=rcix;
	chn[i]=hash[h];
	rcnt[i]=nrcnt;
	hash[h]=i;

	return(1);
}

int BTM_RaycastTryAddHitCix(BTM_World *wrl, u64 cix)
{
	u64 cix1, cix2;
	int i, l, to;
	if(cix==wrl->scr_hpred)
		return(0);
	wrl->scr_hpred=cix;

	l=BTM_WorldCameraDistCix(wrl, cix);
	if(l>btm_drawdist)
		return(0);

//	to=33;
	to=64;

#ifdef _WIN32
//	to=255;
	to=127;
#endif

	i=BTM_RaycastTryAddHitCix1(wrl, cix, to);

#if 0
	i+=BTM_RaycastTryAddHitCix1(wrl, cix+1ULL, to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix-1ULL, to);
//	i+=BTM_RaycastTryAddHitCix1(wrl, cix+(1ULL<<24), to);
//	i+=BTM_RaycastTryAddHitCix1(wrl, cix-(1ULL<<24), to);
//	i+=BTM_RaycastTryAddHitCix1(wrl, cix+(1ULL<<48), to);
//	i+=BTM_RaycastTryAddHitCix1(wrl, cix-(1ULL<<48), to);

//	i+=BTM_RaycastTryAddHitCix1(wrl, cix+(1ULL<<7), to);
//	i+=BTM_RaycastTryAddHitCix1(wrl, cix-(1ULL<<7), to);
//	i+=BTM_RaycastTryAddHitCix1(wrl, cix+(1ULL<<14), to);
//	i+=BTM_RaycastTryAddHitCix1(wrl, cix-(1ULL<<14), to);

	i+=BTM_RaycastTryAddHitCix1(wrl, cix+(1ULL<<4), to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix-(1ULL<<4), to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix+(1ULL<<8), to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix-(1ULL<<8), to);
#endif

#if 1
//	cix1=BTM_BlockOffsetRcix(cix,  1, 0, 0);
//	cix2=BTM_BlockOffsetRcix(cix, -1, 0, 0);
	cix1=BTM_BlockOffsetRcixPx1(cix);
	cix2=BTM_BlockOffsetRcixNx1(cix);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix1, to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix2, to);

//	cix1=BTM_BlockOffsetRcix(cix, 0,  1, 0);
//	cix2=BTM_BlockOffsetRcix(cix, 0, -1, 0);
	cix1=BTM_BlockOffsetRcixPy1(cix);
	cix2=BTM_BlockOffsetRcixNy1(cix);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix1, to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix2, to);

//	cix1=BTM_BlockOffsetRcix(cix, 0, 0,  1);
//	cix2=BTM_BlockOffsetRcix(cix, 0, 0, -1);
	cix1=BTM_BlockOffsetRcixPz1(cix);
	cix2=BTM_BlockOffsetRcixNz1(cix);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix1, to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix2, to);
#endif

	return(i);
}

BTM_Region * BTM_RegionForPos(BTM_World *wrl, u64 spos)
{
	BTM_Region *rgn;
	int rix;

	rix=BTM_WorldCorgToRix(spos);
	rgn=BTM_GetRegionForRix(wrl, rix);
	return(rgn);
}

u64 BTM_ScaleRayVectorEigth(u64 step)
{
	u64 step1;
	int x, y, z;

	x=(step>> 0)&0x00FFFFFF;
	y=(step>>24)&0x00FFFFFF;
	z=(step>>48)&0x0000FFFF;
	
	x=((s32)(x<<8))>>8;
	y=((s32)(y<<8))>>8;
	z=((s32)(z<<16))>>16;
//	x>>=2;	y>>=2;	z>>=2;
	x>>=3;	y>>=3;	z>>=3;
	
	step1=
		((x&0x00FFFFFFULL)    ) |
		((y&0x00FFFFFFULL)<<24) |
		((z&0x0000FFFFULL)<<48) ;
	return(step1);
}

u64 BTM_ScaleRayVectorFourth(u64 step)
{
	u64 step1;
	int x, y, z, x1, y1, z1;

	x=(step>> 0)&0x00FFFFFF;
	y=(step>>24)&0x00FFFFFF;
	z=(step>>48)&0x0000FFFF;
	
	x=((s32)(x<<8))>>8;
	y=((s32)(y<<8))>>8;
	z=((s32)(z<<16))>>16;
	x1=x>>2;	y1=y>>2;	z1=z>>2;
	
//	x1+=(x1==0)&(x!=0);
//	y1+=(y1==0)&(y!=0);
//	z1+=(z1==0)&(z!=0);
//	x>>=3;	y>>=3;	z>>=3;
	
	x1+=(x1>>31)*2+1;
	y1+=(y1>>31)*2+1;
	z1+=(z1>>31)*2+1;
	
	step1=
		((x1&0x00FFFFFFULL)    ) |
		((y1&0x00FFFFFFULL)<<24) |
		((z1&0x0000FFFFULL)<<48) ;
	return(step1);
}

int BTM_RaycastLineSingle(BTM_World *wrl, int max,
	u64 spos, u64 step, int flag)
{
	BTM_Region *rgn;
	register u64 *voxbm, *voxbmcs;
	register u16 *voxbmix;
	register u64 cpos, cstep;
	u64 blk, clpos, rcix, rcix2, rlcix;
	u32 blk_v, blk_d;
	int cx, cy, cz, cix, cix2, rx, ry, rix, rgix, rlix;
	int adx, ady, adz, isqs, isair;
	int cxm, czm, xsh, zsh;
	int n, n1;
	int i, j, k, h;

//	rix=BTM_WorldCorgToRix(spos);
	rcix=BTM_WorldCorgToRcix(spos);
	rix=BTM_Rcix2Rix(rcix);
	cix=BTM_Rcix2Cix(rcix);

	rgn=BTM_GetRegionForRix(wrl, rix);
	BTM_GenerateBaseRegion(wrl, rgn);
	rgix=rix;
	
	if(!(flag&4))
	{
		rgn->dirty|=2;	/* Player has been here. */
		
		if(flag&1)
		{
			wrl->scr_laspos=spos;
		}
	}
	
	voxbm=rgn->voxbm;
	voxbmix=rgn->voxbmix;

	n=max;
	cpos=spos;
	cstep=step;
	clpos=cpos;
	isair=1;

	voxbmcs=voxbm+(voxbmix[cix>>12]<<6);
	blk=voxbmcs[(cix>>6)&63];
	if((blk>>(cix&63))&1)
	{
#ifdef BTM_TARGET_SMALL
		/* Small targets, treat as opaque. */
		return(0);
#endif

		blk_v=BTM_GetRegionBlockCix(wrl, rgn, cix);
		i=blk_v&255;
		
		if(!BTM_BlockIsTransparentP(wrl, blk_v))
		{
			wrl->scr_lhit=rcix;
			wrl->scr_lahit=0;
			i=BTM_RaycastTryAddHitCix(wrl, rcix);
			return(i);
		}

		isair=0;
//		n1=16;
		n1=max>>2;
//		n1=max>>3;
		while((n1--)>0)
		{
			rcix=BTM_WorldCorgToRcix(cpos);
			rix=BTM_Rcix2Rix(rcix);
			cix=BTM_Rcix2Cix(rcix);

			if(rix!=rgix)
			{
				rgn=BTM_GetRegionForRix(wrl, rix);
				BTM_GenerateBaseRegion(wrl, rgn);
				rgix=rix;
				voxbm=rgn->voxbm;
				voxbmix=rgn->voxbmix;
			}
			
			voxbmcs=voxbm+(voxbmix[cix>>12]<<6);
			blk=voxbmcs[(cix>>6)&63];
			
			if(!(blk>>(cix&63))&1)
			{
				/* We have hit open air. */
				break;
			}

			blk_v=BTM_GetRegionBlockCix(wrl, rgn, cix);
			if(!BTM_BlockIsTransparentP(wrl, blk_v))
			{
				/* We have hit something opaque. */
				break;
			}
			
			BTM_RaycastTryAddHitCix(wrl, rcix);

			rlix=rix;
			rlcix=rcix;
			clpos=cpos;
			cpos+=cstep;
		}
		
		if(n1<=0)
		{
			wrl->scr_lhit=0;
			wrl->scr_lahit=0;
//			wrl->scr_laepos=0;
			if(flag&1)
				wrl->scr_laepos=clpos;
			return(0);
		}

		n=n1<<2;
//		n=n1<<3;
//		n-=(16-n1)<<2;
	}

	isqs=1;
	if(flag&1)
	{
		isqs=2;
//		isqs=3;
	}

//	while((n--)>0)
	while(n>0)
	{
//		rcix=BTM_WorldCorgToRcix(cpos);
//		rix=BTM_Rcix2Rix(rcix);
//		cix=BTM_Rcix2Cix(rcix);

		while((n--)>0)
		{
			rcix=BTM_WorldCorgToRcix(cpos);
			rix=BTM_Rcix2Rix(rcix);
			cix=BTM_Rcix2Cix(rcix);
			i=voxbmix[cix>>12];
			j=(cix>>6)&63;
			voxbmcs=voxbm+(i<<6);
			k=(cix&63);
			blk=voxbmcs[j];

			if(rix!=rgix)
				break;
			if((blk>>k)&1)
				break;

			clpos=cpos;
			cpos+=cstep;
		}

		if(rix!=rgix)
		{
			rgn=BTM_GetRegionForRix(wrl, rix);
			BTM_GenerateBaseRegion(wrl, rgn);
			rgix=rix;
			voxbm=rgn->voxbm;
			voxbmix=rgn->voxbmix;
		}
		
		voxbmcs=voxbm+(voxbmix[cix>>12]<<6);
		blk=voxbmcs[(cix>>6)&63];
		if((blk>>(cix&63))&1)
		{
//			break;

			if(!isqs)
				break;

			blk_v=BTM_GetRegionBlockCix(wrl, rgn, cix);
			i=blk_v&255;
			blk_d=btmgl_vox_atlas_side[i];
			if(blk_d&(BTM_BLKDFL_SEETHRU|BTM_BLKDFL_FLUID))
				break;

			n+=16; isqs--;
//			cstep=BTM_ScaleRayVectorEigth(cstep);
			cstep=BTM_ScaleRayVectorFourth(cstep);
			cpos=clpos;
			continue;
		}
		
//		blk=voxbm[cix>>6];
//		if((blk>>(cix&63))&1)
//			break;
			
#if 0
		cix2=cix-(1<<14);
		blk=voxbm[cix2>>6];
		if((blk>>(cix2&63))&1)
		{
			rcix2=rcix-(1<<14);
			BTM_RaycastTryAddHitCix(wrl, rcix2);
		}
#endif

//		rlix=rix;
//		rlcix=rcix;
		clpos=cpos;
		cpos+=cstep;
	}

	if(n<=0)
	{
		wrl->scr_lhit=0;
		wrl->scr_lahit=0;
//		wrl->scr_laepos=0;
		if(flag&1)
			wrl->scr_laepos=clpos;
		return(0);
	}
	
//	blk_v=rgn->vox[cix];
	blk_v=BTM_GetRegionBlockCix(wrl, rgn, cix);
	i=blk_v&255;
	blk_d=btmgl_vox_atlas_side[i];
	if(i<4)
		return(0);

#ifndef BTM_TARGET_SMALL
//	if(blk_d&BTM_BLKDFL_SEETHRU)
//	if(blk_d&(BTM_BLKDFL_SEETHRU|BTM_BLKDFL_FLUID))
//	if((blk_d&(BTM_BLKDFL_SEETHRU|BTM_BLKDFL_FLUID)) && (n>48))
	if((blk_d&(BTM_BLKDFL_SEETHRU|BTM_BLKDFL_FLUID)) && (n>48) &&
		!(isair && (blk_d&BTM_BLKDFL_FLUID)))
//	if((blk_d&(BTM_BLKDFL_SEETHRU|BTM_BLKDFL_FLUID)) && (n>64))
	{
		i=BTM_RaycastTryAddHitCix(wrl, rcix);
		i+=BTM_RaycastLineSingle(wrl, n-1, cpos+step, step, flag|4);
		return(i);
	}
#endif

	rcix2=BTM_WorldCorgToRcix(clpos);
	
	wrl->scr_lhit=rcix;
	wrl->scr_lahit=rcix2;

	if(flag&1)
	{
		wrl->scr_laepos=clpos;
	}

	i=BTM_RaycastTryAddHitCix(wrl, rcix);
	return(i);
}

#if 0
int BTM_RaycastLineQuad(BTM_World *wrl, int max, int frmax,
	u64 spos0, u64 spos1, u64 spos2, u64 spos3,
	u64 step0, u64 step1, u64 step2, u64 step3)
{
	u64 *voxbm;
	u64 cpos0, cpos1, cpos2, cpos3;
	u64 cpos4, cpos5, cpos6, cpos7, cpos8;
	u64 step4, step5, step6, step7, step8;
	u64 blk;
	int cx0, cy0, cz0, cix0;
	int cx1, cy1, cz1, cix1;
	int adx, ady, adz, split;
	int cxm, czm, xsh, zsh;
	int i, j, k, n, h;
	
	if(frmax<=0)
	{
		i=BTM_RaycastLineSingle(wrl, max, spos0, step0);
		i+=BTM_RaycastLineSingle(wrl, max, spos1, step1);
		i+=BTM_RaycastLineSingle(wrl, max, spos2, step2);
		i+=BTM_RaycastLineSingle(wrl, max, spos3, step3);
		return(i);
	}
	
//	xsh=wrl->xsh;
//	zsh=wrl->zsh;
//	cxm=(1<<xsh)-1;
//	czm=(1<<zsh)-1;
//	voxbm=wrl->voxbm;
	
	n=max;
	cpos0=spos0;	cpos1=spos1;
	cpos2=spos2;	cpos3=spos3;
	while((n--)>0)
	{
		split=0;
//		if(n>=48)
		if(frmax>0)
		{
			adx=(cpos0>> 0)-(cpos1>> 0);
			adx=((int)(adx<<8))>>8;
			ady=(cpos0>>24)-(cpos1>>24);
			ady=((int)(ady<<8))>>8;
			adz=(cpos0>>48)-(cpos1>>48);
			adz=(short)adz;

			if(adx<0)adx=-adx;
			if(ady<0)ady=-ady;
			if(adz<0)adz=-adz;

//			split = (adx>256) | (ady>256) | (adz>256);
//			split = (adx>128) | (ady>128) | (adz>128);
			split = (adx+ady+adz)>128;
			if(split)
				break;
		}

		cx0=(cpos0>> 8)&cxm;
		cx1=(cpos1>> 8)&cxm;
		cy0=(cpos0>>32)&cxm;
		cy1=(cpos1>>32)&cxm;
		cz0=(cpos0>>56)&czm;
		cz1=(cpos1>>56)&czm;
		cix0=(((cz0<<xsh)|cy0)<<xsh)|cx0;
		cix1=(((cz1<<xsh)|cy1)<<xsh)|cx1;

//		adx=cx1-cx0;
//		ady=cy1-cy0;
//		adz=cz1-cz0;

//		if(cix0!=cix1)
		
		blk=voxbm[cix0>>6];
		if(blk&(1ULL<<(cix0&63)))
			break;

		cpos0+=step0;	cpos1+=step1;
		cpos2+=step2;	cpos3+=step3;
	}

	if(n<=0)
		return(0);

//	if(cix0!=cix1)
//	if((cix0!=cix1) && (n>=32))
//	if(split && (n>=32))
	if(split)
	{
//		cpos4=(cpos0+cpos1)>>1;
//		cpos5=(cpos2+cpos3)>>1;
//		cpos6=(cpos0+cpos2)>>1;
//		cpos7=(cpos1+cpos3)>>1;
//		cpos8=(cpos4+cpos5)>>1;

		cpos4=btm_raycast_avgvec(cpos0, cpos1);
		cpos5=btm_raycast_avgvec(cpos2, cpos3);
		cpos6=btm_raycast_avgvec(cpos0, cpos2);
		cpos7=btm_raycast_avgvec(cpos1, cpos3);
		cpos8=btm_raycast_avgvec(cpos4, cpos5);

//		step4=(step0+step1)>>1;
//		step5=(step2+step3)>>1;
//		step6=(step0+step2)>>1;
//		step7=(step1+step3)>>1;
//		step8=(step4+step5)>>1;

		step4=btm_raycast_avgvec(step0, step1);
		step5=btm_raycast_avgvec(step2, step3);
		step6=btm_raycast_avgvec(step0, step2);
		step7=btm_raycast_avgvec(step1, step3);
		step8=btm_raycast_avgvec(step4, step5);
		
		i=BTM_RaycastLineQuad(wrl, n, frmax-1,
			cpos0, cpos4, cpos6, cpos8,
			step0, step4, step6, step8);
		i+=BTM_RaycastLineQuad(wrl, n, frmax-1,
			cpos4, cpos1, cpos8, cpos7,
			step4, step1, step8, step7);

		i+=BTM_RaycastLineQuad(wrl, n, frmax-1,
			cpos6, cpos8, cpos2, cpos5,
			step6, step8, step2, step5);
		i+=BTM_RaycastLineQuad(wrl, n, frmax-1,
			cpos8, cpos7, cpos5, cpos3,
			step8, step7, step5, step3);

		return(i);
	}
	
	i=BTM_RaycastTryAddHitCix(wrl, cix0);
	return(i);
}
#endif

int BTM_WorldCameraDistCix(BTM_World *wrl, u64 rcix)
{
	u64 bpos;
	int rix, cix;
	int cx, cy, cz;
	int vx, vy, vq;
	int dx, dy, d;

	bpos=BTM_ConvRcixToBlkPos(rcix);
	cx=(bpos>>0)&65535;
	cy=(bpos>>16)&65535;
	cz=(bpos>>32)&255;

	vx=(wrl->cam_org>> 8)&65535;
	vy=(wrl->cam_org>>32)&65535;
	
	dx=(signed char)(vx-cx);
	dy=(signed char)(vy-cy);

#ifndef BTM_RAYTHREAD
	vq=(wrl->cam_yaw>>5)&7;
	
	switch(vq)
	{
	case 7: case 0:
		if(dy<0)
			dy<<=3;
		break;
	case 1: case 2:
		if(dx>0)
			dx<<=3;
		break;
	case 3: case 4:
		if(dy>0)
			dy<<=3;
		break;
	case 5: case 6:
		if(dx<0)
			dx<<=3;
		break;
	}
#endif
	
	dx^=dx>>31;
	dy^=dy>>31;
	
	if(dx>dy)
		d=dx+(dy>>1);
	else
		d=dy+(dx>>1);
	
	return(d);
}

int BTM_RaycastSceneQuad(BTM_World *wrl)
{
	static int ndelay=0;
//	short	*
	byte	jitter[16]={
		0x00, 0xF8, 0x04, 0xFC,
		0x02, 0xFA, 0x06, 0xFE,
		0x01, 0xF9, 0x05, 0xFD,
		0x03, 0xFB, 0x07, 0xFF};
	u64		corg, corg1, step_fw;
	u64		step0, step1, step2, step3;
	u64		rcix;
	u32		blk;
	int		x, y, z, yaw, pitch, yji, pji;
	int		y0_ang, p0_ang;
	int		y1_ang, p1_ang;
	int		i, j, k, l, h, n;

	if(ndelay)
	{
		ndelay--;
		return(0);
	}
	ndelay=4;

	BTM_RaycastInitTables();

	BTMGL_LockWorld();

	BTM_CheckWorldMagic(wrl);

	for(i=0; i<BTM_RAYCAST_HASHSZ; i++)
	{
		wrl->scr_pts_hash[i]=-1;
		wrl->scr_cxpred[i]=-1;
	}
	n=wrl->scr_npts;
	wrl->scr_npts=0;
	
	for(i=0; i<1024; i++)
	{
		btm_drawstat_dist[i]=0;
		btm_drawstat_dist_hrsc[i]=0;
	}
	
	for(i=0; i<n; i++)
	{
		j=wrl->scr_pts_rcnt[i];
		if(j<=0)
			continue;
		rcix=wrl->scr_pts_list[i];
		
		l=BTM_WorldCameraDistCix(wrl, rcix);
//		if(l>36)
//		if(l>96)
//		if(l>128)
		if(l>(2*btm_drawdist))
			continue;

		if(l>btm_drawdist)
		{
			j-=3;
			if(j<0)
				continue;
		}
		
		btm_drawstat_dist[l]++;

		blk=BTM_GetWorldBlockCix(wrl, rcix);
		btm_drawstat_dist_hrsc[l]+=btmgl_vox_hrsc[blk&255];
		
		BTM_RaycastTryAddHitCix1(wrl, rcix, j-1);
	}
	
	wrl->frame++;

	corg=wrl->cam_org;
	yaw=wrl->cam_yaw;
	pitch=wrl->cam_pitch;

//	pitch=0;
	if(pitch>128)
		pitch=128;
	if(pitch<(-128))
		pitch=-128;

//	for(y=0; y<48; y++)
//		for(x=0; x<64; x++)

	step_fw=BTM_RaycastStepVector(yaw, pitch);
	
//	yji=(wrl->frame&7)-3;

//	pji=((wrl->frame>>0)&7)-3;
//	yji=((wrl->frame>>3)&7)-3;
//	yji=((wrl->frame>>3)&15)-7;

//	pji=((wrl->frame>>0)&15)-7;
//	pji=((wrl->frame>>3)&15)-7;
//	yji=((wrl->frame>>1)&15)-7;

	yji=(signed char)(jitter[(wrl->frame>>1)&15]);
	pji=(signed char)(jitter[(wrl->frame>>3)&15]);

#ifdef BTM_TARGET_SMALL
	yji<<=2;
	pji<<=2;
#endif

// #ifdef BTM_RAYTHREAD
#if 1

	corg1=corg;
	corg1+=((u64)(btmra_sinang[btmgl_time_ms&255]>>23))<<48;

	k=wrl->frame&1;
#ifdef BTM_TARGET_SMALL
	for(y=0; y<17; y++)
		for(x=0; x<32; x++)
#else
	for(y=0; y<68; y++)
//		for(x=0; x<256; x++)
		for(x=0; x<128; x++)
#endif
	{
		if(y<4)
		{
			if((y==0) && (x!=0))
				continue;
			if(x&7)
				continue;
		}

		if(y<8)
		{
			if(x&3)
				continue;
		}
	
#ifdef BTM_TARGET_SMALL
		y0_ang=x*32+yji;
		p0_ang=256-(y*16)-pji;
		if(k)p0_ang=-p0_ang;
#else
//		y0_ang=x*4+yji;
		y0_ang=x*8+yji;

		p0_ang=256-(y*4)-pji;
		if(k)p0_ang=-p0_ang;
#endif

		step0=BTM_RaycastStepVectorB(y0_ang, p0_ang);
		BTM_RaycastLineSingle(wrl, btm_drawdist, corg1, step0, 0);
	}

#else

#if 1
//	for(y=0; y<80; y++)
//		for(x=0; x<112; x++)
//	for(y=0; y<64; y++)
//		for(x=0; x<96; x++)
#ifndef BTM_TARGET_SMALL
	for(y=0; y<64; y++)
		for(x=0; x<80; x++)
#endif
//	for(y=0; y<40; y++)
//		for(x=0; x<56; x++)
//	for(y=0; y<36; y++)
//		for(x=0; x<48; x++)
//	for(y=0; y<32; y++)
//		for(x=0; x<42; x++)

#ifdef BTM_TARGET_SMALL
	for(y=0; y<28; y++)
		for(x=0; x<36; x++)
#endif
	{
//		y0_ang=(yaw+((x-56)*1))&255;
//		y0_ang=(y0_ang+yji)&255;

//		p0_ang=(pitch+((y-40)*1))&255;
//		p0_ang=(p0_ang+yji)&255;

#if 0
		y0_ang=yaw*4+((x-56)*3);
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-40)*3);
		p0_ang=p0_ang+yji;
#endif

#if 0
		y0_ang=yaw*4+((x-48)*3);
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-32)*4);
		p0_ang=p0_ang+yji;
#endif

#if 1
		y0_ang=yaw*4+((x-40)*4);
		y0_ang=y0_ang+yji;

//		p0_ang=pitch*4+((y-32)*4);
		p0_ang=pitch*4+((y-32)*3);
		p0_ang=p0_ang+yji;
#endif

#if 0
		y0_ang=yaw*4+((x-28)*5);
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-20)*5);
		p0_ang=p0_ang+yji;
#endif

#if 0
		y0_ang=yaw*4+((x-24)*6);
//		y0_ang=yaw*4+((x-24)*5);
		y0_ang=y0_ang+yji;

//		p0_ang=pitch*4+((y-18)*6);
		p0_ang=pitch*4+((y-18)*4);
		p0_ang=p0_ang+yji;
#endif

//#if 0
#ifdef BTM_TARGET_SMALL
		y0_ang=yaw*4+((x-21)*7);
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-16)*7);
		p0_ang=p0_ang+yji;
#endif

#if 0
		y0_ang=yaw*4+((x-18)*8);
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-14)*8);
		p0_ang=p0_ang+yji;
#endif

		if(p0_ang>256)
//		if(p0_ang>1024)
			continue;
		if(p0_ang<(-256))
//		if(p0_ang<(-1024))
			continue;

//		step0=BTM_RaycastStepVector(y0_ang, p0_ang);
		step0=BTM_RaycastStepVectorB(y0_ang, p0_ang);
		
//		corg1=corg+(step_fw-step0);
//		blk=BTM_GetWorldBlockCorg(wrl, corg1);
//		if((blk&255)>=2)
//			corg1=corg;
		
		corg1=corg;
		corg1+=((u64)(btmra_sinang[btmgl_time_ms&255]>>23))<<48;

//		BTM_RaycastLineSingle(wrl, 112, corg1, step0);
//		BTM_RaycastLineSingle(wrl, 80, corg1, step0, 0);
		BTM_RaycastLineSingle(wrl, btm_drawdist, corg1, step0, 0);
//		BTM_RaycastLineSingle(wrl, 32, corg1, step0);
//		BTM_RaycastLineSingle(wrl, 28, corg1, step0);
//		BTM_RaycastLineSingle(wrl, 24, corg1, step0);
	}
#endif

	corg1=corg;
	corg1+=((u64)(btmra_sinang[btmgl_time_ms&255]>>23))<<48;

	k=wrl->frame&1;
//	for(y=0; y<16; y++)
	for(y=0; y<12; y++)
//		for(x=0; x<32; x++)
		for(x=0; x<8; x++)
	{
		if(y<4)
		{
			if((y==0) && (x!=0))
				continue;
			if(x&7)
				continue;
		}

//		if(y<8)
//		{
//			if(x&3)
//				continue;
//		}
	
		y0_ang=x*32;
//		p0_ang=-256+(y*12);

		p0_ang=256-(y*12);
		if(k)p0_ang=-p0_ang;

		step0=BTM_RaycastStepVectorB(y0_ang, p0_ang);
//		BTM_RaycastLineSingle(wrl, 32, corg1, step0, 0);
		BTM_RaycastLineSingle(wrl, btm_drawdist/2, corg1, step0, 0);

//		p0_ang=256-(y*12);

//		step0=BTM_RaycastStepVectorB(y0_ang, p0_ang);
//		BTM_RaycastLineSingle(wrl, 64, corg1, step0, 0);
	}

#endif

	/* Raycast directly forwards. */
	step0=BTM_RaycastStepVectorB(yaw*4, pitch*4);

	BTM_RaycastLineSingle(wrl, 24,
//	BTM_RaycastLineSingle(wrl, 96,
		corg, step0, 1);

	BTM_CheckWorldMagic(wrl);

	BTMGL_UnlockWorld();
	
	return(0);
}
