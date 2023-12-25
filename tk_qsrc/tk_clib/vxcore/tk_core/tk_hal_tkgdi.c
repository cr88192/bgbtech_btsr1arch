#include <tk_core.h>
#include <tkgdi/tkgdi.h>

int tkgdi_vid_frnum;
int tkgdi_vid_scrmode;
int tkgdi_vid_cellstride;
int tkgdi_vid_rowstride;
byte tkgdi_vid_planar;
byte tkgdi_vid_noutx2;
byte tkgdi_vid_is8bit;

int tkgdi_vid_xsize;
int tkgdi_vid_ysize;

int tkgdi_vid_bmxsize;

u64 *tkgdi_vid_screenutx;		//screen UTX2 buffer
u64 *tkgdi_vid_screenrgb;		//screen RGB buffer
byte *tkgdi_vid_screendirty;		//screen dirtry-mask buffer

static u64 tkgdi_ptrmangle_key=0;

byte tk_img_d9to8tab[512];
byte tk_img_d15to8tab0[32768];
byte tk_img_d15to8tab1[32768];
u16 tk_img_d8to15tab[256];
byte tk_img_d8to15ready;


int tkgdi_blitupdate_getconbuf_sticky;
int tkgdi_blitupdate_getconbuf_sticky_cnt;

TKGDI_EVENTBUF	*tkgdi_freemsg;

/* Existing Windows */
_tkgdi_window_t		*tkgdi_windows[256];
int					tkgdi_n_windows;

/* Visible Windows */
_tkgdi_window_t		*tkgdi_window_vis[256];
int					tkgdi_n_window_vis;

TKPE_TaskInfo *tkgdi_comglue_curtask;

TKGSTATUS TKGDI_DevPushEvent(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_EVENT *imsg);

TKGSTATUS TKGDI_WindowMarkDirtyRect(
	_tkgdi_context_t *ctx,
	_tkgdi_window_t *wctx,
	int xo_src, int yo_src, int xs_src, int ys_src);

TKGDI_EVENTBUF *TKGDI_AllocEventBuf()
{
	TKGDI_EVENTBUF *tmp;
	
	tmp=tkgdi_freemsg;
	if(tmp)
	{
		tkgdi_freemsg=tmp->next;
		tmp->next=NULL;
		return(tmp);
	}
	
	tmp=tk_malloc(sizeof(TKGDI_EVENTBUF));
	memset(tmp, 0, sizeof(TKGDI_EVENTBUF));
	return(tmp);
}

void TKGDI_FreeEventBuf(TKGDI_EVENTBUF *tmp)
{
	tmp->next=tkgdi_freemsg;
	tkgdi_freemsg=tmp;
}

int TKGDI_UpdateWindowCells(
	_tkgdi_window_t *wctx)
{
	u64 *ct, *ct1;
	u16 *cs, *cs1;
	byte *bm1, *bm2;
	int x, y, b, xs, ys, bxs, bys, bmxs;

//	tk_printf("TKGDI_UpdateWindowCells: A\n");

	if(!(wctx->dirty1))
		return(0);

	if(tkgdi_vid_noutx2)
		return(0);

	TKGDI_BlitUpdate_FillEncode2Tab();

//	tk_printf("TKGDI_UpdateWindowCells: B\n");

	xs=wctx->size_x;
	ys=wctx->size_y;
	bxs=wctx->size_bxs;
	bys=wctx->size_bys;
	bm1=wctx->buf_dirty1;
	bm2=wctx->buf_dirty2;
	bmxs=wctx->size_bmxs;
	
	cs=wctx->buf_data;
	ct=wctx->buf_utx2;
	
	for(y=0; y<bys; y++)
	{
		cs1=cs;
//		ct1=ct;
		for(x=0; x<bxs; x++)
		{
			b=bm1[x>>3];
			if(!b)
				{ x=((x+8)&(~7))-1; continue; }

			if(((b>>(x&7))&15)==15)
			{
				TKGDI_BlitUpdate_EncodeCell4xUTX2(cs1, xs, ct+x);
				bm1[x>>3]=b&(~(15<<(x&7)));
				bm2[x>>3]|= (15<<(x&7));
				cs1+=16;
				x+=3;
				continue;
			}

			if(!(b&(1<<(x&7))))
				continue;
			ct[x]=TKGDI_BlitUpdate_EncodeCellUTX2(cs1, xs);
			bm1[x>>3]=b&(~(1<<(x&7)));
			bm2[x>>3]|= (1<<(x&7));
			cs1+=4;
		}
		cs+=xs*4;
		ct+=bxs;
		bm1+=bmxs;
		bm2+=bmxs;
	}
	
	wctx->dirty1=0;
	wctx->dirty2=1;
	return(1);
}

int TKGDI_ScreenMarkDirty(void)
{
	int xs, ys, bxs, bys, bxs2, bys2, bmsz;

	xs=tkgdi_vid_xsize;
	ys=tkgdi_vid_ysize;
	bxs2=(xs+7)>>3;
	bys2=(ys+7)>>3;
	bmsz=(bxs2*bys2+7)>>3;

	memset(tkgdi_vid_screendirty, 0xFF, bmsz);
	
	return(0);
}

int TKGDI_ScreenMarkDirtyRect(int mx, int my, int nx, int ny)
{
	int xs, ys, bxs, bys, bxs2, bys2, bmsz;
	int bmx, bmy, bnx, bny;
	int x, y, z;

	if(!tkgdi_vid_screendirty)
		return(-1);

	bmx=mx>>3;
	bmy=my>>3;
	bnx=(nx+7)>>3;
	bny=(ny+7)>>3;

	xs=tkgdi_vid_xsize;
	ys=tkgdi_vid_ysize;
	bxs2=(xs+7)>>3;
	bys2=(ys+7)>>3;
	bmsz=(bxs2*bys2+7)>>3;

	if(bmx<0)	bmx=0;
	if(bmy<0)	bmy=0;
	if(bnx>=bxs2)	bnx=bxs2-1;
	if(bny>=bys2)	bny=bys2-1;

	if((bmx>bnx) || (bmy>bny))
		return(-1);

	for(y=bmy; y<=bny; y++)
		for(x=bmx; x<=bnx; x++)
	{
		z=y*bxs2+x;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
	}

//	memset(tkgdi_vid_screendirty, 0xFF, bmsz);
	
	return(0);
}

int TKGDI_UpdateWindowStack_CopyFillPattern(
	int x, int y, int bxs,
	u64 bp0, u64 bp1)
{
	u64 *scr;
	int ix;
//	int y2p0, y2p1, x2p0, x2p1;
//	int yxp00, yxp01, yxp10, yxp11;
	
//	y2p0=((y<<1)+0)*bxs;
//	y2p1=((y<<1)+1)*bxs;
//	x2p0=((x<<1)+0);
//	x2p1=((x<<1)+1);
	
//	yxp00=(y2p0+x2p0)*4;
//	yxp01=(y2p0+x2p1)*4;
//	yxp10=(y2p1+x2p0)*4;
//	yxp11=(y2p1+x2p1)*4;
	
	scr=tkgdi_vid_screenrgb;
	
	ix=((y<<3)+0)*bxs+(x<<1);
	scr[ix+0]=bp0;	scr[ix+1]=bp0;

	ix=((y<<3)+1)*bxs+(x<<1);
	scr[ix+0]=bp1;	scr[ix+1]=bp1;

	ix=((y<<3)+2)*bxs+(x<<1);
	scr[ix+0]=bp0;	scr[ix+1]=bp0;

	ix=((y<<3)+3)*bxs+(x<<1);
	scr[ix+0]=bp1;	scr[ix+1]=bp1;

	ix=((y<<3)+4)*bxs+(x<<1);
	scr[ix+0]=bp0;	scr[ix+1]=bp0;

	ix=((y<<3)+5)*bxs+(x<<1);
	scr[ix+0]=bp1;	scr[ix+1]=bp1;

	ix=((y<<3)+6)*bxs+(x<<1);
	scr[ix+0]=bp0;	scr[ix+1]=bp0;

	ix=((y<<3)+7)*bxs+(x<<1);
	scr[ix+0]=bp1;	scr[ix+1]=bp1;

	return(0);
}


int TKGDI_UpdateWindowStack_CopyFillPattern4x(
	int x, int y, int bxs,
	u64 bp0, u64 bp1)
{
	u64 *scr;
	int ix;
	
	scr=tkgdi_vid_screenrgb;
	
	ix=((y<<2)+0)*bxs+(x<<0);
	scr[ix+0]=bp0;	scr[ix+1]=bp0;

	ix=((y<<2)+1)*bxs+(x<<0);
	scr[ix+0]=bp1;	scr[ix+1]=bp1;

	ix=((y<<2)+2)*bxs+(x<<0);
	scr[ix+0]=bp0;	scr[ix+1]=bp0;

	ix=((y<<2)+3)*bxs+(x<<0);
	scr[ix+0]=bp1;	scr[ix+1]=bp1;

	ix=((y<<2)+4)*bxs+(x<<0);
	scr[ix+0]=bp0;	scr[ix+1]=bp0;

	ix=((y<<2)+5)*bxs+(x<<0);
	scr[ix+0]=bp1;	scr[ix+1]=bp1;

	ix=((y<<2)+6)*bxs+(x<<0);
	scr[ix+0]=bp0;	scr[ix+1]=bp0;

	ix=((y<<2)+7)*bxs+(x<<0);
	scr[ix+0]=bp1;	scr[ix+1]=bp1;

	return(0);
}

int TKGDI_UpdateWindowStack_CopyFillFlat(
	int x, int y, int bxs,
	u16 px)
{
	u64 bp;
	bp=px;
	bp|=bp<<16;
	bp|=bp<<32;
	TKGDI_UpdateWindowStack_CopyFillPattern4x(x, y, bxs, bp, bp);

	return(0);
}


int TKGDI_UpdateWindowStack_CopyFillTile(
	int x, int y, int bxs,
	u64 *bpa)
{
	u64 *scr;
	u64 p0, p1, am;
	int ix;
	
	scr=tkgdi_vid_screenrgb;
	am=0x8000800080008000ULL;
	
	ix=((y<<2)+0)*bxs+(x<<0);
	p0=bpa[0];	p1=bpa[1];
	if(!((p0|p1)&am))
	{
		scr[ix+0]=p0;
		scr[ix+1]=p1;
	}

	ix=((y<<2)+1)*bxs+(x<<0);
	p0=bpa[2];	p1=bpa[3];
	if(!((p0|p1)&am))
	{
		scr[ix+0]=p0;
		scr[ix+1]=p1;
	}

	ix=((y<<2)+2)*bxs+(x<<0);
	p0=bpa[4];	p1=bpa[5];
	if(!((p0|p1)&am))
	{
		scr[ix+0]=p0;
		scr[ix+1]=p1;
	}

	ix=((y<<2)+3)*bxs+(x<<0);
	p0=bpa[6];	p1=bpa[7];
	if(!((p0|p1)&am))
	{
		scr[ix+0]=p0;
		scr[ix+1]=p1;
	}

	ix=((y<<2)+4)*bxs+(x<<0);
	p0=bpa[8];	p1=bpa[9];
	if(!((p0|p1)&am))
	{
		scr[ix+0]=p0;
		scr[ix+1]=p1;
	}

	ix=((y<<2)+5)*bxs+(x<<0);
	p0=bpa[10];	p1=bpa[11];
	if(!((p0|p1)&am))
	{
		scr[ix+0]=p0;
		scr[ix+1]=p1;
	}

	ix=((y<<2)+6)*bxs+(x<<0);
	p0=bpa[12];	p1=bpa[13];
	if(!((p0|p1)&am))
	{
		scr[ix+0]=p0;
		scr[ix+1]=p1;
	}

	ix=((y<<2)+7)*bxs+(x<<0);
	p0=bpa[14];	p1=bpa[15];
	if(!((p0|p1)&am))
	{
		scr[ix+0]=p0;
		scr[ix+1]=p1;
	}

	return(0);
}

int TKGDI_UpdateWindowStack_CopyFillTilePx(
	int x, int y, int bxs,
	u64 pxy, u64 pxa, u16 clra, u16 clrb)
{
	static u16 pxt_bpa[64*64];
	static u64 pxt_pxy[64];
	static u64 pxt_pxa[64];
	static u16 pxt_clra[64];
	static u16 pxt_clrb[64];
	u16 *bpa;
	int i, j, k, h;
	
	h=(pxy*31)^(pxy>>31)^pxa^(pxa>>27)^(clra+clrb);
	h=(h*31)^(h>>15);
	h=(h*251)+(h>>8);
	h=((h*251+1)>>8)&63;
	
	bpa=pxt_bpa+h*64;
	
	if(	(pxt_pxy[h]==pxy) &&
		(pxt_pxa[h]==pxa) &&
		(pxt_clra[h]==clra) &&
		(pxt_clrb[h]==clrb))
	{
		TKGDI_UpdateWindowStack_CopyFillTile(x, y, bxs,
			(u64 *)bpa);
		return(0);
	}

	pxt_pxy[h]=pxy;
	pxt_pxa[h]=pxa;
	pxt_clra[h]=clra;
	pxt_clrb[h]=clrb;
	
	for(i=0; i<64; i++)
	{
		bpa[i]=clrb;
		j=63-i;
		if((pxy>>j)&1)
			bpa[i]=clra;
		if((pxa>>j)&1)
			bpa[i]=0x8000;
	}
	
	TKGDI_UpdateWindowStack_CopyFillTile(x, y, bxs,
		(u64 *)bpa);
	return(0);
}

int TKGDI_UpdateWindowStack_CopyFillSource(
	int x, int y, int bxs,
	u64 *src, int sx, int sy, int sxs)
{
	u64 *scr;
	u64 v0, v1, v2, v3;
	int ix, sy4, sx2;
	
	scr=tkgdi_vid_screenrgb;
	
	sy4=sy<<2;
	sx2=sx<<0;

#if 1
	ix=sy4*sxs+sx2;
	v0=src[ix];
	ix+=sxs;
	v1=src[ix];
	ix+=sxs;
	v2=src[ix];
	ix+=sxs;
	v3=src[ix];

//	v0=src[(sy4+0)*sxs+sx2];
//	v1=src[(sy4+1)*sxs+sx2];
//	v2=src[(sy4+2)*sxs+sx2];
//	v3=src[(sy4+3)*sxs+sx2];

//	ix=((y<<2)+0)*bxs+(x<<0);
	ix=(y<<2)*bxs+x;
	scr[ix]=v0;
//	ix=((y<<2)+1)*bxs+(x<<0);
	ix+=bxs;
	scr[ix]=v1;
//	ix=((y<<2)+2)*bxs+(x<<0);
	ix+=bxs;
	scr[ix]=v2;
//	ix=((y<<2)+3)*bxs+(x<<0);
	ix+=bxs;
	scr[ix]=v3;
#endif

	return(0);
}

