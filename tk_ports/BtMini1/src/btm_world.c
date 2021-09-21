/*
Voxel:
  (31:24): Entity Spawn
  (23:22): Light, Sky
  (21:16): Occlusion Bits
  (15:12): Light Level (Block)
  (11: 8): Attrib / Ext
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
		iy&=0x00FFFFFF;

		ix&=0x0000FFFF;
		v=(((u64)ix)<<48)|(((u64)iy)<<24)|(iy);
		btm_step_pdir[i]=v;
	}
	
	BTMRA_RastInitTables();
	
	return(1);
}

u64 BTM_RaycastStepVectorB(int yang, int pang)
{
	float sy, cy, sp, cp, la, lb;
	float fx, fy, fz, fd;
	int vx, vy, vz, sx;
	u64 ystep, pstep, step;

#if 0
	la=yang*((2*M_PI)/1024);
	lb=pang*((2*M_PI)/1024);
	sy=sin(la);	cy=-cos(la);
	sp=sin(lb);	cp=cos(lb);

//	fx=sy;
//	fy=cy;

	fx=sy*cp;
	fy=cy*cp;
	fz=sp;

#if 0
	fd=1.0/sqrt(fx*fx+fy*fy+fz*fz);
//	fd=1.0/((fabs(fx)+fabs(fy)+fabs(fz))/3.0);
	fx*=fd;
	fy*=fd;
	fz*=fd;
#endif

	vx=fx*256;
	vy=fy*256;
	vz=fz*256;
	
//	vx=sy*cp*256;
//	vy=cy*cp*256;
//	vz=sp*256;

	vx&=0x00FFFFFF;
	vy&=0x00FFFFFF;

	step=(((u64)vz)<<48)|(((u64)vy)<<24)|(vx);
	return(step);
#endif

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

u64 BTM_RaycastStepVector(int yang, int pang)
{
	return(BTM_RaycastStepVectorB(yang*4, pang*4));
}

/*
Raycast the map, stopping at the first non-air block.
 */
int BTM_RaycastLine(BTM_World *wrl, u64 spos, u64 step, int max)
{
	u32 *vox;
	u64 cpos;
	u32 blk;
	int cx, cy, cz, cix;
	int cxm, czm, xsh, zsh;
	int n;
	
	xsh=wrl->xsh;
	zsh=wrl->zsh;
	cxm=(1<<xsh)-1;
	czm=(1<<zsh)-1;
	vox=wrl->vox;
	
	n=max; cpos=spos;
	while((n--)>0)
	{
		cx=(cpos>> 8)&cxm;
		cy=(cpos>>32)&cxm;
		cz=(cpos>>56)&czm;
		cix=(((cz<<xsh)|cy)<<xsh)|cx;

		blk=vox[cix];
		if(blk&255)
			break;

		cpos+=step;
	}
	
	if(n<=0)
		return(-1);

	cix=(((cz<<12)|cy)<<12)|cx;
	return(cix);
}

int BTM_RaycastScene(BTM_World *wrl)
{
//	short	*
	u64		step;
	int		x, y, yaw, pitch, y_ang, p_ang, hit;
	int		i, j, k, h, n;

	BTM_RaycastInitTables();

	yaw=wrl->cam_yaw;
	pitch=wrl->cam_pitch;
	for(y=0; y<24; y++)
		for(x=0; x<32; x++)
	{
		y_ang=(yaw+((x-16)*2))&255;
		p_ang=(pitch+((y-12)*2))&255;
		step=btm_step_ydir[y_ang];
		step+=btm_step_pdir[p_ang];
		
		hit=BTM_RaycastLine(wrl, wrl->cam_org, step, 128);
		wrl->scr_pts_hit[y*32+x]=hit;
	}

	for(i=0; i<64; i++)
		wrl->scr_pts_hash[i]=-1;
	wrl->scr_npts=0;
	
	for(i=0; i<(32*24); i++)
	{
		hit=wrl->scr_pts_hit[i];
		h=((hit*(65521*251))>>24)&63;
		j=wrl->scr_pts_hash[h];
		while(j>=0)
		{
			k=wrl->scr_pts_list[j];
			if(hit==k)
				break;
			j=wrl->scr_pts_chn[j];
		}
		if(j>=0)
			continue;
		j=wrl->scr_npts++;
		wrl->scr_pts_list[j]=hit;
		wrl->scr_pts_chn[j]=wrl->scr_pts_hash[h];
		wrl->scr_pts_hash[h]=j;
	}
	
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

	return(1);
}

