void *bgtf_malloc(int sz)
{
	void *ptr;
	ptr=malloc(sz);
	memset(ptr, 0, sz);
	return(ptr);
}

void bgtf_free(void *ptr)
{
	free(ptr);
}

void *bgtf_realloc(void *ptr, int sz)
{
	return(realloc(ptr, sz));
}

byte *BGTF_LoadFileI(char *name, int *rsz, int pad)
{
	char tb[256];
	byte *buf;
	FILE *fd;
	int sz, i;

//	sprintf(tb, "resource/%s", name);
	strcpy(tb, name);
	
//	fd=fopen(name, "rb");
	fd=fopen(tb, "rb");
	if(!fd)
	{
//		buf=FS_LoadFile(name, rsz, pad);
//		if(buf)
//			return(buf);
		
		printf("BGTF_LoadFile: Failed open %s\n", name);
		return(NULL);
	}
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);

	buf=bgtf_malloc(sz+24);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	if(i!=sz)
	{
		printf("BGTF_LoadFile: Size mismatch %s exp=%d got=%d\n",
			name, sz, i);
		if(i>0)
		{
			sz=i;
		}else
		{
			if(!(pad&6))
				bgtf_free(buf);
			return(NULL);
		}
	}
	
	if(pad&1)
	{
//		*(u64 *)(buf+sz+0)=0;
//		*(u64 *)(buf+sz+8)=0;
		memset(buf+sz, 0, 16);
	}
	
	*rsz=sz;
	return(buf);
}

byte *BGTF_LoadFile(char *name, int *rsz)
{
	return(BGTF_LoadFileI(name, rsz, 1));
}


int bgtf_sext12(int v)
{
	return(((signed int)(v<<20))>>20);
}

BGTF_FontImage *BGTF_LoadFontImage(char *name)
{
	BGTF_FontImage *tmp;
	u32 *buf;
	u32 v;
	int sz;
	int i, j, k;
	
	sz=0;
	buf=(u32 *)BGTF_LoadFile(name, &sz);
	if(!buf)
		return(NULL);
	
	if(buf[0]!=BGTF_MAGIC)
	{
		bgtf_free(buf);
		return(NULL);
	}
	
	sz=sz>>2;
	
	tmp=bgtf_malloc(sizeof(BGTF_FontImage));
	tmp->img_data=buf;
	tmp->sz_image=sz;
	tmp->dircompact=0;
	
	if((buf[1]>>28)==15)
	{
		tmp->dircompact=1;
		tmp->pagedir=buf+(buf[1]&0x00FFFFFF);
	}else
	{
		tmp->pagedir=buf+buf[1];
	}
	
	for(i=buf[2]; i<sz; i++)
	{
		v=buf[i];
		if(!v)
			break;
		if((v>>24)==0x01)
		{
			tmp->celsz_x=bgtf_sext12(v>> 0);
			tmp->celsz_y=bgtf_sext12(v>>12);
			continue;
		}

		if((v>>24)==0x06)
		{
			tmp->min_x=bgtf_sext12(v>> 0);
			tmp->min_y=bgtf_sext12(v>>12);
			continue;
		}
		if((v>>24)==0x07)
		{
			tmp->max_x=bgtf_sext12(v>> 0);
			tmp->max_y=bgtf_sext12(v>>12);
			continue;
		}
	}
	
	return(tmp);
}

