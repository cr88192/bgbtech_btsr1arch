
void Broom_UnpackTile(byte *tbuf, int tstride, u32 tdat)
{
	int clra, clrb;
	int i;
	
	clra=(tdat>>16)&63;
	clrb=(tdat>>22)&63;

//	clrb=(tdat>>16)&63;
//	clra=(tdat>>22)&63;

#if 0
	i=0;
	tbuf[i+0]=(tdat&0x8000)?clra:clrb;
	tbuf[i+1]=(tdat&0x0800)?clra:clrb;
	tbuf[i+2]=(tdat&0x0080)?clra:clrb;
	tbuf[i+3]=(tdat&0x0008)?clra:clrb;
	
//	tbuf[i+1]=0x0F;

//	__debugbreak();

	i=tstride;
	tbuf[i+0]=(tdat&0x4000)?clra:clrb;
	tbuf[i+1]=(tdat&0x0400)?clra:clrb;
	tbuf[i+2]=(tdat&0x0040)?clra:clrb;
	tbuf[i+3]=(tdat&0x0004)?clra:clrb;

//	tbuf[i+1]=0x0F;

//	__debugbreak();

	i=tstride*2;
	tbuf[i+0]=(tdat&0x2000)?clra:clrb;
	tbuf[i+1]=(tdat&0x0200)?clra:clrb;
	tbuf[i+2]=(tdat&0x0020)?clra:clrb;
	tbuf[i+3]=(tdat&0x0002)?clra:clrb;

//	tbuf[i+1]=0x0F;

//	__debugbreak();

	i=tstride*3;
	tbuf[i+0]=(tdat&0x1000)?clra:clrb;
	tbuf[i+1]=(tdat&0x0100)?clra:clrb;
	tbuf[i+2]=(tdat&0x0010)?clra:clrb;
	tbuf[i+3]=(tdat&0x0001)?clra:clrb;

//	tbuf[i+1]=0x0F;
#endif

#if 1
	i=0;
	tbuf[i++]=(tdat&0x0008)?clra:clrb;
	tbuf[i++]=(tdat&0x0080)?clra:clrb;
	tbuf[i++]=(tdat&0x0800)?clra:clrb;
	tbuf[i  ]=(tdat&0x8000)?clra:clrb;

	i=tstride;
	tbuf[i++]=(tdat&0x0004)?clra:clrb;
	tbuf[i++]=(tdat&0x0040)?clra:clrb;
	tbuf[i++]=(tdat&0x0400)?clra:clrb;
	tbuf[i  ]=(tdat&0x4000)?clra:clrb;

	i=tstride*2;
	tbuf[i++]=(tdat&0x0002)?clra:clrb;
	tbuf[i++]=(tdat&0x0020)?clra:clrb;
	tbuf[i++]=(tdat&0x0200)?clra:clrb;
	tbuf[i  ]=(tdat&0x2000)?clra:clrb;

	i=tstride*3;
	tbuf[i++]=(tdat&0x0001)?clra:clrb;
	tbuf[i++]=(tdat&0x0010)?clra:clrb;
	tbuf[i++]=(tdat&0x0100)?clra:clrb;
	tbuf[i  ]=(tdat&0x1000)?clra:clrb;
#endif

}

void Broom_UnpackSpanN(byte *tbuf, int tstride, u32 *tdat, int txs, int tys)
{
	int i, j, k;
	
	j=0; k=0;
	for(i=0; i<tys; i++)
	{
		Broom_UnpackTile(tbuf+j, tstride, tdat[k]);
		j+=4;
		k+=txs;
	}
}


void Broom_PutPixel(int x, int y, int c)
{
	if((x|y)<0)
		return;
	if(x>=160)
		return;
	if(y>=80)
		return;
	if(c==0x0F)
		return;
	putpixel(x, 79-y, c);
}

void Broom_DrawScaleSpan(int sx, int sy,
	byte *pix, int npix, int scf)
{
	byte *cs;
	int cx, cy;
	int i, j;
	
	if(((u16)sx)>=160)
		return;
	
	cy=sy<<8; cs=pix;
	for(i=0; i<npix; i++)
	{
//		j=*cs++;
		j=pix[i];
		Broom_PutPixel(sx, cy>>8, j);
		cy+=scf;
	}
}