int BTM_UpdateWorldBlockOccCix(BTM_World *wrl, int cix)
{
	int xmsk, ymsk;
	int cix1, xsh2, bt;
	int xsh, zsh, msk;
	u64 bm;
	u32 blk, blkd, blk2, blk2d;

	xsh=wrl->xsh;
	zsh=wrl->zsh;
	msk=(1<<(xsh+xsh+zsh))-1;

	cix&=msk;
	
	xmsk=(1<<xsh)-1;
	ymsk=xmsk<<xsh;

	xsh2=xsh<<1;
	blk=wrl->vox[cix];

	bt=(blk&0xFF);
	if(bt<2)
	{
//		wrl->vox[cix]=blk;
		return(0);
	}

	blkd=btmgl_vox_atlas_side[bt];

	if(blkd&BTM_BLKDFL_NODRAW)
	{
		blk|=(0x3FU<<24);
		wrl->vox[cix]=blk;
		return(0);
	}

	
	blk&=~(0x3FU<<24);

//	cix1=(cix-1)&msk;
	cix1=((cix-1)&xmsk)|(cix&(~xmsk));
	bm=wrl->voxbm[cix1>>6];
	if(bm&(1ULL<<(cix1&63)))
	{
		blk2=wrl->vox[cix1];
		blk2d=btmgl_vox_atlas_side[blk2&255];
		if(BTM_UpdateWorldBlockOccBlkCheck(wrl, blkd, blk2d))
			blk|=1U<<(24+3);
	}

//	cix1=(cix+1)&msk;
	cix1=((cix+1)&xmsk)|(cix&(~xmsk));
	bm=wrl->voxbm[cix1>>6];
	if(bm&(1ULL<<(cix1&63)))
	{
		blk2=wrl->vox[cix1];
		blk2d=btmgl_vox_atlas_side[blk2&255];
		if(BTM_UpdateWorldBlockOccBlkCheck(wrl, blkd, blk2d))
			blk|=1U<<(24+1);
	}
	
//	cix1=(cix-(1<<xsh))&msk;
	cix1=((cix-(1<<xsh))&ymsk)|(cix&(~ymsk));
	bm=wrl->voxbm[cix1>>6];
	if(bm&(1ULL<<(cix1&63)))
	{
		blk2=wrl->vox[cix1];
		blk2d=btmgl_vox_atlas_side[blk2&255];
		if(BTM_UpdateWorldBlockOccBlkCheck(wrl, blkd, blk2d))
			blk|=1U<<(24+0);
	}

//	cix1=(cix+(1<<xsh))&msk;
	cix1=((cix+(1<<xsh))&ymsk)|(cix&(~ymsk));
	bm=wrl->voxbm[cix1>>6];
	if(bm&(1ULL<<(cix1&63)))
	{
		blk2=wrl->vox[cix1];
		blk2d=btmgl_vox_atlas_side[blk2&255];
		if(BTM_UpdateWorldBlockOccBlkCheck(wrl, blkd, blk2d))
			blk|=1U<<(24+2);
	}

	cix1=(cix-(1<<xsh2))&msk;
	bm=wrl->voxbm[cix1>>6];
	if(bm&(1ULL<<(cix1&63)))
	{
		blk2=wrl->vox[cix1];
		blk2d=btmgl_vox_atlas_side[blk2&255];
		if(BTM_UpdateWorldBlockOccBlkCheck(wrl, blkd, blk2d))
			blk|=1U<<(24+5);
	}

	cix1=(cix+(1<<xsh2))&msk;
	bm=wrl->voxbm[cix1>>6];
	if(bm&(1ULL<<(cix1&63)))
	{
		blk2=wrl->vox[cix1];
		blk2d=btmgl_vox_atlas_side[blk2&255];
		if(BTM_UpdateWorldBlockOccBlkCheck(wrl, blkd, blk2d))
			blk|=1U<<(24+4);
	}

	wrl->vox[cix]=blk;
	
	return(0);
}

int BTM_SetWorldBlockCix(BTM_World *wrl, int cix, u32 blk)
{
	u64 bm;

	wrl->vox[cix]=blk;

	bm=wrl->voxbm[cix>>6];
	if((blk&255)>=2)
		bm|=1ULL<<(cix&63);
	else
		bm&=~(1ULL<<(cix&63));

	wrl->voxbm[cix>>6]=bm;
	return(0);
}

int BTM_SetWorldBlockXYZ(BTM_World *wrl,
	int cx, int cy, int cz, u32 blk)
{
	int ix, xsh, xsh2, msk;

	msk=(1<<wrl->xsh)-1;
	cx&=msk;
	cy&=msk;
//	msk=(1<<wrl->zsh)-1;
//	cz&=msk;

	if(cz<0)
		return(0);
	if(cz>=(1<<wrl->zsh))
		return(0);

	msk=(1<<(wrl->xsh+wrl->xsh+wrl->zsh))-1;

	xsh=wrl->xsh;
	xsh2=xsh<<1;
	ix=(((cz<<xsh)|cy)<<xsh)|cx;
	
	BTM_SetWorldBlockCix(wrl, ix&msk, blk);
//	BTM_UpdateWorldBlockOccCix(wrl, ix, xsh, msk);
//	BTM_UpdateWorldBlockOccCix(wrl, ix+1, xsh, msk);
//	BTM_UpdateWorldBlockOccCix(wrl, ix-1, xsh, msk);
//	BTM_UpdateWorldBlockOccCix(wrl, ix+(1<<xsh), xsh, msk);
//	BTM_UpdateWorldBlockOccCix(wrl, ix-(1<<xsh), xsh, msk);
//	BTM_UpdateWorldBlockOccCix(wrl, ix+(1<<xsh2), xsh, msk);
//	BTM_UpdateWorldBlockOccCix(wrl, ix-(1<<xsh2), xsh, msk);
	return(0);
}

