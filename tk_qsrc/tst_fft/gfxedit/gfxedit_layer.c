void GfxEdit_RenderLayers(GfxEdit_Context *ctx)
{
	byte *can_pix, *lyr_pix, *cs, *ct;
	int x, y, z, c, ixs, iys, clr_trans;

	if(!ctx->layer_max)
		return;

	can_pix=ctx->canvas_pixels;
	ixs=ctx->canvas_width;
	iys=ctx->canvas_height;
	clr_trans=ctx->clr_trans;
	
	memset(can_pix, clr_trans, ixs*iys);
	
	for(z=0; z<ctx->layer_max; z++)
	{
		if(ctx->layer[z]->flag&1)
			continue;
		lyr_pix=ctx->layer[z]->pixels;
		cs=lyr_pix;
		ct=can_pix;
		for(y=0; y<iys; y++)
		{
			for(x=0; x<ixs; x++)
			{
				c=cs[x];
				if(c==clr_trans)
					continue;
				ct[x]=c;
			}
			cs+=ixs;
			ct+=ixs;
		}
	}
}

void GfxEdit_SelectLayer(GfxEdit_Context *ctx, int lyr)
{
	if(!ctx->layer_max)
		return;
	ctx->layer_cur=lyr;
	ctx->layer_pixels=ctx->layer[lyr]->pixels;
}

void GfxEdit_FlipLayerVisible(GfxEdit_Context *ctx)
{
	if(!ctx->layer_max)
		return;
	ctx->layer[ctx->layer_cur]->flag^=1;
}

void GfxEdit_AddLayer(GfxEdit_Context *ctx)
{
	int i, j, k, n;

	if(!ctx->layer_max)
	{
		if(!ctx->layer[0])
		{
			ctx->layer[0]=malloc(sizeof(GfxEdit_DrawLayer));
			memset(ctx->layer[0], 0, sizeof(GfxEdit_DrawLayer));
		}
		
		ctx->layer[0]->pixels=malloc(
			ctx->canvas_width*ctx->canvas_height);
		memcpy(
			ctx->layer[0]->pixels,
			ctx->canvas_pixels,
			ctx->canvas_width*ctx->canvas_height);
		ctx->layer_pixels=
			ctx->layer[0]->pixels;
		ctx->layer_max=1;
		ctx->layer_cur=0;
	}
	
	n=ctx->layer_max++;
	for(i=n; i>ctx->layer_cur; i--)
		ctx->layer[i]=ctx->layer[i-1];

	i=ctx->layer_cur+1;
	ctx->layer[i]=malloc(sizeof(GfxEdit_DrawLayer));
	memset(ctx->layer[i], 0, sizeof(GfxEdit_DrawLayer));
	ctx->layer[i]->pixels=malloc(
		ctx->canvas_width*ctx->canvas_height);
	memset(ctx->layer[i]->pixels, ctx->clr_trans,
		ctx->canvas_width*ctx->canvas_height);
	
	GfxEdit_SelectLayer(ctx, i);
}

byte *GfxEdit_GetLayerPixels(GfxEdit_Context *ctx, int lyr)
{
	if(!ctx->layer_max)
		return(ctx->canvas_pixels);
	return(ctx->layer[lyr]->pixels);
}


byte gfxedit_predfn_none(byte *ibuf, int xs)
	{ return(0); }
byte gfxedit_predfn_last(byte *ibuf, int xs)
	{ return(ibuf[-1]); }
byte gfxedit_predfn_up(byte *ibuf, int xs)
	{ return(ibuf[-xs]); }
byte gfxedit_predfn_avg(byte *ibuf, int xs)
	{ return((ibuf[-1]+ibuf[-xs])>>1); }

byte gfxedit_predfn_paeth(byte *ibuf, int xs)
{
	byte pa, pb, pc, pp, p;
	int da, db, dc;

	pa=ibuf[-1];
	pb=ibuf[-xs];
	pc=ibuf[-xs-1];
	pp=pa+pb-pc;
	da=(pp>pa)?(pp-pa):(pa-pp);
	db=(pp>pb)?(pp-pb):(pb-pp);
	dc=(pp>pc)?(pp-pc):(pc-pp);
	p=(da<=db)?((da<=dc)?pa:pc):((db<=dc)?pb:pc);
	return(p);
}

