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


BTM_MobSprite	* volatile btm_vissprites;

BTM_MobSprite	* volatile btm_freesprites;
BTM_MobSprite	* volatile btm_buildsprites;
void * volatile btm_sprite_mutex;

BTM_ConCmd	*btm_concmds;

int BTM_LockSprites()
{
	if(!btm_sprite_mutex)
		btm_sprite_mutex=thMutex();
	thLockMutex(btm_sprite_mutex);
	return(0);
}

int BTM_UnlockSprites()
{
	thUnlockMutex(btm_sprite_mutex);
	return(0);
}

BTM_MobSprite *BTM_AllocSprite()
{
	BTM_MobSprite *tmp;
	tmp=btm_freesprites;
	if(tmp)
	{
		btm_freesprites=tmp->next;
		return(tmp);
	}
	
	tmp=btm_malloc(sizeof(BTM_MobSprite));
	return(tmp);
}

int BTM_FreeSprite(BTM_MobSprite *spr)
{
	spr->next=btm_freesprites;
	btm_freesprites=spr;
	return(0);
}


int BTMGL_DrawSprite(BTM_World *wrl,
	BTM_MobSprite *mob)
{
	float sorg[3], vorg[3], v_fw[3], v_rt[3];
	float so_lf[3], so_rt[3];
	float so_tlf[3], so_trt[3];

	float to_lf[2], to_rt[2];
	float to_tlf[2], to_trt[2];

	int cxfull, cxhalf, cxlqtr, cxhqtr;
	int cx, cy, cz, vx, vy, vz, tex, ll;
	int va, vda, vdr;
	float ox, oy, oz, os, ot;
	u32 rgb;

	cxfull=1<<(24-0);
	cxhalf=1<<(24-1);
	cxlqtr=1<<(24-2);
	cxhqtr=cxhalf+cxlqtr;

	vx=(wrl->cam_org>> 0)&0x00FFFFFF;
	vy=(wrl->cam_org>>24)&0x00FFFFFF;
	vz=(wrl->cam_org>>48)&0x0000FFFF;
	
	vorg[0]=vx*(1.0/256);
	vorg[1]=vy*(1.0/256);
	vorg[2]=vz*(1.0/256);
	
	cx=mob->org_x;
	cy=mob->org_y;
	cz=mob->org_z;

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

//	ll=BTM_GetLightForXYZ(wrl, cx>>8, cy>>8, cz>>8);
//	rgb=BTM_GetColorRgbForBlockLight(ll);
	rgb=mob->spr_rgb;

//	tex=mob->spr_id;
	tex=BTMGL_LoadSpriteForName(
		mob->spr_base, mob->spr_seq, mob->spr_frame/4);
	
	sorg[0]=cx*(1.0/256);
	sorg[1]=cy*(1.0/256);
	sorg[2]=cz*(1.0/256);
	
	v_fw[0]=sorg[0]-vorg[0];
	v_fw[1]=sorg[1]-vorg[1];
	v_fw[2]=0;

	BTM_V3F_Normalize(v_fw, v_fw);

	v_rt[0]=-v_fw[1];
	v_rt[1]= v_fw[0];
	v_rt[2]=0;
	
	v_rt[0]=(v_rt[0]+wrl->cam_rt[0])/2;
	v_rt[1]=(v_rt[1]+wrl->cam_rt[1])/2;
	
	va=atan2(v_fw[1], v_fw[0])*(128.0/M_PI);
//	va=atan2(v_fw[0], v_fw[1])*(128.0/M_PI);
//	vda=mob->yaw-va;
	vda=64-(va-mob->yaw);
//	vda=va-mob->yaw;
	
	switch((vda>>5)&7)
	{
		case 0: vdr=0; break;
		case 7: vdr=0; break;

//		case 1: vdr=2; break;
//		case 2: vdr=2; break;

		case 1: vdr=3; break;
		case 2: vdr=3; break;

		case 3: vdr=1; break;
		case 4: vdr=1; break;

//		case 5: vdr=3; break;
//		case 6: vdr=3; break;

		case 5: vdr=2; break;
		case 6: vdr=2; break;
	}
	
//	so_lf[0]=sorg[0]-(mob->spr_dxs*wrl->cam_rt[0]*0.5);
//	so_lf[1]=sorg[1]-(mob->spr_dxs*wrl->cam_rt[1]*0.5);
//	so_lf[2]=sorg[2]-(mob->spr_dxs*wrl->cam_rt[2]*0.5);

//	so_rt[0]=sorg[0]+(mob->spr_dxs*wrl->cam_rt[0]*0.5);
//	so_rt[1]=sorg[1]+(mob->spr_dxs*wrl->cam_rt[1]*0.5);
//	so_rt[2]=sorg[2]+(mob->spr_dxs*wrl->cam_rt[2]*0.5);

	so_lf[0]=sorg[0]-(mob->spr_dxs*v_rt[0]*0.5);
	so_lf[1]=sorg[1]-(mob->spr_dxs*v_rt[1]*0.5);
	so_lf[2]=sorg[2]-(mob->spr_dxs*v_rt[2]*0.5);

	so_rt[0]=sorg[0]+(mob->spr_dxs*v_rt[0]*0.5);
	so_rt[1]=sorg[1]+(mob->spr_dxs*v_rt[1]*0.5);
	so_rt[2]=sorg[2]+(mob->spr_dxs*v_rt[2]*0.5);

	so_tlf[0]=so_lf[0];
	so_tlf[1]=so_lf[1];
	so_tlf[2]=so_lf[2]+mob->spr_dzs;

	so_trt[0]=so_rt[0];
	so_trt[1]=so_rt[1];
	so_trt[2]=so_rt[2]+mob->spr_dzs;

	os=(mob->spr_frame&3)*0.25;
//	ot=0.0;
	ot=vdr*0.25;

	to_lf[0] =os+0.00;	to_lf[1] =ot+0.25;
	to_rt[0] =os+0.25;	to_rt[1] =ot+0.25;
	to_tlf[0]=os+0.00;	to_tlf[1]=ot+0.00;
	to_trt[0]=os+0.25;	to_trt[1]=ot+0.00;

	pglEnable(GL_ALPHA_TEST);

	pglBindTexture(TKRA_TEXTURE_2D, tex);
//	pglColor4f(1.0, 1.0, 1.0, 1.0);
	pglColor4ubv((void *)&rgb);
//	pglColor4f(
//		((rgb    )&255)*(1.0/255),
//		((rgb>>16)&255)*(1.0/255),
//		((rgb>> 8)&255)*(1.0/255),
//		((rgb>>16)&255)*(1.0/255),
//		((rgb>> 0)&255)*(1.0/255),
//		1.0);
	
	pglBegin(GL_QUADS);

	pglTexCoord2fv(to_tlf);
	pglVertex3fv(so_tlf);

	pglTexCoord2fv(to_lf);
	pglVertex3fv(so_lf);

	pglTexCoord2fv(to_rt);
	pglVertex3fv(so_rt);

	pglTexCoord2fv(to_trt);
	pglVertex3fv(so_trt);

	pglEnd();
	
	return(0);
}

