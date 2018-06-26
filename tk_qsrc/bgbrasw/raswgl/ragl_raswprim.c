#include <bgbrasw.h>

void RASWGL_VertexToViewportPixels(
	RASWGL_Context *ctx, float *sxyz, int *dxy, bgbrasw_zbuf *dz)
{
	float f, g;
	float z, ez, zs;
	
	zs=(ctx->depth_far-ctx->depth_near);
//	z=(sxyz[2]*0.4999+0.5)*zs+ctx->depth_near;
	z=sxyz[2]*zs+ctx->depth_near;

#if 1
	if(ctx->caps_enabled&BGBRASW_ENABLE_OFFSET_FILL)
	{
		ez=z*0.0001;
		z+=ez*ctx->offset_factor+
			ctx->offset_units*(1.0/16711680.0);
	}
#endif

	if(z<0)z=0;
	if(z>1.0)z=1.0;

//	dxy[0]=(sxyz[0]*0.501+0.5)*ctx->viewport_xs+ctx->viewport_x+0.5;
//	dxy[1]=(sxyz[1]*0.501+0.5)*ctx->viewport_ys+ctx->viewport_y+0.5;
//	dxy[0]=(sxyz[0]*0.5+0.5)*ctx->viewport_xs+ctx->viewport_x+0.5;
//	dxy[1]=(sxyz[1]*0.5+0.5)*ctx->viewport_ys+ctx->viewport_y+0.5;

	dxy[0]=256*(sxyz[0]*0.5+0.5)*ctx->viewport_xs+ctx->viewport_x+0.5;
	dxy[1]=256*(sxyz[1]*0.5+0.5)*ctx->viewport_ys+ctx->viewport_y+0.5;

//	dz[0]=z*4294967295.0+0.5;
	dz[0]=z*4294967039.0+0.5;
	dz[0]&=0xFFFFFF00UL;

#if 0
	if(dxy[0]<ctx->viewport_x)
		dxy[0]=ctx->viewport_x;
	if(dxy[1]<ctx->viewport_y)
		dxy[1]=ctx->viewport_y;

//	if(dxy[0]>=(ctx->viewport_x+ctx->viewport_xs))
//		dxy[0]=(ctx->viewport_x+ctx->viewport_xs)-1;
//	if(dxy[1]>=(ctx->viewport_y+ctx->viewport_ys))
//		dxy[1]=(ctx->viewport_y+ctx->viewport_ys)-1;

	if(dxy[0]>(ctx->viewport_x+ctx->viewport_xs))
		dxy[0]=(ctx->viewport_x+ctx->viewport_xs);
	if(dxy[1]>(ctx->viewport_y+ctx->viewport_ys))
		dxy[1]=(ctx->viewport_y+ctx->viewport_ys);
#endif

#if 1
	if(dxy[0]<(256*ctx->viewport_x))
		dxy[0]=256*ctx->viewport_x;
	if(dxy[1]<(256*ctx->viewport_y))
		dxy[1]=256*ctx->viewport_y;

	if(dxy[0]>(256*(ctx->viewport_x+ctx->viewport_xs)))
		dxy[0]=256*(ctx->viewport_x+ctx->viewport_xs);
	if(dxy[1]>(256*(ctx->viewport_y+ctx->viewport_ys)))
		dxy[1]=256*(ctx->viewport_y+ctx->viewport_ys);
#endif

	if((ctx->ractx->tgt_xs!=ctx->ractx->tgt_lxs) ||
		(ctx->ractx->tgt_ys!=ctx->ractx->tgt_lys))
	{
		f=(float)ctx->ractx->tgt_xs/(float)ctx->ractx->tgt_lxs;
		g=(float)ctx->ractx->tgt_ys/(float)ctx->ractx->tgt_lys;
		dxy[0]=dxy[0]*f;
		dxy[1]=dxy[1]*g;
	}
}

void RASWGL_CoordsToViewportTexels(
	RASWGL_Context *ctx, BGBRASW_Texture *tex,
	float *suv, int *duv)
{
	if(!tex)
	{
		duv[0]=0; duv[1]=0;
		return;
	}

//	duv[0]=suv[0]*tex->xs;
//	duv[1]=suv[1]*tex->ys;
//	duv[0]=suv[0]*tex->xs+0.5;
//	duv[1]=suv[1]*tex->ys+0.5;

//	duv[0]=suv[0]*tex->xs*256;
//	duv[1]=suv[1]*tex->ys*256;
	duv[0]=suv[0]*tex->xs*256+0.5;
	duv[1]=suv[1]*tex->ys*256+0.5;
}

void RASWGL_ColorsToViewportColors(
	RASWGL_Context *ctx,
	float *srgb, bgbrasw_pixel *drgb)
{
	int cr, cg, cb, ca;

	cr=srgb[0]*255;	cg=srgb[1]*255;
	cb=srgb[2]*255;	ca=srgb[3]*255;
	cr=BGBRASW_CLAMP(cr, 0, 255);
	cg=BGBRASW_CLAMP(cg, 0, 255);
	cb=BGBRASW_CLAMP(cb, 0, 255);
	ca=BGBRASW_CLAMP(ca, 0, 255);
	
	*drgb=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
}

void RASWGL_ColorsToViewportColors2(
	RASWGL_Context *ctx,
	float *xyzw, float *srgb,
	bgbrasw_pixel *drgb)
{
	float tv[8];
	int cr, cg, cb, ca;

	if(!(ctx->caps_enabled&BGBRASW_ENABLE_LIGHTING))
	{
		RASWGL_ColorsToViewportColors(ctx, srgb, drgb);
		return;
	}
	
	RASWGL_CalcLightVertexColor(ctx, xyzw, srgb, tv);

	cr=tv[0]*255;	cg=tv[1]*255;
	cb=tv[2]*255;	ca=tv[3]*255;
	cr=BGBRASW_CLAMP(cr, 0, 255);
	cg=BGBRASW_CLAMP(cg, 0, 255);
	cb=BGBRASW_CLAMP(cb, 0, 255);
	ca=BGBRASW_CLAMP(ca, 0, 255);
	*drgb=BGBRASW_MAKEPIXEL(cr, cg, cb, ca);
}