u64 gfxedit_predfn8x_none(byte *ibuf, int xs)
	{ return(0); }
u64 gfxedit_predfn8x_last(byte *ibuf, int xs)
	{ return(*(u64 *)(ibuf-1)); }
u64 gfxedit_predfn8x_up(byte *ibuf, int xs)
	{ return(*(u64 *)(ibuf-xs)); }

#if 0
u64 GfxEdit_PackedSub8xB(u64 va, u64 vb)
{
	static const u64 mask_lo7p=0x7F7F7F7F7F7F7F7FULL;
	u64 vc, nvb;
	nvb=(~vb);
	vc=(va&mask_lo7p)+(nvb&mask_lo7p);
	vc^=(va^nvb)&(~mask_lo7p);
	return(vc);
}
#endif

u64 GfxEdit_PackedAdd8xB(u64 va, u64 vb)
{
	static const u64 mask_lo7p=0x7F7F7F7F7F7F7F7FULL;
	u64 vc;
	vc=(va&mask_lo7p)+(vb&mask_lo7p);
	vc^=(va^vb)&(~mask_lo7p);
	return(vc);
}

u64 GfxEdit_PackedSub8xB(u64 va, u64 vb)
{
	u64 nvb, vc;
	nvb=GfxEdit_PackedAdd8xB(~vb, 0x0101010101010101ULL);
	vc=GfxEdit_PackedAdd8xB(va, nvb);
	return(vc);
}

byte *GfxEdit_GetLayerPixelsPred(GfxEdit_Context *ctx, int lyr)
{
	byte *bpix, *ppix, *ccs, *cct;
	byte (*fn)(byte *ibuf, int xs);
	u64 (*fn8x)(byte *ibuf, int xs);
	u64 p8x, v8x, d8x;
	int x, y, xs, ys, npix, d, p, cd, cf, bd, bfn, lbfn;
	
	xs=ctx->canvas_width;
	ys=ctx->canvas_height;
	npix=xs*ys;
	bpix=GfxEdit_GetLayerPixels(ctx, lyr);
	ppix=ctx->buf_layerpred;
	if(!ppix)
	{
		ppix=malloc(xs*ys+ys);
		ctx->buf_layerpred=ppix;
	}
	
	lbfn=0;
	for(y=0; y<ys; y++)
	{
		ccs=bpix+y*xs;
		cct=ppix+y*xs;
			
		bfn=0; bd=99999999;
		for(cf=0; (cf<5) && (y>0); cf++)
		{
			switch(cf)
			{
			case 0: fn=gfxedit_predfn_none; break;
			case 1: fn=gfxedit_predfn_last; break;
			case 2: fn=gfxedit_predfn_up  ; break;
			case 3: fn=gfxedit_predfn_avg ; break;
			case 4: fn=gfxedit_predfn_paeth; break;
			}
			
//			if((cf!=0) && (cf!=4))
//				continue;
			
//			cd=ccs[0];
			cd=0;
//			for(x=1; x<xs; x++)
			for(x=4; x<xs; x+=8)
			{
				p=fn(ccs+x, xs);
				d=ccs[x]-p;
				if(d<0)		d=-d;
				cd+=d;
			}

			if(cf==lbfn)
				{ cd*=0.85; }
			else if(cf==4)
				{ cd*=1.15; }
			
			if(cd<bd)
				{ bfn=cf; bd=cd; }
		}

		lbfn=bfn;

		fputc('0'+bfn, stdout);

		ppix[npix+y]=bfn;
		
		if(!bfn)
		{
			memcpy(cct, ccs, xs);
			continue;
		}
		
		switch(bfn)
		{
		case 0:
			fn=gfxedit_predfn_none;
			fn8x=gfxedit_predfn8x_none;
			break;
		case 1:
			fn=gfxedit_predfn_last;
			fn8x=gfxedit_predfn8x_last;
			break;
		case 2:
			fn=gfxedit_predfn_up  ;
			fn8x=gfxedit_predfn8x_up  ;
			break;
		case 3:
			fn=gfxedit_predfn_avg ;
			fn8x=NULL;
			break;
		case 4:
			fn=gfxedit_predfn_paeth;
			fn8x=NULL;
			break;
		}
		
		if(bfn)		{ cct[0]=ccs[0]-ccs[-xs]; }
		else		{ cct[0]=ccs[0]; }
		if(fn8x)
		{
			for(x=1; (x+8)<=xs; x+=8)
			{
				p8x=fn8x(ccs+x, xs);
				v8x=*(u64 *)(ccs+x);
				d8x=GfxEdit_PackedSub8xB(v8x, p8x);
				*(u64 *)(cct+x)=d8x;
			}
			for(; x<xs; x++)
			{
				p=fn(ccs+x, xs);
				cct[x]=ccs[x]-p;
			}
		}else
		{
			for(x=1; x<xs; x++)
			{
				p=fn(ccs+x, xs);
				cct[x]=ccs[x]-p;
			}
		}
	}
	
	fputc('\n', stdout);
	fflush(stdout);
	
	return(ppix);
}