int BTMGL_DrawVisibleSprites(BTM_World *wrl)
{
	BTM_MobSprite	*mob;
	
	BTM_LockSprites();
	mob=btm_vissprites;
	while(mob)
	{
		BTMGL_DrawSprite(wrl, mob);
		mob=mob->next;
	}
	BTM_UnlockSprites();
	
	return(0);
}


int BTMGL_DrawEntityBasic(BTM_World *wrl,
	BTM_MobEntity *mob)
{
	BTM_MobSprite	*spr;
	int cx, cy, cz;
	int ll, d;
	u32 rgb;

	d=BTMGL_MobCalcDistanceCamera(wrl, mob);
	if(d>btm_drawdist)
		return(0);

	spr=BTM_AllocSprite();
	
	cx=mob->org_x;
	cy=mob->org_y;
	cz=mob->org_z;
	
	spr->org_x=cx;
	spr->org_y=cy;
	spr->org_z=cz;
	spr->yaw=mob->yaw;
	spr->pitch=mob->pitch;

	spr->spr_dxs=mob->spr_dxs;
	spr->spr_dzs=mob->spr_dzs;

	spr->spr_base=mob->spr_base;
	spr->spr_seq=mob->spr_seq;
	spr->spr_frame=mob->spr_frame;

	ll=BTM_GetLightForXYZ(wrl, cx>>8, cy>>8, cz>>8);
	rgb=BTM_GetColorRgbForBlockLight(ll);
	spr->spr_rgb=rgb;
	
	spr->next=btm_buildsprites;
	btm_buildsprites=spr;
	
	return(0);
}

int btmgl_tex_rov=64;

char *btmgl_tex_names[1024];
int btmgl_tex_txn[1024];
int btmgl_tex_cnt=0;

int BTMGL_LoadTextureForName(char *name)
{
	byte *tbuf;
	int tex, sz;
	int i, j, k;
	
	for(i=0; i<btmgl_tex_cnt; i++)
	{
		if(!strcmp(btmgl_tex_names[i], name))
			return(btmgl_tex_txn[i]);
	}
	
	sz=0;
	tbuf=BTM_LoadFileTmp(name, &sz);
	if(tbuf)
	{
		tex=btmgl_tex_rov++;
		pglBindTexture(TKRA_TEXTURE_2D, tex);
		BTMGL_UploadCompressed(tbuf, 1, 1);
//		btm_free(tbuf);
	}else
	{
		tex=0;
	}
	
	i=btmgl_tex_cnt++;
	btmgl_tex_names[i]=bccx_strdup(name);
	btmgl_tex_txn[i]=tex;
	return(tex);
}

int BTMGL_LoadSpriteForName(char *base, int seq, int frm)
{
	char tb[256];
	
	if(frm)
	{
		sprintf(tb, "sprites/%s_m%uf%u.dds", base, seq, frm);
	}else
	{
		sprintf(tb, "sprites/%s_m%u.dds", base, seq);
	}
	return(BTMGL_LoadTextureForName(tb));
}


bccx_cxstate	bccx_mobj;

BTM_MobEntity *BTM_AllocWorldMob(BTM_World *wrl)
{
	BTM_MobEntity *tmp;
	
	tmp=wrl->free_mobent;
	if(tmp)
	{
		wrl->free_mobent=tmp;
		memset(tmp, 0, sizeof(BTM_MobEntity));
		return(tmp);
	}
	
	tmp=malloc(sizeof(BTM_MobEntity));
	memset(tmp, 0, sizeof(BTM_MobEntity));
	return(tmp);
}

int BTM_RunTickMobNone(BTM_World *wrl, BTM_MobEntity *self)
{
	return(0);
}

int BTM_RunMobTickMove(BTM_World *wrl, BTM_MobEntity *self)
{
	float org[3], vel[3], ivel[3];
	float dtf, frc;
	float f0, f1, f2;
	int mvflag;
	
	dtf=0.1;

	org[0]=self->org_x*(1.0/256);
	org[1]=self->org_y*(1.0/256);
	org[2]=self->org_z*(1.0/256);

	vel[0]=self->vel_x*(1.0/256);
	vel[1]=self->vel_y*(1.0/256);
	vel[2]=self->vel_z*(1.0/256);

	ivel[0]=self->ivel_x*(1.0/256);
	ivel[1]=self->ivel_y*(1.0/256);
	ivel[2]=self->ivel_z*(1.0/256);

	mvflag=self->mvflag;

	vel[2]-=16*dtf;
	
	if(mvflag&4)
	{
		frc=1.0-4*dtf;
		vel[2]=vel[2]*frc+26*dtf;
	}else
		if(mvflag&2)
	{
		frc=1.0-2*dtf;
		vel[2]=vel[2]*frc+20*dtf;
	}

	frc=1.0-8*dtf;

	if(mvflag&2)
		frc=1.0-1*dtf;

	if(frc<0)
		frc=0;

	if(TKRA_Vec3F_DotProduct(vel, vel)<0.2)
		frc=0;

	if(mvflag&1)
	{
		f0=TKRA_Vec3F_DotProduct(vel, ivel);
		f1=TKRA_Vec3F_DotProduct(vel, vel)+0.01;
		f2=f0/f1;
	
		if(f2<=0.9)
		{
			vel[0]*=frc;
			vel[1]*=frc;
//					TKRA_Vec3F_Scale(cam_vel, frc, cam_vel);
		}
	}else
		if(mvflag&2)
	{
		f0=TKRA_Vec3F_DotProduct(vel, ivel);
		f1=TKRA_Vec3F_DotProduct(vel, vel)+0.01;
		f2=f0/f1;
	
		if(f2<=0.9)
		{
			vel[0]*=frc;
			vel[1]*=frc;
//					TKRA_Vec3F_Scale(cam_vel, frc, cam_vel);
		}
	}
	
	if(TKRA_Vec3F_DotProduct(vel, ivel)<49)
	{
		if(mvflag&1)
		{
			vel[0]+=ivel[0]*dtf*4.0;
			vel[1]+=ivel[1]*dtf*4.0;
			vel[2]+=ivel[2]*dtf*4.0;
		}else
			if(mvflag&2)
		{
			vel[0]+=ivel[0]*dtf*1.0;
			vel[1]+=ivel[1]*dtf*1.0;
			vel[2]+=ivel[2]*dtf*1.0;
		}else
		{
			vel[0]+=ivel[0]*dtf*0.1;
			vel[1]+=ivel[1]*dtf*0.1;
			vel[2]+=ivel[2]*dtf*0.1;
		}
	}

	BTM_CheckWorldMoveVelSz(wrl, dtf,
		org, vel,
		self->rad_x*(1.0/16),
		self->rad_z*(1.0/16),
		self->rad_ofs_z*(1.0/16),
		org, vel, &mvflag);
	
	self->org_x=org[0]*256;
	self->org_y=org[1]*256;
	self->org_z=org[2]*256;

	self->vel_x=vel[0]*256;
	self->vel_y=vel[1]*256;
	self->vel_z=vel[2]*256;

	self->mvflag=mvflag;


	if(	(self->orgl_x!=self->org_x) ||
		(self->orgl_y!=self->org_y) ||
		(self->orgl_z!=self->org_z) )
	{
		self->rgn->dirty|=32;
	}

	self->orgl_x=self->org_x;
	self->orgl_y=self->org_y;
	self->orgl_z=self->org_z;

	return(0);
}