int TKGDI_UpdateWindowStack_CopyFillSource4x(
	int x, int y, int bxs,
	u64 *src, int sx, int sy, int sxs)
{
	u64 *scr, *cs, *ct;
	u64 v0, v1, v2, v3;
	int ixs, ixt, sy4, sx2;
	
	scr=tkgdi_vid_screenrgb;
	
	sy4=sy<<2;
	sx2=sx<<0;

#if 1
	ixs=sy4*sxs+sx2;
	ixt=(y<<2)*bxs+x;
	
	cs=src+ixs;
	ct=scr+ixt;

	v0=cs[0];	v1=cs[1];
	v2=cs[2];	v3=cs[3];
	ct[0]=v0;	ct[1]=v1;
	ct[2]=v2;	ct[3]=v3;
	cs+=sxs;	ct+=bxs;
	v0=cs[0];	v1=cs[1];
	v2=cs[2];	v3=cs[3];
	ct[0]=v0;	ct[1]=v1;
	ct[2]=v2;	ct[3]=v3;
	cs+=sxs;	ct+=bxs;
	v0=cs[0];	v1=cs[1];
	v2=cs[2];	v3=cs[3];
	ct[0]=v0;	ct[1]=v1;
	ct[2]=v2;	ct[3]=v3;
	cs+=sxs;	ct+=bxs;
	v0=cs[0];	v1=cs[1];
	v2=cs[2];	v3=cs[3];
	ct[0]=v0;	ct[1]=v1;
	ct[2]=v2;	ct[3]=v3;
#endif

	return(0);
}

int TKGDI_UpdateWindowStack_CopyFillSource8x(
	int x, int y, int bxs,
	u64 *src, int sx, int sy, int sxs)
{
	u64 *scr, *cs, *ct;
	u64 v0, v1, v2, v3;
	int ixs, ixt, sy4, sx2;
	
	scr=tkgdi_vid_screenrgb;
	
	sy4=sy<<2;
	sx2=sx<<0;

#if 1
	ixs=sy4*sxs+sx2;
	ixt=(y<<2)*bxs+x;
	
	cs=src+ixs;
	ct=scr+ixt;

	v0=cs[0];	v1=cs[1];
	v2=cs[2];	v3=cs[3];
	ct[0]=v0;	ct[1]=v1;
	ct[2]=v2;	ct[3]=v3;
	v0=cs[4];	v1=cs[5];
	v2=cs[6];	v3=cs[7];
	ct[4]=v0;	ct[5]=v1;
	ct[6]=v2;	ct[7]=v3;

	cs+=sxs;	ct+=bxs;

	v0=cs[0];	v1=cs[1];
	v2=cs[2];	v3=cs[3];
	ct[0]=v0;	ct[1]=v1;
	ct[2]=v2;	ct[3]=v3;
	v0=cs[4];	v1=cs[5];
	v2=cs[6];	v3=cs[7];
	ct[4]=v0;	ct[5]=v1;
	ct[6]=v2;	ct[7]=v3;

	cs+=sxs;	ct+=bxs;

	v0=cs[0];	v1=cs[1];
	v2=cs[2];	v3=cs[3];
	ct[0]=v0;	ct[1]=v1;
	ct[2]=v2;	ct[3]=v3;
	v0=cs[4];	v1=cs[5];
	v2=cs[6];	v3=cs[7];
	ct[4]=v0;	ct[5]=v1;
	ct[6]=v2;	ct[7]=v3;

	cs+=sxs;	ct+=bxs;

	v0=cs[0];	v1=cs[1];
	v2=cs[2];	v3=cs[3];
	ct[0]=v0;	ct[1]=v1;
	ct[2]=v2;	ct[3]=v3;
	v0=cs[4];	v1=cs[5];
	v2=cs[6];	v3=cs[7];
	ct[4]=v0;	ct[5]=v1;
	ct[6]=v2;	ct[7]=v3;
#endif

	return(0);
}

int tkgdi_ps2ms_x;
int tkgdi_ps2ms_y;
int tkgdi_ps2ms_b;
int tkgdi_ps2ms_moved;

int tkgdi_ps2ms_time_dn1;
int tkgdi_ps2ms_time_dn;

int tkgdi_ps2ms_lx;
int tkgdi_ps2ms_ly;
int tkgdi_ps2ms_lb;

int tkgdi_ps2ms_lx1;
int tkgdi_ps2ms_ly1;
int tkgdi_ps2ms_lb1;

int TKGDI_UpdateWindowStack_PumpInput(void)
{
	TKGDI_EVENT t_imsg;
	TKGDI_EVENT *imsg;
	int i, j, k;

	imsg=&t_imsg;

	while(tk_kbhit())
	{
		i=tk_getch();
		if(i<=0)
			continue;

		if(i==0x80)
		{
			j=tk_getch();
			k=(j<<8)|tk_getch();
		}else
			if((i==0x7F) || (i==0xFF))
		{
			j=tk_getch();
			k=((i&0x80)<<8)|j;
		}else
		{
			k=((i&0x80)<<8)|(i&0x7F);
		}
		
		if(!(k&0x7FFF))
			continue;

		imsg->evSize=sizeof(TKGDI_EVENT);
		imsg->dev=1;
		imsg->fccMsg=TKGDI_FCC_keyb;
		imsg->ptMsec=TK_GetTimeMs();
		imsg->wParm1=k;
		imsg->wParm2=0;
		imsg->lParm1=0;
		imsg->ptMsX=tkgdi_ps2ms_lx;
		imsg->ptMsY=tkgdi_ps2ms_ly;
		imsg->ptMsB=tkgdi_ps2ms_lb;

		TKGDI_DevPushEvent(NULL, 1, imsg);
	}
	
	return(0);
}

int TKGDI_UpdateWindowStack_PumpMouse(void)
{
	TKGDI_EVENT t_imsg;
	TKGDI_EVENT *imsg;
	int i, j, k;

	imsg=&t_imsg;

	k=0;

	if((tkgdi_ps2ms_lb&1) && !(tkgdi_ps2ms_lb1&1))
	{
		tk_printf("TKGDI_UpdateWindowStack_PumpMouse: Click\n");
		k|=1;
		
		tkgdi_ps2ms_time_dn1=tkgdi_ps2ms_time_dn;
		tkgdi_ps2ms_time_dn=TK_GetTimeMs();
		
		j=tkgdi_ps2ms_time_dn-tkgdi_ps2ms_time_dn1;
		if((j>50) && (j<250))
		{
			tk_printf("TKGDI_UpdateWindowStack_PumpMouse: DoubleClick\n");
			k|=2;
		}
	}

	imsg->evSize=sizeof(TKGDI_EVENT);
	imsg->dev=1;
	imsg->fccMsg=TKGDI_FCC_mous;
	imsg->ptMsec=TK_GetTimeMs();
	imsg->wParm1=k;
	imsg->wParm2=0;
	imsg->lParm1=0;
	imsg->ptMsX=tkgdi_ps2ms_lx;
	imsg->ptMsY=tkgdi_ps2ms_ly;
	imsg->ptMsB=tkgdi_ps2ms_lb;

	TKGDI_DevPushEvent(NULL, 1, imsg);
}

int TKGDI_WindowSetActiveTab(_tkgdi_window_t *wctx, int tab)
{
	_tkgdi_conparm *con;
	int i, j, k;
	
	if((tab<0) || (tab>=10))
		return(-1);

	if(tab==wctx->tabactive)
	{
		return(0);
	}

	con=wctx->contab[tab];
	if(!con)
	{
		con=tk_malloc(sizeof(_tkgdi_conparm));
		memset(con, 0, sizeof(_tkgdi_conparm));
		tkgdi_con_init(con);
		wctx->contab[tab]=con;
	}

	con->pixbuf=wctx->buf_data;
	con->cell_xs=wctx->cell_xs;
	con->cell_ys=wctx->cell_ys;

	wctx->tabactive=tab;	
	wctx->con=con;
	tkgdi_con_markdirty(con);
	tkgdi_con_redrawbuffer(con);

	TKGDI_WindowMarkDirtyRect(NULL, wctx,
		0, 0, wctx->size_x, wctx->size_y);

	TKGDI_UpdateWindowCells(wctx);

	TKGDI_ScreenMarkDirtyRect(
		wctx->base_x, wctx->base_y-10,
		wctx->base_x+wctx->size_x,
		wctx->base_y+wctx->size_y);

	return(0);
}

int TKGDI_UpdateWindowStack(void)
{
	_tkgdi_window_t *wctx, *wctx2;
	byte *cs;
	u64 *wutx, *wucs, *wuct;
	u64 blkb, bp0, bp1;
	int xs, ys, bxs, bys, bxs2, bys2, bmsz, bmxs2;
	int x, y, z, z0, z1, bx, by, wxs, wys, wbmxs;
	int sx, sy, z0a, z1a, b, issel;
	int i, j, k;

	for(i=0; i<tkgdi_n_window_vis; i++)
	{
		wctx=tkgdi_window_vis[i];
		
		if(wctx->con && wctx->con->conrowmask)
		{
			tkgdi_con_redrawbuffer(wctx->con);
		}
	}

	TKGDI_UpdateWindowStack_PumpInput();

	tk_ps2ms_getpos(&tkgdi_ps2ms_x, &tkgdi_ps2ms_y, &tkgdi_ps2ms_b);
	tkgdi_ps2ms_moved=0;

	if(	(tkgdi_ps2ms_x!=tkgdi_ps2ms_lx) ||
		(tkgdi_ps2ms_y!=tkgdi_ps2ms_ly) ||
		(tkgdi_ps2ms_b!=tkgdi_ps2ms_lb))
	{
		if(tkgdi_ps2ms_x<0)
			tkgdi_ps2ms_x=0;
		if(tkgdi_ps2ms_y<0)
			tkgdi_ps2ms_y=0;
		if(tkgdi_ps2ms_x>=tkgdi_vid_xsize)
			tkgdi_ps2ms_x=tkgdi_vid_xsize-1;
		if(tkgdi_ps2ms_y>=tkgdi_vid_ysize)
			tkgdi_ps2ms_y=tkgdi_vid_ysize-1;
		tk_ps2ms_setpos(tkgdi_ps2ms_x, tkgdi_ps2ms_y);

//		tk_printf("TKGDI_UpdateWindowStack: Mouse: %d %d %d\n",
//			tkgdi_ps2ms_x, tkgdi_ps2ms_y, tkgdi_ps2ms_b);
			
		tkgdi_ps2ms_lx1=tkgdi_ps2ms_lx;
		tkgdi_ps2ms_ly1=tkgdi_ps2ms_ly;
		tkgdi_ps2ms_lb1=tkgdi_ps2ms_lb;

		tkgdi_ps2ms_lx=tkgdi_ps2ms_x;
		tkgdi_ps2ms_ly=tkgdi_ps2ms_y;
		tkgdi_ps2ms_lb=tkgdi_ps2ms_b;
		
		tkgdi_ps2ms_moved=1;
		TKGDI_UpdateWindowStack_PumpMouse();

//		for(i=0; i<tkgdi_n_window_vis; i++)
		for(i=tkgdi_n_window_vis-1; i>=0; i--)
		{
			wctx=tkgdi_window_vis[i];
			
//			if(wctx->con && wctx->con->conrowmask)
//			{
//				tkgdi_con_redrawbuffer(wctx->con);
//			}

//			if(	(tkgdi_ps2ms_ly1>=(wctx->base_y-8))	&&
//				(tkgdi_ps2ms_ly1<=(wctx->base_y-0))	&&
//				(tkgdi_ps2ms_lx1>(wctx->base_x+8))	&&
//				(tkgdi_ps2ms_lx1<(wctx->base_x+wctx->size_x-16))
//				)
			if(	(tkgdi_ps2ms_ly1>=(wctx->base_y-8))	&&
				(tkgdi_ps2ms_ly1<=(wctx->base_y-0))	&&
				(tkgdi_ps2ms_lx1>=(wctx->base_x))	&&
				(tkgdi_ps2ms_lx1<=(wctx->base_x+wctx->size_x))
				)
			{
				if(tkgdi_ps2ms_lb&1)
				{
					if(i!=(tkgdi_n_window_vis-1))
					{
						wctx2=tkgdi_window_vis[tkgdi_n_window_vis-1];
					
						for(j=i; j<tkgdi_n_window_vis; j++)
							tkgdi_window_vis[j]=tkgdi_window_vis[j+1];
						tkgdi_window_vis[tkgdi_n_window_vis-1]=wctx;
						TKGDI_ScreenMarkDirtyRect(
							wctx->base_x, wctx->base_y-10,
							wctx->base_x+wctx->size_x,
							wctx->base_y+wctx->size_y);
						TKGDI_ScreenMarkDirtyRect(
							wctx2->base_x, wctx2->base_y-10,
							wctx2->base_x+wctx2->size_x,
							wctx2->base_y+wctx2->size_y);
						break;
					}

					if(	(tkgdi_ps2ms_lx1>=(wctx->base_x+0))	&&
						(tkgdi_ps2ms_lx1<(wctx->base_x+8))
						)
					{
						tk_printf("TKGDI_UpdateWindowStack: Click Close\n");
						break;
					}

					if(	(tkgdi_ps2ms_lx1>=(wctx->base_x+wctx->size_x-16)) &&
						(tkgdi_ps2ms_lx1<(wctx->base_x+wctx->size_x-8))
						)
					{
						tk_printf("TKGDI_UpdateWindowStack: Click Minimize\n");
						break;
					}

					if(	(tkgdi_ps2ms_lx1>=(wctx->base_x+wctx->size_x-8)) &&
						(tkgdi_ps2ms_lx1<(wctx->base_x+wctx->size_x-0))
						)
					{
						tk_printf("TKGDI_UpdateWindowStack: Click Maximize\n");
						break;
					}

					j=(tkgdi_ps2ms_lx1-(wctx->base_x+wctx->size_x-12*8))>>3;
					if((j>=0) && (j<9) && wctx->con)
					{
						tk_printf("TKGDI_UpdateWindowStack: Click Tab %d\n",
							j+1);

						TKGDI_WindowSetActiveTab(wctx, j);
//						tkgdi_con_settab(wctx->con, j);
						break;
					}

					TKGDI_ScreenMarkDirtyRect(
						wctx->base_x, wctx->base_y-10,
						wctx->base_x+wctx->size_x,
						wctx->base_y+wctx->size_y);

					wctx->base_x+=tkgdi_ps2ms_lx-tkgdi_ps2ms_lx1;
					wctx->base_y+=tkgdi_ps2ms_ly-tkgdi_ps2ms_ly1;
					
					if(wctx->base_x<0)
						wctx->base_x=0;
					if(wctx->base_y<8)
						wctx->base_y=8;
						
					if(wctx->base_x>(tkgdi_vid_xsize-wctx->size_x))
						wctx->base_x=(tkgdi_vid_xsize-wctx->size_x);

					if(wctx->base_y>(tkgdi_vid_ysize-wctx->size_y))
						wctx->base_y=(tkgdi_vid_ysize-wctx->size_y);

					TKGDI_ScreenMarkDirtyRect(
						wctx->base_x, wctx->base_y-10,
						wctx->base_x+wctx->size_x,
						wctx->base_y+wctx->size_y);
					
					break;
				}
			}
			
			if(	(wctx->base_x<0) ||
				(wctx->base_y<8) ||
				(wctx->base_x>(tkgdi_vid_xsize-wctx->size_x)) ||
				(wctx->base_y>(tkgdi_vid_ysize-wctx->size_y))
				)
			{
				if(wctx->base_x<0)
					wctx->base_x=0;
				if(wctx->base_y<8)
					wctx->base_y=8;
					
				if(wctx->base_x>(tkgdi_vid_xsize-wctx->size_x))
					wctx->base_x=(tkgdi_vid_xsize-wctx->size_x);

				if(wctx->base_y>(tkgdi_vid_ysize-wctx->size_y))
					wctx->base_y=(tkgdi_vid_ysize-wctx->size_y);

				TKGDI_ScreenMarkDirtyRect(
					wctx->base_x, wctx->base_y-10,
					wctx->base_x+wctx->size_x,
					wctx->base_y+wctx->size_y);
			}
		}

	}

	xs=tkgdi_vid_xsize;
	ys=tkgdi_vid_ysize;
	bxs2=(xs+7)>>3;
	bys2=(ys+7)>>3;

//	bmsz=(bxs2*bys2+7)>>3;

	tkgdi_vid_bmxsize=(bxs2+7)>>3;
	bmxs2=tkgdi_vid_bmxsize<<3;
	bmsz=tkgdi_vid_bmxsize*bys2;
	
//	tk_printf("TKGDI_UpdateWindowStack: %d %d\n",
//		tkgdi_vid_bmxsize, bmsz);
	
	bxs=bxs2<<1;
	bys=bys2<<1;
	
	if(!tkgdi_vid_screendirty)
	{	tkgdi_vid_screendirty=tk_malloc(bmsz+(4*tkgdi_vid_bmxsize));
		memset(tkgdi_vid_screendirty, 0xFF, bmsz);	}
	
	
	if(tkgdi_ps2ms_moved)
	{
		/* Mouse has moved, so mark spots dirty */

		bx=tkgdi_ps2ms_lx1>>2;
		by=tkgdi_ps2ms_ly1>>2;
		z0a=(by>>1)*bmxs2+(bx>>1);

		z=z0a+0;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
		z=z0a+1;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
		z=z0a+bmxs2+0;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
		z=z0a+bmxs2+1;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));


		bx=tkgdi_ps2ms_x>>2;
		by=tkgdi_ps2ms_y>>2;
		z0a=(by>>1)*bmxs2+(bx>>1);

		z=z0a+0;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
		z=z0a+1;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
		z=z0a+bmxs2+0;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
		z=z0a+bmxs2+1;
		tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
	}
	
