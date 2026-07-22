GfxEdit_Context *GfxEdit_CreateContext()
{
	GfxEdit_Context *ctx;
	ctx=malloc(sizeof(GfxEdit_Context));
	memset(ctx, 0, sizeof(GfxEdit_Context));
	
	ctx->screen=malloc(320*200*2);
	memset(ctx->screen, 0, 320*200*2);
	
	return(ctx);
}

GfxEdit_UndoLevel *GfxEdit_AllocUndoRect(GfxEdit_Context *ctx,
	int x0, int y0, int x1, int y1, int flags)
{
	GfxEdit_UndoLevel *tmp;
	
	tmp=ctx->undo_free;
	if(tmp)
	{
		ctx->undo_free=tmp->undo;
		tmp->undo=NULL;
		tmp->redo=NULL;
		memset(tmp, 0, sizeof(GfxEdit_UndoLevel));
	}else
	{
		tmp=malloc(sizeof(GfxEdit_UndoLevel));
		memset(tmp, 0, sizeof(GfxEdit_UndoLevel));
	}
	
	tmp->x0=x0; tmp->y0=y0;
	tmp->x1=x1; tmp->y1=y1;
	tmp->xs=(x1-x0)+1;
	tmp->ys=(y1-y0)+1;
	tmp->flags=flags;

	if(flags&1)
	{
		tmp->pixels_old=malloc(tmp->xs*tmp->ys);
		tmp->pixels_new=malloc(tmp->xs*tmp->ys);
	}
	
	if(flags&4)
	{
		tmp->pal4_old=malloc(256*4);
		tmp->pal4_new=malloc(256*4);
	}

	return(tmp);
}

void GfxEdit_FreeUndo(GfxEdit_Context *ctx, GfxEdit_UndoLevel *tmp)
{
	if(tmp->pixels_old)
		{ free(tmp->pixels_old); tmp->pixels_old=NULL; }
	if(tmp->pixels_new)
		{ free(tmp->pixels_new); tmp->pixels_new=NULL; }

	if(tmp->pal4_old)
		{ free(tmp->pal4_old); tmp->pal4_old=NULL; }
	if(tmp->pal4_new)
		{ free(tmp->pal4_new); tmp->pal4_new=NULL; }

	tmp->redo=NULL;
	tmp->undo=ctx->undo_free;
	ctx->undo_free=tmp;
}

void GfxEdit_DoUndoLimit(GfxEdit_Context *ctx)
{
	GfxEdit_UndoLevel *tmp, *nxt;
	int n;
	
	tmp=ctx->undo; n=0;
	while(tmp && tmp->undo)
		{ n++; tmp=tmp->undo; }
	while(n>=512)
	{
		nxt=tmp->redo;
		nxt->undo=NULL;
		GfxEdit_FreeUndo(ctx, tmp);
		n--;
		tmp=nxt;
	}
	
	tmp=ctx->undo;
	if(tmp && tmp->redo)
	{
		nxt=tmp->redo;
		tmp->redo=NULL;
		
		tmp=nxt;
		while(tmp)
		{
			nxt=tmp->redo;
			GfxEdit_FreeUndo(ctx, tmp);
			tmp=nxt;
		}
	}
}

void GfxEdit_MarkUndoRect(GfxEdit_Context *ctx,
	int x0, int y0, int x1, int y1, int flags)
{
	GfxEdit_UndoLevel *tmp;
	int x, y;

	if(ctx->undo_inhibit)
		return;

	if(x1<x0)
		{ x=x0; x0=x1; x1=x; }
	if(y1<y0)
		{ y=y0; y0=y1; y1=y; }
	if(x0<0)	x0=0;
	if(y0<0)	y0=0;
	if(x1>=ctx->canvas_width)	x1=ctx->canvas_width-1;
	if(y1>=ctx->canvas_height)	y1=ctx->canvas_height-1;

	if((x1<x0) || (y1<y0))
		{ flags&=~3; }

	GfxEdit_DoUndoLimit(ctx);

	if(!ctx->undo)
	{
		ctx->undo=GfxEdit_AllocUndoRect(ctx, 0, 0, 0, 0, 0);
	}

	tmp=GfxEdit_AllocUndoRect(ctx, x0, y0, x1, y1, flags);
	tmp->layer_cur=ctx->layer_cur;
	
	if(flags&1)
	{
		for(y=0; y<tmp->ys; y++)
		{
			memcpy(
				tmp->pixels_old+(y*tmp->xs),
//				ctx->canvas_pixels+(y0+y)*ctx->canvas_width+x0, tmp->xs);
				ctx->layer_pixels+(y0+y)*ctx->canvas_width+x0, tmp->xs);
		}
	}

	if(flags&2)
	{
//		tmp->pix_old=ctx->canvas_pixels[y0*ctx->canvas_width+x0];
		tmp->pix_old=ctx->layer_pixels[y0*ctx->canvas_width+x0];
	}
	
	if(flags&4)
	{
		memcpy(tmp->pal4_old, ctx->canvas_pal4, 256*4);
		tmp->pal4_bpp_old=ctx->canvas_bpp;
	}

	tmp->undo=ctx->undo;
	if(ctx->undo)
		ctx->undo->redo=tmp;
	ctx->undo=tmp;
}

void GfxEdit_MarkRedoRect(GfxEdit_Context *ctx)
{
	GfxEdit_UndoLevel *tmp;
	int y;
	
	if(ctx->undo_inhibit)
		return;

	tmp=ctx->undo;

	if(tmp->flags&1)
	{
		for(y=0; y<tmp->ys; y++)
		{
			memcpy(
				tmp->pixels_new+(y*tmp->xs),
//				ctx->canvas_pixels+(tmp->y0+y)*ctx->canvas_width+tmp->x0,
				ctx->layer_pixels+(tmp->y0+y)*ctx->canvas_width+tmp->x0,
				tmp->xs);
		}
	}

	if(tmp->flags&2)
	{
//		tmp->pix_new=ctx->canvas_pixels[tmp->y0*ctx->canvas_width+tmp->x0];
		tmp->pix_new=ctx->layer_pixels[tmp->y0*ctx->canvas_width+tmp->x0];
	}	

	if(tmp->flags&4)
	{
		memcpy(tmp->pal4_new, ctx->canvas_pal4, 256*4);
		tmp->pal4_bpp_new=ctx->canvas_bpp;
	}

	ctx->undo_inhibit=1;
}

void GfxEdit_MarkUndoPix(GfxEdit_Context *ctx, int x, int y)
{
	GfxEdit_MarkUndoRect(ctx, x, y, x, y, 2);
}

void GfxEdit_MarkRedoPix(GfxEdit_Context *ctx)
{
	GfxEdit_MarkRedoRect(ctx);
}