u64 BTM_MobGetOriginPos(BTM_World *wrl, BTM_MobEntity *self)
{
	u64 cpos;
	int cx, cy, cz;

	cx=self->org_x;
	cy=self->org_y;
	cz=self->org_z;

	cpos=
		((cx&0x00FFFFFFULL)    ) |
		((cy&0x00FFFFFFULL)<<24) |
		((cz&0x0000FFFFULL)<<48) ;
	return(cpos);
}

u64 BTM_MobGetVelPos(BTM_World *wrl, BTM_MobEntity *self)
{
	u64 cpos;
	int cx, cy, cz;

	cx=self->vel_x;
	cy=self->vel_y;
	cz=self->vel_z;

	cpos=
		((cx&0x00FFFFFFULL)    ) |
		((cy&0x00FFFFFFULL)<<24) |
		((cz&0x0000FFFFULL)<<48) ;
	return(cpos);
}

u64 BTM_MobGetImpulsePos(BTM_World *wrl, BTM_MobEntity *self)
{
	u64 cpos;
	int cx, cy, cz;

	cx=self->ivel_x;
	cy=self->ivel_y;
	cz=self->ivel_z;

	cpos=
		((cx&0x00FFFFFFULL)    ) |
		((cy&0x00FFFFFFULL)<<24) |
		((cz&0x0000FFFFULL)<<48) ;
	return(cpos);
}

u64 BTM_MobGetOriginBlockPos(BTM_World *wrl, BTM_MobEntity *self)
{
	u64 cpos;
	int cx, cy, cz;

	cx=self->org_x>>8;
	cy=self->org_y>>8;
	cz=self->org_z>>8;

	cpos=
		((cx&0x0000FFFFULL)    ) |
		((cy&0x0000FFFFULL)<<16) |
		((cz&0x000000FFULL)<<32) ;
	return(cpos);
}

int BTM_CheckMobCanMoveSpot(BTM_World *wrl, BTM_MobEntity *self, u64 spot)
{
	u64 cpos;
	u32 blk0, blk1, blk2, blkc0, blkc1;
	
	cpos=BTM_MobGetOriginPos(wrl, self);

	blk0=BTM_TryGetWorldBlockCorg(wrl, spot);
	if(!(blk0&255))
		return(0);

	blk1=BTM_TryGetWorldBlockCorg(wrl, spot-(1LL<<56));
	blk2=BTM_TryGetWorldBlockCorg(wrl, spot+(1LL<<56));

	if(!(blk1&255))
		return(0);
	if(!(blk2&255))
		return(0);


	blkc0=BTM_TryGetWorldBlockCorg(wrl, cpos);
	blkc1=BTM_TryGetWorldBlockCorg(wrl, cpos-(1LL<<56));

	if(!(blkc0&255))
		return(0);
	if(!(blkc1&255))
		return(0);

	if((blk0&255)>=4)
	{
		if((blk2&255)<4)
		{
			return(2);
		}
		return(0);
	}

	if((blk1&255)<4)
	{
		return(0);
	}
	
	if(BTM_BlockIsFluidP(wrl, blk1))
	{
		if(BTM_BlockIsFluidP(wrl, blkc1))
		{
			return(1);
		}
		return(0);
	}
	
	return(1);
}

int BTM_MobPlaySound(BTM_World *wrl, BTM_MobEntity *self,
	char *name, int vol)
{
	u64 cpos, cvel;
	int tone;

	tone=256+((rand()&127)-63);

	cpos=BTM_MobGetOriginPos(wrl, self);
	cvel=BTM_MobGetVelPos(wrl, self);

	BTM_PlaySample3D(name, cpos, cvel, vol, tone);
	return(0);
}

int BTM_RunTickMobBasic(BTM_World *wrl, BTM_MobEntity *self)
{
	u64 cpos, mvpos;
	char *snd;
	int mvfl, d;

	d=BTMGL_MobCalcDistanceCamera(wrl, self);
	if(d>(2*btm_drawdist))
		return(0);

	BTM_RunMobTickMove(wrl, self);

	cpos=BTM_MobGetOriginPos(wrl, self);
	mvpos=cpos+BTM_MobGetImpulsePos(wrl, self);

	mvfl=BTM_CheckMobCanMoveSpot(wrl, self, mvpos);
	if(!mvfl)
	{
		self->mob_rtick=0;
	}

	if(self->mob_rtick>0)
	{
		self->mob_rtick--;
	}else
	{
		self->yaw=rand()&255;
		self->mob_rtick=128+(rand()&63);
		self->mob_mvtick=rand()&63;
		
		if(!(rand()&15))
		{
			snd=NULL;
			
			if(!strcmp(self->cname, "pig"))
				snd="sound/animal/pig0.wav";

			if(!strcmp(self->cname, "sheep"))
			{
				if(rand()&1)
					snd="sound/animal/sheep_idle0.wav";
				else
					snd="sound/animal/sheep_idle1.wav";
			}

			if(!strcmp(self->cname, "cow"))
				snd="sound/animal/moo0.wav";

			if(!strcmp(self->cname, "chicken"))
			{
				if(rand()&1)
					snd="sound/animal/cluck1.wav";
				else
					snd="sound/animal/cluck1.wav";
			}
		
			if(snd)
				BTM_MobPlaySound(wrl, self, snd, 255);
		}
	}

	if((self->vel_x+self->vel_y)!=0)
	{
		self->spr_frame=(self->spr_frame+1)&3;
	}

	if(self->mob_mvtick>0)
	{
		self->mob_mvtick--;
		self->ivel_x= 2*sin(self->yaw*(M_PI/128))*256;
		self->ivel_y=-2*cos(self->yaw*(M_PI/128))*256;
		if(mvfl&2)
		{
//			self->ivel_z=12;
			self->org_z+=384;
		}
	}else
	{
		self->ivel_x=0;
		self->ivel_y=0;
	}

	return(0);
}


int BTM_MobCalcDistanceXY(BTM_World *wrl,
	BTM_MobEntity *self, BTM_MobEntity *other)
{
	int dx, dy, d;
	
	dx=self->org_x-other->org_x;
	dy=self->org_y-other->org_y;
	if(dx<0)	dx^=dx>>31;
	if(dy<0)	dy^=dy>>31;
	if(dx>dy)
		d=dx+(dy>>1);
	else
		d=dy+(dx>>1);
	return(d);
}

int BTM_MobCalcDistanceZ(BTM_World *wrl,
	BTM_MobEntity *self, BTM_MobEntity *other)
{
	int d;
	d=self->org_z-other->org_z;
	if(d<0)	d^=d>>31;
	return(d);
}

