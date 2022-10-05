u32	btmra_rcptab[512];
// s32 btmra_cosang[256];
s32 btmra_sincosang[256+64];
s32 *btmra_sinang;
s32 *btmra_cosang;

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
	}
	
	btmra_sinang=btmra_sincosang+ 0;
	btmra_cosang=btmra_sincosang+64;

	return(0);
}

u32 BTMRA_SoftRcpU(u32 a)
{
	int ix, sh;
	if(a<512)
		return(btmra_rcptab[a]);
	ix=a; sh=0;
	while(ix>=512)
		{ ix>>=1; sh++; }
	return(btmra_rcptab[ix]>>sh);
}

s32 BTMRA_SoftRcpS(s32 a)
{
	if(a<0)
		return(-BTMRA_SoftRcpU(-a));
	return(BTMRA_SoftRcpU(a));
}

u32 BTMRA_SoftDivU(u32 a, u32 b)
{
	u32 rb, c;
	rb=BTMRA_SoftRcpU(b);
	c=(((u64)a)*((u64)rb))>>32;
	return(c);
}

s32 BTMRA_SoftDivS(s32 a, s32 b)
{
	if(a<0)
	{
		if(b<0)
			return(BTMRA_SoftDivU(-a, -b));
		return(-BTMRA_SoftDivU(-a, b));
	}else
	{
		if(b<0)
			return(-BTMRA_SoftDivU(a, -b));
		return(BTMRA_SoftDivU(a, b));
	}
}