void GfxEdit_MarkUndoAll(GfxEdit_Context *ctx, int flags)
{
	GfxEdit_MarkUndoRect(ctx, 0, 0,
		ctx->canvas_width-1,
		ctx->canvas_height-1,
		flags);
}

void GfxEdit_MarkRedoAll(GfxEdit_Context *ctx)
{
	GfxEdit_MarkRedoRect(ctx);
}

void GfxEdit_MarkUndoPal(GfxEdit_Context *ctx)
{
	GfxEdit_MarkUndoAll(ctx, 1|4);
}

void GfxEdit_MarkRedoPal(GfxEdit_Context *ctx)
{
	GfxEdit_MarkRedoRect(ctx);
}


void GfxEdit_DoUndo(GfxEdit_Context *ctx)
{
	GfxEdit_UndoLevel *tmp;
	int oldlyr;
	int y;
	
	tmp=ctx->undo;
	if(!tmp->undo)
		return;

	oldlyr=ctx->layer_cur;
	GfxEdit_SelectLayer(ctx, tmp->layer_cur);
	
	if(tmp->flags&1)
	{
		for(y=0; y<tmp->ys; y++)
		{
			memcpy(
//				ctx->canvas_pixels+(tmp->y0+y)*ctx->canvas_width+tmp->x0,
				ctx->layer_pixels+(tmp->y0+y)*ctx->canvas_width+tmp->x0,
				tmp->pixels_old+(y*tmp->xs),
				tmp->xs);
		}
	}
	if(tmp->flags&2)
	{
//		ctx->canvas_pixels[tmp->y0*ctx->canvas_width+tmp->x0]=tmp->pix_old;
		ctx->layer_pixels[tmp->y0*ctx->canvas_width+tmp->x0]=tmp->pix_old;
	}

	if(tmp->flags&4)
	{
		memcpy(ctx->canvas_pal4, tmp->pal4_old, 256*4);
		ctx->canvas_bpp=tmp->pal4_bpp_old;
		GfxEdit_SetupPalFinish(ctx);
	}
	
	GfxEdit_SelectLayer(ctx, oldlyr);

	ctx->undo=tmp->undo;
}

void GfxEdit_DoRedo(GfxEdit_Context *ctx)
{
	GfxEdit_UndoLevel *tmp;
	int y, oldlyr;
	
	tmp=ctx->undo;
	if(!tmp->redo)
		return;
	tmp=tmp->redo;
	ctx->undo=tmp;

	oldlyr=ctx->layer_cur;
	GfxEdit_SelectLayer(ctx, tmp->layer_cur);
	
	if(tmp->flags&1)
	{
		for(y=0; y<tmp->ys; y++)
		{
			memcpy(
//				ctx->canvas_pixels+(tmp->y0+y)*ctx->canvas_width+tmp->x0,
				ctx->layer_pixels+(tmp->y0+y)*ctx->canvas_width+tmp->x0,
				tmp->pixels_new+(y*tmp->xs),
				tmp->xs);
		}
	}
	if(tmp->flags&2)
	{
//		ctx->canvas_pixels[tmp->y0*ctx->canvas_width+tmp->x0]=tmp->pix_new;
		ctx->layer_pixels[tmp->y0*ctx->canvas_width+tmp->x0]=tmp->pix_new;
	}

	if(tmp->flags&4)
	{
		memcpy(ctx->canvas_pal4, tmp->pal4_new, 256*4);
		ctx->canvas_bpp=tmp->pal4_bpp_new;
		GfxEdit_SetupPalFinish(ctx);
	}

	GfxEdit_SelectLayer(ctx, oldlyr);
}

void GfxEdit_BucketFill(GfxEdit_Context *ctx, int cax, int cay, int clr)
{
	short stk_x[256], stk_y[256];
	byte *ibuf, *csl;
	int stkpos, clr_ref, xs, ys;
	int x, y, x0, x1, x2, x3;
	
	xs=ctx->canvas_width;
	ys=ctx->canvas_height;
//	ibuf=ctx->canvas_pixels;
	ibuf=ctx->layer_pixels;
	clr_ref=ibuf[cay*xs+cax];
	if(clr_ref==clr)
		return;
	stk_x[0]=cax;
	stk_y[0]=cay;
	stkpos=1;
	
	while(stkpos>0)
	{
		stkpos--;
		x=stk_x[stkpos];
		y=stk_y[stkpos];
		if(ibuf[y*xs+x]!=clr_ref)
			continue;
		
		x0=x; x1=x;		
		csl=ibuf+y*xs;
		while((x0>0) && (csl[x0-1]==clr_ref))x0--;
		while((x1<(xs-1)) && (csl[x1+1]==clr_ref))x1++;
		memset(csl+x0, clr, (x1-x0)+1);
		
		if((stkpos+12)>256)
			continue;
		
		x2=(x+x0)>>1;
		x3=(x+x1)>>1;
		
		if(y>0)
		{
			if(ibuf[(y-1)*xs+x]==clr_ref)
				{ stk_x[stkpos]=x; stk_y[stkpos]=y-1; stkpos++; }
			if(x0<x)
			{
				if(ibuf[(y-1)*xs+x0]==clr_ref)
					{ stk_x[stkpos]=x0; stk_y[stkpos]=y-1; stkpos++; }
				if((x2!=x0) && (x2!=x) && (ibuf[(y-1)*xs+x2]==clr_ref))
					{ stk_x[stkpos]=x2; stk_y[stkpos]=y-1; stkpos++; }
			}
			if(x1>x)
			{
				if(ibuf[(y-1)*xs+x1]==clr_ref)
					{ stk_x[stkpos]=x1; stk_y[stkpos]=y-1; stkpos++; }
				if((x3!=x1) && (x3!=x) && (ibuf[(y-1)*xs+x3]==clr_ref))
					{ stk_x[stkpos]=x3; stk_y[stkpos]=y-1; stkpos++; }
			}
		}
		if(y<(ys-1))
		{
			if(ibuf[(y+1)*xs+x]==clr_ref)
				{ stk_x[stkpos]=x ; stk_y[stkpos]=y+1; stkpos++; }
			if(x0<x)
			{
				if(ibuf[(y+1)*xs+x0]==clr_ref)
					{ stk_x[stkpos]=x0; stk_y[stkpos]=y+1; stkpos++; }
				if((x2!=x0) && (x2!=x) && (ibuf[(y+1)*xs+x2]==clr_ref))
					{ stk_x[stkpos]=x2; stk_y[stkpos]=y+1; stkpos++; }
			}
			if(x1>x)
			{
				if(ibuf[(y+1)*xs+x1]==clr_ref)
					{ stk_x[stkpos]=x1; stk_y[stkpos]=y+1; stkpos++; }
				if((x3!=x1) && (x3!=x) && (ibuf[(y+1)*xs+x3]==clr_ref))
					{ stk_x[stkpos]=x3; stk_y[stkpos]=y+1; stkpos++; }
			}
		}
	}
}