int BTMGL_MobCalcDistanceCamera(BTM_World *wrl,
	BTM_MobEntity *mob)
{
	u64 cpos, epos;
	int dist;

	cpos=wrl->cam_org;
	epos=BTM_MobGetOriginPos(wrl, mob);
	dist=BTM_CalcRayDistApprox(cpos, epos);
	return(dist>>8);
}

int BTM_MobCheckHandleCollide(BTM_World *wrl,
	BTM_MobEntity *self, BTM_MobEntity *other)
{
	int dx, dy, dz, rx, rz, ix, iz;

	if(self==other)
		return(0);
		
	dx=BTM_MobCalcDistanceXY(wrl, self, other);
	rx=(self->rad_x+other->rad_x)<<4;
	if(dx>=rx)
		return(0);

	dz=BTM_MobCalcDistanceZ(wrl, self, other);
	rz=(self->rad_z+other->rad_z)<<4;
	if(dz>=rz)
		return(0);
		
	ix=rx-dx;
	iz=rz-dz;

	dx=self->org_x-other->org_x;
	dy=self->org_y-other->org_y;
	dz=self->org_z-other->org_z;

	self->vel_x+=(dx*ix)>>8;
	self->vel_y+=(dy*ix)>>8;
	self->vel_z+=(dz*iz)>>8;

	return(0);
}

int BTMGL_DrawEntityBasic(BTM_World *wrl,
	BTM_MobEntity *mob);

int BTM_RunSpawnerForEntity(BTM_World *wrl,
	BTM_MobEntity *mob)
{
	mob->Tick=BTM_RunTickMobNone;
	mob->Draw=BTMGL_DrawEntityBasic;
	mob->spr_dxs=1;
	mob->spr_dzs=1;

	if(!strcmp(mob->cname, "chicken"))
	{
		mob->Tick=BTM_RunTickMobBasic;
		mob->spr_base="chicken1";
	}

	if(!strcmp(mob->cname, "pig"))
	{
		mob->Tick=BTM_RunTickMobBasic;
		mob->spr_base="pig1";
		mob->spr_dxs=2;
		mob->spr_dzs=2;
	}

	if(!strcmp(mob->cname, "sheep"))
	{
		mob->Tick=BTM_RunTickMobBasic;
		mob->spr_base="sheep1";
		mob->spr_dxs=2;
		mob->spr_dzs=2;
	}

	if(!strcmp(mob->cname, "cow"))
	{
		mob->Tick=BTM_RunTickMobBasic;
		mob->spr_base="cow1";
		mob->spr_dxs=3;
		mob->spr_dzs=3;
	}

	return(0);
}

int BTM_CheckForEntityClass(BTM_World *wrl,
	BTM_MobEntity *mob, int cls)
{
	if(!cls)
		return(1);
		
	if(cls==1)
	{
		if(!strcmp(mob->cname, "chicken"))
			return(1);
		if(!strcmp(mob->cname, "pig"))
			return(1);
		if(!strcmp(mob->cname, "cow"))
			return(1);
		if(!strcmp(mob->cname, "sheep"))
			return(1);
		return(0);
	}
	
	return(0);
}

int BTM_CountRegionEntityClass(BTM_World *wrl,
	BTM_Region *rgn, int cls)
{
	BTM_MobEntity *mob;
	int cnt;
	
	cnt=0;
	mob=rgn->live_entity;
	while(mob)
	{
		if(BTM_CheckForEntityClass(wrl, mob, cls))
			cnt++;
		mob=mob->next;
	}
	return(cnt);
}

bccx_cxstate	bccx_classname;
bccx_cxstate	bccx_org_x;
bccx_cxstate	bccx_org_y;
bccx_cxstate	bccx_org_z;
bccx_cxstate	bccx_yaw;
bccx_cxstate	bccx_pitch;
bccx_cxstate	bccx_flags;
bccx_cxstate	bccx_entvar;

bccx_cxstate	bccx_name;
bccx_cxstate	bccx_value;
bccx_cxstate	bccx_global;
bccx_cxstate	bccx_player;
bccx_cxstate	bccx_time;
bccx_cxstate	bccx_day;
bccx_cxstate	bccx_inven;
bccx_cxstate	bccx_index;
bccx_cxstate	bccx_count;
bccx_cxstate	bccx_item;
bccx_cxstate	bccx_setcvar;

int BTM_MobBindVarInt(BTM_MobEntity *mob, char *name, s64 val)
{
	BCCX_AttrVal *av;
	int i, j, k, ix;

	ix=BCCX_StringToStridx(name);

	i=mob->mobvar_cnt-1;
	while(i>=0)
	{
		j=mob->mobvar_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(mob->mobvar_val+i);
			mob->mobvar_name[i]=ix|(BCCX_IVTY_INT<<12);
			av->i=val;
			return(0);
		}
		i--;
	}

	i=mob->mobvar_cnt++;
	av=(BCCX_AttrVal *)(mob->mobvar_val+i);
	mob->mobvar_name[i]=ix|(BCCX_IVTY_INT<<12);
	av->i=val;

	return(0);
}

int BTM_MobBindVarReal(BTM_MobEntity *mob, char *name, double val)
{
	BCCX_AttrVal *av;
	int i, j, k, ix;

	ix=BCCX_StringToStridx(name);

	i=mob->mobvar_cnt-1;
	while(i>=0)
	{
		j=mob->mobvar_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(mob->mobvar_val+i);
			mob->mobvar_name[i]=ix|(BCCX_IVTY_REAL<<12);
			av->f=val;
			return(0);
		}
		i--;
	}

	i=mob->mobvar_cnt++;
	av=(BCCX_AttrVal *)(mob->mobvar_val+i);
	mob->mobvar_name[i]=ix|(BCCX_IVTY_REAL<<12);
	av->f=val;

	return(0);
}

int BTM_MobBindVarStr(BTM_MobEntity *mob, char *name, char *val)
{
	BCCX_AttrVal *av;
	int i, j, k, ix;

	ix=BCCX_StringToStridx(name);

	i=mob->mobvar_cnt-1;
	while(i>=0)
	{
		j=mob->mobvar_name[i];
		if((j&4095)==ix)
		{
			av=(BCCX_AttrVal *)(mob->mobvar_val+i);
			mob->mobvar_name[i]=ix|(BCCX_IVTY_STRING<<12);
			av->s=bccx_strdup(val);
			return(0);
		}
		i--;
	}

	i=mob->mobvar_cnt++;
	av=(BCCX_AttrVal *)(mob->mobvar_val+i);
	mob->mobvar_name[i]=ix|(BCCX_IVTY_STRING<<12);
	av->s=bccx_strdup(val);

	return(0);
}