int BTM_UpdateWorldBlockOccCix2(BTM_World *wrl, int cix)
{
	int xsh, xsh2;
	
	xsh=wrl->xsh;
	xsh2=xsh<<1;

	BTM_UpdateWorldBlockOccCix(wrl, cix);
	BTM_UpdateWorldBlockOccCix(wrl, cix+1);
	BTM_UpdateWorldBlockOccCix(wrl, cix-1);
	BTM_UpdateWorldBlockOccCix(wrl, cix+(1<<xsh));
	BTM_UpdateWorldBlockOccCix(wrl, cix-(1<<xsh));
	BTM_UpdateWorldBlockOccCix(wrl, cix+(1<<xsh2));
	BTM_UpdateWorldBlockOccCix(wrl, cix-(1<<xsh2));
	return(0);
}

u32 BTM_GetWorldBlockCix(BTM_World *wrl, int cix)
{
	u32 blk;

	blk=wrl->vox[cix];
	return(blk);
}

u32 BTM_GetWorldBlockXYZ(BTM_World *wrl,
	int cx, int cy, int cz)
{
	int ix, xsh, xsh2, msk;

	msk=(1<<wrl->xsh)-1;
	cx&=msk;
	cy&=msk;

	if(cz<0)
		return(0);
	if(cz>=(1<<wrl->zsh))
		return(0);

	msk=(1<<(wrl->xsh+wrl->xsh+wrl->zsh))-1;

	xsh=wrl->xsh;
	ix=(((cz<<xsh)|cy)<<xsh)|cx;
	return(wrl->vox[ix&msk]);
}

u32 BTM_GetWorldBlockCorg(BTM_World *wrl, u64 corg)
{
	int cx, cy, cz;
	
	cx=corg>>8;
	cy=corg>>32;
	cz=corg>>56;
	return(BTM_GetWorldBlockXYZ(wrl, cx, cy, cz));
}

int BTM_UpdateWorldBlockOcc(BTM_World *wrl)
{
	int xsh, zsh, msk;
	int i;

	xsh=wrl->xsh;
	zsh=wrl->zsh;
	msk=(1<<(xsh+xsh+zsh))-1;
	for(i=0; i<(msk+1); i++)
		BTM_UpdateWorldBlockOccCix(wrl, i);
	return(0);
}

BTM_World *BTM_AllocWorld(int xsh, int zsh)
{
	BTM_World *tmp;
	int x, y, z, ix, xs, zs;
	int i, j;
	
	tmp=malloc(sizeof(BTM_World));
	memset(tmp, 0, sizeof(BTM_World));

	tmp->xsh=xsh;
	tmp->zsh=zsh;
	tmp->vox=malloc((1<<(xsh+xsh+zsh))*sizeof(u32));
	memset(tmp->vox, 0, (1<<(xsh+xsh+zsh))*sizeof(u32));

	tmp->voxbm=malloc((1<<(xsh+xsh+zsh-6))*8);
	memset(tmp->voxbm, 0, (1<<(xsh+xsh+zsh-6))*8);
	
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

#if 0
	for(i=0; i<xs; i++)
		for(z=0; z<zs; z++)
	{
		x=i; y=0;
		ix=(((z<<xsh)|y)<<xsh)|x;
		BTM_SetWorldBlockCix(tmp, ix, 0x22);

		x=i; y=xs-1;
		ix=(((z<<xsh)|y)<<xsh)|x;
		BTM_SetWorldBlockCix(tmp, ix, 0x22);

		y=i; x=0;
		ix=(((z<<xsh)|y)<<xsh)|x;
		BTM_SetWorldBlockCix(tmp, ix, 0x22);

		y=i; x=xs-1;
		ix=(((z<<xsh)|y)<<xsh)|x;
		BTM_SetWorldBlockCix(tmp, ix, 0x22);
	}
#endif
	
	return(tmp);
}