void GfxEdit_DoBoxPaste(GfxEdit_Context *ctx, int fl)
{
	byte *src_pix, *dst_pix;
	int caw, cah;
	int x, y, x0, y0, x1, y1, z0, z1, xd, yd, c;
	int xini, xfin, xsgn, yini, yfin, ysgn;
	int p_xr, p_yr, p_x0, p_y0;

	src_pix=ctx->layer_pixels;
	dst_pix=ctx->layer_pixels;
	p_xr=ctx->paste_xr;
	p_yr=ctx->paste_yr;
	p_x0=ctx->paste_x0;
	p_y0=ctx->paste_y0;

	caw=ctx->canvas_width;
	cah=ctx->canvas_height;
	xd=ctx->paste_xd;
	yd=ctx->paste_yd;
	
	if(fl&4)
	{
		if(!ctx->clip_pixels)
		{
			ctx->clip_pixels=malloc(caw*cah);
			memset(ctx->clip_pixels, ctx->clr_trans, caw*cah);
		}

		dst_pix=ctx->clip_pixels;

		x0=ctx->line_x0;	y0=ctx->line_y0;
		x1=ctx->line_x1;	y1=ctx->line_y1;
		if(x1<x0)	{ x=x0; x0=x1; x1=x; }
		if(y1<y0)	{ y=y0; y0=y1; y1=y; }

		p_xr=x0;
		p_yr=y0;
		xd=x1-x0;
		yd=y1-y0;

//		p_xr=ctx->paste_xr;
//		p_yr=ctx->paste_yr;
		p_x0=p_xr;
		p_y0=p_yr;
	}
	
	if(fl&8)
	{
		if(!ctx->clip_pixels)
		{
			ctx->clip_pixels=malloc(caw*cah);
			memset(ctx->clip_pixels, ctx->clr_trans, caw*cah);
		}

		src_pix=ctx->clip_pixels;

		p_xr=ctx->clip_x0;
		p_yr=ctx->clip_y0;
		p_x0=ctx->paste_x0;
		p_y0=ctx->paste_y0;
		xd=(ctx->clip_x1-ctx->clip_x0);
		yd=(ctx->clip_y1-ctx->clip_y0);
	}
	
	if(p_xr<0)		return;
	if(p_yr<0)		return;
	if(p_x0<0)		return;
	if(p_y0<0)		return;
	
	if((p_xr+xd)>=caw)		xd=(caw-1)-p_xr;
	if((p_x0+xd)>=caw)		xd=(caw-1)-p_x0;
	if((p_yr+yd)>=cah)		yd=(cah-1)-p_yr;
	if((p_y0+yd)>=cah)		yd=(cah-1)-p_y0;
	if(xd<0)		return;
	if(yd<0)		return;

	if(fl&4)
	{
		ctx->clip_x0=p_xr;
		ctx->clip_y0=p_yr;
		ctx->clip_x1=p_xr+xd;
		ctx->clip_y1=p_yr+yd;
	}

	xini=0;		yini=0;
	xfin=xd;	yfin=yd;
	xsgn=1;		ysgn=1;

	if(fl&1)
	{
		if(p_xr>p_x0)	{ xini=xd; xfin=0; xsgn=-1; }
		if(p_yr>p_y0)	{ yini=yd; yfin=0; ysgn=-1; }
	}else
	{
		if(p_xr<p_x0)	{ xini=xd; xfin=0; xsgn=-1; }
		if(p_yr<p_y0)	{ yini=yd; yfin=0; ysgn=-1; }
	}
	
	for(y=yini; (y-ysgn)!=yfin; y+=ysgn)
		for(x=xini; (x-xsgn)!=xfin; x+=xsgn)
	{
		x0=p_xr+x;	y0=p_yr+y;
		x1=p_x0+x;	y1=p_y0+y;
		z0=y0*caw+x0;
		z1=y1*caw+x1;

		if((x0<caw) && (y0<cah))
		{
//			c=ctx->canvas_pixels[z0];
//			c=ctx->layer_pixels[z0];
			c=src_pix[z0];
			
			if(fl&2)
			{
//				ctx->layer_pixels[z0]=ctx->clr_trans;
				src_pix[z0]=ctx->clr_trans;
			}
		}else
		{
			c=ctx->clr_trans;
		}
	
//		ctx->canvas_pixels[z1]=c;
//		ctx->layer_pixels[z1]=c;
		dst_pix[z1]=c;
	}
}

void GfxEdit_CopyClip(GfxEdit_Context *ctx)
{
	GfxEdit_DoBoxPaste(ctx, 4);
}

void GfxEdit_CutClip(GfxEdit_Context *ctx)
{
	GfxEdit_DoBoxPaste(ctx, 6);
}

void GfxEdit_PasteClip(GfxEdit_Context *ctx)
{
	GfxEdit_DoBoxPaste(ctx, 8);
}

void GfxEdit_StrokeBrush(GfxEdit_Context *ctx, int cax, int cay, int clr)
{
	byte *pixels;
	int caw, cah, bsz;
	int x, y, x0, x1, y0, y1, dx, dy, d;

	pixels=ctx->layer_pixels;
	caw=ctx->canvas_width;
	cah=ctx->canvas_height;
	bsz=ctx->brushsize;

	if(!bsz)
	{
		pixels[cay*caw+cax]=clr;
		return;
	}
	
	x0=cax-bsz;		y0=cay-bsz;
	x1=cax+bsz;		y1=cay+bsz;
	if(x0<0)		x0=0;
	if(y0<0)		y0=0;
	if(x1>(caw-1))	x1=(caw-1);
	if(y1>(cah-1))	y1=(cah-1);

	for(y=y0; y<=y1; y++)
		for(x=x0; x<=x1; x++)
	{
#if 1
		dx=x-cax;	dy=y-cay;
		d=dx*dx+dy*dy;
		d=gfxedit_isqrtapx(d);
#endif
#if 0
		dx=x-cax;	dy=y-cay;
		if(dx<0)	dx=-dx;
		if(dy<0)	dy=-dy;
		if(dx>dy)
			d=dx+(dy>>1);
		else
			d=dy+(dx>>1);
#endif
		if(d>bsz)continue;
		pixels[y*caw+x]=clr;
	}
	
}