BGTF_FontGlyph *BGTF_ImageLoadGlyph(BGTF_FontImage *img, int utf)
{
	static BGTF_FontVert verts[512];
	static BGTF_FontVert spans[16];
	BGTF_FontGlyph *glif;
	u32 v;
	int ixh, ixl, pto, bofs;
	int vx, vy, vt, nv, ns;
	int i, j, k;
	
	ixh=(utf>>8)&255;
	ixl=(utf>>0)&255;
	
	if(img->dircompact)
	{
		for(i=0; i<256; i++)
		{
			pto=img->pagedir[i];
			if(!pto)
				break;
			if(((pto>>24)&255)==ixh)
				break;
		}
		pto&=0x00FFFFFF;
		if(!pto || (i>=256))
			return(NULL);
		for(i=0; i<256; i++)
		{
			bofs=img->img_data[pto+i];
			if(!bofs)
				break;
			if(((bofs>>24)&255)==ixl)
				break;
		}
		bofs&=0x00FFFFFF;
		if(!bofs || (i>=256))
			return(NULL);
	}else
	{
		pto=img->pagedir[ixh];
		if(!pto)
			return(NULL);
		bofs=img->img_data[pto+ixl];
		if(!bofs)
			return(NULL);
	}
	
	glif=bgtf_malloc(sizeof(BGTF_FontGlyph));

	nv=0;
	ns=0;
	for(i=bofs; i<img->sz_image; i++)
	{
		v=img->img_data[i];
		if(!v)
			break;
		
		vx=bgtf_sext12(v>> 0);
		vy=bgtf_sext12(v>>12);
		vt=v>>24;
		if((v>>28)==0)
		{
			switch((v>>24)&15)
			{
			case 4:
				glif->csz_x=vx;
				glif->csz_y=vy;
				break;
			case 5:
				glif->org_x=vx;
				glif->org_y=vy;
				break;
			case 6:
				glif->min_x=vx;
				glif->min_y=vy;
				break;
			case 7:
				glif->max_x=bgtf_sext12(v>> 0);
				glif->max_y=bgtf_sext12(v>>12);
				break;
			}
		}else
			if((v>>28)==1)
		{
			verts[nv].x=vx;
			verts[nv].y=vy;
			verts[nv].tag=vt;
			nv++;
		}else
			if((v>>28)==2)
		{
			spans[ns].x=nv;
			verts[nv].x=vx;
			verts[nv].y=vy;
			verts[nv].tag=vt;
			nv++;
		}else
			if((v>>28)==3)
		{
			verts[nv].x=vx;
			verts[nv].y=vy;
			verts[nv].tag=vt;
			nv++;
			spans[ns].y=nv;
			ns++;
		}
	}
	
	glif->spans=bgtf_malloc((nv+ns)*sizeof(BGTF_FontVert));
	glif->verts=glif->spans+ns;
	memcpy(glif->verts, verts, nv*sizeof(BGTF_FontVert));
	memcpy(glif->spans, spans, ns*sizeof(BGTF_FontVert));
	glif->nvert=nv;
	glif->nspan=ns;
	glif->utf=utf;
	glif->image=img;
	
	return(glif);
}

BGTF_FontGlyph *BGTF_ImageGetGlyph(BGTF_FontImage *img, int utf)
{
	BGTF_FontGlyph *gcur;
	
	gcur=img->cache;
	while(gcur)
	{
		if(gcur->utf==utf)
			return(gcur);
		gcur=gcur->next;
	}
	
	gcur=BGTF_ImageLoadGlyph(img, utf);
	if(gcur)
	{
		gcur->next=img->cache;
		img->cache=gcur;
	}
	return(gcur);
}


int bgtf_clamp(int x, int m, int n)
{
	if(x<m)
		return(m);
	if(x>n)
		return(n);
	return(x);
}

int BGTF_Dot2(int px, int py, int vx, int vy)
{
	int d;
	
	d=vx*px+vy*py;
	return(d);
}

