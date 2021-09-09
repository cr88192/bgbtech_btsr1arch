/*
Redesigned Raycaster
 */

/*
Raycast diverging line.
When the coordinates fall into different blocks, split the rays.
 */

u64 BTM_RaycastStepVector(int yang, int pang);
u64 BTM_RaycastStepVectorB(int yang, int pang);

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

int BTM_RaycastTryAddHitCix1(BTM_World *wrl, int cix, int nrcnt)
{
	u64 blk;
	int *lst;
	short *chn;
	short *hash;
	byte *rcnt;
	int msk, bt, fm;

	int i, j, k, n, h;

	msk=(1<<(wrl->xsh+wrl->xsh+wrl->zsh))-1;
	cix&=msk;

	blk=wrl->vox[cix];
	bt=blk&0xFF;
	fm=(blk>>24)&63;
	if(bt<2)
		return(0);
	if(fm==63)
		return(0);

	h=((cix*(251*65521))>>24)&63;

	if(nrcnt>30)
	{
		if(wrl->scr_cxpred[h]==cix)
			return(0);
		wrl->scr_cxpred[h]=cix;
	}

	lst=wrl->scr_pts_list;
	chn=wrl->scr_pts_chn;
	hash=wrl->scr_pts_hash;
	rcnt=wrl->scr_pts_rcnt;

	i=hash[h];
	while(i>=0)
	{
		if(lst[i]==cix)
		{
			rcnt[i]=nrcnt;
			return(0);
		}
		i=chn[i];
	}
	
	if(wrl->scr_npts>=16383)
		return(0);
	
	i=wrl->scr_npts++;
	lst[i]=cix;
	chn[i]=hash[h];
	rcnt[i]=nrcnt;
	hash[h]=i;

	return(1);
}

int BTM_RaycastTryAddHitCix(BTM_World *wrl, int cix)
{
	int i, to;
	if(cix==wrl->scr_hpred)
		return(0);
	wrl->scr_hpred=cix;

	to=33;

	i=BTM_RaycastTryAddHitCix1(wrl, cix, to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix+1ULL, to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix-1ULL, to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix+(1ULL<<24), to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix-(1ULL<<24), to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix+(1ULL<<48), to);
	i+=BTM_RaycastTryAddHitCix1(wrl, cix-(1ULL<<48), to);
	return(i);
}