byte broom_spanbuf[4*128];
u16 broom_spantex;
byte broom_spancol;
byte broom_spancol_px;


void Broom_DrawScaleSprite(int sx, int sy, int sxcf, int sycf,
	u32 *tdat, int txs, int tys,
	int txo, int tyo,
	int txc, int tyc)
{
	u32 *tdat1;
	int cx, np;
	int i, j;
	
	np=tyc<<2;
	if((sy+np)<=0)
		return;
	if(sy>80)
		return;
	
	tdat1=tdat+(tyo*txs+txo);
//	tdat1=tdat+((tys-tyo-1)*txs+txo);
	cx=sx<<8;
	for(i=0; i<txc; i++)
	{
//		Broom_UnpackSpanN(broom_spanbuf, 64, tdat1+i, -txs, tyc);
		Broom_UnpackSpanN(broom_spanbuf, 128, tdat1+i, txs, tyc);
		
		Broom_DrawScaleSpan(cx>>8, sy, broom_spanbuf    , np, sycf);
		cx+=sxcf;
		Broom_DrawScaleSpan(cx>>8, sy, broom_spanbuf+128, np, sycf);
		cx+=sxcf;
		Broom_DrawScaleSpan(cx>>8, sy, broom_spanbuf+256, np, sycf);
		cx+=sxcf;
		Broom_DrawScaleSpan(cx>>8, sy, broom_spanbuf+384, np, sycf);
		cx+=sxcf;
	}
}

static const short sintab[128]={
0x0000, 0x0647, 0x0C8B, 0x12C7, 0x18F8, 0x1F19, 0x2527, 0x2B1E,
0x30FB, 0x36B9, 0x3C56, 0x41CD, 0x471C, 0x4C3F, 0x5133, 0x55F4,
0x5A81, 0x5ED6, 0x62F1, 0x66CE, 0x6A6C, 0x6DC9, 0x70E1, 0x73B5,
0x7640, 0x7883, 0x7A7C, 0x7C29, 0x7D89, 0x7E9C, 0x7F61, 0x7FD7,
0x7FFF, 0x7FD7, 0x7F61, 0x7E9C, 0x7D89, 0x7C29, 0x7A7C, 0x7883,
0x7640, 0x73B5, 0x70E1, 0x6DC9, 0x6A6C, 0x66CE, 0x62F1, 0x5ED6,
0x5A81, 0x55F4, 0x5133, 0x4C3F, 0x471C, 0x41CD, 0x3C56, 0x36B9,
0x30FB, 0x2B1E, 0x2527, 0x1F19, 0x18F8, 0x12C7, 0x0C8B, 0x0647,
0x0000, 0xF9B9, 0xF375, 0xED39, 0xE708, 0xE0E7, 0xDAD9, 0xD4E2,
0xCF05, 0xC947, 0xC3AA, 0xBE33, 0xB8E4, 0xB3C1, 0xAECD, 0xAA0C,
0xA57F, 0xA12A, 0x9D0F, 0x9932, 0x9594, 0x9237, 0x8F1F, 0x8C4B,
0x89C0, 0x877D, 0x8584, 0x83D7, 0x8277, 0x8164, 0x809F, 0x8029,
0x8001, 0x8029, 0x809F, 0x8164, 0x8277, 0x83D7, 0x8584, 0x877D,
0x89C0, 0x8C4B, 0x8F1F, 0x9237, 0x9594, 0x9932, 0x9D0F, 0xA12A,
0xA57F, 0xAA0C, 0xAECD, 0xB3C1, 0xB8E4, 0xBE33, 0xC3AA, 0xC947,
0xCF05, 0xD4E2, 0xDAD9, 0xE0E7, 0xE708, 0xED39, 0xF375, 0xF9B9
};

int Broom_Sin16(int a)
{
	return(sintab[(a>>2)&127]<<1);
}

int Broom_Cos16(int a)
{
	return(sintab[((a>>2)+32)&127]<<1);
}