void GfxEdit_MouseClickDown(GfxEdit_Context *ctx, int mx, int my, int mb)
{
	int cax, cay, tx, ty, z, zc8;

	if(ctx->condown)
		return;

	if(mx>=32)
	{
		if(ctx->zoom>=0)
		{
			cax=((mx-32)>>ctx->zoom)+ctx->canvas_xorg;
			cay=(my>>ctx->zoom)+ctx->canvas_yorg;
		}else
		{
			return;
		}
		
		if(cax>=ctx->canvas_width)
			return;
		if(cay>=ctx->canvas_height)
			return;
		
		if(ctx->undo_inhibit)
		{
			if(cax!=ctx->undo_inhibit_cax)
				ctx->undo_inhibit=0;
			if(cay!=ctx->undo_inhibit_cay)
				ctx->undo_inhibit=0;
		}

		ctx->undo_inhibit_cax=cax;
		ctx->undo_inhibit_cay=cay;
		
//		if(	(ctx->sel_tool==GFXEDIT_TOOL_PENCIL) ||
//			(ctx->sel_tool==GFXEDIT_TOOL_BRUSH)	)
		if(ctx->sel_tool==GFXEDIT_TOOL_PENCIL)
		{
			z=cay*ctx->canvas_width+cax;
			GfxEdit_MarkUndoPix(ctx, cax, cay);
			if(mb&1)
//				{ ctx->canvas_pixels[z]=ctx->sel_color1; }
				{ ctx->layer_pixels[z]=ctx->sel_color1; }
			if(mb&4)
//				{ ctx->canvas_pixels[z]=ctx->sel_color2; }
				{ ctx->layer_pixels[z]=ctx->sel_color2; }
			GfxEdit_MarkRedoPix(ctx);
			ctx->redraw_img_dirty=1;
			return;
		}

		if(ctx->sel_tool==GFXEDIT_TOOL_BRUSH)
		{
			z=cay*ctx->canvas_width+cax;
			GfxEdit_MarkUndoRect(ctx,
				cax-ctx->brushsize, cay-ctx->brushsize,
				cax+ctx->brushsize, cay+ctx->brushsize,
				1);
			if(mb&1)
				{ GfxEdit_StrokeBrush(ctx, cax, cay, ctx->sel_color1); }
//				{ ctx->canvas_pixels[z]=ctx->sel_color1; }
//				{ ctx->layer_pixels[z]=ctx->sel_color1; }
			if(mb&4)
				{ GfxEdit_StrokeBrush(ctx, cax, cay, ctx->sel_color2); }
//				{ ctx->canvas_pixels[z]=ctx->sel_color2; }
//				{ ctx->layer_pixels[z]=ctx->sel_color2; }
			GfxEdit_MarkRedoRect(ctx);
			ctx->redraw_img_dirty=1;
			return;
		}

		if(ctx->sel_tool==GFXEDIT_TOOL_PICK)
		{
			if(ctx->canvas_bpp==1)
				return;
		
			z=cay*ctx->canvas_width+cax;
			if(mb&1)
//				{ ctx->sel_color1=ctx->canvas_pixels[z]; }
				{ ctx->sel_color1=ctx->layer_pixels[z]; }
			if(mb&4)
//				{ ctx->sel_color2=ctx->canvas_pixels[z]; }
				{ ctx->sel_color2=ctx->layer_pixels[z]; }
			ctx->redraw_img_dirty=1;
			ctx->redraw_tools_dirty=1;
			return;
		}
		if(ctx->sel_tool==GFXEDIT_TOOL_ERASESMOL)
		{
			z=cay*ctx->canvas_width+cax;
			if(mb&5)
			{
				GfxEdit_MarkUndoPix(ctx, cax, cay);
//				ctx->canvas_pixels[z]=ctx->clr_trans;
				ctx->layer_pixels[z]=ctx->clr_trans;
				GfxEdit_MarkRedoPix(ctx);
			}
			ctx->redraw_img_dirty=1;
			return;
		}

		if(ctx->sel_tool==GFXEDIT_TOOL_ERASEBIG)
		{
			z=cay*ctx->canvas_width+cax;
			if(mb&5)
			{
//				GfxEdit_MarkUndoPix(ctx, cax, cay);
				GfxEdit_MarkUndoRect(ctx,
					cax-ctx->brushsize, cay-ctx->brushsize,
					cax+ctx->brushsize, cay+ctx->brushsize,
					1);
				GfxEdit_StrokeBrush(ctx, cax, cay, ctx->clr_trans);
				GfxEdit_MarkRedoPix(ctx);
			}
			ctx->redraw_img_dirty=1;
			return;
		}

		if(ctx->sel_tool==GFXEDIT_TOOL_BUCKET)
		{		
			GfxEdit_MarkUndoAll(ctx, 1);
			if(mb&1)
				{ GfxEdit_BucketFill(ctx, cax, cay, ctx->sel_color1); }
			if(mb&4)
				{ GfxEdit_BucketFill(ctx, cax, cay, ctx->sel_color2); }
			GfxEdit_MarkRedoRect(ctx);
			ctx->redraw_img_dirty=1;
			return;
		}

		if(	(ctx->sel_tool==GFXEDIT_TOOL_LINE) ||
			(ctx->sel_tool==GFXEDIT_TOOL_BOXSEL) ||
			(ctx->sel_tool==GFXEDIT_TOOL_BOX)	 ||
			(ctx->sel_tool==GFXEDIT_TOOL_CIRC)	)
		{		
			if(mb&1)
				{ ctx->line_x0=cax; ctx->line_y0=cay; }
			if(mb&4)
				{ ctx->line_x1=cax; ctx->line_y1=cay; }
			ctx->redraw_img_dirty=1;
			return;
		}

		if(		(ctx->sel_tool==GFXEDIT_TOOL_PASTESEL) ||
				(ctx->sel_tool==GFXEDIT_TOOL_PASTEREP) ||
				(ctx->sel_tool==GFXEDIT_TOOL_MOVESEL) )
		{
			if(mb&5)
				{ ctx->paste_x0=cax; ctx->paste_y0=cay; }
			ctx->redraw_img_dirty=1;
		}
		return;
	}else
	{
		tx=mx/4;
		ty=(my-72)/4;
		zc8=ty*8+tx;

		tx=mx/8;
		ty=my/8;
		z=ty*4+tx;
		
		if(z<GFXEDIT_TOOL_MAX)
		{
			ctx->sel_tool=z;
			ctx->redraw_tools_dirty=1;
			ctx->redraw_img_dirty=1;
			return;
		}
		
		if(	(ctx->canvas_bpp> 1) &&
			(ctx->canvas_bpp<=5))
		{
			if(	(z>=GFXEDIT_CLRBOX_MIN5) &&
				(z<GFXEDIT_CLRBOX_MAX5) &&
				((z-GFXEDIT_CLRBOX_MIN5)<(1<<(ctx->canvas_bpp))))
			{
				if(mb&1)
					ctx->sel_color1=z-GFXEDIT_CLRBOX_MIN5;
				if(mb&4)
					ctx->sel_color2=z-GFXEDIT_CLRBOX_MIN5;
				ctx->redraw_tools_dirty=1;
			}
		}

		if(ctx->canvas_bpp==8)
		{
			if((zc8>=0) && (zc8<=255))
			{
				if(mb&1)
					ctx->sel_color1=zc8;
				if(mb&4)
					ctx->sel_color2=zc8;
				ctx->redraw_tools_dirty=1;
			}
		}
	}
}