int RASWGL_ClasifyPrimitive_FlatColorP(
	RASWGL_Context *ctx, BGBRASW_Primitive *prim)
{
	if(prim->type==BGBRASW_PTYPE_TRIANGLE)
	{
		if((prim->rgba[0]==prim->rgba[1])&&
			(prim->rgba[1]==prim->rgba[2]))
				return(1);
		return(0);
	}

	if(prim->type==BGBRASW_PTYPE_QUAD)
	{
		if((prim->rgba[0]==prim->rgba[1])&&
			(prim->rgba[1]==prim->rgba[2])&&
			(prim->rgba[2]==prim->rgba[3]))
				return(1);
		return(0);
	}
	
	return(0);
}

int RASWGL_ClasifyPrimitive_OpaqueP(
	RASWGL_Context *ctx, BGBRASW_Primitive *prim)
{
	if(prim->type==BGBRASW_PTYPE_TRIANGLE)
	{
		if((prim->rgba[0]==prim->rgba[1])&&
			(prim->rgba[1]==prim->rgba[2]))
		{
			if(BGBRASW_PIXEL_A(prim->rgba[0])>=240)
			{
				if(!prim->tex)return(1);
				if(prim->tex->flags&BGBRASW_TEXFLAG_OPAQUE)
					return(1);
			}
		}
		if((BGBRASW_PIXEL_A(prim->rgba[0])>=240) &&
			(BGBRASW_PIXEL_A(prim->rgba[1])>=240) &&
			(BGBRASW_PIXEL_A(prim->rgba[2])>=240))
		{
			if(!prim->tex)return(1);
			if(prim->tex->flags&BGBRASW_TEXFLAG_OPAQUE)
				return(1);
		}
		return(0);
	}

	if(prim->type==BGBRASW_PTYPE_QUAD)
	{
		if((prim->rgba[0]==prim->rgba[1])&&
			(prim->rgba[1]==prim->rgba[2])&&
			(prim->rgba[2]==prim->rgba[3]))
		{
			if(BGBRASW_PIXEL_A(prim->rgba[0])>=240)
			{
				if(!prim->tex)return(1);
				if(prim->tex->flags&BGBRASW_TEXFLAG_OPAQUE)
					return(1);
			}
		}
		if((BGBRASW_PIXEL_A(prim->rgba[0])>=240) &&
			(BGBRASW_PIXEL_A(prim->rgba[1])>=240) &&
			(BGBRASW_PIXEL_A(prim->rgba[2])>=240) &&
			(BGBRASW_PIXEL_A(prim->rgba[3])>=240))
		{
			if(!prim->tex)return(1);
			if(prim->tex->flags&BGBRASW_TEXFLAG_OPAQUE)
				return(1);
		}
		return(0);
	}
	
	return(0);
}

int RASWGL_ClasifyPrimitive_BasicOpaqueP(
	RASWGL_Context *ctx, BGBRASW_Primitive *prim)
{
	if(prim->type==BGBRASW_PTYPE_TRIANGLE)
	{
		if((prim->rgba[0]==prim->rgba[1])&&
			(prim->rgba[1]==prim->rgba[2]))
		{
//			if(prim->rgba[0]==0xFFFFFFFFUL)
			if((BGBRASW_PIXEL_A(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[0])>=240))
			{
				if(!prim->tex)return(1);
				if(prim->tex->flags&BGBRASW_TEXFLAG_OPAQUE)
					return(1);
			}
		}else
		{
			if((BGBRASW_PIXEL_A(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_A(prim->rgba[1])>=240) &&
				(BGBRASW_PIXEL_A(prim->rgba[2])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[1])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[2])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[1])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[2])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[1])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[2])>=240))
			{
				return(1);
			}
		}

		return(0);
	}

	if(prim->type==BGBRASW_PTYPE_QUAD)
	{
		if((prim->rgba[0]==prim->rgba[1])&&
			(prim->rgba[1]==prim->rgba[2])&&
			(prim->rgba[2]==prim->rgba[3]))
		{
//			if(prim->rgba[0]==0xFFFFFFFFUL)
			if((BGBRASW_PIXEL_A(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[0])>=240))
			{
				if(!prim->tex)return(1);
				if(prim->tex->flags&BGBRASW_TEXFLAG_OPAQUE)
					return(1);
			}
		}else
		{
			if((BGBRASW_PIXEL_A(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_A(prim->rgba[1])>=240) &&
				(BGBRASW_PIXEL_A(prim->rgba[2])>=240) &&
				(BGBRASW_PIXEL_A(prim->rgba[3])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[1])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[2])>=240) &&
				(BGBRASW_PIXEL_G(prim->rgba[3])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[1])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[2])>=240) &&
				(BGBRASW_PIXEL_R(prim->rgba[3])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[0])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[1])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[2])>=240) &&
				(BGBRASW_PIXEL_B(prim->rgba[3])>=240))
			{
				return(1);
			}
		}

		return(0);
	}
	
	return(0);
}

int RASWGL_ClasifyPrimitive_TexValidP(
	RASWGL_Context *ctx, BGBRASW_Primitive *prim)
{
	if(!prim->tex)
		return(0);
	if(!prim->tex->rgba[0])
		return(0);
	return(1);
}