int BGTF_GlyphCheckLine(
	int rx0, int ry0,
	int rx1, int ry1,
	int vx0, int vy0,
	int vx1, int vy1)
{
	int rdx, rdy, vdx, vdy;
	int rd0, vd0, rd1, vd1;
	int d0, d1, d2, d3;
	
	rdx=rx1-rx0;	rdy=ry1-ry0;
	vdx=vx1-vx0;	vdy=vy1-vy0;
	
	rd0=BGTF_Dot2(rx0, ry0, -rdy, rdx);
	vd0=BGTF_Dot2(vx0, vy0, -vdy, vdx);
	rd1=BGTF_Dot2(rx1, ry1, -rdy, rdx);
	vd1=BGTF_Dot2(vx1, vy1, -vdy, vdx);
	
	if(rd0!=rd1)
	{
		printf("BGTF_GlyphCheckLine: Dot R %d %d\n", rd0, rd1);
	}
	
	if(vd0!=vd1)
	{
		printf("BGTF_GlyphCheckLine: Dot V %d %d\n", vd0, vd1);
	}
	
	d0=BGTF_Dot2(rx0, ry0, -vdy, vdx)-vd0;
	d1=BGTF_Dot2(rx1, ry1, -vdy, vdx)-vd0;
	d2=BGTF_Dot2(vx0, vy0, -rdy, rdx)-rd0;
	d3=BGTF_Dot2(vx1, vy1, -rdy, rdx)-rd0;
	
	if((d0^d1)>=0)
		return(0);
	if((d2^d3)>=0)
		return(0);
	
	if(d0<0)
		return(3);
	
	return(1);
}

#if 1
int BGTF_GlyphCheckCurve(
	int rx0, int ry0, int rx1, int ry1,
	int vx0, int vy0, int vx1, int vy1,
	int vx2, int vy2, int vx3, int vy3,
	byte tg0, byte tg1, byte tg2, byte tg3)
{
	int px0, py0, px1, py1, px, py, d;
	int i0, i1, i2, i3;
	
	i0=vx2-vx1;
	i1=vy2-vy1;
	i0=i0^(i0>>31);
	i1=i1^(i1>>31);
//	d=i0+i1;
	if(i0>i1)
		d=i0+(i1>>1);
	else
		d=i1+(i0>>1);

	if(d<96)
	{
		return(BGTF_GlyphCheckLine(rx0, ry0, rx1, ry1, vx1, vy1, vx2, vy2));
	}
	
//	px0=2*vx1-vx0;
//	py0=2*vy1-vy0;
//	px1=2*vx2-vx3;
//	py1=2*vy2-vy3;

//	px0=(3*vx1-vx0)/2;
//	py0=(3*vy1-vy0)/2;
//	px1=(3*vx2-vx3)/2;
//	py1=(3*vy2-vy3)/2;

	px0=(5*vx1-vx0)/4;
	py0=(5*vy1-vy0)/4;
	px1=(5*vx2-vx3)/4;
	py1=(5*vy2-vy3)/4;

	px=(px0+px1)/2;
	py=(py0+py1)/2;

//	px=(vx1+vx2)/2;
//	py=(vy1+vy2)/2;

//	i0=BGTF_GlyphCheckLine(vx1, vy1, px, py, rx0, ry0, rx1, ry1);
//	i1=BGTF_GlyphCheckLine(px, py, vx2, vy2, rx0, ry0, rx1, ry1);
//	if(!i0 && !i1)
//		return(0);

	px0=(5*vx1-vx0)/4;
	py0=(5*vy1-vy0)/4;
	px1=(5*vx2-vx3)/4;
	py1=(5*vy2-vy3)/4;

	px0=(3*px0+px)/4;
	py0=(3*py0+py)/4;
	px1=(3*px1+px)/4;
	py1=(3*py1+py)/4;

//	px=(px+vx2)/2;
//	py=(py+vy2)/2;
//	px=(2*vx1+px+vx2)/4;
//	py=(2*vy1+py+vy2)/4;
//	px=(4*vx1+px+3*vx2)/8;
//	py=(4*vy1+py+3*vy2)/8;
	
//	if(d>=192)
	if(0)
	{
		i0=BGTF_GlyphCheckLine(rx0, ry0, rx1, ry1,
			vx1, vy1,
			px0, py0);
		i1=BGTF_GlyphCheckLine(rx0, ry0, rx1, ry1,
			px0, py0,
			px, py);
		i2=BGTF_GlyphCheckLine(rx0, ry0, rx1, ry1,
			px, py,
			px1, py1);
		i3=BGTF_GlyphCheckLine(rx0, ry0, rx1, ry1,
			px1, py1,
			vx2, vy2);

		if(i0)
			return(i0);
		if(i1)
			return(i1);
		if(i2)
			return(i2);
		if(i3)
			return(i3);

		return(0);
	}else
	{
		i0=BGTF_GlyphCheckLine(rx0, ry0, rx1, ry1,
			vx1, vy1,
			px, py);
		i1=BGTF_GlyphCheckLine(rx0, ry0, rx1, ry1,
			px, py,
			vx2, vy2);

		if(i0)
			return(i0);
		if(i1)
			return(i1);

		return(0);
	}
}
#endif