int BTMRA_DrawWall(BTM_Screen *scr, BTM_TexImg *tex,
	u64 relpos, byte relang)
{
	u64 parm[BTMRA_DS_NPARM];
	btmra_rastpixel *scr_ct, *scr_rgb;
	int rcpy, rcpy_lf, rcpy_rt, cosa, sina;
	int rx, ry, rz, ra, scrh, rz_lf, rz_rt;
	int ch, cw, ch_lf, ch_rt, ch_c, ch_s;
	int cx, cy, x, y, w, h, sxs, sys;
	int cy_lf, cy_rt, cy_s;
	int ctex_cs, ctex_ss, ctex_istep;
	int ctex_cts, ctex_sts, ctex_ts_lf, ctex_ts_rt;
	int ctex_s0, ctex_t0;
	int i, j, k;

	rx=(relpos>> 0)&0xFFFFFF;
	ry=(relpos>>24)&0xFFFFFF;
	rz=(relpos>>48)&0xFFFF;
	rx=(rx<<8)>>8;
	ry=(ry<<8)>>8;
	rz=(s16)rz;
	ra=relang;
	
	rcpy=BTMRA_SoftRcpS(ry);
	rx=btmra_mulhi8(rx, rcpy);
//	rz=btmra_mulhi8(rz, rcpy);
	
//	rx=BTMRA_SoftDivS(rx, ry);
//	rz=BTMRA_SoftDivS(rz, ry);
	
	k=scr->fov_angbi;
	i=rx+k;
	if((i<0) || (i>(k*2)))
		return(0);
	ra+=scr->fov_ang[i];
	ra=ra&255;
	
	cosa=btmra_cosang[ra];
//	sina=btmra_cosang[(ra-64)&255];
	sina=btmra_sinang[ra];

	i=sina>>24;
//	rcpy_lf = BTMRA_SoftRcpS(ry+i);
//	rcpy_rt = BTMRA_SoftRcpS(ry-i);

	rcpy_lf = BTMRA_SoftRcpS(ry-i);
	rcpy_rt = BTMRA_SoftRcpS(ry+i);
	
	sxs=scr->xs;
	sys=scr->ys;
	scrh=sys<<8;
	ch=btmra_mulhi8(scrh, rcpy);			//height, center
	ch_lf=btmra_mulhi8(scrh, rcpy_lf);		//height, left
	ch_rt=btmra_mulhi8(scrh, rcpy_rt);		//height, right
	cw=btmra_mulhi1(ch, cosa);				//width (fixed.8)
	
	rz_lf=btmra_mulhi8(rz, rcpy_lf);
	rz_rt=btmra_mulhi8(rz, rcpy_rt);
	
//	cx=btmra_mulhi8(rx, scrh);		//center X (screen, fixed.8)
//	cy=btmra_mulhi8(rz, scrh);		//center Y (screen, fixed.8)

	cx=btmra_mullo8(rx, scrh);		//center X (screen, fixed.8)
//	cy=btmra_mullo8(rz, scrh);		//center Y (screen, fixed.8)

	cy_lf=btmra_mullo8(rz_lf, scrh);	//center Y (screen, fixed.8)
	cy_rt=btmra_mullo8(rz_rt, scrh);	//center Y (screen, fixed.8)

	cx+=sxs<<7;
//	cy+=sys<<7;

	cy_lf+=sys<<7;
	cy_rt+=sys<<7;

	w=cw>>8;
//	w=(cw+127)>>8;
	ch_s=BTMRA_SoftDivS(ch_rt-ch_lf, w);
	cy_s=BTMRA_SoftDivS(cy_rt-cy_lf, w);
	ch_c=ch_lf;
	cy=cy_lf;
	
	ctex_cs=0;
	ctex_istep=1<<(tex->sz_lsh+16);
	ctex_ss=BTMRA_SoftDivS(ctex_istep, w);

	ctex_ts_lf=BTMRA_SoftDivS(ctex_istep, ch_lf>>8);
	ctex_ts_rt=BTMRA_SoftDivS(ctex_istep, ch_rt>>8);
	ctex_cts=ctex_ts_lf;
	ctex_sts=BTMRA_SoftDivS(ctex_ts_rt-ctex_ts_lf, w);

	parm[BTMRA_DS_CTX]=(u64)scr;
	parm[BTMRA_DS_TEXIMG]=(u64)(tex->rgb);
	parm[BTMRA_DS_YMASK]=(1<<(tex->sz_lsh*2))-1;

	parm[BTMRA_DS_CPOS]=0xFF80FF80FF80FF80ULL;
	parm[BTMRA_DS_CSTEP]=0;

	scr_rgb=scr->rgb;

	x=(cx>>8)-(w>>1);
	for(i=0; i<w; i++)
	{
		h=ch_c>>8;
		y=(cy>>8)-(h>>1);
		
		if(x<0)
			goto skip_col;
		if(x>=sxs)
			break;
		
		ctex_s0=ctex_cs;
		ctex_t0=0;
		
		if(y<0)
		{
			if((y+h)<=0)
				goto skip_col;
			ctex_t0+=(-y)*ctex_cts;
			h+=y;
			y=0;
		}
		
		if((y+h)>sys)
		{
			h=sys-y;
			if(h<=0)
				goto skip_col;
		}

		parm[BTMRA_DS_TPOS]=(((u64)ctex_t0)<<32)|((u64)ctex_s0);
		parm[BTMRA_DS_TSTEP]=(((u64)ctex_cts)<<32);
	
		scr_ct=scr_rgb+(y*sxs)+x;
		BTMRA_DrawCol_ModTexMort(parm, scr_ct, NULL, h, sxs);
		
		skip_col:
		x++;
		ctex_cts+=ctex_sts;
		ctex_cs+=ctex_ss;
		ch_c+=ch_s;
		cy+=cy_s;
	}
	
	return(0);
}

int BTMRA_DrawFloor(BTM_Screen *scr, BTM_TexImg *tex,
	u64 relpos, byte relang)
{
	u64 parm[BTMRA_DS_NPARM];
	btmra_rastpixel *scr_ct, *scr_rgb;
	int rcpy, rcpy_lf, rcpy_rt, cosa, sina;
	int rx, ry, rz, ra, scrh, rz_lf, rz_rt;
	int ch, cw, ch_lf, ch_rt, ch_c, ch_s;
	int cx, cy, x, y, w, h, sxs, sys;
	int cy_lf, cy_rt, cy_s;
	int ctex_cs, ctex_ss, ctex_istep;
	int ctex_cts, ctex_sts, ctex_ts_lf, ctex_ts_rt;
	int ctex_s0, ctex_t0;
	int i, j, k;

	rx=(relpos>> 0)&0xFFFFFF;
	ry=(relpos>>24)&0xFFFFFF;
	rz=(relpos>>48)&0xFFFF;
	rx=(rx<<8)>>8;
	ry=(ry<<8)>>8;
	rz=(s16)rz;
	ra=relang;
	
	rcpy=BTMRA_SoftRcpS(ry);
	rx=btmra_mulhi8(rx, rcpy);
	
	k=scr->fov_angbi;
	i=rx+k;
	if((i<0) || (i>(k*2)))
		return(0);
	ra+=scr->fov_ang[i];
	ra=ra&255;
	
	cosa=btmra_cosang[ra];
	sina=btmra_sinang[ra];

	i=sina>>24;

}