int RASWGL_ClasifyPrimitive_NeedsTabsP(
	RASWGL_Context *ctx, BGBRASW_Primitive *prim)
{
	int rt;
	if(ctx->caps_enabled&BGBRASW_ENABLE_ALPHA_TEST)
	{
		if((ctx->caps_enabled&BGBRASW_ENABLE_TEXTURE_2D) &&
			RASWGL_ClasifyPrimitive_TexValidP(ctx, prim) &&
			!RASWGL_ClasifyPrimitive_OpaqueP(ctx, prim))
		{
			return(1);
		}
	}

	if(ctx->caps_enabled&BGBRASW_ENABLE_STENCIL_TEST)
	{
		return(1);
	}

	if(ctx->caps_enabled&BGBRASW_ENABLE_BLEND)
	{
		rt=1;
		switch(ctx->blend_src)
		{
		case GL_ZERO:
			break;
		case GL_ONE:
			switch(ctx->blend_dst)
			{
			case GL_ZERO:
				rt=0;
				break;
			default:
				break;
			}
			break;
		case GL_SRC_ALPHA:
			switch(ctx->blend_dst)
			{
			case GL_ONE_MINUS_SRC_ALPHA:
				if((!(ctx->caps_enabled&BGBRASW_ENABLE_TEXTURE_2D) ||
					!RASWGL_ClasifyPrimitive_TexValidP(ctx, prim)) &&
					RASWGL_ClasifyPrimitive_OpaqueP(ctx, prim))
						rt=0;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		if(rt)return(1);
	}
	
	if(ctx->caps_enabled&BGBRASW_ENABLE_DEPTH_TEST)
	{
		if((ctx->depth_func!=GL_LESS) &&
			(ctx->depth_func!=GL_LEQUAL))
				return(1);
	}

	return(0);
}

void RASWGL_ClasifyPrimitive(
	RASWGL_Context *ctx, BGBRASW_Primitive *prim)
{
	prim->mode=BGBRASW_PMODE_NONE;

	if(RASWGL_ClasifyPrimitive_NeedsTabsP(ctx, prim))
	{
		if((ctx->caps_enabled&BGBRASW_ENABLE_TEXTURE_2D) &&
			RASWGL_ClasifyPrimitive_TexValidP(ctx, prim))
		{
			prim->mode=BGBRASW_PMODE_TEXINTERPTABS;
		}else
		{
			prim->mode=BGBRASW_PMODE_FLATINTERPTABS;
		}
		
		return;
	}

	if(ctx->caps_enabled&BGBRASW_ENABLE_DEPTH_TEST)
	{
		if((ctx->caps_enabled&BGBRASW_ENABLE_TEXTURE_2D) &&
			RASWGL_ClasifyPrimitive_TexValidP(ctx, prim))
		{
			if(RASWGL_ClasifyPrimitive_BasicOpaqueP(ctx, prim))
				{ prim->mode=BGBRASW_PMODE_TEXBASICZTEST; }
			else
				{ prim->mode=BGBRASW_PMODE_TEXINTERPZTEST; }
		}else
		{
			if(RASWGL_ClasifyPrimitive_FlatColorP(ctx, prim))
				{ prim->mode=BGBRASW_PMODE_FLATBASICZTEST; }
			else
				{ prim->mode=BGBRASW_PMODE_FLATINTERPZTEST; }
		}
	}else
	{
		if((ctx->caps_enabled&BGBRASW_ENABLE_TEXTURE_2D) &&
			RASWGL_ClasifyPrimitive_TexValidP(ctx, prim))
		{
			if(RASWGL_ClasifyPrimitive_BasicOpaqueP(ctx, prim))
				{ prim->mode=BGBRASW_PMODE_TEXBASIC; }
			else
				{ prim->mode=BGBRASW_PMODE_TEXINTERP; }
		}else
		{
			if(RASWGL_ClasifyPrimitive_FlatColorP(ctx, prim))
				{ prim->mode=BGBRASW_PMODE_FLATBASIC; }
			else
				{ prim->mode=BGBRASW_PMODE_FLATINTERP; }
		}
	}
}

int raswgl_dist2d2(int *xy0, int *xy1)
{
	int d, dx, dy;
	dx=xy0[0]-xy1[0];
	dy=xy0[1]-xy1[1];
	d=dx*dx+dy*dy;
	if(d<0)d=0;
	return(d);
}

void raswgl_avgxy2d(
	int *xy0, int *xy1,
	int *st0, int *st1,
	bgbrasw_pixel clra,	bgbrasw_pixel clrb,
	bgbrasw_zbuf z0, bgbrasw_zbuf z1, int w0, int w1,
	int *dxy, int *dst,
	bgbrasw_pixel *dclr,
	bgbrasw_zbuf *dz, int *dw)
{
//	float gz0, gz1;
	int fz0, fz1;
	int px0, py0, px1, py1, px2, py2;
	int ps0, pt0, ps1, pt1, ps2, pt2;
	int pz0, pz1, pz2;
	
#if 1
	fz0=(z0>>24)&255;
	fz1=(z1>>24)&255;
//	fz0=(fz0*w0)>>8;
//	fz1=(fz1*w1)>>8;
	if(!fz0)fz0++;
	if(!fz1)fz1++;

//	px0=(xy0[0]<<8)/fz0;	py0=(xy0[1]<<8)/fz0;
//	px1=(xy1[0]<<8)/fz1;	py1=(xy1[1]<<8)/fz1;

	ps0=(st0[0]<<8)/fz0;	pt0=(st0[1]<<8)/fz0;
	ps1=(st1[0]<<8)/fz1;	pt1=(st1[1]<<8)/fz1;

//	px2=(px0+px1)>>1;	py2=(py0+py1)>>1;
	ps2=(ps0+ps1)>>1;	pt2=(pt0+pt1)>>1;

//	px2=(px2*(fz0+fz1))>>9;	py2=(py2*(fz0+fz1))>>9;
	ps2=(ps2*(fz0+fz1))>>9;	pt2=(pt2*(fz0+fz1))>>9;

//	dxy[0]=px2;	dxy[1]=py2;
	dst[0]=ps2;	dst[1]=pt2;

	dxy[0]=(xy0[0]+xy1[0])>>1;
	dxy[1]=(xy0[1]+xy1[1])>>1;

//	dst[0]=(st0[0]+st1[0])>>1;
//	dst[1]=(st0[1]+st1[1])>>1;
	
//	pz0=z0/fz0;
//	pz1=z1/fz1;
//	pz2=(pz0+pz1)>>1;
//	pz2=pz2*((fz0+fz1)>>1);
//	*dz=pz2;
	
	*dclr=((clra>>1)&0x7F7F7F7FUL)+((clrb>>1)&0x7F7F7F7FUL);
	*dz=((z0>>1)+(z1>>1));
	*dw=(w0+w1)>>1;
#endif

#if 0
	dxy[0]=(xy0[0]+xy1[0])>>1;
	dxy[1]=(xy0[1]+xy1[1])>>1;

	dst[0]=(st0[0]+st1[0])>>1;
	dst[1]=(st0[1]+st1[1])>>1;
	
	*dclr=((clra>>1)&0x7F7F7F7FUL)+((clrb>>1)&0x7F7F7F7FUL);
	*dz=((z0>>1)+(z1>>1));
#endif
}

int raswgl_copyxy2d(
	int *sxy, int *sst, bgbrasw_pixel  sclr, bgbrasw_zbuf  sz, int sw,
	int *dxy, int *dst, bgbrasw_pixel *dclr, bgbrasw_zbuf *dz, int *dw)
{
	dxy[0]=sxy[0];	dxy[1]=sxy[1];
	dst[0]=sst[0];	dst[1]=sst[1];
	*dclr=sclr;		*dz=sz;
}

BGBRASW_Primitive *RASWGL_SubdivideInsertTriangle(
	RASWGL_Context *ctx,
	BGBRASW_Primitive *plst,
	BGBRASW_Primitive *ptmp)
{
	int pxy[4][2], pst[4][2];
	int pz[4], pw[4];
	bgbrasw_pixel pclr[4];
	BGBRASW_Primitive *ptmp2;
	int d0, d1, d2;
	
	d0=raswgl_dist2d2(ptmp->xy[0], ptmp->xy[1]);
	d1=raswgl_dist2d2(ptmp->xy[1], ptmp->xy[2]);
	d2=raswgl_dist2d2(ptmp->xy[2], ptmp->xy[0]);
//	if((d0+d1+d2)<(3*4096))
	if((d0+d1+d2)<(3*16384))
	{
		ptmp->next=plst;
		plst=ptmp;
		return(plst);
	}
	
	raswgl_avgxy2d(
		ptmp->xy[0],	ptmp->xy[1],	ptmp->st[0],	ptmp->st[1],
		ptmp->rgba[0],	ptmp->rgba[1],	ptmp->z[0],		ptmp->z[1],
		ptmp->w[0],		ptmp->w[1],
		pxy[0], pst[0], pclr+0, pz+0, pw+0);
	raswgl_avgxy2d(
		ptmp->xy[1],	ptmp->xy[2],	ptmp->st[1],	ptmp->st[2],
		ptmp->rgba[1],	ptmp->rgba[2],	ptmp->z[1],		ptmp->z[2],
		ptmp->w[1],		ptmp->w[2],
		pxy[1], pst[1], pclr+1, pz+1, pw+1);
	raswgl_avgxy2d(
		ptmp->xy[2],	ptmp->xy[0],		ptmp->st[2],	ptmp->st[0],
		ptmp->rgba[2],	ptmp->rgba[0],		ptmp->z[2],		ptmp->z[0],
		ptmp->w[2],		ptmp->w[0],
		pxy[2], pst[2], pclr+2, pz+2, pw+2);

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=ptmp->type;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;
	raswgl_copyxy2d(ptmp->xy[0], ptmp->st[0], ptmp->rgba[0],
		ptmp->z[0], ptmp->w[0],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp2->z+0, ptmp2->w+0);
	raswgl_copyxy2d(pxy[0], pst[0], pclr[0], pz[0], pw[0],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp2->z+1, ptmp2->w+1);
	raswgl_copyxy2d(pxy[2], pst[2], pclr[2], pz[2], pw[2],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp2->z+2, ptmp2->w+2);
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=ptmp->type;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;
	raswgl_copyxy2d(pxy[0], pst[0], pclr[0], pz[0], pw[0],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp2->z+0, ptmp2->w+0);
	raswgl_copyxy2d(ptmp->xy[1], ptmp->st[1], ptmp->rgba[1],
		ptmp->z[1], ptmp->w[1],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp2->z+1, ptmp2->w+1);
	raswgl_copyxy2d(pxy[1], pst[1], pclr[1], pz[1], pw[1],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp2->z+2, ptmp2->w+2);
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=ptmp->type;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;
	raswgl_copyxy2d(pxy[2], pst[2], pclr[2], pz[2], pw[2],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp2->z+0, ptmp2->w+0);
	raswgl_copyxy2d(pxy[1], pst[1], pclr[1], pz[1], pw[1],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp2->z+1, ptmp2->w+1);
	raswgl_copyxy2d(ptmp->xy[2], ptmp->st[2], ptmp->rgba[2],
		ptmp->z[2], ptmp->w[2],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp2->z+2, ptmp2->w+2);
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=ptmp->type;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;
	raswgl_copyxy2d(pxy[0], pst[0], pclr[0], pz[0], pw[0],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp2->z+0, ptmp2->w+0);
	raswgl_copyxy2d(pxy[1], pst[1], pclr[1], pz[1], pw[1],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp2->z+1, ptmp2->w+1);
	raswgl_copyxy2d(pxy[2], pst[2], pclr[2], pz[2], pw[2],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp2->z+2, ptmp2->w+2);
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	BGBRASW_FreePrimitive(ctx->ractx, ptmp);

	return(plst);
}

BGBRASW_Primitive *RASWGL_SubdivideInsertQuad(
	RASWGL_Context *ctx,
	BGBRASW_Primitive *plst,
	BGBRASW_Primitive *ptmp)
{
	int pxy[6][2], pst[6][2];
	int pz[4];
	bgbrasw_pixel pclr[4];
	BGBRASW_Primitive *ptmp1, *ptmp2;
	int d0, d1, d2, d3;
	
	if(ptmp->type==BGBRASW_PTYPE_TRIANGLE)
	{
		return(RASWGL_SubdivideInsertTriangle(
			ctx, plst, ptmp));
	}
	
	d0=raswgl_dist2d2(ptmp->xy[0], ptmp->xy[1]);
	d1=raswgl_dist2d2(ptmp->xy[1], ptmp->xy[2]);
	d2=raswgl_dist2d2(ptmp->xy[2], ptmp->xy[3]);
	d3=raswgl_dist2d2(ptmp->xy[3], ptmp->xy[0]);
//	if((d0+d1+d2)<(3*4096))
	if((d0+d1+d2+d3)<(4*16384))
	{
		ptmp->next=plst;
		plst=ptmp;
		return(plst);
	}

	ptmp1=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp1->type=BGBRASW_PTYPE_TRIANGLE;		ptmp1->mode=ptmp->mode;
	ptmp1-> tex=ptmp-> tex;		ptmp1->tabs=ptmp->tabs;

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=BGBRASW_PTYPE_TRIANGLE;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;

	raswgl_copyxy2d(ptmp->xy[0], ptmp->st[0], ptmp->rgba[0],
		ptmp->z[0], ptmp->w[0],
		ptmp1->xy[0], ptmp1->st[0], ptmp1->rgba+0, ptmp1->z+0, ptmp1->w+0);
	raswgl_copyxy2d(ptmp->xy[1], ptmp->st[1], ptmp->rgba[1],
		ptmp->z[1], ptmp->w[1],
		ptmp1->xy[1], ptmp1->st[1], ptmp1->rgba+1, ptmp1->z+1, ptmp1->w+1);
	raswgl_copyxy2d(ptmp->xy[2], ptmp->st[2], ptmp->rgba[2],
		ptmp->z[2], ptmp->w[2],
		ptmp1->xy[2], ptmp1->st[2], ptmp1->rgba+2, ptmp1->z+2, ptmp1->w+2);
//	ptmp1->w[0]=ptmp->w[0];	ptmp1->w[1]=ptmp->w[1];	ptmp1->w[2]=ptmp->w[2];
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp1);

	raswgl_copyxy2d(ptmp->xy[0], ptmp->st[0], ptmp->rgba[0],
		ptmp->z[0], ptmp->w[0],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp2->z+0, ptmp2->w+0);
	raswgl_copyxy2d(ptmp->xy[2], ptmp->st[2], ptmp->rgba[2],
		ptmp->z[2], ptmp->w[2],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp2->z+1, ptmp2->w+1);
	raswgl_copyxy2d(ptmp->xy[3], ptmp->st[3], ptmp->rgba[3],
		ptmp->z[3], ptmp->w[3],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp2->z+2, ptmp2->w+2);
//	ptmp2->w[0]=ptmp->w[0];	ptmp2->w[1]=ptmp->w[2];	ptmp2->w[2]=ptmp->w[3];
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	BGBRASW_FreePrimitive(ctx->ractx, ptmp);

	return(plst);

#if 0
	raswgl_avgxy2d(
		ptmp->xy[0],	ptmp->xy[1],	ptmp->st[0],	ptmp->st[1],
		ptmp->rgba[0],	ptmp->rgba[1],	ptmp->z[0],		ptmp->z[1],
		pxy[0], pst[0], pclr+0, pz+0);
	raswgl_avgxy2d(
		ptmp->xy[1],	ptmp->xy[2],	ptmp->st[1],	ptmp->st[2],
		ptmp->rgba[1],	ptmp->rgba[2],	ptmp->z[1],		ptmp->z[2],
		pxy[1], pst[1], pclr+1, pz+1);
	raswgl_avgxy2d(
		ptmp->xy[2],	ptmp->xy[3],		ptmp->st[2],	ptmp->st[3],
		ptmp->rgba[2],	ptmp->rgba[3],		ptmp->z[2],		ptmp->z[3],
		pxy[2], pst[2], pclr+2, pz+2);
	raswgl_avgxy2d(
		ptmp->xy[3],	ptmp->xy[0],		ptmp->st[3],	ptmp->st[0],
		ptmp->rgba[3],	ptmp->rgba[0],		ptmp->z[3],		ptmp->z[0],
		pxy[3], pst[3], pclr+3, pz+3);

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=ptmp->type;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;
	raswgl_copyxy2d(ptmp->xy[0], ptmp->st[0], ptmp->rgba[0], ptmp->z[0],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp->z+0);
	raswgl_copyxy2d(pxy[0], pst[0], pclr[0], pz[0],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp->z+1);
	raswgl_copyxy2d(pxy[2], pst[2], pclr[2], pz[2],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp->z+2);
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=ptmp->type;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;
	raswgl_copyxy2d(pxy[0], pst[0], pclr[0], pz[0],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp->z+0);
	raswgl_copyxy2d(ptmp->xy[1], ptmp->st[1], ptmp->rgba[1], ptmp->z[1],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp->z+1);
	raswgl_copyxy2d(pxy[1], pst[1], pclr[1], pz[1],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp->z+2);
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=ptmp->type;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;
	raswgl_copyxy2d(pxy[2], pst[2], pclr[2], pz[2],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp->z+0);
	raswgl_copyxy2d(pxy[1], pst[1], pclr[1], pz[1],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp->z+1);
	raswgl_copyxy2d(ptmp->xy[2], ptmp->st[2], ptmp->rgba[2], ptmp->z[2],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp->z+2);
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	ptmp2=BGBRASW_AllocPrimitive(ctx->ractx);
	ptmp2->type=ptmp->type;		ptmp2->mode=ptmp->mode;
	ptmp2-> tex=ptmp-> tex;		ptmp2->tabs=ptmp->tabs;
	raswgl_copyxy2d(pxy[0], pst[0], pclr[0], pz[0],
		ptmp2->xy[0], ptmp2->st[0], ptmp2->rgba+0, ptmp->z+0);
	raswgl_copyxy2d(pxy[1], pst[1], pclr[1], pz[1],
		ptmp2->xy[1], ptmp2->st[1], ptmp2->rgba+1, ptmp->z+1);
	raswgl_copyxy2d(pxy[2], pst[2], pclr[2], pz[2],
		ptmp2->xy[2], ptmp2->st[2], ptmp2->rgba+2, ptmp->z+2);
	plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp2);

	BGBRASW_FreePrimitive(ctx->ractx, ptmp);

	return(plst);
#endif
}