int BTM_RaycastLineSingle(BTM_World *wrl, int max,
	u64 spos, u64 step)
{
	register u64 *voxbm;
	register u64 cpos, cstep;
	u64 blk, clpos;
	int cx, cy, cz, cix;
	int adx, ady, adz;
	int cxm, czm, xsh, zsh;
	int n;
	int i, j, k, h;
	
	xsh=wrl->xsh;
	zsh=wrl->zsh;
	cxm=(1<<xsh)-1;
	czm=(1<<zsh)-1;
	voxbm=wrl->voxbm;
	
	n=max;
	cpos=spos;
	cstep=step;
	
	if((cxm==255) && (czm==63))
	{
#if 1
		while(n>4)
		{
			i=0;
			clpos=cpos;

//			cx=(byte)(cpos>> 8);
//			cy=(byte)(cpos>>32);
//			cz=(cpos>>56)&63;
//			cix=(cz<<16)|(cy<<8)|cx;
			cix=	((cpos>>40)&0x003F0000) |
					((cpos>>24)&0x0000FF00) |
					((cpos>> 8)&0x000000FF) ;
			blk=voxbm[cix>>6];
//			i|=((blk>>(cix&63))&1);
			i|=(blk>>(cix&63));
			cpos+=cstep;

//			cx=(byte)(cpos>> 8);
//			cy=(byte)(cpos>>32);
//			cz=(cpos>>56)&63;
//			cix=(cz<<16)|(cy<<8)|cx;
			cix=	((cpos>>40)&0x003F0000) |
					((cpos>>24)&0x0000FF00) |
					((cpos>> 8)&0x000000FF) ;
			blk=voxbm[cix>>6];
			i|=(blk>>(cix&63));
			cpos+=cstep;

//			cx=(byte)(cpos>> 8);
//			cy=(byte)(cpos>>32);
//			cz=(cpos>>56)&63;
//			cix=(cz<<16)|(cy<<8)|cx;
			cix=	((cpos>>40)&0x003F0000) |
					((cpos>>24)&0x0000FF00) |
					((cpos>> 8)&0x000000FF) ;
			blk=voxbm[cix>>6];
			i|=(blk>>(cix&63));
			cpos+=cstep;

//			cx=(byte)(cpos>> 8);
//			cy=(byte)(cpos>>32);
//			cz=(cpos>>56)&63;
//			cix=(cz<<16)|(cy<<8)|cx;
			cix=	((cpos>>40)&0x003F0000) |
					((cpos>>24)&0x0000FF00) |
					((cpos>> 8)&0x000000FF) ;
			blk=voxbm[cix>>6];
			i|=(blk>>(cix&63));
			cpos+=cstep;

			if(i&1)
				{ cpos=clpos; break; }

			n-=4;
		}
#endif

		while((n--)>0)
		{
//			cx=(cpos>> 8)&255;
//			cy=(cpos>>32)&255;
			cx=(byte)(cpos>> 8);
			cy=(byte)(cpos>>32);
			cz=(cpos>>56)&63;
			cix=(cz<<16)|(cy<<8)|cx;
			blk=voxbm[cix>>6];
			if((blk>>(cix&63))&1)
				break;

			cpos+=cstep;
		}
	}else
	{
		while((n--)>0)
		{
			cx=(cpos>> 8)&cxm;
			cy=(cpos>>32)&cxm;
			cz=(cpos>>56)&czm;
			cix=(((cz<<xsh)|cy)<<xsh)|cx;
			
			blk=voxbm[cix>>6];
	//		if(blk&(1ULL<<(cix&63)))
			if((blk>>(cix&63))&1)
				break;

			cpos+=step;
		}
	}

	if(n<=0)
		return(0);
	
	i=BTM_RaycastTryAddHitCix(wrl, cix);
	return(i);
}

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
	
	xsh=wrl->xsh;
	zsh=wrl->zsh;
	cxm=(1<<xsh)-1;
	czm=(1<<zsh)-1;
	voxbm=wrl->voxbm;
	
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