int BTMRA_DrawBox(BTM_Screen *scr, BTM_TexImg *tex,
	u64 relpos, byte relang, byte mask)
{
	int cosa, sina, nsina, ra;
	u64 rpx, rpy;

	ra=relang&255;
	cosa=btmra_cosang[ra];
	sina=btmra_sinang[ra];
	cosa=cosa>>24;
	sina=sina>>24;
	
	cosa&=0x00FFFFFF;
	sina&=0x00FFFFFF;
	nsina=(~sina)&0x00FFFFFF;
	
	rpx=(((u64)sina)<<24) | ((u64)cosa);
	rpy=(((u64)cosa)<<24) | ((u64)nsina);

//	rpx=(((u64)nsina)<<24) | ((u64)cosa);
//	rpy=(((u64)cosa )<<24) | ((u64)sina);

	if(mask&0x01)
		BTMRA_DrawWall(scr, tex, relpos-rpx, relang- 64);
	if(mask&0x02)
		BTMRA_DrawWall(scr, tex, relpos+rpx, relang+ 64);
	if(mask&0x04)
		BTMRA_DrawWall(scr, tex, relpos-rpy, relang+  0);
	if(mask&0x08)
		BTMRA_DrawWall(scr, tex, relpos+rpy, relang+128);
}

BTM_Screen *BTMRA_AllocScreen(int xs, int ys)
{
	BTM_Screen *tmp;
	int sz_fovang, ang_bi;
	double f, g, fpx;
	int i, j, k;
	
	tmp=malloc(sizeof(BTM_Screen));
	memset(tmp, 0, sizeof(BTM_Screen));
	
	tmp->rgb=malloc(xs*ys*sizeof(btmra_rastpixel));
	tmp->zb=malloc(xs*ys*sizeof(btmra_zbufpixel));
	tmp->xs=xs;
	tmp->ys=ys;
	
	sz_fovang=xs*3;
	ang_bi=sz_fovang/2;
	tmp->fov_ang=malloc(sz_fovang);
	tmp->fov_angbi=ang_bi;
	
	fpx=(64.0/xs);
	for(i=0; i<sz_fovang; i++)
	{
		j=i-(sz_fovang/2);
		tmp->fov_ang[i]=j*fpx;
	}
	
	return(tmp);
}

BTM_TexImg *BTMRA_AllocTexture(int xsh)
{
	BTM_TexImg *tmp;
	int xs;

	tmp=malloc(sizeof(BTM_TexImg));
	memset(tmp, 0, sizeof(BTM_TexImg));
	
	xs=1<<xsh;
	tmp->rgb=malloc(xs*xs*sizeof(btmra_rastpixel));
	tmp->sz_lsh=xsh;
	
	return(tmp);
}

int BTMRA_DrawSky(BTM_Screen *scr, BTM_TexImg *tex, byte relang)
{
	u64 parm[BTMRA_DS_NPARM];
	btmra_rastpixel *scr_ct, *scr_rgb;
	u32 ctex_t0, ctex_s0, ctex_sstep;
	int y, x, sxs;
	int i, j, k;

	sxs=scr->xs;
	parm[BTMRA_DS_CTX]=(u64)scr;
	parm[BTMRA_DS_TEXIMG]=(u64)(tex->rgb);
	parm[BTMRA_DS_YMASK]=(1<<(tex->sz_lsh*2))-1;

	parm[BTMRA_DS_CPOS]=0xFF80FF80FF80FF80ULL;
	parm[BTMRA_DS_CSTEP]=0;

	scr_rgb=scr->rgb;
	ctex_s0=(relang<<16);
	ctex_sstep=(256<<16)/320;

	for(y=0; y<200; y++)
		for(x=0; x<4; x++)
	{
//		ctex_s0=(relang<<16)+(x*((64<<16)/320));
		ctex_s0=(relang<<16)+(x*(64<<16));
		ctex_t0=(y<<16);
		parm[BTMRA_DS_TPOS]=(((u64)ctex_t0)<<32)|((u64)ctex_s0);
		parm[BTMRA_DS_TSTEP]=ctex_sstep;
	
		scr_ct=scr_rgb+(y*sxs)+(x*80);
		BTMRA_DrawSpan_ModTexMort(parm, scr_ct, NULL, 80);
	}
}