int BGTF_GlyphCheckPoint2b(
	BGTF_FontGlyph *glif,
	int cn, int x0, int y0, int x1, int y1)
{
	int vm, vn, i0, i1, i2, i3;
	int i, j, k, lc, lp, ln, pp;
	
	vm=glif->spans[cn].x;
	vn=glif->spans[cn].y;
	lc=0;
	lp=0; ln=0;
	for(j=vm; j<vn; j++)
	{
		i0=j;
		i1=j+1;
		if(i1>=vn)
			i1=vm;

		i2=j-1;
		if(i2<0)
//			i2+=(vn-vm);
			i2=vn+i2;

		i3=j+2;
		if(i3>=vn)
//			i3-=(vn-vm);
			i3=vm+(i3-vn);
		
		if(!(glif->verts[i2].tag&0x06))
			i2=i0;
		if(!(glif->verts[i3].tag&0x06))
			i3=i1;

//		if(glif->verts[i0].tag&0x01)
//		if(glif->verts[i1].tag&0x05)
//		if(	(glif->verts[i0].tag&0x05) &&
//			(glif->verts[i1].tag&0x05))
		if(0)
		{
			k=BGTF_GlyphCheckCurve(x0, y0, x1, y1,
				glif->verts[i2].x, glif->verts[i2].y,
				glif->verts[i0].x, glif->verts[i0].y,
				glif->verts[i1].x, glif->verts[i1].y,
				glif->verts[i3].x, glif->verts[i3].y,
				glif->verts[i2].tag, glif->verts[i0].tag,
				glif->verts[i1].tag, glif->verts[i3].tag);
		}else
		{
			k=BGTF_GlyphCheckLine(x0, y0, x1, y1,
				glif->verts[i0].x, glif->verts[i0].y,
				glif->verts[i1].x, glif->verts[i1].y);
		}
		if(k>0)
		{
			lc++;
			if(k&2)
				ln++;
			else
				lp++;
		}
	}
	
//	return(lp!=ln);
//	return(lp>ln);
//	return(lp<ln);

	return(lp-ln);
	
//	return(lc);
}

int BGTF_GlyphCheckPoint2(
	BGTF_FontGlyph *glif,
	int x0, int y0, int x1, int y1)
{
	int vm, vn, i0, i1;
	int i, j, k, lc, pp;
	
	pp=0;
	for(i=0; i<glif->nspan; i++)
	{
		lc=BGTF_GlyphCheckPoint2b(glif, i, x0, y0, x1, y1);
//		if(lc&1)
//		if(lc>=2)
		if(lc>0)
			pp=1;

		if(lc<0)
			pp=0;
		
//		if(lc==4)
//			pp=1;
	}
	return(pp);
}