BGBRASW_Primitive *RASWGL_BuildPrimitiveListArrays(
	RASWGL_Context *ctx,
	int prim, int base, int nxyz,
	int xyzsize, float *xyz, 
	int stsize, float *st, 
	int normsize, float *norm,
	int rgbsize, float *rgb)
{
	BGBRASW_Primitive *plst, *ptmp;
	int i, j, k, n;
	
	if(prim==GL_TRIANGLES)
	{
		plst=0; n=nxyz/3;
		for(i=0; i<n; i++)
		{
			ptmp=BGBRASW_AllocPrimitive(ctx->ractx);
			ptmp->type=BGBRASW_PTYPE_TRIANGLE;
			ptmp->tex=ctx->texture;
			ptmp->tabs=ctx->tabs;

//			Vec4F_Copy()

			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*3+0)*xyzsize, ptmp->xy[0], ptmp->z+0);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*3+1)*xyzsize, ptmp->xy[1], ptmp->z+1);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*3+2)*xyzsize, ptmp->xy[2], ptmp->z+2);

			ptmp->w[0]=(xyz+(base+i*3+0)*xyzsize)[3];
			ptmp->w[1]=(xyz+(base+i*3+1)*xyzsize)[3];
			ptmp->w[2]=(xyz+(base+i*3+2)*xyzsize)[3];

			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*3+0)*stsize, ptmp->st[0]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*3+1)*stsize, ptmp->st[1]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*3+2)*stsize, ptmp->st[2]);