int BTM_SpawnRegionEntitySubTag(BTM_World *wrl,
	BTM_Region *rgn, BTM_MobEntity *mob, BCCX_Node *ent)
{
	char *name, *str;
	s64 li;
	double lf;
	int ani, vx, vy, vz;

	if(BCCX_TagIsCstP(ent, &bccx_entvar, "entvar"))
	{
		name=BCCX_GetCst(ent, &bccx_name, "name");
		ani=BCCX_GetAttrNameCst(ent, &bccx_value, "value");
		
		if((ani>>12)==BCCX_IVTY_INT)
		{
			li=BCCX_GetIntCst(ent, &bccx_value, "value");
			BTM_MobBindVarInt(mob, name, li);
		}else
			if((ani>>12)==BCCX_IVTY_REAL)
		{
			lf=BCCX_GetFloatCst(ent, &bccx_value, "value");
			BTM_MobBindVarReal(mob, name, lf);
		}else if((ani>>12)==BCCX_IVTY_STRING)
		{
			str=BCCX_GetCst(ent, &bccx_value, "value");
			BTM_MobBindVarStr(mob, name, str);
		}
		
		return(0);
	}
		
	return(0);
}

int BTM_SpawnRegionEntity(BTM_World *wrl,
	BTM_Region *rgn, BCCX_Node *ent)
{
	double org[3];
	BTM_MobEntity *mob;
	BCCX_Node *nt1;
	char *cname;
	int na, ci;
	int yaw, pitch;

	if(BCCX_TagIsCstP(ent, &bccx_mobj, "mobj"))
	{
		cname=BCCX_GetCst(ent, &bccx_classname, "classname");
		org[0]=BCCX_GetFloatCst(ent, &bccx_org_x, "org_x");
		org[1]=BCCX_GetFloatCst(ent, &bccx_org_y, "org_y");
		org[2]=BCCX_GetFloatCst(ent, &bccx_org_z, "org_z");
		yaw=BCCX_GetIntCst(ent, &bccx_yaw, "yaw");
		pitch=BCCX_GetIntCst(ent, &bccx_pitch, "pitch");
		
		mob=BTM_AllocWorldMob(wrl);
		
		mob->cname=cname;
		mob->org_x=org[0]*256;
		mob->org_y=org[1]*256;
		mob->org_z=org[2]*256;
		mob->yaw=yaw;
		mob->yaw=pitch;
		
		mob->orgl_x=mob->org_x;
		mob->orgl_y=mob->org_y;
		mob->orgl_z=mob->org_z;
		
		mob->wrl=wrl;
		mob->rgn=rgn;
		
		mob->next=rgn->live_entity;
		rgn->live_entity=mob;

		na=BCCX_GetNodeChildCount(ent);
		for(ci=0; ci<na; ci++)
		{
			nt1=BCCX_GetNodeIndex(ent, ci);
			BTM_SpawnRegionEntitySubTag(wrl, rgn, mob, nt1);
		}

		
		BTM_RunSpawnerForEntity(wrl, mob);
		return(0);
	}

	return(0);
}

int BTM_SpawnRegionEntities(BTM_World *wrl,
	BTM_Region *rgn, BCCX_Node *ents)
{
	BCCX_Node *ntmp;
	int na, ci;

	if(!ents)
		return(0);

	na=BCCX_GetNodeChildCount(ents);
	for(ci=0; ci<na; ci++)
	{
		ntmp=BCCX_GetNodeIndex(ents, ci);
		BTM_SpawnRegionEntity(wrl, rgn, ntmp);
	}

	return(0);
}

BCCX_Node *BTM_FlattenRegionLiveEntities(
	BTM_World *wrl, BTM_Region *rgn)
{
	BCCX_Node *ntmp, *elst, *nt1;
	BTM_MobEntity *mob;
	BCCX_AttrVal *av;
	char *str;
	int i, j, k, ix;

	mob=rgn->live_entity;
	if(!mob)
	{
		return(NULL);
	}

	elst=BCCX_New("entities");
	while(mob)
	{
		ntmp=BCCX_NewCst(&bccx_mobj, "mobj");
		BCCX_SetCst(ntmp, &bccx_classname, "classname", mob->cname);
		BCCX_SetFloatCst(ntmp, &bccx_org_x, "org_x", mob->org_x*(1.0/256));
		BCCX_SetFloatCst(ntmp, &bccx_org_y, "org_y", mob->org_y*(1.0/256));
		BCCX_SetFloatCst(ntmp, &bccx_org_z, "org_z", mob->org_z*(1.0/256));

		if(mob->yaw)
			BCCX_SetIntCst(ntmp, &bccx_yaw, "yaw", mob->yaw);
		if(mob->pitch)
			BCCX_SetIntCst(ntmp, &bccx_pitch, "pitch", mob->pitch);
		
		for(i=0; i<mob->mobvar_cnt; i++)
		{
			av=(BCCX_AttrVal *)(mob->mobvar_val+i);
			ix=mob->mobvar_name[i];
			str=BCCX_StridxToString(ix);
			
			nt1=BCCX_NewCst(&bccx_entvar, "entvar");
			BCCX_SetCst(nt1, &bccx_name, "name", str);

			if((ix>>12)==BCCX_IVTY_INT)
			{
				BCCX_SetIntCst(nt1, &bccx_value, "value", av->i);
			}else if((ix>>12)==BCCX_IVTY_REAL)
			{
				BCCX_SetFloatCst(nt1, &bccx_value, "value", av->f);
			}else if((ix>>12)==BCCX_IVTY_STRING)
			{
				BCCX_SetCst(nt1, &bccx_value, "value", av->s);
			}else if((ix>>12)==BCCX_IVTY_NODE)
			{
				BCCX_Add(nt1, BCCX_New1("value", BCCX_CloneS(av->p)));
			}

			BCCX_Add(ntmp, nt1);
		}
		
		BCCX_Add(elst, ntmp);

		mob=mob->next;
	}

	return(elst);
}