void GfxEdit_LoadLayerUnPred(GfxEdit_Context *ctx, int lyr)
{
	byte *bpix, *ppix, *ccs, *cct, *ccx1;
	byte (*fn)(byte *ibuf, int xs);
	u64 (*fn8x)(byte *ibuf, int xs);
	u64 p8x, v8x, d8x;
	int x, y, xs, ys, npix, d, p, cd, cf, bd, bfn;
	int p0, p1, p2, p3, pn;
	
	xs=ctx->canvas_width;
	ys=ctx->canvas_height;
	npix=xs*ys;
	bpix=GfxEdit_GetLayerPixels(ctx, lyr);
//	ppix=ctx->buf_layerpred;
	ppix=bpix;

	for(y=0; y<ys; y++)
	{
		ccs=bpix+y*xs;
		cct=ppix+y*xs;
		bfn=ppix[npix+y];

		if(!bfn)
		{
			if(ccs!=cct)
				memcpy(cct, ccs, xs);
			continue;
		}

		if((bfn==1) && (ccs==cct))
		{
			cct[0]=ccs[0]+ccs[-xs];
			for(x=1; (x+8)<=xs; x+=8)
			{
				ccx1=cct+x;
				pn=ccx1[-1];
				p0=ccx1[0]; p1=ccx1[1];
				p2=ccx1[2]; p3=ccx1[3];
				p0+=pn; p1+=p0;
				p2+=p1; p3+=p2;
				ccx1[0]=p0; ccx1[1]=p1;
				ccx1[2]=p2; ccx1[3]=p3;
				pn=p3;
				p0=ccx1[4]; p1=ccx1[5];
				p2=ccx1[6]; p3=ccx1[7];
				p0+=pn; p1+=p0;
				p2+=p1; p3+=p2;
				ccx1[4]=p0; ccx1[5]=p1;
				ccx1[6]=p2; ccx1[7]=p3;
			}
			for(; x<xs; x++)
			{
				p=ccs[x-1];
				cct[x]=p+ccs[x];
			}
			continue;
		}

//		ppix[npix+y]=bfn;
		switch(bfn)
		{
		case 0:
			fn=gfxedit_predfn_none;
			fn8x=gfxedit_predfn8x_none;
			break;
		case 1:
			fn=gfxedit_predfn_last;
//			fn8x=gfxedit_predfn8x_last;
			fn8x=NULL;
			break;
		case 2:
			fn=gfxedit_predfn_up  ;
			fn8x=gfxedit_predfn8x_up  ;
			break;
		case 3:
			fn=gfxedit_predfn_avg ;
			fn8x=NULL;
			break;
		case 4:
			fn=gfxedit_predfn_paeth;
			fn8x=NULL;
			break;
		}
		
		if(bfn)	{ cct[0]=ccs[0]+ccs[-xs]; }
		else	{ cct[0]=ccs[0]; }
		if(fn8x)
		{
			for(x=1; (x+8)<=xs; x+=8)
			{
				p8x=fn8x(ccs+x, xs);
				v8x=*(u64 *)(ccs+x);
				d8x=GfxEdit_PackedAdd8xB(v8x, p8x);
				*(u64 *)(cct+x)=d8x;
			}
			for(; x<xs; x++)
			{
				p=fn(ccs+x, xs);
				cct[x]=p+ccs[x];
			}
		}else
		{
			for(x=1; x<xs; x++)
			{
				p=fn(ccs+x, xs);
				cct[x]=p+ccs[x];
			}
		}
	}
}