void Broom_RotatePoint(int x, int y, int a, int *rx2, int *ry2)
{
	int ss, sc;
	int x1, y1;
	
	ss=Broom_Sin16(a);
	sc=Broom_Cos16(a);
	
//	x1=(x*sc)+(y*ss);
//	y1=(x*ss)-(y*sc);
	x1=(x*sc)-(y*ss);
	y1=(x*ss)+(y*sc);

	*rx2=x1>>16;
	*ry2=y1>>16;
}

void Broom_RotatePointOrg(
	int vx, int vy,
	int xo, int yo,
	int a,
	int *rx2, int *ry2)
{
	int x1, y1;
	
	Broom_RotatePoint(vx-xo, vy-yo, a, &x1, &y1);
	*rx2=x1+xo;
	*ry2=y1+yo;
}

void Broom_RotatePointLocal(
	int vx, int vy,
	int xo, int yo,
	int a,
	int *rx2, int *ry2)
{
	int x1, y1;
	
	Broom_RotatePoint(vx-xo, vy-yo, a, &x1, &y1);
	*rx2=x1;
	*ry2=y1;
}

typedef struct Broom_Vertex_s		Broom_Vertex;
typedef struct Broom_LineDef_s		Broom_LineDef;
typedef struct Broom_Sector_s		Broom_Sector;

typedef struct Broom_TempVertex_s	Broom_TempVertex;
typedef struct Broom_Camera_s		Broom_Camera;
typedef struct Broom_TempWall_s	Broom_TempWall;
typedef struct Broom_TexInfo_s		Broom_TexInfo;

struct Broom_Vertex_s
{
s16 x;
s16 y;
};

struct Broom_LineDef_s
{
u16 v0;			//first vertex
u16 v1;			//second vertex
byte texid;		//texture
byte sect;		//sector
};

struct Broom_Sector_s
{
sbyte zbot;				//bottom Z position
sbyte ztop;				//top Z position
};


struct Broom_TempVertex_s
{
s16 x;
s16 y;
u16 s;
u16 sc;
};

struct Broom_Camera_s
{
s16 x;
s16 y;
s16 a;
};

struct Broom_TexInfo_s
{
u32 *dat;
u16 texid;
byte txs, tys;
};

struct Broom_TempWall_s
{
Broom_TexInfo *tex;		//wall texture
sbyte zbot;				//bottom Z position
sbyte ztop;				//top Z position
};

void Broom_RotateVertexLocal(
	Broom_TempVertex *v0,
	Broom_Camera *cam,
	Broom_TempVertex *v1)
{
	int x1, y1;
	
	Broom_RotatePoint(
		v0->x - cam->x,
		v0->y - cam->y,
		cam->a, &x1, &y1);
	v1->x=x1;
	v1->y=y1;
	v1->s=v0->s;

	puts("Broom_RotateVertexLocal\n");
	print_decimal(v0->x);
	puts(" ");
	print_decimal(v0->y);
	puts(" ");
	print_decimal(v1->x);
	puts(" ");
	print_decimal(v1->y);
	puts("\n");
}

int Broom_ScaleForDist(int z)
{
	int sc;

	if(z<=0)
		return(-999);

//	if(z<0)z=-z;
//	sc=1024/((z>>2)+1);
	sc=20480/(z+1);
	return(sc);
}

void Broom_DrawSetupTexSpan(Broom_TexInfo *tex, int s)
{
	int cx, px;
	
	cx=s>>2;
	cx&=(tex->txs-1);

	if((broom_spantex==tex->texid) && (broom_spancol==cx))
		return;

	Broom_UnpackSpanN(broom_spanbuf, 128,
		tex->dat+cx, tex->txs, tex->tys);
	broom_spantex=tex->texid;
	broom_spancol=cx;
	broom_spancol_px=s&3;
}

void Broom_DrawWallSpan(
	Broom_TempWall *twall,
	int x, int s, int sc)
{
	int zb, zt;
	int z0, z1, zd;

	if(sc<0)
		return;

	Broom_DrawSetupTexSpan(twall->tex, s);
	
//	puts("Broom_DrawWallSpan\n");
//	print_decimal(x);
//	puts(" ");
//	print_decimal(s);
//	puts(" ");
//	print_decimal(sc);
//	puts("\n");
	
//	sc=128;
	
	zb=twall->zbot;
	zt=twall->ztop;
	zb-=56;
	zt-=56;
	
	z0=(zb*sc)>>8;
	z1=(zt*sc)>>8;
	zd=z1-z0;

//	print_decimal(z0);
//	puts(" ");
//	print_decimal(z1);
//	puts(" ");
//	print_decimal(zd);
//	puts("\n");
	
	Broom_DrawScaleSpan(x+80, z0+40,
		broom_spanbuf+(broom_spancol_px<<8),
		zd, sc);
}