void GfxEdit_MouseClickUp(GfxEdit_Context *ctx, int mx, int my, int mb)
{
	ctx->undo_inhibit=0;
}

void GfxEdit_UseDrawLine(GfxEdit_Context *ctx,
	int x0, int y0, int x1, int y1, int clr)
{
	byte *img;
	int x, y, zf, ixs, iys, xdist, ydist, xsgn, ysgn;
	
//	img=ctx->canvas_pixels;
	img=ctx->layer_pixels;
	ixs=ctx->canvas_width;
	iys=ctx->canvas_height;
	
	xdist=x1-x0;	xsgn=1;
	ydist=y1-y0;	ysgn=1;
	if(xdist<0)		{ xdist=-xdist; xsgn=-1; }
	if(ydist<0)		{ ydist=-ydist; ysgn=-1; }

	img[y0*ixs+x0]=clr;
	img[y1*ixs+x1]=clr;
	
	if(xdist>ydist)
	{
		x=x0; y=y0; zf=0;
		while(x!=x1)
		{
			img[y*ixs+x]=clr;
			x+=xsgn; zf+=ydist;
			if(zf>=xdist)
				{ y+=ysgn; zf-=xdist; }
		}
	}else
	{
		x=x0; y=y0; zf=0;
		while(y!=y1)
		{
			img[y*ixs+x]=clr;
			y+=ysgn; zf+=xdist;
			if(zf>=ydist)
				{ x+=xsgn; zf-=ydist; }
		}
	}
}

void GfxEdit_DoBoxFill(GfxEdit_Context *ctx,
	int x0, int y0, int x1, int y1, int clr)
{
	byte *img;
	int x, y, zf, ixs, iys, xdist, ydist, xsgn, ysgn;
	
//	img=ctx->canvas_pixels;
	img=ctx->layer_pixels;
	ixs=ctx->canvas_width;
	iys=ctx->canvas_height;
	
	if(x1<x0)
		{ x=x1; x1=x0; x0=x; }
	if(y1<y0)
		{ y=y1; y1=y0; y0=y; }
	
	if(x0<0)		x0=0;
	if(y0<0)		y0=0;
	if(x1>(ixs-1))	x1=0;
	if(y1>(iys-1))	y1=0;
	if((x1<x0) || (y1<y0))
		return;
	
	for(y=y0; y<=y1; y++)
	{
		for(x=x0; x<=x1; x++)
			{ img[y*ixs+x]=clr; }
	}
}

void GfxEdit_DoDrawBox(GfxEdit_Context *ctx,
	int x0, int y0, int x1, int y1, int clr)
{
	GfxEdit_UseDrawLine(ctx, x0, y0, x1, y0, clr);
	GfxEdit_UseDrawLine(ctx, x1, y0, x1, y1, clr);
	GfxEdit_UseDrawLine(ctx, x0, y1, x1, y1, clr);
	GfxEdit_UseDrawLine(ctx, x0, y0, x0, y1, clr);
}

void GfxEdit_DoDrawCircle(GfxEdit_Context *ctx,
	int x0, int y0, int x1, int y1, int clr)
{
	static float sctab[256];
	float cx, cy, rx, ry, f, s;
	int i, j, n, j0, j1;

	if(sctab[64]==0)
	{
		for(i=0; i<256; i++)
			sctab[i]=sin(i*(3.1415926535899/128));
	}

	cx=(x0+x1)*0.5;	cy=(y0+y1)*0.5;
	rx=x1-x0;		ry=y1-y0;
	if(rx<0)		rx=-rx;
	if(ry<0)		ry=-ry;
	rx*=0.5;		ry*=0.5;

//	n=gfxedit_log2up(rx+ry)-2;
//	if(n<1)	n=1;
//	n=n*16;

	n=6.3*((rx+ry)*0.5+1.0)*0.5;
	if(n>256)	n=256;

	f=0;	s=256.0/n;
	for(i=0; i<n; i++)
	{
		j0=f; f+=s; j1=f;
		GfxEdit_UseDrawLine(ctx,
			cx+sctab[(j0+ 0)&255]*rx+0.5,
			cy+sctab[(j0+64)&255]*ry+0.5,
			cx+sctab[(j1+ 0)&255]*rx+0.5,
			cy+sctab[(j1+64)&255]*ry+0.5,
			clr);
	}

//	GfxEdit_UseDrawLine(ctx, x0, y0, x1, y0, clr);
//	GfxEdit_UseDrawLine(ctx, x1, y0, x1, y1, clr);
//	GfxEdit_UseDrawLine(ctx, x0, y1, x1, y1, clr);
//	GfxEdit_UseDrawLine(ctx, x0, y0, x0, y1, clr);
}