int BTM_SpawnWorldGlobal(BTM_World *wrl, BCCX_Node *ent)
{
	double org[3];
	BCCX_Node *tn1, *tn2, *tn3;
	char *name, *str, *s1, *s2;
	s64 li, cpos;
	double lf;
	int ani, vx, vy, vz, na, ci, ix, cn, it;

	if(BCCX_TagIsCstP(ent, &bccx_global, "global"))
	{
		name=BCCX_GetCst(ent, &bccx_name, "name");
		ani=BCCX_GetAttrNameCst(ent, &bccx_value, "value");
		
		if((ani>>12)==BCCX_IVTY_INT)
		{
			li=BCCX_GetIntCst(ent, &bccx_value, "value");
			BTM_InstSetgVarInt(wrl, name, li);
		}else
			if((ani>>12)==BCCX_IVTY_REAL)
		{
			lf=BCCX_GetFloatCst(ent, &bccx_value, "value");
			BTM_InstSetgVarReal(wrl, name, lf);
		}else if((ani>>12)==BCCX_IVTY_STRING)
		{
			str=BCCX_GetCst(ent, &bccx_value, "value");
			BTM_InstSetgVarStr(wrl, name, str);
		}
		
		return(0);
	}

	if(BCCX_TagIsCstP(ent, &bccx_player, "player"))
	{
		org[0]=BCCX_GetFloatCst(ent, &bccx_org_x, "org_x");
		org[1]=BCCX_GetFloatCst(ent, &bccx_org_y, "org_y");
		org[2]=BCCX_GetFloatCst(ent, &bccx_org_z, "org_z");
		wrl->cam_yaw=BCCX_GetIntCst(ent, &bccx_yaw, "yaw");
		wrl->cam_pitch=BCCX_GetIntCst(ent, &bccx_pitch, "pitch");
		wrl->cam_flags=BCCX_GetIntCst(ent, &bccx_flags, "flags");
		
		vx=org[0]*256.0;
		vy=org[1]*256.0;
		vz=org[2]*256.0;
		cpos=
			((vx&0x00FFFFFFULL)<< 0) |
			((vy&0x00FFFFFFULL)<<24) |
			((vz&0x0000FFFFULL)<<48) ;
		wrl->cam_org=cpos;
		
		wrl->daytimer=BCCX_GetIntCst(ent, &bccx_time, "time");
		wrl->day=BCCX_GetIntCst(ent, &bccx_day, "day");

		na=BCCX_GetNodeChildCount(ent);
		for(ci=0; ci<na; ci++)
		{
			tn1=BCCX_GetNodeIndex(ent, ci);
			BTM_SpawnWorldGlobal(wrl, tn1);
		}

		return(0);
	}

	if(BCCX_TagIsCstP(ent, &bccx_inven, "inven"))
	{
		ix=BCCX_GetIntCst(ent, &bccx_index, "index");
		cn=BCCX_GetIntCst(ent, &bccx_count, "count");
		it=BCCX_GetIntCst(ent, &bccx_item, "item");
		
		if(cn>0)
			{ wrl->cam_inven[ix]=it|((cn-1)<<16); }
		else
			{ wrl->cam_inven[ix]=0; }

		return(0);
	}

	if(BCCX_TagIsP(ent, "setcvar"))
	{
		s1=BCCX_Get(ent, "name");
		s2=BCCX_Get(ent, "value");
		BTM_CvarSetStr(s1, s2);
		return(0);
	}

	if(BCCX_TagIsP(ent, "concmd"))
	{
		s2=BCCX_Get(ent, "text");
		BTM_ConsoleCommand(s2);
		return(0);
	}

	if(BCCX_TagIsP(ent, "conecho"))
	{
		s2=BCCX_Get(ent, "text");
		BTM_ConPuts(s2);
		return(0);
	}

	return(-1);
}

int BTM_SpawnWorldGlobalState(BTM_World *wrl, BCCX_Node *ents)
{
	BCCX_Node *ntmp;
	int na, ci;

	if(!ents)
		return(0);

	na=BCCX_GetNodeChildCount(ents);
	for(ci=0; ci<na; ci++)
	{
		ntmp=BCCX_GetNodeIndex(ents, ci);
		BTM_SpawnWorldGlobal(wrl, ntmp);
	}

	return(0);
}

BCCX_Node *BTM_FlattenWorldGlobalState(BTM_World *wrl)
{
	BCCX_Node *ntmp, *elst;
	BCCX_AttrVal *av;
	BTM_ConCmd *cvar;
	char *str, *s0, *s1;
	int vx, vy, vz;
	int i, j, k, ix;

	elst=BCCX_New("worldstate");

	if(1)
	{
		ntmp=BCCX_NewCst(&bccx_player, "player");
//		BCCX_SetCst(ntmp, &bccx_name, "name", str);

		vx=(wrl->cam_org>> 0)&0x00FFFFFFU;
		vy=(wrl->cam_org>>24)&0x00FFFFFFU;
		vz=(wrl->cam_org>>48)&0x0000FFFFU;

		BCCX_SetFloatCst(ntmp, &bccx_org_x, "org_x", vx*(1.0/256));
		BCCX_SetFloatCst(ntmp, &bccx_org_y, "org_y", vy*(1.0/256));
		BCCX_SetFloatCst(ntmp, &bccx_org_z, "org_z", vz*(1.0/256));
		BCCX_SetIntCst(ntmp, &bccx_yaw, "yaw", wrl->cam_yaw);
		BCCX_SetIntCst(ntmp, &bccx_pitch, "pitch", wrl->cam_pitch);
		BCCX_SetIntCst(ntmp, &bccx_flags, "flags", wrl->cam_flags);

		BCCX_SetIntCst(ntmp, &bccx_time, "time", wrl->daytimer);
		BCCX_SetIntCst(ntmp, &bccx_day, "day", wrl->day);
		
		BCCX_Add(elst, ntmp);


		ntmp=BCCX_NewCst(&bccx_player, "setcvar");
		BCCX_SetCst(ntmp, &bccx_name, "name", "r_drawdist");
		BCCX_SetIntCst(ntmp, &bccx_name, "value", btm_drawdist);
	}

	for(i=0; i<wrl->tgen_vargbl_cnt; i++)
	{
		av=(BCCX_AttrVal *)(wrl->tgen_vargbl_val+i);
		ix=wrl->tgen_vargbl_name[i];
		str=BCCX_StridxToString(ix);
		
		ntmp=BCCX_NewCst(&bccx_global, "global");
		BCCX_SetCst(ntmp, &bccx_name, "name", str);

		if((ix>>12)==BCCX_IVTY_INT)
		{
			BCCX_SetIntCst(ntmp, &bccx_value, "value", av->i);
		}else if((ix>>12)==BCCX_IVTY_REAL)
		{
			BCCX_SetFloatCst(ntmp, &bccx_value, "value", av->f);
		}else if((ix>>12)==BCCX_IVTY_STRING)
		{
			BCCX_SetCst(ntmp, &bccx_value, "value", av->s);
		}else if((ix>>12)==BCCX_IVTY_NODE)
		{
//			BCCX_SetCst(ntmp, &bccx_value, "value", av->s);
			BCCX_Add(ntmp, BCCX_New1("value", BCCX_CloneS(av->p)));
		}

		BCCX_Add(elst, ntmp);
	}
	
	cvar=btm_concmds;
	while(cvar)
	{
		if(cvar->cvar && !(cvar->flag&1))
		{
			s0=cvar->name;
			s1=BTM_CvarGetStr(s0);

			ntmp=BCCX_NewCst(&bccx_setcvar, "setcvar");
			BCCX_SetCst(ntmp, &bccx_name, "name", s0);
			BCCX_SetCst(ntmp, &bccx_value, "value", s1);
			BCCX_Add(elst, ntmp);
		}
	
		cvar=cvar->next;
	}
	
#if 0
	while(mob)
	{
		ntmp=BCCX_NewCst(&bccx_mobj, "mobj");
		BCCX_SetCst(ntmp, &bccx_classname, "classname", mob->cname);
		BCCX_SetFloatCst(ntmp, &bccx_org_x, "org_x", mob->org_x*(1.0/256));
		BCCX_SetFloatCst(ntmp, &bccx_org_y, "org_y", mob->org_y*(1.0/256));
		BCCX_SetFloatCst(ntmp, &bccx_org_z, "org_z", mob->org_z*(1.0/256));

		if(mob->yaw)
			BCCX_SetIntCst(ntmp, &bccx_yaw, "yaw", mob->yaw);
		if(mob->pitch)
			BCCX_SetIntCst(ntmp, &bccx_pitch, "pitch", mob->pitch);
		
		BCCX_Add(elst, ntmp);

		mob=mob->next;
	}
#endif

	return(elst);
}