//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*3+0)*rgbsize, ptmp->rgba+0);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*3+1)*rgbsize, ptmp->rgba+1);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*3+2)*rgbsize, ptmp->rgba+2);

			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*3+0)*xyzsize,
				rgb+(base+i*3+0)*rgbsize, ptmp->rgba+0);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*3+1)*xyzsize,
				rgb+(base+i*3+1)*rgbsize, ptmp->rgba+1);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*3+2)*xyzsize,
				rgb+(base+i*3+2)*rgbsize, ptmp->rgba+2);

#if 0
			ptmp->bbox[0][0]=min(ptmp->xy[0][0],
				min(ptmp->xy[1][0], ptmp->xy[2][0]));
			ptmp->bbox[0][1]=min(ptmp->xy[0][1],
				min(ptmp->xy[1][1], ptmp->xy[2][1]));

			ptmp->bbox[1][0]=max(ptmp->xy[0][0],
				max(ptmp->xy[1][0], ptmp->xy[2][0]));
			ptmp->bbox[1][1]=max(ptmp->xy[0][1],
				max(ptmp->xy[1][1], ptmp->xy[2][1]));

			ptmp->bbox[0][2]=min(ptmp->z[0], min(ptmp->z[1], ptmp->z[2]));
			ptmp->bbox[1][2]=max(ptmp->z[0], max(ptmp->z[1], ptmp->z[2]));