#if 0
int BTM_DrawSceneBlocks(BTM_Screen *scr, BTM_World *wrl)
{
	BTM_TexImg *tex;
	u32 *dbd;
	u64 *dbp;
	int *dbl;
	u32 *vox;

	u64 cpos;
	u32 blk;

	int pos, cix, cx, cy, cz, vx, vy, vz;
	int px, py, pz, pd;
	int cxm, czm, xsh, zsh;
	int ra, sina, cosa;
	int np;
	int i, j, k;
	
	xsh=wrl->xsh;
	zsh=wrl->zsh;
	cxm=(1<<xsh)-1;
	czm=(1<<zsh)-1;
	vox=wrl->vox;
	
	ra=wrl->cam_yaw&255;
	cosa=btmra_cosang[ra];
	sina=btmra_sinang[ra];

	vx=(wrl->cam_org>> 0)&0xFFFFFF;
	vy=(wrl->cam_org>>24)&0xFFFFFF;
	vz=(wrl->cam_org>>48)&0x00FFFF;
	vx=(vx<<8)>>8;
	vy=(vy<<8)>>8;
	vz=(vz<<8)>>8;

	np=wrl->scr_npts;
	dbd=wrl->dblk_data;
	dbp=wrl->dblk_pos;
	dbl=wrl->dblk_dist;
	for(i=0; i<np; i++)
	{
		pos=wrl->scr_pts_hit[i];
		cx=(pos>>0)&4095;
		cy=(pos>>12)&4095;
		cz=(pos>>24)&255;
		cix=(((cz<<xsh)|cy)<<xsh)|cx;
		blk=vox[cix];
//		cpos=	(((u64)cx)<< 8) |
//				(((u64)cy)<<32) |
//				(((u64)cz)<<56) |
//				0x0080000080000080ULL;

		cx=((cx<<8)+0x80)-vx;
		cy=((cy<<8)+0x80)-vy;
		cz=((cz<<8)+0x80)-vz;
		
		px=btmra_mulhi1(cx, cosa)+btmra_mulhi1(cy, sina);
		py=btmra_mulhi1(cy, cosa)-btmra_mulhi1(cx, sina);
		pz=cz;
		
		px&=0x00FFFFFF;
		py&=0x00FFFFFF;
		pz&=0x0000FFFF;
		
		cx=px>>8;
		cy=py>>8;
		cz=pz>>8;
		pd=(cx*cx)+(cy*cy)+(cz*cz);

		cpos=	(((u64)px)<< 8) |
				(((u64)py)<<32) |
				(((u64)pz)<<56) ;

		dbd[i]=blk;
		dbp[i]=cpos;
		dbl[i]=pd;
	}

	for(i=0; i<np; i++)
	{
		blk=dbd[i];
		tex=wrl->texlist[blk&255];
		if(!tex)
			continue;

		BTMRA_DrawBox(scr, tex, dbp[i], wrl->cam_yaw, blk>>24);
	}
	
	return(0);
}
#endif

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

float *btmgl_blkemit_xyz;
float *btmgl_blkemit_st;
u32 *btmgl_blkemit_rgb;
int btmgl_blkemit_npts;
int btmgl_blkemit_mpts;

int BTMGL_EmitBlockVertex(float *xyz, float *st, u32 rgb)
{
	int ix;
	int i, j, k;
	
	if(!btmgl_blkemit_xyz)
	{
		ix=4096;
		btmgl_blkemit_xyz=malloc(ix*4*sizeof(float));
		btmgl_blkemit_st=malloc(ix*2*sizeof(float));
		btmgl_blkemit_rgb=malloc(ix*1*sizeof(u32));
		btmgl_blkemit_mpts=ix;
	}
	
	if((btmgl_blkemit_npts+1)>=btmgl_blkemit_mpts)
	{
		ix=btmgl_blkemit_mpts+(btmgl_blkemit_mpts>>1);
		btmgl_blkemit_xyz=realloc(btmgl_blkemit_xyz, ix*4*sizeof(float));
		btmgl_blkemit_st=realloc(btmgl_blkemit_st, ix*2*sizeof(float));
		btmgl_blkemit_rgb=realloc(btmgl_blkemit_rgb, ix*1*sizeof(u32));
		btmgl_blkemit_mpts=ix;
	}
	
	ix=btmgl_blkemit_npts++;

#if 0
	i=ix*4;
	btmgl_blkemit_xyz[i+0]=xyz[0];
	btmgl_blkemit_xyz[i+1]=xyz[1];
	btmgl_blkemit_xyz[i+2]=xyz[2];

	i=ix*2;
	btmgl_blkemit_st[i+0]=st[0];
	btmgl_blkemit_st[i+1]=st[1];
#endif

	memcpy(btmgl_blkemit_xyz+ix*4, xyz, 3*sizeof(float));
	memcpy(btmgl_blkemit_st+ix*2, st, 2*sizeof(float));

	btmgl_blkemit_rgb[ix]=rgb;
	return(0);
}