int BTMGL_SpawnWorldMobClass(BTM_World *wrl, u64 cpos, int cls)
{
	float org[3];
	BTM_Region *rgn;
	BCCX_Node *ntmp;
	char *cname;
	int rix, j;
	
	rix=BTM_WorldCorgToRix(cpos);
	rgn=BTM_GetRegionForRix(wrl, rix);

	org[0]=((cpos>> 0)&0x00FFFFFF)*(1.0/256);
	org[1]=((cpos>>24)&0x00FFFFFF)*(1.0/256);
	org[2]=((cpos>>48)&0x0000FFFF)*(1.0/256);

	cname=NULL;
	
	if(cls==1)
	{
		j=rand()&3;
		switch(j)
		{
			case 0: cname="chicken"; break;
			case 1: cname="pig"; break;
			case 2: cname="cow"; break;
			case 3: cname="sheep"; break;
		}
	}

	if(cname)
	{
		ntmp=BCCX_NewCst(&bccx_mobj, "mobj");
		BCCX_SetCst(ntmp,
			&bccx_classname, "classname", cname);
		BCCX_SetFloatCst(ntmp,
			&bccx_org_x, "org_x", org[0]);
		BCCX_SetFloatCst(ntmp,
			&bccx_org_y, "org_y", org[1]);
		BCCX_SetFloatCst(ntmp,
			&bccx_org_z, "org_z", org[2]);

		BTM_SpawnRegionEntity(wrl, rgn, ntmp);
	}
	return(0);
}

int BTMGL_DrawWorldEntities(BTM_World *wrl)
{
	BTM_Region *rgn;
	BTM_MobEntity *mob;
	BTM_MobSprite	*oldspr, *curspr, *nxtspr;
	BCCX_Node	*ntmp;
	u64 rpos;
	int cx, cy, vx, vy, dx, dy, d;

	BTMGL_LockWorld();

	vx=(wrl->cam_org>> 8)&0xFFFF;
	vy=(wrl->cam_org>>32)&0xFFFF;

	btm_buildsprites=NULL;

	rgn=wrl->region;
	while(rgn)
	{
#if 0
		if(rgn->rgnix==0)
		{
			if(!rgn->live_entity)
			{
				ntmp=BCCX_NewCst(&bccx_mobj, "mobj");
				BCCX_SetCst(ntmp,
					&bccx_classname, "classname", "chicken");
				BCCX_SetFloatCst(ntmp,
					&bccx_org_x, "org_x", 127);
				BCCX_SetFloatCst(ntmp,
					&bccx_org_y, "org_y", 127);
				BCCX_SetFloatCst(ntmp,
					&bccx_org_z, "org_z", 96);

				BTM_SpawnRegionEntity(wrl, rgn, ntmp);
			}
		}
#endif
	
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
		
		if(d>=256)
//		if(d>=384)
		{
			rgn=rgn->next;
			continue;
		}

		mob=rgn->live_entity;
		while(mob)
		{
			cx=(mob->org_x>>8)&65535;
			cy=(mob->org_y>>8)&65535;

			dx=(s16)(vx-cx);
			dy=(s16)(vy-cy);
			dx=dx^(dx>>31);
			dy=dy^(dy>>31);
			d=dx+dy;
			
			if(d>80)
//			if(d>BTM_TARGET_DRAWDIST)
			{
				mob=mob->next;
				continue;
			}

//			BTMGL_DrawEntity(wrl, mob);
			if(mob->Draw)
				mob->Draw(wrl, mob);
			mob=mob->next;
		}


//		BTM_BlockTickRegion(wrl, rgn);
		rgn=rgn->next;
	}

	BTM_LockSprites();

	oldspr=btm_vissprites;
	btm_vissprites=btm_buildsprites;
	btm_buildsprites=NULL;

	curspr=oldspr;
	while(curspr)
	{
		nxtspr=curspr->next;
		BTM_FreeSprite(curspr);
		curspr=nxtspr;
	}

	BTM_UnlockSprites();

	BTMGL_UnlockWorld();

	return(0);
}

int BTM_CalcRayXyzDistApprox(int dx, int dy, int dz)
{
	int adx, ady, adz, ad;
	int ada, adb, adc;

	adx=dx^(dx>>31);
	ady=dy^(dy>>31);
	adz=dz^(dz>>31);
	ad=adx+ady+adz;
	
	if(adx>ady)
	{
		if(adx>adz)
		{
			ada=adx;
			if(ady>adz)		{ adb=ady; adc=adz; }
			else			{ adb=adz; adc=ady; }
		}else				{ ada=adz; adb=adx; adc=ady; }
	}else
	{
		if(ady>adz)
		{
			ada=ady;
			if(adx>adz)		{ adb=adx; adc=adz; }
			else			{ adb=adz; adc=adx; }
		}else				{ ada=adz; adb=ady; adc=adx; }
	}
	
	ad=ada+(adb>>1)+(adc>>2);
	
	return(ad);
}

int BTM_CalcRayDistApprox(u64 spos, u64 epos)
{
	u64 step;
	int sx, sy, sz, ex, ey, ez, dx, dy, dz, d;
	
	sx=(spos>> 0)&0x00FFFFFFU;
	sy=(spos>>24)&0x00FFFFFFU;
	sz=(spos>>48)&0x0000FFFFU;
	
	ex=(epos>> 0)&0x00FFFFFFU;
	ey=(epos>>24)&0x00FFFFFFU;
	ez=(epos>>48)&0x0000FFFFU;
	
	sx=((s32)(sx<<8))>>8;
	sy=((s32)(sy<<8))>>8;
	
	ex=((s32)(ex<<8))>>8;
	ey=((s32)(ey<<8))>>8;
	
	dx=ex-sx;
	dy=ey-sy;
	dz=ez-sz;
	
	if(dx>( 0x800000))
		dx-=0x1000000;
	if(dx<(-0x800000))
		dx+=0x1000000;

	if(dy>( 0x800000))
		dy-=0x1000000;
	if(dy<(-0x800000))
		dy+=0x1000000;
		
	
	d=BTM_CalcRayXyzDistApprox(dx, dy, dz);
	return(d);
}

u64 BTM_CalcRayStepVector(u64 spos, u64 epos)
{
	u64 step;
	int sx, sy, sz, ex, ey, ez, dx, dy, dz, d;
	
	sx=(spos>> 0)&0x00FFFFFFU;
	sy=(spos>>24)&0x00FFFFFFU;
	sz=(spos>>48)&0x0000FFFFU;
	
	ex=(epos>> 0)&0x00FFFFFFU;
	ey=(epos>>24)&0x00FFFFFFU;
	ez=(epos>>48)&0x0000FFFFU;
	
	sx=((s32)(sx<<8))>>8;
	sy=((s32)(sy<<8))>>8;
	
	ex=((s32)(ex<<8))>>8;
	ey=((s32)(ey<<8))>>8;
	
	dx=ex-sx;
	dy=ey-sy;
	dz=ez-sz;
	
	if(dx>( 0x800000))
		dx-=0x1000000;
	if(dx<(-0x800000))
		dx+=0x1000000;

	if(dy>( 0x800000))
		dy-=0x1000000;
	if(dy<(-0x800000))
		dy+=0x1000000;
		
	
	d=BTM_CalcRayXyzDistApprox(dx, dy, dz);
	while(d>256)
	{
		dx=dx>>1;
		dy=dy>>1;
		dz=dz>>1;
		d=BTM_CalcRayXyzDistApprox(dx, dy, dz);
	}
	
	step=
		((dx&0x00FFFFFFULL)<< 0) |
		((dy&0x00FFFFFFULL)<<24) |
		((dz&0x0000FFFFULL)<<48) ;
	return(step);
}