void GfxEdit_PumpKey(GfxEdit_Context *ctx, int key, int down)
{
	int step;
	int l, p;

	if(key==K_CTRL)
		{ ctx->keydown_ctrl=down; }
	if(key==K_SHIFT)
		{ ctx->keydown_shift=down; }

	if(down)
	{
		if(key=='`')
		{
			ctx->condown=!ctx->condown;
			ctx->redraw_tools_dirty=1;
			ctx->redraw_img_dirty=1;
			ctx->redraw_con_dirty=1;
			return;
		}

		if(ctx->condown)
		{
			if(key==K_LEFTARROW)
			{
				if(ctx->con_cur_pos>0)
					ctx->con_cur_pos--;
			}

			if(key==K_RIGHTARROW)
			{
				if(ctx->con_cur_pos<strlen(ctx->conline))
					ctx->con_cur_pos++;
			}

			if(key==K_BACKSPACE)
			{
				l=strlen(ctx->conline);
				p=ctx->con_cur_pos;
				if(p>0)
				{
					memmove(ctx->conline+p-1, ctx->conline+p, (l-p)+1);
					ctx->con_cur_pos=p-1;
				}
			}

			if((key=='\r') || (key=='\n'))
			{
				GfxEdit_ConPrintf(ctx, " %s\n", ctx->conline);
				GfxEdit_ConRunCmd(ctx, ctx->conline);
				ctx->conline[0]=0;
				ctx->con_cur_pos=0;
			}

			if((key>=0x20) && (key<=0x7E))
			{
				l=strlen(ctx->conline);
				p=ctx->con_cur_pos;
				if(p<l)
				{
					memmove(ctx->conline+p+1, ctx->conline+p, (l-p)+1);
					ctx->conline[p]=key;
				}else
				{
					ctx->conline[p]=key;
					ctx->conline[p+1]=0;
				}
				
				ctx->con_cur_pos=p+1;
			}
		
			if(ctx->con_cur_pos<ctx->con_cur_xbase)
				ctx->con_cur_xbase=ctx->con_cur_pos;
			if(ctx->con_cur_pos>(ctx->con_cur_xbase+39))
				ctx->con_cur_xbase=ctx->con_cur_pos-39;
			ctx->redraw_con_dirty=1;
			return;
		}
		
		if(ctx->keydown_ctrl)
		{
			if((key=='s') || (key=='S'))
			{
				if(ctx->imgname)
				{
//					GfxEdit_ConRun_Save(ctx, NULL);
					GfxEdit_ConRunCmd(ctx, "save");
				}
			}

			if((key=='l') || (key=='L'))
			{
				if(ctx->imgname)
				{
//					GfxEdit_ConRun_Load(ctx, NULL);
					GfxEdit_ConRunCmd(ctx, "load");
				}
			}

			if((key=='z') || (key=='Z'))
			{
				GfxEdit_DoUndo(ctx);
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}

			if((key=='y') || (key=='Y'))
			{
				GfxEdit_DoRedo(ctx);
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}

			if(key=='n')
			{
				GfxEdit_AddLayer(ctx);
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}

			if(key=='c')
			{
				GfxEdit_CopyClip(ctx);
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}

			if(key=='x')
			{
				GfxEdit_MarkUndoAll(ctx, 1);
				GfxEdit_CutClip(ctx);
				GfxEdit_MarkRedoRect(ctx);
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}

			if(key=='v')
			{
				GfxEdit_MarkUndoAll(ctx, 1);
				GfxEdit_PasteClip(ctx);
				GfxEdit_MarkRedoRect(ctx);
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}

			return;
		}

		if((key=='-') || (key==K_NUMPAD_SUB))
		{
//			if(ctx->zoom>0)
			if(ctx->zoom>(-4))
			{
				ctx->zoom--;
				ctx->redraw_img_dirty=1;
			}
		}
		if((key=='+') || (key=='=') || (key==K_NUMPAD_ADD))
		{
			if(ctx->zoom<3)
			{
				ctx->zoom++;
				ctx->redraw_img_dirty=1;
			}
		}
		
		if(ctx->zoom>=0)
			{ step=8>>ctx->zoom; }
		else
			{ step=8<<(-ctx->zoom); }
		if(!step)
			step=1;
		
		if((key==K_LEFTARROW) || (key==K_NUMPAD4))
			{ ctx->canvas_xorg-=step; ctx->redraw_img_dirty=1; }
		if((key==K_UPARROW) || (key==K_NUMPAD8))
			{ ctx->canvas_yorg-=step; ctx->redraw_img_dirty=1; }
		if((key==K_RIGHTARROW) || (key==K_NUMPAD6))
			{ ctx->canvas_xorg+=step; ctx->redraw_img_dirty=1; }
		if((key==K_DOWNARROW) || (key==K_NUMPAD2))
			{ ctx->canvas_yorg+=step; ctx->redraw_img_dirty=1; }

		if(key==K_NUMPAD5)
		{
			ctx->canvas_xorg=0;
			ctx->canvas_yorg=0;
			ctx->zoom=1;
			ctx->redraw_img_dirty=1;
		}

		if(!ctx->keydown_shift)
		{
			if((key>='0') && (key<='9'))
			{
				GfxEdit_SelectLayer(ctx, key-'0');
			}
		}

		if(key=='{')
		{
			if(	(ctx->sel_tool==GFXEDIT_TOOL_BRUSH) ||
				(ctx->sel_tool==GFXEDIT_TOOL_ERASEBIG))
			{
				if(ctx->brushsize>0)
					ctx->brushsize--;
			}
			ctx->redraw_tools_dirty=1;
		}

		if(key=='}')
		{
			if(	(ctx->sel_tool==GFXEDIT_TOOL_BRUSH) ||
				(ctx->sel_tool==GFXEDIT_TOOL_ERASEBIG))
			{
				if(ctx->brushsize<255)
					ctx->brushsize++;
			}
			ctx->redraw_tools_dirty=1;
		}
		
		if(ctx->layer_max)
		{
			if(key=='[')
			{
				if(ctx->layer_cur>0)
					{ GfxEdit_SelectLayer(ctx, ctx->layer_cur-1); }
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}
			if(key==']')
			{
				if(ctx->layer_cur<(ctx->layer_max-1))
					{ GfxEdit_SelectLayer(ctx, ctx->layer_cur+1); }
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}

			if(key=='/')
			{
				GfxEdit_FlipLayerVisible(ctx);
				ctx->redraw_img_dirty=1;
				ctx->redraw_tools_dirty=1;
			}
		}

		if((key==K_ENTER) || (key==K_INS))
		{
			if(ctx->sel_tool==GFXEDIT_TOOL_LINE)
			{
				if(	(ctx->line_x0>=0) && (ctx->line_y0>=0) &&
					(ctx->line_x1>=0) && (ctx->line_y1>=0))
				{
					GfxEdit_MarkUndoRect(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						1);
					GfxEdit_UseDrawLine(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						ctx->sel_color1);
					GfxEdit_MarkRedoRect(ctx);
					ctx->redraw_img_dirty=1;
				}
			}
			
			if(ctx->sel_tool==GFXEDIT_TOOL_PASTESEL)
			{
				GfxEdit_MarkUndoRect(ctx,
					ctx->paste_x0, ctx->paste_y0,
					ctx->paste_x1, ctx->paste_y1,
					1);
//				GfxEdit_MarkUndoAll(ctx, 1);
				GfxEdit_DoBoxPaste(ctx, 0);
				GfxEdit_MarkRedoRect(ctx);
				ctx->redraw_img_dirty=1;
			}
			if(ctx->sel_tool==GFXEDIT_TOOL_PASTEREP)
			{
				GfxEdit_MarkUndoRect(ctx,
					ctx->paste_x0, ctx->paste_y0,
					ctx->paste_x1, ctx->paste_y1,
					1);
//				GfxEdit_MarkUndoAll(ctx, 1);
				GfxEdit_DoBoxPaste(ctx, 1);
				GfxEdit_MarkRedoRect(ctx);
				ctx->redraw_img_dirty=1;
			}
			if(ctx->sel_tool==GFXEDIT_TOOL_MOVESEL)
			{
//				GfxEdit_MarkUndoRect(ctx,
//					ctx->paste_x0, ctx->paste_y0,
//					ctx->paste_x1, ctx->paste_y1,
//					1);
				GfxEdit_MarkUndoAll(ctx, 1);
				GfxEdit_DoBoxPaste(ctx, 2);
				GfxEdit_MarkRedoRect(ctx);
				ctx->redraw_img_dirty=1;
			}

			if(ctx->sel_tool==GFXEDIT_TOOL_BOX)
			{
				if(	(ctx->line_x0>=0) && (ctx->line_y0>=0) &&
					(ctx->line_x1>=0) && (ctx->line_y1>=0))
				{
					GfxEdit_MarkUndoRect(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						1);
					GfxEdit_DoDrawBox(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						ctx->sel_color1);
					GfxEdit_MarkRedoRect(ctx);
					ctx->redraw_img_dirty=1;
				}
			}

			if(ctx->sel_tool==GFXEDIT_TOOL_CIRC)
			{
				if(	(ctx->line_x0>=0) && (ctx->line_y0>=0) &&
					(ctx->line_x1>=0) && (ctx->line_y1>=0))
				{
					GfxEdit_MarkUndoRect(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						1);
					GfxEdit_DoDrawCircle(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						ctx->sel_color1);
					GfxEdit_MarkRedoRect(ctx);
					ctx->redraw_img_dirty=1;
				}
			}
		}

		if((key==K_BACKSPACE) || (key==K_DEL))
		{
			if(ctx->sel_tool==GFXEDIT_TOOL_LINE)
			{
				if(	(ctx->line_x0>=0) && (ctx->line_y0>=0) &&
					(ctx->line_x1>=0) && (ctx->line_y1>=0))
				{
					GfxEdit_MarkUndoRect(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						1);
					GfxEdit_UseDrawLine(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						ctx->sel_color2);
					GfxEdit_MarkRedoRect(ctx);
					ctx->redraw_img_dirty=1;
				}
			}

			if(ctx->sel_tool==GFXEDIT_TOOL_BOXSEL)
			{
				p=ctx->sel_color2;
				if(key==K_DEL)
					p=ctx->clr_trans;
				GfxEdit_MarkUndoRect(ctx,
					ctx->line_x0, ctx->line_y0,
					ctx->line_x1, ctx->line_y1,
					1);
				GfxEdit_DoBoxFill(ctx,
					ctx->line_x0, ctx->line_y0,
					ctx->line_x1, ctx->line_y1,
					p);
				GfxEdit_MarkRedoRect(ctx);
				ctx->redraw_img_dirty=1;
			}

			if(ctx->sel_tool==GFXEDIT_TOOL_BOX)
			{
				p=ctx->sel_color2;
				if(key==K_DEL)
					p=ctx->clr_trans;
				if(	(ctx->line_x0>=0) && (ctx->line_y0>=0) &&
					(ctx->line_x1>=0) && (ctx->line_y1>=0))
				{
					GfxEdit_MarkUndoRect(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						1);
					GfxEdit_DoDrawBox(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						p);
					GfxEdit_MarkRedoRect(ctx);
					ctx->redraw_img_dirty=1;
				}
			}

			if(ctx->sel_tool==GFXEDIT_TOOL_CIRC)
			{
				p=ctx->sel_color2;
				if(key==K_DEL)
					p=ctx->clr_trans;
				if(	(ctx->line_x0>=0) && (ctx->line_y0>=0) &&
					(ctx->line_x1>=0) && (ctx->line_y1>=0))
				{
					GfxEdit_MarkUndoRect(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						1);
					GfxEdit_DoDrawCircle(ctx,
						ctx->line_x0, ctx->line_y0,
						ctx->line_x1, ctx->line_y1,
						p);
					GfxEdit_MarkRedoRect(ctx);
					ctx->redraw_img_dirty=1;
				}
			}
		}
	}
}