int BGTF_GlyphCheckPoint(BGTF_FontGlyph *glif, int x, int y, int psz)
{
	int pp, ppn, bi, cld, cldx, cldy;
	int i;
	
	if(x<glif->min_x)
		return(0);
	if(x>glif->max_x)
		return(0);
	if(y<glif->min_y)
		return(0);
	if(y>glif->max_y)
		return(0);
	
	bi=psz/2;
	pp=0;
	cld=2048;
	
	cldx=cld^(x>>31);
	cldy=cld^(y>>31);

//	if(BGTF_GlyphCheckPoint2(glif, x, y, x+cldx, y+cldy))
//		pp|=15;

#if 1
	if(BGTF_GlyphCheckPoint2(glif, x-bi, y-bi, x+cld, y+cld))
		pp|=1;
	if(BGTF_GlyphCheckPoint2(glif, x-bi, y+bi, x+cld, y-cld))
		pp|=2;
	if(BGTF_GlyphCheckPoint2(glif, x+bi, y-bi, x-cld, y+cld))
		pp|=4;
	if(BGTF_GlyphCheckPoint2(glif, x+bi, y+bi, x-cld, y-cld))
		pp|=8;
#endif

	if(pp==15)
		return(1);
	if(pp==0)
		return(0);
		
	i=0;
	if(pp&1)i++;
	if(pp&2)i++;
	if(pp&4)i++;
	if(pp&8)i++;

	return(i>2);
}

int BGTF_PlotPointIx8(
	byte *oimg, int oxs, int oys,
	int x, int y, int clr)
{
	int i, j;
	if((x<0) || (y<0))
		return(0);
	if((x>=oxs) || (y>=oys))
		return(0);
	
	i=y*oys+x;
	j=oimg[i];

//	if((j==0xF9) && (clr>=0xFA))
	if((j!=0xF8) && (clr>=0xFA))
		return(0);

	oimg[i]=clr;
	return(1);
}

int BGTF_DrawLineIx8(
	byte *oimg, int oxs, int oys,
	int x0, int y0, int x1, int y1,
	int clr)
{
	int dx, dy, sx, sy, cx, cy, e1, e2;
	
	dx=x1-x0;	dy=y1-y0;
	sx=1;		sy=1;
	if(dx<0)
		{ dx=-dx; sx=-1; }
	if(dy<0)
		{ dy=-dy; sy=-1; }
	dy=-dy;

	cx=x0;
	cy=y0;
	e1=dx+dy;

	while(1)
	{
		BGTF_PlotPointIx8(oimg, oxs, oys, cx, cy, clr);
		if((cx==x1) && (cy==y1))
			break;
		e2=e1+e1;
		if(e2>=dy)
		{
			if(cx==x1)
				break;
			e1=e1+dy;
			cx=cx+sx;
		}
		if(e2<=dx)
		{
			if(cy==y1)
				break;
			e1=e1+dx;
			cy=cy+sy;
		}
	}
}

/* Line skips first 2 and last 2 pixels. */
int BGTF_DrawLineD2Ix8(
	byte *oimg, int oxs, int oys,
	int x0, int y0, int x1, int y1,
	int clr)
{
	int l1x, l1y, l2x, l2y;
	int dx, dy, sx, sy, cx, cy, e1, e2, inh;
	
	dx=x1-x0;	dy=y1-y0;
	sx=1;		sy=1;
	if(dx<0)
		{ dx=-dx; sx=-1; }
	if(dy<0)
		{ dy=-dy; sy=-1; }
	dy=-dy;

	cx=x0;
	cy=y0;
	e1=dx+dy;

	l1x=-1; l1y=-1; l2x=-1; l2y=-1;
	inh=1;

	while(1)
	{
//		BGTF_PlotPointIx8(oimg, oxs, oys, cx, cy, clr);
		BGTF_PlotPointIx8(oimg, oxs, oys, l1x, l1y, clr);
//		BGTF_PlotPointIx8(oimg, oxs, oys, l2x, l2y, clr);
		
		l2x=l1x; l2y=l1y;
		l1x=cx; l1y=cy;
		if(inh>0)
		{
			l1x=-1; l1y=-1;
			inh--;
		}
		
		if((cx==x1) && (cy==y1))
			break;
		e2=e1+e1;
		if(e2>=dy)
		{
			if(cx==x1)
				break;
			e1=e1+dy;
			cx=cx+sx;
		}
		if(e2<=dx)
		{
			if(cy==y1)
				break;
			e1=e1+dx;
			cy=cy+sy;
		}
	}
}