#if 0
int BTM_CalcRayRelativeApproach(u64 svel, u64 evel)
{
	u64 step;
	int sx, sy, sz, ex, ey, ez, dx, dy, dz, d;
	
	sx=(svel>> 0)&0x00FFFFFFU;
	sy=(svel>>24)&0x00FFFFFFU;
	sz=(svel>>48)&0x0000FFFFU;
	
	ex=(evel>> 0)&0x00FFFFFFU;
	ey=(evel>>24)&0x00FFFFFFU;
	ez=(evel>>48)&0x0000FFFFU;
	
	sx=((s32)(sx<<8))>>8;
	sy=((s32)(sy<<8))>>8;
	
	ex=((s32)(ex<<8))>>8;
	ey=((s32)(ey<<8))>>8;
	
	dx=ex-sx;
	dy=ey-sy;
	dz=ez-sz;		
	
//	d=BTM_CalcRayXyzDistApprox(dx, dy, dz);
	return(d);
}
#endif

BTM_MobEntity *BTM_QueryWorldEntitiesAtPos(
	BTM_World *wrl, BTM_MobEntity *slst, u64 spos)
{
	u64 abpos[8];
	BTM_Region *rgn;
	BTM_MobEntity *mob, *mlst;
	u64 blk, clpos, rcix, rcix2, rlcix, bpos;
	int rix, cix;
	int cx, cy, cz, sx, sy, sz, h;
	int i;
	
	mlst=slst;

	rcix=BTM_WorldCorgToRcix(spos);
	rix=BTM_Rcix2Rix(rcix);
	cix=BTM_Rcix2Cix(rcix);

	rgn=BTM_GetRegionForRix(wrl, rix);
//	BTM_GenerateBaseRegion(wrl, rgn);
//	rgix=rix;

//	bpos=BTM_ConvCorgToBlkPos(spos);

	sx=(spos>> 8)&65535;
	sy=(spos>>32)&65535;
	sz=(spos>>56)&255;

//	bpos=BTM_ConvRcixToBlkPos(rcix);
	abpos[0]=(((u64)(sz+0))<<32)|(((u64)(sy+0))<<16)|(((u64)(sx+0))<<0);
	abpos[1]=(((u64)(sz-1))<<32)|(((u64)(sy+0))<<16)|(((u64)(sx+0))<<0);
	abpos[2]=(((u64)(sz+1))<<32)|(((u64)(sy+0))<<16)|(((u64)(sx+0))<<0);
	abpos[3]=(((u64)(sz+0))<<32)|(((u64)(sy-1))<<16)|(((u64)(sx+0))<<0);
	abpos[4]=(((u64)(sz+0))<<32)|(((u64)(sy+1))<<16)|(((u64)(sx+0))<<0);
	abpos[5]=(((u64)(sz+0))<<32)|(((u64)(sy+0))<<16)|(((u64)(sx-1))<<0);
	abpos[6]=(((u64)(sz+0))<<32)|(((u64)(sy+0))<<16)|(((u64)(sx+1))<<0);

	for(i=0; i<7; i++)
	{
		bpos=abpos[i];
		h=BTM_HashForBlkPos(bpos);
		mob=rgn->live_entity_hash[h];
		while(mob)
		{
			if(mob->bpos==bpos)
			{
				if(!mob->chn_bpos)
				{
					mob->chn_bpos=mlst;
					mlst=mob;
				}
			}
			mob=mob->nxt_bpos;
		}
	}

	return(mlst);
}

BTM_MobEntity *BTM_QueryWorldEntitiesForRay(
	BTM_World *wrl, u64 spos, u64 epos)
{
	BTM_Region *rgn;
	BTM_MobEntity *mob, *mlst;
	u64 cpos, step;
	int dist, n;

	if(!spos || !epos)
		return(NULL);

	step=BTM_CalcRayStepVector(spos, epos);
	dist=BTM_CalcRayDistApprox(spos, epos);
	
	mlst=NULL;
	cpos=spos; n=((dist+255)>>8)+2;
	while((n--)>0)
	{
		mlst=BTM_QueryWorldEntitiesAtPos(wrl, mlst, cpos);
		cpos+=step;
	}
	return(mlst);
}

int BTM_QueryUnlinkMobChains(
	BTM_World *wrl, BTM_MobEntity *smob)
{
	BTM_MobEntity *cmob, *nmob;
	
	cmob=smob;
	while(cmob)
	{
		nmob=cmob->chain;
		cmob->chain=NULL;
		cmob=nmob;
	}

	cmob=smob;
	while(cmob)
	{
		nmob=cmob->chn_bpos;
		cmob->chn_bpos=NULL;
		cmob=nmob;
	}
	return(0);
}

BTM_MobEntity *BTM_QueryWorldEntityForRay(
	BTM_World *wrl, u64 spos, u64 epos)
{
	BTM_Region *rgn;
	BTM_MobEntity *mob, *mlst;
	u64 cpos, step;
	int dist, n;

	if(!spos || !epos)
		return(NULL);

	step=BTM_CalcRayStepVector(spos, epos);
	dist=BTM_CalcRayDistApprox(spos, epos);
	
	mlst=NULL;
	cpos=spos; n=((dist+255)>>8)+2;
	while((n--)>0)
	{
		mlst=BTM_QueryWorldEntitiesAtPos(wrl, mlst, cpos);
		if(mlst)
			break;
		cpos+=step;
	}
	
	BTM_QueryUnlinkMobChains(wrl, mlst);
	return(mlst);
}

int BTM_EventPlayerUseMob(
	BTM_World *wrl, BTM_MobEntity *mob)
{
	int i, j;

	wrl->tgen_varstk_pos=0;	
	for(i=0; i<mob->mobvar_cnt; i++)
	{
		j=wrl->tgen_varstk_pos++;
		wrl->tgen_varstk_name[j]=mob->mobvar_name[i];
		wrl->tgen_varstk_val[j]=mob->mobvar_val[i];
	}

	if(!strcmp(mob->cname, "pig"))
		BTM_ShowMenu("main", "use_pig");
	if(!strcmp(mob->cname, "cow"))
		BTM_ShowMenu("main", "use_cow");
	if(!strcmp(mob->cname, "chicken"))
		BTM_ShowMenu("main", "use_chicken");
	if(!strcmp(mob->cname, "sheep"))
		BTM_ShowMenu("main", "use_sheep");
	return(0);
}