//	if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555) ||
//		(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_RGB555))
	if(tkgdi_vid_noutx2)
	{
		if(!tkgdi_vid_screenrgb)
		{
			tkgdi_vid_screenrgb=tk_malloc(bxs*bys*(4*8));
			memset(tkgdi_vid_screenrgb, 0x55, bxs*bys*(4*8));
		}

	//	blkb=0;
		blkb=0x55AA55AA5555AAAAULL;
		bp0=0x5555AAAA5555AAAAULL;
		bp1=0xAAAA5555AAAA5555ULL;
		/* Clear any dirty blocks with background color. */
		for(y=0; y<bys2; y++)
		{
			z1=y*bmxs2;
			for(x=0; x<bxs2; x++)
			{
		//		z=y*bxs2+x;
				z=z1+x;
				
				b=tkgdi_vid_screendirty[z>>3];
				if(!b && !(x&7))
					{ x+=7; continue; }
				
				if(!(b&(1<<(z&7))))
					continue;

				TKGDI_UpdateWindowStack_CopyFillPattern(
					x, y, bxs, bp0, bp1);
			}
		}

		for(i=0; i<tkgdi_n_window_vis; i++)
		{
			wctx=tkgdi_window_vis[i];
			
			bx=wctx->base_x>>2;
			by=wctx->base_y>>2;
			wxs=wctx->size_bxs;
			wys=wctx->size_bys;
			wbmxs=wctx->size_bmxs;

			issel=(i==(tkgdi_n_window_vis-1));

#if 1
//			if(1)
//			if(wctx->dirty1)
			if(!(wctx->style&1))
			{
				/* Draw Title Bar */

#if 0
				for(x=0; x<(wxs-1); x+=2)
				{
					sy=by-2;
					if(sy<0)
						continue;
					if(sy>=bys)
						continue;

					sx=bx+x;

					if(sx<0)
						continue;
					if(sx>=bxs)
						continue;

					z0a=((sy+0)>>1)*bmxs2+((bx+x)>>1);
					z1a=((sy+1)>>1)*bmxs2+((bx+x)>>1);
//					z=z0a+(x>>1);
//					z1=z1a+x;
					
					if(	!(tkgdi_vid_screendirty[z0a>>3]&(3<<(z0a&7))) &&
						!(tkgdi_vid_screendirty[z1a>>3]&(3<<(z1a&7))))
					{
						continue;
					}

					TKGDI_UpdateWindowStack_CopyFillFlat(
						sx, sy, bxs,
						issel?TKGDI_CLR_TITLEBLUSEL:TKGDI_CLR_TITLEBLU);

					tkgdi_vid_screendirty[z0a>>3]|=(3<<(z0a&7));
					tkgdi_vid_screendirty[z1a>>3]|=(3<<(z1a&7));
				}
#endif

				x=1;

				if(wctx->title)
				{
					cs=(byte *)wctx->title;
				}else
				{
					cs="  ";
				}

				while(*cs)
				{
					j=*cs++;
					blkb=TK_Con_GlyphForCodepoint(j);

					sy=by-2;
					if(sy<0)
						continue;
					if(sy>=bys)
						continue;

					sx=bx+x*2;
					x++;

					if(sx<0)
						continue;
					if(sx>=bxs)
						continue;

					z0a=((sy+0)>>1)*bmxs2+((bx+x*2)>>1);
					z1a=((sy+1)>>1)*bmxs2+((bx+x*2)>>1);
					
					if(	!(tkgdi_vid_screendirty[z0a>>3]&(3<<(z0a&7))) &&
						!(tkgdi_vid_screendirty[z1a>>3]&(3<<(z1a&7))))
					{
						continue;
					}

					TKGDI_UpdateWindowStack_CopyFillTilePx(
						sx, sy, bxs,
						blkb, TKGDI_ICOPX_00,
						0x7FFF,
						issel?TKGDI_CLR_TITLEBLUSEL:TKGDI_CLR_TITLEBLU);

					(*(u16 *)(tkgdi_vid_screendirty+(z0a>>3)))|=
						(3<<(z0a&7));
					(*(u16 *)(tkgdi_vid_screendirty+(z1a>>3)))|=
						(3<<(z1a&7));
				}

#if 1
				while(x<((wxs-1)>>1))
				{
					sy=by-2;
					if(sy<0)
						break;
					if(sy>=bys)
						break;

					sx=bx+x*2;
					x++;

					if(sx<0)
						continue;
					if(sx>=bxs)
						break;

					z0a=((sy+0)>>1)*bmxs2+((bx+x*2)>>1);
					z1a=((sy+1)>>1)*bmxs2+((bx+x*2)>>1);
					
					j=(*(u16 *)(tkgdi_vid_screendirty+(z0a>>3)))>>(z0a&7);
					k=(*(u16 *)(tkgdi_vid_screendirty+(z1a>>3)))>>(z1a&7);
					if(	!(j&255) && !(k&255))
						{ x+=7; continue; }
					if(	!(j&15) && !(k&15))
						{ x+=3; continue; }
					if(	!(j&1) && !(k&1))
						{ continue; }

#if 0
//					if(	!(tkgdi_vid_screendirty[z0a>>3]&(3<<(z0a&7))) &&
//						!(tkgdi_vid_screendirty[z1a>>3]&(3<<(z1a&7))))
					if(	!((*(u16 *)(tkgdi_vid_screendirty+(z0a>>3)))&
							(3<<(z0a&7))) &&
						!((*(u16 *)(tkgdi_vid_screendirty+(z1a>>3)))&
							(3<<(z1a&7))))
					{
						continue;
					}
#endif

					TKGDI_UpdateWindowStack_CopyFillFlat(
						sx, sy, bxs,
						issel?TKGDI_CLR_TITLEBLUSEL:TKGDI_CLR_TITLEBLU);

//					tkgdi_vid_screendirty[z0a>>3]|=(3<<(z0a&7));
//					tkgdi_vid_screendirty[z1a>>3]|=(3<<(z1a&7));

					(*(u16 *)(tkgdi_vid_screendirty+(z0a>>3)))|=(3<<(z0a&7));
					(*(u16 *)(tkgdi_vid_screendirty+(z1a>>3)))|=(3<<(z1a&7));
				}
#endif

				if(wctx->con)
				{
					x-=11;
					for(j=0; j<9; j++)
					{
						blkb=TK_Con_GlyphForCodepoint('1'+j);

						sy=by-2;
						if(sy<0)
							continue;
						if(sy>=bys)
							continue;

						sx=bx+x*2;
						x++;

						if(sx<0)
							continue;
						if(sx>=bxs)
							continue;

						z0a=((sy+0)>>1)*bmxs2+((bx+x*2)>>1);
						z1a=((sy+1)>>1)*bmxs2+((bx+x*2)>>1);
						
						if(	!(tkgdi_vid_screendirty[z0a>>3]&(3<<(z0a&7))) &&
							!(tkgdi_vid_screendirty[z1a>>3]&(3<<(z1a&7))))
						{
							continue;
						}

						TKGDI_UpdateWindowStack_CopyFillTilePx(
							sx, sy, bxs,
							blkb, TKGDI_ICOPX_00,
							TKGDI_CLR_GRAY25,
							(j==wctx->tabactive)?
							TKGDI_CLR_GRAY50 :
							TKGDI_CLR_GRAY75);

						(*(u16 *)(tkgdi_vid_screendirty+(z0a>>3)))|=
							(3<<(z0a&7));
						(*(u16 *)(tkgdi_vid_screendirty+(z1a>>3)))|=
							(3<<(z1a&7));
					}
				}

				TKGDI_UpdateWindowStack_CopyFillTilePx(bx, by-2, bxs,
					TKGDI_ICOPX_X, TKGDI_ICOPX_00,
					TKGDI_CLR_GRAY25, TKGDI_CLR_GRAY75);

				TKGDI_UpdateWindowStack_CopyFillTilePx(bx+wxs-4, by-2, bxs,
					TKGDI_ICOPX_DN, TKGDI_ICOPX_00,
					TKGDI_CLR_GRAY25, TKGDI_CLR_GRAY75);
				TKGDI_UpdateWindowStack_CopyFillTilePx(bx+wxs-2, by-2, bxs,
					TKGDI_ICOPX_UP, TKGDI_ICOPX_00,
					TKGDI_CLR_GRAY25, TKGDI_CLR_GRAY75);
			}
#endif
			
			wutx=((u64 *)wctx->buf_data);
			for(y=0; y<wys; y++)
			{
				sy=by+y;
				if(sy<0)
					continue;
				if(sy>=bys)
					continue;

//				wucs=wutx+(y*wxs);
//				wuct=tkgdi_vid_screenrgb+(sy*bxs+bx);
				z0a=(sy>>1)*bmxs2+(bx>>1);
				z1a=y*(wbmxs<<3);

				for(x=0; x<wxs; x++)
				{
					sx=bx+x;
					if(sx<0)
						continue;
					if(sx>=bxs)
						continue;

					z=z0a+(x>>1);
					z1=z1a+x;
					z0=z+1;
					
//					if(!(tkgdi_vid_screendirty[z>>3]&(1<<(z&7))))
					if(	!(tkgdi_vid_screendirty[z >>3]&(1<<(z &7))) &&
						!(tkgdi_vid_screendirty[z0>>3]&(1<<(z0&7))) )
					{
//						if(!(wctx->dirty2))
						if(!(wctx->dirty1))
						{
							if((x+16)<=wxs)
							{
								if(	!((*(u16 *)(tkgdi_vid_screendirty+(z >>3)))&
										(255<<(z &7))) &&
									!((*(u16 *)(tkgdi_vid_screendirty+(z0>>3)))&
										(255<<(z0&7))))
								{
									x+=15;
									continue;
								}
							}
							continue;
						}
//						if(!(wctx->buf_dirty2[z1>>3]&(1<<(z1&7))))
						if(!(wctx->buf_dirty1[z1>>3]&(1<<(z1&7))))
						{
							if((x+16)<=wxs)
							{
								if(	!((*(u16 *)(tkgdi_vid_screendirty+(z >>3)))&
										(255<<(z &7))) &&
									!((*(u16 *)(tkgdi_vid_screendirty+(z0>>3)))&
										(255<<(z0&7))) &&
									!((*(u32 *)(wctx->buf_dirty1+(z1>>3)))&
										(65535<<(z1&7))))
								{
									x+=15;
									continue;
								}
							}
							continue;
						}
					}
					
					k=((*(u16 *)(wctx->buf_dirty1+(z1>>3)))>>(z1&7));
					if((k&255)==255)
					{
						TKGDI_UpdateWindowStack_CopyFillSource8x(
							sx, sy, bxs,
							wutx, x, y, wxs);
						*(u16 *)(tkgdi_vid_screendirty+(z>>3))|=(15<<(z&7));
						x+=7;
					}else
						if((k&15)==15)
					{
						TKGDI_UpdateWindowStack_CopyFillSource4x(
							sx, sy, bxs,
							wutx, x, y, wxs);
						*(u16 *)(tkgdi_vid_screendirty+(z>>3))|=(3<<(z&7));
						x+=3;
					}else
					{
						TKGDI_UpdateWindowStack_CopyFillSource(
							sx, sy, bxs,
							wutx, x, y, wxs);
						tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
					}
				}
			}
			
//			memset(wctx->buf_dirty2, 0, wctx->size_bmsz);
			memset(wctx->buf_dirty1, 0, wctx->size_bmsz);
			wctx->dirty2=0;
			wctx->dirty1=0;
		}

#if 1
		for(x=0; x<bxs; x+=2)
		{
			sy=bys-2;
			sx=x;

			z0a=((sy+0)>>1)*bmxs2+(sx>>1);
			z1a=((sy+1)>>1)*bmxs2+(sx>>1);
			
			if(	!(tkgdi_vid_screendirty[z0a>>3]&(3<<(z0a&7))) &&
				!(tkgdi_vid_screendirty[z1a>>3]&(3<<(z1a&7))))
			{
				continue;
			}

			TKGDI_UpdateWindowStack_CopyFillFlat(
				sx, sy, bxs,
				TKGDI_CLR_GRAY75);

			tkgdi_vid_screendirty[z0a>>3]|=(3<<(z0a&7));
			tkgdi_vid_screendirty[z1a>>3]|=(3<<(z1a&7));
		}
#endif

		for(i=2; i<tkgdi_n_window_vis; i++)
		{
			wctx=tkgdi_window_vis[i];
			issel=(i==(tkgdi_n_window_vis-1));
			
			x=i-1;

			sy=bys-2;
			sx=x;

			z0a=((sy+0)>>1)*bmxs2+(sx>>1);
			z1a=((sy+1)>>1)*bmxs2+(sx>>1);
			
			if(	!(tkgdi_vid_screendirty[z0a>>3]&(3<<(z0a&7))) &&
				!(tkgdi_vid_screendirty[z1a>>3]&(3<<(z1a&7))))
			{
				continue;
			}
			
			k='$';
			if(wctx->title)
				k=wctx->title[0];

			blkb=TK_Con_GlyphForCodepoint(k);
			TKGDI_UpdateWindowStack_CopyFillTilePx(
				sx, sy, bxs,
				blkb, TKGDI_ICOPX_00,
				0x7FFF,
				issel?TKGDI_CLR_TITLEBLUSEL:TKGDI_CLR_TITLEBLU);
		}

		wutx=(u16 *)tkgdi_vid_screenrgb;
		bx=tkgdi_ps2ms_x>>2;
		by=tkgdi_ps2ms_y>>2;
//		wutx[(by*4+0)*bxs+bx]=0x0;
//		wutx[(by*4+1)*bxs+bx]=0x0;
//		wutx[(by*4+2)*bxs+bx]=0x0;
//		wutx[(by*4+3)*bxs+bx]=0x0;

//		wutx[(by*4+0)*bxs+bx]=~wutx[(by*4+0)*bxs+bx];
//		wutx[(by*4+1)*bxs+bx]=~wutx[(by*4+1)*bxs+bx];
//		wutx[(by*4+2)*bxs+bx]=~wutx[(by*4+2)*bxs+bx];
//		wutx[(by*4+3)*bxs+bx]=~wutx[(by*4+3)*bxs+bx];

		wutx[(by*4+0)*bxs+bx]=wutx[(by*4+0)*bxs+bx]+0x3333333333333333ULL;
		wutx[(by*4+1)*bxs+bx]=wutx[(by*4+1)*bxs+bx]+0x3333333333333333ULL;
		wutx[(by*4+2)*bxs+bx]=wutx[(by*4+2)*bxs+bx]+0x3333333333333333ULL;
		wutx[(by*4+3)*bxs+bx]=wutx[(by*4+3)*bxs+bx]+0x3333333333333333ULL;

//		TKGDI_BlitUpdate_BlkRgb5_Mask(
//			0, 0, xs, ys,
//			tkgdi_vid_screenutx, tkgdi_vid_screendirty,
//			0, 0, xs, -ys);

		TKGDI_BlitUpdate_BlkRgb555_Mask(
			0, 0, xs, ys,
			(u16 *)tkgdi_vid_screenrgb,
			tkgdi_vid_screendirty,
			0, 0, xs, -ys);

		memset(tkgdi_vid_screendirty, 0, bmsz);
	}



	if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC) ||
		(tkgdi_vid_scrmode==TKGDI_SCRMODE_800x600_CC))
	{
		if(!tkgdi_vid_screenutx)
			{ tkgdi_vid_screenutx=tk_malloc(bxs2*bys2*(4*8)); }

	//	blkb=0;
		blkb=0x55AA55AA5555AAAAULL;
		/* Clear any dirty blocks with background color. */
		for(y=0; y<bys2; y++)
		{
			z1=y*bmxs2;
			for(x=0; x<bxs2; x++)
			{
		//		z=y*bxs2+x;
				z=z1+x;
				
				b=tkgdi_vid_screendirty[z>>3];
				if(!b && !(x&7))
					{ x+=7; continue; }
				
				if(!(b&(1<<(z&7))))
					continue;
				tkgdi_vid_screenutx[(((y<<1)+0)*bxs)+((x<<1)+0)]=blkb;
				tkgdi_vid_screenutx[(((y<<1)+0)*bxs)+((x<<1)+1)]=blkb;
				tkgdi_vid_screenutx[(((y<<1)+1)*bxs)+((x<<1)+0)]=blkb;
				tkgdi_vid_screenutx[(((y<<1)+1)*bxs)+((x<<1)+1)]=blkb;
			}
		}
		
		for(i=0; i<tkgdi_n_window_vis; i++)
		{
			wctx=tkgdi_window_vis[i];
			
			bx=wctx->base_x>>2;
			by=wctx->base_y>>2;
			wxs=wctx->size_bxs;
			wys=wctx->size_bys;
			wbmxs=wctx->size_bmxs;
			
			wutx=wctx->buf_utx2;
			for(y=0; y<wys; y++)
			{
				sy=by+y;
				if(sy<0)
					continue;
				if(sy>=bxs)
					continue;

				wucs=wutx+(y*wxs);
				wuct=tkgdi_vid_screenutx+(sy*bxs+bx);
				z0a=(sy>>1)*bmxs2+(bx>>1);
				z1a=y*(wbmxs<<3);

				for(x=0; x<wxs; x++)
				{
					sx=bx+x;
	//				if((sx<0) || (sy<0))
					if(sx<0)
						continue;
	//				if((sx>=bxs) || (sy>=bxs))
					if(sx>=bxs)
						continue;

		//			z=(sy>>1)*bxs2+(sx>>1);
	//				z=(sy>>1)*bmxs2+(sx>>1);
	//				z1=y*(wbmxs<<3)+x;
					z=z0a+(x>>1);
					z1=z1a+x;
					
					if(!(tkgdi_vid_screendirty[z>>3]&(1<<(z&7))))
					{
						if(!(wctx->dirty2))
							continue;
						if(!(wctx->buf_dirty2[z1>>3]&(1<<(z1&7))))
							continue;
					}

	//				blkb=wutx[y*wxs+x];
					blkb=wucs[x];
					
		//			blkb=0x55AA55AA7FFF0000ULL;
					
	//				tkgdi_vid_screenutx[sy*bxs+sx]=blkb;
					wuct[x]=blkb;

					tkgdi_vid_screendirty[z>>3]|=(1<<(z&7));
				}
			}

			memset(wctx->buf_dirty2, 0, wctx->size_bmsz);
			wctx->dirty2=0;
		}
		
		TKGDI_BlitUpdate_BlkUtx2_CCT_Mask(
			0, 0, xs, ys,
			tkgdi_vid_screenutx, tkgdi_vid_screendirty,
			0, 0, xs, -ys);
		memset(tkgdi_vid_screendirty, 0, bmsz);
	}
	
	return(1);
}