void GfxEdit_EmitGtlvTagFCC(byte **rct, u32 fcc, u32 len)
{
	byte *ct;
	u32 nlen;

	ct=*rct;
	nlen=~(len+8);
	ct[0]=(fcc >> 0)&255;
	ct[1]=(fcc >> 8)&255;
	ct[2]=(fcc >>16)&255;
	ct[3]=(fcc >>24)&255;
	ct[4]=(nlen>> 0)&255;
	ct[5]=(nlen>> 8)&255;
	ct[6]=(nlen>>16)&255;
	ct[7]=(nlen>>24)&255;
	*rct=ct+8;
}

void GfxEdit_UpdateGtlvTagFCC(byte *ct, u32 fcc, u32 len)
{
	u32 nlen;

	nlen=~(len+8);
	ct[0]=(fcc >> 0)&255;
	ct[1]=(fcc >> 8)&255;
	ct[2]=(fcc >>16)&255;
	ct[3]=(fcc >>24)&255;
	ct[4]=(nlen>> 0)&255;
	ct[5]=(nlen>> 8)&255;
	ct[6]=(nlen>>16)&255;
	ct[7]=(nlen>>24)&255;
}

void GfxEdit_FlattenContextGELI(GfxEdit_Context *ctx, byte **robuf, int *rosz)
{
	static const char *hexstr="0123456789ABCDEF";
#ifdef GFXEDIT_GELI_ENA_RP2STF
	static byte *tmp1buf;
	static byte *tmp2buf;
#endif
	BGBCC_BITMAPINFOHEADER *bmi;
	u32 *pal;
	byte *ct, *obuf, *bpal, *bpix, *ppix;
	u32 fcc;
	int iosz, osz, npix, nlyr, nclr, sz_head, xs, ys;
	int i, j, k, k1, k2, k3;
	
	pal=ctx->canvas_pal4;
	nclr=1<<ctx->canvas_bpp;
	
	xs=ctx->canvas_width;
	ys=ctx->canvas_height;
	npix=xs*ys;
	nlyr=ctx->layer_max;
	if(!nlyr)nlyr=1;
	iosz=1536+nlyr*npix;

#ifdef GFXEDIT_GELI_ENA_RP2STF
	if(!tmp1buf)
	{
		tmp1buf=malloc(xs*ys*1.5);
		tmp2buf=malloc(xs*ys*1.5);
	}
#endif
	
	obuf=malloc(iosz);
	ct=obuf;
	GfxEdit_EmitGtlvTagFCC(&ct, GFXEDIT_FCC_GELI, 0);

	sz_head=sizeof(BGBCC_BITMAPINFOHEADER)+(nclr*4);
	GfxEdit_EmitGtlvTagFCC(&ct, GFXEDIT_FCC_head, sz_head);
	bmi=(BGBCC_BITMAPINFOHEADER *)ct;
	bpal=(byte *)(bmi+1);
	ct+=sz_head;

	bmi->biSize=40;
	bmi->biWidth=xs;
	bmi->biHeight=-ys;
	bmi->biPlanes=1;
	bmi->biBitCount=8;
	bmi->biCompression=0;
	bmi->biSizeImage=ys*xs;
	bmi->biXPelsPerMeter=2835;
	bmi->biYPelsPerMeter=2835;
	bmi->biClrUsed=nclr;
	bmi->biClrImportant=nclr;
	
	for(i=0; i<nclr; i++)
	{
		bpal[i*4+0]=(pal[i]>> 0)&255;
		bpal[i*4+1]=(pal[i]>> 8)&255;
		bpal[i*4+2]=(pal[i]>>16)&255;
		bpal[i*4+3]=(pal[i]>>24)&255;
	}

	for(i=0; i<nlyr; i++)
	{
#if 1

		bpix=GfxEdit_GetLayerPixels(ctx, i);
#ifdef GFXEDIT_GELI_ENA_RP2STF
		ppix=GfxEdit_GetLayerPixelsPred(ctx, i);
#endif

//		k=GfxEdit_EncodeRP2(
		k1=GfxEdit_EncodeRP2Test(
			ct+8,
			bpix, npix+256, npix);
		k=k1;

		if((k1*1.20)<npix)
		{
#ifdef GFXEDIT_GELI_ENA_RP2STF
			k3=GfxEdit_EncodeRP2Test(
				tmp2buf,
				ppix, npix+256, npix+ys);

//			k2=BTM_StfRk_EncodeBufferPostRp2(tmp2buf, tmp1buf, k1);
//			k2=BTM_StfRk_EncodeBufferPostRp2(tmp1buf, ct+8, k1);
			k2=BTM_StfRk_EncodeBufferPostRp2(tmp1buf, tmp2buf, k3);
//			k3=BTM_StfRk_DecodeBufferPostRp2(tmp2buf, tmp1buf, k2);
//			memcpy(tmp2buf, ct+8, k1);
//			k3=BTM_StfRk_DummyDecodeBufferPostRp2(tmp2buf, tmp1buf, k2);
//			memcpy(ct+8, tmp1buf, k);

//			if((k2*1.10)<k1)
//			if((k2*1.075)<k1)
//			if((k2*1.067)<k1)
			if((k2*1.05)<k1)
			{
				printf("used STFRK %d->%d\n", k1, k2);
				k=k2; memcpy(ct+8, tmp1buf, k2);
			}else
			{
				printf("no use STFRK %d->%d\n", k1, k2);
			}
#endif

			fcc=GFXEDIT_FOURCC('l', '3', hexstr[i>>4], hexstr[i&15]);
			GfxEdit_EmitGtlvTagFCC(&ct, fcc, k);
			ct+=k;
		}else
		{
			fcc=GFXEDIT_FOURCC('l', 'i', hexstr[i>>4], hexstr[i&15]);
			GfxEdit_EmitGtlvTagFCC(&ct, fcc, npix);
			memcpy(ct, GfxEdit_GetLayerPixels(ctx, i), npix);
			ct+=npix;
		}
#endif

#if 0
		fcc=GFXEDIT_FOURCC('l', 'i', hexstr[i>>4], hexstr[i&15]);
		GfxEdit_EmitGtlvTagFCC(&ct, fcc, npix);
		memcpy(ct, GfxEdit_GetLayerPixels(ctx, i), npix);
		ct+=npix;
#endif
	}
	
	osz=ct-obuf;
	GfxEdit_UpdateGtlvTagFCC(obuf, GFXEDIT_FCC_GELI, osz-8);
	
	*robuf=obuf;
	*rosz=osz;
	
	printf("GfxEdit_FlattenContextGELI: sz=%d\n", osz);
}