int BGTF_GlyphDrawLines(
	BGTF_FontGlyph *glif,
	byte *oimg, int oxs, int oys, int oxo, int oyo,
	int vmx, int vmy, int vxs, int vys,
	int clr)
{
	int vm, vn, cn, i0, i1, i2, i3;
	int vx0, vy0, vx1, vy1, dx, dy, adx, ady;
	int vx0b, vy0b, vx1b, vy1b;
	int rxs, rys;
	int i, j, k, lc, lp, ln, pp;
	
	rxs=0x10000/vxs;
	rys=0x10000/vys;
	
	for(cn=0; cn<glif->nspan; cn++)
	{
		vm=glif->spans[cn].x;
		vn=glif->spans[cn].y;
		lc=0;
		lp=0; ln=0;
		for(j=vm; j<vn; j++)
		{
			i0=j;
			i1=j+1;
			if(i1>=vn)
				i1=vm;

			i2=j-1;
			if(i2<0)
				i2=vn+i2;

			i3=j+2;
			if(i3>=vn)
				i3=vm+(i3-vn);
			
			if(!(glif->verts[i2].tag&0x06))
				i2=i0;
			if(!(glif->verts[i3].tag&0x06))
				i3=i1;

			vx0=glif->verts[i0].x;
			vy0=glif->verts[i0].y;
			vx1=glif->verts[i1].x;
			vy1=glif->verts[i1].y;
			
			vx0-=vmx;	vy0-=vmy;
			vx1-=vmx;	vy1-=vmy;

			vx0=(vx0*rxs)>>16;
			vy0=(vy0*rys)>>16;
			vx1=(vx1*rxs)>>16;
			vy1=(vy1*rys)>>16;
			
			BGTF_DrawLineIx8(
				oimg, oxs, oys,
				vx0+oxo, vy0+oyo,
				vx1+oxo, vy1+oyo,
				clr);
			
			dx=vx1-vx0;
			dy=vy1-vy0;
			adx=abs(dx);
			ady=abs(dy);
			
			vx0b=(3*vx0+vx1)/4;
			vy0b=(3*vy0+vy1)/4;
			vx1b=(3*vx1+vx0)/4;
			vy1b=(3*vy1+vy0)/4;

#if 1

			if(ady>(2*adx))
			{
				if(dy<=-5)
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo+1, vy0+oyo,
						vx1+oxo+1, vy1+oyo,
						0xFB);
				}

				if(dy>=5)
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo-1, vy0+oyo,
						vx1+oxo-1, vy1+oyo,
						0xFB);
				}
			}else
				if(adx>(2*ady))
			{
				if(dx<=-5)
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo, vy0+oyo-1,
						vx1+oxo, vy1+oyo-1,
						0xFB);
				}

				if(dx>=5)
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo, vy0+oyo+1,
						vx1+oxo, vy1+oyo+1,
						0xFB);
				}
			}

#if 1
			else if((dx>0) && (dy>0))
			{
				if(ady>adx)
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo-1, vy0+oyo,
						vx1+oxo-1, vy1+oyo,
						0xFB);
				}else
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo, vy0+oyo+1,
						vx1+oxo, vy1+oyo+1,
						0xFB);
				}
			}else if((dx<0) && (dy<0))
			{
				if(ady>adx)
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo+1, vy0+oyo,
						vx1+oxo+1, vy1+oyo,
						0xFB);
				}else
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo, vy0+oyo-1,
						vx1+oxo, vy1+oyo-1,
						0xFB);
				}
			}
#endif