_tkgdi_window_t *TKGDI_AllocNewWindow(
	_tkgdi_context_t *ctx)
{
	_tkgdi_window_t *wctx;
	int i, j, k;

	for(i=2; i<tkgdi_n_windows; i++)
	{
		if(!tkgdi_windows[i])
			break;
	}
	
	if(i>=tkgdi_n_windows)
	{
		i=tkgdi_n_windows++;
		while(i<2)
			i=tkgdi_n_windows++;
		tkgdi_windows[i]=NULL;
	}
	
	wctx=tk_malloc(sizeof(_tkgdi_window_t));
	memset(wctx, 0, sizeof(_tkgdi_window_t));
	
	wctx->idx=i;
	tkgdi_windows[i]=wctx;
	
	wctx->buf_info=&(wctx->t_buf_info);
	
	return(wctx);
}

_tkgdi_window_t *TKGDI_AllocNewConsoleWindow(
	_tkgdi_context_t *ctx)
{
	_tkgdi_window_t *wctx;
	_tkgdi_conparm *con;
	int i, j, k;

	wctx=TKGDI_AllocNewWindow(ctx);
	
	con=tk_malloc(sizeof(_tkgdi_conparm));
	memset(con, 0, sizeof(_tkgdi_conparm));
	tkgdi_con_init(con);
	
	wctx->tabactive=0;
	wctx->contab[0]=con;
	wctx->con=con;
	
//	wctx->size_x=80*8;
//	wctx->size_y=25*8;
//	wctx->size_bxs=wctx->size_x>>2;
//	wctx->size_bys=wctx->size_y>>2;
	
	con->cell_xs=4;
	con->cell_ys=6;
	
	wctx->size_x=80*4;
	wctx->size_y=25*6;
	wctx->size_bxs=wctx->size_x>>2;
	wctx->size_bys=wctx->size_y>>2;
	wctx->cell_xs=4;
	wctx->cell_ys=6;

	return(wctx);
}

TKGSTATUS TKGDI_BlitSubImageNew(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_RRECT *drect,
//	int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	TKGDI_RRECT *rect)
//	int xo_src, int yo_src, int xs_src, int ys_src)
{
	_tkgdi_window_t *wctx;
	byte *bmct;
	u16 *cs, *ct;
	u32 *pal;
	int xo_dev, yo_dev;
	int xo_src, yo_src, xs_src, ys_src;
	int xs, ys, mxs, mys, bxs, bys, nx, ny, flip;
	int i, j, k;

	if(dev<=0)
		return(-1);
	
	xo_dev=drect->xorg;
	yo_dev=drect->yorg;
	
	xo_src=rect->xorg;
	yo_src=rect->yorg;
	xs_src=rect->xsize;
	ys_src=rect->ysize;
	
	if(xs_src<0)
		xs_src=-xs_src;
	if(ys_src<0)
		ys_src=-ys_src;
	
	flip=(info->biHeight>0);
	
//	return(-1);
		
	if(dev==1)
	{
		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555_LFB)
		{
			xs=xs_src;
			ys=ys_src;
			
			mxs=320-xo_dev;
			mys=200-yo_dev;
			
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression == TKGDI_BI_RGB)
			{
				if(info->biBitCount == 16)
				{
					TKGDI_BlitUpdate_LfbRgb555(xo_dev, yo_dev, xs, ys,
						data, xo_src, yo_src, info->biWidth, info->biHeight);
				}
			}

		}
	
//		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555)
		if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555) ||
			(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_RGB555))
		{
			xs=xs_src;
			ys=ys_src;
			
			mxs=320-xo_dev;
			mys=200-yo_dev;
			
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression == TKGDI_BI_RGB)
			{
				if(info->biBitCount == 16)
				{
					TKGDI_BlitUpdate_BlkRgb555(xo_dev, yo_dev, xs, ys,
						data, xo_src, yo_src, info->biWidth, info->biHeight);
				}

				if(info->biBitCount == 8)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkIndex8(xo_dev, yo_dev, xs, ys,
						data, pal, xo_src, yo_src,
						info->biWidth, info->biHeight);
				}

				if(info->biBitCount == 4)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkIndex4(xo_dev, yo_dev, xs, ys,
						data, pal, xo_src, yo_src,
						info->biWidth, info->biHeight);
				}

				if(info->biBitCount == 24)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkRgb888(xo_dev, yo_dev, xs, ys,
						data, 3, xo_src, yo_src,
						info->biWidth, info->biHeight);
				}

				if(info->biBitCount == 32)
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkRgb888(xo_dev, yo_dev, xs, ys,
						data, 4, xo_src, yo_src,
						info->biWidth, info->biHeight);
				}
			}
		}

//		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC)
		if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)			||
			(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC)	||
			(tkgdi_vid_scrmode==TKGDI_SCRMODE_800x600_CC)	)
		{
			xs=xs_src;
			ys=ys_src;
			
//			mxs=640-xo_dev;
			mxs=tkgdi_vid_xsize-xo_dev;
//			mys=400-yo_dev;
			mys=tkgdi_vid_ysize-yo_dev;
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression == TKGDI_BI_RGB)
			{
				if(info->biBitCount == 16)
				{
					TKGDI_BlitUpdate_BlkRgb555_CCE(xo_dev, yo_dev, xs, ys,
						data, xo_src, yo_src, info->biWidth, info->biHeight);
				}

				if((info->biBitCount == 8) ||
					(info->biBitCount == 4))
				{
					pal=(u32 *)(((byte *)info)+info->biSize);
					TKGDI_BlitUpdate_BlkIndex_CCE(xo_dev, yo_dev, xs, ys,
						data, pal, xo_src, yo_src,
						info->biWidth, info->biHeight,
						info->biBitCount);
				}

				if((info->biBitCount == 24) ||
					(info->biBitCount == 32))
				{
					TKGDI_BlitUpdate_BlkIndex_CCE(xo_dev, yo_dev, xs, ys,
						data, NULL, xo_src, yo_src,
						info->biWidth, info->biHeight,
						info->biBitCount);
				}
			}

			if(info->biCompression == TKGDI_FCC_UTX2)
			{
				TKGDI_BlitUpdate_BlkUtx2_CCT(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}
		}

#if 0
		if(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)
		{
			xs=xs_src;
			ys=ys_src;
			
			mxs=640-xo_dev;
			mys=200-yo_dev;
			if(xs>mxs)
				xs=mxs;
			if(ys>mys)
				ys=mys;

			if(info->biCompression==0)
			{
				TKGDI_BlitUpdate_BlkRgb555_CCE(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}

			if(info->biCompression==TKGDI_FCC_UTX2)
			{
				TKGDI_BlitUpdate_BlkUtx2_CCT(xo_dev, yo_dev, xs, ys,
					data, xo_src, yo_src, info->biWidth, info->biHeight);
			}
		}
#endif

		return(0);
	}
	
	if((dev>1) && (dev<tkgdi_n_windows))
	{
		wctx=tkgdi_windows[dev];
		
		if(!wctx || !wctx->buf_info)
		{
			tk_printf("TKGDI_BlitSubImage: Bad wctx=%p\n",
				wctx);
			return(-1);
		}

		xs=xs_src;
		ys=ys_src;
		
		bxs=wctx->buf_info->biWidth;
		bys=wctx->buf_info->biHeight;
		mxs=bxs-xo_dev;
		mys=bys-yo_dev;
		if(xs>mxs)
			xs=mxs;
		if(ys>mys)
			ys=mys;
			
		cs=data;
		ct=wctx->buf_data;
		
		if(flip)
		{
			cs+=(ys-1)*xs_src;
			ct+=yo_dev*bxs+xo_dev;
			for(i=0; i<ys; i++)
			{
				memcpy(ct, cs, xs*2);
				cs-=xs_src;
				ct+=bxs;
			}
		}else
		{
			ct+=yo_dev*bxs+xo_dev;
			for(i=0; i<ys; i++)
			{
				memcpy(ct, cs, xs*2);
				cs+=xs_src;
				ct+=bxs;
			}
		}
		
		nx=(xo_dev+xs+3)>>2;
		ny=(yo_dev+ys+3)>>2;

		for(i=(yo_dev>>2); i<ny; i++)
		{
			bmct=wctx->buf_dirty1+(i*wctx->size_bmxs);
			for(j=(xo_dev>>2); j<nx; j++)
			{
				if(!(j&7) && ((j+8)<=nx))
				{
					bmct[j>>3]=0xFF;
					j+=7;
					continue;
				}
				bmct[j>>3]|=1<<(j&7);
			}
		}
		wctx->dirty1=1;

		if(wctx->con)
			tkgdi_con_redrawbuffer(wctx->con);

		TKGDI_UpdateWindowCells(wctx);
		TKGDI_UpdateWindowStack();

		return(0);
	}
	
	tk_printf("TKGDI_BlitSubImage: Bad hDc=%d max=%d\n",
		dev, tkgdi_n_windows);

	return(-1);
}