u16 GfxEdit_RGBA32to555(u32 rgb)
{
	int cr, cg, cb, ca, c;
	ca=(rgb>>24)&255;
	cr=(rgb>>16)&255;
	cg=(rgb>> 8)&255;
	cb=(rgb>> 0)&255;
	if(ca<240)
	{
		cr=(cr+8)>>4;
		cg=(cg+8)>>4;
		cb=(cb+8)>>4;
		if(cr>15)cr=15;
		if(cg>15)cg=15;
		if(cb>15)cb=15;
		c=(cr<<11)|(cg<<6)|(cb<<1);
		c|=0x8000|(((ca>>7)&1)<<10)|(((ca>>6)&1)<<5)|(((ca>>5)&1)<<0);
		return(c);
	}else
	{
		cr=(cr+4)>>3;
		cg=(cg+4)>>3;
		cb=(cb+4)>>3;
		if(cr>31)cr=31;
		if(cg>31)cg=31;
		if(cb>31)cb=31;
		c=(cr<<10)|(cg<<5)|cb;
		return(c);
	}
}

byte GfxEdit_RGBA32toLuma(u32 rgb)
{
	int cr, cg, cb, ca, c;
	ca=(rgb>>24)&255;
	cr=(rgb>>16)&255;
	cg=(rgb>> 8)&255;
	cb=(rgb>> 0)&255;
	c=(2*cg+cr+cb)/4;
	return(c);
}

u32 GfxEdit_RGB555to32(u16 px)
{
	int cr, cg, cb, ca;
	u32 c;
	cr=(px>>10)&31;
	cg=(px>> 5)&31;
	cb=(px>> 0)&31;
	cr=(cr<<3)|(cr>>2);
	cg=(cg<<3)|(cg>>2);
	cb=(cb<<3)|(cb>>2);
	ca=255;
	
	if(px&0x8000)
	{
		ca=	(((px>>10)&1)<<7) |
			(((px>> 5)&1)<<6) |
			(((px>> 0)&1)<<5) ;
		cr=(px>>11)&15;
		cg=(px>> 6)&15;
		cb=(px>> 1)&15;
		cr=(cr<<4)|(cr>>0);
		cg=(cg<<4)|(cg>>0);
		cb=(cb<<4)|(cb>>0);
	}
	
//	c=(255U<<24)|(cr<<16)|(cg<<8)|cb;
	c=(ca<<24)|(cr<<16)|(cg<<8)|cb;
	return(c);
}

u16 GfxEdit_AvgRGB555(u16 c0, u16 c1)
{
	u32 p0, p1, p2;
	
	p0=GfxEdit_RGB555to32(c0);
	p1=GfxEdit_RGB555to32(c1);
	p2=((p0&0xFEFEFEFEU)>>1)+((p1&0xFEFEFEFEU)>>1);
	return(GfxEdit_RGBA32to555(p2));
}