byte GfxEdit_TwoccHexToByte(u16 tcc)
{
	int c0, c1, c;

	c0=(tcc>>0)&255;
	c1=(tcc>>8)&255;

	c=0;
	if((c0>='0') && (c0<='9'))	c+=c0-'0';
	else if((c0>='A') && (c0<='F'))	c+=10+(c0-'A');
	else if((c0>='a') && (c0<='f'))	c+=10+(c0-'a');
	else c|=-1;

	c<<=4;
	if((c1>='0') && (c1<='9'))	c+=c1-'0';
	else if((c1>='A') && (c1<='F'))	c+=10+(c1-'A');
	else if((c1>='a') && (c1<='f'))	c+=10+(c1-'a');
	else c|=-1;
	return(c);
}

int GfxEdit_LoadContextGELI(GfxEdit_Context *ctx, byte *imgbuf, int imgsz)
{
	static byte *tmp1buf;
	static int tmp1buf_npix;
	BGBCC_BITMAPINFOHEADER *bmi;
	u32 *pal;
	byte *ct, *obuf, *bpal;
	byte *tcs, *tcse, *cs1;
	u32 fcc, nlen, len;
	int xs, ys, npix, np1, nclr, lyrid, flip, len1, len2;
	
	fcc=((u32 *)imgbuf)[0];
	nlen=((u32 *)imgbuf)[1];
	if(fcc!=GFXEDIT_FCC_GELI)
		return(-1);
	len=~nlen;
	if(len>imgsz)
		return(-1);
	
	xs=0; ys=0; nclr=0; flip=0;
	
	tcs=imgbuf+8;
	tcse=imgbuf+len;
	while(tcs<tcse)
	{
		fcc=((u32 *)tcs)[0];
		nlen=((u32 *)tcs)[1];
		if(fcc&0x80808080U)
			break;
		if(!(nlen&0x80000000U))
			break;
		len=~nlen;
		if(len<8)
			break;
		cs1=tcs+8;
		tcs+=len;

		if(fcc==GFXEDIT_FCC_head)
		{
			bmi=(BGBCC_BITMAPINFOHEADER *)cs1;
			bpal=cs1+bmi->biSize;

			xs=bmi->biWidth;
			ys=-bmi->biHeight;
			nclr=bmi->biClrUsed;

			if(ys<0)
				{ ys=-ys; flip=1; }

			if(ctx->canvas_pixels)
				free(ctx->canvas_pixels);
			ctx->canvas_pixels=malloc(xs*ys);
			ctx->canvas_width=xs;
			ctx->canvas_height=ys;
			ctx->canvas_bpp=gfxedit_log2up(nclr);

			npix=xs*ys;
			
			memset(ctx->canvas_pal4, 0, 256*4);
			memcpy(ctx->canvas_pal4, bpal, nclr*4);
		}

		if((fcc&0xFFFF)==GFXEDIT_TCC_li)
		{
			lyrid=GfxEdit_TwoccHexToByte(fcc>>16);

			if(lyrid>=0)
			{
				if(!ctx->layer[lyrid])
				{
					ctx->layer[lyrid]=malloc(sizeof(GfxEdit_DrawLayer));
					memset(ctx->layer[lyrid], 0, sizeof(GfxEdit_DrawLayer));
				}
				if(lyrid>=ctx->layer_max)
					ctx->layer_max=lyrid+1;
			
				if(ctx->layer[lyrid]->pixels)
					free(ctx->layer[lyrid]->pixels);
				ctx->layer[lyrid]->pixels=malloc(xs*ys+ys+64);
				memcpy(ctx->layer[lyrid]->pixels, cs1, xs*ys+ys);

				if(len>=(xs*ys+ys+8))
					{ GfxEdit_LoadLayerUnPred(ctx, lyrid); }
			}
		}

		if((fcc&0xFFFF)==GFXEDIT_TCC_l3)
		{
			lyrid=GfxEdit_TwoccHexToByte(fcc>>16);

			if(lyrid>=0)
			{
				if(!ctx->layer[lyrid])
				{
					ctx->layer[lyrid]=malloc(sizeof(GfxEdit_DrawLayer));
					memset(ctx->layer[lyrid], 0, sizeof(GfxEdit_DrawLayer));
				}
				if(lyrid>=ctx->layer_max)
					ctx->layer_max=lyrid+1;
			
				if(ctx->layer[lyrid]->pixels)
					free(ctx->layer[lyrid]->pixels);
				ctx->layer[lyrid]->pixels=malloc(xs*ys+ys+64);
				if(BTM_StfRk_CheckPostRp2Blob(cs1))
				{
					if((xs*ys)>tmp1buf_npix)
						{ free(tmp1buf); tmp1buf=NULL; }
					if(!tmp1buf)
					{
						tmp1buf=malloc(xs*ys);
						tmp1buf_npix=xs*ys;
					}

					len1=BTM_StfRk_DecodeBufferPostRp2(
						tmp1buf, cs1, len-8);
					np1=GfxEdit_DecodeRP2(
						ctx->layer[lyrid]->pixels,
						tmp1buf, xs*ys+ys, len1);
				}else
				{
					np1=GfxEdit_DecodeRP2(
						ctx->layer[lyrid]->pixels,
						cs1, xs*ys+ys, len-8);
				}
				if(np1>=(xs*ys+ys))
					{ GfxEdit_LoadLayerUnPred(ctx, lyrid); }
			}
		}
	}
	
	GfxEdit_SetupPalFinish(ctx);
	GfxEdit_SelectLayer(ctx, 0);
	return(1);
}