int BTMGL_EmitBlockFaces(int cx, int cy, int cz, int fm, u32 blk)
{
	float xyz[8*4];
	float sta[4*4*2];
	const int *tri;
	float *st;
	float mx, my, mz, nx, ny, nz;
	u32 rgb, binf;
	int bt, tx, ty;
	int i, j, k;
	
	if(!(fm&0x3F))
		return(0);
	
	bt=blk&255;
	if(bt<2)
		return;
	if(bt==0x22)
		return;
	
	k=(blk>>24)&0x3F;
	fm&=~k;
	if(!(fm&0x3F))
		return(0);
	
	binf=btmgl_vox_atlas_side[bt];
	
	mx=cx;	nx=cx+1;
	my=cy;	ny=cy+1;
	mz=cz;	nz=cz+1;
	
	for(i=0; i<8; i++)
	{
		j=i*4;
		xyz[j+0]=(i&1)?nx:mx;
		xyz[j+1]=(i&2)?ny:my;
		xyz[j+2]=(i&4)?nz:mz;
	}

//	rgb=0xFFFFFFFFU;

	
	tx=(binf>> 0)&15;	ty=15-((binf>>4)&15);
//	sta[0]=(tx+0)*(1.0/16)+(1.0/256);	sta[1]=(ty+0)*(1.0/16)+(1.0/256);
//	sta[2]=(tx+1)*(1.0/16)-(1.0/256);	sta[3]=(ty+0)*(1.0/16)+(1.0/256);
//	sta[4]=(tx+0)*(1.0/16)+(1.0/256);	sta[5]=(ty+1)*(1.0/16)-(1.0/256);
//	sta[6]=(tx+1)*(1.0/16)-(1.0/256);	sta[7]=(ty+1)*(1.0/16)-(1.0/256);

	sta[0]=(tx+0)*(1.0/16)+(1.0/256);	sta[1]=1.0-(1.0/256)-(ty+0)*(1.0/16);
	sta[2]=(tx+1)*(1.0/16)-(1.0/256);	sta[3]=1.0-(1.0/256)-(ty+0)*(1.0/16);
	sta[4]=(tx+0)*(1.0/16)+(1.0/256);	sta[5]=1.0+(1.0/256)-(ty+1)*(1.0/16);
	sta[6]=(tx+1)*(1.0/16)-(1.0/256);	sta[7]=1.0+(1.0/256)-(ty+1)*(1.0/16);

//	for(i=0; i<4; i++)
//	{
//		sta[i*2+1]=1.0-sta[i*2+1];
//	}

	if(((binf>>8)&0xFF) == (binf&0xFF))
//	if(0)
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
	for(i=0; i<6; i++)
	{
		if(!(fm&1<<i))
			continue;

		if(i==4)
		{
			rgb=0xFFFFFFFFU;
			st=sta+8;
		}else if(i==5)
		{
			rgb=0xFF7F7F7FU;
			st=sta+16;
		}else
		{
			rgb=0xFFBFBFBFU;
			st=sta;
		}

		tri=btmgl_cube_tris+i*6;
		BTMGL_EmitBlockVertex(xyz+tri[0]*4, st+0*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[1]*4, st+1*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[2]*4, st+3*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[3]*4, st+0*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[4]*4, st+3*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[5]*4, st+2*2, rgb);
	}
#endif

#if 1
	for(i=0; i<6; i++)
	{
		if(!(fm&1<<i))
			continue;

		if(i==4)
		{
			rgb=0xFFFFFFFFU;
			st=sta+8;
		}else if(i==5)
		{
			rgb=0xFF7F7F7FU;
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

		tri=btmgl_cube_quads+i*4;
		BTMGL_EmitBlockVertex(xyz+tri[0]*4, st+0*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[1]*4, st+1*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[2]*4, st+3*2, rgb);
		BTMGL_EmitBlockVertex(xyz+tri[3]*4, st+2*2, rgb);
	}
#endif
}

int BTMGL_DrawSceneBlocks(BTM_World *wrl)
{
	u32 *dbd;
	u64 *dbp;
	int *dbl;
	u32 *vox;

	u64 cpos;
	u32 blk;

	int pos, cix, cx, cy, cz, vx, vy, vz;
	int px, py, pz, pd;
	int cxm, czm, xsh, zsh, xzshr;
	int cxfull, cxhalf, cxlqtr, cxhqtr;
	int ra, sina, cosa;
	int np, fm;
	int i, j, k;
	
	xsh=wrl->xsh;
	zsh=wrl->zsh;
	cxm=(1<<xsh)-1;
	czm=(1<<zsh)-1;
	vox=wrl->vox;
	
	xzshr=xsh+xsh;
	
	cxfull=1<<xsh;
	cxhalf=1<<(xsh-1);
	cxlqtr=1<<(xsh-2);
	cxhqtr=cxhalf+cxlqtr;
	
	ra=wrl->cam_yaw&255;
	cosa=btmra_cosang[ra];
	sina=btmra_sinang[ra];

	vx=(wrl->cam_org>> 0)&0xFFFFFF;
	vy=(wrl->cam_org>>24)&0xFFFFFF;
	vz=(wrl->cam_org>>48)&0x00FFFF;
	vx=(vx<< 8)>> 8;
	vy=(vy<< 8)>> 8;
	vz=(vz<<16)>>16;
	
	vx>>=8;
	vy>>=8;
	vz>>=8;

	btmgl_blkemit_npts = 0;

	np=wrl->scr_npts;
	for(i=0; i<np; i++)
	{
		cix=wrl->scr_pts_list[i];
		
		cx=(cix>>0)&cxm;
		cy=(cix>>xsh)&cxm;
		cz=(cix>>xzshr)&czm;

#if 0		
		if((vx>cxhalf) && (cx<cxlqtr))
			cx+=cxfull;
		if((vx<cxhalf) && (cx>cxhqtr))
			cx-=cxfull;
		if((vy>cxhalf) && (cy<cxlqtr))
			cy+=cxfull;
		if((vy<cxhalf) && (cy>cxhqtr))
			cy-=cxfull;
#endif

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

		blk=vox[cix];
		fm=0x3F;
		
		if(vy>cy)
			fm&=~1;
		else
			fm&=~4;

		if(vx>cx)
			fm&=~8;
		else
			fm&=~2;

		if(vz>=cz)
			fm&=~32;
		else
			fm&=~16;

		if(cix==wrl->scr_lhit)
			fm|=64;

		BTMGL_EmitBlockFaces(cx, cy, cz, fm, blk);

//		cx=((cx<<8)+0x80);
//		cy=((cy<<8)+0x80);
//		cz=((cz<<8)+0x80);
	}

	if(btmgl_blkemit_npts<=0)
		return(0);

	tkra_glBindTexture(TKRA_TEXTURE_2D, 2);

	tkra_glVertexPointer(3, TKRA_GL_FLOAT, 4*4, btmgl_blkemit_xyz+0);
	tkra_glTexCoordPointer(2, TKRA_GL_FLOAT, 2*4, btmgl_blkemit_st+0);
	tkra_glColorPointer(4, TKRA_GL_UNSIGNED_BYTE, 4, btmgl_blkemit_rgb+0);
//	tkra_glDrawArrays(TKRA_GL_TRIANGLES, 0, btmgl_blkemit_npts);
	tkra_glDrawArrays(TKRA_GL_QUADS, 0, btmgl_blkemit_npts);

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
	if(!alpha)
//		txc=GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		txc=TKRA_GL_CMPR_RGB_S3TC_DXT1;

	cs=css; mip=0;
	while(xshl>=0)
	{
		if(mip && !mipmap)
			break;
	
		xshl1=xshl-2;
		if(xshl1<0)
			xshl1=0;
	
		isz=1<<(xshl1+xshl1+3);
		tkra_glCompressedTexImage2D(
			GL_TEXTURE_2D, mip, 
			txc,
			1<<xshl, 1<<xshl, 0, isz, cs);
		cs+=isz;
		xshl--; mip++;
	}

	if (mipmap)
	{
		tkra_glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, btmgl_filter_min);
		tkra_glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, btmgl_filter_max);
	}
	else
	{
		tkra_glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER, btmgl_filter_max);
		tkra_glTexParameterf(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER, btmgl_filter_max);
	}
}