TKGSTATUS TKGDI_WindowMarkDirtyRect(
	_tkgdi_context_t *ctx,
	_tkgdi_window_t *wctx,
//	TKGDI_RRECT *rect)
	int xo_src, int yo_src, int xs_src, int ys_src)
{
//	_tkgdi_window_t *wctx;
	byte *bmct;
	u16 *cs, *ct;
	u32 *pal;
	int xo_dev, yo_dev;
//	int xo_src, yo_src, xs_src, ys_src;
	int xs, ys, mxs, mys, bxs, bys, nx, ny;
	int i, j, k;

//	if(dev<=0)
//		return(-1);
	
//	xo_src=rect->xorg;
//	yo_src=rect->yorg;
//	xs_src=rect->xsize;
//	ys_src=rect->ysize;
	
//	if(xs_src<0)
//		xs_src=-xs_src;
//	if(ys_src<0)
//		ys_src=-ys_src;
	
//	return(-1);

	bxs=wctx->buf_info->biWidth;
	bys=wctx->buf_info->biHeight;
	mxs=bxs-xo_src;
	mys=bys-yo_src;
	if(xs_src>mxs)
		xs_src=mxs;
	if(ys_src>mys)
		ys_src=mys;

	nx=(xo_src+xs_src+3)>>2;
	ny=(yo_src+ys_src+3)>>2;

	for(i=(yo_src>>2); i<=ny; i++)
	{
		bmct=wctx->buf_dirty1+(i*wctx->size_bmxs);
		for(j=(xo_src>>2); j<=nx; j++)
		{
			if(!(j&7) && ((j+8)<=nx))
			{
				bmct[j>>3]=0xFF;
				j+=7;
				continue;
			}
			bmct[j>>3]|=1<<(j&7);
		}
	}
	wctx->dirty1=1;

//	TKGDI_UpdateWindowCells(wctx);
	return(0);
}
		

TKGSTATUS TKGDI_BlitSubImageOld(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	int xo_dev, int yo_dev,
	TKGDI_BITMAPINFOHEADER *info, void *data,
	int xo_src, int yo_src, int xs_src, int ys_src)
{
	TKGDI_RRECT t_rect, t_drect;
	TKGDI_RRECT *rect;
	TKGDI_RRECT *drect;
	
	rect=&t_rect;
	drect=&t_drect;
	rect->xorg=xo_src;
	rect->yorg=yo_src;
	rect->xsize=xs_src;
	rect->ysize=ys_src;

	drect->xorg=xo_dev;
	drect->yorg=yo_dev;
	drect->xsize=xs_src;
	drect->ysize=ys_src;
	
	return(ctx->vt->BlitSubImageNew(ctx,
		dev,
		drect,
		info, data,
		rect));
}

int TKGDI_ModeForInputFormat(TKGDI_BITMAPINFOHEADER *ifmt)
{
	int ofmt_mode;

	if(!ifmt)
	{
		tk_printf("TKGDI_ModeForInputFormat: Got NULL\n");
		return(-1);
	}

	ofmt_mode=-1;

	tk_printf("TKGDI_ModeForInputFormat: W=%d H=%d bpp=%d\n",
		ifmt->biWidth, ifmt->biHeight, ifmt->biBitCount);

	if((ifmt->biBitCount == 16) || (ifmt->biBitCount == 15))
	{

#if 0
		if(	(ifmt->biWidth		== 320) &&
			(ifmt->biHeight		== 200) )
		{
//			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555;		//Use hi-color
			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555_LFB;		//Use hi-color
		}
#endif

		if(	(ifmt->biWidth		== 320) &&
			(ifmt->biHeight		== 200) )
		{
			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555;		//Use hi-color
//			ofmt_mode=TKGDI_SCRMODE_320x200_RGB555_LFB;		//Use hi-color
		}

		if(	(ifmt->biWidth		== 640) &&
			(ifmt->biHeight		== 200) )
		{
			ofmt_mode=TKGDI_SCRMODE_TEXT;		//Use 80x25 color cell
		}

		if(	(ifmt->biWidth		== 640) &&
			(ifmt->biHeight		== 400) )
		{
//			ofmt_mode=TKGDI_SCRMODE_640x400_CC;	//Use 80x50 color cell
//			ofmt_mode=TKGDI_SCRMODE_640x400_RGB555;
			ofmt_mode=TKGDI_SCRMODE_640x400_PAL8B;
		}
		
		if(	(ifmt->biWidth		== 800) &&
			(ifmt->biHeight		== 600) )
		{
			ofmt_mode=TKGDI_SCRMODE_800x600_CC;	//Use 100x75 color cell
//			ofmt_mode=TKGDI_SCRMODE_800x600_PAL8B;	//Use 800x600 8bpp
		}
	}
	
	return(ofmt_mode);
}

TKGSTATUS TKGDI_QueryCreateDisplay(
	_tkgdi_context_t *ctx,
	TKGDI_BITMAPINFOHEADER *ifmt,
	TKGDI_BITMAPINFOHEADER *ofmt)
{
	int ofmt_mode;

	if(ifmt && ofmt)
	{
		return(-1);
	}

	ofmt_mode=tkgdi_vid_scrmode;

	if(ifmt)
	{
		ofmt_mode=TKGDI_ModeForInputFormat(ifmt);
	}

	if(ofmt)
	{
		ofmt->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
		ofmt->biWidth=640;
		ofmt->biHeight=200;
		ofmt->biBitCount=16;
		ofmt->biPlanes=1;
		ofmt->biCompression=0;
		ofmt->biSizeImage=ofmt->biWidth*ofmt->biHeight*2;
		ofmt->biXPelsPerMeter=72;
		ofmt->biYPelsPerMeter=72;
		ofmt->biClrUsed=0;
		ofmt->biClrImportant=0;

		if(ofmt_mode<0)
			return(0);

		if(ofmt_mode==TKGDI_SCRMODE_TEXT)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=200;
//			ofmt->biBitCount=16;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x400_CC)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
//			ofmt->biBitCount=16;
//			ofmt->biCompression=TKGDI_FCC_UTX2;
		}

		if(ofmt_mode==TKGDI_SCRMODE_800x600_CC)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
//			ofmt->biBitCount=16;
//			ofmt->biCompression=TKGDI_FCC_UTX2;
		}

		if(ofmt_mode==TKGDI_SCRMODE_320x200_RGB555)
		{
			ofmt->biWidth=320;
			ofmt->biHeight=200;
//			ofmt->biBitCount=16;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x400_RGB555)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
//			ofmt->biBitCount=16;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x480_PAL8B)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=480;
			ofmt->biBitCount=8;
		}

		if(ofmt_mode==TKGDI_SCRMODE_640x400_PAL8B)
		{
			ofmt->biWidth=640;
			ofmt->biHeight=400;
			ofmt->biBitCount=8;
		}

		if(ofmt_mode==TKGDI_SCRMODE_800x600_PAL8B)
		{
			ofmt->biWidth=800;
			ofmt->biHeight=600;
			ofmt->biBitCount=8;
		}

		ofmt->biSizeImage=
			ofmt->biWidth*
			ofmt->biHeight*
			((ofmt->biBitCount+7)>>3);

		return(1);
	}
	
	return(0);
}

TKGSTATUS TKGDI_TryMapDisplayBuffer(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_BITMAPINFOHEADER *info,
	void **rfbuf)
{
	void *ptr;

	if(dev!=1)
	{
		*rfbuf=NULL;
		return(TKGDI_STATUS_FAIL);
	}
	
	if(tkgdi_vid_scrmode==TKGDI_SCRMODE_320x200_RGB555_LFB)
	{
		*rfbuf=NULL;

		if((info->biWidth!=320) || (info->biHeight!=(-200)))
		{
			tk_printf("TKGDI_TryMapDisplayBuffer: Fail, Resolution, %d x %d\n",
				info->biWidth, info->biHeight);
			return(TKGDI_STATUS_FAIL);
		}

		if((info->biBitCount!=15) && (info->biBitCount!=16))
		{
			tk_printf("TKGDI_TryMapDisplayBuffer: Fail, Bit Count\n");
			return(TKGDI_STATUS_FAIL);
		}
		
		if(tkgdi_blitupdate_getconbuf_sticky_cnt>0)
		{
			ptr=TKGDI_BlitUpdate_GetConbuf();
			ptr=TKGDI_BlitUpdate_GetConbuf();
		}
		
		ptr=TKGDI_BlitUpdate_GetConbufFast();

		if(!ptr)
			return(TKGDI_STATUS_FAIL);
		
//		if(((((long)ptr)>>44)&15)==15)
//		{
//			tk_printf("TKGDI_TryMapDisplayBuffer: Fail, MMIO\n");
//			return(TKGDI_STATUS_FAIL);
//		}

		*rfbuf=ptr;
		return(TKGDI_STATUS_OK);
	}

	*rfbuf=NULL;
	return(TKGDI_STATUS_FAIL);
}

TKGSTATUS TKGDI_UnmapDisplayBuffer(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_BITMAPINFOHEADER *ifmt,
	void **rfbuf)
{
	if(dev!=1)
	{
		return(TKGDI_STATUS_FAIL);
	}

	return(TKGDI_STATUS_OK);
}

TKGSTATUS TKGDI_MapFlipFrame(
	_tkgdi_context_t *ctx,
	TKGHDC dev)
{
	if(dev!=1)
	{
		return(TKGDI_STATUS_FAIL);
	}
	
	TKGDI_BlitUpdate_FlipConbuf();
	return(TKGDI_STATUS_OK);
}

u32 tkgdi_eventseq;

TKGSTATUS TKGDI_DevPushEvent(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_EVENT *imsg)
{
	TKGDI_EVENTBUF *tmsg, *tmcur;
	_tkgdi_window_t *wctx;
	int didx;
	int i, j, k;

	if(dev<=0)
		return(0);

	didx=dev&0xFFFFF;

//	if(dev>1)
	if(didx>1)
	{
//		i=dev;
		i=didx;

		wctx=tkgdi_window_vis[i];
		if(!wctx)
			return(0);
		
		tmsg=TKGDI_AllocEventBuf();
		tmsg->ev=*imsg;
		
		tmsg->next=wctx->msgqueue;
		wctx->msgqueue=tmsg;

//		tk_dbg_printf("TKGDI_DevPushEvent: Reject Dev=%04X EV %08X %02X\n",
//			dev, imsg->fccMsg, imsg->wParm1);
		return(0);
	}

//	tk_dbg_printf("TKGDI_DevPushEvent: Send EV %08X %02X\n",
//		imsg->fccMsg, imsg->wParm1);
		
	if(!imsg->evSeqNum)
	{
		k=tkgdi_eventseq++;
		if(!k)
			k=tkgdi_eventseq++;
		imsg->evSeqNum=k;
	}

	for(i=0; i<tkgdi_n_window_vis; i++)
	{
		wctx=tkgdi_window_vis[i];

		if(imsg->fccMsg==TKGDI_FCC_keyb)
		{
			/* Keyboard events only go to the window with input focus. */
			if(i!=(tkgdi_n_window_vis-1))
				continue;
		}

		tmsg=TKGDI_AllocEventBuf();
		tmsg->ev=*imsg;
		tmsg->next=NULL;
		
//		tmsg->next=wctx->msgqueue;
//		wctx->msgqueue=tmsg;

		tmcur=wctx->msgqueue;
		if(tmcur)
		{
			k=0;
			while(tmcur->next)
				{ tmcur=tmcur->next; k++; }
			
			if(k>99)
			{
				/* Too many events backlogged... */
				TKGDI_FreeEventBuf(tmsg);
			}else
			{
				tmcur->next=tmsg;
			}
		}else
		{
			wctx->msgqueue=tmsg;
		}
		
		if(wctx->con && wctx->con->conrowmask)
		{
//			tkgdi_con_redrawbuffer(wctx->con);
		}
	}
	return(0);
}

TKGSTATUS TKGDI_DevPollEvent(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGDI_EVENT *imsg)
{
	_tkgdi_window_t *wctx;
	TKGDI_EVENTBUF *tmsg;
	TKGDI_RECT *rect;
	int didx, dtab;

	if(dev<1)
	{
		return(-1);
	}

	didx=dev&0xFFFFF;
	if(didx<1)
	{
		return(-1);
	}
	
//	wctx=tkgdi_windows[dev];
	wctx=tkgdi_windows[didx];
	if(!wctx)
	{
		return(-1);
	}

	dtab=(dev>>20)&15;
	if(dtab!=wctx->tabactive)
	{
		imsg->dev=0;
		imsg->fccMsg=0;
		return(0);
	}

	tmsg=wctx->msgqueue;
	if(!tmsg)
	{
		imsg->dev=0;
		imsg->fccMsg=0;
		return(0);
	}
	
	*imsg=tmsg->ev;

//	tk_dbg_printf("TKGDI_DevPollEvent: Saw EV %08X %02X\n",
//		imsg->fccMsg, imsg->wParm1);

	wctx->msgqueue=tmsg->next;
	TKGDI_FreeEventBuf(tmsg);
	
	return(1);
}

TKGSTATUS TKGDI_QueryDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	int i;

	if(parm==TKGDI_FCC_crea)
	{
		return(TKGDI_QueryCreateDisplay(ctx, ifmt, ofmt));
	}

	if(parm==TKGDI_FCC_mapf)
	{
		return(TKGDI_TryMapDisplayBuffer(ctx, dev, ifmt, ofmt));
	}
	
	if(parm==TKGDI_FCC_umap)
	{
		return(TKGDI_UnmapDisplayBuffer(ctx, dev, ifmt, ofmt));
	}
	
	if(parm==TKGDI_FCC_flip)
	{
		return(TKGDI_MapFlipFrame(ctx, dev));
	}

	if(parm==TKGDI_FCC_poll)
	{
		if(ifmt)
		{
			i=TKGDI_DevPushEvent(ctx, dev, ifmt);
			return(i);
		}

		if(ofmt)
		{
			i=TKGDI_DevPollEvent(ctx, dev, ofmt);
			return(i);
		}
		
		return(0);
	}
	
	tk_printf("TKGDI_QueryDisplay: Fail, Bad Request\n");
	return(-1);
}