#endif

			RASWGL_ClasifyPrimitive(ctx, ptmp);
			
//			plst=RASWGL_SubdivideInsertTriangle(ctx, plst, ptmp);
			
			ptmp->next=plst;
			plst=ptmp;
		}
		
		return(plst);
	}

	if(prim==GL_QUADS)
	{
		plst=0; n=nxyz/4;
		for(i=0; i<n; i++)
		{
			ptmp=BGBRASW_AllocPrimitive(ctx->ractx);
			ptmp->type=BGBRASW_PTYPE_QUAD;
			ptmp->tex=ctx->texture;
			ptmp->tabs=ctx->tabs;

			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*4+0)*xyzsize, ptmp->xy[0], ptmp->z+0);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*4+1)*xyzsize, ptmp->xy[1], ptmp->z+1);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*4+2)*xyzsize, ptmp->xy[2], ptmp->z+2);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*4+3)*xyzsize, ptmp->xy[3], ptmp->z+3);

			ptmp->w[0]=(xyz+(base+i*4+0)*xyzsize)[3];
			ptmp->w[1]=(xyz+(base+i*4+1)*xyzsize)[3];
			ptmp->w[2]=(xyz+(base+i*4+2)*xyzsize)[3];
			ptmp->w[3]=(xyz+(base+i*4+3)*xyzsize)[3];

			if((ptmp->xy[2][0]==ptmp->xy[3][0]) &&
				(ptmp->xy[2][1]==ptmp->xy[3][1]) &&
				(ptmp->z[2]==ptmp->z[3]))
			{
				ptmp->type=BGBRASW_PTYPE_TRIANGLE;
			}

			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*4+0)*stsize, ptmp->st[0]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*4+1)*stsize, ptmp->st[1]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*4+2)*stsize, ptmp->st[2]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*4+3)*stsize, ptmp->st[3]);

//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*4+0)*rgbsize, ptmp->rgba+0);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*4+1)*rgbsize, ptmp->rgba+1);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*4+2)*rgbsize, ptmp->rgba+2);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*4+3)*rgbsize, ptmp->rgba+3);

			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*4+0)*xyzsize,
				rgb+(base+i*4+0)*rgbsize, ptmp->rgba+0);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*4+1)*xyzsize,
				rgb+(base+i*4+1)*rgbsize, ptmp->rgba+1);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*4+2)*xyzsize,
				rgb+(base+i*4+2)*rgbsize, ptmp->rgba+2);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*4+3)*xyzsize,
				rgb+(base+i*4+3)*rgbsize, ptmp->rgba+3);

			ptmp->bbox[0][0]=min(
				min(ptmp->xy[0][0], ptmp->xy[1][0]),
				min(ptmp->xy[2][0], ptmp->xy[3][0]));
			ptmp->bbox[0][1]=min(
				min(ptmp->xy[0][1], ptmp->xy[1][1]),
				min(ptmp->xy[2][1], ptmp->xy[3][1]));
			ptmp->bbox[0][0]=max(
				max(ptmp->xy[0][0], ptmp->xy[1][0]),
				max(ptmp->xy[2][0], ptmp->xy[3][0]));
			ptmp->bbox[0][1]=max(
				max(ptmp->xy[0][1], ptmp->xy[1][1]),
				max(ptmp->xy[2][1], ptmp->xy[3][1]));

			ptmp->bbox[0][2]=min(
				min(ptmp->z[0], ptmp->z[1]),
				min(ptmp->z[2], ptmp->z[3]));
			ptmp->bbox[0][2]=max(
				max(ptmp->z[0], ptmp->z[1]),
				max(ptmp->z[2], ptmp->z[3]));
			
			RASWGL_ClasifyPrimitive(ctx, ptmp);

//			plst=RASWGL_SubdivideInsertQuad(ctx, plst, ptmp);

			ptmp->next=plst;
			plst=ptmp;
		}
		
		return(plst);
	}
	
	return(NULL);
}


BGBRASW_Primitive *RASWGL_AllocPrimitiveForPoint(
	RASWGL_Context *ctx, float *org,
	float *min, float *max)
{
	BGBRASW_Primitive *plst, *ptmp;
	float f;
	int n, n1, n2;

	if(!ctx->ractx_n_thread)
	{
		ptmp=BGBRASW_AllocPrimitive(ctx->ractx);
		ptmp->tabs=ctx->ractx->tabs;
		ptmp->seq=-1;
		return(ptmp);
	}
	
	f=(org[1]+1)*0.5;
	n=f*(ctx->ractx_n_thread-1)+0.5;

	f=(min[1]+1)*0.5;
	n1=f*(ctx->ractx_n_thread-1)+0.5;
	f=(max[1]+1)*0.5;
	n2=f*(ctx->ractx_n_thread-1)+0.5;

	if(n<0)n=0;
	if(n1<0)n1=0;
	if(n2<0)n2=0;
	if(n>=ctx->ractx_n_thread)
		n=ctx->ractx_n_thread-1;
	if(n1>=ctx->ractx_n_thread)
		n1=ctx->ractx_n_thread-1;
	if(n2>=ctx->ractx_n_thread)
		n2=ctx->ractx_n_thread-1;

	BGBRASW_LockContext(ctx->ractx_thread[n]);
	ptmp=BGBRASW_AllocPrimitive(ctx->ractx_thread[n]);
	ptmp->tabs=ctx->ractx_thread[n]->tabs;
	ptmp->tabs->refcount++;
	ptmp->seq=-1;
	if((n1!=n) || (n2!=n))
		ptmp->seq=-2;
	BGBRASW_UnlockContext(ctx->ractx_thread[n]);
	return(ptmp);
}