int BTM_GenTree(BTM_World *wrl, int cx, int cy, int cz)
{
	int i, j, k, d, h;

	h=3+(rand()&3);

	for(i=0; i<5; i++)
		for(j=0; j<5; j++)
			for(k=0; k<5; k++)
	{
		d=abs(i-2)+abs(j-2)+abs(k-2);
		if(d>4)
			continue;
		BTM_SetWorldBlockXYZ(wrl,
			cx+(i-2), cy+(j-2), cz+(h-2)+k,
			BTM_BLKTY_LEAVES);
	}

	for(k=0; k<4; k++)
	{
		BTM_SetWorldBlockXYZ(wrl, cx, cy, cz+k,
			BTM_BLKTY_LOG);
	}
}

int BTM_CheckWorldMoveBlockContents(BTM_World *wrl, int btm)
{
	u32 blkd;

	blkd=btmgl_vox_atlas_side[btm&0xFF];

	if(blkd&BTM_BLKDFL_NONSOLID)
		{ return(0); }
	if(blkd&BTM_BLKDFL_FLUID)
		{ return(2); }

	return(1);
}

int BTM_CheckWorldMoveSpot(BTM_World *wrl, float *org, const float *bbox)
{
	int cont;
	int cxm, cxn, cym, cyn, czm, czn;
	u32 blk0, blk1, blk2, blk3;
	u32 blk4, blk5, blk6, blk7;

//	cxm=org[0]-0.35;
//	cxn=org[0]+0.35;
//	cym=org[1]-0.35;
//	cyn=org[1]+0.35;

//	czm=org[2]-1.7;
//	czn=org[2]+0.1;

	cxm=org[0]+bbox[0];
	cxn=org[0]+bbox[3];
	cym=org[1]+bbox[1];
	cyn=org[1]+bbox[4];

	czm=org[2]+bbox[2];
	czn=org[2]+bbox[5];

//	blk0=BTM_GetWorldBlockXYZ(wrl, org[0], org[1], org[2]-2);
//	blk1=BTM_GetWorldBlockXYZ(wrl, org[0], org[1], org[2]-1);

	blk0=BTM_GetWorldBlockXYZ(wrl, cxm, cym, czm);
	blk1=BTM_GetWorldBlockXYZ(wrl, cxn, cym, czm);
	blk2=BTM_GetWorldBlockXYZ(wrl, cxm, cyn, czm);
	blk3=BTM_GetWorldBlockXYZ(wrl, cxn, cyn, czm);

	cont=0;

	cont|=BTM_CheckWorldMoveBlockContents(wrl, blk0);
	cont|=BTM_CheckWorldMoveBlockContents(wrl, blk1);
	cont|=BTM_CheckWorldMoveBlockContents(wrl, blk2);
	cont|=BTM_CheckWorldMoveBlockContents(wrl, blk3);

//	if(((blk0&255)>=2) && BTM_CheckWorldMoveBlockSolidP(wrl, blk0))
//		cont|=1;
//	if(((blk1&255)>=2) && BTM_CheckWorldMoveBlockSolidP(wrl, blk1))
//		cont|=1;
//	if(((blk2&255)>=2) && BTM_CheckWorldMoveBlockSolidP(wrl, blk2))
//		cont|=1;
//	if(((blk3&255)>=2) && BTM_CheckWorldMoveBlockSolidP(wrl, blk3))
//		cont|=1;

	blk4=BTM_GetWorldBlockXYZ(wrl, cxm, cym, czn);
	blk5=BTM_GetWorldBlockXYZ(wrl, cxn, cym, czn);
	blk6=BTM_GetWorldBlockXYZ(wrl, cxm, cyn, czn);
	blk7=BTM_GetWorldBlockXYZ(wrl, cxn, cyn, czn);

//	if(((blk4&255)>=2) && BTM_CheckWorldMoveBlockSolidP(wrl, blk4))
//		cont|=1;
//	if(((blk5&255)>=2) && BTM_CheckWorldMoveBlockSolidP(wrl, blk5))
//		cont|=1;
//	if(((blk6&255)>=2) && BTM_CheckWorldMoveBlockSolidP(wrl, blk6))
//		cont|=1;
//	if(((blk7&255)>=2) && BTM_CheckWorldMoveBlockSolidP(wrl, blk7))
//		cont|=1;

	cont|=BTM_CheckWorldMoveBlockContents(wrl, blk4);
	cont|=BTM_CheckWorldMoveBlockContents(wrl, blk5);
	cont|=BTM_CheckWorldMoveBlockContents(wrl, blk6);
	cont|=BTM_CheckWorldMoveBlockContents(wrl, blk7);

	if((blk4|blk5|blk6|blk7)&2)
		cont|=4;

//	if(((blk0&255)<2) && ((blk1&255)<2))
//		return(1);

	return(cont);
}