TKGHDC TKGDI_CreateDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	TKGDI_BITMAPINFOHEADER *info)
{
	_tkgdi_window_t *wctx;
	int tgt_mode, domodeset;
	int xs, ys, bxs, bys, cxs, cys;
	int i, j, k;

	tk_printf("TKGDI_CreateDisplay: A\n");

	if((tkgdi_vid_scrmode<0) || (tkgdi_vid_scrmode>16))
		tkgdi_vid_scrmode=0;

	tgt_mode=TKGDI_ModeForInputFormat(info);
	
	domodeset=0;
	if(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT)
	{
		if(tgt_mode>0)
		{
			domodeset=1;
		}
	}
	
	if(tgt_mode==TKGDI_SCRMODE_TEXT)
	{
		if(tkgdi_vid_scrmode!=TKGDI_SCRMODE_TEXT)
		{
			domodeset=1;
		}
	}

	if((dev<=0) && domodeset)
	{
		/* Fullscreen modes. */

		tkgdi_blitupdate_getconbuf_sticky=0;
		tkgdi_blitupdate_getconbuf_sticky_cnt=64;

		if(tgt_mode==TKGDI_SCRMODE_TEXT)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0081;	//80x25 color-cell
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=200;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=0;
			tkgdi_vid_is8bit=0;
			tk_con_enable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x400_CC)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0085;	//80x50 color-cell
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=400;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=0;
			tkgdi_vid_is8bit=0;
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_800x600_CC)
		{
			tkgdi_vid_scrmode=tgt_mode;

			((u32 *)0xFFFFF00BFF00UL)[0]=0x008F;	//100x75 color-cell
			tkgdi_vid_cellstride=4;
			tkgdi_vid_rowstride=100*4;

//			((u32 *)0xFFFFF00BFF00UL)[0]=0x008D;	//100x75 color-cell
//			tkgdi_vid_cellstride=8;
//			tkgdi_vid_rowstride=100*8;

			tkgdi_vid_xsize=800;
			tkgdi_vid_ysize=600;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=0;
			tkgdi_vid_is8bit=0;
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_320x200_RGB555)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x0095;	//320x200x16bpp, RGB555
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=8;
			tkgdi_vid_rowstride=80*8;
			tkgdi_vid_xsize=320;
			tkgdi_vid_ysize=200;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=0;
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x400_RGB555)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x000D0095;	//640x400x16bpp, RGB555
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=2;
			tkgdi_vid_rowstride=160*2;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=400;
			tkgdi_vid_planar=1;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=0;
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x480_PAL8B)
		{
//			((u32 *)0xFFFFF00BFF00UL)[0]=0x000D0297;	//640x480x8bpp
			((u32 *)0xFFFFF00BFF00UL)[0]=0x000DC297;	//640x480x8bpp
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=1;
			tkgdi_vid_rowstride=160*1;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=480;
			tkgdi_vid_planar=1;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=1;
			tk_img_SetupPal8();
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_640x400_PAL8B)
		{
//			((u32 *)0xFFFFF00BFF00UL)[0]=0x000D0097;	//640x400x8bpp
			((u32 *)0xFFFFF00BFF00UL)[0]=0x000DC097;	//640x400x8bpp
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=1;
			tkgdi_vid_rowstride=160*1;
			tkgdi_vid_xsize=640;
			tkgdi_vid_ysize=400;
			tkgdi_vid_planar=1;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=1;
			tk_img_SetupPal8();
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_800x600_PAL8B)
		{
//			((u32 *)0xFFFFF00BFF00UL)[0]=0x000D009F;	//200x150 Pal8
//			((u32 *)0xFFFFF00BFF00UL)[0]=0x000DC09F;	//200x150 Pal8
			((u32 *)0xFFFFF00BFF00UL)[0]=0x002DC09F;	//200x150 Pal8
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=1;
			tkgdi_vid_rowstride=200*1;
			tkgdi_vid_xsize=800;
			tkgdi_vid_ysize=600;
			tkgdi_vid_planar=1;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=1;
			tk_img_SetupPal8();
			tk_con_disable();
			return(1);
		}

		if(tgt_mode==TKGDI_SCRMODE_320x200_RGB555_LFB)
		{
			((u32 *)0xFFFFF00BFF00UL)[0]=0x08000495;	//320x200x16bpp, RGB555
			tkgdi_vid_scrmode=tgt_mode;
			tkgdi_vid_cellstride=2;
			tkgdi_vid_rowstride=80;
			tkgdi_vid_xsize=320;
			tkgdi_vid_ysize=200;
			tkgdi_vid_planar=0;
			tkgdi_vid_noutx2=1;
			tkgdi_vid_is8bit=0;
			tk_con_disable();
			return(1);
		}

		tk_printf("TKGDI_CreateDisplay: Invalid Mode\n");
		return(TKGHDC_NULL);
	}

	if(dev<=1)
	{
		if(info->biCompression==TKGDI_FCC_text)
		{
			wctx=TKGDI_AllocNewConsoleWindow(ctx);
		}else
		{
			wctx=TKGDI_AllocNewWindow(ctx);
		}
		
		xs=info->biWidth;
		ys=info->biHeight;

		if(xs<0)
			xs=-xs;
		if(ys<0)
			ys=-ys;

		if(info->biCompression==TKGDI_FCC_text)
		{
			if((xs>0) && (ys)>0)
			{
				cxs=xs/80;
				cys=ys/25;
				if(cxs<4)	cxs=4;
				if(cys<6)	cys=6;
				if(cxs>8)	cxs=8;
				if(cys>8)	cys=8;
				
				if(cxs==5)
					cxs=4;
				if(cxs==7)
					cxs=6;

				if(cxs==4)
					cys=6;
				if(cxs==6)
					cys=8;
				if(cxs==8)
					cys=8;
				
				xs=cxs*80;
				ys=cys*25;
				
				wctx->con->cell_xs=cxs;
				wctx->con->cell_ys=cys;
				wctx->cell_xs=cxs;
				wctx->cell_ys=cys;
			}else
			{
				cxs=wctx->con->cell_xs;
				cys=wctx->con->cell_ys;
				xs=cxs*80;
				ys=cys*25;
				wctx->cell_xs=cxs;
				wctx->cell_ys=cys;
			}
		}

		bxs=(xs+3)>>2;
		bys=(ys+3)>>2;
		
		wctx->buf_info->biWidth=xs;
		wctx->buf_info->biHeight=ys;
		wctx->buf_info->biBitCount=16;
		
		wctx->size_bxs=bxs;
		wctx->size_bys=bys;
		wctx->size_x=bxs<<2;
		wctx->size_y=bys<<2;
		
		tk_printf("  xs=%d ys=%d\n", xs, ys);
		tk_printf("  bxs=%d bys=%d\n", bxs, bys);
		
		wctx->size_bmxs=(bxs+7)>>3;
		wctx->size_bmsz=wctx->size_bmxs*bys;
		
		wctx->buf_data=tk_malloc(xs*(ys+8)*2);
		wctx->buf_utx2=tk_malloc(bxs*(bys+2)*8);
		wctx->buf_dirty1=tk_malloc(wctx->size_bmsz+512);
		wctx->buf_dirty2=tk_malloc(wctx->size_bmsz+512);
		memset(wctx->buf_data, 0x55, xs*ys*2);
		memset(wctx->buf_dirty1, 0xFF, wctx->size_bmsz);
		memset(wctx->buf_dirty2, 0xFF, wctx->size_bmsz);
		
		wctx->dirty1=1;
		wctx->dirty2=0;

		if(tkgdi_comglue_curtask)
			wctx->owner_pid=tkgdi_comglue_curtask->pid;

		if(wctx->con)
			wctx->con->pixbuf=wctx->buf_data;

		i=tkgdi_n_window_vis++;
		tkgdi_window_vis[i]=wctx;

		if(wctx->con)
			tkgdi_con_redrawbuffer(wctx->con);

		TKGDI_UpdateWindowCells(wctx);
		TKGDI_UpdateWindowStack();
		
		return(wctx->idx);
	}
}

static u16 tkgdi_rgbi2rgb555[16]={
	0x0000, /* 0, 0.000-00.00-000.0-0000 */
	0x0015, /* 1, 0.000-00.00-000.1-0101 */
	0x02A0, /* 2, 0.000-00.10-101.0-0000 */
	0x02B5, /* 3, 0.000-00.10-101.1-0101 */
	0x5400, /* 4, 0.101-01.00-000.0-0000 */
	0x5415, /* 5, 0.101-01.00-000.1-0101 */
	0x56A0, /* 6, 0.101-01.10-101.0-0000 */
	0x56B5, /* 7, 0.101-01.10-101.1-0101 */
	0x294A, /* 8, 0.010-10.01-010.0-1010 */
	0x295F, /* 9, 0.010-10.01-010.1-1111 */
	0x2BEA, /* A, 0.010-10.11-111.0-1010 */
	0x2BFF, /* B, 0.010-10.11-111.1-1111 */
	0x7D4A, /* C, 0.111-11.01-010.0-1010 */
	0x7D5F, /* D, 0.111-11.01-010.1-1111 */
	0x7FEA, /* E, 0.111-11.11-111.0-1010 */
	0x7FFF  /* F, 0.111-11.11-111.1-1111 */
};

int TKGDI_DrawCharBasic(int xo, int yo, int ch, long long mode)
{
	u32 *conbufa;
	u64 pix;
	u32 cxa;
	int xi, yi, ci;

	conbufa=(u32 *)0xFFFFF00A0000ULL;

	if(	(tkgdi_vid_scrmode==TKGDI_SCRMODE_TEXT		)	||
		(tkgdi_vid_scrmode==TKGDI_SCRMODE_640x400_CC	)	)
	{
		if(((mode&255)==8) && !(xo&7) && !(yo&7))
		{
			xi=xo>>3;
			yi=yo>>3;
			ci=(yi*80+xi)*8;
		
			pix=TK_Con_GlyphForCodepoint(ch);
			cxa=0x80000000ULL |
				(tkgdi_rgbi2rgb555[(mode>>12)&15]<<15) |
				(tkgdi_rgbi2rgb555[(mode>> 8)&15]<< 0) ;
			conbufa[ci+0]=cxa;
			conbufa[ci+1]=0;
			conbufa[ci+2]=pix;
			conbufa[ci+3]=pix>>32;
			
			return(0);
		}
	}

	return(-1);
}

TKGSTATUS TKGDI_DrawString(
	_tkgdi_context_t *ctx,
	TKGHDC dev, int xo_dev, int yo_dev,
	char *text, TKGHFONT font, long long mode)
{
	_tkgdi_window_t *wctx;
	_tkgdi_conparm	*con;
	TKGDI_RECT *rect;

	char *s;
	int ocx, ocy, cxs, cys;
	int cx, cy, sz, ch;
	int didx, dtab;
	
	didx=dev&0xFFFF;
	dtab=(dev>>20)&15;

	if(didx>1)
	{
		wctx=tkgdi_windows[didx];
		if(!wctx)
		{
			return(-1);
		}
		
		con=wctx->contab[dtab];
		if(!con)
		{
			con=tk_malloc(sizeof(_tkgdi_conparm));
			memset(con, 0, sizeof(_tkgdi_conparm));
			tkgdi_con_init(con);
			wctx->contab[dtab]=con;
			con->cell_xs=wctx->cell_xs;
			con->cell_ys=wctx->cell_ys;
		}
		
		if(con)
		{
			ocx=con->x;
			ocy=con->y;

			cxs=con->cell_xs;
			cys=con->cell_ys;

			if((xo_dev>=0) && (yo_dev>=0))
			{
				con->x=xo_dev;
				con->y=yo_dev;
			}
			
			s=text;
			while(*s)
			{
				ch=*s++;
				cx=con->x;
				cy=con->y;
				tkgdi_con_putc(con, ch);

				if(dtab==wctx->tabactive)
				{
					TKGDI_WindowMarkDirtyRect(ctx, wctx,
						cx*cxs, cy*cys, cxs, cys);
					
					if(wctx->con->dirty&2)
					{
						TKGDI_WindowMarkDirtyRect(ctx, wctx,
							0, 0, wctx->size_x, wctx->size_y);
					}
				}
			}

			if((xo_dev>=0) && (yo_dev>=0))
			{
				con->x=ocx;
				con->y=ocy;
			}

			if(dtab==wctx->tabactive)
				TKGDI_UpdateWindowCells(wctx);
			
			return(0);
		}

		return(-1);
	}

	cx=xo_dev;
	cy=yo_dev;
	sz=mode&255;
	s=text;
	
	while(*s)
	{
		ch=*s++;
		
		if(ch=='\r')
		{
			cx=xo_dev;
			continue;
		}
		if(ch=='\n')
		{
			cx=xo_dev;
			cy+=sz;
			continue;
		}
		
		TKGDI_DrawCharBasic(cx, cy, ch, mode);
	}
	return(0);
}

TKGSTATUS TKGDI_DestroyDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev)
{
	_tkgdi_window_t *wctx;
	TKGDI_RECT *rect;
	int i, j, k;

	if(dev<2)
		return(0);

	wctx=tkgdi_windows[dev];
	if(!wctx)
		return(0);

	for(i=0; i<tkgdi_n_window_vis; i++)
	{
		if(tkgdi_window_vis[i]==wctx)
		{
			for(j=i; j<tkgdi_n_window_vis; j++)
				{ tkgdi_window_vis[j]=tkgdi_window_vis[j+1]; }
			tkgdi_n_window_vis--;
			break;
		}
	}
	
	tkgdi_windows[dev]=NULL;

	if(wctx->buf_data)
		tk_free(wctx->buf_data);
	if(wctx->buf_utx2)
		tk_free(wctx->buf_utx2);
	if(wctx->buf_dirty1)
		tk_free(wctx->buf_dirty1);
	if(wctx->buf_dirty2)
		tk_free(wctx->buf_dirty2);
	tk_free(wctx);

	TKGDI_ScreenMarkDirty();

	return(0);
}

TKGSTATUS TKGDI_ModifyDisplay(
	_tkgdi_context_t *ctx,
	TKGHDC dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	_tkgdi_window_t *wctx;
	TKGDI_RECT *rect;

	wctx=tkgdi_windows[dev];
	if(!wctx)
	{
		return(-1);
	}

	if(parm==TKGDI_FCC_move)
	{
		TKGDI_ScreenMarkDirty();

		rect=ifmt;
		wctx->base_x=rect->left;
		wctx->base_y=rect->top;
		return(1);
	}

	if(parm==TKGDI_FCC_text)
	{
		TKGDI_ScreenMarkDirty();

		if(ifmt)
		{
			wctx->title=strdup(ifmt);
		}else
		{
			wctx->title=NULL;
		}

//		rect=ifmt;
//		wctx->base_x=rect->left;
//		wctx->base_y=rect->top;
		return(1);
	}

	if(parm==TKGDI_FCC_styl)
	{
		TKGDI_ScreenMarkDirty();
		wctx->style=*(u64 *)(ifmt);
		return(1);
	}

	return(0);
}


void	TKGDI_Snd_SetupOutputMode(int chan, int rate, int bits);
void	TKGDI_Snd_WriteModeSamples2(void *mixbuf, int nsamp, int nsamp2);
void	TKGDI_Snd_Submit(void);

TKGHSND TKGDI_CreateAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHDC dev, TKGFOURCC clz, TKGDI_WAVEFORMATEX *info)
{
	TKGDI_Snd_SetupOutputMode(
		info->nChannels,
		info->nSamplesPerSec,
		info->wBitsPerSample);
	return(1);
}