void RASWGL_SubmitPrimitive(
	RASWGL_Context *ctx, BGBRASW_Primitive *prim)
{
	if(ctx->ractx_n_thread)
	{
		BGBRASW_LockContext(prim->ctx);

		if(prim->seq==-2)
			{ prim->seq=ctx->ractx->priseq++; }
		
		if(prim->ctx->pridraw)
		{
			prim->next=NULL;
			prim->ctx->pridrawe->next=prim;
			prim->ctx->pridrawe=prim;
//			prim->next=prim->ctx->pridraw;
//			prim->ctx->pridraw=prim;
		}else
		{
			prim->next=NULL;
			prim->ctx->pridraw=prim;
			prim->ctx->pridrawe=prim;
		}
		BGBRASW_UnlockContext(prim->ctx);
	}else
	{
		prim->next=prim->ctx->pridraw;
		prim->ctx->pridraw=prim;
	}
}

void RASWGL_BuildInsertPrimitiveArrays(
	RASWGL_Context *ctx,
	int prim, int base, int nxyz,
	int xyzsize, float *xyz, 
	int stsize, float *st, 
	int normsize, float *norm,
	int rgbsize, float *rgb)
{
	BGBRASW_Primitive *plst, *ptmp;
	float tv[4], tv_min[4], tv_max[4];
	int i, j, k, n;
	
	if(prim==GL_LINES)
	{
		plst=0; n=nxyz/2;
		for(i=0; i<n; i++)
		{
			tv_min[1]=min(
						xyz[(base+i*2+0)*xyzsize+1],
						xyz[(base+i*2+1)*xyzsize+1]);
			tv_max[1]=max(
						xyz[(base+i*2+0)*xyzsize+1],
						xyz[(base+i*2+1)*xyzsize+1]);
			tv[1]=	(xyz[(base+i*2+0)*xyzsize+1]+
					 xyz[(base+i*2+1)*xyzsize+1])*(1.0/2);
	
			ptmp=RASWGL_AllocPrimitiveForPoint(ctx, tv, tv_min, tv_max);
//			ptmp=BGBRASW_AllocPrimitive(ctx->ractx);
			ptmp->type=BGBRASW_PTYPE_LINE;
			ptmp->tex=ctx->texture;

			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*3+0)*xyzsize, ptmp->xy[0], ptmp->z+0);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*2+1)*xyzsize, ptmp->xy[1], ptmp->z+1);

			ptmp->w[0]=(xyz+(base+i*2+0)*xyzsize)[3];
			ptmp->w[1]=(xyz+(base+i*2+1)*xyzsize)[3];

			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*2+0)*stsize, ptmp->st[0]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*2+1)*stsize, ptmp->st[1]);

//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*2+0)*rgbsize, ptmp->rgba+0);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*2+1)*rgbsize, ptmp->rgba+1);

			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*2+0)*xyzsize,
				rgb+(base+i*2+0)*rgbsize, ptmp->rgba+0);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*2+1)*xyzsize,
				rgb+(base+i*2+1)*rgbsize, ptmp->rgba+1);

			RASWGL_ClasifyPrimitive(ctx, ptmp);
			RASWGL_SubmitPrimitive(ctx, ptmp);
		}
	}else if(prim==GL_TRIANGLES)
	{
		plst=0; n=nxyz/3;
		for(i=0; i<n; i++)
		{
//			tv_min[0]=min(min(
//						xyz[(base+i*3+0)*xyzsize+0],
//						xyz[(base+i*3+1)*xyzsize+0]),
//						xyz[(base+i*3+2)*xyzsize+0]);
//			tv_max[0]=max(max(
//						xyz[(base+i*3+0)*xyzsize+0],
//						xyz[(base+i*3+1)*xyzsize+0]),
//						xyz[(base+i*3+2)*xyzsize+0]);
			tv_min[1]=min(min(
						xyz[(base+i*3+0)*xyzsize+1],
						xyz[(base+i*3+1)*xyzsize+1]),
						xyz[(base+i*3+2)*xyzsize+1]);
			tv_max[1]=max(max(
						xyz[(base+i*3+0)*xyzsize+1],
						xyz[(base+i*3+1)*xyzsize+1]),
						xyz[(base+i*3+2)*xyzsize+1]);
		
//			tv[0]=	(xyz[(base+i*3+0)*xyzsize+0]+
//					xyz[(base+i*3+1)*xyzsize+0]+
//					xyz[(base+i*3+2)*xyzsize+0])*(1.0/3);
			tv[1]=	(xyz[(base+i*3+0)*xyzsize+1]+
					xyz[(base+i*3+1)*xyzsize+1]+
					xyz[(base+i*3+2)*xyzsize+1])*(1.0/3);
	
			ptmp=RASWGL_AllocPrimitiveForPoint(ctx, tv, tv_min, tv_max);
//			ptmp=BGBRASW_AllocPrimitive(ctx->ractx);
			ptmp->type=BGBRASW_PTYPE_TRIANGLE;
			ptmp->tex=ctx->texture;
//			ptmp->tabs=ctx->tabs;

//			Vec4F_Copy()

			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*3+0)*xyzsize, ptmp->xy[0], ptmp->z+0);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*3+1)*xyzsize, ptmp->xy[1], ptmp->z+1);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*3+2)*xyzsize, ptmp->xy[2], ptmp->z+2);

			ptmp->w[0]=(xyz+(base+i*3+0)*xyzsize)[3];
			ptmp->w[1]=(xyz+(base+i*3+1)*xyzsize)[3];
			ptmp->w[2]=(xyz+(base+i*3+2)*xyzsize)[3];

			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*3+0)*stsize, ptmp->st[0]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*3+1)*stsize, ptmp->st[1]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*3+2)*stsize, ptmp->st[2]);

//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*3+0)*rgbsize, ptmp->rgba+0);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*3+1)*rgbsize, ptmp->rgba+1);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*3+2)*rgbsize, ptmp->rgba+2);

			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*3+0)*xyzsize,
				rgb+(base+i*3+0)*rgbsize, ptmp->rgba+0);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*3+1)*xyzsize,
				rgb+(base+i*3+1)*rgbsize, ptmp->rgba+1);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*3+2)*xyzsize,
				rgb+(base+i*3+2)*rgbsize, ptmp->rgba+2);