int BTM_CheckWorldBoxMoveVel(BTM_World *wrl, float dt,
	float *sorg, float *svel, const float *bbox,
	float *dorg, float *dvel, int *rfl)
{
	float tdo[4], tdv[4];
	int fl, cfl;
	int i, j, k;

	if(dt>0.1)
	{
		BTM_CheckWorldBoxMoveVel(wrl, dt*0.5, sorg, svel, bbox, tdo, tdv, rfl);
		BTM_CheckWorldBoxMoveVel(wrl, dt*0.5, tdo, tdv, bbox, dorg, dvel, rfl);
		return(0);
	}

	TKRA_Vec3F_Copy(sorg, tdo);
	TKRA_Vec3F_Copy(svel, tdv);

	TKRA_Vec3F_AddScale(sorg, tdv, dt, tdo);
	fl=*rfl;

	if(svel[2]>0)
		fl&=~1;

	cfl=BTM_CheckWorldMoveSpot(wrl, tdo, bbox);

	if(!(cfl&1))
	{
		if(svel[2]<0)
			fl&=~1;

		if(cfl&2)		
			fl|=2;
		else
			fl&=~2;

		TKRA_Vec3F_Copy(tdo, dorg);
		TKRA_Vec3F_Copy(tdv, dvel);
		*rfl=fl;
		return(0);
	}
	
//	if(tdv[2]<0)
	if(1)
	{
		TKRA_Vec3F_Copy(svel, tdv);
		tdv[2]=0;

		TKRA_Vec3F_AddScale(sorg, tdv, dt, tdo);
		if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
		{
			if(svel[2]<=0)
				fl|=1;
		
			TKRA_Vec3F_Copy(tdo, dorg);
			TKRA_Vec3F_Copy(tdv, dvel);
			*rfl=fl;
			return(0);
		}
	}

	if(1)
	{
		TKRA_Vec3F_Copy(svel, tdv);
//		tdv[2]=0;

		TKRA_Vec3F_AddScale(sorg, tdv, dt, tdo);
		tdo[2]+=0.5;
		
		if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
		{
//			if(svel[2]<0)
//				fl|=1;
		
			TKRA_Vec3F_Copy(tdo, dorg);
			TKRA_Vec3F_Copy(tdv, dvel);
			*rfl=fl;
			return(0);
		}
	}

	if(1)
	{
		TKRA_Vec3F_Copy(svel, tdv);
		tdv[0]=0;

		TKRA_Vec3F_AddScale(sorg, tdv, dt, tdo);
		if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
		{
			TKRA_Vec3F_Copy(tdo, dorg);
			TKRA_Vec3F_Copy(tdv, dvel);
			*rfl=fl;
			return(0);
		}
	}

	if(1)
	{
		TKRA_Vec3F_Copy(svel, tdv);
		tdv[1]=0;

		TKRA_Vec3F_AddScale(sorg, tdv, dt, tdo);
		if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
		{
			TKRA_Vec3F_Copy(tdo, dorg);
			TKRA_Vec3F_Copy(tdv, dvel);
			*rfl=fl;
			return(0);
		}
	}

	if(1)
	{
		TKRA_Vec3F_Copy(svel, tdv);
		tdv[2]=0;
		tdv[0]=0;

		TKRA_Vec3F_AddScale(sorg, tdv, dt, tdo);
		if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
		{
			if(svel[2]<=0)
				fl|=1;

			TKRA_Vec3F_Copy(tdo, dorg);
			TKRA_Vec3F_Copy(tdv, dvel);
			*rfl=fl;
			return(0);
		}
	}

	if(1)
	{
		TKRA_Vec3F_Copy(svel, tdv);
		tdv[2]=0;
		tdv[1]=0;

		TKRA_Vec3F_AddScale(sorg, tdv, dt, tdo);
		if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
		{
			if(svel[2]<=0)
				fl|=1;

			TKRA_Vec3F_Copy(tdo, dorg);
			TKRA_Vec3F_Copy(tdv, dvel);
			*rfl=fl;
			return(0);
		}
	}

	if(1)
	{
		TKRA_Vec3F_Copy(svel, tdv);
		tdv[0]=0;
		tdv[1]=0;

		TKRA_Vec3F_AddScale(sorg, tdv, dt, tdo);
		if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
		{
			TKRA_Vec3F_Copy(tdo, dorg);
			TKRA_Vec3F_Copy(tdv, dvel);
			*rfl=fl;
			return(0);
		}
	}

	if(BTM_CheckWorldMoveSpot(wrl, sorg, bbox)&1)
	{
		tdv[0]=0;
		tdv[1]=0;
		tdv[2]=0;

//		TKRA_Vec3F_Copy(svel, tdv);
		TKRA_Vec3F_Copy(sorg, tdo);
		tdo[2]+=1.25;

		if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
		{
			TKRA_Vec3F_Copy(tdo, dorg);
			TKRA_Vec3F_Copy(tdv, dvel);
			*rfl=fl;
			return(0);
		}
		
		for(i=-1; i<2; i++)
			for(j=-1; j<2; j++)
				for(k=-1; k<2; k++)
		{
			TKRA_Vec3F_Copy(sorg, tdo);
			tdo[0]+=k*0.25;
			tdo[1]+=j*0.25;
			tdo[2]+=i*0.25;
			if(!(BTM_CheckWorldMoveSpot(wrl, tdo, bbox)&1))
			{
				TKRA_Vec3F_Copy(tdo, dorg);
				TKRA_Vec3F_Copy(tdv, dvel);
				*rfl=fl;
				return(0);
			}

		}
	}

	tdv[0]=0;
	tdv[1]=0;
	tdv[2]=0;

	TKRA_Vec3F_Copy(sorg, dorg);
	TKRA_Vec3F_Copy(tdv, dvel);
	*rfl=fl;
	return(0);
}

int BTM_CheckWorldMoveVel(BTM_World *wrl, float dt,
	float *sorg, float *svel,
	float *dorg, float *dvel, int *rfl)
{
	static const float box[6] = { -0.35, -0.35, -1.7, 0.35, 0.35, 0.1};
	return(BTM_CheckWorldBoxMoveVel(wrl, dt,
		sorg, svel, box, dorg, dvel, rfl));
}