TKGSTATUS TKGDI_DestroyAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev)
{
	return(0);
}

TKGSTATUS TKGDI_ModifyAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev,
	TKGFOURCC parm,
	void *ifmt,
	void *ofmt)
{
	TKGDI_MIDI_COMMAND *mcmd;
	TKGDI_MIDI_PATCHINFO *mpat;
	byte *mdat;
	int op, ch, d0, d1;

	if(parm==TKGDI_FCC_mcmd)
	{
		TK_Midi_Init();
	
		mcmd=ifmt;
		
		op=mcmd->op;
		ch=mcmd->ch;
		d0=mcmd->d0;
		d1=mcmd->d1;

		switch(mcmd->op)
		{
		case 0:
			TK_Midi_NoteOff(ch, d0, d1);
			break;
		case 1:
			TK_Midi_NoteOn(ch, d0, d1);
			break;
		case 2:
//			TK_Midi_PitchBlend(ch, d0<<6);
			TK_Midi_PitchBlend(ch, d0<<7);
			break;
		case 3:
			TK_Midi_Controller(ch,  d0, d1);
			break;
		case 4:
			TK_Midi_ProgramChange(ch,  d0);
			break;
			
		case 16:
			TK_Midi_SetFmRegisterData(d0, d1, mcmd->u0);
			break;
		case 17:
			if(d0==0)
			{
				TK_Midi_SilenceAll();
				break;
			}
			TK_Midi_SetMasterParam(d0, d1);
			break;

		default:
			break;
		}
	}
	
	if(parm==TKGDI_FCC_mpat)
	{
		mpat=ifmt;
		mdat=((byte *)ifmt)+mpat->cbSize;
		TK_Midi_UploadPatch(mpat, mdat);
	}

	return(0);
}

TKGSTATUS TKGDI_QueryAudioDevice(
	_tkgdi_context_t *ctx,
	TKGHSND dev,
	TKGFOURCC parm,
	void *info,
	void *rinfo)
{
	return(0);
}

TKGSTATUS TKGDI_WriteAudioSamples(
	_tkgdi_context_t *ctx,
	TKGHSND dev, void *buffer, int cnt, int ovcnt)
{
	if(dev==1)
	{
		if(ovcnt>0)
			TKGDI_Snd_WriteModeSamples2(buffer, cnt, ovcnt);
		TKGDI_Snd_Submit();
	}
	return(0);
}


void TKGDI_HalCleanupForTask(TKPE_TaskInfo *task)
{
	_tkgdi_window_t *wctx;
	int i, j, k;

//	if(dev<2)
//		return(0);

//	wctx=tkgdi_windows[dev];
//	if(!wctx)
//		return(0);

	for(i=2; i<tkgdi_n_windows; i++)
	{
		wctx=tkgdi_windows[i];
		if(!wctx)
			continue;
		if(wctx->owner_pid==task->pid)
		{
			TKGDI_DestroyDisplay(NULL, i);
		}
	}
	
	TK_Midi_SilenceAll();
	TKGDI_Snd_SilenceAll();

}

const _tkgdi_context_vtable_t tkgdi_context_vtable_vt = {
NULL,
NULL,

(void *)0x12345678,

TKGDI_BlitSubImageOld,
TKGDI_QueryDisplay,
TKGDI_CreateDisplay,
TKGDI_DestroyDisplay,
TKGDI_ModifyDisplay,

TKGDI_DrawString,

(void *)0x12345678,

TKGDI_CreateAudioDevice,
TKGDI_DestroyAudioDevice,
TKGDI_ModifyAudioDevice,
TKGDI_QueryAudioDevice,
TKGDI_WriteAudioSamples,

(void *)0x12345678,

TKGDI_BlitSubImageNew,
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10

(void *)0x12345678

};

// _tkgdi_context_t tkgdi_context_dfl = {
// &tkgdi_context_vtable_vt
// };

_tkgdi_context_t *tkgdi_context_ctx[256];
TKPE_TaskInfo *tkgdi_context_task[256];
int tkgdi_n_contexts;

void *TKGDI_GetHalContext(TKPE_TaskInfo *task,
	u64 apiname, u64 subname)
{
	_tkgdi_context_t *ctx;
	int i, j, k;
	
	tk_printf("TKGDI_GetHalContext:\n");
	
	for(i=0; i<tkgdi_n_contexts; i++)
	{
		if(tkgdi_context_task[i]==task)
		{
			return(tkgdi_context_ctx[i]);
		}
	}
	
	ctx=tk_malloc(sizeof(_tkgdi_context_t));
	memset(ctx, 0, sizeof(_tkgdi_context_t));
	ctx->vt=&tkgdi_context_vtable_vt;
	
	i=tkgdi_n_contexts++;
	tkgdi_context_task[i]=task;
	tkgdi_context_ctx[i]=ctx;
	
	return(ctx);
}

void tkgdi_comglue_dispatchfcn(void *obj, void *fcn, void *pret, void *args);

__asm {
tkgdi_comglue_dispatchfcn:
	ADD		-256, SP
	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R4, (SP, 192)
	MOV.Q	R5, (SP, 200)
	MOV.Q	R6, (SP, 208)
	MOV.Q	R7, (SP, 216)
	
	
	MOV		R7, R19
	MOV		R5, R3

	MOV.Q	(R19, 0), R5
	MOV.Q	(R19, 8), R6
	MOV.Q	(R19, 16), R7
	MOV.Q	(R19, 24), R20
	MOV.Q	(R19, 32), R21
	MOV.Q	(R19, 40), R22
	MOV.Q	(R19, 48), R23
	
	JSR		R3

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 208), R18
	NOP
	MOV.Q	R2, (R18)
	NOP
	NOP
	ADD		256, SP
	JMP 	R1
};

/*

VTable:
  If Vt[2]==0x12345678
    Use daisy-chained object method.
  If Vt[2]==0x789ABCDE
    Use daisy-chained VTable method (Vt[3]).

 */

void TKGDI_ComGlueDispatch(TKPE_TaskInfo *task,
	void *sObj, int idx, void *pret, void *args)
{
	void *obj1, **vt0, **vt, *fn;
	
	vt0=(*(void **)sObj);
	fn=vt0[2];
	vt=vt0[3];
//	if(fn==(void *)0x12345678)
//		vt=NULL;

//	if(vt)
	if(fn==((void *)0x789ABCDE))
	{
		vt=(void *)(((u64)vt)^tkgdi_ptrmangle_key);
		obj1=sObj;
		fn=vt[idx];
	}else if(fn==((void *)0x12345678))
	{
		obj1=((void **)sObj)[1];
		obj1=(void *)(((u64)obj1)^tkgdi_ptrmangle_key);
		vt=(void **)(*(void **)obj1);
		fn=vt[idx];
	}else
	{
		__debugbreak();
	}

	tkgdi_comglue_curtask=task;
	tkgdi_comglue_dispatchfcn(obj1, fn, pret, args);
}


void tkgdi_comglue_wrapcall3(void *obj);
void tkgdi_comglue_wrapcall4(void *obj);
void tkgdi_comglue_wrapcall5(void *obj);
void tkgdi_comglue_wrapcall6(void *obj);
void tkgdi_comglue_wrapcall7(void *obj);
void tkgdi_comglue_wrapcall8(void *obj);
void tkgdi_comglue_wrapcall9(void *obj);
void tkgdi_comglue_wrapcall10(void *obj);
void tkgdi_comglue_wrapcall11(void *obj);
void tkgdi_comglue_wrapcall12(void *obj);
void tkgdi_comglue_wrapcall13(void *obj);
void tkgdi_comglue_wrapcall14(void *obj);
void tkgdi_comglue_wrapcall15(void *obj);
void tkgdi_comglue_wrapcall16(void *obj);
void tkgdi_comglue_wrapcall17(void *obj);
void tkgdi_comglue_wrapcall18(void *obj);
void tkgdi_comglue_wrapcall19(void *obj);
void tkgdi_comglue_wrapcall20(void *obj);
void tkgdi_comglue_wrapcall21(void *obj);
void tkgdi_comglue_wrapcall22(void *obj);
void tkgdi_comglue_wrapcall23(void *obj);
void tkgdi_comglue_wrapcall24(void *obj);
void tkgdi_comglue_wrapcall25(void *obj);
void tkgdi_comglue_wrapcall26(void *obj);
void tkgdi_comglue_wrapcall27(void *obj);
void tkgdi_comglue_wrapcall28(void *obj);
void tkgdi_comglue_wrapcall29(void *obj);
void tkgdi_comglue_wrapcall30(void *obj);
void tkgdi_comglue_wrapcall31(void *obj);

void tkgdi_comglue_rv_wrapcall3(void *obj);
void tkgdi_comglue_rv_wrapcall4(void *obj);
void tkgdi_comglue_rv_wrapcall5(void *obj);
void tkgdi_comglue_rv_wrapcall6(void *obj);
void tkgdi_comglue_rv_wrapcall7(void *obj);
void tkgdi_comglue_rv_wrapcall8(void *obj);
void tkgdi_comglue_rv_wrapcall9(void *obj);
void tkgdi_comglue_rv_wrapcall10(void *obj);
void tkgdi_comglue_rv_wrapcall11(void *obj);
void tkgdi_comglue_rv_wrapcall12(void *obj);
void tkgdi_comglue_rv_wrapcall13(void *obj);
void tkgdi_comglue_rv_wrapcall14(void *obj);
void tkgdi_comglue_rv_wrapcall15(void *obj);
void tkgdi_comglue_rv_wrapcall16(void *obj);
void tkgdi_comglue_rv_wrapcall17(void *obj);
void tkgdi_comglue_rv_wrapcall18(void *obj);
void tkgdi_comglue_rv_wrapcall19(void *obj);
void tkgdi_comglue_rv_wrapcall20(void *obj);
void tkgdi_comglue_rv_wrapcall21(void *obj);
void tkgdi_comglue_rv_wrapcall22(void *obj);
void tkgdi_comglue_rv_wrapcall23(void *obj);
void tkgdi_comglue_rv_wrapcall24(void *obj);
void tkgdi_comglue_rv_wrapcall25(void *obj);
void tkgdi_comglue_rv_wrapcall26(void *obj);
void tkgdi_comglue_rv_wrapcall27(void *obj);
void tkgdi_comglue_rv_wrapcall28(void *obj);
void tkgdi_comglue_rv_wrapcall29(void *obj);
void tkgdi_comglue_rv_wrapcall30(void *obj);
void tkgdi_comglue_rv_wrapcall31(void *obj);

int tk_syscall_utxt(void *sobj, int umsg, void *pptr, void *args);
int tk_syscall_rv_utxt(void *sobj, int umsg, void *pptr, void *args);

__asm {
.section .utext

.balign	16

tk_syscall_utxt:
	nop
	nop
	keybrk_xg2
	nop
	nop
	syscall	0
	nop
	nop
	nop
	nop
	nop
	keybrk_xg2
	nop
	keybrk_xg2
	nop
	nop
	rts
	nop
	nop
	nop
	nop
	brk
	brk

tkgdi_comglue_wrapcall_gen:
	ADD		-256, SP
	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R4, (SP, 128)
	MOV.Q	R5, (SP, 136)
	MOV.Q	R6, (SP, 144)
	MOV.Q	R7, (SP, 152)
	MOV.Q	R20, (SP, 160)
	MOV.Q	R21, (SP, 168)
	MOV.Q	R22, (SP, 176)
	MOV.Q	R23, (SP, 184)
	
	MOV		R3, R5
	LEA.B	(SP, 112), R6
//	LEA.B	(SP, 128), R7
	LEA.B	(SP, 136), R7
	BSR		tk_syscall_utxt

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 112), R2
	MOV.Q	(SP, 120), R3

	ADD		256, SP
	JMP 	R1

tkgdi_comglue_wrapcall3:
	MOV		TK_UMSG_COMGLUE_VMT3, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall4:
	MOV		TK_UMSG_COMGLUE_VMT4, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall5:
	MOV		TK_UMSG_COMGLUE_VMT5, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall6:
	MOV		TK_UMSG_COMGLUE_VMT6, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall7:
	MOV		TK_UMSG_COMGLUE_VMT7, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall8:
	MOV		TK_UMSG_COMGLUE_VMT8, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall9:
	MOV		TK_UMSG_COMGLUE_VMT9, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall10:
	MOV		TK_UMSG_COMGLUE_VMT10, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall11:
	MOV		TK_UMSG_COMGLUE_VMT11, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall12:
	MOV		TK_UMSG_COMGLUE_VMT12, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall13:
	MOV		TK_UMSG_COMGLUE_VMT13, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall14:
	MOV		TK_UMSG_COMGLUE_VMT14, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall15:
	MOV		TK_UMSG_COMGLUE_VMT15, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall16:
	MOV		TK_UMSG_COMGLUE_VMT16, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall17:
	MOV		TK_UMSG_COMGLUE_VMT17, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall18:
	MOV		TK_UMSG_COMGLUE_VMT18, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall19:
	MOV		TK_UMSG_COMGLUE_VMT19, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall20:
	MOV		TK_UMSG_COMGLUE_VMT20, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall21:
	MOV		TK_UMSG_COMGLUE_VMT21, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall22:
	MOV		TK_UMSG_COMGLUE_VMT22, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall23:
	MOV		TK_UMSG_COMGLUE_VMT23, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall24:
	MOV		TK_UMSG_COMGLUE_VMT24, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall25:
	MOV		TK_UMSG_COMGLUE_VMT25, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall26:
	MOV		TK_UMSG_COMGLUE_VMT26, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall27:
	MOV		TK_UMSG_COMGLUE_VMT27, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall28:
	MOV		TK_UMSG_COMGLUE_VMT28, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall29:
	MOV		TK_UMSG_COMGLUE_VMT29, R3
	BRA		tkgdi_comglue_wrapcall_gen

tkgdi_comglue_wrapcall30:
	MOV		TK_UMSG_COMGLUE_VMT30, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall31:
	MOV		TK_UMSG_COMGLUE_VMT31, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall32:
	MOV		TK_UMSG_COMGLUE_VMT32, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall33:
	MOV		TK_UMSG_COMGLUE_VMT33, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall34:
	MOV		TK_UMSG_COMGLUE_VMT34, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall35:
	MOV		TK_UMSG_COMGLUE_VMT35, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall36:
	MOV		TK_UMSG_COMGLUE_VMT36, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall37:
	MOV		TK_UMSG_COMGLUE_VMT37, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall38:
	MOV		TK_UMSG_COMGLUE_VMT38, R3
	BRA		tkgdi_comglue_wrapcall_gen
tkgdi_comglue_wrapcall39:
	MOV		TK_UMSG_COMGLUE_VMT39, R3
	BRA		tkgdi_comglue_wrapcall_gen


#if 1
.riscv

tk_syscall_rv_utxt:
	nop
	nop
	nop
	nop
	syscall
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	rts
	nop
	nop
	nop
	nop

tkgdi_comglue_rv_wrapcall_gen:
	ADD		-256, SP