void Broom_DrawWallLocal(
	Broom_TempVertex *v0, Broom_TempVertex *v1,
	Broom_TempWall *twall,
	Broom_Camera *cam)
{
	Broom_TempVertex tv0a, tv1a, tv2a;
	int sc0, sc1;
	int cx, nx, dx, csc, ssc;
	int css, sss;
	
	Broom_RotateVertexLocal(v0, cam, &tv0a);
	Broom_RotateVertexLocal(v1, cam, &tv1a);
	
	sc0=Broom_ScaleForDist(tv0a.y);
	sc1=Broom_ScaleForDist(tv1a.y);
	
	if((sc0<0) && (sc1<0))
		return;
	
	tv0a.x=(tv0a.x*sc0)>>8;
	tv1a.x=(tv1a.x*sc1)>>8;
	tv0a.sc=sc0;
	tv1a.sc=sc1;
	
	if(tv1a.x<tv0a.x)
		{ tv2a=tv0a; tv0a=tv1a; tv1a=tv2a; }
	
	cx=tv0a.x;	nx=tv1a.x;
	dx=(nx-cx)+1;
	csc=tv0a.sc<<8;
	ssc=((tv1a.sc-tv0a.sc)<<8)/dx;
	css=tv0a.s<<8;
	sss=((tv1a.s-tv0a.s)<<8)/dx;
	for(; cx<nx; cx++)
	{
		Broom_DrawWallSpan(twall, cx, css>>8, csc>>8);
		csc+=ssc;	css+=sss;
	}
}


Broom_Camera broom_cam;

const Broom_TexInfo broom_tex_brick={
	brick1,
	1,
	16, 16
};

const Broom_Vertex broom_wrl_vertices[]={
{-256, -256},
{ 256, -256},
{ 256,  256},
{-256,  256},
{0, 0}
};

const Broom_LineDef broom_wrl_linedef[]={
{0, 1, 1, 1},
{1, 2, 1, 1},
{2, 3, 1, 1},
{3, 0, 1, 1},
{0, 0, 0, 0}
};

const Broom_Sector broom_wrl_sector[]={
{0, 64},
{0, 64},
{0, 0}
};

Broom_TempWall broom_twalla;
Broom_TempVertex broom_tv0a;
Broom_TempVertex broom_tv1a;

void Broom_DrawWorld()
{
	Broom_TempWall *twall;
	Broom_TempVertex *tv0, *tv1;
	Broom_LineDef *ld;
	Broom_Sector *sect;
	int dx, dy;
	int i;
	
	twall=&broom_twalla;
	tv0=&broom_tv0a;
	tv1=&broom_tv1a;
	
	for(i=0;; i++)
	{
		ld=(&broom_wrl_linedef+i);
		if((ld->v0==ld->v1) && !ld->v0)
			break;
		
		sect=(&broom_wrl_sector+ld->sect);
		
		tv0->x=broom_wrl_vertices[ld->v0].x;
		tv0->y=broom_wrl_vertices[ld->v0].y;
		tv1->x=broom_wrl_vertices[ld->v1].x;
		tv1->y=broom_wrl_vertices[ld->v1].y;

		dx=tv1->x-tv0->x;
		dy=tv1->y-tv0->y;
		dx=(dx<0)?(-dx):dx;
		dy=(dy<0)?(-dy):dy;

		if(dx>dy)
		{
			tv0->s=tv0->x;
			tv1->s=tv1->x;
		}else
		{
			tv0->s=tv0->y;
			tv1->s=tv1->y;
		}
		
		twall->tex=&broom_tex_brick;
		twall->zbot=sect->zbot;
		twall->ztop=sect->ztop;
		
		Broom_DrawWallLocal(tv0, tv1, twall, &broom_cam);
	}
}