int BTM_RaycastSceneQuad(BTM_World *wrl)
{
//	short	*
	u64		corg, step_fw;
	u64		step0, step1, step2, step3;
	int		x, y, yaw, pitch, yji, pji;
	int		y0_ang, p0_ang;
	int		y1_ang, p1_ang;
	int		i, j, k, h, n;

	BTM_RaycastInitTables();

	for(i=0; i<64; i++)
	{
		wrl->scr_pts_hash[i]=-1;
		wrl->scr_cxpred[i]=-1;
	}
	n=wrl->scr_npts;
	wrl->scr_npts=0;
	
	for(i=0; i<n; i++)
	{
		j=wrl->scr_pts_rcnt[i];
		if(j<=0)
			continue;
		k=wrl->scr_pts_list[i];
		BTM_RaycastTryAddHitCix1(wrl, k, j-1);
	}
	
	wrl->frame++;

	corg=wrl->cam_org;
	yaw=wrl->cam_yaw;
	pitch=wrl->cam_pitch;

//	for(y=0; y<48; y++)
//		for(x=0; x<64; x++)

	step_fw=BTM_RaycastStepVector(yaw, pitch);
	
//	yji=(wrl->frame&7)-3;

//	pji=((wrl->frame>>0)&7)-3;
//	yji=((wrl->frame>>3)&7)-3;
//	yji=((wrl->frame>>3)&15)-7;

	pji=((wrl->frame>>0)&15)-7;
	yji=((wrl->frame>>1)&15)-7;

#if 0
	for(y=0; y<32; y++)
		for(x=0; x<48; x++)
//	for(y=0; y<24; y++)
//		for(x=0; x<32; x++)
//	for(y=0; y<12; y++)
//		for(x=0; x<16; x++)
	{
//		y0_ang=(yaw+((x-20)*3))&255;
//		y1_ang=(yaw+((x-19)*3))&255;

		y0_ang=(yaw+((x-24)*3))&255;
//		y1_ang=(yaw+((x-23)*3))&255;
//		y1_ang=(y0_ang+2)&255;
		y1_ang=(y0_ang+4)&255;
		
		y0_ang=(y0_ang+yji)&255;
		y1_ang=(y1_ang+yji)&255;

//		y0_ang=(yaw+((x-32)*2))&255;
//		y1_ang=(yaw+((x-31)*2))&255;

//		y0_ang=(yaw+((x-16)*3))&255;
//		y1_ang=(yaw+((x-15)*3))&255;
//		p0_ang=(pitch+((y-12)*4))&255;
//		p1_ang=(pitch+((y-11)*4))&255;

		p0_ang=(pitch+((y-16)*3))&255;
//		p1_ang=(pitch+((y-15)*3))&255;
		p1_ang=(p0_ang+4)&255;

		p0_ang=(p0_ang+yji)&255;
		p1_ang=(p1_ang+yji)&255;

//		p0_ang=(pitch+((y-24)*2))&255;
//		p1_ang=(pitch+((y-23)*2))&255;

//		y0_ang=(yaw+((x-8)*4))&255;
//		y1_ang=(yaw+((x-7)*4))&255;
//		p0_ang=(pitch+((y-6)*4))&255;
//		p1_ang=(pitch+((y-5)*4))&255;

//		y0_ang=(yaw+((x-8)*6))&255;
//		y1_ang=(yaw+((x-7)*6))&255;
//		p0_ang=(pitch+((y-6)*6))&255;
//		p1_ang=(pitch+((y-5)*6))&255;

		step0=BTM_RaycastStepVector(y0_ang, p0_ang);
		step1=BTM_RaycastStepVector(y1_ang, p0_ang);
		step2=BTM_RaycastStepVector(y0_ang, p1_ang);
		step3=BTM_RaycastStepVector(y1_ang, p1_ang);

//		step0=btm_step_ydir[y0_ang];
//		step1=btm_step_ydir[y1_ang];
//		step2=btm_step_ydir[y0_ang];
//		step3=btm_step_ydir[y1_ang];

//		step0+=btm_step_pdir[p0_ang];
//		step1+=btm_step_pdir[p0_ang];
//		step2+=btm_step_pdir[p1_ang];
//		step3+=btm_step_pdir[p1_ang];
		
		BTM_RaycastLineQuad(wrl, 32, 4,
//			corg, corg, corg, corg,
			corg+(step_fw-step0),
			corg+(step_fw-step1),
			corg+(step_fw-step2),
			corg+(step_fw-step3),
			step0, step1, step2, step3);
	}
#endif

#if 1
//	for(y=0; y<80; y++)
//		for(x=0; x<112; x++)
//	for(y=0; y<64; y++)
//		for(x=0; x<96; x++)
//	for(y=0; y<64; y++)
//		for(x=0; x<80; x++)
//	for(y=0; y<40; y++)
//		for(x=0; x<56; x++)
//	for(y=0; y<36; y++)
//		for(x=0; x<48; x++)
//	for(y=0; y<32; y++)
//		for(x=0; x<42; x++)
	for(y=0; y<28; y++)
		for(x=0; x<36; x++)
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

#if 0
		y0_ang=yaw*4+((x-40)*4);
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-32)*4);
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
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-18)*6);
		p0_ang=p0_ang+yji;
#endif

#if 0
		y0_ang=yaw*4+((x-21)*7);
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-16)*7);
		p0_ang=p0_ang+yji;
#endif

#if 1
		y0_ang=yaw*4+((x-18)*8);
		y0_ang=y0_ang+yji;

		p0_ang=pitch*4+((y-14)*8);
		p0_ang=p0_ang+yji;
#endif

		if(p0_ang>256)
			continue;
		if(p0_ang<(-256))
			continue;

//		step0=BTM_RaycastStepVector(y0_ang, p0_ang);
		step0=BTM_RaycastStepVectorB(y0_ang, p0_ang);
		
//		BTM_RaycastLineSingle(wrl, 28,
		BTM_RaycastLineSingle(wrl, 24,
//			corg,
			corg+(step_fw-step0),
//			corg-(step_fw+step0),
			step0);
	}
#endif
	
	return(0);
}