//	MOV		LR, R1
	MOV.Q	R1, (SP, 248)

	MOV.Q	R10, (SP, 128)
	MOV.Q	R11, (SP, 136)
	MOV.Q	R12, (SP, 144)
	MOV.Q	R13, (SP, 152)
	MOV.Q	R14, (SP, 160)
	MOV.Q	R15, (SP, 168)
	MOV.Q	R16, (SP, 176)
	MOV.Q	R17, (SP, 184)
	
	MOV		R5, R11
	ADD		SP, 112, R12
	ADD		SP, 136, R13
	BSR		tk_syscall_utxt

	MOV.Q	(SP, 248), R1
	MOV.Q	(SP, 112), R10
	MOV.Q	(SP, 120), R11

	ADD		256, SP
	JMP 	R1

tkgdi_comglue_rv_wrapcall3:
	MOV		TK_UMSG_COMGLUE_VMT3, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall4:
	MOV		TK_UMSG_COMGLUE_VMT4, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall5:
	MOV		TK_UMSG_COMGLUE_VMT5, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall6:
	MOV		TK_UMSG_COMGLUE_VMT6, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall7:
	MOV		TK_UMSG_COMGLUE_VMT7, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

tkgdi_comglue_rv_wrapcall8:
	MOV		TK_UMSG_COMGLUE_VMT8, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall9:
	MOV		TK_UMSG_COMGLUE_VMT9, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

tkgdi_comglue_rv_wrapcall10:
	MOV		TK_UMSG_COMGLUE_VMT10, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall11:
	MOV		TK_UMSG_COMGLUE_VMT11, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall12:
	MOV		TK_UMSG_COMGLUE_VMT12, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall13:
	MOV		TK_UMSG_COMGLUE_VMT13, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall14:
	MOV		TK_UMSG_COMGLUE_VMT14, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall15:
	MOV		TK_UMSG_COMGLUE_VMT15, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall16:
	MOV		TK_UMSG_COMGLUE_VMT16, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall17:
	MOV		TK_UMSG_COMGLUE_VMT17, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall18:
	MOV		TK_UMSG_COMGLUE_VMT18, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall19:
	MOV		TK_UMSG_COMGLUE_VMT19, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

tkgdi_comglue_rv_wrapcall20:
	MOV		TK_UMSG_COMGLUE_VMT20, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall21:
	MOV		TK_UMSG_COMGLUE_VMT21, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall22:
	MOV		TK_UMSG_COMGLUE_VMT22, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall23:
	MOV		TK_UMSG_COMGLUE_VMT23, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall24:
	MOV		TK_UMSG_COMGLUE_VMT24, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall25:
	MOV		TK_UMSG_COMGLUE_VMT25, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall26:
	MOV		TK_UMSG_COMGLUE_VMT26, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall27:
	MOV		TK_UMSG_COMGLUE_VMT27, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall28:
	MOV		TK_UMSG_COMGLUE_VMT28, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen
tkgdi_comglue_rv_wrapcall29:
	MOV		TK_UMSG_COMGLUE_VMT29, R5
	BRA		tkgdi_comglue_rv_wrapcall_gen

.endriscv
#endif
};


const _tkgdi_context_vtable_t tkgdi_context_vtable_gvt = {
NULL,						//0
NULL,						//1
(void *)0x12345678,			//2
tkgdi_comglue_wrapcall3,	//3
tkgdi_comglue_wrapcall4,
tkgdi_comglue_wrapcall5,
tkgdi_comglue_wrapcall6,
tkgdi_comglue_wrapcall7,
tkgdi_comglue_wrapcall8,
(void *)0x12345678,			//9
tkgdi_comglue_wrapcall10,	//10
tkgdi_comglue_wrapcall11,
tkgdi_comglue_wrapcall12,
tkgdi_comglue_wrapcall13,
tkgdi_comglue_wrapcall14,	//14
(void *)0x12345678,			//15
tkgdi_comglue_wrapcall16,	//16
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10
(void *)0x12345678
};

const _tkgdi_context_vtable_t tkgdi_context_vtable_grvvt = {
NULL,						//0
NULL,						//1
(void *)0x12345678,			//2
tkgdi_comglue_rv_wrapcall3,	//3
tkgdi_comglue_rv_wrapcall4,
tkgdi_comglue_rv_wrapcall5,
tkgdi_comglue_rv_wrapcall6,
tkgdi_comglue_rv_wrapcall7,
tkgdi_comglue_rv_wrapcall8,
(void *)0x12345678,			//9
tkgdi_comglue_rv_wrapcall10,	//10
tkgdi_comglue_rv_wrapcall11,
tkgdi_comglue_rv_wrapcall12,
tkgdi_comglue_rv_wrapcall13,
tkgdi_comglue_rv_wrapcall14,	//14
(void *)0x12345678,			//15
tkgdi_comglue_rv_wrapcall16,	//16
NULL,	//1
NULL,	//2
NULL,	//3
NULL,	//4
NULL,	//5
NULL,	//6
NULL,	//7
NULL,	//8
NULL,	//9
NULL,	//10
(void *)0x12345678
};

void *tkgdi_transvt_orgvt[256];
void *tkgdi_transvt_trgvt_jx[256];
void *tkgdi_transvt_trgvt_rv[256];
int tkgdi_transvt_n_vt;

void *TKGDI_GetWrapVTableForTask(TKPE_TaskInfo *task,
	void *orgvt, int size)
{
	TKPE_TaskInfo *ctask;
	void **vt_o, **vt_jx, **vt_rv;
	void *fn, *ovt1;
	u64 uli;
	int i, j, k, n;

	ctask=TK_GetCurrentTask();

	if(ctask==task)
		return(orgvt);
	
	for(i=0; i<tkgdi_transvt_n_vt; i++)
	{
		if(tkgdi_transvt_orgvt[i]==orgvt)
		{
			if(TK_Task_SyscallTaskIsRiscV(task))
			{
				return(tkgdi_transvt_trgvt_rv[i]);
			}else
			{
				return(tkgdi_transvt_trgvt_jx[i]);
			}
		}
	}
	
	if(!tkgdi_ptrmangle_key)
	{
		tkgdi_ptrmangle_key=TK_GetRandom48ASLR();
	}
	
	ovt1=(void *)(((u64)orgvt)^tkgdi_ptrmangle_key);
	
	i=tkgdi_transvt_n_vt++;
	tkgdi_transvt_orgvt[i]=orgvt;
	
	n=size/sizeof(void *);
	vt_o=orgvt;
	vt_jx=tk_malloc(n*sizeof(void *));
	vt_rv=tk_malloc(n*sizeof(void *));
	
	memset(vt_jx, 0, n*sizeof(void *));
	memset(vt_rv, 0, n*sizeof(void *));

	vt_jx[2]=(void *)0x789ABCDE;
	vt_rv[2]=(void *)0x789ABCDE;
//	vt_jx[3]=orgvt;
//	vt_rv[3]=orgvt;
	vt_jx[3]=ovt1;
	vt_rv[3]=ovt1;

	tkgdi_transvt_trgvt_jx[i]=vt_jx;
	tkgdi_transvt_trgvt_rv[i]=vt_rv;
	
	for(j=4; j<n; j++)
	{
		fn=vt_o[j];
		if(!fn)
		{
			vt_jx[j]=NULL;
			vt_rv[j]=NULL;
			continue;
		}
		
		fn=NULL;
		switch(j)
		{
			case  4: fn=tkgdi_comglue_wrapcall4; break;
			case  5: fn=tkgdi_comglue_wrapcall5; break;
			case  6: fn=tkgdi_comglue_wrapcall6; break;
			case  7: fn=tkgdi_comglue_wrapcall7; break;

			case  8: fn=tkgdi_comglue_wrapcall8; break;
			case  9: fn=tkgdi_comglue_wrapcall9; break;

			case 10: fn=tkgdi_comglue_wrapcall10; break;
			case 11: fn=tkgdi_comglue_wrapcall11; break;
			case 12: fn=tkgdi_comglue_wrapcall12; break;
			case 13: fn=tkgdi_comglue_wrapcall13; break;
			case 14: fn=tkgdi_comglue_wrapcall14; break;
			case 15: fn=tkgdi_comglue_wrapcall15; break;
			case 16: fn=tkgdi_comglue_wrapcall16; break;
			case 17: fn=tkgdi_comglue_wrapcall17; break;
			case 18: fn=tkgdi_comglue_wrapcall18; break;
			case 19: fn=tkgdi_comglue_wrapcall19; break;

			case 20: fn=tkgdi_comglue_wrapcall20; break;
			case 21: fn=tkgdi_comglue_wrapcall21; break;
			case 22: fn=tkgdi_comglue_wrapcall22; break;
			case 23: fn=tkgdi_comglue_wrapcall23; break;
			case 24: fn=tkgdi_comglue_wrapcall24; break;
			case 25: fn=tkgdi_comglue_wrapcall25; break;
			case 26: fn=tkgdi_comglue_wrapcall26; break;
			case 27: fn=tkgdi_comglue_wrapcall27; break;
			case 28: fn=tkgdi_comglue_wrapcall28; break;
			case 29: fn=tkgdi_comglue_wrapcall29; break;
		}
		
		uli=(u64)fn;
		uli&=0x0000FFFFFFFFFFFEULL;
		uli|=0x0000000000000001ULL;
		fn=(void *)uli;
		
		vt_jx[j]=fn;

		fn=NULL;
		switch(j)
		{
			case  4: fn=tkgdi_comglue_rv_wrapcall4; break;
			case  5: fn=tkgdi_comglue_rv_wrapcall5; break;
			case  6: fn=tkgdi_comglue_rv_wrapcall6; break;
			case  7: fn=tkgdi_comglue_rv_wrapcall7; break;

			case  8: fn=tkgdi_comglue_rv_wrapcall8; break;
			case  9: fn=tkgdi_comglue_rv_wrapcall9; break;

			case 10: fn=tkgdi_comglue_rv_wrapcall10; break;
			case 11: fn=tkgdi_comglue_rv_wrapcall11; break;
			case 12: fn=tkgdi_comglue_rv_wrapcall12; break;
			case 13: fn=tkgdi_comglue_rv_wrapcall13; break;
			case 14: fn=tkgdi_comglue_rv_wrapcall14; break;
			case 15: fn=tkgdi_comglue_rv_wrapcall15; break;
			case 16: fn=tkgdi_comglue_rv_wrapcall16; break;
			case 17: fn=tkgdi_comglue_rv_wrapcall17; break;
			case 18: fn=tkgdi_comglue_rv_wrapcall18; break;
			case 19: fn=tkgdi_comglue_rv_wrapcall19; break;

			case 20: fn=tkgdi_comglue_rv_wrapcall20; break;
			case 21: fn=tkgdi_comglue_rv_wrapcall21; break;
			case 22: fn=tkgdi_comglue_rv_wrapcall22; break;
			case 23: fn=tkgdi_comglue_rv_wrapcall23; break;
			case 24: fn=tkgdi_comglue_rv_wrapcall24; break;
			case 25: fn=tkgdi_comglue_rv_wrapcall25; break;
			case 26: fn=tkgdi_comglue_rv_wrapcall26; break;
			case 27: fn=tkgdi_comglue_rv_wrapcall27; break;
			case 28: fn=tkgdi_comglue_rv_wrapcall28; break;
			case 29: fn=tkgdi_comglue_rv_wrapcall29; break;
		}
		
//		uli=(u64)fn;
//		uli&=0x0000FFFFFFFFFFFEULL;
//		uli|=0x0000000000000001ULL;
//		fn=(void *)uli;
		
		vt_rv[j]=fn;
	}

	if(TK_Task_SyscallTaskIsRiscV(task))
	{
		return(tkgdi_transvt_trgvt_rv[i]);
	}else
	{
		return(tkgdi_transvt_trgvt_jx[i]);
	}
}

_tkgdi_context_vtable_t *tkgdi_context_vtable_gvtc;
_tkgdi_context_vtable_t *tkgdi_context_vtable_grvvtc;

_tkgdi_context_t *tkgdi_gcontext_ctx[256];
TKPE_TaskInfo *tkgdi_gcontext_task[256];
int tkgdi_n_gcontexts;

void *TKGDI_GetHalContextComGlue(TKPE_TaskInfo *task,
	u64 apiname, u64 subname)
{
	void **ppv, *pv;
	_tkgdi_context_t *ctx;
	_tkgdi_context_t *ctx2;
	TKPE_TaskInfo *ctask;
	u64 lv;
	int i, j, k, n;

	if(((u32)apiname)!=TK_FCC_GDI)
		return(NULL);

	tk_printf("TKGDI_GetHalContext:\n");
	
	for(i=0; i<tkgdi_n_gcontexts; i++)
	{
		if(tkgdi_gcontext_task[i]==task)
		{
			return(tkgdi_gcontext_ctx[i]);
		}
	}

	ctask=TK_GetCurrentTask();

	ctx2=TKGDI_GetHalContext(task, apiname, subname);
	
	if(ctask==task)
		return(ctx2);
	
	if(!tkgdi_context_vtable_gvtc)
	{
		tkgdi_context_vtable_gvtc=tk_malloc_usr(
			sizeof(_tkgdi_context_vtable_t));
		tkgdi_context_vtable_grvvtc=tk_malloc_usr(
			sizeof(_tkgdi_context_vtable_t));
		memcpy(tkgdi_context_vtable_gvtc,
			&tkgdi_context_vtable_gvt,
			sizeof(_tkgdi_context_vtable_t));
		memcpy(tkgdi_context_vtable_grvvtc,
			&tkgdi_context_vtable_grvvt,
			sizeof(_tkgdi_context_vtable_t));
			
		n=sizeof(_tkgdi_context_vtable_t)/sizeof(void *);
		ppv=(void **)tkgdi_context_vtable_gvtc;
		for(i=0; i<n; i++)
		{
			pv=ppv[i];
			lv=(u64)pv;
			if(!lv)
				continue;
			if(lv==0x12345678)
				continue;

			lv&=0x0000FFFFFFFFFFFEULL;
			lv|=0x0000000000000001ULL;
			ppv[i]=(void *)lv;
		}
	}

	if(!tkgdi_ptrmangle_key)
	{
		tkgdi_ptrmangle_key=TK_GetRandom48ASLR();
	}

	ctx=tk_malloc_usr(sizeof(_tkgdi_context_t));
	memset(ctx, 0, sizeof(_tkgdi_context_t));
//	ctx->vt=&tkgdi_context_vtable_gvt;
	ctx->vt=tkgdi_context_vtable_gvtc;
	
	if(TK_Task_SyscallTaskIsRiscV(task))
	{
		ctx->vt=tkgdi_context_vtable_grvvtc;
	}
	
	ctx->sobj=TKMM_LVA_ExportSharedObjectPtr(ctx2);

	ctx->sobj=(void *)(((u64)ctx->sobj)^tkgdi_ptrmangle_key);

	i=tkgdi_n_gcontexts++;
	tkgdi_gcontext_task[i]=task;
	tkgdi_gcontext_ctx[i]=ctx;
	
	return(ctx);
}