u16 GfxEdit_Avg4xRGB555(u16 c0, u16 c1, u16 c2, u16 c3)
{
	u32 p0, p1, p2, p3, p4;
	
	p0=GfxEdit_RGB555to32(c0);
	p1=GfxEdit_RGB555to32(c1);
	p2=GfxEdit_RGB555to32(c1);
	p3=GfxEdit_RGB555to32(c2);
	p2=	((p0&0xFCFCFCFCU)>>2)+
		((p1&0xFCFCFCFCU)>>2)+
		((p2&0xFCFCFCFCU)>>2)+
		((p3&0xFCFCFCFCU)>>2);
	return(GfxEdit_RGBA32to555(p2));
}

void GfxEdit_SetupPalFinish(GfxEdit_Context *ctx)
{
	int i, j, k, clr_tr, n;
	for(i=0; i<256; i++)
		{ ctx->canvas_pal2[i]=GfxEdit_RGB24to555(ctx->canvas_pal4[i]); }

	clr_tr=-1;
	n=(1<<(ctx->canvas_bpp));
	
	for(i=0; i<n; i++)
	{
		if((ctx->canvas_pal4[i]>>24)==0)
		{
			if(clr_tr!=(-1))
				{ clr_tr=-2; continue; }
			clr_tr=i;
			continue;
		}
	}

	for(i=0; i<n; i++)
	{
		if(i!=clr_tr)
			{ ctx->canvas_pal4[i]|=0xFF000000U; }
	}
	
	if(clr_tr<0)
	{
		clr_tr=255;
		if(n<256)
		{
			ctx->canvas_pal4[255]=0;
			ctx->canvas_pal2[255]=0x8000;
		}
	}else
	{
		ctx->canvas_pal2[clr_tr]|=0x8000;
	}
	
	ctx->sel_color1&=n-1;
	ctx->sel_color2&=n-1;
	ctx->clr_trans=clr_tr;
	ctx->redraw_tools_dirty=1;
	ctx->redraw_img_dirty=1;
}

void GfxEdit_SetupPalRGBI(GfxEdit_Context *ctx)
{
	u32 c;
	int hi, lo;
	int i, j, k;
	
	for(i=0; i<16; i++)
	{
		hi=0xAA; lo=0x00;
		if(i&8)
			{ hi=0xFF; lo=0x55; }
		c=0xFF000000;
		c|=((i&1)?hi:lo)<< 0;
		c|=((i&2)?hi:lo)<< 8;
		c|=((i&4)?hi:lo)<<16;
		
		if(i==6)
			c=0xFFAA5500;
		ctx->canvas_pal4[i]=c;
	}

	ctx->canvas_pal4[0xD]&=0x00FFFFFFU;

	ctx->canvas_bpp=4;
	GfxEdit_SetupPalFinish(ctx);
}


void GfxEdit_SetupPalRGBIx2(GfxEdit_Context *ctx)
{
	u32 c;
	int hi, lo;
	int i, j, k;
	
	for(i=0; i<16; i++)
	{
		hi=0xAA; lo=0x00;
		if(i&8)
			{ hi=0xFF; lo=0x55; }
		c=0xFF000000;
		c|=((i&1)?hi:lo)<< 0;
		c|=((i&2)?hi:lo)<< 8;
		c|=((i&4)?hi:lo)<<16;
		
		if(i==6)
			c=0xFFAA5500;
		ctx->canvas_pal4[i]=c;

		c=(c&0xFEFEFEFEU)>>1;
		c|=0xFF000000;
		ctx->canvas_pal4[16+i]=c;
	}

	ctx->canvas_pal4[16]&=0x00FFFFFFU;

	ctx->canvas_bpp=5;
	GfxEdit_SetupPalFinish(ctx);
}

void GfxEdit_SetupPalWeb216(GfxEdit_Context *ctx)
{
	u32 c;
	int hi, lo, mx, my, mz;
	int i, j, k;
	
	for(i=0; i<256; i++)
	{
		c=0xFFFF00FFU;
		if(i==255)
			c=0x00FF00FFU;
		ctx->canvas_pal4[i]=c;
	}

	for(i=0; i<16; i++)
	{
		hi=0xAA; lo=0x00;
		if(i&8)
			{ hi=0xFF; lo=0x55; }
		c=0xFF000000U;
		c|=((i&1)?hi:lo)<< 0;
		c|=((i&2)?hi:lo)<< 8;
		c|=((i&4)?hi:lo)<<16;
		
		if(i==6)
			c=0xFFAA5500U;
		ctx->canvas_pal4[i]=c;
		
		mx=i|(i<<4);
		c=(mx<<16)|(mx<<8)|(mx<<0);
		c|=0xFF000000U;
		ctx->canvas_pal4[16+i]=c;
	}

	for(i=0; i<216; i++)
	{
		mx=(i/ 1)%6;
		my=(i/ 6)%6;
		mz=(i/36)%6;
		mx*=51; my*=51; mz*=51;
		c=(mz<<16)|(my<<8)|(mx<<0);
		c|=0xFF000000U;
		ctx->canvas_pal4[32+i]=c;
	}

	ctx->canvas_pal4[32]&=0x00FFFFFFU;

	ctx->canvas_bpp=8;
	GfxEdit_SetupPalFinish(ctx);
}

void GfxEdit_SetupPalMono(GfxEdit_Context *ctx)
{
	ctx->canvas_pal4[0]=0xFF000000U;
	ctx->canvas_pal4[1]=0xFFFFFFFFU;
	ctx->canvas_bpp=1;
	ctx->sel_color1=0;
	ctx->sel_color2=1;
	ctx->clr_trans=0;
	GfxEdit_SetupPalFinish(ctx);
}

void GfxEdit_SetupPalCga2b(GfxEdit_Context *ctx, int psub)
{
	ctx->canvas_pal4[0]=0xFF000000U;
	ctx->canvas_pal4[1]=0xFFFFFFFFU;
	ctx->canvas_pal4[2]=0xFF00FFFFU;
	ctx->canvas_pal4[3]=0xFFFF00FFU;
	
	if(psub==1)
	{
		ctx->canvas_pal4[2]=0xFFFF0000U;
		ctx->canvas_pal4[3]=0xFF00FF00U;
	}

	if(psub==2)
	{
		ctx->canvas_pal4[1]=0xFFFFFF00U;
		ctx->canvas_pal4[2]=0xFFFF0000U;
		ctx->canvas_pal4[3]=0xFF00FF00U;
	}

	if(psub==3)
	{
		ctx->canvas_pal4[2]=0xFF00FFFFU;
		ctx->canvas_pal4[3]=0xFF0FF000U;
	}

	ctx->canvas_bpp=2;
	ctx->sel_color1=0;
	ctx->sel_color2=1;
	ctx->clr_trans=0;
	GfxEdit_SetupPalFinish(ctx);
}