#if 1
			else if((dx>0) && (dy<0))
			{
				if(ady>adx)
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo+1, vy0+oyo,
						vx1+oxo+1, vy1+oyo,
						0xFB);
				}else
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo, vy0+oyo+1,
						vx1+oxo, vy1+oyo+1,
						0xFB);
				}
			}else if((dx<0) && (dy>0))
			{
				if(ady>adx)
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo-1, vy0+oyo,
						vx1+oxo-1, vy1+oyo,
						0xFB);
				}else
				{
					BGTF_DrawLineD2Ix8(
						oimg, oxs, oys,
						vx0+oxo, vy0+oyo-1,
						vx1+oxo, vy1+oyo-1,
						0xFB);
				}
			}
#endif

#endif

		}
	}

	return(0);
}

int BGTF_CheckFloodFillSpanIx8(
	byte *oimg, int oxs, int oys,
	int oxo, int oyo, int cxs, int cys,
	int y, int cr)
{
	int x;
	int i, j, k;
	
	for(x=0; x<cxs; x++)
	{
		i=(oyo+y)*oxs+(oxo+x);
		if(oimg[i]==0xF8)
		{
			if(y>0)
			{
				j=oimg[i-oxs];
				if(j==cr)
				{
					oimg[i]=j;
					continue;
				}
			}

			if(y<(cys-1))
			{
				j=oimg[i+oxs];
				if(j==cr)
				{
					oimg[i]=j;
					continue;
				}
			}

			if(x>0)
			{
				j=oimg[i-1];
				if(j==cr)
				{
					oimg[i]=j;
					continue;
				}
			}
		}
	}

	for(x=cxs-2; x>=0; x--)
	{
		i=(oyo+y)*oxs+(oxo+x);
		if(oimg[i]==0xF8)
		{
			j=oimg[i+1];
			if(j==cr)
			{
				oimg[i]=j;
				continue;
			}
		}
	}
}