#if 0
			ptmp->bbox[0][0]=min(ptmp->xy[0][0],
				min(ptmp->xy[1][0], ptmp->xy[2][0]));
			ptmp->bbox[0][1]=min(ptmp->xy[0][1],
				min(ptmp->xy[1][1], ptmp->xy[2][1]));

			ptmp->bbox[1][0]=max(ptmp->xy[0][0],
				max(ptmp->xy[1][0], ptmp->xy[2][0]));
			ptmp->bbox[1][1]=max(ptmp->xy[0][1],
				max(ptmp->xy[1][1], ptmp->xy[2][1]));

			ptmp->bbox[0][2]=min(ptmp->z[0], min(ptmp->z[1], ptmp->z[2]));
			ptmp->bbox[1][2]=max(ptmp->z[0], max(ptmp->z[1], ptmp->z[2]));
#endif

			RASWGL_ClasifyPrimitive(ctx, ptmp);
			RASWGL_SubmitPrimitive(ctx, ptmp);
		}
	}else if(prim==GL_QUADS)
	{
		plst=0; n=nxyz/4;
		for(i=0; i<n; i++)
		{
			tv_min[1]=min(
				min(	xyz[(base+i*4+0)*xyzsize+1],
						xyz[(base+i*4+1)*xyzsize+1]),
				min(	xyz[(base+i*4+2)*xyzsize+1],
						xyz[(base+i*4+3)*xyzsize+1]));
			tv_max[1]=max(
				max(	xyz[(base+i*4+0)*xyzsize+1],
						xyz[(base+i*4+1)*xyzsize+1]),
				max(	xyz[(base+i*4+2)*xyzsize+1],
						xyz[(base+i*4+3)*xyzsize+1]));

//			tv[0]=	(xyz[(base+i*4+0)*xyzsize+0]+
//					 xyz[(base+i*4+1)*xyzsize+0]+
//					 xyz[(base+i*4+2)*xyzsize+0]+
//					 xyz[(base+i*4+3)*xyzsize+0])*(1.0/4);
			tv[1]=	(xyz[(base+i*4+0)*xyzsize+1]+
					 xyz[(base+i*4+1)*xyzsize+1]+
					 xyz[(base+i*4+2)*xyzsize+1]+
					 xyz[(base+i*4+3)*xyzsize+1])*(1.0/4);
	
			ptmp=RASWGL_AllocPrimitiveForPoint(ctx, tv, tv_min, tv_max);
//			ptmp=BGBRASW_AllocPrimitive(ctx->ractx);
			ptmp->type=BGBRASW_PTYPE_QUAD;
			ptmp->tex=ctx->texture;
//			ptmp->tabs=ctx->tabs;

			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*4+0)*xyzsize, ptmp->xy[0], ptmp->z+0);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*4+1)*xyzsize, ptmp->xy[1], ptmp->z+1);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*4+2)*xyzsize, ptmp->xy[2], ptmp->z+2);
			RASWGL_VertexToViewportPixels(ctx,
				xyz+(base+i*4+3)*xyzsize, ptmp->xy[3], ptmp->z+3);

			ptmp->w[0]=(xyz+(base+i*4+0)*xyzsize)[3];
			ptmp->w[1]=(xyz+(base+i*4+1)*xyzsize)[3];
			ptmp->w[2]=(xyz+(base+i*4+2)*xyzsize)[3];
			ptmp->w[3]=(xyz+(base+i*4+3)*xyzsize)[3];

			if((ptmp->xy[2][0]==ptmp->xy[3][0]) &&
				(ptmp->xy[2][1]==ptmp->xy[3][1]) &&
				(ptmp->z[2]==ptmp->z[3]))
			{
				ptmp->type=BGBRASW_PTYPE_TRIANGLE;
			}

			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*4+0)*stsize, ptmp->st[0]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*4+1)*stsize, ptmp->st[1]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*4+2)*stsize, ptmp->st[2]);
			RASWGL_CoordsToViewportTexels(ctx, ptmp->tex,
				st+(base+i*4+3)*stsize, ptmp->st[3]);

//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*4+0)*rgbsize, ptmp->rgba+0);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*4+1)*rgbsize, ptmp->rgba+1);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*4+2)*rgbsize, ptmp->rgba+2);
//			RASWGL_ColorsToViewportColors(ctx,
//				rgb+(base+i*4+3)*rgbsize, ptmp->rgba+3);

			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*4+0)*xyzsize,
				rgb+(base+i*4+0)*rgbsize, ptmp->rgba+0);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*4+1)*xyzsize,
				rgb+(base+i*4+1)*rgbsize, ptmp->rgba+1);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*4+2)*xyzsize,
				rgb+(base+i*4+2)*rgbsize, ptmp->rgba+2);
			RASWGL_ColorsToViewportColors2(ctx,
				xyz+(base+i*4+3)*xyzsize,
				rgb+(base+i*4+3)*rgbsize, ptmp->rgba+3);

			ptmp->bbox[0][0]=min(
				min(ptmp->xy[0][0], ptmp->xy[1][0]),
				min(ptmp->xy[2][0], ptmp->xy[3][0]));
			ptmp->bbox[0][1]=min(
				min(ptmp->xy[0][1], ptmp->xy[1][1]),
				min(ptmp->xy[2][1], ptmp->xy[3][1]));
			ptmp->bbox[0][0]=max(
				max(ptmp->xy[0][0], ptmp->xy[1][0]),
				max(ptmp->xy[2][0], ptmp->xy[3][0]));
			ptmp->bbox[0][1]=max(
				max(ptmp->xy[0][1], ptmp->xy[1][1]),
				max(ptmp->xy[2][1], ptmp->xy[3][1]));

			ptmp->bbox[0][2]=min(
				min(ptmp->z[0], ptmp->z[1]),
				min(ptmp->z[2], ptmp->z[3]));
			ptmp->bbox[0][2]=max(
				max(ptmp->z[0], ptmp->z[1]),
				max(ptmp->z[2], ptmp->z[3]));
			
			RASWGL_ClasifyPrimitive(ctx, ptmp);
			RASWGL_SubmitPrimitive(ctx, ptmp);
		}
	}
	
	if(ctx->ractx->pridraw)
	{
		plst=ctx->ractx->pridraw;
		ctx->ractx->pridraw=NULL;
		BGBRASW_DrawPrimitiveList(ctx->ractx, plst);
		BGBRASW_FreePrimitiveList(ctx->ractx, plst);
	}
}
