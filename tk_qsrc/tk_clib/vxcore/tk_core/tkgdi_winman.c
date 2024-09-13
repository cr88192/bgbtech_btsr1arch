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
int tkgdi_vid_bxs;
int tkgdi_vid_bys;

int tkgdi_vid_bmxsize;

u64 *tkgdi_vid_screenutx;		//screen UTX2 buffer
u64 *tkgdi_vid_screenrgb;		//screen RGB buffer
byte *tkgdi_vid_screendirty;		//screen dirtry-mask buffer

byte tk_img_d9to8tab[512];
// byte tk_img_d15to8tab0[32768];
// byte tk_img_d15to8tab1[32768];
// u16 tk_img_d8to15tab[256];
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
		if(tmp->magic0!=TKGDI_CTX_MAGIC)
			__debugbreak();
		tkgdi_freemsg=tmp->next;
		tmp->next=NULL;
		return(tmp);
	}
	
	tmp=tk_malloc_krn(sizeof(TKGDI_EVENTBUF));
	if(!tmp)
	{
		__debugbreak();
		return(NULL);
	}
	memset(tmp, 0, sizeof(TKGDI_EVENTBUF));
	tmp->magic0=TKGDI_CTX_MAGIC;
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
	
	if((xs<=0) || (ys<=0))
	{
		__debugbreak();
	}
	
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
	
	if((x|y)<0)
		return(-1);
	if(x>=tkgdi_vid_bxs)
		return(-1);
	if(y>=tkgdi_vid_bys)
		return(-1);
	
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
	
	if((x|y)<0)
		return(-1);
	if(x>=(tkgdi_vid_bxs<<1))
		return(-1);
	if(y>=(tkgdi_vid_bys<<1))
		return(-1);
	
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
	
	if((x|y)<0)
		return(-1);
	if(x>=(tkgdi_vid_bxs<<1))
		return(-1);
	if(y>=(tkgdi_vid_bys<<1))
		return(-1);
	
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

#if 1
	if((x|y)<0)
		return(-1);
	if(x>=(tkgdi_vid_bxs<<1))
		return(-1);
	if(y>=(tkgdi_vid_bys<<1))
		return(-1);
#endif
	
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

#if 1
	if((x|y)<0)
		return(-1);
	if(x>=(tkgdi_vid_bxs<<1))
		return(-1);
	if(y>=(tkgdi_vid_bys<<1))
		return(-1);
#endif

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
	
#if 1
	if((x|y)<0)
		return(-1);
	if(x>=(tkgdi_vid_bxs<<1))
		return(-1);
	if(y>=(tkgdi_vid_bys<<1))
		return(-1);
#endif

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

int TKGDI_UpdateWindowStack_PushWinVirtKey(int dev, int key)
{
	TKGDI_EVENT t_imsg;
	TKGDI_EVENT *imsg;
	int i, j, k;

	imsg=&t_imsg;

	imsg->evSize=sizeof(TKGDI_EVENT);
	imsg->dev=1;
	imsg->fccMsg=TKGDI_FCC_keyb;
	imsg->ptMsec=TK_GetTimeMs();
	imsg->wParm1=key;
	imsg->wParm2=0;
	imsg->lParm1=0;
	imsg->ptMsX=tkgdi_ps2ms_lx;
	imsg->ptMsY=tkgdi_ps2ms_ly;
	imsg->ptMsB=tkgdi_ps2ms_lb;

	TKGDI_DevPushEvent(NULL, dev, imsg);
	
	return(0);
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
		con=tk_malloc_krn(sizeof(_tkgdi_conparm));
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
						TKGDI_UpdateWindowStack_PushWinVirtKey(
							wctx->idx, TK_K_GUI_CLOSE);
						break;
					}

					if(	(tkgdi_ps2ms_lx1>=(wctx->base_x+wctx->size_x-16)) &&
						(tkgdi_ps2ms_lx1<(wctx->base_x+wctx->size_x-8))
						)
					{
						tk_printf("TKGDI_UpdateWindowStack: Click Minimize\n");
						TKGDI_UpdateWindowStack_PushWinVirtKey(
							wctx->idx, TK_K_GUI_MINIMIZE);
						break;
					}

					if(	(tkgdi_ps2ms_lx1>=(wctx->base_x+wctx->size_x-8)) &&
						(tkgdi_ps2ms_lx1<(wctx->base_x+wctx->size_x-0))
						)
					{
						tk_printf("TKGDI_UpdateWindowStack: Click Maximize\n");
						TKGDI_UpdateWindowStack_PushWinVirtKey(
							wctx->idx, TK_K_GUI_MAXIMIZE);
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
	{
//		tkgdi_vid_screendirty=tk_malloc(bmsz+(4*tkgdi_vid_bmxsize));

//		tkgdi_vid_screendirty=tk_malloc(bmsz+(16*tkgdi_vid_bmxsize));
		tkgdi_vid_screendirty=tk_malloc_krn(bmsz+(16*tkgdi_vid_bmxsize));
		tkgdi_vid_screendirty+=8*tkgdi_vid_bmxsize;
		memset(tkgdi_vid_screendirty, 0xFF, bmsz);
	}
	
	
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
			tkgdi_vid_screenrgb=tk_malloc_krn(bxs*bys*(4*8));
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

		for(i=0; i<tkgdi_n_window_vis; i++)
		{
			wctx=tkgdi_window_vis[i];
			issel=(i==(tkgdi_n_window_vis-1));
			
//			x=i-1;
			x=i;

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

		wutx=(u64 *)tkgdi_vid_screenrgb;
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
			{ tkgdi_vid_screenutx=tk_malloc_krn(bxs2*bys2*(4*8)); }

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
	
	wctx=tk_malloc_krn(sizeof(_tkgdi_window_t));
	memset(wctx, 0, sizeof(_tkgdi_window_t));
	
	wctx->magic0=TKGDI_CTX_MAGIC;
	wctx->magic1=TKGDI_CTX_MAGIC;
	
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
	
	con=tk_malloc_krn(sizeof(_tkgdi_conparm));
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