int BGTF_GlyphDrawIndex8(BGTF_FontGlyph *glif,
	byte *oimg, int oxs, int oys, int oxo, int oyo,
	int cxs, int cys, int clr0, int clr1)
{
	int x, y, vx, vy, vxs, vys, vsz;
	int mx, my;
	int i, j, k;
	
	vxs=glif->image->celsz_x/cxs;
	vys=glif->image->celsz_y/cys;
	vsz=(vxs+vys)/2;
	
	mx=glif->image->min_x;
	my=glif->image->min_y;

	for(y=0; y<cys; y++)
		for(x=0; x<cxs; x++)
	{
		i=(oyo+y)*oxs+(oxo+x);
		oimg[i]=0xF8;
		
		if((y==0) || (x==0))
		{
			oimg[i]=0xFA;
		}

		if((y==(cys-1)) || (x==(cxs-1)))
		{
			oimg[i]=0xFA;
		}
	}

	BGTF_GlyphDrawLines(glif,
		oimg, oxs, oys, oxo, oyo,
		mx, my, vxs, vys, 0xF9);

	for(i=0; i<2; i++)
	{
		for(y=0; y<cys; y++)
		{
			BGTF_CheckFloodFillSpanIx8(
				oimg, oxs, oys,
				oxo, oyo, cxs, cys, y, 0xFA);
		}

		for(y=cys-1; y>0; y--)
		{
			BGTF_CheckFloodFillSpanIx8(
				oimg, oxs, oys,
				oxo, oyo, cxs, cys, y, 0xFA);
		}
	}

#if 0
	for(y=0; y<cys; y++)
		for(x=0; x<cxs; x++)
	{
		if((x==0) || (y==0))
			continue;
		if((x==(cxs-1)) || (y==(cys-1)))
			continue;

		i=(oyo+y)*oxs+(oxo+x);
		if((oimg[i]==0xF8) &&
			(oimg[i-oxs-1]==0xF8) &&
			(oimg[i-oxs+1]==0xF8) &&
			(oimg[i+oxs-1]==0xF8) &&
			(oimg[i+oxs+1]==0xF8))
		{
			j=oimg[i-oxs];
			if((j==0xFA) || (j==0xFB))
			{
				oimg[i]=j;
				continue;
			}

			j=oimg[i-1];
			if((j==0xFA) || (j==0xFB))
			{
				oimg[i]=j;
				continue;
			}

#if 1
			vx=x*vxs;
			vy=y*vys;	
			vx+=mx;
			vy+=my;
			k=BGTF_GlyphCheckPoint(glif, vx, vy, vsz/2);
			if(k)
				oimg[i]=0xFB;
			else
				oimg[i]=0xFA;
#endif
		}
	}
#endif

#if 1
	for(i=0; i<2; i++)
	{
		for(y=0; y<cys; y++)
		{
			BGTF_CheckFloodFillSpanIx8(
				oimg, oxs, oys,
				oxo, oyo, cxs, cys, y, 0xFB);
		}

		for(y=cys-1; y>0; y--)
		{
			BGTF_CheckFloodFillSpanIx8(
				oimg, oxs, oys,
				oxo, oyo, cxs, cys, y, 0xFB);
		}
	}
#endif

#if 1
	for(y=0; y<cys; y++)
		for(x=0; x<cxs; x++)
	{
		i=(oyo+y)*oxs+(oxo+x);
		j=oimg[i];
		
		if((j==0xF9)||(j==0xFB))
		{
			oimg[i]=clr1;
		}else
		{
			oimg[i]=clr0;
		}
	}
#endif

#if 0
	for(y=0; y<cys; y++)
		for(x=0; x<cxs; x++)
	{
		vx=x*vxs;
		vy=y*vys;
		
		vx+=mx;
		vy+=my;

		j=BGTF_GlyphCheckPoint(glif, vx, vy, vsz);
		i=(oyo+y)*oxs+(oxo+x);
//		oimg[i]=j?clr1:clr0;
		if(j)
			{ oimg[i]=clr1; }
		else if(clr0>=0)
			{ oimg[i]=clr0; }
	}
#endif
}

void BGTF_FetchFontGlyphBits(
	byte *dstbits, int dxs, int dys,
	BGTF_FontImage *font, int utf)
{
	static byte *tmpcel;
	static int sz_tmpcel;
	BGTF_FontGlyph *glif;
	int vxs, vys, n;
	int x, y, cx, cy, tc;
	int i, j, k;
	
	vxs=dxs*4;
	vys=dys*4;
	
	n=vxs*vys;
	
	if(!tmpcel)
	{
		k=n;
		if(k<(128*128))
			k=128*128;
		tmpcel=bgtf_malloc(k);
		sz_tmpcel=k;
	}
	
	if(n>sz_tmpcel)
	{
		k=sz_tmpcel;
		while(k<n)
			k=k+(k>>1);
		tmpcel=bgtf_realloc(tmpcel, k);
		sz_tmpcel=k;
	}

	memset(dstbits, 0, ((dxs*dys)+7)>>3);

	glif=BGTF_ImageGetGlyph(font, utf);
	if(!glif)
	{
//		memset(dst, 0, (n+7)>>3);
		return;
	}

	BGTF_GlyphDrawIndex8(glif,
		tmpcel, vxs, vys, 0, 0,
		vxs, vys, 0, 1);
	
	for(y=0; y<dys; y++)
		for(x=0; x<dxs; x++)
	{
		tc=0;
		for(cy=0; cy<4; cy++)
		{
			k=(y*4+cy)*vxs+(x*4+0);
			for(cx=0; cx<4; cx++)
			{
				if(tmpcel[k+cx])
					tc++;
			}
		}
		
		k=y*dxs+x;
		if(tc>=12)
		{
			dstbits[k>>3]|=1<<(k&7);
		}
	}
}
